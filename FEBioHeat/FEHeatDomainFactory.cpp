#include "FEHeatDomainFactory.h"
#include "FEHeatTransferMaterial.h"
#include "FEHeatSolidDomain.h"

//-----------------------------------------------------------------------------
//! Use the material and the element type to determine the domain type.
FEDomain* FEHeatDomainFactory::CreateDomain(const FE_Element_Spec& spec, FEMesh* pm, FEMaterial* pmat)
{
	FEModel& fem = *pmat->GetFEModel();
	FE_Element_Class eclass = spec.eclass;
	const char* sztype = 0;
	if (dynamic_cast<FEHeatTransferMaterial*>(pmat))
	{
		if (eclass == FE_ELEM_SOLID) sztype = "heat-solid";
	}

	if (sztype)
	{
		FEDomain* pd = fecore_new<FESolidDomain>(sztype, &fem);
		if (pd) pd->SetMaterial(pmat);
		return pd;
	}
	else return 0;
}
