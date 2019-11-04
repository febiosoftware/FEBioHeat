#pragma once
#include <FECore/FESurfacePairConstraint.h>
#include <FECore/FESurface.h>
#include <FECore/FELinearSystem.h>

class FEGapHeatFluxSurface : public FESurface
{
public:
	class Data
	{
	public:
		FESurfaceElement*	m_pme;	// master element that connects to this slave integration point
		vec2d				m_rs;	// iso-parametric coordinates of projection onto master element
		vec3d				m_nu;	// normal at master element
	};

public:
	FEGapHeatFluxSurface(FEModel* fem);

	void UnpackLM(FEElement& el, vector<int>& lm);

	// one-time initialization
	bool Init() override;

public:
	vector< vector<Data> >	m_Data;
	
private:
	int			m_dofT;
};

// Class that implements a contact or gap heat flux
class FEGapHeatFlux : public FESurfacePairConstraint
{
public:
	FEGapHeatFlux(FEModel* fem);

	// one-time initialization
	bool Init() override;

	// activation
	void Activate() override;

	// stiffness matrix
	void StiffnessMatrix(FELinearSystem& LS, const FETimeInfo& tp);

// from FESurfacePairConstraint
public:
	//! return the master surface
	FESurface* GetMasterSurface() override { return &m_ms; }

	//! return the slave surface
	FESurface* GetSlaveSurface() override { return &m_ss; }

	//! temporary construct to determine if contact interface uses nodal integration rule (or facet)
	bool UseNodalIntegration() override { return false; }

	// Build the matrix profile
	void BuildMatrixProfile(FEGlobalMatrix& M) override;

private:
	void ProjectSurface(FEGapHeatFluxSurface& ss, FEGapHeatFluxSurface& ms);

private:
	double		m_stol;		// gap search tolerance
	double		m_hc;		// heat transfer coefficient

private:
	FEGapHeatFluxSurface	m_ms;
	FEGapHeatFluxSurface	m_ss;

	DECLARE_FECORE_CLASS();
};
