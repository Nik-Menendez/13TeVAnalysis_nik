#ifndef __FASTJETCLUSTERER_H__
#define __FASTJETCLUSTERER_H__


#include <vector>
#include <string>
#include <memory>

#include <TString.h>

#include <fastjet/PseudoJet.hh>
#include <fastjet/JetDefinition.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/ClusterSequenceArea.hh>

#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/JetReco/interface/Jet.h"
namespace pat{
class Jet;
}


namespace ucsbsusy {
//=============================================================================
class FastJetClusterer
{
/*************************/
/*   Supporting Types    */
/*************************/
public:
  enum PRTTYPE {RECO,GEN,PARTON,PU};
  class UserInfo : public fastjet::PseudoJet::UserInfoBase, public reco::CandidatePtr {
  public:
    PRTTYPE type;
    template<typename ParticleCollection>
    UserInfo(const ParticleCollection& inputParticles, const unsigned int index, const PRTTYPE type_)
      : reco::CandidatePtr(inputParticles, index), type(type_)
    { }
  };
  
  class CompoundPseudoJet : public fastjet::PseudoJet {
  public :
    CompoundPseudoJet(const fastjet::PseudoJet & jet, const std::vector<fastjet::PseudoJet> & constituents, const int superjetindex) : fastjet::PseudoJet(jet), constituents_(constituents.size()), superjetindex_(superjetindex) {
      std::copy(constituents.begin(), constituents.end(), constituents_.begin());
    }

    ~CompoundPseudoJet() {}

    std::vector<fastjet::PseudoJet>  constituents () const { return constituents_;  }
    int                              superjetIndex() const { return superjetindex_; }

  protected :
    std::vector<fastjet::PseudoJet>  constituents_;
    int                              superjetindex_;

  };
  
  static const reco::Jet::Point                             DEFAULT_VERTEX;

  
/*************************/
/*      Data Members     */
/*************************/
protected:
  static double                                             currentGhostArea    ;
  bool                                                      computeArea         ;
  bool                                                      explicitGhosts      ;

  std::vector<int>                                          randomSeeds         ;

  std::shared_ptr<fastjet::JetDefinition>                   jetDefinition       ;
  std::shared_ptr<fastjet::ClusterSequence>                 clusterSequence     ;
  std::shared_ptr<fastjet::JetDefinition>                   subJetDefinition    ;
  std::vector<std::shared_ptr<fastjet::ClusterSequence> >   subClusterSequences ;
  std::vector<fastjet::PseudoJet>                           subClusterOrigins   ;

  std::vector<fastjet::PseudoJet>                           particles           ;
  std::vector<fastjet::PseudoJet>                           jets                ;
  std::vector<fastjet::PseudoJet>                           superJets           ;
  std::vector<CompoundPseudoJet>                            compoundJets        ;

  std::vector<unsigned int>                                 nSubJetsCA          ;


/*************************/
/*       Interface       */
/*************************/
public:
  FastJetClusterer(bool computeArea = false, bool explicitGhosts = false, const std::vector<int> * inRandomSeeds = 0)
    : computeArea     (computeArea   )
    , explicitGhosts  (explicitGhosts)
    , randomSeeds     (inRandomSeeds ? *inRandomSeeds : std::vector<int>())
  { }
  FastJetClusterer(const FastJetClusterer& other);
  FastJetClusterer& operator=(const FastJetClusterer& other);

  virtual ~FastJetClusterer();
  template<typename Particle>
  void addParticles ( const edm::Handle<std::vector<Particle> >& inputParticles
      , PRTTYPE                       type
      , int                           status        = -1
      , double                        minInputPT    = 0
      , double                        maxInputEta   = 5
      , bool                          (*select)(const Particle&) = 0
      , std::vector<bool>*            vetoes        = 0
      , double                        scale         = 1
                    );

  const std::vector<fastjet::PseudoJet>&  getParticles() const  { return particles; }
  const std::vector<fastjet::PseudoJet>&  getJets     () const  { return jets     ; }
  std::vector<fastjet::PseudoJet>&        getJets     ()        { return jets     ; }
  const std::vector<CompoundPseudoJet>&   getCompoundJets () const  { return compoundJets; }
  std::vector<CompoundPseudoJet>&         getCompoundJets ()        { return compoundJets; }
  const std::vector<fastjet::PseudoJet>&  getSuperJets    () const  { return superJets     ; }
  std::vector<fastjet::PseudoJet>&        getSuperJets    ()        { return superJets     ; }
  void                                    sortJets    ()        { jets  = fastjet::sorted_by_pt(jets); }

  template<typename Particle, typename Jet>
  static void     distillJets ( const edm::Handle<std::vector<Particle> >&  inputParticles
                              , const std::vector<CompoundPseudoJet>&       inputJets
                              , std::vector<Jet>&                           outputJets
                              , const edm::EventSetup&                      eventSetup
                              , bool                                        (*select)(const Particle&) = 0
                              , bool                                        keepEmptyJets = false
                              , bool                                        recomputeP4   = true
                              , const reco::Jet::Point&                     vertex        = DEFAULT_VERTEX
                              , std::vector<int>&                           superJetIndices = *(new std::vector<int>())
                              );
  template<typename Particle, typename InputJet, typename Jet>
  static void     distillJets ( const edm::Handle<std::vector<Particle> >&  inputParticles
                              , const std::vector<InputJet>&                inputJets
                              , std::vector<Jet>&                           outputJets
                              , const edm::EventSetup&                      eventSetup
                              , bool                                        (*select)(const Particle&) = 0
                              , bool                                        keepEmptyJets = false
                              , bool                                        recomputeP4   = true
                              , const reco::Jet::Point&                     vertex        = DEFAULT_VERTEX
                              , std::vector<int>&                           superJetIndices = *(new std::vector<int>())
                              );
  template<typename Particle, typename Jet>
  void            distillJets ( const edm::Handle<std::vector<Particle> >&  inputParticles
                              , std::vector<Jet>&                           outputJets
                              , const edm::EventSetup&                      eventSetup
                              , bool                          (*select)(const Particle&) = 0
                              , bool                                        keepEmptyJets = false
                              , bool                                        recomputeP4   = true
                              , const reco::Jet::Point&                     vertex        = DEFAULT_VERTEX
                              , std::vector<int>&                           superJetIndices = *(new std::vector<int>())
                              ) const
  {
    FastJetClusterer::distillJets( inputParticles,jets, outputJets, eventSetup,select, keepEmptyJets, recomputeP4, vertex, superJetIndices );
  }
  template<typename Particle, typename Jet>
  void            distillSuperJets ( const edm::Handle<std::vector<Particle> >&  inputParticles
                              , std::vector<Jet>&                           outputJets
                              , const edm::EventSetup&                      eventSetup
                              , bool                          (*select)(const Particle&) = 0
                              , bool                                        keepEmptyJets = false
                              , bool                                        recomputeP4   = true
                              , const reco::Jet::Point&                     vertex        = DEFAULT_VERTEX
                              ) const
  {
    FastJetClusterer::distillJets( inputParticles,superJets, outputJets, eventSetup,select, keepEmptyJets, recomputeP4, vertex );
  }
  
  static double   getCurrentGhostArea()   { return currentGhostArea;  }
  void setDeterministicSeed(const unsigned int runNumber, const unsigned int eventNumber);

  std::vector<unsigned int> getNSubjetsCA() { return nSubJetsCA; }

/*************************/
/*      Computations     */
/*************************/
public:

  void    clusterJets       ( const fastjet::JetDefinition&     definition
                            , double                    minJetPT          = 0
                            , double                    maxGhostEta       = 5
                            , double                    ghostArea         = fastjet::gas::def_ghost_area
                            , double                    meanGhostPT       = fastjet::gas::def_mean_ghost_pt
                            , int                       numAreaRepeats    = 1 //fastjet::gas::def_repeat
                            , double                    ghostGridScatter  = fastjet::gas::def_grid_scatter
                            , double                    ghostPTScatter    = fastjet::gas::def_pt_scatter
                            );
  void    clusterJets       ( fastjet::JetAlgorithm     algorithm
                            , double                    dCut 
                            , double                    minJetPT          = 0
                            , double                    maxGhostEta       = 5
                            , double                    ghostArea         = fastjet::gas::def_ghost_area
                            , double                    meanGhostPT       = fastjet::gas::def_mean_ghost_pt
                            , int                       numAreaRepeats    = 1//fastjet::gas::def_repeat
                            , double                    ghostGridScatter  = fastjet::gas::def_grid_scatter
                            , double                    ghostPTScatter    = fastjet::gas::def_pt_scatter
                            );
  void    clusterJets       ( fastjet::JetDefinition::Plugin*   plugin
                            , double                    minJetPT          = 0
                            , double                    maxGhostEta       = 5
                            , double                    ghostArea         = fastjet::gas::def_ghost_area
                            , double                    meanGhostPT       = fastjet::gas::def_mean_ghost_pt
                            , int                       numAreaRepeats    = 1//fastjet::gas::def_repeat
                            , double                    ghostGridScatter  = fastjet::gas::def_grid_scatter
                            , double                    ghostPTScatter    = fastjet::gas::def_pt_scatter
                            );
  void    storeSuperJets   () {superJets = jets;}
  void    trimJets         (const double rFilter, double trimPtFracMin, bool useTrimmedSubjets);
  void    applySubjetCountingCA(const double mass_cut_off, const double ycut, const double R_min, const double pt_cut);

  void    selectJets        ( double                    minJetPT 
                            , double                    maxJetEta
                            );
  template<typename Jet>
  void    selectJets        ( double                    minJetPT 
                            , double                    maxJetEta
                            , std::vector<Jet>&         satellites
                            );

  template<typename Splitter>
  void    pickySubjets      ( const Splitter*          splitter
                            , int                       maxSplits   = 4
                            );
/*************************/
/*   Helper Functions    */
/*************************/
public:
  static const TString&   fastJetAlgoName(fastjet::JetAlgorithm algorithm);
  void         findHardSubst(const fastjet::PseudoJet & this_jet, std::vector<fastjet::PseudoJet> & t_parts, const double mCutoff, const double rMin, const double yCut) const;
};  // end class FastJetClusterer
}

namespace reco
{
/// @todo   For now, assumes that all jets are of PF type.
void writeSpecific( pat::Jet& jet, reco::Particle::LorentzVector const & p4, reco::Particle::Point const & point
                  , std::vector<reco::CandidatePtr> const & constituents, edm::EventSetup const & c
                  );
}

#include "ObjectProducers/JetProducers/src/FastJetClusterer.icc"


#endif //__FASTJETCLUSTERER_H__
