#include <FECore/sdk.h>
#include "FEIsotropicFourier.h"
#include "FEPlotHeatFlux.h"
#include "FEHeatFlux.h"
#include "FEConvectiveHeatFlux.h"
#include "FEHeatSolver.h"
#include "FEHeatSolidDomain.h"
#include "FEHeatDomainFactory.h"
#include "FEHeatSource.h"
#include "FEHeatSolidDomain.h"
#include "FEGapHeatFlux.h"

FECORE_PLUGIN void PluginInitialize(FECoreKernel& fecore)
{
	FECoreKernel::SetInstance(&fecore);

	// create the heat-transfer module
	fecore.CreateModule("heat");

	// Domain factory
	fecore.RegisterDomain(new FEHeatDomainFactory);
	REGISTER_FECORE_CLASS(FEHeatSolidDomain, "heat-solid");

	// feature classes
	REGISTER_FECORE_CLASS(FEHeatSolver        , "heat");
	REGISTER_FECORE_CLASS(FEIsotropicFourier  , "isotropic Fourier");
	REGISTER_FECORE_CLASS(FEConstHeatSource   , "heat_source");
	REGISTER_FECORE_CLASS(FEHeatFlux          , "heatflux");
	REGISTER_FECORE_CLASS(FEConvectiveHeatFlux, "convective_heatflux");
	REGISTER_FECORE_CLASS(FEPlotHeatFlux      , "heat flux");
	REGISTER_FECORE_CLASS(FEGapHeatFlux       , "gap heat flux");

	fecore.SetActiveModule(0);
}

FECORE_PLUGIN int GetSDKVersion()
{
	return FE_SDK_VERSION;
}
