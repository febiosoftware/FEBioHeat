#include "FEGapHeatFlux.h"
#include <FECore/FEModel.h>
#include <FECore/FEGlobalMatrix.h>
#include <FECore/FEClosestPointProjection.h>

FEGapHeatFluxSurface::FEGapHeatFluxSurface(FEModel* fem) : FESurface(fem)
{
	m_dofT = -1;
}

// one-time initialization
bool FEGapHeatFluxSurface::Init()
{
	// do base class first
	if (FESurface::Init() == false) return false;

	// allocate data structures
	const int NE = Elements();
	m_Data.resize(NE);
	for (int i = 0; i<NE; ++i)
	{
		FESurfaceElement& el = Element(i);
		int nint = el.GaussPoints();
		m_Data[i].resize(nint);
	}

	FEModel& fem = *GetFEModel();
	DOFS& dofs = fem.GetDOFS();
	m_dofT = dofs.GetDOF("T");

	return true;
}

void FEGapHeatFluxSurface::UnpackLM(FEElement& el, vector<int>& lm)
{
	int N = el.Nodes();
	lm.resize(N);
	for (int i = 0; i<N; ++i)
	{
		int n = el.m_node[i];
		FENode& node = m_pMesh->Node(n);
		vector<int>& id = node.m_ID;

		lm[i] = id[m_dofT];
	}
}

//=================================================================================================

BEGIN_FECORE_CLASS(FEGapHeatFlux, FESurfacePairConstraint)
	ADD_PARAMETER(m_hc  , "hc");
	ADD_PARAMETER(m_stol, "search_tol");
END_FECORE_CLASS();

FEGapHeatFlux::FEGapHeatFlux(FEModel* fem) : FESurfacePairConstraint(fem), m_ms(fem), m_ss(fem)
{
	m_hc = 0.0;
	m_stol = 0.01;
}

// one-time initialization
bool FEGapHeatFlux::Init()
{
	if (m_ms.Init() == false) return false;
	if (m_ss.Init() == false) return false;

	return true;
}

// activation
void FEGapHeatFlux::Activate()
{
	// base class first
	FESurfacePairConstraint::Activate();

	// project slave surface onto master surface
	ProjectSurface(m_ss, m_ms);
}

void FEGapHeatFlux::ProjectSurface(FEGapHeatFluxSurface& ss, FEGapHeatFluxSurface& ms)
{
	FEClosestPointProjection cpp(ms);
	cpp.HandleSpecialCases(true);
	cpp.SetTolerance(m_stol);
	cpp.Init();

	// loop over all slave elements
	for (int i = 0; i<ss.Elements(); ++i)
	{
		// get the slave element
		FESurfaceElement& se = ss.Element(i);
		int nn = se.Nodes();

		// get nodal coordinates
		vec3d re[FEElement::MAX_NODES];
		for (int l = 0; l<nn; ++l) re[l] = ss.GetMesh()->Node(se.m_node[l]).m_rt;

		// loop over all its integration points
		int nint = se.GaussPoints();
		for (int j = 0; j<nint; ++j)
		{
			// get the integration point data
			FEGapHeatFluxSurface::Data& pt = ss.m_Data[i][j];

			// calculate the global coordinates of this integration point
			double* H = se.H(j);

			vec3d x(0, 0, 0), q;
			for (int k = 0; k<nn; ++k) x += re[k] * H[k];

			// find the master segment this element belongs to
			pt.m_rs = vec2d(0, 0);
			FESurfaceElement* pme = cpp.Project(x, q, pt.m_rs);
			pt.m_pme = pme;

			// update normal and gap at integration point
			if (pt.m_pme)
			{
				double r = pt.m_rs[0];
				double s = pt.m_rs[1];

				// the slave normal is set to the master element normal
				pt.m_nu = ms.SurfaceNormal(*pt.m_pme, r, s);
			}
		}
	}
}

