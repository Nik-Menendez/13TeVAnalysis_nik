//--------------------------------------------------------------------------------------------------
// 
// TreeWriter
// 
// Class to help with tree writing.
// 
// TreeWriter.h created on Tue Oct 14 22:30:18 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_UTILITIES_TREEWRITER_H
#define ANALYSISTOOLS_UTILITIES_TREEWRITER_H

#include <string>
#include <vector>
#include <assert.h>
#include <TString.h>
#include <TTree.h>

namespace ucsbsusy {

  class TreeWriter {

    public :
      TreeWriter(TTree *tree, const char *treename="Events");

      ~TreeWriter();

      Int_t		getBufSize()		  const { return fBufSize;	}
      Int_t		getSplitLevel()		  const { return fSplitLevel;	}
      TString		getTreeName()		  const { return fTreeName;	}
      void		setBufSize(Int_t b	)	{ fBufSize = b;		}
      void		setSplitLevel(Int_t s	)	{ fSplitLevel = s;	}
      void		setTreeName(TString n	)	{ fTreeName = n;	fTree->SetName(fTreeName.Data());	}
      void		fill()				{ fTree->Fill();	}
      TTree		*getTree()			{ return fTree;		}

      template<class T>
      void		book(const char *name, T& var, const char *type)	{ fTree->Branch(name, &var, TString(name).Append("/").Append(type).Data()); }

      template<class T>
      void		book(const char *name, std::vector<T>& varv)		{ fTree->Branch(name, &varv);	}

    protected :
      Int_t		fBufSize;
      Int_t		fSplitLevel;
      TString		fTreeName;
      TTree		*fTree;

  }; // TreeWriter

}

#endif
