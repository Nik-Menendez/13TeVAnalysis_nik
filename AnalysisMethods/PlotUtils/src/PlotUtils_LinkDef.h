#include "../interface/Plot.hh"
#include "../interface/StyleTools.hh"
#include "../interface/PlotROCCurves.h"
#include "../interface/EffPlotTools.hh"
#include "../interface/PlotStuff.h"

//#include "../interface/Refold.h"
//#include "../interface/MakeRefold.h"

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ typedef h1D        ;
#pragma link C++ typedef h2D        ;
#pragma link C++ typedef graph        ;
#pragma link C++ typedef profile        ;
#pragma link C++ class Plot ;

#pragma link C++ class PlotROCCurves ;

#pragma link C++ namespace StyleTools;
#pragma link C++ enum StyleTools::Colors;
#pragma link C++ typedef StyleTools::ColorMap;
//

#pragma link C++ namespace EffPlotTools;
#pragma link C++ function EffPlotTools::computeEffGraph;
#pragma link C++ function EffPlotTools::computeROCCurve;
#pragma link C++ function EffPlotTools::getCutValueForEfficiency;
#pragma link C++ function EffPlotTools::getEfficiencyForCutValue;
#pragma link C++ function EffPlotTools::getCutValueForBestEffSOverSqrtEffB;

#pragma link C++ class PlotStuff;

#pragma link C++ namespace PlotTools;
#pragma link C++ function PlotTools::initSamples;


#pragma link C++ class Sample ;

#endif