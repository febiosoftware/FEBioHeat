#include "FEPlotHeatFlux.h"
#include "FEHeatSolidDomain.h"
#include "FEHeatTransferMaterial.h"
#include <FECore/writeplot.h>

//-----------------------------------------------------------------------------
bool FEPlotHeatFlux::Save(FEDomain &dom, FEDataStream& a)
{
	FEHeatSolidDomain* pbd = dynamic_cast<FEHeatSolidDomain*>(&dom);
	if (pbd)
	{
		writeAverageElementValue<vec3d>(dom, a, [](const FEMaterialPoint& mp) {
			const FEHeatMaterialPoint* pt = (mp.ExtractData<FEHeatMaterialPoint>());
			return pt->m_q;
		});
	}
	return false;
}
