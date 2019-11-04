#include "FEHeatSource.h"
#include "FEHeatSolidDomain.h"

//-----------------------------------------------------------------------------
FEHeatSource::FEHeatSource(FEModel* pfem) : FEBodyLoad(pfem)
{
}


//-----------------------------------------------------------------------------
void FEHeatSource::ForceVector(FEGlobalVector& R)
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

//-----------------------------------------------------------------------------
BEGIN_FECORE_CLASS(FEBioHeatSource, FEHeatSource);
	ADD_PARAMETER(m_Ta, "Ta");
	ADD_PARAMETER(m_W , "W");
	ADD_PARAMETER(m_cb, "cb");
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
FEBioHeatSource::FEBioHeatSource(FEModel* fem) : FEHeatSource(fem)
{
	m_Ta = 0;
	m_W = 0;
	m_cb = 0;
}

double FEBioHeatSource::value(FEMaterialPoint& mp)
{
	FEHeatMaterialPoint& pt = *mp.ExtractData<FEHeatMaterialPoint>();

	return -m_W*m_cb*(pt.m_T - m_Ta);
}

void FEBioHeatSource::StiffnessMatrix(FELinearSystem& LS, const FETimeInfo& tp)
{
	FEDomainList& domList = GetDomainList();
	assert(domList.IsEmpty() == false);
	for (int idom = 0; idom < domList.Domains(); ++idom)
	{
		FESolidDomain& dom = *dynamic_cast<FESolidDomain*>(domList.GetDomain(idom));

		int NE = dom.Elements();
		for (int ie = 0; ie < NE; ++ie)
		{
			FESolidElement& el = dom.Element(ie);

			int neln = el.Nodes();

			FEElementMatrix ke;
			ke.resize(neln, neln);
			ke.zero();

			int nint = el.GaussPoints();
			double* w = el.GaussWeights();
			for (int n = 0; n < nint; ++n)
			{
				FEMaterialPoint& mp = *el.GetMaterialPoint(n);
				double* H = el.H(n);
				double Jw = dom.detJt(el, n) * w[n];

				for (int i = 0; i < neln; ++i)
					for (int j = 0; j < neln; ++j)
					{
						ke[i][j] += -Jw*H[i] * H[j]*m_W*m_cb;
					}
			}

			vector<int> lm(neln, 0);
			dom.UnpackLM(el, lm);

			ke.SetNodes(el.m_node);
			ke.SetIndices(lm);

			LS.Assemble(ke);
		}
	}
}
