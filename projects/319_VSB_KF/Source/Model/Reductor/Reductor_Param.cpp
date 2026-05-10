#include "stdafx.h"
#include "Reductor.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}


int CReductor::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  if ( IsCtrlGas )
    {
    PARM( P_Gas, "#Давление газа(и)" );
    PARM( dP_Gas_In, "Заданный перепад газ-вход редуктора" );
    }
  ACS_Params( "АСУ", this );
	return 0;
	}

int CReductor::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &P_Z )
    {
    if ( P_Z > P_Max )
      P_Max = P_Z;
    }
  if ( Param.Addr == &P_Max )
    {
    if ( P_Z > P_Max )
      P_Max = P_Z;
    }
  if ( Param.Addr == &_dP )
    {
    _P_out_Z = !_dP;
    }
  return 0;
  }

//#include "ShowParams.h"

int CReductor::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }
