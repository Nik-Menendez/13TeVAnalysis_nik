
#include "TRandom3.h"
#include "TFile.h"
#include "AnalysisTools/QuickRefold/interface/TF1Container.h"

#include "AnalysisBase/TreeAnalyzer/interface/JetAndMETCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"


namespace ucsbsusy {
JetResolutionCorr::JetResolutionCorr(TRandom3 * inRandGen, TString jetResFileName, TString jetResName)
: Correction("JetResolution"), randGen(inRandGen) {
  std::clog << "     Loading Jet Resolutions: "<< jetResName <<" from file: "<< jetResFileName << std::endl;
  jetResolutionFile = TFile::Open(jetResFileName,"read");
  if(!jetResolutionFile) throw std::invalid_argument("JetResolutionCorr::JetResolutionCorr: Res file not found!");
  jetResolution = (const QuickRefold::TF1Container*)(jetResolutionFile->Get(jetResName) );
  if(!jetResolution) throw std::invalid_argument("JetResolutionCorr::JetResolutionCorr: Jet resolution file could not be found!");
}

JetResolutionCorr::~JetResolutionCorr(){
  if(jetResolutionFile) jetResolutionFile->Close();
  delete jetResolutionFile;
}

bool JetResolutionCorr::getCorrectedPT(const double gausWidth, const RecoJetF& jet, double& newPT) const {
  newPT = jet.pt();

  //base cuts
  if(jet.pt() < 10) return false;

  double absEta, pt;

  //if the gen jet is above 20 gev we use its properties
  if(jet.genJet() && jet.genJet()->pt() > 20 ){
    absEta = jet.genJet()->absEta();
    pt = jet.genJet()->pt();
  } else {
    absEta = jet.absEta();
    pt = jet.pt();
  }

  double sigma = jetResolution->getValue(absEta).Eval(std::min(std::max(pt,20.0),1000.0));

  double rndNum = 10;
  while(TMath::Abs(rndNum) > 3 ){ //only allow three sigma fluctuations
    rndNum = randGen->Gaus(0,1);
  }

  newPT += rndNum*gausWidth*sigma*pt;
  if(newPT < 0) newPT = 0;

  return true;
}

void JetResolutionCorr::correctJetsAndMET(const double resSF, std::vector<RecoJetF>& jets, MomentumF& met) const {
  if(resSF < 1) throw std::invalid_argument("JetResolutionCorr::correctJetsAndMET: resSF must be greater than 1!");
  if(resSF == 1) return;

  double resWidth = TMath::Sqrt(resSF*resSF -1);

  for(auto& j : jets){
    double newPT;
    if(!getCorrectedPT(resWidth,j,newPT)) continue;

     met.p4() += j.p4();
     CylLorentzVectorF newMom( newPT < 0 ? 0 : newPT, j.eta(), j.phi(), newPT < 0 ? 0 : newPT*j.mass() /j.pt());
     j.setP4(newMom);

     met.p4() -= j.p4();
  }

}


float METScaleCorr::scaleCorr(float trueMETPT) const {
  double x = std::min(std::max(20.0,double(trueMETPT)),300.0);
  return (-0.0402319 + 0.285912/x -64.209/(x*x)) - (0.015672 -0.163545/x -38.5237/(x*x));
}
CylLorentzVectorF METScaleCorr::getCorrectedMET(const CylLorentzVectorF& trueBosons, const CylLorentzVectorF& met) const {
  if(trueBosons.pt() <= 0)
    return met;
  return met + CylLorentzVectorF(trueBosons.pt(),0,trueBosons.phi(),0)*scaleCorr(trueBosons.pt()) ;
}

METResCorr::METResCorr() : Correction("METRes"){}
CylLorentzVectorF METResCorr::getCorrectedMET(const CylLorentzVectorF& trueBosons,const CylLorentzVectorF& trueMET, CylLorentzVectorF met) const {
  if(trueBosons.pt() <= 0)
    return met;
  if(trueMET.pt() >= 0){
    met -= trueMET;
  }

  float metParTrue   = met.pt() * TMath::Cos( PhysicsUtilities::deltaPhi(trueBosons,met) );
  float metPerpTrue  = met.pt() * TMath::Sin( PhysicsUtilities::deltaPhi(trueBosons,met) );

  //variable scaling
  double scalingPT = std::min(trueBosons.pt(),float(300.0));
  double parScale = 0.857944 + 0.000750737*scalingPT;
  double perpScale = 0.842937 + 0.000569862*scalingPT;

  CylLorentzVectorF truePar ( TMath::Abs(metParTrue) * parScale, 0, metParTrue > 0 ? trueBosons.phi() : TVector2::Phi_mpi_pi(trueBosons.phi() - TMath::Pi()) , 0);
  CylLorentzVectorF truePerp ( TMath::Abs(metPerpTrue) * perpScale, 0,
      metPerpTrue > 0 ? TVector2::Phi_mpi_pi(trueBosons.phi() - TMath::PiOver2() ) : TVector2::Phi_mpi_pi(trueBosons.phi() + TMath::PiOver2() ) , 0 );


  if(trueMET.pt() >= 0){
    return trueMET + truePar + truePerp;
  } else {
    return truePar + truePerp;
  }


}

float METNoHFScaleCorr::scaleCorr(float trueMETPT) const {
  double x = std::min(std::max(20.0,double(trueMETPT)),300.0);
  return ( (-0.0569193 -2.10679/x -52.8809/(x*x)) - (-0.00187469 -2.90383/x -33.1923/(x*x) ) );
}
CylLorentzVectorF METNoHFScaleCorr::getCorrectedMET(const CylLorentzVectorF& trueBosons, const CylLorentzVectorF& met) const {
  if(trueBosons.pt() <= 0)
    return met;
  return met + CylLorentzVectorF(trueBosons.pt(),0,trueBosons.phi(),0)*scaleCorr(trueBosons.pt()) ;
}

METNoHFResCorr::METNoHFResCorr() : Correction("METNoHFResCorr"){}
CylLorentzVectorF METNoHFResCorr::getCorrectedMET(const CylLorentzVectorF& trueBosons,const CylLorentzVectorF& trueMET, CylLorentzVectorF met) const {
  if(trueBosons.pt() <= 0)
    return met;
  if(trueMET.pt() >= 0){
    met -= trueMET;
  }

  float metParTrue   = met.pt() * TMath::Cos( PhysicsUtilities::deltaPhi(trueBosons,met) );
  float metPerpTrue  = met.pt() * TMath::Sin( PhysicsUtilities::deltaPhi(trueBosons,met) );

  CylLorentzVectorF truePar ( TMath::Abs(metParTrue) * 0.97, 0, metParTrue > 0 ? trueBosons.phi() : TVector2::Phi_mpi_pi(trueBosons.phi() - TMath::Pi()) , 0);
  CylLorentzVectorF truePerp ( TMath::Abs(metPerpTrue) * 0.94, 0,
      metPerpTrue > 0 ? TVector2::Phi_mpi_pi(trueBosons.phi() - TMath::PiOver2() ) : TVector2::Phi_mpi_pi(trueBosons.phi() + TMath::PiOver2() ) , 0 );


  if(trueMET.pt() >= 0){
    return trueMET + truePar + truePerp;
  } else {
    return truePar + truePerp;
  }


}
JetAndMETCorrectionSet::JetAndMETCorrectionSet(): metScale(0),metResolution(0), jetResolution(0), metNoHFScale(0), metNoHFResolution(0)
,originalMET(new MomentumF),correctedMET(new MomentumF),originalMETNoHF(new MomentumF),correctedMETNoHF(new MomentumF),
trueBosons(new CylLorentzVectorF),trueMET(new CylLorentzVectorF)
{}
JetAndMETCorrectionSet::~JetAndMETCorrectionSet(){
  delete originalMET;
  delete correctedMET;
  delete originalMETNoHF;
  delete correctedMETNoHF;
  delete trueBosons;
  delete trueMET;
}
void JetAndMETCorrectionSet::load(int correctionOptions,TString jetResolutionFile,TRandom3 * randomGenerator)
{
  loadSimple("JetAndMET",correctionOptions);
  if(options_ & METSCALE){
    metScale = new METScaleCorr;
    corrections.push_back(metScale);
    metNoHFScale = new METNoHFScaleCorr;
    corrections.push_back(metNoHFScale);
  }
  if(options_ & METRESOLUTION){
    metResolution = new METResCorr;
    corrections.push_back(metResolution);
    metNoHFResolution = new METNoHFResCorr;
    corrections.push_back(metNoHFResolution);
  }
  if(options_ & JETRESOLUTION){
    jetResolution = new JetResolutionCorr(randomGenerator,jetResolutionFile);
    corrections.push_back(jetResolution);
  }
}

void JetAndMETCorrectionSet::processMET(const BaseTreeAnalyzer * ana) {
  (*originalMET) =  (*ana->met);
  (*correctedMET) = (*ana->met);

  (*originalMETNoHF) =  (*ana->metNoHF);
  (*correctedMETNoHF) = (*ana->metNoHF);

  if(ana->process < defaults::TTBAR || ana->process > defaults::SIGNAL ){
    return;
  }

  if(options_ & NULLOPT) return;


  (*trueBosons) = CylLorentzVectorF();
  (*trueMET)    = CylLorentzVectorF();


  for(const auto& g: ana->genParticleReader.genParticles ){
    if(!ParticleInfo::isLastInChain(&g)) continue;

    int pdgid = TMath::Abs(g.pdgId());
    if(ParticleInfo::isBSM(pdgid)){
      (*trueBosons) += g.p4();
      (*trueMET) += g.p4();
      continue;
    }
    if(pdgid == ParticleInfo::p_gamma && ParticleInfo::isDoc(g.status())){
      (*trueBosons) += g.p4();
      continue;
    }
    if(pdgid == ParticleInfo::p_Wplus || pdgid == ParticleInfo::p_Z0){
      bool isLeptonic = false;
      for(unsigned int iD = 0; iD < g.numberOfDaughters(); ++iD){
        GenParticleFRef dau = ParticleInfo::getFinal(g.daughterRef(iD));
        int dauPdgid = TMath::Abs(dau->pdgId());
        if(!ParticleInfo::isLeptonOrNeutrino(dauPdgid)) continue;
        isLeptonic = true;

        if(ParticleInfo::isANeutrino(dauPdgid)){
          (*trueMET) += dau->p4();
        } else if(pdgid == ParticleInfo::p_tauminus){
          for(unsigned int iD2 = 0; iD2 < dau->numberOfDaughters(); ++iD2){
            GenParticleFRef dau2 = ParticleInfo::getFinal(dau->daughterRef(iD2));
            if(ParticleInfo::isANeutrino(dau2->pdgId()))
              (*trueMET) += dau2->p4();
          }
        }
      }
      if(isLeptonic)
        (*trueBosons) += g.p4();
      continue;
    }
    continue;
  }


  CylLorentzVectorF tempMET = correctedMET->p4();

  if(metResolution) {
    tempMET = metResolution->getCorrectedMET(*trueBosons,*trueMET,tempMET);
  }
  if(metScale) {
    tempMET = metScale->getCorrectedMET(*trueBosons,tempMET);
  }
  correctedMET->setP4(tempMET);

  CylLorentzVectorF tempMETNoHF = correctedMETNoHF->p4();

  if(metResolution) {
    tempMETNoHF = metNoHFResolution->getCorrectedMET(*trueBosons,*trueMET,tempMETNoHF);
  }
  if(metScale) {
    tempMETNoHF = metNoHFScale->getCorrectedMET(*trueBosons,tempMETNoHF);
  }
  correctedMETNoHF->setP4(tempMETNoHF);

}

void JetAndMETCorrectionSet::correctJetResolution(const BaseTreeAnalyzer * ana, RecoJetFCollection& jets, MomentumF& met){
  if(!jetResolution) return;
  if(!ana->isMC()) return;
  jetResolution->correctJetsAndMET(ana->getAnaCfg().corrections.jetResCorr,jets,met);
}

MomentumF JetAndMETCorrectionSet::getCorrectedMET() const {return *correctedMET;}
MomentumF JetAndMETCorrectionSet::getOriginalMET() const {return *originalMET;}
MomentumF JetAndMETCorrectionSet::getCorrectedMETNoHF() const {return *correctedMETNoHF;}
MomentumF JetAndMETCorrectionSet::getOriginalMETNoHF() const {return *originalMETNoHF;}

} /* namespace ucsbsusy */
