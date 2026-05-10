#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include "Tunning.h"
#include <ConnectY.h>
#include "ReadParms.h"

enum eType { L, M, H };

static CY_Create AS_H ( "AS-H", CY_AS_HML::Create, H );
static CY_Create AS_M ( "AS-M", CY_AS_HML::Create, M );
static CY_Create AS_L ( "AS-L", CY_AS_HML::Create, L );

CY_AS_HML::CY_AS_HML( char * _Name, int _Type, int number, int domain ) : CY_Base()
{
  Type = (eType)_Type;
  W = NULL; 
//
	IN1 = NULL;
	IN2 = NULL;
	IN3 = NULL;
	INT = NULL;
//
//
  Bad1 = Bad2 = Bad3 = false;
	Limits = false;
	UseSW_IN = true;
	SW_TO = -1;
	switch ( Type )
		{
		case H:
			{
      CAS_H * W_H = (CAS_H*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CAS_H::TypeID, number, domain );
      W = W_H;
			}
		break;
		case L:
			{
      CAS_L * W_L = (CAS_L*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CAS_L::TypeID, number, domain );
      W = W_L;
			}
		break;
		case M:
			{
      CAS_M * W_M = (CAS_M*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CAS_M::TypeID, number, domain );
      W = W_M;
			}
		break;
		default:
			ASS(FALSE)
		break;
		}
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
	OUT.Name = "OUT";
	OUT << W->MV;
	CY_Base::pMode = &W->mode;
	INFO(W->PV,"PV")
	INFO(SEL,"SEL")
	INFO(W->SW,"SW")
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
	ObrIn.pSH = &W->SH;
	ObrIn.pSL = &W->SL;
	ObrIn.pPV_Overshoot = &W->PV_Overshoot;
	ObrIn.pPV_Limit = &W->PV_Limit;
}

void CY_AS_HML::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_AS_HML,CAS_COMM,30,50)

void CY_AS_HML::InitParm()
  {
#include "Yokogawa\AS_P.hpp" 
#include "Yokogawa\AS.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

bool CY_AS_HML::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( !strncmp(ParmName, "NCCS", 4) )
  {
    char Fld[4][8] = { 0 };
    int K = SetFld_X( ParmValue, ':', Fld );
    ASS( K==2 );
    int pos = atoi( Fld[0] );
    ENCCS en = NC;
    if( 'C'==Fld[1][0] )
      en = C;
    switch( pos )
    {
    case 1:
      W->ENCCS1 = en;
      break;
    case 2:
      W->ENCCS2 = en;
      break;
    case 3:
      W->ENCCS3 = en;
      break;
    }
  }
  return __super::ReadParm(ParmName, ParmValue, pData);
}

bool CY_AS_HML::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool b = __super::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( CY_Base::s_ReadingTunningData && !strncmp(VarName, "RV", 2) && b )
  {
    int pos = atoi( VarName+2 );
    switch( pos )
    {
      case 1:
        *pVar = &W->RV1_const;
        break;
      case 2:
        *pVar = &W->RV2_const;
        break;
      case 3:
        *pVar = &W->RV3_const;
        break;
    }
    if( Work )
      *Work = evtP;
  }
  return b;
}

CSignal * CY_AS_HML::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_AS_HML::GetIN ( char * Name )
	{
	GET_IN(IN1)
	GET_IN(IN2)
	GET_IN(IN3)
	GET_IN(INT)
	return NULL;
	}
////////////////////////////////////////////////////////////////////////////////
//
void CY_AS_HML::Signal_selection( )
	{
///////////////////////////////////////////////
	W->mode.TRK = false;
///////////////////////////////////////////////
  if( NC==W->ENCCS1)
	  ObrIn.PV_Overshoot ( IN1, W->RV1, &Bad1, false );
  if( NC==W->ENCCS2)
	  ObrIn.PV_Overshoot ( IN2, W->RV2, &Bad2, false );
  if( NC==W->ENCCS3)
	  ObrIn.PV_Overshoot ( IN3, W->RV3, &Bad3, false );
//
	if ( W->SW <= 0 )
		return;
	if ( W->SW >= 1 && W->SW <= 3	)
		{
		SEL = W->SW;
		}
	else
		{
		bool Bad = Bad1 && Bad2 && Bad3;
		if ( Bad )
			{
			W->SW = 0;
			SEL = 0;
			}
		else
			{
			switch ( Type )
				{
				case H:
					SS_H( );
				break;
				case M:
					SS_M( );
				break;
				case L:
					SS_L( );
				break;
				}
			}
		}
//
	if ( SEL == 1 )
		{
		W->PV = W->RV1;
		return ;
		}
	if ( SEL == 2 )
		{
		W->PV = W->RV2;
		return ;
		}
	if ( SEL == 3 )
		{
		W->PV = W->RV3;
		return ;
		}
	}

void CY_AS_HML::Automatic_control_output_computation( )
	{
	MVn = ( W->MSH - W->MSL ) / ( W->SH - W->SL ) * ( W->PV - W->SL ) + W->MSL;
	}

