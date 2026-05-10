#include "stdafx.h"
#include "UniHydro.h"
#include "Err.h"
#include "CommProc.h"


_W_UniHydro::_W_UniHydro()
	{
  memset ( this, 0, sizeof ( _W_UniHydro ));
	}


int CUniHydro::GetParams( char * )
	{
#define SHOW
	#include "IO_Parms.h"
	#include "Points.h"
#undef SHOW
	return 0;
	}

int CUniHydro::UpdateParam( CParams & Param )
  {
  return 0;
  }

//#include "ShowParams.h"

int CUniHydro::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
/*
	switch ( DataType )
		{
		case sp_Params:
		  GetParams( pSD->N, pSD->Params );
		return 1;
		}
*/
  return 0;
  }
