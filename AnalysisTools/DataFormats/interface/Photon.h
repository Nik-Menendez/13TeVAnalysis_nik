/*
 * Photon.h
 *
 *  Class for storing photon object information.
 *
 *  Created on: May 22, 2015
 *      Author: hqu
 */

#ifndef ANALYSISTOOLS_DATAFORMATS_PHOTON_H
#define ANALYSISTOOLS_DATAFORMATS_PHOTON_H

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

  template <class CoordSystem>
  class Photon : public Momentum<CoordSystem>
  {

    public :
      Photon() : index_(-1), scEta_(0), r9_(0), pfdbetaiso_(0), isloose_(false), ismedium_(false), istight_(false) {}

      template <class InputCoordSystem>
      Photon(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
          int inIndex = -1, float inSCEta = 0, float inR9 = 0, float inPfdbetaiso = 0,
          bool inIsLoose = false, bool inIsMedium = false, bool inIsTight = false) :
          Momentum<InputCoordSystem>(inMomentum),
          index_(inIndex), scEta_(inSCEta), r9_(inR9), pfdbetaiso_(inPfdbetaiso),
          isloose_(inIsLoose), ismedium_(inIsMedium), istight_(inIsTight) {}

      ~Photon() {}

      int   index()           const { return index_; }
      float scEta()           const { return scEta_; }
      float r9()              const { return r9_; }
      float pfdbetaiso()      const { return pfdbetaiso_; }
      bool  isloose()         const { return isloose_; }
      bool  ismedium()        const { return ismedium_; }
      bool  istight()         const { return istight_; }

      void  setIndex(int newIndex)        { index_ = newIndex;    }
      void  setSCEta(float newSCEta)      { scEta_ = newSCEta;    }
      void  setR9(float newR9)            { r9_    = newR9;       }
      void  setPFDBetaIso(float newIso)   { pfdbetaiso_ = newIso; }
      void  setIsLoose(bool newType)      { isloose_ = newType;   }
      void  setIsMedium(bool newType)     { ismedium_ = newType;  }
      void  setIsTight(bool newType)      { istight_ = newType;   }

    protected :
      int   index_;  //Index in photon vector
      float scEta_;
      float r9_;
      float pfdbetaiso_;
      bool  isloose_;
      bool  ismedium_;
      bool  istight_;


  };

  typedef Photon<CylLorentzCoordF>  PhotonF;
  typedef std::vector<PhotonF>    PhotonFCollection;


  class ExtendedPhoton : public PhotonF
  {
    public :
      ExtendedPhoton() : sigietaieta_(0), passElectronVeto_(false), full5x5sigietaieta_(0), hOverE_(0), rhoPFchargedHadronIso_(0), rhoPFneutralHadronIso_(0), rhoPFphotonIso_(0), trackiso_(0), ecaliso_(0), hcaliso_(0), pfchargedHadronIso_(0), pfneutralHadronIso_(0), pfphotonIso_(0), pfpuiso_(0) {}

      ~ExtendedPhoton() {}

      float sigietaieta()             const { return sigietaieta_;    }
      bool  passElectronVeto()        const { return passElectronVeto_; }
      float full5x5sigietaieta()      const { return full5x5sigietaieta_; }
      float hOverE()                  const { return hOverE_;   }
      float rhoPFChargedHadronIso()   const { return rhoPFchargedHadronIso_; }
      float rhoPFNeutralHadronIso()   const { return rhoPFneutralHadronIso_; }
      float rhoPFphotonIso()          const { return rhoPFphotonIso_;}
      float trackiso()                const { return trackiso_;   }
      float ecaliso()                 const { return ecaliso_;    }
      float hcaliso()                 const { return hcaliso_;    }
      float pfChargedHadronIso()      const { return pfchargedHadronIso_;   }
      float pfNeutralHadronIso()      const { return pfneutralHadronIso_;   }
      float pfPhotonIso()             const { return pfphotonIso_;    }
      float pfPUIso()                 const { return pfpuiso_;    }

      void  setSigIEtaIEta(float newVal)            { sigietaieta_ = newVal;  }
      void  setPassElectronVeto(bool newVal)        { passElectronVeto_ = newVal; }
      void  setFull5x5SigIEtaIEta(float newVal)     { full5x5sigietaieta_ = newVal; }
      void  setHOverE(float newVal)                 { hOverE_ = newVal;   }
      void  setRhoPFChargedHadronIso(float newVal)  { rhoPFchargedHadronIso_ = newVal; }
      void  setRhoPFNeutralHadronIso(float newVal)  { rhoPFneutralHadronIso_ = newVal; }
      void  setRhoPFPhotonIso(float newVal)         { rhoPFphotonIso_ = newVal; }
      void  setTrackIso(float newVal)               { trackiso_ = newVal;   }
      void  setEcalIso(float newVal)                { ecaliso_ = newVal;    }
      void  setHcalIso(float newVal)                { hcaliso_ = newVal;    }
      void  setPFChargedHadronIso(float newVal)     { pfchargedHadronIso_ = newVal;      }
      void  setPFNeutralHadronIso(float newVal)     { pfneutralHadronIso_ = newVal;      }
      void  setPFPhotonIso(float newVal)            { pfphotonIso_ = newVal;      }
      void  setPFPUIso(float newVal)                { pfpuiso_ = newVal;    }

    protected :
      // ID Vars
      float sigietaieta_;
      bool  passElectronVeto_;
      float full5x5sigietaieta_;
      float hOverE_;
      float rhoPFchargedHadronIso_;
      float rhoPFneutralHadronIso_;
      float rhoPFphotonIso_;
      // Iso Vars
      float trackiso_;
      float ecaliso_;
      float hcaliso_;
      float pfchargedHadronIso_;
      float pfneutralHadronIso_;
      float pfphotonIso_;
      float pfpuiso_;

  };

  typedef std::vector<ExtendedPhoton> ExtendedPhotonCollection;

}

#endif /* ANALYSISTOOLS_DATAFORMATS_PHOTON_H */
