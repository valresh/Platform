#include "stdafx.h"
#include "CoolObj.h"
#include "Err.h"
#include "CommProc.h"


_W_CoolObj::_W_CoolObj()
	{
  memset ( this, 0, sizeof ( _W_CoolObj ));
	}


int CCoolObj::GetParams( char * )
	{
	 Char<64>Name;
	#include "IO_Parms.h"
#define GET
	#include "Points.h"
  TAB("Номинальные Т", 1 )
  for ( int n = 0; n < kSens; n++ )
    {
    PARM(Tz[n], "Ном Т" )
    }
  ETAB
  ACS_Params( "АСУ", this );
  Defect_Params(this);
//	TAB("Модули",5)
	return 0;
	}

int CCoolObj::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &Set_Tz && Set_Tz )
    {
    for ( int n = 0; n < kSens; n++ )
      {
      Tz[n] = T_min + r01() * ( T_max - T_min );
      }
    return 0;
    }
  if ( Param.Addr == &F_nom )
    {
    if ( F_nom <= 0. )
      F_nom = F_kg;
    return 0;
    }
  return 0;
  }

//#include "ShowParams.h"

int CCoolObj::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }
