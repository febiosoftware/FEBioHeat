#include "FEPlotHeatFlux.h"
#include "FEHeatSolidDomain.h"
#include "FEHeatTransferMaterial.h"
#include <FECore/writeplot.h>
#include <FECore/FEModel.h>

//-----------------------------------------------------------------------------
bool FEPlotTemperature::Save(FEMesh& mesh, FEDataStream& a)
{
	int dof_T = GetFEModel()->GetDOFIndex("T");
	if (dof_T < 0) return false;

	for (int i = 0; i < mesh.Nodes(); ++i)
	{
		FENode& node = mesh.Node(i);
		double T = node.get(dof_T);
		a << T;
	}
	return true;
}

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
		return true;
	}
	return false;
}
