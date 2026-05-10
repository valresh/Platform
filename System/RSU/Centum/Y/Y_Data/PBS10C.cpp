#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create PBS10C ( "PBS10C", CY_PBS10C::Create );

CY_PBS10C::CY_PBS10C( CPBS10C  * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	JMOD = NULL;
	JBST = NULL;
	JALM = NULL;
//
	Q01 = NULL;
	Q02 = NULL;
	Q03 = NULL;
	Q04 = NULL;
	Q05 = NULL;
  Q06 = NULL;
  Q07 = NULL;
  Q08 = NULL;
  Q09 = NULL;
  Q10 = NULL;
//
	INT = NULL;
//
	Out[0].Name = "B01";
	Out[0] << W->MV01;
	Out[1].Name = "B02";
	Out[1] << W->MV02;
	Out[2].Name = "B03";
	Out[2] << W->MV03;
	Out[3].Name = "B04";
	Out[3] << W->MV04;
	Out[4].Name = "B05";
	Out[4] << W->MV05;
  Out[5].Name = "B06";
  Out[5] << W->MV06;
  Out[6].Name = "B07";
  Out[6] << W->MV07;
  Out[7].Name = "B08";
  Out[7] << W->MV08;
  Out[8].Name = "B09";
  Out[8] << W->MV09;
  Out[9].Name = "B10";
  Out[9] << W->MV10;
  kOut = 10;

	MV[0] = &W->MV01;
	MV[1] = &W->MV02;
	MV[2] = &W->MV03;
	MV[3] = &W->MV04;
	MV[4] = &W->MV05;
  MV[5] = &W->MV06;
  MV[6] = &W->MV07;
  MV[7] = &W->MV08;
  MV[8] = &W->MV09;
  MV[9] = &W->MV10;
	oMV[0] = &oMV01;
	oMV[1] = &oMV02;
	oMV[2] = &oMV03;
	oMV[3] = &oMV04;
	oMV[4] = &oMV05;
  oMV[5] = &oMV06;
  oMV[6] = &oMV07;
  oMV[7] = &oMV08;
  oMV[8] = &oMV09;
  oMV[9] = &oMV10;
	PV[0] = &W->PV01;
	PV[1] = &W->PV02;
	PV[2] = &W->PV03;
	PV[3] = &W->PV04;
	PV[4] = &W->PV05;
  PV[5] = &W->PV06;
  PV[6] = &W->PV07;
  PV[7] = &W->PV08;
  PV[8] = &W->PV09;
  PV[9] = &W->PV10;
  PosZ = -1;
  SetoMV = false;
  W->mode.M = SMode::MAN;
  memset( &Alarm.Possible, -1, sizeof(Alarm.Possible) );
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pObj = this;
}

void CY_PBS10C::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_PBS10C,CPBS10C,60,100)

void CY_PBS10C::InitParm()
  {
#include "Yokogawa\PBS10C_P.hpp" 
#include "Yokogawa\PBS10C.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_PBS10C::GetOUT ( char * Name )
	{
	GET_STD_OUT
	/*GET_OUT(MV01)
	GET_OUT(MV02)
	GET_OUT(MV03)
	GET_OUT(MV04)
	GET_OUT(PV01)
	GET_OUT(PV02)
	GET_OUT(PV03)
	GET_OUT(PV04)*/
	return NULL;
	}

CSignal ** CY_PBS10C::GetIN ( char * Name )
	{
	GET_IN(JMOD)
	GET_IN(JBST)
	GET_IN(JALM)
//
	GET_IN(Q01)
	GET_IN(Q02)
	GET_IN(Q03)
	GET_IN(Q04)
	GET_IN(Q05)
  GET_IN(Q06)
  GET_IN(Q07)
  GET_IN(Q08)
  GET_IN(Q09)
  GET_IN(Q10)
//
	GET_IN(INT)
//
	return NULL;
	}

void CY_PBS10C::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
	W->mode.M = SMode::MAN;
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    Out[0].Status |= CSignal::BAD;
		return ;
    }
  else
    Out[0].Status &= ~CSignal::BAD;
// Входы
  if ( SetoMV )
    {
    SetoMV = false;
		for ( int n = 0; n < nElements; n++ )
			{
			*oMV[n] = *MV[n];
			}
    }
