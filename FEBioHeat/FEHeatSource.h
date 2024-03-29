#pragma once
#include "FECore/FEParameterList.h"
#include "FECore/FEGlobalVector.h"
#include "FECore/FEBodyLoad.h"

//-----------------------------------------------------------------------------
// Forward declaration of FEModel class
class FEModel;

//-----------------------------------------------------------------------------
// Base class for heat sources
class FEHeatSource : public FEBodyLoad
{
public:
	//! constructor
	FEHeatSource(FEModel* pfem);

	void LoadVector(FEGlobalVector& R) override;

public:
	virtual double value(FEMaterialPoint& mp) = 0;
};

//-----------------------------------------------------------------------------
class FEConstHeatSource : public FEHeatSource
{
public:
	FEConstHeatSource(FEModel* fem);

	double value(FEMaterialPoint& mp) override { return m_Q; }

private:
	double	m_Q;	// source value

	DECLARE_FECORE_CLASS();
};

//-----------------------------------------------------------------------------
class FEBioHeatSource : public FEHeatSource
{
public:
	FEBioHeatSource(FEModel* fem);

	double value(FEMaterialPoint& mp) override;

	void StiffnessMatrix(FELinearSystem& LS) override;

protected:
	double	m_Ta;	// arterial temperature
	double	m_W;	// tissue-blood perfusion rate
	double	m_cb;	// blood-specific heat

	DECLARE_FECORE_CLASS();
};
