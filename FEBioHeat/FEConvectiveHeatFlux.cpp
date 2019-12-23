#include "FEConvectiveHeatFlux.h"
#include "FECore/FEModel.h"

//-----------------------------------------------------------------------------
BEGIN_FECORE_CLASS(FEConvectiveHeatFlux, FESurfaceLoad)
	ADD_PARAMETER(m_hc, "hc");
	ADD_PARAMETER(m_Ta, "Ta");
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
FEConvectiveHeatFlux::FEConvectiveHeatFlux(FEModel* pfem) : FESurfaceLoad(pfem), m_dofT(pfem)
{
	m_hc = 0;
	m_Ta = 1.0;
}

//-----------------------------------------------------------------------------
void FEConvectiveHeatFlux::SetSurface(FESurface* psurf)
{
	FESurfaceLoad::SetSurface(psurf);
}

//-----------------------------------------------------------------------------
//! initialization
bool FEConvectiveHeatFlux::Init()
{
	m_dofT.Clear();
	if (m_dofT.AddDof(GetFEModel()->GetDOFIndex("T")) == false) return false;
	return FESurfaceLoad::Init();
}

//-----------------------------------------------------------------------------
//! residual
void FEConvectiveHeatFlux::LoadVector(FEGlobalVector& R, const FETimeInfo& tp)
{
	m_psurf->LoadVector(R, m_dofT, false, [&](FESurfaceMaterialPoint& mp, const FESurfaceDofShape& node_a, vector<double>& fa) {

		// evaluate flux
		double q = m_Ta;

		// Jacobian
		double J = (mp.dxr ^ mp.dxs).norm();

		// evaluate nodal value
		double H = node_a.shape;
		fa[0] = H * q*J;
	});
}

//-----------------------------------------------------------------------------
//! stiffness matrix
void FEConvectiveHeatFlux::StiffnessMatrix(FELinearSystem& LS, const FETimeInfo& tp)
{
	m_psurf->LoadStiffness(LS, m_dofT, m_dofT, [=](FESurfaceMaterialPoint& pt, const FESurfaceDofShape& node_a, const FESurfaceDofShape& node_b, matrix& Kab) {

		double J = (pt.dxr ^ pt.dxs).norm();
		double H_a = node_a.shape;
		double H_b = node_b.shape;
		double kij = m_hc*H_a * H_b * J;
		Kab[0][0] = kij;
	});
}
