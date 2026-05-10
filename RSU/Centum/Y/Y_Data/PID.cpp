#include "stdafx.h"
#include "Y_Data.h"
#include <SetDataTypes.h>
#include "StdDef.h"
#include "Tunning.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include "GetObj.h"
#include <macros/StrHelps.h>
#include <macros/AutoRestore.h>

static CY_Create PID ( "PID", CY_PID::Create, CY_PID::PID );
static CY_Create PID_STC ( "PID-STC", CY_PID::Create, CY_PID::PID_STC  );
static CY_Create FF_PID ( "FF-PID", CY_PID::Create, CY_PID::FF_PID  );

bool CY_PID::s_globalApplyOutGainBias = true;

CY_PID_W::CY_PID_W()
{
	memset ( this, 0, sizeof ( CY_PID_W ));
}

CY_PID::CY_PID( char * _Name, int _Type, int number, int domain ) : CY_Base()
{
  bFirstCall = true;
  Type = (eType)_Type; 
  m_tSUB = eaodt_DataItem;
  m_SUB_Value = 0;
  mv4Out = DBL_MAX;
  switch( Type )
  {
  case CY_PID::PID:
    W = (CPID*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CPID::TypeID, number, domain );
	  break;
  case CY_PID::PID_STC:
	  W = (CPID*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CPID_STC::TypeID, number, domain );
	  break;
  case CY_PID::FF_PID:
    W = (CPID*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CFF_PID::TypeID, number, domain );
    break;
  default:
	  ASS( FALSE );
	  break;
  }
  CY_Base::InitFlagUse( W->FlagUse );
  InitData();
  nCtrlPeriodWork = 1;
}

void CY_PID::OnReadTunningData()
{
  W->mode.M = SMode::MAN;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_PID,CPID_COMM,103,107)

void CY_PID::InitParm()
{
#include "Yokogawa\PID_P.hpp" 
#include "Yokogawa\PID.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

void CY_PID::InitData( )
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
  OUT << mv4Out;//W->MV;
  SUB.Name = "SUB";
  SUB << m_SUB_Value;
  Out_MV = false;
  PV_SV = false;
  StdLevIn = false;
  StdLevOut = false;
  StdFlowUp = false;
  StdFlowDown = false;
// ObrOut
	ObrOut.pMH = &W->MH;
  ObrOut.pML = &W->ML;
	ObrOut.pMSH = &W->MSH;
  ObrOut.pMSL = &W->MSL;
	ObrOut.pMV = &mv4Out;//&W->MV;
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
  INFO(mv4Out,"mv4Out")
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
  Alarm.pMV = &mv4Out;//W->MV;
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
  GP.pMVdata = &mv4Out;//W->MV;
	GP.pP = &W->P;
  GP.pI = &W->I;
  GP.pD = &W->D;
	Limits = false;
	Get_SV = false;
	//dOpt = dMed = dMin = dMax = 0.;
  //MVOpt = 0.; 
  W->PMV = 0.;
  W->mode.M = SMode::MAN;
  W->m_mvForOutBySUBSYS = W->MV = 0.;
  NoBack = false;
}

void CY_PID::ResetAlarms()
{
  Alarm.SetBlk( W );
}

CSignal * CY_PID::GetOUT ( char * Name )
	{
  if ( strcmp ( Name, "OUT" ) == 0 )
	  return &OUT;
  if ( strcmp ( Name, "SUB" ) == 0 )
	  return &SUB;
  return NULL;
	}

CSignal ** CY_PID::GetIN ( char * Name )
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

int CY_PID::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM ( W->Comment, "#Что это" );
	PARM_MODEFULL( W->mode, "#Mode" );
	PARM ( W->PV, "#PV" );
	PARM ( W->SV, "#SV" );
	PARM ( W->MV, "#MV" );
  PARM ( mv4Out, "#mv4Out" );
  PARM ( W->RMV,"#RMV" );
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
	PARM( Get_SV, "#Брать SV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\PID_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\PID.hpp"
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
	PARM ( W->Tune_PV, "#Тип:PV" )
	PARM ( W->Tune_SV, "#Тип:SV" )
	PARM ( W->Tune_MV, "#Тип:MV" )
	return 0;
	}

bool CY_PID::CreatePSMGraf(  struct CGrafParm ** pGP )
	{
  if( pGP )
    *pGP = &GP;
	return true;
	}

void CY_PID::Init( )
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

int CY_PID::Prepare0()
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
#include "Yokogawa\PID_P.hpp" 
  //MH = MSH;
  //ML = MSL;

	if ( !W->PIDD )
		W->DR = 1.;
	else
		W->DR = 0.;
  W->PSW = 0;
  if( CY_PID::FF_PID==Type )
    W->HYS_HL = W->HYS_VL = W->HYS_DB = W->HYS_DL = W->HYS_OUT = 0.01;    

	return 0;
}


