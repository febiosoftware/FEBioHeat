#include "FEHeatSolver.h"
#include "FEHeatFlux.h"
#include "FEConvectiveHeatFlux.h"
#include "FEGapHeatFlux.h"
#include "FEHeatTransferMaterial.h"
#include "FEHeatSource.h"
#include <FECore/FEModel.h>
#include <FECore/FEAnalysis.h>
#include <FECore/FENodalLoad.h>

//-----------------------------------------------------------------------------
// define the parameter list
BEGIN_FECORE_CLASS(FEHeatSolver, FELinearSolver)
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
//! constructor for the class
FEHeatSolver::FEHeatSolver(FEModel* pfem) : FELinearSolver(pfem)
{
	m_ntotref = 0;
	m_niter = 0;
	m_nrhs = 0;

	// Allocate degrees of freedom
	DOFS& dofs = pfem->GetDOFS();
	int varT = dofs.AddVariable("temperature");
	dofs.SetDOFName(varT, 0, "T");

	// set the active degrees of freedom for this solver
	const int dof_T = dofs.GetDOF("T");
	vector<int> dof;
	dof.push_back(dof_T);
	SetDOF(dof);
}

//-----------------------------------------------------------------------------
FEHeatSolver::~FEHeatSolver()
{
}

//-----------------------------------------------------------------------------
//! Do one-time initialization for data
bool FEHeatSolver::Init()
{
	// Call base class first
	if (FELinearSolver::Init() == false) return false;

	FEModel& fem = *GetFEModel();

	const int dof_T = fem.GetDOFS().GetDOF("T");
	if (dof_T == -1) { assert(false); return false; }

	// allocate data structures
	int neq = NumberOfEquations();

	// get mesh
	FEMesh& mesh = fem.GetMesh();

	// Identify the heat-transfer domains
	// TODO: I want this to be done automatically
	//       e.g. while the input file is being read
	FEAnalysis* pstep = fem.GetCurrentStep();
	pstep->ClearDomains();
	for (int nd=0; nd<mesh.Domains(); ++nd)
	{
		FEHeatSolidDomain* pd = dynamic_cast<FEHeatSolidDomain*>(&mesh.Domain(nd));
		if (pd) pstep->AddDomain(nd);
	}
	assert(pstep->Domains() != 0);

	return true;
}

//-----------------------------------------------------------------------------
//! Calculate the global stiffness matrix. This function simply calls 
//! HeatStiffnessMatrix() for each domain which will calculate the
//! contribution to the global stiffness matrix from each domain.
bool FEHeatSolver::StiffnessMatrix(FELinearSystem& LS)
{
	FEModel& fem = *GetFEModel();

	FEAnalysis* pstep = fem.GetCurrentStep();

	// see if this is a dynamic problem
	bool bdyn = (pstep->m_nanalysis == FE_DYNAMIC);

	// get the time information
	const FETimeInfo& tp = fem.GetTime();

	// Add stiffness contribution from all domains
	for (int i=0; i<pstep->Domains(); ++i)
	{
		FEHeatDomain& bd = dynamic_cast<FEHeatDomain&>(*pstep->Domain(i));

		// add the conduction stiffness
		bd.ConductionMatrix(LS);

		// for a dynamic analysis add the capacitance matrix
		if (bdyn) 
		{
			bd.CapacitanceMatrix(LS, tp.timeIncrement);
		}
	}

	// add convective heat fluxes
	for (int i=0; i<fem.SurfaceLoads(); ++i)
	{
		FEConvectiveHeatFlux* pbc = dynamic_cast<FEConvectiveHeatFlux*>(fem.SurfaceLoad(i));
		if (pbc && pbc->IsActive()) pbc->StiffnessMatrix(LS, tp);
	}

	// add contact gap fluxes
	for (int i=0; i<fem.SurfacePairConstraints(); ++i)
	{
		FEGapHeatFlux* pci = dynamic_cast<FEGapHeatFlux*>(fem.SurfacePairConstraint(i));
		if (pci && pci->IsActive()) pci->StiffnessMatrix(LS, tp);
	}

	return true;
}
