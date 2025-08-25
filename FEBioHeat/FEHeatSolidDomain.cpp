#include "FEHeatSolidDomain.h"
#include <FECore/FEModel.h>
#include <FECore/Integrate.h>
using namespace std::placeholders;

//-----------------------------------------------------------------------------
//! constructor
FEHeatSolidDomain::FEHeatSolidDomain(FEModel* pfem) : FESolidDomain(pfem), m_dof(pfem)
{
	m_pMat = 0;

	// list the degrees of freedom
	m_dof.AddDof(pfem->GetDOFIndex("T"));
}

//-----------------------------------------------------------------------------
void FEHeatSolidDomain::SetMaterial(FEMaterial* pmat)
{
	m_pMat = dynamic_cast<FEHeatTransferMaterial*>(pmat);
	assert(m_pMat);
}

//-----------------------------------------------------------------------------
// Calculate the heat conduction matrix
void FEHeatSolidDomain::ConductionMatrix(FELinearSystem& ls)
{
	auto fp = bind(&FEHeatSolidDomain::ElementConduction, this, _1, _2);
	AssembleSolidDomain(*this, ls, fp);
}

//-----------------------------------------------------------------------------
// Calculate the capacitance matrix
void FEHeatSolidDomain::CapacitanceMatrix(FELinearSystem& ls)
{
	auto fp = bind(&FEHeatSolidDomain::ElementCapacitance, this, _1, _2);
	AssembleSolidDomain(*this, ls, fp);
}

//-----------------------------------------------------------------------------
void FEHeatSolidDomain::HeatSource(FEGlobalVector& R, FEHeatSource& hs)
{
	vector<double> fe;
	vector<int> lm;
	int NE = Elements();
	for (int i = 0; i<NE; ++i)
	{
		FESolidElement& el = Element(i);
		int ne = el.Nodes();
		fe.resize(ne);
		ElementHeatSource(hs, el, fe);
		UnpackLM(el, lm);
		R.Assemble(el.m_node, lm, fe);
	}
}

void FEHeatSolidDomain::CapacitanceLoad(FEGlobalVector& R)
{
	auto fp = bind(&FEHeatSolidDomain::ElementCapacitanceLoad, this, _1, _2);
	AssembleSolidDomain(*this, R, fp);
}

void FEHeatSolidDomain::ElementCapacitanceLoad(FESolidElement& el, vector<double>& fe)
{
	FETimeInfo& ti = GetFEModel()->GetTime();
	double dt = ti.timeIncrement;

	double c = m_pMat->Capacitance();
	double d = m_pMat->Density();
	double alpha = c * d / dt;

	zero(fe);
	double* w = el.GaussWeights();
	int ne = el.Nodes();
	int ni = el.GaussPoints();
	for (int n = 0; n < ni; ++n)
	{
		FEMaterialPoint& mp = *el.GetMaterialPoint(n);
		FEHeatMaterialPoint* pt = (mp.ExtractData<FEHeatMaterialPoint>());

		double Ti = pt->m_T;
		double* H = el.H(n);
		double J = detJt(el, n);
		for (int i = 0; i < ne; ++i)
		{
			fe[i] += alpha * H[i] * Ti * J * w[n];
		}
	}
}

//-----------------------------------------------------------------------------
//! calculate element contribution to heat source term
void FEHeatSolidDomain::ElementHeatSource(FEHeatSource& hs, FESolidElement& el, vector<double>& fe)
{
	zero(fe);
	double* w = el.GaussWeights();
	int ne = el.Nodes();
	int ni = el.GaussPoints();
	for (int n = 0; n<ni; ++n)
	{
		FEMaterialPoint& mp = *el.GetMaterialPoint(n);
		double* H = el.H(n);
		double J = detJt(el, n);
		for (int i = 0; i<ne; ++i)
		{
			double Q = hs.value(mp);
			fe[i] += Q*H[i] * J*w[n];
		}
	}
}

//-----------------------------------------------------------------------------
//! This function calculates the element stiffness matrix for a particular
//! element.
//!
void FEHeatSolidDomain::ElementConduction(FESolidElement& el, matrix& ke)
{
	// zero the matrix
	ke.zero();

	// do the element integration
	IntegrateBDB(*this, el, [=](const FEMaterialPoint& mp) {
		return m_pMat->Conductivity(const_cast<FEMaterialPoint&>(mp));
	}, ke);
}

//-----------------------------------------------------------------------------
void FEHeatSolidDomain::ElementCapacitance(FESolidElement &el, matrix &ke)
{
	FETimeInfo& ti = GetFEModel()->GetTime();
	double dt = ti.timeIncrement;

	// zero stiffness matrix
	ke.zero();

	// calculate data
	double c = m_pMat->Capacitance();
	double d = m_pMat->Density();
	double alpha = c*d/dt;

	// do the element integration
	IntegrateNCN(*this, el, alpha, ke);
}

//-----------------------------------------------------------------------------
// This function updates the domain state data. 
// This function is called during FESolver::Update after the nodal variables are udpated.
void FEHeatSolidDomain::Update(const FETimeInfo& tp)
{
	FEMesh& mesh = *GetMesh();
	int NE = Elements();
	const int dof_T = m_dof[DOF_T];
	for (int j=0; j<NE; ++j)
	{
		FESolidElement& el = Element(j);
		int ni = el.GaussPoints();
		int ne = el.Nodes();

		// get the nodal temperatures
		double T[FEElement::MAX_NODES];
		for (int n=0; n<ne; ++n) T[n] = mesh.Node(el.m_node[n]).get(dof_T);

		// calculate heat flux for each integration point
		for (int n=0; n<ni; ++n)
		{
			FEMaterialPoint& mp = *el.GetMaterialPoint(n);
			FEHeatMaterialPoint* pt = (mp.ExtractData<FEHeatMaterialPoint>());
			assert(pt);

			pt->m_T = el.Evaluate(T, n);

			vec3d gradT = gradient(el, T, n);
			mat3ds D = m_pMat->Conductivity(mp);
			pt->m_q = -(D*gradT);
		}
	}
}
