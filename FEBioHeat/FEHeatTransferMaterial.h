#pragma once
#include "FECore/FEMaterial.h"

//-----------------------------------------------------------------------------
// Material point class for heat transfer materials.
class FEHeatMaterialPoint : public FEMaterialPointData
{
public:
	FEHeatMaterialPoint(FEMaterialPointData* mp) : FEMaterialPointData(mp)
	{
		m_T = m_T0 = 0.0;
		m_q = vec3d(0,0,0);
	}

	FEMaterialPointData* Copy()
	{
		FEHeatMaterialPoint* pt = new FEHeatMaterialPoint(*this);
		if (m_pNext) pt->m_pNext = m_pNext->Copy();
		return pt;
	}

	void Serialize(DumpStream& ar)
	{
		ar & m_q & m_T & m_T0;
		FEMaterialPointData::Serialize(ar);
	}

public:
	double	m_T;	//!< current temperature
	double	m_T0;	//!< reference temperature
	vec3d	m_q;	//!< heat flux
};

//-----------------------------------------------------------------------------
// Base class for heat-transfer problems
class FEHeatTransferMaterial : public FEMaterial
{
public:
	//! constructor
	FEHeatTransferMaterial(FEModel* pfem) : FEMaterial(pfem) {}

	//! create material point data
	FEMaterialPointData* CreateMaterialPointData() override { return new FEHeatMaterialPoint(0); }

	//! get the material's conductivity
	virtual mat3ds Conductivity(FEMaterialPoint& mp) = 0;

	//! get the material's capacitance
	virtual double Capacitance() = 0;

	//! get the material's density
	virtual double Density() = 0;
};
