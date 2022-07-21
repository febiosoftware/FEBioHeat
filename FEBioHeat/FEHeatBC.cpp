#include "FEHeatBC.h"
#include <FECore/FEModel.h>
#include <FECore/DOFS.h>

//=======================================================================================
FEFixedTemperature::FEFixedTemperature(FEModel* fem) : FEFixedBC(fem)
{

}

bool FEFixedTemperature::Init()
{
	SetDOFList(GetDOFIndex("T"));
	return FEFixedBC::Init();
}

//=======================================================================================
// NOTE: I'm setting FEBoundaryCondition is the base class since I don't want to pull
//       in the parameters of FEPrescribedDOF. 
BEGIN_FECORE_CLASS(FEPrescribedTemperature, FEBoundaryCondition)
	ADD_PARAMETER(m_scale, "value")->setUnits(UNIT_TEMPERATURE);
	ADD_PARAMETER(m_brelative, "relative");
END_FECORE_CLASS();

FEPrescribedTemperature::FEPrescribedTemperature(FEModel* fem) : FEPrescribedDOF(fem)
{

}

bool FEPrescribedTemperature::Init()
{
	SetDOF(GetDOFIndex("T"));
	return FEPrescribedDOF::Init();
}
