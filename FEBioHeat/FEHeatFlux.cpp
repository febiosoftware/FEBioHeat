#include "FEHeatFlux.h"
#include "FECore/FEModel.h"

//-----------------------------------------------------------------------------
BEGIN_FECORE_CLASS(FEHeatFlux, FESurfaceLoad)
	ADD_PARAMETER(m_flux, "flux" );
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
FEHeatFlux::FEHeatFlux(FEModel* pfem) : FESurfaceLoad(pfem), m_dofT(pfem)
{
	m_flux = 1.0;
}

//-----------------------------------------------------------------------------
bool FEHeatFlux::Init()
{
	m_dofT.Clear();
	m_dofT.AddDof(GetFEModel()->GetDOFIndex("T"));
	return FESurfaceLoad::Init();
}

//-----------------------------------------------------------------------------
//! allocate storage
void FEHeatFlux::SetSurface(FESurface* psurf)
{ 
	FESurfaceLoad::SetSurface(psurf);
	m_flux.SetItemList(psurf->GetFacetSet());
}

//-----------------------------------------------------------------------------
//! Calculate the heat flux residual
void FEHeatFlux::LoadVector(FEGlobalVector& R, const FETimeInfo& tp)
{
	m_psurf->LoadVector(R, m_dofT, false, [=](FESurfaceMaterialPoint& mp, int node_a, vector<double>& fa) {
		
		// heat flux
		double q = m_flux(mp);

		// Jacobian
		double J = (mp.dxr ^ mp.dxs).norm();

		// evaluate integrand
		double* H = mp.m_shape;
		fa[0] = H[node_a] * q*J;
	});
}
