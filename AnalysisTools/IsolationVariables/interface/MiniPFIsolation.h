/*
 * MiniPFIsolation.h
 *
 * Calculate the mini PFIsolation, and the local annulus activity variable used by MT2 search [AN2015-009]
 *
 *  Created on: Sep 23, 2015
 *      Author: hqu
 */

#ifndef ANALYSISTOOLS_ISOLATIONVARIABLES_MINIPFISOLATION_H_
#define ANALYSISTOOLS_ISOLATIONVARIABLES_MINIPFISOLATION_H_

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

class MiniPFIsolation{
public:
  MiniPFIsolation(double minIsoConeSize, double maxIsoConeSize, double ktScale, double activityConeSize) :
    minIsoConeSize2_(minIsoConeSize*minIsoConeSize),
    maxIsoConeSize2_(maxIsoConeSize*maxIsoConeSize),
    ktScale_(ktScale),
    actConeSize2_(activityConeSize*activityConeSize) {}

  virtual ~MiniPFIsolation() {}

  void setDeadCone(double charged, double neutral, double photon, double pu){
    chargedDeadConeEB2_ = charged*charged; neutralDeadConeEB2_ = neutral*neutral; photonDeadConeEB2_ = photon*photon; puDeadConeEB2_ = pu*pu;
  }
  void setDeadConeEE(double charged, double neutral, double photon, double pu){
    chargedDeadConeEE2_ = charged*charged; neutralDeadConeEE2_ = neutral*neutral; photonDeadConeEE2_ = photon*photon; puDeadConeEE2_ = pu*pu;
  }
  void setObjectMinPt(double obj_min_pt) { minPt_ = obj_min_pt; }
  void setPtThreshold(double pt_threshold) { ptThreshold_ = pt_threshold; }

  double getMiniIso() const { return miniIso_; }
  double getActivity() const { return activity_; }

  virtual void compute(const reco::Candidate& obj, const pat::PackedCandidateCollection& pfcands);

protected:
  virtual bool isEndCap(const reco::Candidate& obj);
  virtual void reset() {
    isoConeSize2_ = 0; miniIso_ = 0; activity_ = 0;
    chargedIso_ = 0; neutralIso_ = 0; photonIso_ = 0; puIso_ = 0;
    chargedAct_ = 0; neutralAct_ = 0; photonAct_ = 0; puAct_ = 0;
  }
  virtual void calcIsoConeSize(double pt) { isoConeSize2_ = std::max(minIsoConeSize2_, std::min(maxIsoConeSize2_, std::pow(ktScale_/pt,2))); }
  virtual double calcPFWeight(const pat::PackedCandidate& pfc, const pat::PackedCandidateCollection& pfcands);
  virtual void addToIsolation(const pat::PackedCandidate& pfc, const pat::PackedCandidateCollection& pfcands, double deltaR2);
  virtual void addToActivity(const pat::PackedCandidate& pfc);

private:
  double minIsoConeSize2_, maxIsoConeSize2_, ktScale_;
  double actConeSize2_;
  double chargedDeadConeEB2_ = 0, neutralDeadConeEB2_ = 0, photonDeadConeEB2_ = 0, puDeadConeEB2_ = 0;
  double chargedDeadConeEE2_ = 0, neutralDeadConeEE2_ = 0, photonDeadConeEE2_ = 0, puDeadConeEE2_ = 0;
  double minPt_ = 5;
  double ptThreshold_ = 0;
  bool   usePFWeight_ = false;
  bool   chargedOnly_ = false;

  // reset per calculation

  double deadcone2_nh = 0, deadcone2_ch = 0, deadcone2_ph = 0, deadcone2_pu = 0;

  double miniIso_ = 0;
  double activity_ = 0;

  double chargedIso_ = 0, neutralIso_ = 0, photonIso_ = 0, puIso_ = 0;
  double chargedAct_ = 0, neutralAct_ = 0, photonAct_ = 0, puAct_ = 0;
  double isoConeSize2_ = 0;
};


namespace Isolation{
inline double calcDeltaBetaIso(double chargediso, double neutraliso, double photoniso, double puiso){
  return chargediso + std::max(neutraliso + photoniso - 0.5*puiso, 0.);
}
}

#endif /* ANALYSISTOOLS_ISOLATIONVARIABLES_MINIPFISOLATION_H_ */
