/*
 * CorrectionSet.cc
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */
#include <assert.h>
#include "TFile.h"

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"

namespace ucsbsusy {

Correction::Correction(TString corrName) : name(corrName) {
  std::clog << "Loading correction: "<< name <<std::endl;
}
RefoldCorrection::RefoldCorrection(TString corrName, TFile * file) : Correction(corrName),corr(0) {
  assert(file);
  corr = (const QuickRefold::Refold*)(file->Get(name) );
  assert(corr);
}

CorrectionSet::CorrectionSet() : file(0), options_(0)  {}

void CorrectionSet::loadFile(TString correctionSetName, TString fileName, int correctionOptions) {
  options_ =correctionOptions;
  std::clog << "Loading file: "<< fileName <<" and correctionSet: " << correctionSetName <<std::endl;
  file = TFile::Open(fileName,"read");
  assert(file);
}

CorrectionSet::~CorrectionSet() {
  if(file)
    file->Close();
  delete file;

  for(auto correction : corrections)
    delete correction;
}
} /* namespace ucsbsusy */
