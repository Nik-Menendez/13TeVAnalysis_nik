/*
  MT2bl
  mt2bl.cc: Helper fxns mt2blIndirect, mt2blDirect, and mt2blOrder; original authors' code
  mt2bl.h:  Prototypes of helper fxns; original authors' code

  Usage with two arbitrary jets:
    // make an instance of mt2bl, it's in the namespace mt2bl_bisect
    mt2bl_bisect::mt2bl mt2bl_test;
    // turn your arbitrary jets into MomentumF's
    MomentumF* b1 = new MomentumF(jets.at(0)->p4());
    MomentumF* b2 = new MomentumF(jets.at(1)->p4());
    // call mt2blDirect with jets, the visible lepton, and met
    mt2bl = mt2bl_test.mt2blDirect(b1, b2, lep, met);

  Usage with user-defined btagging
    // make an instance of mt2bl, it's in the namespace mt2bl_bisect
    mt2bl_bisect::mt2bl mt2bl_test2;
    // make a bool vector of btags of the same size as your jets
    // turn your jets into MomentumF's
    vector<bool> btags;
    vector<MomentumF*> fjets;
    MomentumF* tempjet;
    for(auto* jet : jets) {
      btags.push_back(jet->csv() > 0.600); // my bjet selection
      tempjet = new MomentumF(jet->p4()); // ugly, for conversion
      fjets.push_back(tempjet);
    }
    // call mt2blIndirect with converted jets, btags, visible lepton, and met
    mt2bl = mt2bl_test2.mt2blIndirect(fjets,btags,lep,met);
*/

// original authors' textblock follows:

/***********************************************************************/
/*                                                                     */
/*              Finding MT2bl                                           */
/*              Reference:  arXiv:1203.4813 [hep-ph]                   */
/*              Authors: Yang Bai, Hsin-Chia Cheng,                    */
/*                       Jason Gallicchio, Jiayin Gu                   */
/*              Based on MT2 by: Hsin-Chia Cheng, Zhenyu Han           */
/*              Nov 9, 2012, v1.00a                                    */
/*                                                                     */
/***********************************************************************/
/*******************************************************************************
 Usage:

 1. Define an object of type "mt2bl":

 mt2blw_bisect::mt2bl mt2bl_event;

 2. Set momenta:

 mt2bl_event.set_momenta(pl,pb1,pb2,pmiss);

 where array pl[0..3], pb1[0..3], pb2[0..3] contains (E,px,py,pz), pmiss[0..2] contains (0,px,py)
 for the visible particles and the missing momentum. pmiss[0] is not used.
 All quantities are given in double.

 (Or use non-pointer method to do the same.)

 3. Use mt2bl::get_mt2bl() to obtain the value of mt2bl:

 double mt2bl_value = mt2bl_event.get_mt2bl();

 *******************************************************************************/

//note:  For a given trial mother mass my (for top in this case), Deltasq is defined as my^2-mn1^2,
//       where mn1 is the mass of one of the daughter particle (the neutrino on the visible lepton
//       chain or the invisible W-boson, see the code for details.)  We try to find the minimum
//       compatible Deltasq and transfer it to minimum my.
//
//       There is also variables "delta" and "delta2" which are NOT the sqrt of Delasq.
//
//another note:  This code can be easily modified to calculate mT2 with different daughter masses
//               in general.

#include "AnalysisTools/KinematicVariables/interface/mt2bl.h"

using namespace std;
using namespace ucsbsusy;

namespace mt2bl_bisect
{

