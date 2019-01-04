#include "FEHeatSource.h"
#include "FEHeatSolidDomain.h"

//-----------------------------------------------------------------------------
FEHeatSource::FEHeatSource(FEModel* pfem) : FEBodyLoad(pfem)
{
}


//-----------------------------------------------------------------------------
void FEHeatSource::Residual(const FETimeInfo& tp, FEGlobalVector& R)
{
	for (int j = 0; j < Domains(); ++j)
	{
		FEHeatDomain& dom = dynamic_cast<FEHeatDomain&>(*Domain(j));
		dom.HeatSource(R, *this);
	}
}

//-----------------------------------------------------------------------------
BEGIN_FECORE_CLASS(FEConstHeatSource, FEHeatSource);
	ADD_PARAMETER(m_Q, "Q");
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
FEConstHeatSource::FEConstHeatSource(FEModel* pfem) : FEHeatSource(pfem)
{
	m_Q = 0;
}
