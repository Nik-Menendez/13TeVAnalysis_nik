//--------------------------------------------------------------------------------------------------
// 
// TauMVA
// 
// Class to compute discriminator value for hadronic tau candidates.
// 
// TauMVA.cc created on Wed Jan 28 03:33:04 CET 2015 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/ObjectSelection/interface/TauMVA.h"

using namespace ucsbsusy;

TauMVA::TauMVA(TString mvafileName, TString mvaName) :
  numParameters(0),
  parIndex_pt(-1),
  index_pt(-1),
  index_abseta(-1),
  index_absdz(-1),
  index_chreliso0p1(-1),
  index_chreliso0p2(-1),
  index_chreliso0p3(-1),
  index_chreliso0p4(-1),
  index_totreliso0p1(-1),
  index_totreliso0p2(-1),
  index_totreliso0p3(-1),
  index_totreliso0p4(-1),
  index_nearesttrkdr(-1),
  index_contjetdr(-1),
  index_contjetcsv(-1)
{

  TFile* infile = TFile::Open(mvafileName, "READ");
  assert(infile);

  mvaPar = dynamic_cast<ParamatrixMVA*>(infile->Get(mvaName));
  assert(mvaPar);

  delete infile;

  std::clog << "Loading Tau MVA: " << mvafileName << " (" << mvaName << ")" << std::endl;
  mvaPar->sitrep();

  const_cast<int&>(numParameters)      = mvaPar->getNumParams();

  const_cast<int&>(parIndex_pt)        = mvaPar->findAxis("pt"); assert(parIndex_pt > -1);

  const auto *mva = mvaPar->getOne();

  const_cast<int&>(index_pt)           = mva->findVariable("pt"          ); assert(index_pt > -1);
  const_cast<int&>(index_abseta)       = mva->findVariable("abseta"      ); assert(index_abseta > -1);
  const_cast<int&>(index_absdz)        = mva->findVariable("absdz"       ); assert(index_absdz > -1);
  const_cast<int&>(index_chreliso0p1)  = mva->findVariable("chreliso0p1" ); assert(index_chreliso0p1 > -1);
  const_cast<int&>(index_chreliso0p2)  = mva->findVariable("chreliso0p2" ); assert(index_chreliso0p2 > -1);
  const_cast<int&>(index_chreliso0p3)  = mva->findVariable("chreliso0p3" ); assert(index_chreliso0p3 > -1);
  const_cast<int&>(index_chreliso0p4)  = mva->findVariable("chreliso0p4" ); assert(index_chreliso0p4 > -1);
  const_cast<int&>(index_totreliso0p1) = mva->findVariable("totreliso0p1"); assert(index_totreliso0p1 > -1);
  const_cast<int&>(index_totreliso0p2) = mva->findVariable("totreliso0p2"); assert(index_totreliso0p2 > -1);
  const_cast<int&>(index_totreliso0p3) = mva->findVariable("totreliso0p3"); assert(index_totreliso0p3 > -1);
  const_cast<int&>(index_totreliso0p4) = mva->findVariable("totreliso0p4"); assert(index_totreliso0p4 > -1);
  const_cast<int&>(index_nearesttrkdr) = mva->findVariable("neartrkdr"   ); assert(index_nearesttrkdr > -1);
  const_cast<int&>(index_contjetdr)    = mva->findVariable("contjetdr"   ); assert(index_contjetdr > -1);
  const_cast<int&>(index_contjetcsv)   = mva->findVariable("contjetcsv"  ); assert(index_contjetcsv > -1);

}

double TauMVA::evaluateMVA(float pt, float eta, float dz, float chiso0p1, float chiso0p2, float chiso0p3, float chiso0p4, float totiso0p1, float totiso0p2, float totiso0p3, float totiso0p4, float nearesttrkdr, float contjetdr, float contjetcsv)
{

  std::vector<double> parameters(numParameters);

  parameters[parIndex_pt] = pt;

  const Panvariate* mvaReader = mvaPar->get(parameters);

  assert(mvaReader);

  mvaReader->setVariable(index_pt, pt);
  mvaReader->setVariable(index_abseta, fabs(eta));
  mvaReader->setVariable(index_absdz, fabs(dz));
  mvaReader->setVariable(index_chreliso0p1, chiso0p1/pt);
  mvaReader->setVariable(index_chreliso0p2, chiso0p2/pt);
  mvaReader->setVariable(index_chreliso0p3, chiso0p3/pt);
  mvaReader->setVariable(index_chreliso0p4, chiso0p4/pt);
  mvaReader->setVariable(index_totreliso0p1, totiso0p1/pt);
  mvaReader->setVariable(index_totreliso0p2, totiso0p2/pt);
  mvaReader->setVariable(index_totreliso0p3, totiso0p3/pt);
  mvaReader->setVariable(index_totreliso0p4, totiso0p4/pt);
  mvaReader->setVariable(index_nearesttrkdr, nearesttrkdr);
  mvaReader->setVariable(index_contjetdr, contjetdr);
  mvaReader->setVariable(index_contjetcsv, contjetcsv);

 return mvaReader->evaluateMethod(0);

}
