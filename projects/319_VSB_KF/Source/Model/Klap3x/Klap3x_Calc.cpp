#include "stdafx.h"
#include "Klap3x.h"
#include "Err.h"
#include "CommProc.h"
#include "SysDataTypes.h"


void CKlap3x::Calc( double dt )
	{
  SET_BP BreakPoint;
  //if ( Задание.Use())
  //  pPos->VentZ = Задание;
  //if ( _0.On())
  //  pPos->VentZ = 0.;
  //if ( _50.On())
  //  pPos->VentZ = 50.;
  //if ( _100.On())
  //  pPos->VentZ = 100.;
  //if ( Defect == 1 )
  //  pPos->VentZ = Fixed_Pos;
  //if ( LocalBlk || pSys->IsBlk )
  //  {
  //  if ( Соленоид.Use() || Соленоид_2.Use())
  //    {
  //    bool On = false;
  //    if ( Prty_1 )
  //      {
  //      if ( Соленоид.On() || Соленоид_2.On())
  //        pPos->VentZ = 100.;
  //      else
  //        pPos->VentZ = 0.;
  //      }
  //    else
  //      {
  //      if ( Соленоид.Off() || Соленоид_2.Off())
  //        pPos->VentZ = 0.;
  //      else
  //        pPos->VentZ = 100.;
  //      }
  //    }
  //  }
  //else
  //  {
  //  bool Err = false;
  //  if ( Соленоид.Use() || Соленоид_2.Use())
  //    {
  //    bool On = false;
  //    if ( Prty_1 )
  //      {
  //      if ( Соленоид.On() || Соленоид_2.On())
  //        {
  //        if ( pPos->VentZ == 0.)
  //          Err = true;
  //        }
  //      else
  //        {
  //        if ( pPos->VentZ > 0.)
  //          Err = true;
  //        }
  //      }
  //    else
  //      {
  //      if ( Соленоид.Off() || Соленоид_2.Off())
  //        {
  //        if ( pPos->VentZ > 0.)
  //          Err = true;
  //        }
  //      else
  //        {
  //        if ( pPos->VentZ == 0.)
  //          Err = true;
  //        }
  //      }
  //    }
  //  ErrInBlk = Err;
  //  if ( Err )
  //    {
  //    lstrcpy ( Err_BlkInObj, ObjName );
  //    }
  //  }
  //if ( Defect == 1 )
  //  pPos->VentZ = Fixed_Pos;
  //Vent = pPos->VentZ;
  //pPos->Vent = Vent;
//-------------------------------------------------
  if ( Задание.Use())
    Vent = Задание;
  if ( _0.On())
    Vent = 0.;
  if ( _50.On())
    Vent = 50.;
  if ( _100.On())
    Vent = 100.;
  if ( Defect == 1 )
    Vent = Fixed_Pos;
  if ( LocalBlk || pSys->IsBlk )
    {
    if ( Соленоид.Use() || Соленоид_2.Use())
      {
      bool On = false;
      if ( Prty_1 )
        {
        if ( Соленоид.On() || Соленоид_2.On())
          Vent = 100.;
        else
          Vent = 0.;
        }
      else
        {
        if ( Соленоид.Off() || Соленоид_2.Off())
          Vent = 0.;
        else
          Vent = 100.;
        }
      }
    }
  else
    {
    bool Err = false;
    if ( Соленоид.Use() || Соленоид_2.Use())
      {
      bool On = false;
      if ( Prty_1 )
        {
        if ( Соленоид.On() || Соленоид_2.On())
          {
          if ( Vent == 0.)
            Err = true;
          }
        else
          {
          if ( Vent > 0.)
            Err = true;
          }
        }
      else
        {
        if ( Соленоид.Off() || Соленоид_2.Off())
          {
          if ( Vent > 0.)
            Err = true;
          }
        else
          {
          if ( Vent == 0.)
            Err = true;
          }
        }
      }
    ErrInBlk = Err;
    if ( Err )
      {
      lstrcpy ( Err_BlkInObj, ObjName );
      }
    }
  if ( Defect == 1 )
    Vent = Fixed_Pos;
//  Vent = pPos->VentZ;
//  pPos->Vent = Vent;
  bool IsComp[K_GAS];
  double Cmol[K_GAS];
  CLEAR(IsComp)
  CLEAR(Cmol)
  double H_in = 0.;
  double  F_in = 0.; 
  for ( int n = 0; n < 3; n++ )
    {
    if ( !pFlow[n])
      KKK();
    if ( pFlow[n] && pFlow[n]->Flow_mol > 0. )
      {
      Add ( dt, IsComp, Cmol, H_in, F_in, pFlow[n] );
      }
    }
  double h = pWaterFlow->h_Flow;
  if ( F_in > 0.01 )
    {
    h = H_in / F_in;
    Norm( IsComp, Cmol );
    pOut->Set( IsComp, Cmol );
    }
  for ( int n = 0; n < 3; n++ )
    {
    if ( pFlow[n] && pFlow[n]->Flow_mol <= 0. )
      {
      pFlow[n]->h_Flow = h;
      pFlow[n]->ID_COMP = ID_Out;
      pFlow[n]->pComp = pOut;
      pFlow[n]->To_kg = Mw( pOut );
      }
    }
  if ( pFlow[1])
    {
    _Flow_1 = pFlow[1]->Flow_kg();
    _Flow_2 = pFlow[2]->Flow_kg();
    }
  Положение = Vent;//pPos->VentZ;
	}

void CKlap3x:: Add ( double dt, bool * IsComp, double * Cmol, double & H, double & F, CFlow * pFlow )
  {
  H += pFlow->Flow_mol * pFlow->h_Flow;
  F += pFlow->Flow_mol;
  CComp * pComp = pFlow->Get_Comp();
  if ( pComp == NULL )
    return;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if( pComp->IsComp[n] )
      {
      IsComp[n] = true;
      Cmol[n] += pFlow->Flow_mol * pComp->Cmol[n];
      }
    }
  }
