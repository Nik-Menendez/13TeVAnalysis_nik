/**
  @file         ParticleInfo.h
  @author       Sue Ann Koay (sakoay@cern.ch)
*/


#ifndef PARTICLEUTILITIES_H
#define PARTICLEUTILITIES_H

#include <vector>
#include <TString.h>

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

namespace ParticleUtilities
{
//_____________________________________________________________________________
// check if second particle is daughter of first
template<typename ParticleA, typename ParticleB>
bool isAncestor(const ParticleA* ancestor, const ParticleB* particle);
//_____________________________________________________________________________
//check particle status
template<typename Particle>
bool isOutgoing(const Particle* particle);

//_____________________________________________________________________________
// Original/Final version of the particle
//_____________________________________________________________________________

/// Traces particle back up the chain of radiation vertices to the first instance of the same particle.
template<typename Particle>
const Particle* getOriginal(const Particle* particle);

/// Traces particle back up the chain of radiation vertices to the first instance of the same particle.
template<typename ParticleRef, typename Container>
ParticleRef getOriginal(ParticleRef particle, const Container& particles);

/// Traces particle down the chain of radiation vertices to the last instance of the same particle.
template<typename Particle>
const Particle* getFinal(const Particle* particle, int maxNumDaughters = -1);

/// Traces particle down the chain of radiation vertices to the last instance of the same particle.
template<typename ParticleRef, typename Container>
ParticleRef getFinal(ParticleRef particle, const Container& particles, int maxNumDaughters = -1);

template<typename Particle, typename OutParticle>
int getOutgoing(const Particle* particle, std::vector<OutParticle>& output, bool (*vetoID)(int) = 0);

template<typename Particle, typename OutParticle>
int getDecayProducts(const Particle* particle, std::vector<OutParticle>& output, bool (*vetoID)(int) = 0, std::vector<OutParticle>* vetoed = 0);

template<typename Particle>
bool isLastInChain(const Particle* particle);

template<typename Particle>
std::vector<const Particle*> getProgenitors(const std::vector<Particle>& particles);

template<typename Particle>
std::vector<const Particle*> getDecayProducts(const std::vector<const Particle*>& progenitors);

//_____________________________________________________________________________
// Find particle decay products
//_____________________________________________________________________________

bool findBosonDaughters(const std::vector<reco::GenParticle> &genParticles, const reco::GenParticle* &boson, const reco::GenParticle* &dau1, const reco::GenParticle* &dau2);

void findTauDaughter(const std::vector<reco::GenParticle> &genParticles, const reco::GenParticle* &tau, const reco::GenParticle* &dau);

//_____________________________________________________________________________
// Classify particles
//_____________________________________________________________________________

struct LesserIDorGreaterPT {
  template<typename ObjectRef>
  bool operator()(const ObjectRef& x, const ObjectRef& y) const 
  {
    const int         xID   = TMath::Abs(x->pdgId());
    const int         yID   = TMath::Abs(y->pdgId());
    if (xID < yID)    return true ;
    if (xID > yID)    return false;
    return x->pt() >= y->pt();
  }
};

template<typename Particle>
TString classifyInitialFlavors(const std::vector<Particle>& genParticles);

template<typename Particle>
TString classifyIncomingFlavors(const std::vector<Particle>& genParticles);

template<typename Particle>
TString classifyOutgoingFlavors(const std::vector<Particle>& genParticles, const std::vector<int>& mePartonIndex, const unsigned int numMEPartons);

template<typename Particle>
TString classifyProduction(const std::vector<Particle>& genParticles, ParticleInfo::JetFlavor special);

template<typename Particle>
int countWithStatus(const std::vector<Particle>& particles, int status, bool (*test)(int) = 0, bool shortCircuit = false);

template<typename Particle>
int countWithID(const std::vector<Particle>& particles, int pdgId, int status = -1, bool checkCharge = false, bool shortCircuit = false);

//template<typename Particle>
//int countProducedID(const std::vector<Particle>& particles, int pdgId, bool checkCharge = false, bool shortCircuit = false, unsigned int firstProduced = 6, unsigned int minNumParents = 2);


//_____________________________________________________________________________
//    Particle Information
//_____________________________________________________________________________

double poleMass(int pdgId);


//_____________________________________________________________________________
//    Print GenParticle history
//_____________________________________________________________________________

/// Prints the history (ancestors and their decays) of the indexed genParticle.
std::ostream& printGenHistory(const std::vector<reco::GenParticle>& genParticles, const unsigned int particleIndex);

/// Prints the entire particle creation/decay history, for the first genBound number of genParticles.
void printGenInfo(const std::vector<reco::GenParticle>& genParticles, int genBound = 30);

};  // end class ParticleInfo


#include "AnalysisTools/Utilities/src/ParticleUtilities.icc"

#endif //PARTICLEINFO_H
