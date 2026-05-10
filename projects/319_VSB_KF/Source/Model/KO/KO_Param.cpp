#include "stdafx.h"
#include "KO.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  Omega_In = 1.0;
  Omega_Full = 1000.;
	}


int CKO::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
	return 0;
	}

int CKO::UpdateParam( CParams & Param )
  {
  return 0;
  }

//#include "ShowParams.h"

int CKO::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
/*
	switch ( DataType )
		{
		case sp_Params:
		  GetParams( "" );
		return 1;
		}
*/
  return 0;
  }
