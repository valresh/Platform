#include "stdafx.h"
#include "Y_Data.h"
#include <SetDataTypes.h>
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include "GetObj.h"
#include <macros/StrHelps.h>
#include <macros/AutoRestore.h>

static CY_Create PI_HLD( "PI-HLD", CY_PI_HLD::Create );

bool CY_PI_HLD::s_globalApplyOutGainBias = true;

CY_PI_HLD_W::CY_PI_HLD_W()
{
  memset( this, 0, sizeof(*this));
}

CY_PI_HLD::CY_PI_HLD( CPI_HLD * _W ) : CY_Base()
{
  W = _W;
  bFirstCall = true;
  m_tSUB = eaodt_DataItem;
  m_SUB_Value = 0;
  CY_Base::InitFlagUse( W->FlagUse );
  InitData();
}

void CY_PI_HLD::OnReadTunningData()
{
  W->mode.M = SMode::MAN;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_PI_HLD,CPI_HLD,200,250)

void CY_PI_HLD::InitParm()
{
#include "Yokogawa\PI-HLD_P.hpp" 
#include "Yokogawa\PI-HLD.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

void CY_PI_HLD::InitData()
{
  CY_Base::pMode = &W->mode;
  CY_Base::pComm = W->Comment;
  //
  IN = NULL;
  SET = NULL;
  BIN = NULL;
  RL1 = NULL;
  RL2 = NULL;
  TIN = NULL;
  TSI = NULL;
  INT = NULL;
  OUT.Name = "OUT";
  OUT << W->MV;
  SUB.Name = "SUB";
  SUB << m_SUB_Value;
  /*Out_MV = false;
  PV_SV = false;
  StdLevIn = false;
  StdLevOut = false;
  StdFlowUp = false;
  StdFlowDown = false;*/
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
  ObrOut.pOutTrk = NULL;//&Out_Trk;
  ObrOut.pMode = &W->mode.M;
  ObrOut.pMAN_bypass = &W->MAN_bypass;
  ObrOut.pOut_VL = &W->Out_VL;
  ObrOut.pLimit_On_CLP = &W->Limit_On_CLP;
  ObrOut.pPositional = &W->Positional;
  ObrOut.OutputLimiter = true;
  //
  ObrIn.pSH = &W->SH;
  ObrIn.pSL = &W->SL;
  ObrIn.pPV_Overshoot = &W->PV_Overshoot;
  ObrIn.pPV_Limit = &W->PV_Limit;
  //	INFO(Mode,"Mode")
  INFO(W->PV,"PV")
  INFO(W->SV,"SV")
  INFO(W->MV,"MV")
  //
  Alarm.Possible = A_HHLL|A_HL|A_DV|A_MHL|A_VEL|A_CNF;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pAlarmLevel = &W->AlarmLevel;
  Alarm.pInput_Open_alarm = &W->Input_Open_alarm;
  Alarm.pHHLL = &W->HHLL;
  Alarm.pHL = &W->HL;
  //
  Alarm.pPV = &W->PV;
  Alarm.pSH = &W->SH;
  Alarm.pSL = &W->SL;
  Alarm.pH_Limit = &W->H_Limit;
  Alarm.pL_Limit = &W->L_Limit;
  Alarm.pSV = &W->SV;
  Alarm.pHH = &W->HH;
  Alarm.pLL = &W->LL;
  Alarm.pH = &W->PH;
  Alarm.pL = &W->PL;
  Alarm.pHYS_HL = &W->HYS_HL;
  //
  Alarm.pVL = &W->VL;
  Alarm.pSides_VL = &W->Sides_VL;
  Alarm.pdT_V_scan = NULL;//??&dT_V_scan;
  Alarm.pHYS_VL = &W->HYS_VL;
  //
  Alarm.pDeviation_DL = &W->Deviation_DL;
  Alarm.pDL = &W->DL;
  Alarm.pGAIN_DL = &W->GAIN_DL;
  Alarm.pTime_DL = &W->Time_DL;
  Alarm.pHYS_DL = &W->HYS_DL;
  //
  //-------------------------------------------
  Alarm.pMV = &W->MV;
  Alarm.pMH = &W->MH;
  Alarm.pML = &W->ML;
  Alarm.pMSH = &W->MSH;
  Alarm.pMSL = &W->MSL;
  Alarm.pHL_OUT = &W->HL_OUT;
  Alarm.pHYS_OUT = &W->HYS_OUT;
  Alarm.pObj = this;
  Alarm.pCALI = &W->CALI;
  //
  //
  Alarm.AF = &W->AF;
  //
  GP.Step = 1;
  GP.kPnt = 300;
  GP.pPVdata = &W->PV;
  GP.pSVdata = &W->SV;
  GP.pMVdata = &W->MV;
  GP.pP = &W->P;
  GP.pI = &W->I;
  GP.pD = &W->D;
  //dOpt = dMed = dMin = dMax = 0.;
  W->PMV = 0.;
  W->mode.M = SMode::MAN;
  W->m_mvForOutBySUBSYS = W->MV = 0.;
  //NoBack = false;
}

void CY_PI_HLD::ResetAlarms()
{
  Alarm.SetBlk( W );
}

CSignal * CY_PI_HLD::GetOUT ( char * Name )
{
  if ( strcmp ( Name, "OUT" ) == 0 )
    return &OUT;
  if ( strcmp ( Name, "SUB" ) == 0 )
    return &SUB;
  return NULL;
}

CSignal ** CY_PI_HLD::GetIN ( char * Name )
{
  GET_IN(IN)
  GET_IN(SET)
  GET_IN(BIN)
  GET_IN(RL1)
  GET_IN(RL2)
  GET_IN(TIN)
  GET_IN(TSI)
  GET_IN(INT)
  return NULL;
}

bool CY_PI_HLD::CreatePSMGraf(  struct CGrafParm ** pGP )
{
  if( pGP )
    *pGP = &GP;
  return true;
}

void CY_PI_HLD::Init( )
{
  CalcSUB();
  if( PW==W->OutputSignalConvertion )
    OUT << pwmUP;
  if( SUBSYS==W->OutputSignalConvertion )
  {
    OUT << W->m_mvForOutBySUBSYS;
    CalcOUT();
  }
}

int CY_PI_HLD::Prepare0()
{
  if ( W->MSH == DBL_MAX )
    W->MSH = 100.;
  if ( W->MSL == DBL_MAX )
    W->MSL = 0.;
  if( DBL_MAX==W->SH && DBL_MAX==W->SL )
  {
    W->SH = W->HH;
    W->SL = W->LL;
  }
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\PI-HLD_P.hpp" 

  /*if( !W->PIDD )
    W->DR = 1.;
  else
    W->DR = 0.;*/
  W->PSW = 0;

  return 0;
}

int CY_PI_HLD::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  W->notACK = 0;
  if ( W->P == 0. )
    W->P = 200.;
  if ( W->I == 0. )
    W->I = 10.;
  if ( GP.PV_min == 0. )
    GP.PV_min = W->SL;
  if ( GP.PV_max == 0. )
    GP.PV_max = W->SH;
  if ( GP.MV_min == 0. )
    GP.MV_min = W->MSL;
  if ( GP.MV_max == 0. )
    GP.MV_max = W->MSH;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::MAN;

  if( 3==W->InputSignalConversion )
    Alarm.GAINin = ObrIn.GAIN = W->Data_GAIN;
  if( PW==W->OutputSignalConvertion )
  {
    if( OUT.pTo[0] )
    {
      CY_Base * pObj = (*pFindDObj)( OUT.pTo[0], pDR->NumbFCS, pDR->domain );
      ASS( pObj );
      if( pObj )
      {
        CY_DI *pN = GetNextDI( pObj );
        if( pN )
        {
          CY_Base * pObj2 = (*pFindYObjN)( pN->Name, pN->NumbFCS, pDR->domain );
          ASS( pObj2 );
          if( pObj2 )
          {
            CSignal ** IN = pObj2->GetIN ( "IN" );
            OUT_DOWN << pwmDOWN;
            *IN = &OUT_DOWN;
            OUT_DOWN.pTo[0] = pN->Name;
            OUT_DOWN.pFrom = Name;
          }
        }
      }
    }
  }
  else
  {
    W->Positional = 1;
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
              m_tSUB = eaodt_ProcessIO;
              break;
            case id__Y:
              m_tSUB = eaodt_ProcessIO;
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

  if( !(W->HHLL&1) ) W->AF |= 0x00080000;//HH
  else W->AF &= ~0x00080000;
  if( !(W->HHLL&2) ) W->AF |= 0x00040000;//LL
  else W->AF &= ~0x00040000;
  if( !(W->HL&1) ) W->AF |= 0x00008000;//H
  else W->AF &= ~0x00008000;
  if( !(W->HL&2) ) W->AF |= 0x00004000;//L
  else W->AF &= ~0x00004000;

  return 0;
}

int CY_PI_HLD::SaveState( )
{
  SAVE_BASE_OUT
	SAVE_CLASS("W",CY_PI_HLD_W)
  ObrOut.SaveState( );
  return 0;
}

int CY_PI_HLD::RestoreState ( char * StrName )
{
  W->ALRM = 0;
  TAutoRestore<bool> curIsLimit(OUT.IsLimits);
	REST_BASE_OUT
	REST_CLASS("W",CY_PI_HLD_W)
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  return 1;
}

int CY_PI_HLD::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
	PARM ( W->Comment, "#Что это" );
	PARM_MODEFULL( W->mode, "#Mode" );
	PARM ( W->PV, "#PV" );
	PARM ( W->SV, "#SV" );
	PARM ( W->MV, "#MV" );
  PARM ( W->RMV,"#RMV" );
  PARM ( W->cMes, "#Размерность" );
	PARM(W->P, "P" ); //Proportional band  0 to 1000% 100%
	PARM(W->I, "I" ); //Integral time  0.1 to 10,000 seconds 20 seconds
	PARM(W->D, "D" ); //Derivative time  0 to 10,000 seconds 0 second
  PARM(W->PIDD, "PIDD" );
  PARM(W->trkError, "#trkError" ); 
	if ( pDR && pDR->FCS )
  {
		PARM_CALLBACK( pDR->FCS, "#FCS" );
  } 
  if( PW==W->OutputSignalConvertion )
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
  }
  W_PARM(SH)
  W_PARM(SL)
  W_PARM(HH)
  W_PARM(PH)
  W_PARM(PL)
  W_PARM(LL)
  W_PARM(MH)
  W_PARM(ML)
	/*MENU("Тип")
		PARM ( StdLevOut, "#Уровень слив");
		PARM ( StdLevIn, "#Уровень налив");
		PARM ( StdFlowUp, "#Рост при открытии");
		PARM ( StdFlowDown, "#Падение при открытии");
	E_MENU("Тип")*/
  PARM( W->Positional, "#Позиционный")
	PARM( OUT.BackVar, "#Возврат OUT" );
  //PARM( NoBack, "Не брать возврат")
	//PARM( Limits, "#Задать пределы" );
  //PARM( Out_MV, "#MV с выхода" );
  //PARM( PV_SV, "#PV->SV" );
	//PARM( Get_SV, "#Брать SV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\PI-HLD_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\PI-HLD.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN)
	PARM_IN(SET)
	PARM_IN(BIN)
	PARM_IN(RL1)
	PARM_IN(RL2)
	PARM_IN(TIN)
	PARM_IN(TSI)
	PARM_IN(INT)
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	MENU("График")
  PARM(	GP.PV_min, "GP.PV_min" )
  PARM(	GP.PV_max, "GP.PV_max" )
	PARM(	GP.MV_min, "GP.MV_min" ) 
  PARM(	GP.MV_max, "GP.MV_max" )
  PARM(	GP.Step, "GP.Step" )
  PARM(	GP.kPnt, "GP.kPnt" )
	E_MENU("График")
	/*TUNE_MODE
	PARM ( W->Tune_PV, "#Тип:PV" )
	PARM ( W->Tune_SV, "#Тип:SV" )
	PARM ( W->Tune_MV, "#Тип:MV" )*/
  return 0;
}

bool CY_PI_HLD::SetLimits( )
{
  if( IN )
  {
    IN->Min = W->SL;
    IN->Max = W->SH;
    IN->IsLimits = true;
  }
  if( SET )
  {
    SET ->Min = W->SL;
    SET ->Max = W->SH;
    SET ->IsLimits = true;
  }
  if( OUT.IsLimits )
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

int CY_PI_HLD::SetData( int TypeData, void * pData )
{
  return 0;
}
