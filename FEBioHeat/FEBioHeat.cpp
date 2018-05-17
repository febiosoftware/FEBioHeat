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
	REGISTER_FECORE_CLASS(FEHeatSolidDomain, FEDOMAIN_ID, "heat-solid");

	// feature classes
	REGISTER_FECORE_CLASS(FEHeatSolver        , FESOLVER_ID                , "heat");
	REGISTER_FECORE_CLASS(FEIsotropicFourier  , FEMATERIAL_ID              , "isotropic Fourier");
	REGISTER_FECORE_CLASS(FEConstHeatSource   , FEBODYLOAD_ID              , "heat_source");
	REGISTER_FECORE_CLASS(FEHeatFlux          , FESURFACELOAD_ID           , "heatflux");
	REGISTER_FECORE_CLASS(FEConvectiveHeatFlux, FESURFACELOAD_ID           , "convective_heatflux");
	REGISTER_FECORE_CLASS(FEPlotHeatFlux      , FEPLOTDATA_ID              , "heat flux");
	REGISTER_FECORE_CLASS(FEGapHeatFlux       , FESURFACEPAIRINTERACTION_ID, "gap heat flux");

	fecore.SetActiveModule(0);
}

FECORE_PLUGIN int GetSDKVersion()
{
	return FE_SDK_VERSION;
}
