#include "stdafx.h"
#include "Y_Data.h"
#include <SetDataTypes.h>
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"
#include "GetObj.h"

bool CY_PVI::s_globalUseInGainBias = true;

static CY_Create PVI ( "PVI", CY_PVI::Create );

CY_PVI::CY_PVI( CPVI * _W ) : CY_Base()
{
  W = _W;
  m_tSUB = eaodt_DataItem;
  m_SUB_Value = 0;
  CY_Base::InitFlagUse( W->FlagUse );
  InitData( );
  Y_Type = id_PVI;
  void * pPV = GetValueAddr();
  KKK();
}

void CY_PVI::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_PVI,CPVI,51,71)

void CY_PVI::InitParm()
  {
#include "Yokogawa\PVI_P.hpp" 
#include "Yokogawa\PVI.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

#include <macros/StrHelps.h>
bool CY_PVI::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  bool b = __super::ReadParm( ParmName, ParmValue, pData );
  if( !strcmp(ParmName,"ESCL") )
  {
    char sz2[2][64] = {};
    int C = SplitString( ParmValue, sz2, ':', false);
    char* b = strchr( sz2[0], '.' );
    if( !b )
      W->nAfterDotPV = 0;
    else
      W->nAfterDotPV = (int)strlen( b+1 );
  }
  return b;
}

void CY_PVI::InitData( )
	{
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
//
	IN = NULL;
	OUT.Name = "OUT";
  OUT << W->PV;
	SUB.Name = "SUB";
  SUB << m_SUB_Value;
//
  ObrIn.pSH = &W->SH;
  ObrIn.pSL = &W->SL;
  ObrIn.pPV_Overshoot = &W->PV_Overshoot;
  ObrIn.pPV_Limit = &W->PV_Limit;
//
  Alarm.Possible = A_HHLL|A_HL|A_VEL|A_CNF;
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
//  Alarm.pSV = &W->SV;
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
	Alarm.pObj = this;
  Alarm.pCALI = &W->CALI;
//
  Alarm.AF = &W->AF;
//
	INFO(W->PV,"PV")
	dOpt = dMed = dMin = dMax = 0.;
	Y_Type = id_PVI;
  W->AF = 0;
  xInfo = 80;
  yInfo = -20;
  W->SUM = 0.;
  W->mode.M = SMode::AUT;
  UseAlg = 3;
	}

CSignal * CY_PVI::GetOUT ( char * Name )
	{
  if ( strcmp ( Name, "OUT" ) == 0 )
	  return &OUT;
  if ( strcmp ( Name, "SUB" ) == 0 )
    return &SUB;
  return NULL;
	}

CSignal ** CY_PVI::GetIN ( char * Name )
{
	GET_IN(IN)
	return NULL;
}

void CY_PVI::CalcSUB()
{
  if( eaodt_DataItem==m_tSUB )
  {
    switch( W->Aux )
    {
    case 0:
      m_SUB_Value = W->PV;
      break;
    case 1:
      m_SUB_Value = W->dPVn;
      break;
    }
  }
  else if( eaodt_ProcessIO==m_tSUB )
  {
    switch( W->Aux )
    {
    case 0:
      m_SUB_Value = (W->PV - W->SL) / (W->SH - W->SL) * 100.;
      break;
    case 1:
      m_SUB_Value = W->dPVn / (W->SH - W->SL) * 100.;
      break;
    }
  }
}

static const double To_h = 1. / 3600.;

