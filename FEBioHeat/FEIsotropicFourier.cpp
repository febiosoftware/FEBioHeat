#include "FEIsotropicFourier.h"

//-----------------------------------------------------------------------------
// define the parameter list
BEGIN_FECORE_CLASS(FEIsotropicFourier, FEMaterial)
	ADD_PARAMETER(m_rho, FE_RANGE_GREATER(0.0), "density")->setUnits(UNIT_DENSITY);
	ADD_PARAMETER(m_k  , FE_RANGE_GREATER(0.0), "k")->setUnits("W/L.T");
	ADD_PARAMETER(m_c  , FE_RANGE_GREATER(0.0), "c")->setUnits("E/M.T");
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
mat3ds FEIsotropicFourier::Conductivity(FEMaterialPoint& mp)
{
	return mat3dd(m_k);
}
