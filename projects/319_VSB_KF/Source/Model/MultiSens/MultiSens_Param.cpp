#include "stdafx.h"
#include "MultiSens.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}


int CMultiSens::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
//  ShowAddPoints( "Точки" );
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//#include "ACS_Param.h"
//#include "ACS.h"
//	TAB("Модули",5)
	return 0;
	}

int CMultiSens::UpdateParam( CParams & Param )
  {
  return 0;
  }

//#include "ShowParams.h"

int CMultiSens::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }

/*extern "C"
EXP int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes < 1 )
    {
    ModMsg ( "У измерителя '%s' не задан вход оборотов", ObjName );
    return 1;
    }
  int kIn = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    if ( pOP->PntName == "Обороты" )
      {
      kIn++;
      continue;
      }
    }
  if ( kIn != 1 )
    {
    ModMsg ( "У измерителя '%s' неправильно задан вход 'Обороты'", ObjName );
    return 1;
    }
  return 0;
  }*/
