#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/HTTTopJetTagInfo.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "HTTTopJetProducer.h"

using namespace edm;
using namespace cms;
using namespace reco;
using namespace std;

HTTTopJetProducer::HTTTopJetProducer(edm::ParameterSet const& conf):
       FastjetJetProducer( conf ),
       optimalR_(false),
       qJets_(false),
       minFatjetPt_(200.),
       minSubjetPt_(20.),
       minCandPt_(200.),
       maxFatjetAbsEta_(2.5),
       subjetMass_(30.),
       muCut_(0.8),
       filtR_(0.3),
       filtN_(5),       
       mode_(0),
       minCandMass_(150.),
       maxCandMass_(200.),
       massRatioWidth_(15),
       minM23Cut_(0.35),
       minM13Cut_(0.2),
       maxM13Cut_(1.3),
       maxR_(1.5),
       minR_(0.5),
       rejectMinR_(false),
       verbose_(false )
{
  
  // Read in all the options from the configuration
  if ( conf.exists("optimalR") ) 
    optimalR_ = conf.getParameter<bool>("optimalR");

  if ( conf.exists("qJets") ) 
    qJets_ = conf.getParameter<bool>("qJets");

  if ( conf.exists("minFatjetPt") ) 
    minFatjetPt_ = conf.getParameter<double>("minFatjetPt");
  
  if ( conf.exists("minSubjetPt") ) 
    minSubjetPt_ = conf.getParameter<double>("minSubjetPt");
  
  if ( conf.exists("minCandPt") ) 
    minCandPt_ = conf.getParameter<double>("minCandPt");
  
  if ( conf.exists("maxFatjetAbsEta") )
    maxFatjetAbsEta_ = conf.getParameter<double>("maxFatjetAbsEta");
  
  if ( conf.exists("subjetMass") )
    subjetMass_ = conf.getParameter<double>("subjetMass");
  
  if ( conf.exists("muCut") )
    muCut_ = conf.getParameter<double>("muCut");

  if ( conf.exists("filtR") )
    filtR_ = conf.getParameter<double>("filtR");

  if ( conf.exists("filtN") )
    filtN_ = conf.getParameter<int>("filtN");
  
  if ( conf.exists("mode") )
    mode_ = conf.getParameter<int>("mode");
  
  if ( conf.exists("minCandMass") )
    minCandMass_ = conf.getParameter<double>("minCandMass");
  
  if ( conf.exists("maxCandMass") )
    maxCandMass_ = conf.getParameter<double>("maxCandMass");
  
  if ( conf.exists("massRatioWidth") )
    massRatioWidth_ = conf.getParameter<double>("massRatioWidth");
  
  if ( conf.exists("minM23Cut") )
    minM23Cut_ = conf.getParameter<double>("minM23Cut");

  if ( conf.exists("minM13Cut") )
    minM13Cut_ = conf.getParameter<double>("minM13Cut");
  
  if ( conf.exists("maxM13Cut") )
    maxM13Cut_ = conf.getParameter<double>("maxM13Cut");

  if ( conf.exists("maxR") )
    maxR_ = conf.getParameter<double>("maxR");

  if ( conf.exists("minR") )
    minR_ = conf.getParameter<double>("minR");

  if ( conf.exists("rejectMinR") )
    rejectMinR_ = conf.getParameter<bool>("rejectMinR");

  if ( conf.exists("verbose") )
    verbose_ = conf.getParameter<bool>("verbose");
  
  // Create the tagger-wrapper
  produces<HTTTopJetTagInfoCollection>();

  // Signal to the VirtualJetProducer that we have to add HTT information
  fromHTTTopJetProducer_ = 1;
  
  fjHEPTopTagger_ = std::auto_ptr<fastjet::HEPTopTaggerV2>(new fastjet::HEPTopTaggerV2(
										       optimalR_,
										       qJets_,
										       minSubjetPt_, 
										       minCandPt_,
										       subjetMass_, 	    
										       muCut_, 		    
										       filtR_,
										       filtN_,
										       mode_, 		    
										       minCandMass_, 	    
										       maxCandMass_, 	    
										       massRatioWidth_, 	    
										       minM23Cut_, 	    
										       minM13Cut_, 	    
										       maxM13Cut_,
										       rejectMinR_
										       )); 
  
}

		



void HTTTopJetProducer::produce(  edm::Event & e, const edm::EventSetup & c ) 
{

  if (qJets_){
    edm::Service<edm::RandomNumberGenerator> rng;
    CLHEP::HepRandomEngine* engine = &rng->getEngine(e.streamID());
    fjHEPTopTagger_->set_rng(engine);
  }

  FastjetJetProducer::produce(e, c);
}

