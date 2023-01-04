#include "FEHeatAnalysis.h"


BEGIN_FECORE_CLASS(FEHeatAnalysis, FEAnalysis)
	// The analysis parameter is already defined in the FEAnalysis base class. 
	// Here, we just need to set the enum values for the analysis parameter.
	FindParameterFromData(&m_nanalysis)->setEnums("STEADY-STATE\0TRANSIENT\0");
END_FECORE_CLASS()

FEHeatAnalysis::FEHeatAnalysis(FEModel* fem) : FEAnalysis(fem)
{

}
