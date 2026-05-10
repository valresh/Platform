#include "stdafx.h"
#define DLL_Control
#include "Control.h"
#include "Data.h"
#include "Chem.h"
#include "EpsVol.h"



CShowMesValue::CShowMesValue()
  {
  Mes = 0;
  Value = 0.;
  MesList = NULL;
  MesName = NULL;
  pMain = NULL;
  pMes = NULL;
  }

#include "IO_Parms.h"
void CShowMesValue::GetParams( char * ParmName, bool ShowList )
  {
  if ( CParams::AddParmToList( ParmName ) )
    {
    CParams & Pv = CParams::Get();
    Pv.SetDefaultValue(NULL);
    Pv.Set( pMain, $ ParmName, Value, sizeof ( Value ) );
    Pv.pShowParam = this;
    Pv.Used = NULL;
    Pv.Property = false;
    }
  if ( ShowList )
    {
    CParams & Pm = CParams::Get();
    Pm.pDefValue = MesList;
    if ( pMes )
      Pm.SetAddr ( pMain, MesName, pMes, 'L', 4 );
    else
      Pm.SetAddr ( pMain, MesName, &Mes, 'L', 4 );
    Pm.Property = false;
    Pm.Used = NULL;
    }
  }

////////////////////////////////////////////////////////////////////////
CShowP::CShowP()
  {
  MesList = "атм;Па;КПа;МПа;мм в.ст.;мм рт.ст.;атм(а);Па(а);мм в.ст.(а);мм рт.ст.(а);";
  MesName = "Размерность давления"; 
  }

void CShowP::Init( IBaseModel * _pMain, double * _pPabs )
  {
  pMain = _pMain;
  pPabs = _pPabs;
  }

double CShowP::Show_Dbl ( struct CParams * Param )
  {
  double P = *pPabs - 1.;
  int M = Mes;
  if ( pMes )
    M = *pMes;
  switch ( M )
    {
    case 0: //атм;
      Value = P;
      break;
    case 1: //Па;
      Value = P * 98066.5;
      break;
    case 2: //КПа;
      Value = P * 98.0665;
      break;
    case 3: //МПа;
      Value = P * 0.0980665;
      break;
    case 4: //мм в.ст.;
      Value = P * 1e4;
      break;
    case 5: //мм рт.ст.;
      Value = P * 760.;
      break;
    case 6: //атм(а);
      Value = P + 1.;
      break;
    case 7: //Па(а);
      Value = ( P + 1. ) * 98066.5;
      break;
    case 8: //мм в.ст.(а);
      Value = ( P + 1. ) * 1e4;
      break;
    case 9: //мм рт.ст.(а);
      Value = ( P + 1. ) * 760.;
      break;
    }
  return Value;
  }
////////////////////////////////////////////////////////////////////////
CShowF::CShowF()
  {
  MesList = "кмоль/ч;кг/ч;м3/ч(г);м3/ч(ж);л/ч(ж);нм3/ч(г);тыс.нм3/ч(г);";
  MesName = "Размерность потока";
  pFlow = NULL;
  pState = NULL;
  Sign = ' ';
  }

void CShowF::Init(  IBaseModel * _pMain, CFlow * _pFlow )
  {
  pMain = _pMain;
  pFlow = _pFlow;
  }

double CShowF::Show_Dbl ( struct CParams * Param )
  {
  double F = 0.;
  if ( pFlow == NULL )
    {
    Value = 0.;
    return Value;
    }
  F = pFlow->Flow_mol;
  if ( Sign == '+' )
    F = -F;
  if ( pMes )
    Mes = *pMes;
  switch ( Mes )
    {
    case 0: //кмоль/ч;
      Value = F;
      break;
    case 1: //кг/ч;
      Value = F * Mw( pFlow->Get_Comp());
      break;
    case 2: //м3/ч(г);
      {
      if ( pState == NULL )
        pState = new CSEps ( pFlow->Name, pMain );
      pState->PH( pFlow );
      if ( pState->E == 0. )
        {
        Value = 0.;
        break;
        }
      Value = F * pState->E * ( RP * ( pState->T + TK )) / pFlow->P;
      }
      break;
    case 3: //м3/ч(ж);
      {
      if ( pState == NULL )
        pState = new CSEps ( pFlow->Name, pMain );
      pState->PH( pFlow );
      if ( pState->E == 1. )
        {
        Value = 0.;
        break;
        }
      double To_m3 = To_m3_liq( pState->T, pState->IsComp, pState->X_W );
      Value = F * ( 1 -  pState->E ) * To_m3;
      }
      break;
    case 4: //л/ч;
      {
      if ( pState == NULL )
        pState = new CSEps ( pFlow->Name, pMain );
      pState->PH( pFlow );
      if ( pState->E == 1. )
        {
        Value = 0.;
        break;
        }
      double To_m3 = To_m3_liq( pState->T, pState->IsComp, pState->X_W );
      Value = F * ( 1 -  pState->E ) * To_m3 * 1000.;
      }
      break;
    case 5: //нм3/ч;
      if ( pState == NULL )
        pState = new CSEps ( pFlow->Name, pMain );
      pState->PH( pFlow );
      if ( pState->E == 0. )
        {
        Value = 0.;
        break;
        }
      Value = F * pState->E * M3;
      break;
    case 6: //тыс.нм3/ч;
      if ( pState == NULL )
        pState = new CSEps ( pFlow->Name, pMain );
      pState->PH( pFlow );
      if ( pState->E == 0. )
        {
        Value = 0.;
        break;
        }
      Value = F * pState->E * M3 * 1e-3;
      break;
    }
  return Value;
  }
