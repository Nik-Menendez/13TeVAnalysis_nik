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

TString corrTypeName (CORRTYPE type) {
  switch(type){
    case NOMINAL:
      return "NOMINAL";
    case UP:
      return "UP";
    case DOWN:
      return "DOWN";
    case NONE:
      return "NONE";
    default:
      return "??";
  }
}

Correction::Correction(TString corrName) : name(corrName) {
  std::clog << "Loading correction: "<< name <<std::endl;
}
RefoldCorrection::RefoldCorrection(TString corrName, TFile * file) : Correction(corrName),corr(0) {
  if(!file) throw std::invalid_argument("RefoldCorrection::RefoldCorrection: file could not be found!");
  corr = (const QuickRefold::Refold*)(file->Get(name) );
  if(!corr) throw std::invalid_argument("RefoldCorrection::RefoldCorrection: Corrector could not be found!");
}

HistogramCorrection::HistogramCorrection(TString corrName, TFile * file) : Correction(corrName),targetBin(1) {
  if(!file) throw std::invalid_argument("HistogramCorrection::HistogramCorrection: file could not be found!");
  corrHist = (TH1F*)(file->Get(name) );
  if(!corrHist) throw std::invalid_argument("HistogramCorrection::HistogramCorrection: Histogram could not be found!");
}

LepHistogramCorrection::LepHistogramCorrection(TString corrName, TString tnpElFileName, TString tnpMuFileName) : Correction(corrName)
{
  std::clog << "Loading files: "<< tnpElFileName << ", " << tnpElFileName <<" and correctionSet: " << corrName <<std::endl;
  fileEl = TFile::Open(tnpElFileName,"read");
  fileMu = TFile::Open(tnpMuFileName,"read");
  if(!fileEl) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: el file could not be found!");
  if(!fileMu) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: mu file could not be found!");
  corrHistEl = (TH2F*)(fileEl->Get("TNPEL") );
  corrHistMu = (TH2F*)(fileMu->Get("TNPMU") );
  if(!corrHistEl) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: el histogram could not be found!");
  if(!corrHistMu) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: mu histogram could not be found!");
}

LepHistogramCorrection::~LepHistogramCorrection() {
  if(fileEl) fileEl->Close();
  if(fileMu) fileMu->Close();
  delete fileEl;
  delete fileMu;
}

CorrectionSet::CorrectionSet() : file(0), options_(0)  {}

void CorrectionSet::loadSimple(TString correctionSetName, int correctionOptions) {
  options_ =correctionOptions;
  std::clog << "Loading correctionSet: " << correctionSetName <<std::endl;
  return;

}

void CorrectionSet::loadFile(TString correctionSetName, TString fileName, int correctionOptions) {
  options_ |=correctionOptions;

  std::clog << "Loading file: "<< fileName <<" and correctionSet: " << correctionSetName <<std::endl;
  file = TFile::Open(fileName,"read");
  if(!file) throw std::invalid_argument("CorrectionSet::loadFile: File could not be found!");
}

CorrectionSet::~CorrectionSet() {
  if(file)
    file->Close();
  delete file;

  for(auto correction : corrections)
    delete correction;
}
} /* namespace ucsbsusy */