void CY_AS_HML::Control_output_action()
	{
	}

void CY_AS_HML::Output_pushback()
	{
	double RV = ( W->SH - W->SL ) / ( W->MSH - W->MSL ) * ( W->MV - W->MSL ) + W->SL;
// Нужно только для входов, используемых в каскадных операциях
// см. p 619 ??
	//W->RV1 = W->RV2 = W->RV3 = RV;
	bool ManMode = W->mode.M == SMode::MAN;
	if ( IN1 )
		{
		//W->RV1 = RV;
		IN1 ->BackVar = RV;
		if ( W->mode.IMAN || ( W->SW != 4 && SEL != 1 ) || ManMode )
//		if ( W->mode.IMAN || SEL != 1 || ManMode )
//		if ( W->mode.IMAN || ManMode )
			IN1 ->Status |= CSignal::CND;
		else
			IN1 ->Status &= ~CSignal::CND;
		}
	if ( IN2 )
		{
		//W->RV2 = RV;
		IN2 ->BackVar = RV;
		if ( W->mode.IMAN || ( W->SW != 4 && SEL != 2 ) || ManMode )
//		if ( W->mode.IMAN || SEL != 2 || ManMode )
//		if ( W->mode.IMAN || ManMode )
			IN2 ->Status |= CSignal::CND;
		else
			IN2 ->Status &= ~CSignal::CND;
		}
	if ( IN3 )
		{
		//W->RV3 = RV;
		IN3 ->BackVar = RV;
		if ( W->mode.IMAN || ( W->SW != 4 && SEL != 3 ) || ManMode )
//		if ( W->mode.IMAN || SEL != 3 || ManMode )
//		if ( W->mode.IMAN || ManMode )
			IN3 ->Status |= CSignal::CND;
		else
			IN3 ->Status &= ~CSignal::CND;
		}
	}

void CY_AS_HML::Initialization_manual()
	{
	if ( OUT.Status & CSignal::CND )
		{
		W->mode.IMAN = true;
// Track output
		MVn = OUT.BackVar;
		}
	else
		W->mode.IMAN = false;
	}

void CY_AS_HML::SS_H( )
	{
	double V = -DBL_MAX;
	if ( !Bad1 ) 
		{
		V = W->RV1;
		SEL = 1;
		}
	if ( !Bad2 && W->RV2 > V ) 
		{
		V = W->RV2;
		SEL = 2;
		}
	if ( !Bad3 && W->RV3 > V ) 
		{
		SEL = 3;
		}
	}

void CY_AS_HML::SS_M( )
	{
// Анализ BAD
	int kBAD = 0;
	if ( Bad1 )
		kBAD++;
	if ( Bad2 )
		kBAD++;
	if ( Bad3 )
		kBAD++;
// kBAD == 0
	if ( kBAD == 0 )
		{
		int K1 = 0;
		int K2 = 0;
		int K3 = 0;
		if ( W->RV1 >= W->RV2 )
			K1++;
		else
			K2++;
		if ( W->RV1 >= W->RV3 )
			K1++;
		else
			K3++;
		if ( K1 == 1 )
			{
			SEL = 1;
			return;
			}
		if ( W->RV2 >= W->RV3 )
			K2++;
		else
			K3++;
		if ( K2 == 1 )
			{
			SEL = 2;
			return;
			}
		SEL = 3;
		return;
		}
// kBAD == 1
	if ( kBAD == 1 )
		{
		if ( Bad1 )
			{
			SEL = 2;
			return;
			}
		if ( Bad2 )
			{
			SEL = 1;
			return;
			}
		SEL = 2;
		return;
		}
// kBAD == 2
	if ( !Bad1 )
		{
		SEL = 1;
		return;
		}
	if ( !Bad2 )
		{
		SEL = 2;
		return;
		}
	SEL = 3;
	}

void CY_AS_HML::SS_L( )
	{
	double V = DBL_MAX;
	if ( !Bad1 ) 
		{
		V = W->RV1;
		SEL = 1;
		}
	if ( !Bad2 && W->RV2 < V ) 
		{
		V = W->RV2;
		SEL = 2;
		}
	if ( !Bad3 && W->RV3 < V ) 
		{
		SEL = 3;
		}
	}

