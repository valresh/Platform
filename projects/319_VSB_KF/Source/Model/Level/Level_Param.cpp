#include "stdafx.h"
#include "Level.h"
#include "Err.h"
#include "CommProc.h"


_W_Level::_W_Level()
{
	memset ( this, 0, sizeof ( _W_Level ));
	dOldL = 0.0;
}


int CLevel::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
//	if(pSens)
//	{
		TAB("Состояние", 1 )
//			PARM( pSens->ErrValue, "#Результат" );
		  PARM( dOldL, "#Результат" );
			PARM_B(bZADV_L_TOP, "#ZADV_L_TOP", 0);
			PARM_B(bZADV_L_BOTTOM, "#ZADV_L_BOTTOM", 0);
			//PARM_B(bZADV_L_AIR, "#ZADV_L_AIR", 0);
			PARM_B(bZADV_L_DRAINAGE, "#ZADV_L_DRAINAGE", 0);
		ETAB
//	}
		
  ACS_Params( "АСУ", this );
	return 0;
	}

int CLevel::UpdateParam( CParams & Param )
  {
  //if ( Param.Addr == &Vent_Bott && pBottom )
  //  pBottom->Vent = pBottom->VentZ = Vent_Bott;
  //if ( Param.Addr == &Vent_Top && pTop )
  //  pTop->Vent = pTop->VentZ = Vent_Top;
  //if(pSens)
  //{
	 // if(Param.Addr == &bZADV_L_TOP)
	 // {
		//  if(bZADV_L_TOP)
		//	  pSens->StatusZadv |= ZADV_L_TOP;
		//  else 
		//	 pSens->StatusZadv ^= ZADV_L_TOP;
	 // }
	 // if(Param.Addr == &bZADV_L_BOTTOM)
	 // {
		//  if(bZADV_L_BOTTOM)
		//	  pSens->StatusZadv |= ZADV_L_BOTTOM;
		//  else 
		//	 pSens->StatusZadv ^= ZADV_L_BOTTOM;
	 // }
	 // if(Param.Addr == &bZADV_L_DRAINAGE)
	 // {
		//  if(bZADV_L_DRAINAGE)
		//	  pSens->StatusZadv |= ZADV_L_DRAINAGE;
		//  else 
		//	 pSens->StatusZadv ^= ZADV_L_DRAINAGE;
	 // }
  //}
  return 0;
  }

//#include "ShowParams.h"

int CLevel::ShowParams( DWORD DataType, struct CShowData * pSD )
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