  // user calls this with vector of jets and their btags. it calculates mt for a choice of two bjets.
  // requirement: 3 or more jets
  // error code: error_value, see header file
  // note: same procedure for MT2W and MT2BL
  double mt2bl::mt2blIndirect(const vector<MomentumF*>& fjets, const vector<bool>& btags, const LeptonF* lep, const MomentumF* met) {

        unsigned int n_jets = fjets.size();

        // fault checks
        if (n_jets < 3) return error_value;
        if (n_jets != btags.size()) return error_value;

        // separate four leading pt jets into bjets and non-bjets
        // these hold indices into fjets
        std::vector<int> bjets;
        std::vector<int> non_bjets;
        for(unsigned int i = 0; i < min((unsigned int)4,n_jets); i++ ) {
            if( btags.at(i) ) {
                bjets.push_back(i);
            } else {
                non_bjets.push_back(i);
            }
        }
        unsigned int n_btag = bjets.size();
        // three categories, depending on number of btags in four leading pt jets

        // Category 1: two b tags. apply function directly.
        if (n_btag == 2) {
          return mt2blOrder(fjets[bjets[0]],fjets[bjets[1]],lep,met);
        }
        // Category 2: one b tag. use two leading non-btags as candidate bjets and
        //   take minimum function value.
        else if (n_btag == 1) {
          double op2_1 = mt2blOrder(fjets[bjets[0]],fjets[non_bjets[0]],lep,met);
          double op2_2 = mt2blOrder(fjets[bjets[0]],fjets[non_bjets[1]],lep,met);
          return min(op2_1,op2_2);
        }
        // Category 3: zero, three, or four btags. ignore btags entirely. use three
        //   leading non-btags as candidates and take minimum function value (three combos)
        else { // n_btag = 0, 3, or 4 (we only counted either 3 or 4 jets)
          double op3_1 = mt2blDirect(fjets[0],fjets[1],lep,met);
          double op3_2 = mt2blDirect(fjets[0],fjets[2],lep,met);
          double op3_3 = mt2blDirect(fjets[1],fjets[2],lep,met);
          return min(op3_1,min(op3_2,op3_3));
        }
        return error_value;
  }

  // calls mt2blDirect with correct association of a b-jet to visible lepton
  // deltaR() is in AnalysisTools/Utilities/interface/PhysicsUtilities.h
  double mt2bl::mt2blOrder(const MomentumF* b1, const MomentumF* b2, const LeptonF* lep, const MomentumF* met) {
  // this comment block will pair the visible lepton with the nearest of the two bjets (nonstandard)
  /*
        double deltaRb1 = PhysicsUtilities::deltaR(*b1,*lep);
        double deltaRb2 = PhysicsUtilities::deltaR(*b2,*lep);
        return (deltaRb2>deltaRb1) ? mt2blDirect(b1,b2,lep,met) : mt2blDirect(b2,b1,lep,met);
  */

  // Per the MT2 paper, choose the combo which minimises MT2
      return min(mt2blDirect(b1,b2,lep,met),mt2blDirect(b2,b1,lep,met));
  }