void CY_AS_HML::Clamp( int DataStatus )
	{
	bool Clamp_1 = false;
	bool Clamp_2 = false;
	bool Clamp_3 = false;
	if (( OUT.Status & CSignal::CLPp ) || ( DataStatus & CSignal::CLPp ))
		{
		if ( W->SW == 4 )
			{
			Clamp_1 = true;
			Clamp_2 = true;
			Clamp_3 = true;
			}
		else
			{
			if ( W->SW == 1 )
				Clamp_1 = true;
			if ( W->SW == 2 )
				Clamp_2 = true;
			if ( W->SW == 3 )
				Clamp_3 = true;
			}
		}
	if ( IN1 )
    {
    if ( Clamp_1 )
		  IN1->Status |= CSignal::CLPp;
	  else
		  IN1->Status &= ~CSignal::CLPp;
    }
	if ( IN2 )
    {
    if ( Clamp_2 )
		  IN2->Status |= CSignal::CLPp;
	  else
		  IN2->Status &= ~CSignal::CLPp;
    }
	if ( IN3 )
    {
    if ( Clamp_3 )
		  IN3->Status |= CSignal::CLPp;
	  else
		  IN3->Status &= ~CSignal::CLPp;
    }
////////////////////////////////////////////////////////////
	Clamp_1 = false;
	Clamp_2 = false;
	Clamp_3 = false;
	if (( OUT.Status & CSignal::CLPm ) || ( DataStatus & CSignal::CLPm ))
		{
		if ( W->SW == 4 )
			{
			Clamp_1 = true;
			Clamp_2 = true;
			Clamp_3 = true;
			}
		else
			{
			if ( W->SW == 1 )
				Clamp_1 = true;
			if ( W->SW == 2 )
				Clamp_2 = true;
			if ( W->SW == 3 )
				Clamp_3 = true;
			}
		}
	if ( IN1 )
    {
    if ( Clamp_1 )
		  IN1->Status |= CSignal::CLPm;
	  else
		  IN1->Status &= ~CSignal::CLPm;
    }
	if ( IN2 )
    {
    if ( Clamp_2 )
		  IN2->Status |= CSignal::CLPm;
	  else
		  IN2->Status &= ~CSignal::CLPm;
    }
	if ( IN3 )
    {
    if ( Clamp_3 )
		  IN3->Status |= CSignal::CLPm;
	  else
		  IN3->Status &= ~CSignal::CLPm;
    }
	}

void CY_AS_HML::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}

	if ( OUT.Status & CSignal::OOP )
		Alarm.SEND_ON( A_OOP, -1. );
	else
		Alarm.SEND_OFF( A_OOP );
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
	Initialization_manual();
	if ( W->mode.M == SMode::MAN )
		{
		Signal_selection( );
    MVn = W->MV;
		}
	else
		{
		Signal_selection( );
		Automatic_control_output_computation( );
		}
	SW_TO = -1;
  if ( W ->SW == 0 )
    SEL = 0;
  W->SEL = SEL;
  if ( !_finite(W->MV))
    W->MV = 0.;
	dMV = MVn - W->MV;
	ObrOut.OutputProcessing( false );
  Clamp( ObrOut.DataStatus );
	Output_pushback();
  }

void CY_AS_HML::Init( )
	{
	}

static int C2 = 2;

int CY_AS_HML::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\AS_P.hpp" 
	Alarm.pAlarmLevel = &C2;
	Alarm.SetBlk( W );
  if( C==W->ENCCS1 )
    W->RV1 = W->RV1_const;
  if( C==W->ENCCS2 )
    W->RV2 = W->RV2_const;
  if( C==W->ENCCS3 )
    W->RV3 = W->RV3_const;
	return 0;
	}

bool CY_AS_HML::SetLimits( )
  {
  if ( IN1 )
    {
    IN1 ->Min = W->SL;
    IN1 ->Max = W->SH;
    IN1 ->IsLimits = true;
    }
  if ( IN2 )
    {
    IN2 ->Min = W->SL;
    IN2 ->Max = W->SH;
    IN2 ->IsLimits = true;
    }
  if ( IN3 )
    {
    IN3 ->Min = W->SL;
    IN3 ->Max = W->SH;
    IN3 ->IsLimits = true;
    }
  if ( !OUT.IsLimits )
    return false;
  /*W->MSH = W->MH = OUT.Max;
  W->MSL = W->ML = OUT.Min;*/
  return true;
  }


int CY_AS_HML::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->SEL == 0 )
		W->SW = 4;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

static LPCSTR s_Name = "W";
int CY_AS_HML::SaveState( )
	{
	OUT.SaveState();
	SAVE_CLASS(s_Name,CY_AS_HML_W)
	return 0;
	}

int CY_AS_HML::RestoreState ( char * StrName )
	{
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS(s_Name,CY_AS_HML_W)
	return 1;
	}

int CY_AS_HML::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  //OUT.StateSave( pStateSer );
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 4 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "SEL", SEL );
  WriteFullStruct( pStateSer, eKeyAnyVar, "PSW", PSW );
  WriteFullStruct( pStateSer, eKeyAnyVar, "MVn", MVn );
  return 0;
}

int CY_AS_HML::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  //OUT.StateRestore( pStateSer );
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 4 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "SEL", SEL );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "PSW", PSW );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "MVn", MVn );
  return 0;
}

int CY_AS_HML::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
  PARM_MODEFULL( W->mode, "#Mode" )
  PARM( W->PV, "#PV" )
  PARM( W->SW, "#W->SW" )
  PARM( SEL, "#SEL" )
  PARM( W->MV, "#MV" )
	PARM ( Limits, "#Задать пределы" );
	PARM ( SW_TO, "#Переключение" );
	PARM ( UseSW_IN, "Использовать входы SW" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\AS_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\AS.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN1)
	PARM_IN(IN2)
	PARM_IN(IN3)
	PARM_IN(INT)
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

