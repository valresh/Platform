#include "stdafx.h"
#include "ChemSpec.h"
#include "Err.h"
#include "CommProc.h"


_W_ChemSpec::_W_ChemSpec()
	{
  memset ( this, 0, sizeof ( _W_ChemSpec ));
	}


int CChemSpec::GetParams( char * )
	{
#define SHOW
	#include "IO_Parms.h"
	#include "Points.h"
#undef SHOW
	return 0;
	}

int CChemSpec::UpdateParam( CParams & Param )
  {
  return 0;
  }

//#include "ShowParams.h"

int CChemSpec::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
  return 0;
  }
