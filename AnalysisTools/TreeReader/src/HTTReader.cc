
#include "AnalysisTools/TreeReader/interface/HTTReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;

const int HTTReader::defaultOptions = HTTReader::LOADRECO | HTTReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
HTTReader::HTTReader() : BaseReader(){

  pt              = new std::vector<float>;
  eta             = new std::vector<float>;
  phi             = new std::vector<float>;
  mass            = new std::vector<float>;
  csv             = new std::vector<float>;
  ropt            = new std::vector<float>;
  frec            = new std::vector<float>;
  roptcalc        = new std::vector<float>;
  ptforropt       = new std::vector<float>;
  ropt_pt         = new std::vector<float>;
  ropt_eta        = new std::vector<float>;
  ropt_phi        = new std::vector<float>;
  ropt_mass       = new std::vector<float>;
  ropt_tau1       = new std::vector<float>;
  ropt_tau2       = new std::vector<float>;
  ropt_tau3       = new std::vector<float>;
  sd_mass         = new std::vector<float>;
  sd_tau1         = new std::vector<float>;
  sd_tau2         = new std::vector<float>;
  sd_tau3         = new std::vector<float>;
  nsubjets        = new std::vector<int  >;
  w1_pt           = new std::vector<float>;
  w1_eta          = new std::vector<float>;
  w1_phi          = new std::vector<float>;
  w1_mass         = new std::vector<float>;
  w1_csv          = new std::vector<float>;
  w2_pt           = new std::vector<float>;
  w2_eta          = new std::vector<float>;
  w2_phi          = new std::vector<float>;
  w2_mass         = new std::vector<float>;
  w2_csv          = new std::vector<float>;
  b_pt            = new std::vector<float>;
  b_eta           = new std::vector<float>;
  b_phi           = new std::vector<float>;
  b_mass          = new std::vector<float>;
  b_csv           = new std::vector<float>;


}

//--------------------------------------------------------------------------------------------------
void HTTReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") tops with: ";

    if(options_ & LOADRECO) {
      clog << " +Reco ";


      treeReader->setBranchAddress(branchName_,"fatjet_pt"       ,&pt       ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_eta"      ,&eta      ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_phi"      ,&phi      ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_mass"     ,&mass     ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_csv"      ,&csv      ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_ropt"     ,&ropt     ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_frec"     ,&frec     ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_roptcalc" ,&roptcalc ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_ptforropt",&ptforropt,true);
      treeReader->setBranchAddress(branchName_,"fatjet_ropt_pt"  ,&ropt_pt  ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_ropt_eta" ,&ropt_eta ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_ropt_phi" ,&ropt_phi ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_ropt_mass",&ropt_mass,true);
      treeReader->setBranchAddress(branchName_,"fatjet_ropt_tau1",&ropt_tau1,true);
      treeReader->setBranchAddress(branchName_,"fatjet_ropt_tau2",&ropt_tau2,true);
      treeReader->setBranchAddress(branchName_,"fatjet_ropt_tau3",&ropt_tau3,true);
      treeReader->setBranchAddress(branchName_,"fatjet_sd_mass"  ,&sd_mass  ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_sd_tau1"  ,&sd_tau1  ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_sd_tau2"  ,&sd_tau2  ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_sd_tau3"  ,&sd_tau3  ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_nsubjets" ,&nsubjets ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_w1_pt"    ,&w1_pt    ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_w1_eta"   ,&w1_eta   ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_w1_phi"   ,&w1_phi   ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_w1_mass"  ,&w1_mass  ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_w1_csv"   ,&w1_csv   ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_w2_pt"    ,&w2_pt    ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_w2_eta"   ,&w2_eta   ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_w2_phi"   ,&w2_phi   ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_w2_mass"  ,&w2_mass  ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_w2_csv"   ,&w2_csv   ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_b_pt"     ,&b_pt     ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_b_eta"    ,&b_eta    ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_b_phi"    ,&b_phi    ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_b_mass"   ,&b_mass   ,true);
      treeReader->setBranchAddress(branchName_,"fatjet_b_csv"    ,&b_csv    ,true);
    }
    if(options_ & FILLOBJ)
      clog << " +Objects ";
    clog << endl;
}

//--------------------------------------------------------------------------------------------------
void HTTReader::refresh(){

  if(!(options_ & FILLOBJ)) return;
  if(!(options_ & LOADRECO)) return;

  fatJets.clear();
  fatJets.reserve(pt->size());

  for(unsigned int iJ = 0; iJ < pt->size(); ++iJ){



    fatJets.emplace_back(CylLorentzVectorF(pt->at(iJ),eta->at(iJ),phi->at(iJ),mass->at(iJ)),
        iJ, 0,0,sd_mass->at(iJ),sd_tau1->at(iJ),sd_tau2->at(iJ),sd_tau3->at(iJ));


    if(nsubjets->at(iJ) >= 3 ){
      fatJets.back().addSubjet(CylLorentzVectorF(w1_pt->at(iJ),w1_eta->at(iJ),w1_phi->at(iJ),w1_mass->at(iJ)),
          w1_csv->at(iJ));
      fatJets.back().addSubjet(CylLorentzVectorF(w2_pt->at(iJ),w2_eta->at(iJ),w2_phi->at(iJ),w2_mass->at(iJ)),
          w2_csv->at(iJ));
      fatJets.back().addSubjet(CylLorentzVectorF(b_pt->at(iJ),b_eta->at(iJ),b_phi->at(iJ),b_mass->at(iJ)),
          b_csv->at(iJ));
    }

    fatJets.back().addROptInfo(CylLorentzVectorF(ropt_pt->at(iJ),ropt_eta->at(iJ),ropt_phi->at(iJ),ropt_mass->at(iJ)),
        ropt->at(iJ),frec->at(iJ),roptcalc->at(iJ),ptforropt->at(iJ),ropt_tau1->at(iJ),ropt_tau2->at(iJ),ropt_tau3->at(iJ));

  } 
  std::sort(fatJets.begin(),fatJets.end(),PhysicsUtilities::greaterPT<FatJetF>());

}
