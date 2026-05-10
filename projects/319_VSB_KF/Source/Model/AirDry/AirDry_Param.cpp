#include "stdafx.h"
#include "AirDry.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}


int CAirDry::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
	Defect_Params(this);
	ACS_Params( this );
  //E_MENU("РСУ")
	return 0;
	}

int CAirDry::UpdateParam( CParams & Param )
  {
  return 0;
  }

#include "Param.h"

int CAirDry::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	/*switch ( DataType )
		{
		case sp_Params:
		  
		return 1;
		}*/
  return 0;
  }
