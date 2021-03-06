//--------------------------------------------------------------------------------------------------
// 
// Jet
// 
// Class to hold basic jet information. To be enhanced as needed.
// 
// Jet.h created on Tue Aug 19 16:26:39 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_JET_H
#define ANALYSISTOOLS_DATAFORMATS_JET_H

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

//_____________________________________________________________________________
// Basic jet type....used for both reco and gen jets
//_____________________________________________________________________________

template <class CoordSystem>
class Jet : public Momentum<CoordSystem>
{
public :
  Jet() : index_(-1) {}

  template <class InputCoordSystem>
  Jet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, const int inIndex = -1) : Momentum<CoordSystem>(inMomentum), index_(inIndex) {}

  ~Jet(){}

  int	index()				const { return index_;	}

  void	setIndex(const int& newIndex)	{ index_ = newIndex;	}

  //----Convenience function for throwing an exception when a member does not exist
  void checkStorage (const void * ptr, std::string message) const {
    if(ptr == 0) throw (message+std::string("The object was never loaded!"));
  }

protected :
  int	index_;  //Index in Jet vector

};

typedef Jet<CylLorentzCoordF> JetF;

template <class CoordSystem>
class GenJet : public Jet<CoordSystem>
{
public :
  GenJet() : flavor_(-1) {}

  template <class InputCoordSystem>
  GenJet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, const int inIndex = -1,const int inFlavor = -1)
  : Jet<CoordSystem>(inMomentum, inIndex),  flavor_(inFlavor){};
  ~GenJet(){}

  int flavor()        const { return flavor_;}
  void  setFlavor(const int inFlavor)           { flavor_ = convertTo<size8>(inFlavor,"GenJet::setFlavor"); }


private:
  size8   flavor_;
};

typedef GenJet<CylLorentzCoordF> GenJetF;
typedef std::vector<GenJetF> GenJetFCollection;

template <class CoordSystem> class RecoJet : public Jet<CoordSystem>
{
public :
    RecoJet() :  csv_(-10), genJet_(0) {}

    template <class InputCoordSystem>
    RecoJet(const ROOT::Math::LorentzVector<InputCoordSystem> &inMomentum,
            const int inIndex = -1,
            const float inCSV = -10, const float inPtRaw = 0, const float inUncert = 0,
            const bool inLooseId = false, GenJet<CoordSystem> *inGenJet = 0)
    // const float inUncert = 0)
        : Jet<CoordSystem>(inMomentum, inIndex), csv_(inCSV), pt_raw_(inPtRaw),
          uncertainty_(inUncert), looseid_(inLooseId), genJet_(inGenJet) {}
    ~RecoJet() {}


    float csv()         const { return csv_;    }
    bool  looseid()     const { return looseid_;}
    float pt_raw()      const { return pt_raw_; }
    float uncertainty() const { return uncertainty_;}
    const GenJet<CoordSystem>  *genJet()        const { return genJet_;  }
    GenJet<CoordSystem>        *genJet()        { return genJet_;  }

    void  setPtr(GenJet<CoordSystem> *inGenJet = 0) { genJet_   = inGenJet; }
    void  setCsv(const float inCsv)               {csv_         = inCsv;    }
    void  setLooseId(const float inLooseId)       {looseid_     = inLooseId;}
    void  setPtRaw(const float inPtRaw )          {pt_raw_      = inPtRaw;  }
    void  setUncertainty(const float inUncert)    {uncertainty_ = inUncert; }
protected :
    float csv_;                     //pointer to csv information
    float pt_raw_;                  //magnitude of uncorrected transverse momentum
    float uncertainty_;             //JEC error magnitude
    bool  looseid_;                 //passes loose jet id or not
    GenJet<CoordSystem>  *genJet_;  //Matched genJet

};
typedef RecoJet<CylLorentzCoordF> RecoJetF;
typedef std::vector<RecoJetF>   RecoJetFCollection;
}
#endif
