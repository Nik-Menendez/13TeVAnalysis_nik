#ifndef ZEROLEPTONTREEHELPER_HH
#define ZEROLEPTONTREEHELPER_HH

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

using namespace ucsbsusy;

// Adjustments to default configuration
cfgSet::ConfigSet pars0lep(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  return cfg;
}

cfgSet::ConfigSet pars0lepCR(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_lepton_set;
  return cfg;
}

cfgSet::ConfigSet pars0LepPhoton(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_photon_set;
  return cfg;
}


struct TreeFiller {

  TreeFiller() {}
  size i_run       ;
  size i_lumi      ;
  size i_event     ;
  size i_weight    ;
  size i_puWeight  ;
  size i_pu50NSWeight  ;
  size i_passtrige ;
  size i_passtrigmu;
  size i_passtrige17e12;
  size i_passtrigmu17mu8;
  size i_passtrigmu17tkmu8;
  size i_passtrigphoton165;
  size i_passtright300; 
  size i_passtright350; 
  size i_passtright400; 
  size i_passtright475; 
  size i_passtright600; 
  size i_passtright650; 
  size i_passtright800; 
  size i_passtright900; 
  size i_passjson  ;
  size i_passdijetmet;
  size i_passcscflt;
  size i_passeebadscflt;
  size i_passhbheflt;
  size i_passhbhefixflt;
  size i_genmet    ;
  size i_bosonpt   ;
  size i_bosoneta  ;
  size i_met       ;
  size i_metphi    ;
  size i_metnohf   ;
  size i_metnohfphi;
  size i_npv       ;
  size i_nvetolep  ;
  size i_nvetotau  ;
  size i_nvetomu   ;
  size i_nvetolele ;
  size i_ngoodgenmu;
  size i_ngoodgenele;
  size i_npromptgentau;
  size i_nsellep   ;
  size i_nctt      ;
  size i_ncttstd   ;
  size i_ngenjets  ;
  size i_ngenbjets ;
  size i_njets     ;
  size i_njets60   ;
  size i_nbjets    ;
  size i_ntbjets   ;
  size i_ht        ;
  size i_j1pt      ;
  size i_j1eta     ;
  size i_j2pt      ;
  size i_j2eta     ;
  size i_j3pt      ;
  size i_j3eta     ;
  size i_csvj1pt   ;
  size i_csvj1eta  ;
  size i_csvj2pt   ;
  size i_csvj2eta  ;
  size i_csv1      ;
  size i_csv2      ;
  size i_dphij1met ;
  size i_dphij2met ;
  size i_dphij12met;
  size i_dphij3met ;
  size i_mtcsv1met ;
  size i_mtcsv2met ;
  size i_mtcsv12met;
  size i_dphicsv1met;
  size i_dphicsv2met;
  size i_dphicsv12met;
  size i_absdphilepw;
  size i_leptonpt  ;
  size i_leptoneta ;
  size i_mtlepmet  ;

 bool passCTTSelection(CMSTopF* ctt) {
    return (ctt->topRawMass() > 140.0 && ctt->topRawMass() < 250.0 && ctt->topMinMass() > 50.0 && ctt->topNsubJets() >= 3);
  } 

  void rankedByCSV(vector<RecoJetF*> inJets, vector<RecoJetF*>& outJets) {
    outJets.clear();
    outJets.resize(inJets.size());
    vector<pair<double,int> > rankedJets(inJets.size());

    for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){
      rankedJets[iJ].first = inJets[iJ]->csv();
      rankedJets[iJ].second = iJ;
    }

