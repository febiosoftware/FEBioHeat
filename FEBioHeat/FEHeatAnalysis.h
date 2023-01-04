#pragma once
#include <FECore/FEAnalysis.h>

class FEHeatAnalysis : public FEAnalysis
{
public:
	enum HeatAnalysisType {
		STEADY_STATE,
		TRANSIENT
	};

public:
	FEHeatAnalysis(FEModel* fem);
	DECLARE_FECORE_CLASS();
};
