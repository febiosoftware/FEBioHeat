#pragma once

#include "FECore/FELinearSolver.h"
#include "FEHeatSolidDomain.h"

//-----------------------------------------------------------------------------
//! The FEHeatSolver solves heat-conduction problems
//! 

class FEHeatSolver : public FELinearSolver
{
public:
	//! constructor
	FEHeatSolver(FEModel* pfem);

	//! destructor
	~FEHeatSolver();

	//! one-time initialization
	bool Init() override;

protected: // from FELinearSolver

	//! calculate the stiffness matrix
	bool StiffnessMatrix(FELinearSystem& LS) override; 

protected:
	// declare the parameter list
	DECLARE_FECORE_CLASS();
};
