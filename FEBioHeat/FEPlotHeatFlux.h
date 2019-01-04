#include <FECore/FEPlotData.h>

//-----------------------------------------------------------------------------
//! Class the outputs the heat flux
class FEPlotHeatFlux : public FEPlotDomainData
{
public:
	FEPlotHeatFlux(FEModel* pfem) : FEPlotDomainData(pfem, PLT_VEC3F, FMT_ITEM){}
	bool Save(FEDomain& dom, FEDataStream& a);
};
