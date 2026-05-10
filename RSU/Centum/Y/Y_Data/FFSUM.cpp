#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

#include <macros/StrHelps.h>
#include "GetObj.h"

static CY_Create FFSUM( "FFSUM", CY_FFSUM::Create );

CY_FFSUM::CY_FFSUM( CFFSUM * _W ) : CY_Base()
{
  W = _W;
  InitData();
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_FFSUM,CFFSUM,50,70)

void CY_FFSUM::InitParm()
{
#include "Yokogawa\FFSUM_P.hpp" 
#include "Yokogawa\FFSUM.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

void CY_FFSUM::InitData( )
{
  tSUB = eaodt_DataItem;
  SUB_Value = 0;

  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  IN = NULL;
  SET = NULL;
  BIN = NULL;
  TIN = NULL;
  TSI = NULL;
  INT = NULL;
  OUT.Name = "OUT";
  OUT << W->MV;
  SUB.Name = "SUB";
  SUB << SUB_Value;

  // ObrOut
  ObrOut.pMH = &W->MH;
  ObrOut.pML = &W->ML;
  ObrOut.pMSH = &W->MSH;
  ObrOut.pMSL = &W->MSL;
  ObrOut.pMV = &W->MV;
  ObrOut.pdMV = &dMV;
  //-----------------------------------
  ObrOut.OUT = &OUT;
  ObrOut.pIMAN = &W->mode.IMAN;
  ObrOut.pOutTrk = NULL;
  ObrOut.pMode = &W->mode.M;
  //ObrOut.OutputLimiter = true;
  //
  ObrIn.pSH = &W->SH;
  ObrIn.pSL = &W->SL;
  ObrIn.pPV_Overshoot = &W->PV_Overshoot;
  ObrIn.pPV_Limit = &W->PV_Limit;

  //
  Alarm.Possible = A_HHLL|A_HL|A_DV|A_MHL|A_VEL|A_CNF;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pAlarmLevel = &W->AlarmLevel;
  Alarm.pInput_Open_alarm = &W->Input_Open_alarm;
  //Alarm.pHHLL = &W->HHLL;
  //Alarm.pHL = &W->HL;
  //
  Alarm.pPV = &W->PV;
  Alarm.pSH = &W->SH;
  Alarm.pSL = &W->SL;
  Alarm.pH_Limit = &W->H_Limit;
  Alarm.pL_Limit = &W->L_Limit;
  Alarm.pSV = &W->SV;
  /*Alarm.pHH = &W->HH;
  Alarm.pLL = &W->LL;
  Alarm.pH = &W->PH;
  Alarm.pL = &W->PL;*/
  //Alarm.pHYS_HL = &W->HYS_HL;
  //
  //Alarm.pVL = &W->VL;
  //Alarm.pSides_VL = &W->Sides_VL;
  Alarm.pdT_V_scan = NULL;//??&dT_V_scan;
  //Alarm.pHYS_VL = &W->HYS_VL;
  //
  //Alarm.pTime_DL = &W->Time_DL;
  //Alarm.pHYS_DL = &W->HYS_DL;
  //
  //-------------------------------------------
  Alarm.pMV = &W->MV;
  Alarm.pMH = &W->MH;
  Alarm.pML = &W->ML;
  Alarm.pMSH = &W->MSH;
  Alarm.pMSL = &W->MSL;
  //Alarm.pHL_OUT = &W->HL_OUT;
  //Alarm.pHYS_OUT = &W->HYS_OUT;
  Alarm.pObj = this;
  //
  //
  Alarm.AF = &W->AF;
}

void CY_FFSUM::ResetAlarms()
{
  Alarm.SetBlk( W );
}

CSignal * CY_FFSUM::GetOUT ( char * Name )
{
  if ( strcmp ( Name, "OUT" ) == 0 )
    return &OUT;
  if ( strcmp ( Name, "SUB" ) == 0 )
    return &SUB;
  return NULL;
}

CSignal ** CY_FFSUM::GetIN ( char * Name )
{
  GET_IN(IN)
  GET_IN(SET)
  GET_IN(BIN)
  GET_IN(TIN)
  GET_IN(TSI)
  GET_IN(INT)
  return NULL;
}

int CY_FFSUM::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM ( W->Comment, "#Что это" );
  PARM_MODEFULL( W->mode, "#Mode" );
  PARM ( W->PV, "#PV" );
  PARM ( W->SV, "#SV" );
  PARM ( W->MV, "#MV" );
  /*PARM ( W->RMV,"#RMV" );
  //PARM ( MVOpt, "#MV opt" ); 
  //	PARM ( Mes, "#-Размерность" );
  PARM ( W->cMes, "#Размерность" );
  PARM(W->P, "P" ); //Proportional band  0 to 1000% 100%
  PARM(W->I, "I" ); //Integral time  0.1 to 10,000 seconds 20 seconds
  PARM(W->D, "D" ); //Derivative time  0 to 10,000 seconds 0 second
  PARM(W->PIDD, "PIDD" );
  PARM(W->Algorithm, "Algorithm" ); 
  PARM(nCtrlPeriodWork,"CtrlPeriodWork");
  PARM(dblInDt, "#входной dt");
  PARM((unsigned int&)nStepWork, "#шаг срабатывания");
  PARM(dMV, "#dMV");
  PARM(W->trkError, "#trkError" ); */
  if ( pDR && pDR->FCS )
  {
    PARM_CALLBACK( pDR->FCS, "#FCS" );
  } 
  /*if( PW==W->OutputSignalConvertion )
  {
    if( TIN && TIN->pDbl )
    {
      PARM( *TIN->pDbl, "#TIN" );
    }
    PARM ( pwmUP, "#UP" )
      PARM ( pwmDOWN, "DOWN" )
  } 
  if( SUBSYS==W->OutputSignalConvertion )
  {
    PARM ( s_globalApplyOutGainBias, "Применять_для_OUT_GainBias")
      PARM( W->m_mvForOutBySUBSYS, "#Значение_для_OUT" );
  }*/
  /*W_PARM(SH)
    W_PARM(SL)
    W_PARM(HH)
    W_PARM(PH)
    W_PARM(PL)
    W_PARM(LL)
    W_PARM(MH)
    W_PARM(ML)
    MENU("Тип")
    PARM ( StdLevOut, "#Уровень слив");
  PARM ( StdLevIn, "#Уровень налив");
  PARM ( StdFlowUp, "#Рост при открытии");
  PARM ( StdFlowDown, "#Падение при открытии");
  E_MENU("Тип")
    PARM( W->Positional, "#Позиционный")
    PARM( OUT.BackVar, "#Возврат OUT" );
  PARM( NoBack, "Не брать возврат")
    PARM( Limits, "#Задать пределы" );
  PARM( Out_MV, "#MV с выхода" );
  PARM( PV_SV, "#PV->SV" );
  PARM( Get_SV, "#Брать SV" );*/
  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\FFSUM_P.hpp" 
  E_MENU("Все параметры")
  MENU("Все состояние")
#include "Yokogawa\FFSUM.hpp"
  E_MENU("Все состояние")
  MENU("Входы")
  PARM_IN(IN)
  PARM_IN(SET)
  PARM_IN(BIN)
  PARM_IN(TIN)
  PARM_IN(TSI)
  PARM_IN(INT)
  PARM_OUT
  E_MENU("Входы")
  MENU("Base")
  CY_Base::GetParams( params );
  E_MENU("Base")
  return 0;
}

