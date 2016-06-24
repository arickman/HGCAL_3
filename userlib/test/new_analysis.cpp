/*
 * new_analysis.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: alexrickman
 */

#include<string>
#include<set>
#include<iostream>
#include<fstream>
#include<sstream>
#include <algorithm>
///#include <boost/algorithm/string.hpp>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TNtuple.h"
#include "TH2D.h"
#include "TH2Poly.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "Math/Vector4D.h"

#include "HGCSSEvent.hh"
#include "HGCSSInfo.hh"
#include "HGCSSSimHit.hh"
#include "HGCSSRecoHit.hh"
#include "HGCSSGenParticle.hh"
#include "HGCSSRecoJet.hh"
#include "HGCSSCalibration.hh"
#include "HGCSSDigitisation.hh"
#include "HGCSSDetector.hh"
#include "HGCSSGeometryConversion.hh"
#include "HGCSSSamplingSection.hh"
#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

int main(int argc, char** argv) {
	std::cout << "Opening the file " << argv[1] << std::endl;
	TFile *infile = TFile::Open(argv[1]);
	TTree *tree = (TTree*) infile->Get("HGCSSTree");

	//std::vector<HGCSSEvent> * EventVec = 0;
	//tree->SetBranchAddress("HGCSSEvent", &EventVec);

	std::vector<HGCSSGenParticle> * HadVec = 0;
	tree->SetBranchAddress("HGCSSHadAction", &HadVec);

	//std::vector<HGCSSGenParticle> * GenVec = 0;
	//tree->SetBranchAddress("HGCSSGenAction", &GenVec);

	TFile hfile("analyzed_tuple.root", "RECREATE");
	TTree t1("hadrons", "Hadron Study");

	Int_t nHadrons,hadronId[500],nProtons,nNeutrons;
	Float_t hadronKE[500],hadronTheta[500],hadronPhi[500];

	t1.Branch("nHadrons", &nHadrons, "nHadrons/I");
	t1.Branch("nProtons", &nProtons, "nProtons/I");
	t1.Branch("nNeutrons", &nNeutrons, "nNeutrons/I");

	t1.Branch("hadronId", &hadronId, "hadronId[nHadrons]/I");
	t1.Branch("hadronKE", &hadronKE, "KE[nHadrons]/F");
	t1.Branch("hadronTheta", &hadronTheta, "hadronTheta[nHadrons]/F");

	unsigned nEvts = tree->GetEntries();

	for (unsigned ievt(0); ievt < nEvts; ++ievt) { //loop on entries
		tree->GetEntry(ievt);

		nHadrons = 0;


		for (Int_t j = 0; j < HadVec->size(); j++) {
			HGCSSGenParticle& hadron = (*HadVec)[j];
			Int_t hPdg   = hadron.pdgid();
			hadronId[j]  = hPdg;
			hadronKE[j]	 = hadron.E() - hadron.mass();
			hadronTheta[j] = hadron.theta();
			hadronPhi[j] = hadron.phi();
			nHadrons = nHadrons + 1;
			if (hPdg == 2112){
				nNeutrons  += 1;
			}
			else if (hPdg == 2212){
				nProtons += 1;
			}
		}

		t1.Fill();
	}
	t1.Write();

	return 1;
}