// Build the matrix profile
void FEGapHeatFlux::BuildMatrixProfile(FEGlobalMatrix& K)
{
	FEModel& fem = *GetFEModel();
	FEMesh& mesh = fem.GetMesh();

	// get the DOFS
	const int dof_T = fem.GetDOFIndex("T");

	vector<int> lm(FEElement::MAX_NODES);

	FEGapHeatFluxSurface& ss = m_ss;
	FEGapHeatFluxSurface& ms = m_ms;

	for (int j = 0; j<ss.Elements(); ++j)
	{
		FESurfaceElement& se = ss.Element(j);
		int nint = se.GaussPoints();
		int* sn = &se.m_node[0];
		for (int k = 0; k<nint; ++k)
		{
			FESurfaceElement* pe = ss.m_Data[j][k].m_pme;
			if (pe != 0)
			{
				FESurfaceElement& me = *pe;
				int* mn = &me.m_node[0];

				lm.assign(lm.size(), -1);

				int nseln = se.Nodes();
				int nmeln = me.Nodes();

				for (int l = 0; l<nseln; ++l)
				{
					vector<int>& id = mesh.Node(sn[l]).m_ID;
					lm[l] = id[dof_T];
				}

				for (int l = 0; l<nmeln; ++l)
				{
					vector<int>& id = mesh.Node(mn[l]).m_ID;
					lm[l + nseln] = id[dof_T];
				}

				K.build_add(lm);
			}
		}
	}
}

// stiffness matrix
void FEGapHeatFlux::StiffnessMatrix(FELinearSystem& LS, const FETimeInfo& tp)
{
	vector<int> sLM, mLM, LM, en;
	matrix ke;

	// shape functions
	double Hm[FEElement::MAX_NODES];

	// loop over all slave elements
	const int NE = m_ss.Elements();
	for (int i = 0; i<NE; ++i)
	{
		// get the next element
		FESurfaceElement& se = m_ss.Element(i);
		int nseln = se.Nodes();

		// get the element's LM vector
		m_ss.UnpackLM(se, sLM);

		// integration weights
		double* w = se.GaussWeights();

		// loop over all integration points
		const int nint = se.GaussPoints();
		for (int n = 0; n<nint; ++n)
		{
			// get intgration point data
			FEGapHeatFluxSurface::Data& pt = m_ss.m_Data[i][n];

			// get the master element
			FESurfaceElement* pme = pt.m_pme;
			if (pme)
			{
				// get the master element
				FESurfaceElement& me = *pme;
				int nmeln = me.Nodes();
				m_ms.UnpackLM(me, mLM);

				// calculate jacobian
				double detJ = m_ss.jac0(se, n);

				// slave shape functions
				double* Hs = se.H(n);

				// master shape functions
				double r = pt.m_rs[0];
				double s = pt.m_rs[1];
				me.shape_fnc(Hm, r, s);

				// calculate degrees of freedom
				int ndof = nseln + nmeln;

				// create the stiffness matrix
				ke.resize(ndof, ndof);
				ke.zero();
				for (int k = 0; k<nseln; ++k)
				{
					for (int l = 0; l<nseln; ++l)
					{
						ke[k][l] = Hs[k] * Hs[l];
					}
				}

				for (int k = 0; k<nseln; ++k)
				{
					for (int l = 0; l<nmeln; ++l)
					{
						ke[k][l + nseln] = -Hs[k] * Hm[l];
						ke[l + nseln][k] = -Hs[k] * Hm[l];
					}
				}

				for (int k = 0; k<nmeln; ++k)
					for (int l = 0; l<nmeln; ++l)
					{
						ke[k + nseln][l + nseln] = Hm[k] * Hm[l];
					}

				ke *= m_hc*detJ*w[n];

				// build the LM vector
				LM.resize(ndof);
				for (int k = 0; k<nseln; ++k)
				{
					LM[k] = sLM[k];
				}
				for (int k = 0; k<nmeln; ++k)
				{
					LM[k + nseln] = mLM[k];
				}

				// assemble the global stiffness matrix
				LS.AssembleLHS(LM, ke);
			}
		}
	}
}