/**/
	for ( int n = 0; n < nElements; n++ )
		{
		if ( Q[n] == NULL )
			continue;
		if ( *Q[n] == 0 )
			*PV[n] = 0;
		else
			*PV[n] = 1;
		}
/**/
// MV
	if ( W->Type_Action == 0 )//RADIO
		{
/****/
		int Change = -1;
		for ( int n = 0; n < nElements; n++ )
			{
			if ( *MV[n] != *oMV[n] )
				{
				Change = n;
				break;
				}
			}
		if ( Change < 0 )
			return ;
		for ( int n = 0; n < nElements; n++ )
			{
			if ( n == Change )
				*MV[n] = *oMV[n] = 1;
			else
				*MV[n] = *oMV[n] = 0;
			}
/******/
		return ;
    }
	if ( W->Type_Action == 1 )//MOMENT
		{
		int Change = -1;
		for ( int n = 0; n < nElements; n++ )
			{
			if ( *MV[n] != *oMV[n] )
				{
				Change = n;
				break;
				}
			}
		if ( Change >= 0 )
			{
			W->Timer = W->TimeW * 2.;
			for ( int n = 0; n < nElements; n++ )
				{
				*oMV[n] = *MV[n];
				}
			return;
			}
		if ( Timer == 0. )
			return ;
		Timer -= dt;
		if ( Timer > 0. )
			return ;
		Timer = 0.;
		for ( int n = 0; n < nElements; n++ )
			{
			*oMV[n] = *MV[n] = 0;
			}
		return;
		}
	if ( W->Type_Action == 2 )//ALTER
		{
		}
	}

void CY_PBS10C::Init( )
	{
	}

int CY_PBS10C::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\PBS10C_P.hpp" 
	return 0;
	}

int CY_PBS10C::Prepare1()
{
	Q[0] = Q01;
	Q[1] = Q02;
	Q[2] = Q03;
	Q[3] = Q04;
	Q[4] = Q05;
  Q[5] = Q06;
  Q[6] = Q07;
  Q[7] = Q08;
  Q[8] = Q09;
  Q[9] = Q10;
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  if( W->Type_Action == 1 && Timer == 0. )
  {
    for( int n = 0; n < nElements; n++ )
    {
      *oMV[n] = *MV[n] = 0;
    }
  }
	return 0;
}

int CY_PBS10C::SaveState( )
{
	SAVE_OUT
	SAVE_CLASS("W",CY_PBS10C_W)
	return 0;
}

int CY_PBS10C::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	REST_OUT
	REST_CLASS("W",CY_PBS10C_W)
	return 1;
}

int CY_PBS10C::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 11 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV01", oMV01 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV02", oMV02 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV03", oMV03 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV04", oMV04 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV05", oMV05 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV06", oMV06 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV07", oMV07 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV08", oMV08 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV09", oMV09 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV10", oMV10 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Timer", Timer );

  return 0;
}

int CY_PBS10C::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 11 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV01", oMV01 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV02", oMV02 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV03", oMV03 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV04", oMV04 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV05", oMV05 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV06", oMV06 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV07", oMV07 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV08", oMV08 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV09", oMV09 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV10", oMV10 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Timer", Timer );

  return 0;
}

int CY_PBS10C::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM( W->Type_Action,"#Тип	RADIO,MOMENT,ALTER" )
  PARM( PosZ, "#Позиция(0...9)" )
  PARM( Timer, "#ожидание" )
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\PBS10C_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\PBS10C.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(Q01)
	PARM_IN(Q02)
	PARM_IN(Q03)
	PARM_IN(Q04)
	PARM_IN(Q05)
  PARM_IN(Q06)
  PARM_IN(Q07)
  PARM_IN(Q08)
  PARM_IN(Q09)
  PARM_IN(Q10)
	PARM_ALL_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_PBS10C::SetResetAlrmFromExtern( int alrmNumber, bool bSet )
{
  __int64 alrm = 0x0000000000000001LL << alrmNumber;
  if( bSet )
    Alarm.SEND_ON( alrm, -1. );
  else
    Alarm.SEND_OFF( alrm );
  return true;
}
