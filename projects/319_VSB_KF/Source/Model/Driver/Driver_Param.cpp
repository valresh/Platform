#include "stdafx.h"
#include "Driver.h"
#include "Err.h"
#include "CommProc.h"


_W_Driver::_W_Driver()
	{
  memset ( this, 0, sizeof ( _W_Driver ));
	}


int CDriver::GetParams( char * )
	{
#define SHOW
	#include "IO_Parms.h"
	#include "Points.h"
#undef SHOW
  ACS_Params( this );
	return 0;
	}

int CDriver::UpdateParam( CParams & Param )
  {
  return 0;
  }

//#include "ShowParams.h"

int CDriver::ShowParams( DWORD DataType, struct CShowData * pSD )
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
