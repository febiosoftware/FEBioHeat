#pragma once
#include <FECore/FESurfaceLoad.h>
#include <FECore/FESurfaceMap.h>
#include <FECore/FELinearSystem.h>

//-----------------------------------------------------------------------------
//! Surface that sustains a convective heat flux boundary condition
//!
class FEConvectiveHeatFlux : public FESurfaceLoad
{
public:
	//! constructor
	FEConvectiveHeatFlux(FEModel* pfem);

	//! initialization
	bool Init() override;

	//! Set the surface to apply the load to
	void SetSurface(FESurface* ps) override;

	//! stiffness matrix (new interface)
	void StiffnessMatrix(FELinearSystem& LS, const FETimeInfo& tp);
	
	//! residual
	void LoadVector(FEGlobalVector& R, const FETimeInfo& tp) override;

private:
	double	m_hc;		//!< heat transfer coefficient
	double	m_Ta;		//!< ambient temperature

	FEDofList	m_dofT;

	DECLARE_FECORE_CLASS();
};
