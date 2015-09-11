/*
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_EVENTCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_EVENTCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
namespace ucsbsusy {

class PUCorr50NS : public RefoldCorrection {
public:
  enum axes {NPV,INPUT_MC};
  PUCorr50NS(TFile * file) : RefoldCorrection("PU50NS",file) {}
};

class PUCorr : public RefoldCorrection {
public:
  enum axes {NPV};
  PUCorr(TFile * file) : RefoldCorrection("PU",file) {}
};

class EventCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT         = 0
                          , PU              = (1 <<  0)   ///< Correct PU
  };
  EventCorrectionSet(): puCorr50NS(0), puCorr(0), puWeight(1), pu50NSWeight(1){}
  virtual ~EventCorrectionSet() {};
  virtual void load(TString fileName, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getPUWeight() const {return puWeight;}
  float get50NSPUWeight() const {return pu50NSWeight;}

private:
  //Correction list
  PUCorr50NS * puCorr50NS;
  PUCorr * puCorr;

  //output values
  float puWeight;
  float pu50NSWeight;
};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