void CY_PVI::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}

  if( s_globalUseInGainBias )
  {
    if( 1==W->InputSignalConversion )
    {
      if( UseAlg & 4 )
        Alarm.GAINin = ObrIn.GAIN = W->Data_GAINin;
      else
        Alarm.GAINin = ObrIn.GAIN = 1.;
      if( UseAlg & 8 )
        Alarm.BIASin = ObrIn.BIAS = W->Data_BIASin;
      else
        Alarm.BIASin = ObrIn.BIAS = 0;
    }
    else if( 3==W->InputSignalConversion )
    {
      if( UseAlg & 1 )
        Alarm.GAINin = ObrIn.GAIN = W->Data_GAINin;
      else
        Alarm.GAINin = ObrIn.GAIN = 1.;
      if( UseAlg & 2 )
        Alarm.BIASin = ObrIn.BIAS = W->Data_BIASin;
      else
        Alarm.BIASin = ObrIn.BIAS = 0;
    }
  }
  else
  {
    Alarm.GAINin = ObrIn.GAIN = 1.;
    Alarm.BIASin = ObrIn.BIAS = 0;
  }

	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
  }
  else
    OUT.Status &= ~CSignal::BAD;

  if( IN )
  {
    if( CSignal::Z_Connection & IN->Status )
      Alarm.Possible |= A_IOP;
    IN->Status &= ~CSignal::SIGNALS_MASK;
    Alarm.TestInScaled( IN, dt, OUT.Status, 0 );
	  ObrIn.WorkScaled( IN, W->PV, W->dPVn, 0, W->CALI );
  }
  else
  {
    Alarm.TestIn( NULL, dt, OUT.Status );
    W->FlagUse |= CAlarmBase::fuNoInput;
  }

  if( 0==W->nAfterDotPV )
  {
    double intptr = 0;
    double frac = modf( W->PV, &intptr );
    W->PV = intptr;
  }

	W->SUM += W->PV * dt * To_h;

  if( W->SH > W->SL )
  {
    double _A = ( W->PV - W->SL ) / ( W->SH - W->SL );
    W->RAW = _A * 100;
  }
  CalcSUB();
}

template<size_t _S>
void Out( double V, char (&Txt)[_S] )
{
  if ( !_finite ( V ))
  {
    strcpy_s( Txt, _S, "#" );
    return;
  }
  if ( V > 1e10 )
  {
    strcpy_s( Txt, _S, ">" );
    return;
  }
  if ( V < -1e10 )
  {
    strcpy_s( Txt, _S, "<" );
    return;
  }
  if ( V == 0. )
    strcpy_s( Txt, _S, "0" );
  else if ( fabs ( V ) < 1e-5 )
    sprintf_s( Txt, _S, "%13.5le", V );
  else if ( fabs ( V ) < 1e-4 )
    sprintf_s( Txt, _S, "%11.9lf", V );
  else if ( fabs ( V ) < 1e-3 )
    sprintf_s( Txt, _S, "%10.8lf", V );
  else if ( fabs ( V ) < 1e-2 )
    sprintf_s( Txt, _S, "%9.7lf", V );
  else if ( fabs ( V ) < 1e-1 )
    sprintf_s( Txt, _S, "%8.6lf", V );
  else if ( fabs ( V ) < 1. )
    sprintf_s( Txt, _S, "%7.5lf", V );
  else if ( fabs ( V ) < 10. )
    sprintf_s( Txt, _S, "%6.4lf", V );
  else if ( fabs ( V ) < 100. )
    sprintf_s( Txt, _S, "%5.3lf", V );
  else if ( fabs ( V ) < 1000. )
    sprintf_s( Txt, _S, "%5.2lf", V );
  else if ( fabs ( V ) < 10000. )
    sprintf_s( Txt, _S, "%5.1lf", V );
  else if ( fabs ( V ) > 10000000. )
    sprintf_s( Txt, _S, "%e", V );
  else
    sprintf_s( Txt, _S, "%5.0lf", V );
}

