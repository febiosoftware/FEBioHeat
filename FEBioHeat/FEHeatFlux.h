#pragma once
#include <FECore/FESurfaceLoad.h>
#include <FECore/FEModelParam.h>

//-----------------------------------------------------------------------------
//! Surface that sustains a heat flux boundary condition
//!
class FEHeatFlux : public FESurfaceLoad
{
public:
	//! constructor
	FEHeatFlux(FEModel* pfem);

	//! initialization
	bool Init() override;

	//! Set the surface to apply the load to
	void SetSurface(FESurface* ps) override;

	//! residual
	void LoadVector(FEGlobalVector& R, const FETimeInfo& tp) override;

protected:
	FEParamDouble	m_flux;	//!< heat flux

	FEDofList	m_dofT;

	DECLARE_FECORE_CLASS();
};
