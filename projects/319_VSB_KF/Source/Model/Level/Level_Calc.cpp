#include "stdafx.h"
#include "Level.h"
#include "Err.h"
#include "CommProc.h"

#define	STATUS_L_TOP  (pSens->StatusZadv & ZADV_L_TOP)
#define	STATUS_L_BOTTOM  (pSens->StatusZadv & ZADV_L_BOTTOM)
#define	STATUS_L_AIR  (pSens->StatusZadv & ZADV_L_AIR)
#define	STATUS_L_DRAINAGE  (pSens->StatusZadv & ZADV_L_DRAINAGE)


void CLevel::Calc( double dt )
	{
  SET_BP BreakPoint;
//
  //if ( pBottom )
  //  Vent_Bott = pBottom->Vent = pBottom->VentZ;
  //if ( pTop )
  //  Vent_Top = pTop->Vent = pTop->VentZ;
  double Ksi = dt * 3600. / Tau * Vent_Bott * 0.01;
  double ObjLev = Lev;
  if ( ObjLevel.Use())
    ObjLev = ObjLevel;
  _ObjLev = ObjLev;
	if ( !_finite(Lev))
		Lev = 0.;
  Lev = ( Lev +Ksi * ObjLev )/ ( 1. + Ksi );
  Show = ( Lev - Lev_0 ) / ( Lev_100 - Lev_0 ) * 100.; 
  if ( Show < 0. )
    Show = 0.;
  if ( Show > 100. )
    Show = 100.;
//  pValue->PhysValue = pValue->SensValue = pValue->ShowValue = Show;
//  pSens->PhysValue = pSens->SensValue = pSens->ShowValue = Show;
	}

int CLevel::Work( double dt )
  {
  Calc( dt );
  SET_BP BreakPoint;
  //if(pSens)
  //{
	 // bZADV_L_TOP = STATUS_L_TOP;
	 // bZADV_L_BOTTOM = STATUS_L_BOTTOM;
	 // //bZADV_L_AIR = STATUS_L_AIR;	
	 // bZADV_L_DRAINAGE = STATUS_L_DRAINAGE;	  
	 // if(!STATUS_L_TOP && !STATUS_L_BOTTOM && !STATUS_L_DRAINAGE)
	 // {
		//  pSens->ErrValue = dOldL;
		//  Показания = pSens->ErrValue;
		//  return 0;
	 // }
	 // else if(!STATUS_L_TOP && STATUS_L_BOTTOM && !STATUS_L_DRAINAGE)
	 // {
		//  pSens->ErrValue = pSens->ShowValue / 2.0;
		//  dOldL = pSens->ShowValue;
		//  Показания = pSens->ErrValue;
		//  return 0;
	 // }
	 // else if(STATUS_L_TOP && STATUS_L_BOTTOM && !STATUS_L_DRAINAGE)
	 // {
		//  pSens->ErrValue = pSens->ShowValue;
		//  dOldL = pSens->ShowValue;
		//  Показания = pSens->ErrValue;
		//  return 0;
	 // }
	 // else
	 // {
		//  pSens->ErrValue = 0;
		//  dOldL = pSens->ShowValue;
		//  Показания = pSens->ErrValue;
	 // }
  //}	
///////////////////////////////////
//   bZADV_L_TOP = STATUS_L_TOP;
//   bZADV_L_BOTTOM = STATUS_L_BOTTOM;
   //bZADV_L_AIR = STATUS_L_AIR;	
//   bZADV_L_DRAINAGE = STATUS_L_DRAINAGE;	  
   if(!bZADV_L_TOP && !bZADV_L_BOTTOM && !bZADV_L_DRAINAGE)
   {
    ErrValue = dOldL;
    Показания = ErrValue;
    return 0;
   }
   else if(!bZADV_L_TOP && bZADV_L_BOTTOM && !bZADV_L_DRAINAGE)
   {
    ErrValue = ShowValue / 2.0;
    dOldL = ShowValue;
    Показания = ErrValue;
    return 0;
   }
   else if(bZADV_L_TOP && bZADV_L_BOTTOM && !bZADV_L_DRAINAGE)
   {
    ErrValue = ShowValue;
    dOldL = ShowValue;
    Показания = ErrValue;
    return 0;
   }
   else
   {
    ErrValue = 0;
    dOldL = ShowValue;
    Показания = ErrValue;
   }
  return 0;
  }

int CLevel::TestCtrlPoints( int kNodes, struct CObjPoint ** _ppNodes )
  {
  ASS(0)
  return -1;
  }
