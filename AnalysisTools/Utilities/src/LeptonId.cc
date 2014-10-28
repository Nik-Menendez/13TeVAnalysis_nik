//--------------------------------------------------------------------------------------------------
// 
// LeptonId
// 
// Class for implementing lepton selections.
// 
// LeptonId.cc created on Mon Oct 27 14:08:36 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/Utilities/interface/LeptonId.h"

using namespace ucsbsusy;

LeptonId::LeptonId(int bunchSpacing, bool initElectronIds)
{

  bunchSpacing_ = bunchSpacing;
  eleIdInit_ = false;

  if(initElectronIds) initElectronCuts(bunchSpacing_);

}

LeptonId::~LeptonId()
{

  csa14Cuts_barrel_.clear();
  csa14Cuts_endcap_.clear();

}

bool LeptonId::initElectronCuts(int bunchSpacing)
{

  printf("ID scenario: %dns bunch spacing\n", bunchSpacing);

  switch(bunchSpacing) {

    case 25: {

      csa14Cuts_barrel_["dEtaIn"] 		= {{0.02, 0.0181, 0.0106, 0.0091}};
      csa14Cuts_barrel_["dPhiIn"] 		= {{0.2579, 0.0936, 0.0323, 0.031}};
      csa14Cuts_barrel_["full5x5_sigmaIetaIeta"] = {{0.0125, 0.0123, 0.0107, 0.0106}};
      csa14Cuts_barrel_["hOverE"] 		= {{0.2564, 0.141, 0.067, 0.0532}};
      csa14Cuts_barrel_["d0"] 			= {{0.025, 0.0166, 0.0131, 0.0126}};
      csa14Cuts_barrel_["dz"] 			= {{0.5863, 0.54342, 0.22310, 0.0116}};
      csa14Cuts_barrel_["oOverEmOOverP"] 	= {{0.1508, 0.1353, 0.1043, 0.0609}};
      csa14Cuts_barrel_["pfdbiso"] 		= {{0.3313, 0.24, 0.2179, 0.1649}};
      csa14Cuts_barrel_["nMissingHits"] 	= {{2, 1, 1, 1}};

      csa14Cuts_endcap_["dEtaIn"] 		= {{0.0141, 0.0124, 0.0108, 0.0106}};
      csa14Cuts_endcap_["dPhiIn"] 		= {{0.2591, 0.0642, 0.0455, 0.0359}};
      csa14Cuts_endcap_["full5x5_sigmaIetaIeta"] = {{0.0371, 0.035, 0.0318, 0.0305}};
      csa14Cuts_endcap_["hOverE"] 		= {{0.1335, 0.1115, 0.097, 0.0835}};
      csa14Cuts_endcap_["d0"] 			= {{0.2232, 0.098, 0.0845, 0.0163}};
      csa14Cuts_endcap_["dz"] 			= {{0.9513, 0.9187, 0.7523, 0.5999}};
      csa14Cuts_endcap_["oOverEmOOverP"]	= {{0.1542, 0.1443, 0.1201, 0.1126}};
      csa14Cuts_endcap_["pfdbiso"] 		= {{0.3816, 0.3529, 0.254, 0.2075}};
      csa14Cuts_endcap_["nMissingHits"] 	= {{3, 1, 1, 1}};

      eleIdInit_ = true;
      break;

    }

    case 50: {

      csa14Cuts_barrel_["dEtaIn"] 		= {{0.021, 0.016, 0.015, 0.012}};
      csa14Cuts_barrel_["dPhiIn"] 		= {{0.25, 0.080, 0.051, 0.024}};
      csa14Cuts_barrel_["full5x5_sigmaIetaIeta"] = {{0.012, 0.012, 0.010, 0.010}};
      csa14Cuts_barrel_["hOverE"] 		= {{0.24, 0.15, 0.10, 0.074}};
      csa14Cuts_barrel_["d0"] 			= {{0.031, 0.019, 0.012, 0.0091}};
      csa14Cuts_barrel_["dz"] 			= {{0.50, 0.036, 0.030, 0.017}};
      csa14Cuts_barrel_["oOverEmOOverP"] 	= {{0.32, 0.11, 0.053, 0.026}};
      csa14Cuts_barrel_["pfdbiso"] 		= {{0.24, 0.18, 0.14, 0.10}};
      csa14Cuts_barrel_["nMissingHits"] 	= {{2, 1, 1, 1}};

      csa14Cuts_endcap_["dEtaIn"] 		= {{0.028, 0.025, 0.023, 0.019}};
      csa14Cuts_endcap_["dPhiIn"] 		= {{0.23, 0.097, 0.056, 0.043}};
      csa14Cuts_endcap_["full5x5_sigmaIetaIeta"] = {{0.035, 0.032, 0.030, 0.029}};
      csa14Cuts_endcap_["hOverE"] 		= {{0.19, 0.12, 0.099, 0.080}};
      csa14Cuts_endcap_["d0"] 			= {{0.22, 0.099, 0.068, 0.037}};
      csa14Cuts_endcap_["dz"] 			= {{0.91, 0.88, 0.78, 0.065}};
      csa14Cuts_endcap_["oOverEmOOverP"] 	= {{0.13, 0.11, 0.11, 0.076}};
      csa14Cuts_endcap_["pfdbiso"] 		= {{0.24, 0.21, 0.15, 0.14}};
      csa14Cuts_endcap_["nMissingHits"] 	= {{3, 1, 1, 1}};

      eleIdInit_ = true;
      break;

    }

    default: {

      printf("Cuts not defined for this scenario!\n" );

      eleIdInit_ = false;

    }

  }

  return eleIdInit_;

}

