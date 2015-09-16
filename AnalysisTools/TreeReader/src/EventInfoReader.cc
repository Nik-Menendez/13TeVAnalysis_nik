//--------------------------------------------------------------------------------------------------
// 
// EventInfoReader
// 
// Class for reading event level information from TTree.
// 
// EventInfoReader.cc created on Mon Nov 3 12:56:14 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/EventInfoReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
using namespace ucsbsusy;

EventInfoReader::EventInfoReader()
{

  run = 0;
  lumi = 0;
  event = 0;
  nPV = 0;
  rho = 0;
  pvx = 0;
  pvy = 0;
  pvz = 0;
  met_pt = 0;
  met_phi = 0;
  metsumEt = 0;
  metNoHF_pt = 0;
  metNoHF_phi = 0;
  metNoHF_sumEt = 0;
  genmet_pt = 0;
  genmet_phi = 0;
  goodvertex = false;
  genweight = 1;
  genqscale = 0;
  xsecweight = 1;
  genevtweight = 1;
  evtweight = 1;
  proc = 0;
  process = defaults::NUMPROCESSES;
  datrec = 0;
  datareco = defaults::MC;
  trigbitflags = new vector<unsigned long>;
  trigbitpass = new vector<bool>;
  trigbitprescale = new vector<unsigned int>;
  triggerflag = 0;
  metfilterbitflags = new vector<unsigned long>;
  metfilterbitpass = new vector<bool>;
  hbheHNFlt  = new vector<bool>;
  hbheFixFlt = false;
  hbheFlt    = false;
  cscFlt     = false;
  eeBadSCFlt = false;

}

void EventInfoReader::load(TreeReader *treeReader, int options, string branchName)
{
  clog << "Loading (" << branchName << ") event info with: ";

  loaded_ = true;
  treeReader->setBranchAddress(branchName,"run", &run);
  treeReader->setBranchAddress(branchName,"lumi", &lumi);
  treeReader->setBranchAddress(branchName,"event", &event);
  treeReader->setBranchAddress(branchName,"npv", &nPV);
  treeReader->setBranchAddress(branchName,"rho", &rho);
  treeReader->setBranchAddress(branchName,"pv_x", &pvx);
  treeReader->setBranchAddress(branchName,"pv_y", &pvy);
  treeReader->setBranchAddress(branchName,"pv_z", &pvz);
  treeReader->setBranchAddress(branchName,"met_pt", &met_pt);
  treeReader->setBranchAddress(branchName,"met_phi", &met_phi);
  treeReader->setBranchAddress(branchName,"met_sumEt", &metsumEt);
  treeReader->setBranchAddress(branchName,"metnohf_pt", &metNoHF_pt);
  treeReader->setBranchAddress(branchName,"metnohf_phi", &metNoHF_phi);
  treeReader->setBranchAddress(branchName,"metnohf_sumEt", &metNoHF_sumEt);
  treeReader->setBranchAddress(branchName,"genmet_pt", &genmet_pt);
  treeReader->setBranchAddress(branchName,"genmet_phi", &genmet_phi);
  treeReader->setBranchAddress(branchName,"goodvertex", &goodvertex);
  treeReader->setBranchAddress(branchName,"genweight", &genweight);
  treeReader->setBranchAddress(branchName,"genqscale", &genqscale);
  treeReader->setBranchAddress(defaults::BRANCH_TRIGGERS,"bit_flag", &trigbitflags);
  treeReader->setBranchAddress(defaults::BRANCH_TRIGGERS,"bit_pass", &trigbitpass);
  treeReader->setBranchAddress(defaults::BRANCH_TRIGGERS,"bit_prescale", &trigbitprescale);
  treeReader->setBranchAddress(branchName,"process", &proc);
  treeReader->setBranchAddress(branchName,"datareco", &datrec);
  treeReader->setBranchAddress(branchName,"wgtXSec", &xsecweight);
  treeReader->setBranchAddress(branchName,"evtWgtGen", &genevtweight);
  treeReader->setBranchAddress(defaults::BRANCH_METFILTERS,"bit_flag", &metfilterbitflags);
  treeReader->setBranchAddress(defaults::BRANCH_METFILTERS,"bit_pass", &metfilterbitpass);
  treeReader->setBranchAddress(defaults::BRANCH_METFILTERS,"hbheFilterFix", &hbheHNFlt);

  clog << endl;
}

void EventInfoReader::refresh()
{
  triggerflag = 0;
  met.setP4(CylLorentzVectorF(met_pt,0,met_phi,0));
  //met.setP4(CylLorentzVectorF(metNoHF_pt,0,metNoHF_phi,0));
  metNoHF.setP4(CylLorentzVectorF(metNoHF_pt,0,metNoHF_phi,0));
  genmet.setP4(CylLorentzVectorF(genmet_pt,0,genmet_phi,0));
  process = static_cast<defaults::Process>(proc);
  datareco = static_cast<defaults::DataReco>(datrec);
  evtweight = xsecweight * genevtweight;

  for (unsigned int i = 0; i < trigbitflags->size(); ++i) {
    if(trigbitpass->at(i)) triggerflag |= trigbitflags->at(i);
  }

  cscFlt     = false;
  eeBadSCFlt = false;
  hbheFlt    = false;
  hbheFixFlt = false;

  //  if (hbheHNFlt->size()>0)
  hbheFixFlt = hbheHNFlt->at(0); 
  for (unsigned int i = 0; i < metfilterbitflags->size(); ++i) {

    if (i==2)  { cscFlt     = metfilterbitpass->at(i); }
    if (i==8)  { eeBadSCFlt = metfilterbitpass->at(i); }
    if (i==10) { hbheFlt    = metfilterbitpass->at(i); }

  }

}
