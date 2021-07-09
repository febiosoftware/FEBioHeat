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
#include "FEHeatBC.h"


FECORE_PLUGIN int GetSDKVersion()
{
	return FE_SDK_VERSION;
}

FECORE_PLUGIN void GetPluginVersion(int& major, int& minor, int& patch)
{
	major = 1;
	minor = 0;
	patch = 0;
}

FECORE_PLUGIN void PluginInitialize(FECoreKernel& fecore)
{
	FECoreKernel::SetInstance(&fecore);

	// create the heat-transfer module
	const char* info = \
		"{ "
		"   \"title\" : \"Heat Transfer\","
		"   \"info\"  : \"Transient or steady-state heat conduction analysis.\","
		"   \"author\": \"Steve Maas\","
		"   \"version\": \"1.0\""
        "}";

	fecore.CreateModule("heat", info);

	// Domain factory
	fecore.RegisterDomain(new FEHeatDomainFactory);
	REGISTER_FECORE_CLASS(FEHeatSolidDomain, "heat-solid");

	// feature classes
	REGISTER_FECORE_CLASS(FEHeatSolver        , "heat");
	REGISTER_FECORE_CLASS(FEIsotropicFourier  , "isotropic Fourier");
	REGISTER_FECORE_CLASS(FEConstHeatSource   , "heat_source");
	REGISTER_FECORE_CLASS(FEBioHeatSource     , "bioheat");
	REGISTER_FECORE_CLASS(FEHeatFlux          , "heatflux");
	REGISTER_FECORE_CLASS(FEConvectiveHeatFlux, "convective_heatflux");
	REGISTER_FECORE_CLASS(FEGapHeatFlux       , "gap heat flux");
	REGISTER_FECORE_CLASS(FEFixedTemperature  , "fixed temperature");
	REGISTER_FECORE_CLASS(FEPrescribedTemperature, "prescribed temperature");
	REGISTER_FECORE_CLASS(FEPlotHeatFlux      , "heat flux");
	REGISTER_FECORE_CLASS(FEPlotTemperature   , "temperature");

	fecore.SetActiveModule(0);
}
