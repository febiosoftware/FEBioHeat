#pragma once
#include <FECore/FEFixedBC.h>
#include <FECore/FEPrescribedDOF.h>

class FEFixedTemperature : public FEFixedBC
{
public:
	FEFixedTemperature(FEModel* fem);
	bool Init() override;
};

class FEPrescribedTemperature : public FEPrescribedDOF
{
public:
	FEPrescribedTemperature(FEModel* fem);
	bool Init() override;

	DECLARE_FECORE_CLASS();
};
