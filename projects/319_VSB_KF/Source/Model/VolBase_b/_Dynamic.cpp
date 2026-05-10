#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 

void CVolBase::Dynamic( double dt )
  {
	double M_Liq_0 = M_Liq;
	if ( FullInitN2 )
    {
		VolInit ( pN2, 1., T_air );
    }
	if ( kStep % 10 == 0 )
		ЕстьЗадания = false;
	if ( kStep % 50 == 0 )
		Compress();
  SummFlow( dt );
  if ( !_finite ( T ))
    T = T_air;
//
  SET_BP_SUB
  S.Q = Q_ext;
  S.Q_Cool = k_Cool * ( T_air - T );
  S.Q += S.Q_Cool;
//??  DynamicCond( dt );
////////////////////////////////////////////////////////////////////
//??  DynamicMix_Gas( dt );
//??  DynamicMix_Liq( dt );
  Dynamic_Iter_VC( dt );
////////////////////////////////////////////////////////////////////
/// Химия //////
  if ( pReactions )
    pReactions->Reactions( dt, &ReactionsParam );
////////////////////////////////////////////////////////////////////
//
	if ( Delete )
		{
    DeleteComp( dt );
		}
//
////////////////////////////////////////////////////////
  SET_BP_SUB
//
//  if ( strstr ( ObjName, "Е-101" ))
//    KKK();
  //if ( strstr ( ObjName, "E-23"))
  //  KKK();
  if ( Calc_2F )
    Calc2F( );
  else
    Is_2F = false;
//
//.....................................................
  if ( T_Z != 0. && ( pSys->bWorkReg || kStepTz > 0 ) && !pSys ->bInit_0 )
    {
    ЕстьЗадания = true;
    T = T_Z;
    }
  else
    {
    if ( Fast_T )
      {
      Fast_Calc_T( T, S.Q + S.dQ_mix );
      if ( T < T_min )
        T = T_min;
      }
    else
      {
      CalcT( dt );
      }
    }
  SET_BP_SUB
//
	_dMdt_mol = dMdt_mol = ( M_Liq - M_Liq_0 ) / dt;
//
	if ( P_Z > -1. )
		{
    Set_P_Z( dt );
		}
//
  CalcRoH( );
  if ( Add_1F || Add_2F || Add_Comp )
    Add();
//
//  if ( strstr ( ObjName, "Е-101" ))
//    KKK();
  if ( Объема_2 && Calc_2F )
    Level_2Vol( dt );
  else
    Level_1Vol( dt );
  CalcShowComp( );
  CalcBalance( dt );
  }