  // calculates mt2bl directly given momenta of lep, two bjets, and the met's pt and phi
  // b1 is associated with visible lepton. b2 is associated with lost lepton.
  double mt2bl::mt2blDirect(const MomentumF* b1, const MomentumF* b2, const LeptonF* lep, const MomentumF* met) {

      // fill pl, pb1, pb2, and pmiss arrays with user input
      double metx = met->pt() * cos( met->phi() );
      double mety = met->pt() * sin( met->phi() );

      double pl[4];     // visible lepton E,px,py,pz
      double pb1[4];    // bottom on the same side as the visible lepton
      double pb2[4];    // other bottom, paired with the invisible W
      double pmiss[3];  // <unused>, pmx, pmy   missing pT

      pl[0]  = lep->E();
      pl[1]  = lep->px(); pl[2] = lep->py(); pl[3] = lep->pz();

      pb1[0] = b1->E();
      pb1[1] = b1->px(); pb1[2] = b1->py(); pb1[3] = b1->pz();
      pb2[0] = b2->E();
      pb2[1] = b2->px(); pb2[2] = b2->py(); pb2[3] = b2->pz();

      pmiss[0] = 0.; pmiss[1] = metx; pmiss[2] = mety;

      // run mt2bl procedure on the set
      mt2bl mt2bl_event;
      mt2bl_event.set_momenta(pl, pb1, pb2, pmiss);

      return mt2bl_event.get_mt2bl();
  }

mt2bl::mt2bl()
{
   solved = false;
   momenta_set = false;
   mt2bl_b  = 0.;
   scan_step = 0.1;
}

double mt2bl::get_mt2bl()
{
   if (!momenta_set)
   {
       std::cout <<" Please set momenta first!" << std::endl;
       return 0;
   }

   if (!solved) mt2bl_bisect();
   return mt2bl_b;
}

void mt2bl::set_momenta(double *pl, double *pb1, double *pb2, double* pmiss)
{
	// Pass in pointers to 4-vectors {E, px, py, px} of doubles.
	// and pmiss must have [1] and [2] components for x and y.  The [0] component is ignored.
	set_momenta(pl[0],  pl[1],  pl[2],  pl[3],
				pb1[0], pb1[1], pb1[2], pb1[3],
				pb2[0], pb2[1], pb2[2], pb2[3],
				pmiss[1], pmiss[2]);
}



void mt2bl::set_momenta(double El,  double plx,  double ply,  double plz,
						  double Eb1, double pb1x, double pb1y, double pb1z,
						  double Eb2, double pb2x, double pb2y, double pb2z,
						  double pmissx, double pmissy)
{
	solved = false;     //reset solved tag when momenta are changed.
   momenta_set = true;

   //combine pl and pb1 and call it pa, pb2 is renamed as pb.

   pax  = plx + pb1x;
   pay  = ply + pb1y;
   Easq = (El+Eb1)*(El+Eb1) - (plz+pb1z)*(plz+pb1z);   //Ea is actually the ET of a
   Ea   = sqrt(Easq);
   masq = max(0., Easq - pax*pax - pay*pay);
   ma   = sqrt(masq);


   pbx  = pb2x;
   pby  = pb2y;
   Ebsq = Eb2*Eb2 - pb2z*pb2z;                        //Eb is actually the ET of b
   Eb   = sqrt(Ebsq);
   mbsq = max(0., Ebsq - pbx*pbx - pby*pby);
   mb   = sqrt(mbsq);

   this->pmissx = pmissx;
   this->pmissy = pmissy;

   pmissxsq = pmissx*pmissx;
   pmissysq = pmissy*pmissy;

	mn1 = 0.0;       //mn1 is the mass of neutrino by default
	mn2 = 80.4;      //mass of W-boson


// set ma + mn1 >= mb +mn2 (i.e. if ma + mn1 < mb + mn2, just switch the names of the two sides)
   if(ma + mn1 < mb + mn2)
   {
      double temp;
      temp = pax;  pax  = pbx;  pbx  = temp;
      temp = pay;  pay  = pby;  pby  = temp;
      temp = Ea;   Ea   = Eb;   Eb   = temp;
      temp = Easq; Easq = Ebsq; Ebsq = temp;
      temp = masq; masq = mbsq; mbsq = temp;
      temp = ma;   ma   = mb;   mb   = temp;
	  temp = mn1;  mn1  = mn2;  mn2  = temp;
   }

//   std::cout << std::endl << "ma+mn1 = " << ma + mn1 << std::endl;

    masq = ma*ma;
	mbsq = mb*mb;

   	mn1sq = mn1*mn1;
	mn2sq = mn2*mn2;


   if (ABSOLUTE_PRECISION > 100.*RELATIVE_PRECISION) precision = ABSOLUTE_PRECISION;
   else precision = 100.*RELATIVE_PRECISION;
}

//void mt2bl::print()
//{
//   std::cout << " pax = " << pax*scale << ";   pay = " << pay*scale << ";   ma = " << ma*scale <<";"<< std::endl;
//   std::cout << " pbx = " << pbx*scale << ";   pby = " << pby*scale << ";   mb = " << mb*scale <<";"<< std::endl;
//   std::cout << " pmissx = " << pmissx*scale << ";   pmissy = " << pmissy*scale <<";"<< std::endl;
//   std::cout << " mn = " << mn_unscale<<";" << std::endl;
//}


void mt2bl::mt2bl_bisect()
{


   solved = true;
   std::cout.precision(11);

   double Deltasq0 = ma*(ma + 2*mn1); //The minimum mass square to have two ellipses (in general Deltasq = my^2 - mn1^2)
   double Delta2sq0 = Deltasq0 + mn1sq - mn2sq;

// find the coefficients for the two quadratic equations when Deltasq=Deltasq0.

   a1 = 1-pax*pax/(Easq);
   b1 = -pax*pay/(Easq);
   c1 = 1-pay*pay/(Easq);
   d1 = -pax*(Deltasq0-masq)/(2*Easq);
   e1 = -pay*(Deltasq0-masq)/(2*Easq);
   a2 = 1-pbx*pbx/(Ebsq);
   b2 = -pbx*pby/(Ebsq);
   c2 = 1-pby*pby/(Ebsq);
   d2 = -pmissx+pbx*(Delta2sq0-mbsq)/(2*Ebsq)+pbx*(pbx*pmissx+pby*pmissy)/(Ebsq);
   e2 = -pmissy+pby*(Delta2sq0-mbsq)/(2*Ebsq)+pby*(pbx*pmissx+pby*pmissy)/(Ebsq);
   f2 = pmissx*pmissx+pmissy*pmissy-((Delta2sq0-mbsq)/(2*Eb)+
        (pbx*pmissx+pby*pmissy)/Eb)*((Delta2sq0-mbsq)/(2*Eb)+
        (pbx*pmissx+pby*pmissy)/Eb)+mn2sq;


//std::cout << "pmissx = " << pmissx << "  ,  pmissy = " << pmissy << std::endl;
//std::cout << "ma = " << ma  << "  ,  mb = " << mb <<std::endl;
//std::cout << "a2 = " << a2   << " ,  d2=" << d2  << " ,  e2=" << e2  << " ,  f2=" << f2 << std::endl;

// find the center of the smaller ellipse
   double x0,y0;
   x0 = (c1*d1-b1*e1)/(b1*b1-a1*c1);
   y0 = (a1*e1-b1*d1)/(b1*b1-a1*c1);


// Does the larger ellipse contain the smaller one?
   double dis=a2*x0*x0+2*b2*x0*y0+c2*y0*y0+2*d2*x0+2*e2*y0+f2;

   if(dis<=0.01)
   {
      mt2bl_b  = (double) sqrt(mn1sq+Deltasq0);
      return;
   }


// find the coefficients for the two quadratic equations
// coefficients for quadratic terms do not change
// Coefficients for linear and constant terms are are polynomials of delta=(my^2 - mn1^2 -mb^2)/(2 Eb^2) for ellipse 1
// and  polynomials of delta2 = (my^2 - mn2^2 -mb^2)/(2 Eb^2) for ellipse 2.

   d11 = -pax;
   e11 = -pay;
   f10 = mn1sq;
   f12 = -Easq;
   d21 = pbx;
   d20 = -a2*pmissx - b2*pmissy;
   e21 = pby;
   e20 = -c2*pmissy - b2*pmissx;
   f22 = - Ebsq;
   f21 = -2.*(pmissx*pbx+pmissy*pby);
   f20 = a2*pmissxsq + 2.*b2*pmissx*pmissy + c2*pmissysq + mn2sq;

//Estimate upper bound of mt2bl
//when Deltasq > Deltasq_high1, the larger encloses the center of the smaller
   double p2x0,p2y0;
   double Deltasq_high1;
   p2x0 = pmissx-x0;
   p2y0 = pmissy-y0;
   Deltasq_high1 = 2*Eb*sqrt(p2x0*p2x0+p2y0*p2y0+mn2sq)-2*pbx*p2x0-2*pby*p2y0+mbsq + mn2sq - mn1sq;  // need mn2sq - mn1sq for ellipse 2

//Another estimate, if both ellipses enclose the origin, Deltasq > mt2bl

   double Deltasq_high2, Deltasq_high21, Deltasq_high22;
   Deltasq_high21 = 2*Eb*sqrt(pmissx*pmissx+pmissy*pmissy+mn2sq)-2*pbx*pmissx-2*pby*pmissy+mbsq + mn2sq - mn1sq;
   Deltasq_high22 = 2*Ea*mn1+masq;

   if ( Deltasq_high21 < Deltasq_high22 ) Deltasq_high2 = Deltasq_high22;
   else Deltasq_high2 = Deltasq_high21;

//pick the smaller upper bound
   double Deltasq_high;
   if(Deltasq_high1 < Deltasq_high2) Deltasq_high = Deltasq_high1;
   else Deltasq_high = Deltasq_high2;

   double Deltasq_low; //lower bound
   Deltasq_low = Deltasq0;

//number of solutions at Deltasq_low should not be larger than zero
   if( nsols(Deltasq_low) > 0 )
   {
     //std::cout << "nsolutions(Deltasq_low) > 0"<<std::endl;
     mt2bl_b = (double) sqrt(mn1sq+Deltasq0);
     return;
   }

   int nsols_high, nsols_low;

   nsols_low  = nsols(Deltasq_low);
   int foundhigh;


//if nsols_high=nsols_low, we missed the region where the two ellipse overlap
//if nsols_high=4, also need a scan because we may find the wrong tangent point.

   nsols_high = nsols(Deltasq_high);

   if(nsols_high == nsols_low || nsols_high == 4)
   {
      foundhigh = scan_high(Deltasq_high);
   //   foundhigh = find_high(Deltasq_high);
      if(foundhigh == 0)
      {
	std::cout << __FILE__ << " " << __LINE__ << " Deltasq_high not found, output ma+mn1" << std::endl;
//		  std::cout << "myhigh1 = " << sqrt(Deltasq_high1 + mn1sq) << std::endl;
//		  std::cout << "myhigh2 = " << sqrt(Deltasq_high2 + mn1sq) << std::endl;
//		  std::cout << "nsol(high1) = " << nsols(Deltasq_high1) << std::endl;
//		  std::cout << "nsol(high2) = " << nsols(Deltasq_high2) << std::endl;



         mt2bl_b = sqrt( Deltasq_low + mn1sq );
         return;
      }

   }

   while(sqrt(Deltasq_high+mn1sq) - sqrt(Deltasq_low+mn1sq) > precision)
   {
      double Deltasq_mid,nsols_mid;
      //bisect
      Deltasq_mid = (Deltasq_high+Deltasq_low)/2.;
      nsols_mid = nsols(Deltasq_mid);
      // if nsols_mid = 4, rescan for Deltasq_high
      if ( nsols_mid == 4 )
      {
         Deltasq_high = Deltasq_mid;
      //   scan_high(Deltasq_high);
         find_high(Deltasq_high);
         continue;
      }


      if(nsols_mid != nsols_low) Deltasq_high = Deltasq_mid;
      if(nsols_mid == nsols_low) Deltasq_low  = Deltasq_mid;
   }
   mt2bl_b = (double) sqrt( mn1sq + Deltasq_high);
   return;
}


//For some reason this sophiscicated way of finding the upper bound does not seem to work for mt2bl...
int mt2bl::find_high(double & Deltasq_high)
{
   double x0,y0;
   x0 = (c1*d1-b1*e1)/(b1*b1-a1*c1);
   y0 = (a1*e1-b1*d1)/(b1*b1-a1*c1);
   double Deltasq_low = ma*(ma + 2*mn1);
   do
   {
      double Deltasq_mid = (Deltasq_high + Deltasq_low)/2.;
      int nsols_mid = nsols(Deltasq_mid);
      if ( nsols_mid == 2 )
      {
         Deltasq_high = Deltasq_mid;
         return 1;
      }
      else if (nsols_mid == 4)
      {
         Deltasq_high = Deltasq_mid;
         continue;
      }
      else if (nsols_mid ==0)
      {
         d1 = -pax*(Deltasq_mid-masq)/(2*Easq);
         e1 = -pay*(Deltasq_mid-masq)/(2*Easq);
         d2 = -pmissx + pbx*(Deltasq_mid - mbsq - mn2sq + mn1sq )/(2*Ebsq)
              + pbx*(pbx*pmissx+pby*pmissy)/(Ebsq);
         e2 = -pmissy + pby*(Deltasq_mid - mbsq - mn2sq + mn1sq)/(2*Ebsq)
              + pby*(pbx*pmissx+pby*pmissy)/(Ebsq);
         f2 = pmissx*pmissx+pmissy*pmissy-((Deltasq_mid-mbsq- mn2sq + mn1sq)/(2*Eb)+
              (pbx*pmissx+pby*pmissy)/Eb)*((Deltasq_mid-mbsq- mn2sq + mn1sq)/(2*Eb)+
              (pbx*pmissx+pby*pmissy)/Eb)+mn2sq;
// Does the larger ellipse contain the smaller one?
         double dis = a2*x0*x0 + 2*b2*x0*y0 + c2*y0*y0 + 2*d2*x0 + 2*e2*y0 + f2;
           if (dis < 0) Deltasq_high = Deltasq_mid;
           else Deltasq_low = Deltasq_mid;

      }

   } while ( Deltasq_high - Deltasq_low > 0.001);
   return 0;
}

//...so we just scan to find the upper bound if our initial guess does not work
int mt2bl::scan_high(double & Deltasq_high)
{
   int foundhigh = 0 ;
   int nsols_high;


   //double Deltasq_low;
   double tempmass, maxmass;
   tempmass = mn1 + ma;
   maxmass  = sqrt(mn1sq + Deltasq_high);
   //if (nevt == 32334) std::cout << "Deltasq_high = " << Deltasq_high << std::endl;
   for(double mass = tempmass + scan_step; mass < maxmass; mass += scan_step)
   {
      Deltasq_high = mass*mass - mn1sq;
      nsols_high   = nsols(Deltasq_high);

      if( nsols_high > 0)
      {
	 //Deltasq_low = (mass-scan_step)*(mass-scan_step) - mn1sq;
		foundhigh   = 1;
         break;
      }
    }
    return foundhigh;
}



int mt2bl::nsols(  double Dsq)
{
   double delta = (Dsq-masq)/(2*Easq);
   double delta2 = (Dsq-mbsq - mn2sq + mn1sq )/(2*Ebsq);


//calculate coefficients for the two quadratic equations
   d1 = d11*delta;
   e1 = e11*delta;
   f1 = f12*delta*delta+f10;
   d2 = d21*delta2+d20;
   e2 = e21*delta2+e20;
   f2 = f22*delta2*delta2+f21*delta2+f20;

//obtain the coefficients for the 4th order equation
//devided by Ea^n to make the variable dimensionless
   long double A4, A3, A2, A1, A0;

   A4 =
   -4*a2*b1*b2*c1 + 4*a1*b2*b2*c1 +a2*a2*c1*c1 +
   4*a2*b1*b1*c2 - 4*a1*b1*b2*c2 - 2*a1*a2*c1*c2 +
   a1*a1*c2*c2;

   A3 =
     (-4*a2*b2*c1*d1 + 8*a2*b1*c2*d1 - 4*a1*b2*c2*d1 - 4*a2*b1*c1*d2 +
   8*a1*b2*c1*d2 - 4*a1*b1*c2*d2 - 8*a2*b1*b2*e1 + 8*a1*b2*b2*e1 +
   4*a2*a2*c1*e1 - 4*a1*a2*c2*e1 + 8*a2*b1*b1*e2 - 8*a1*b1*b2*e2 -
     4*a1*a2*c1*e2 + 4*a1*a1*c2*e2)/Ea;


   A2 =
     (4*a2*c2*d1*d1 - 4*a2*c1*d1*d2 - 4*a1*c2*d1*d2 + 4*a1*c1*d2*d2 -
   8*a2*b2*d1*e1 - 8*a2*b1*d2*e1 + 16*a1*b2*d2*e1 +
   4*a2*a2*e1*e1 + 16*a2*b1*d1*e2 - 8*a1*b2*d1*e2 -
   8*a1*b1*d2*e2 - 8*a1*a2*e1*e2 + 4*a1*a1*e2*e2 - 4*a2*b1*b2*f1 +
   4*a1*b2*b2*f1 + 2*a2*a2*c1*f1 - 2*a1*a2*c2*f1 +
     4*a2*b1*b1*f2 - 4*a1*b1*b2*f2 - 2*a1*a2*c1*f2 + 2*a1*a1*c2*f2)/Easq;

   A1 =
     (-8*a2*d1*d2*e1 + 8*a1*d2*d2*e1 + 8*a2*d1*d1*e2 - 8*a1*d1*d2*e2 -
   4*a2*b2*d1*f1 - 4*a2*b1*d2*f1 + 8*a1*b2*d2*f1 + 4*a2*a2*e1*f1 -
   4*a1*a2*e2*f1 + 8*a2*b1*d1*f2 - 4*a1*b2*d1*f2 - 4*a1*b1*d2*f2 -
     4*a1*a2*e1*f2 + 4*a1*a1*e2*f2)/(Easq*Ea);

   A0 =
     (-4*a2*d1*d2*f1 + 4*a1*d2*d2*f1 + a2*a2*f1*f1 +
   4*a2*d1*d1*f2 - 4*a1*d1*d2*f2 - 2*a1*a2*f1*f2 +
     a1*a1*f2*f2)/(Easq*Easq);

   /*
   long  double A0sq, A1sq, A2sq, A4sq;
   A0sq = A0*A0;
   A1sq = A1*A1;
   A2sq = A2*A2;
   A4sq = A4*A4;
   */
   long double A3sq;
   A3sq = A3*A3;

   long double B3, B2, B1, B0;
   B3 = 4*A4;
   B2 = 3*A3;
   B1 = 2*A2;
   B0 = A1;

   long double C2, C1, C0;
   C2 = -(A2/2 - 3*A3sq/(16*A4));
   C1 = -(3*A1/4. -A2*A3/(8*A4));
   C0 = -A0 + A1*A3/(16*A4);

   long double D1, D0;
   D1 = -B1 - (B3*C1*C1/C2 - B3*C0 -B2*C1)/C2;
   D0 = -B0 - B3 *C0 *C1/(C2*C2)+ B2*C0/C2;

   long double E0;
   E0 = -C0 - C2*D0*D0/(D1*D1) + C1*D0/D1;

   long  double t1,t2,t3,t4,t5;
//find the coefficients for the leading term in the Sturm sequence
   t1 = A4;
   t2 = A4;
   t3 = C2;
   t4 = D1;
   t5 = E0;


//The number of solutions depends on diffence of number of sign changes for x->Inf and x->-Inf
   int nsol;
   nsol = signchange_n(t1,t2,t3,t4,t5) - signchange_p(t1,t2,t3,t4,t5);

//Cannot have negative number of solutions, must be roundoff effect
   if (nsol < 0) nsol = 0;

   return nsol;

}

inline int mt2bl::signchange_n( long double t1, long double t2, long double t3, long double t4, long double t5)
{
   int nsc;
   nsc=0;
   if(t1*t2>0) nsc++;
   if(t2*t3>0) nsc++;
   if(t3*t4>0) nsc++;
   if(t4*t5>0) nsc++;
   return nsc;
}
inline int mt2bl::signchange_p( long double t1, long double t2, long double t3, long double t4, long double t5)
{
   int nsc;
   nsc=0;
   if(t1*t2<0) nsc++;
   if(t2*t3<0) nsc++;
   if(t3*t4<0) nsc++;
   if(t4*t5<0) nsc++;
   return nsc;
}

}//end namespace mt2bl_bisect