int CY_FFSUM::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\FFSUM_P.hpp" 
  return 0;
}

int CY_FFSUM::Prepare1()
{
  if( 3==W->InputSignalConversion )
  {
    Alarm.GAINin = ObrIn.GAIN = W->Data_GAIN;
    Alarm.BIASin = ObrIn.BIAS = W->Data_BIAS;
  }

  if( SUB.pTo[0] )
  {
    LPSTR pPoint = strchr( SUB.pTo[0], '.');
    TStringTerminator st( pPoint );
    if( pPoint )
      ++pPoint;
    CY_Base* pDest = GetObj( SUB.pTo[0], this );
    if( pDest )
    {
      if( id_PVI<=pDest->Y_Type && id__WW!=pDest->Y_Type && id__WB!=pDest->Y_Type && id__XW!=pDest->Y_Type )
      {}
      else
      {
        switch( pDest->Y_Type )
        {
        case 0:
          if( !strcmp(pDest->Model, "PIO") )
          {
            CY_IO *pIO = (CY_IO*)pDest;
            switch( pIO->Type )
            {
            case id__Z:
              tSUB = eaodt_ProcessIO;
              break;
            case id__Y:
              tSUB = eaodt_ProcessIO;
              break;
            default:
              ASSD(!"разобраться");
            }
          }
          break;
        default:
          ASSD(!"разобраться");
        }
      }
      CalcSUB();
    }
  }
  else
    OUT.Status &= ~CSignal::CND;

  return 0;
}

void CY_FFSUM::Init( )
{
  CalcSUB();
  if( SUBSYS==W->OutputSignalConvertion )
  {
    OUT << mvForOutBySUBSYS;
    CalcOUT();
  }
}

void CY_FFSUM::CalcSUB()
{
  if( eaodt_DataItem==tSUB )
  {
    switch( W->Aux )
    {
    case 0:
      SUB_Value = W->PV;
      break;
    case 1:
      SUB_Value = dPV;
      break;
    case 2:
      SUB_Value = W->MV;
      break;
    case 3:
      SUB_Value = dMV;
      break;
    }
  }
  else if( eaodt_ProcessIO==tSUB )
  {
    switch( W->Aux )
    {
    case 0:
      SUB_Value = (W->PV - W->SL) / (W->SH - W->SL) * 100.;
      break;
    case 1:
      SUB_Value = dPV / (W->SH - W->SL) * 100.;
      break;
    case 2:
      SUB_Value = (W->MV - W->MSL) / (W->MSH - W->MSL) * 100.;
      break;
    case 3:
      SUB_Value = dMV / (W->MSH - W->MSL) * 100.;
      break;
    }
  }
}