bool LeptonId::passEleIdCSA14CutBased(ExtendedElectron *ele, unsigned int WP)
{

  return passEleIdCSA14CutBased(ele->pt(), ele->scEta(),
				ele->dEtaIn(), ele->dPhiIn(),
				ele->full5x5sigietaieta(), ele->hOverE(),
				ele->d0(), ele->dz(),
				ele->ecalE(), ele->eOverPIn(),
				ele->pfdbetaiso(), ele->passConvVeto(),
				ele->nLostHitsInner(), WP);

}

bool LeptonId::passEleIdCSA14CutBased(float elePt, float eleSCeta,
				      float eleDEtaIn, float eleDPhiIn,
				      float eleFull5x5sigietaieta, float eleHOverE,
				      float eleD0, float eleDz,
				      float eleEcalE, float eleEOverPIn,
				      float elePFdbetaiso, bool elePassConvVeto,
				      int eleNMissingHits, unsigned int WP)
{

  if(!eleIdInit_) {
    printf("Cuts have not been initialized!\n");
    return false;
  }

  if(fabs(eleSCeta) > 2.5) return false;

  bool pass = false;

  eleCuts *cuts = fabs(eleSCeta) < 1.479 ? &csa14Cuts_barrel_ : &csa14Cuts_endcap_;

  float eleOoEmooP = 0.0;
  if(eleEcalE == 0.0) {
    eleOoEmooP = 1e30;
  } else if(!std::isfinite(eleEcalE)) {
    eleOoEmooP = 1e30;
  } else {
    eleOoEmooP = fabs((1.0 - eleEOverPIn)/eleEcalE);
  }

  pass =   fabs(eleDEtaIn) < (*cuts)["dEtaIn"][WP]
	&& fabs(eleDPhiIn) < (*cuts)["dPhiIn"][WP]
	&& eleFull5x5sigietaieta < (*cuts)["full5x5_sigmaIetaIeta"][WP]
	&& eleHOverE < (*cuts)["hOverE"][WP]
	&& fabs(eleD0) < (*cuts)["d0"][WP]
	&& fabs(eleDz) < (*cuts)["dz"][WP]
	&& eleOoEmooP < (*cuts)["oOverEmOOverP"][WP]
	&& elePFdbetaiso/elePt < (*cuts)["pfdbiso"][WP]
	&& elePassConvVeto
	&& eleNMissingHits < (*cuts)["nMissingHits"][WP];

  return pass;

}

bool LeptonId::passEleIdCSA14MVA(ElectronF *ele, unsigned int WP)
{

  printf("Working points not yet defined!\n");

  return false;

}

bool LeptonId::passEleIsoCSA14(ElectronF *ele, unsigned int WP)
{

  return passEleIsoCSA14(ele->pt(), ele->scEta(), ele->pfdbetaiso(), WP);

}

bool LeptonId::passEleIsoCSA14(float elePt, float eleSCeta, float elePFdbetaiso, unsigned int WP)
{

  if(!eleIdInit_) {
    printf("Cuts have not been initialized!\n");
    return false;
  }

  if(fabs(eleSCeta) > 2.5) return false;

  eleCuts *cuts = fabs(eleSCeta) < 1.479 ? &csa14Cuts_barrel_ : &csa14Cuts_endcap_;

  return elePFdbetaiso/elePt < (*cuts)["pfdbiso"][WP];

}
