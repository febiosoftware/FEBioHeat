#include "FEHeatBC.h"
#include <FECore/FEModel.h>
#include <FECore/DOFS.h>

//=======================================================================================
FEFixedTemperature::FEFixedTemperature(FEModel* fem) : FEFixedBC(fem)
{

}

bool FEFixedTemperature::Init()
{
	FEModel* fem = GetFEModel();
	DOFS& dofs = fem->GetDOFS();
	m_dofs.clear();
	m_dofs.push_back(dofs.GetDOF("T"));
	return FEFixedBC::Init();
}

//=======================================================================================
// NOTE: I'm setting FEBoundaryCondition is the base class since I don't want to pull
//       in the parameters of FEPrescribedDOF. 
BEGIN_FECORE_CLASS(FEPrescribedTemperature, FEBoundaryCondition)
	ADD_PARAMETER(m_scale, "value")->setUnits(UNIT_TEMPERATURE);
	ADD_PARAMETER(m_brelative, "relative");
	
	ADD_PROPERTY(m_nodeSet, "node_set");
END_FECORE_CLASS();

FEPrescribedTemperature::FEPrescribedTemperature(FEModel* fem) : FEPrescribedDOF(fem)
{

}

bool FEPrescribedTemperature::Init()
{
	FEModel* fem = GetFEModel();
	int Tdof = fem->GetDOFIndex("T");
	SetDOF(Tdof);
	return FEPrescribedDOF::Init();
}
