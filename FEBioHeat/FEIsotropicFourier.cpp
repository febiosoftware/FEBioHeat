#include "FEIsotropicFourier.h"

BEGIN_FECORE_CLASS(FEIsotropicFourier, FEMaterial)
	ADD_PARAMETER(m_rho, FE_RANGE_GREATER(0.0), "density")->setUnits(UNIT_DENSITY);
	ADD_PARAMETER(m_k  , FE_RANGE_GREATER(0.0), "k")->setUnits("W/L.T");
	ADD_PARAMETER(m_c  , FE_RANGE_GREATER(0.0), "c")->setUnits("E/M.T");
END_FECORE_CLASS();

FEIsotropicFourier::FEIsotropicFourier(FEModel* pfem) : FEHeatTransferMaterial(pfem) 
{
	m_rho = 1;
	m_k = 0;
	m_c = 0;
}

mat3ds FEIsotropicFourier::Conductivity(FEMaterialPoint& mp)
{
	return mat3dd(m_k);
}