bool CY_PVI::Draw ( CDrawData * pDD, int x0, int y0 )
  {
  int OldCol; 
  if ( Alarm.State == 0 )
    OldCol = SetTextColor ( pDD->hDC, RGB ( 255, 255, 255 ));
  else
    {
    if (( Alarm.State & ~A_HL ) == 0 || ( Alarm.State & ~A_LO ) == 0 )
      OldCol = SetTextColor ( pDD->hDC, RGB ( 255, 255, 0 ));
    else
      OldCol = SetTextColor ( pDD->hDC, RGB ( 255, 0, 0 ));
    }
  SelectObject ( pDD->hDC, pDD->hFont );
  char Txt[255];
	Out( W->PV, Txt );
	TextOut ( pDD->hDC, x0, y0, Txt, (int)strlen ( Txt ));
  SetTextColor ( pDD->hDC, OldCol );
	return false;
  }

void CY_PVI::Init( )
{
  CalcSUB();
}

int CY_PVI::Prepare0()
	{
    if( DBL_MAX==W->SH && DBL_MAX==W->SL )
    {
      W->SH = W->HH;
      W->SL = W->LL;
    }
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\PVI_P.hpp" 
/////////////////////////////////////////////////
  return 0;
	}

bool CY_PVI::SetLimits( )
	{
  if ( IN )
    {
    IN ->Min = W->SL;
    IN ->Max = W->SH;
    IN ->IsLimits = true;
    }
  return true;
	}

int CY_PVI::Prepare1()
{
  W->SUM = 0.;
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
//
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
      {
        KKK();
      }
      else
      {
        switch( pDest->Y_Type )
        {
        case id__Z:
        case id__Y:
          m_tSUB = eaodt_ProcessIO;
          break;
        case id__XW:
        case id__WW:
        case id__WB:
          /*if( ectPAZ==pDest->controllerType )
            m_tSUB = eaodt_ProcessIO;*/
          break;
        case 0:
          if( !strcmp(pDest->Model, "PIO") )
          {
            CY_IO *pIO = (CY_IO*)pDest;
            switch( pIO->Type )
            {
            case id__WW:
            case id__XW:
              break;
            case id__Z:
              m_tSUB = eaodt_ProcessIO;
              break;
            case id__Y:
              m_tSUB = eaodt_ProcessIO;
              break;
            default:
              ASSD(!"разобраться");
            }
            KKK();
          }
          else
            KKK();
          break;
        default:
          ASSD(!"разобраться");
        }
      }
      CalcSUB();
    }
  }
  
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

int CY_PVI::SaveState( )
{
	SAVE_BASE_OUT
	return 0;
}

int CY_PVI::RestoreState ( char * StrName )
{
	W->ALRM = 0;
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
//  IGNORE
	REST_BASE_OUT
	return 1;
}

int CY_PVI::SetData( int TypeData, void * pData )
  {
  return 0;
  }

bool CY_PVI::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool bCommon = CY_Base::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( bCommon )
    return bCommon;

  if( !_strcmpi(VarName,"UseAlg") )
  {
    *Type = 'I';
    *pTuneVar = NULL;
    *Work = CY_Base::evtP;
    *pVar = &UseAlg;
    return true;
  }

  return false;
}

int CY_PVI::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->Comment, "#Что это" );
	if ( pDR && pDR->FCS )
		PARM_CALLBACK( pDR->FCS, "#FCS" );
	PARM ( W->PV, "#PV" );
	PARM ( W->HH, "#HH" )
	PARM ( W->PH, "#PH" )
	PARM ( W->PL, "#PL" )
	PARM ( W->LL, "#LL" )
  PARM ( W->Mes, "#iРазмерность" );
  PARM ( W->cMes, "#Размерность" );
  PARM ( UseAlg, "Способ расчета" )
  PARM ( s_globalUseInGainBias, "Использовать InGainBias")
  if( SUB.pTo[0] )
    PARM ( m_SUB_Value, "#Значение для SUB" )
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\PVI_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\PVI.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN)
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
//	TUNE_D(PV)
	PARM ( W->Tune_PV, "#Тип:PV" )
	return 0;
}

double * CY_PVI::GetValueAddr()
  {
  return &W->PV;
  }