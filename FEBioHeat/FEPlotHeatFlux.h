#include <FECore/FEPlotData.h>

//-----------------------------------------------------------------------------
class FEPlotTemperature : public FEPlotNodeData
{
public:
	FEPlotTemperature(FEModel* fem) : FEPlotNodeData(fem, PLT_FLOAT, FMT_ITEM) {}
	bool Save(FEMesh& mesh, FEDataStream& a);
};

//-----------------------------------------------------------------------------
//! Class the outputs the heat flux
class FEPlotHeatFlux : public FEPlotDomainData
{
public:
	FEPlotHeatFlux(FEModel* pfem) : FEPlotDomainData(pfem, PLT_VEC3F, FMT_ITEM){}
	bool Save(FEDomain& dom, FEDataStream& a);
};