    std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());
    for(unsigned int iJ =0; iJ < inJets.size(); ++iJ) {
      outJets[iJ] = inJets[rankedJets[iJ].second];
    }
  }

  void book(TreeWriterData* data) {
    i_run            = data->add<unsigned int>("","run","i",0);
    i_lumi           = data->add<unsigned int>("","lumi","i",0);
    i_event          = data->add<unsigned int>("","event","i",0);
    i_weight         = data->add<float>("","weight","F",0);
    i_puWeight       = data->add<float>("","puWeight","F",0);
    i_pu50NSWeight   = data->add<float>("","pu50NSWeight","F",0);
    i_passtrige      = data->add<bool>("","passtrige","O",0);
    i_passtrigmu     = data->add<bool>("","passtrigmu","O",0);
    i_passtrige17e12 = data->add<bool >("","passtrige17e12","O",0);
    i_passtrigmu17mu8= data->add<bool >("","passtrigmu17mu8","O",0);
    i_passtrigmu17tkmu8 = data->add<bool>("","passtrigmu17tkmu8", "O",0);
    i_passtrigphoton165 = data->add<bool>("","passtrigphoton165", "O",0);
    i_passtright300  = data->add<bool>("","passtright300","O",0); 
    i_passtright350  = data->add<bool>("","passtright350","O",0); 
    i_passtright400  = data->add<bool>("","passtright400","O",0); 
    i_passtright475  = data->add<bool>("","passtright475","O",0); 
    i_passtright600  = data->add<bool>("","passtright600","O",0); 
    i_passtright650  = data->add<bool>("","passtright650","O",0); 
    i_passtright800  = data->add<bool>("","passtright800","O",0); 
    i_passtright900  = data->add<bool>("","passtright900","O",0); 
    i_passdijetmet   = data->add<bool>("","passdijetmet","O",0);
    i_passjson       = data->add<bool>("","passjson","O",0);
    i_passcscflt     = data->add<bool>("","passcscflt","O",0);
    i_passeebadscflt = data->add<bool>("","passeebadscflt","O",0);
    i_passhbheflt    = data->add<bool>("","passhbheflt","O",0);
    i_passhbhefixflt = data->add<bool>("","passhbhefixflt","O",0);
    i_genmet         = data->add<float>("","genmet","F",0);
    i_bosonpt        = data->add<float>("","bosonpt","F",0);
    i_bosoneta       = data->add<float>("","bosoneta","F",0);
    i_met            = data->add<float>("","met","F",0);
    i_metphi         = data->add<float>("","metphi","F",0);
    i_metnohf        = data->add<float>("","metnohf","F",0);
    i_metnohfphi     = data->add<float>("","metnohfphi","F",0);
    i_npv            = data->add<int>("","npv","I",0);
    i_nvetolep       = data->add<int>("","nvetolep","I",0);
    i_nvetotau       = data->add<int>("","nvetotau","I",0);
    i_nvetomu        = data->add<int>("","nvetomu","I",0);
    i_nvetolele      = data->add<int>("","nvetolele","I",0);
    i_ngoodgenmu     = data->add<int>("","ngoodgenmu","I",0);
    i_ngoodgenele    = data->add<int>("","ngoodgenele","I",0);
    i_npromptgentau  = data->add<int>("","npromptgentau","I",0);
    i_nsellep        = data->add<int>("","nsellep","I",0);
    i_nctt           = data->add<int>("","nctt","I",0);
    i_ncttstd        = data->add<int>("","ncttstd","I",0);
    i_ngenjets       = data->add<int>("","ngenjets","I",0);
    i_ngenbjets      = data->add<int>("","ngenbjets","I",0);
    i_njets          = data->add<int>("","njets","I",0);
    i_njets60        = data->add<int>("","njets60","I",0);
    i_nbjets         = data->add<int>("","nbjets","I",0);
    i_ntbjets        = data->add<int>("","ntbjets","I",0);
    i_ht             = data->add<float>("","ht","F",0);
    i_j1pt           = data->add<float>("","j1pt","F",0);
    i_j1eta          = data->add<float>("","j1eta","F",0);
    i_j2pt           = data->add<float>("","j2pt","F",0);
    i_j2eta          = data->add<float>("","j2eta","F",0);
    i_j3pt           = data->add<float>("","j3pt","F",0);
    i_j3eta          = data->add<float>("","j3eta","F",0);
    i_csvj1pt        = data->add<float>("","csvj1pt","F",0);
    i_csvj1eta       = data->add<float>("","csvj1eta","F",0);
    i_csvj2pt        = data->add<float>("","csvj2pt","F",0);
    i_csvj2eta       = data->add<float>("","csvj2eta","F",0);
    i_csv1           = data->add<float>("","csv1","F",0);
    i_csv2           = data->add<float>("","csv2","F",0);
    i_dphij1met      = data->add<float>("","dphij1met","F",0);
    i_dphij2met      = data->add<float>("","dphij2met","F",0);
    i_dphij12met     = data->add<float>("","dphij12met","F",0);
    i_dphij3met      = data->add<float>("","dphij3met","F",3);
    i_mtcsv1met      = data->add<float>("","mtcsv1met","F",0);
    i_mtcsv2met      = data->add<float>("","mtcsv2met","F",0);
    i_mtcsv12met     = data->add<float>("","mtcsv12met","F",0);
    i_dphicsv1met    = data->add<float>("","dphicsv1met","F",0);
    i_dphicsv2met    = data->add<float>("","dphicsv2met","F",0);
    i_dphicsv12met   = data->add<float>("","dphicsv12met","F",0);
    i_leptonpt       = data->add<float>("","leptonpt","F",0);
    i_leptoneta      = data->add<float>("","leptoneta","F",0);
    i_mtlepmet       = data->add<float>("","mtlepmet","F",0);
    i_absdphilepw    = data->add<float>("","absdphilepw","F",0);
  }

  void fillEventInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, int randomLepton = 0, bool lepAddedBack = false, MomentumF* metn = 0) {
    data->fill<unsigned int>(i_run, ana->run);
    data->fill<unsigned int>(i_lumi, ana->lumi);
    data->fill<unsigned int>(i_event, ana->event);
    data->fill<float>(i_weight, ana->weight);
    data->fill<float>(i_puWeight,    ana->eventCorrections.getPUWeight());
    data->fill<float>(i_pu50NSWeight,    ana->eventCorrections.get50NSPUWeight());
    data->fill<bool >(i_passtrige,  ana->isMC() ? ana->triggerflag & kHLT_Ele32_eta2p1_WP75_Gsf : (ana->process==defaults::DATA_SINGLEEL ? ana->triggerflag & kHLT_Ele32_eta2p1_WPLoose_Gsf : false));
    data->fill<bool >(i_passtrigmu, ana->isMC() ? ana->triggerflag & kHLT_IsoTkMu24_eta2p1 : (ana->process==defaults::DATA_SINGLEMU ? ana->triggerflag & kHLT_IsoTkMu24_eta2p1 : false));
    data->fill<bool >(i_passtrige17e12, ana->triggerflag & kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
    data->fill<bool >(i_passtrigmu17mu8, ana->triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ);
    data->fill<bool >(i_passtrigmu17tkmu8, ana->triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ);
    data->fill<bool >(i_passtrigphoton165, ana->triggerflag & kHLT_Photon165_HE10);
    data->fill<bool >(i_passtright300, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT300 : false)); 
    data->fill<bool >(i_passtright350, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT350 : false)); 
    data->fill<bool >(i_passtright400, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT400 : false)); 
    data->fill<bool >(i_passtright475, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT475 : false)); 
    data->fill<bool >(i_passtright600, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT600 : false)); 
    data->fill<bool >(i_passtright650, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT650 : false)); 
    data->fill<bool >(i_passtright800, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT800 : false)); 
    data->fill<bool >(i_passtright900, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT900 : false)); 
    data->fill<bool >(i_passdijetmet, ana->isMC() ? true : (ana->process==defaults::DATA_HTMHT ? ana->triggerflag & kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned : false));
    bool hasJSON = ana->hasJSONFile();
    bool isMC = ana->isMC();
    bool passesLumi = ana->passesLumiMask();
    data->fill<bool>(i_passjson, ((!isMC) && (hasJSON) && (!passesLumi)) ? false : true);
    data->fill<float>(i_genmet, ana->genmet->pt());
    if(!lepAddedBack) {
      data->fill<float>(i_met, ana->met->pt());
      data->fill<float>(i_metphi, ana->met->phi());
    } else {
      data->fill<float>(i_met, metn->pt());
      data->fill<float>(i_metphi, metn->phi());
    }
    data->fill<float>(i_metnohf, ana->metNoHF->pt());
    data->fill<float>(i_metnohfphi, ana->metNoHF->phi());
    data->fill<int  >(i_npv, ana->nPV);
    data->fill<int  >(i_nvetotau, ana->nVetoedTracks);
    data->fill<int  >(i_nvetolep, ana->nVetoedLeptons);

    int nVetoEle = 0; int nVetoMu = 0;
    for(auto i: ana->vetoedLeptons){
		  if(fabs(i->pdgid()) == 11) nVetoEle++;
		  if(fabs(i->pdgid()) == 13) nVetoMu++;
    }
    int nGoodGenMu = 0;int nGoodGenEle = 0;int nPromptTaus = 0;

    for(auto i: ana->genParts){
		const GenParticleF * genPartMom = 0;
		if (i->numberOfMothers()>0) { genPartMom = i->mother(0); }
		else{continue;}
		if ((abs(i->pdgId()) == 11) && (abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24 or abs(genPartMom->pdgId()) == 15)) nGoodGenEle++;
		if ((abs(i->pdgId()) == 13) && (abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24 or abs(genPartMom->pdgId()) == 15)) nGoodGenMu++;
		if ((abs(i->pdgId()) == 15) && (abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24)) {
			bool lepDecay = false;
                	for(unsigned int itd = 0; itd < i->numberOfDaughters(); itd++) {
                        	const GenParticleF* dau = i->daughter(itd);
                                if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) lepDecay = true;}
                        if(!lepDecay)
			nPromptTaus++;}
    }

    data->fill<int  >(i_nvetomu, nVetoMu);
    data->fill<int  >(i_nvetolele, nVetoEle);

    data->fill<int  >(i_ngoodgenmu, nGoodGenMu);
    data->fill<int  >(i_ngoodgenele, nGoodGenEle);
    data->fill<int  >(i_npromptgentau, nPromptTaus);
    data->fill<int  >(i_nsellep, ana->nSelLeptons);
    data->fill<int  >(i_nctt, ana->cttTops.size());
    int ncttstd = 0;
    for(auto* ctt : ana->cttTops) {
      if(passCTTSelection(ctt)) ncttstd++;
    }
    data->fill<int  >(i_ncttstd, ncttstd);
    if(ana->nSelLeptons > 0) {
      MomentumF* lep = new MomentumF(ana->selectedLeptons.at(randomLepton)->p4());
      MomentumF* W = new MomentumF(ana->selectedLeptons.at(randomLepton)->p4() + ana->met->p4());
      data->fill<float>(i_absdphilepw, fabs(PhysicsUtilities::deltaPhi(*W, *lep)) );
      data->fill<float>(i_leptonpt, lep->pt());
      data->fill<float>(i_leptoneta, lep->eta());
      data->fill<float>(i_mtlepmet, JetKinematics::transverseMass(*lep, *ana->met));
    }

    data->fill<bool>(i_passcscflt,ana->evtInfoReader.cscFlt);
    data->fill<bool>(i_passeebadscflt,ana->evtInfoReader.eeBadSCFlt);
    data->fill<bool>(i_passhbheflt,ana->evtInfoReader.hbheFlt);
    data->fill<bool>(i_passhbhefixflt,ana->evtInfoReader.hbheFixFlt);

  }

  void fillGenInfo(TreeWriterData* data, GenParticleF* boson, vector<GenJetF*> genjets, bool cleanjetsvboson = true) {
    data->fill<float>(i_bosonpt, boson->pt());
    data->fill<float>(i_bosoneta, boson->eta());
    int ngenjets = 0, ngenbjets = 0;
    for(auto* j : genjets) {
      if(cleanjetsvboson && PhysicsUtilities::deltaR2(*j, *boson) < 0.16) continue;
      ngenjets++;
      if(fabs(j->flavor()) == JetFlavorInfo::b_jet) ngenbjets++;
    }
    data->fill<int  >(i_ngenjets, ngenjets);
    data->fill<int  >(i_ngenbjets, ngenbjets);
  }

  void fillJetInfo(TreeWriterData* data, vector<RecoJetF*> jets, vector<RecoJetF*> bjets, MomentumF* met) {
    int njets60 = 0;
    for(auto* j : jets) {
      if(j->pt() > 60.0) njets60++;
    }
    int ntbjets = 0;
    for(auto* b : bjets) {
      if(b->csv() > defaults::CSV_TIGHT) ntbjets++;
    }
    data->fill<int>(i_njets, int(jets.size()));
    data->fill<int>(i_njets60, njets60);
    data->fill<int>(i_nbjets, int(bjets.size()));
    data->fill<int>(i_ntbjets, ntbjets);
    data->fill<float>(i_ht, JetKinematics::ht(jets, 20.0, 2.4));

    float dphij1met = 0.0, dphij2met = 0.0;
    if(jets.size() > 0) {
      data->fill<float>(i_j1pt, jets[0]->pt());
      data->fill<float>(i_j1eta, jets[0]->eta());
      dphij1met = fabs(PhysicsUtilities::deltaPhi(*jets[0], *met));
      data->fill<float>(i_dphij1met, dphij1met);
      if(jets.size() == 1)
        data->fill<float>(i_dphij12met, dphij1met);
    }
    if(jets.size() > 1) {
      data->fill<float>(i_j2pt, jets[1]->pt());
      data->fill<float>(i_j2eta, jets[1]->eta());
      dphij2met = fabs(PhysicsUtilities::deltaPhi(*jets[1], *met));
      data->fill<float>(i_dphij2met, dphij2met);
      data->fill<float>(i_dphij12met, min(dphij1met,dphij2met));
    }
    if(jets.size() > 2) {
      data->fill<float>(i_j3pt, jets[2]->pt());
      data->fill<float>(i_j3eta, jets[2]->eta());
      data->fill<float>(i_dphij3met, fabs(PhysicsUtilities::deltaPhi(*jets[2], *met)));
    }

    vector<RecoJetF*> jetsCSVranked;
    rankedByCSV(jets,jetsCSVranked);

    float mtcsv1met = 0.0, mtcsv2met = 0.0,dphicsv1met = 0.0,dphicsv2met = 0.0;
    if(jetsCSVranked.size() > 0) {
      mtcsv1met = JetKinematics::transverseMass(*jetsCSVranked[0], *met);
      dphicsv1met = fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[0], *met));
      data->fill<float>(i_mtcsv1met, mtcsv1met);
      data->fill<float>(i_csvj1pt, jetsCSVranked[0]->pt());
      data->fill<float>(i_csvj1eta, jetsCSVranked[0]->eta());
      data->fill<float>(i_csv1, jetsCSVranked[0]->csv());
      data->fill<float>(i_dphicsv1met, dphicsv1met);
      if(jetsCSVranked.size() == 1) {
        data->fill<float>(i_mtcsv12met, mtcsv1met);
        data->fill<float>(i_dphicsv12met, dphicsv1met);
      }
    }

    if(jetsCSVranked.size() > 1) {
      mtcsv2met = JetKinematics::transverseMass(*jetsCSVranked[1], *met);
      dphicsv2met = fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[1], *met));
      data->fill<float>(i_csvj2pt, jetsCSVranked[1]->pt());
      data->fill<float>(i_csvj2eta, jetsCSVranked[1]->eta());
      data->fill<float>(i_csv2, jetsCSVranked[1]->csv());
      data->fill<float>(i_mtcsv2met, mtcsv2met);
      data->fill<float>(i_mtcsv12met, min(mtcsv1met,mtcsv2met));
      data->fill<float>(i_dphicsv2met, dphicsv2met);
      data->fill<float>(i_dphicsv12met,min(dphicsv1met,dphicsv2met));
    }
   
  }

};

class ZeroLeptonAnalyzer : public TreeCopierManualBranches {

  public :
    ZeroLeptonAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, cfgSet::ConfigSet *pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, isMCTree, pars) {zIsInvisible = true;}

    const double metcut_ = 175.0 ;

    TreeFiller filler;

    virtual ~ZeroLeptonAnalyzer() {}

    void book() {
      filler.book(&data);
    }

    bool fillEvent() {

      if(nVetoedLeptons > 0)  return false;

      if(nVetoedTracks > 0)   return false;

      if(met->pt() < metcut_) return false;
      if(!goodvertex) return false;

      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);
      return true;
    }

};

#endif
