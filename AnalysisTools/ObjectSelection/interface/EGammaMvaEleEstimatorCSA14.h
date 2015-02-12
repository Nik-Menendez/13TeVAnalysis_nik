//--------------------------------------------------------------------------------------------------
// $Id $
//
// EGammaMvaEleEstimatorCSA14
//
// Helper Class for applying MVA electron ID selection
//
// Authors: D.Benedetti, E.DiMaro, S.Xie
//--------------------------------------------------------------------------------------------------

// See https://twiki.cern.ch/twiki/bin/viewauth/CMS/MultivariateElectronIdentificationRun2 for more details


#ifndef EGammaMvaEleEstimatorCSA14_H
#define EGammaMvaEleEstimatorCSA14_H

#include <vector>
#include <TROOT.h>
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

#include "DataFormats/PatCandidates/interface/Electron.h"

using namespace std;
using namespace reco;

class EGammaMvaEleEstimatorCSA14{
  public:
    EGammaMvaEleEstimatorCSA14();
    ~EGammaMvaEleEstimatorCSA14(); 
  
    enum MVAType {
      kTrig = 0,                     // MVA for triggering electrons     
      kNonTrig = 1,                      // MVA for non-triggering electrons
      kNonTrigPhys14 = 2,                // MVA for non-triggering electrons in Phys14                                                                                                                                                     
    };
  
    void     initialize( std::string methodName,
                         std::string weightsfile,
                         EGammaMvaEleEstimatorCSA14::MVAType type);
    void     initialize( std::string methodName,
                         EGammaMvaEleEstimatorCSA14::MVAType type,
                         Bool_t useBinnedVersion,
                         std::vector<std::string> weightsfiles );
    
    Bool_t   isInitialized() const { return fisInitialized; }
    UInt_t   GetMVABin(double eta,double pt ) const;
    
    void bindVariables();
    
    // for kTrig and kNonTrig algorithm

    Double_t mvaValue(const pat::Electron& ele,
                      bool printDebug);


 




  private:

    std::vector<TMVA::Reader*> fTMVAReader;
    std::string                fMethodname;
    Bool_t                     fisInitialized;
    MVAType                    fMVAType;
    Bool_t                     fUseBinnedVersion;
    UInt_t                     fNMVABins;

    Float_t                    fMVAVar_fbrem;
    Float_t                    fMVAVar_kfchi2;
    Float_t                    fMVAVar_kfhits;    //number of layers
    Float_t                    fMVAVar_kfhitsall; //number of hits
    Float_t                    fMVAVar_gsfchi2;

    Float_t                    fMVAVar_deta;
    Float_t                    fMVAVar_dphi;
    Float_t                    fMVAVar_detacalo;

    Float_t                    fMVAVar_see;
    Float_t                    fMVAVar_spp;
    Float_t                    fMVAVar_etawidth;
    Float_t                    fMVAVar_phiwidth;
    Float_t                    fMVAVar_OneMinusE1x5E5x5;
    Float_t                    fMVAVar_R9;

    Float_t                    fMVAVar_HoE;
    Float_t                    fMVAVar_EoP;
    Float_t                    fMVAVar_IoEmIoP;
    Float_t                    fMVAVar_eleEoPout;
    Float_t                    fMVAVar_EoPout; 
    Float_t                    fMVAVar_PreShowerOverRaw;

    Float_t                    fMVAVar_d0;
    Float_t                    fMVAVar_ip3d;
    Float_t                    fMVAVar_ip3dSig;

    Float_t                    fMVAVar_eta;
    Float_t                    fMVAVar_abseta;
    Float_t                    fMVAVar_pt;
    Float_t                    fMVAVar_rho;
    Float_t                    fMVAVar_isBarrel;
    Float_t                    fMVAVar_isEndcap;
    Float_t                    fMVAVar_SCeta;
  
    Float_t                    fMVAVar_ChargedIso_DR0p0To0p1;
    Float_t                    fMVAVar_ChargedIso_DR0p1To0p2;
    Float_t                    fMVAVar_ChargedIso_DR0p2To0p3;
    Float_t                    fMVAVar_ChargedIso_DR0p3To0p4;
    Float_t                    fMVAVar_ChargedIso_DR0p4To0p5;
    Float_t                    fMVAVar_GammaIso_DR0p0To0p1;
    Float_t                    fMVAVar_GammaIso_DR0p1To0p2;
    Float_t                    fMVAVar_GammaIso_DR0p2To0p3;
    Float_t                    fMVAVar_GammaIso_DR0p3To0p4;
    Float_t                    fMVAVar_GammaIso_DR0p4To0p5;
    Float_t                    fMVAVar_NeutralHadronIso_DR0p0To0p1;
    Float_t                    fMVAVar_NeutralHadronIso_DR0p1To0p2;
    Float_t                    fMVAVar_NeutralHadronIso_DR0p2To0p3;
    Float_t                    fMVAVar_NeutralHadronIso_DR0p3To0p4;
    Float_t                    fMVAVar_NeutralHadronIso_DR0p4To0p5;
 
};

#endif