void HTTTopJetProducer::runAlgorithm( edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  if ( !doAreaFastjet_ && !doRhoFastjet_) {
    fjClusterSeq_ = ClusterSequencePtr( new fastjet::ClusterSequence( fjInputs_, *fjJetDefinition_ ) );
  } else if (voronoiRfact_ <= 0) {
    fjClusterSeq_ = ClusterSequencePtr( new fastjet::ClusterSequenceArea( fjInputs_, *fjJetDefinition_ , *fjAreaDefinition_ ) );
  } else {
    fjClusterSeq_ = ClusterSequencePtr( new fastjet::ClusterSequenceVoronoiArea( fjInputs_, *fjJetDefinition_ , fastjet::VoronoiAreaSpec(voronoiRfact_) ) );
  }

  //Run the jet clustering
  vector<fastjet::PseudoJet> inclusiveJets = fjClusterSeq_->inclusive_jets(minFatjetPt_);

  if ( verbose_ ) cout << "Getting central jets" << endl;
  // Find the transient central jets
  vector<fastjet::PseudoJet> centralJets;
  for (unsigned int i = 0; i < inclusiveJets.size(); i++) {
    
    if (inclusiveJets[i].perp() > minFatjetPt_ && fabs(inclusiveJets[i].rapidity()) < maxFatjetAbsEta_) {
      centralJets.push_back(inclusiveJets[i]);
    }
  }

  fastjet::HEPTopTaggerV2 & HEPTagger = *fjHEPTopTagger_;

  vector<fastjet::PseudoJet>::iterator jetIt = centralJets.begin(), centralJetsEnd = centralJets.end();
  if ( verbose_ )cout<<"Loop over jets"<<endl;
  for ( ; jetIt != centralJetsEnd; ++jetIt ) {
    
    if (verbose_) cout << "CMS FJ jet pt: " << (*jetIt).perp() << endl;
    
    fastjet::PseudoJet taggedJet;

    taggedJet = HEPTagger.result(*jetIt);

    if (taggedJet != 0){
      fjJets_.push_back(taggedJet);           
    }
  }
  
}

void HTTTopJetProducer::addHTTTopJetTagInfoCollection( edm::Event& iEvent, 
						       const edm::EventSetup& iSetup,
						       edm::OrphanHandle<reco::BasicJetCollection> & oh){


  // Set up output list
  auto_ptr<HTTTopJetTagInfoCollection> tagInfos(new HTTTopJetTagInfoCollection() );

  // Loop over jets
  for (size_t ij=0; ij != fjJets_.size(); ij++){

    HTTTopJetProperties properties;
    HTTTopJetTagInfo tagInfo;

    // Black magic:
    // In the standard CA treatment the RefToBase is made from the handle directly
    // Since we only have a OrphanHandle (the JetCollection is created by this process) 
    // we have to take the detour via the Ref
    edm::Ref<reco::BasicJetCollection> ref(oh, ij);  
    edm::RefToBase<reco::Jet> rtb(ref);  
    
    fastjet::HEPTopTaggerV2Structure *s = (fastjet::HEPTopTaggerV2Structure*) fjJets_[ij].structure_non_const_ptr();
      
    properties.fjMass           = s->fj_mass();
    properties.fjPt             = s->fj_pt();
    properties.fjEta            = s->fj_eta();
    properties.fjPhi            = s->fj_phi();
    
    properties.topMass           = s->top_mass();
    properties.unfilteredMass	 = s->unfiltered_mass();
    properties.prunedMass	 = s->pruned_mass();
    properties.fRec		 = s->fRec();
    properties.massRatioPassed   = s->mass_ratio_passed();
    
    properties.Ropt	          = s->Ropt();
    properties.RoptCalc           = s->RoptCalc();
    properties.ptForRoptCalc      = s->ptForRoptCalc();
    
    properties.tau1Unfiltered     = s->Tau1Unfiltered();
    properties.tau2Unfiltered	  = s->Tau2Unfiltered();
    properties.tau3Unfiltered	  = s->Tau3Unfiltered();
    properties.tau1Filtered	  = s->Tau1Filtered();
    properties.tau2Filtered	  = s->Tau2Filtered();
    properties.tau3Filtered	  = s->Tau3Filtered();
    properties.QWeight		  = s->Qweight();
    properties.QEpsilon		  = s->Qepsilon(); 
    properties.QSigmaM            = s->QsigmaM();

    tagInfo.insert(rtb, properties );
    tagInfos->push_back( tagInfo );   
  }

  iEvent.put( tagInfos );
  
};

 
//define this as a plug-in
DEFINE_FWK_MODULE(HTTTopJetProducer);