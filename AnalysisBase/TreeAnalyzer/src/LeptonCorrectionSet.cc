#include "AnalysisBase/TreeAnalyzer/interface/LeptonCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

namespace ucsbsusy {

void LeptonCorrectionSet::load(TString fileName, int correctionOptions)
{
  if(correctionOptions & LEP) {
    loadFile("LEP",fileName,correctionOptions);
    if(options_ & LEP) {
      lepCorr = new LepCorr(file);
      corrections.push_back(lepCorr);
      if(options_ & USE_HPSTAUS)
        setUseHPSTaus(true);
    }
  }
}

void LeptonCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  selLepWeight = 1;
  vetoLepWeight = 1;
  if(!ana->isMC()) return;

  if(options_ & LEP) {
    int nGoodGenMu = 0; int nGoodGenEle = 0; int nPromptGenTaus = 0;
    int nSelectedElectrons = 0;  int nSelectedMuons = 0;
    ///COUNT GEN LEPS
    for(auto* p : ana->genParts) {
      const GenParticleF * genPartMom = 0;
      if (p->numberOfMothers()>0) 
        genPartMom = p->mother(0);
      else
        continue;
      // electron from W/Z/tau
      if (ParticleInfo::isA(ParticleInfo::p_eminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom)))
        nGoodGenEle++;
      // muon from W/Z/tau
      else if (ParticleInfo::isA(ParticleInfo::p_muminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom)))
        nGoodGenMu++;
      // tau from W/Z
      else if (ParticleInfo::isA(ParticleInfo::p_tauminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom))) {
        bool lepDecay = false;
        for(unsigned int itd = 0; itd < p->numberOfDaughters(); itd++) {
          const GenParticleF* dau = p->daughter(itd);
          if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) lepDecay = true;
        }
        // hadronic tau
        if(!lepDecay)
          nPromptGenTaus++;
      }
    }

    bool ishighptmu = false, ishighptele = false;
    for(auto* i : ana->vetoedLeptons) {
      if(fabs(i->pdgid()) == 11) {
        nSelectedElectrons++;
        if(i->pt() > 20.0) ishighptele = true;
      }
      if(fabs(i->pdgid()) == 13) {
        nSelectedMuons++;
        if(i->pt() > 20.0) ishighptmu = true;
      }
    }

    vetoLepWeight = 1;
    selLepWeight  = 0;

    // muon selected region
    if (nSelectedMuons >= 1 && nGoodGenMu >= 1) {
      if(ishighptmu) lepCorr->setTargetBin(LepCorr::muCorrBinHighPt);
      else           lepCorr->setTargetBin(LepCorr::muCorrBinLowPt);
    }
    // electron selected region
    else if (nSelectedElectrons >= 1 && nSelectedMuons == 0 && nGoodGenEle >= 1){
      if(ishighptele) lepCorr->setTargetBin(LepCorr::eleCorrBinHighPt);
      else            lepCorr->setTargetBin(LepCorr::eleCorrBinLowPt);
    }
    // tau selected region
    else if (!useHPS && ana->nVetoedTracks >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1) {
      if(ana->vetoedTracks[0]->pt() > 20.0) lepCorr->setTargetBin(LepCorr::tauCorrBinHighPt);
      else                                  lepCorr->setTargetBin(LepCorr::tauCorrBinLowPt);
    }
    // HPS tau selected region
    else if (useHPS && ana->nVetoHPSTaus >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1) {
      lepCorr->setTargetBin(LepCorr::hpsTauCorrBin);
    }
    // fake region
    else  if ((nSelectedMuons >= 1) || (nSelectedElectrons >= 1) || (ana->nVetoedTracks >= 1)) {
      lepCorr->setTargetBin(LepCorr::fakeBin);
    }
    // veto region
    else {
      lepCorr->setTargetBin(LepCorr::defaultBin);
    }

    vetoLepWeight = 1 - lepCorr->get();
    selLepWeight  = lepCorr->get();
                 
    if(options_ & LEP_VARY_UP) {
      vetoLepWeight -= lepCorr->getError();
      selLepWeight  += lepCorr->getError();
    }
    else if(options_ & LEP_VARY_DOWN) {
      vetoLepWeight += lepCorr->getError();
      selLepWeight  -= lepCorr->getError();
    }

  }///LeptonCorrectionSet

}

} /* namespace ucsbsusy */