void CY_FFSUM::CalcOUT()
{
  if( SUBSYS!=W->OutputSignalConvertion )
    return;
  if( W->MV > W->MH )
    W->MV = W->MH;
  if( W->MV < W->ML )
    W->MV = W->ML;
  //if( s_globalApplyOutGainBias )
    mvForOutBySUBSYS = (W->MV - W->Conv_BIAS) / W->Conv_GAIN;
  /*else
    mvForOutBySUBSYS = W->MV;*/
}

bool CY_FFSUM::SetLimits( )
{
  if ( IN )
  {
    IN ->Min = W->SL;
    IN ->Max = W->SH;
    IN ->IsLimits = true;
  }
  if ( SET )
  {
    SET ->Min = W->SL;
    SET ->Max = W->SH;
    SET ->IsLimits = true;
  }
  if ( OUT.IsLimits )
  {
    if( W->MH > OUT.Max )
      W->MH = OUT.Max;
    if( W->ML < OUT.Min )
      W->ML = OUT.Min;
  }
  if( W->MSH == DBL_MAX && W->MH != DBL_MAX )
    W->MSH = W->MH;
  if( W->MSL == DBL_MAX && W->ML != DBL_MAX )
    W->MSL = W->ML;
  return true;
}

int CY_FFSUM::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 2 );
  /*WriteFullStruct( pStateSer, eKeyAnyVar, "VN", VN );
  WriteFullStruct( pStateSer, eKeyAnyVar, "RLV1", RLV1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "RLV2", RLV2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "En", En );
  WriteFullStruct( pStateSer, eKeyAnyVar, "En1", En1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dEn", dEn );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dEn1", dEn1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "PVn1", PVn1 );*/
  WriteFullStruct( pStateSer, eKeyAnyVar, "dPV", dPV );
  //WriteFullStruct( pStateSer, eKeyAnyVar, "dPVn1", dPVn1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  /*WriteFullStruct( pStateSer, eKeyAnyVar, "Deadband_state", Deadband_state );
  WriteFullStruct( pStateSer, eKeyAnyVar, "pwmUP", pwmUP );
  WriteFullStruct( pStateSer, eKeyAnyVar, "pwmDOWN", pwmDOWN );
  WriteFullStruct( pStateSer, eKeyAnyVar, "BSW", BSW );*/

  ObrOut.StateSave( pStateSer );
  return 0;
}

int CY_FFSUM::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 2 );

  /*ReadNamedFullStruct( pStateSer, eKeyAnyVar, "VN", VN );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "RLV1", RLV1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "RLV2", RLV2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "En", En );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "En1", En1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dEn", dEn );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dEn1", dEn1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "PVn1", PVn1 );*/
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dPV", dPV );
  //ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dPVn1", dPVn1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  /*ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Deadband_state", Deadband_state );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "pwmUP", pwmUP );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "pwmDOWN", pwmDOWN );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "BSW", BSW );*/

  ObrOut.StateRestore( pStateSer );
  return 0;
}

void CY_FFSUM::Work ( double dt )
{
  if( IN )
  {
    IN->Status &= ~CSignal::SIGNALS_MASK;
    ObrIn.Work( IN, W->PV, dPV, 0 );
  }

  switch( W->mode.M )
  {
  case SMode::AUT:
  case SMode::MAN:
    W->CSV = W->SV;
    break;
  case SMode::CAS:
    if( SET )
      *SET >> W->CSV;
  default:
    W->SV = W->CSV;
    if( W->SV < W->SVL )
      W->SV = W->SVL;
    if( W->SV > W->SVH )
      W->SV = W->SVH;
  }

  if( BIN )
    *BIN >> W->VN;
  if( TSI )
    *TSI >> W->TSW;

  if ( W->TSW )
    W->mode.TRK = true;
  else
    W->mode.TRK = false;

  double MVrb = OUT.BackVar;
  if ( TIN && W->mode.TRK )
    *TIN >> W->MV;

  dMV = W->MV - MVrb;

  double _MS = W->MSH - W->MSL;
  
  double CV = W->PV + W->CK * (W->VN + W->CB);

  double CVm = (_MS / (W->SH-W->SL) ) * (CV - W->SL) + W->MSL;
  double SVm = (_MS / (W->SSH-W->SSL) ) * (W->SV - W->SSL) + W->MSL;

  switch( W->mode.M )
  {
  case SMode::AUT:
  case SMode::CAS:
    {
      double BLn = 0;
      /*if( W->FSW )
        BLn = MVrb - CVm;
      else
        BLn = MVrb - SVm - CVm;*/
      W->MV = CVm + SVm + BLn;
    }
    break;
  }

  CalcOUT();
  CalcSUB();
}