int CY_PID::Prepare1()
{
  if( mv4Out==DBL_MAX )
  {
    mv4Out = W->MV;
    if( /*user defined*/W->Display_style_MV && W->MSH > W->MSL && W->MVH>W->MVL )
    {
      double percentMV = mv4Out / (0.01 * ( W->MSH - W->MSL ));
      W->MV = ((W->MVH - W->MVL)*0.01)*percentMV + W->MVL;
    }
  }

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
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::MAN;

  if( 3==W->InputSignalConversion )
  {
    Alarm.GAINin = ObrIn.GAIN = W->Data_GAIN;
    Alarm.BIASin = ObrIn.BIAS = W->Data_BIAS;
  }
  if( PW==W->OutputSignalConvertion )
  {
    if( OUT.pTo[0] )
    {
      CY_Base * pObj = (*pFindDObj)( OUT.pTo[0], pDR->NumbFCS, pDR->domain );
      if( !pObj )
      {
        LPSTR pszP = strchr( OUT.pTo[0], '.' );
        if( pszP )
        {
          TStringTerminator st( pszP );
          pObj = (*pFindDObj)( OUT.pTo[0], pDR->NumbFCS, pDR->domain );
        }
      }
      ASSD( pObj );
      if( pObj )
      {
        CY_DI *pN = GetNextDI( pObj );
        if( pN )
        {
          CY_Base * pObj2 = (*pFindYObjN)( pN->Name, pN->NumbFCS, pN->domain );
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

bool CY_PID::SetLimits( )
{
  if ( IN )
    {
    IN->Min = W->SL;
    IN->Max = W->SH;
    IN->IsLimits = true;
    }
  if ( SET )
    {
    SET->Min = W->SL;
    SET->Max = W->SH;
    SET->IsLimits = true;
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


static LPCSTR s_Name = "W";

int CY_PID::SaveState( )
	{
	SAVE_BASE_OUT
	SAVE_CLASS( s_Name, CY_PID_W)
  ObrOut.SaveState( );
  return 0;
	}

int CY_PID::RestoreState ( char * StrName )
{
	W->ALRM = 0;
  TAutoRestore<bool> curIsLimit(OUT.IsLimits);
	REST_BASE_OUT
	REST_CLASS( s_Name, CY_PID_W)
  if ( ObrOut.RestoreState ( StrName ) == 0 )
    return 0;
	return 1;
}

int CY_PID::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  //OUT.StateSave( pStateSer );
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 16 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "VN", VN );
  WriteFullStruct( pStateSer, eKeyAnyVar, "RLV1", RLV1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "RLV2", RLV2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "En", En );
  WriteFullStruct( pStateSer, eKeyAnyVar, "En1", En1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dEn", dEn );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dEn1", dEn1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "PVn1", PVn1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dPVn", dPVn );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dPVn1", dPVn1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Deadband_state", Deadband_state );
  WriteFullStruct( pStateSer, eKeyAnyVar, "pwmUP", pwmUP );
  WriteFullStruct( pStateSer, eKeyAnyVar, "pwmDOWN", pwmDOWN );
  WriteFullStruct( pStateSer, eKeyAnyVar, "BSW", BSW );
  WriteFullStruct( pStateSer, eKeyAnyVar, "mv4Out", mv4Out );

  ObrOut.StateSave( pStateSer );
  return 0;
}

int CY_PID::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  //OUT.StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  int c = 0;
  Read( pYokoStateSer, c );

  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "VN", VN );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "RLV1", RLV1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "RLV2", RLV2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "En", En );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "En1", En1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dEn", dEn );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dEn1", dEn1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "PVn1", PVn1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dPVn", dPVn );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dPVn1", dPVn1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Deadband_state", Deadband_state );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "pwmUP", pwmUP );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "pwmDOWN", pwmDOWN );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "BSW", BSW );
  if( c>=16 )
    ReadNamedFullStruct( pStateSer, eKeyAnyVar, "mv4Out", mv4Out );

  ObrOut.StateRestore( pStateSer );
  return 0;
}

int CY_PID::SetData( int TypeData, void * pData )
  {
  return 0;
  }

bool CY_PID::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  bool r = CY_Base::ReadParm( ParmName, ParmValue, pData );
  if( CY_PID::FF_PID==Type && !strcmp(ParmName,"FPRM") )
  {
    if( ParmValue==strstr(ParmValue,"CONTROL_OPTS") )
    {
      LPCSTR pSep = strchr( ParmValue, ':');
      for( int i=0; i<6; i++ )
      {
        if( !pSep )
          return false;
        pSep++;
        pSep = strchr( pSep, ':');
      }
      if( !pSep )
        return false;
      pSep--;
      switch( *pSep )
      {
      case '1':
      case '0':
        break;
      default:
        ASS( 0 );
        return false;
      }
      r = CY_Base::ReadParm( "PIDD", *pSep=='1' ? "DIRECT" : "REVERSE", pData );
    }
  }
  return r;
}

double * CY_PID::GetValueAddr()
{
  return &W->PV;
}

bool CY_PID::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool bCommon = __super::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( bCommon )
    return bCommon;
  if( !_strcmpi(VarName,"BSW") )
  {
    *pVar  = &BSW;
    *Type = 'W';
    if( pVarSize )
      *pVarSize = sizeof(BSW);
    if( pTuneVar )
      *pTuneVar = NULL;
    if( Work )
      *Work = CY_Base::evtWork;
    return true;
  }
  if( !strcmp(VarName,"STC") && id_PID_STC==W->ID_CLASS )
  {
    CPID_STC *pStc = (CPID_STC*)W;
    *pVar = &pStc->STC;
    *Type = 'D';
    if( pVarSize )
      *pVarSize = sizeof(pStc->STC);
    return true;
  }
  return false;
}
