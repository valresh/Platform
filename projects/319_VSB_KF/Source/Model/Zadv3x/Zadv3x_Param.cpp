#include "stdafx.h"
#include "Zadv3x.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}


int CZadv3x::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
//---------------------------------------------
  if ( Type == 1 )
    {
 // i,o1,o2
    PARM_D( O01, "#Проходимость i->o1", 0. );
    PARM_D( O02, "#Проходимость i->o2", 0. );
    PARM_D( Flow_0, "#Поток i", 0. );
    PARM_D( Flow_1, "#Поток o1", 0. );
    PARM_D( Flow_2, "#Поток o2", 0. );
    }
  else
    {
// o,i1,i2
    PARM_D( O01, "#Проходимость o<-i1", 0. );
    PARM_D( O02, "#Проходимость o<-i2", 0. );
    PARM_D( Flow_0, "#Поток o", 0. );
    PARM_D( Flow_1, "#Поток i1", 0. );
    PARM_D( Flow_2, "#Поток i2", 0. );
    }
  ACS_Params( "АСУ", this );
	return 0;
	}

int CZadv3x::UpdateParam( CParams & Param )
  {
  //if ( Param.Addr == &Vent )
  //  {
  //  pZadv->Vent = pZadv->VentZ = Vent;
	 // if(ПоложениеЗадание.Use())
		//  ПоложениеЗадание = Vent;

  //  }
  return 0;
  }

//#include "ShowParams.h"

int CZadv3x::ShowParams( DWORD DataType, struct CShowData * pSD )
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
