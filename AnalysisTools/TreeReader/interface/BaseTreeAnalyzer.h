//--------------------------------------------------------------------------------------------------
// 
// BaseTreeAnalyzer
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_BASETREEREADER_H
#define ANALYSISTOOLS_TREEREADER_BASETREEREADER_H

#include <string>
#include <vector>
#include <assert.h>
#include <TString.h>
#include <TTree.h>

#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;

namespace ucsbsusy {

  class BaseTreeAnalyzer{
  public:
    enum VarType {AK4JETS, ELECTRONS, MUONS, TAUS};

    BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree = false, TString readOption = "READ");
    ~BaseTreeAnalyzer() {};

    //Load a variable type to be read from the TTree
    //use the defaultOptions if options is less than 1
    //use the default branch name prefix if set to an empty string
    void load(VarType type, int options = -1, string branchName = "" );
    //same as above but for non-default readers
    void load(BaseReader * inReader, int options, string branchName) {reader.load(inReader,options,branchName);}

    //load the next event
    bool nextEvent(bool verbose = false);

    //get event number
    int getEventNumber() const {return reader.eventNumber;}

  private:
    TreeReader reader; //default reader
  public:
    const bool    isMC;

    //All default readers
    JetReader      ak4Reader     ;
    ElectronReader electronReader;
    MuonReader     muonReader    ;
    TauReader      tauReader     ;

    //Pointers to default objects (for ease of access)
    RecoJetFCollection * ak4Jets;
    GenJetFCollection  * ak4GenJets;
    ElectronFCollection* electrons;
    MuonFCollection    * muons;
    TauFCollection     * taus;
  };
}

#endif
