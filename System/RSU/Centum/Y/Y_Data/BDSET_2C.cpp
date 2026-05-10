#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create BDSET_2C ( "BDSET-2C", CY_BDSET_2C::Create );

CY_BDSET_2C::CY_BDSET_2C( CBDSET_2C * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  Out[0].Name = "J01";
  Out[0].Set( W->DT[0] );
  Out[1].Name = "J02";
  Out[1].Set( W->DT[1] );
  Out[2].Name = "J03";
  Out[2].Set( W->DT[2] );
  Out[3].Name = "J04";
  Out[3].Set( W->DT[3] );
  Out[4].Name = "J05";
  Out[4].Set( W->DT[4] );
  Out[5].Name = "J06";
  Out[5].Set( W->DT[5] );
  Out[6].Name = "J07";
  Out[6].Set( W->DT[6] );
  Out[7].Name = "J08";
  Out[7].Set( W->DT[7] );
  Out[8].Name = "J09";
  Out[8].Set( W->DT[8] );
  Out[9].Name = "J10";
  Out[9].Set( W->DT[9] );
  Out[10].Name = "J11";
  Out[10].Set( W->DT[10] );
  Out[11].Name = "J12";
  Out[11].Set( W->DT[11] );
  Out[12].Name = "J13";
  Out[12].Set( W->DT[12] );
  Out[13].Name = "J14";
  Out[13].Set( W->DT[13] );
  Out[14].Name = "J15";
  Out[14].Set( W->DT[14] );
  Out[15].Name = "J16";
  Out[15].Set( W->DT[15] );
  kOut = _countof(Out);
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BDSET_2C,CBDSET_2C,40,60)

void CY_BDSET_2C::InitParm()
  {
#include "Yokogawa\BDSET_2C_P.hpp" 
#include "Yokogawa\BDSET_2C.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_BDSET_2C::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_BDSET_2C::GetIN ( char * Name )
	{
	return NULL;
	}

void CY_BDSET_2C::Work ( double dt )
{
  for( int i=0; i<_countof(ACT); ++i )
  {
    if( ACT[i]>0 )
      Out[i].Off( CSignal::_SkipTrasfer );
    else
      Out[i].On( CSignal::_SkipTrasfer );
  }
}

void CY_BDSET_2C::Init( )
	{
	}

void CY_BDSET_2C::OnAssigned( void *pAddr )
{
  if( !pExecConnectionTransfer )
    return;
  for( int i=0; i<_countof(ACT); ++i )
  {
    if( &ACT[i]!=pAddr )
      continue;
    if( ACT[i]<=0 )
      break;
    int st = Out[i].Status & CSignal::_SkipTrasfer;
    Out[i].Off( CSignal::_SkipTrasfer );
    pExecConnectionTransfer( Out[i].numConnection, 0, 'O' );
    if( st )
      Out[i].On( CSignal::_SkipTrasfer );
  }
}

int CY_BDSET_2C::Prepare0()
{
  memset( ACT, -1, sizeof(ACT) );
  for( int i=0; i<_countof(Out); ++i )
  {
    Out[i].On( CSignal::_SkipTrasfer );
  }
	return 0;
}

int CY_BDSET_2C::Prepare1()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\BDSET_2C_P.hpp" 
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

static LPCSTR s_ActName = "ACTs";
int CY_BDSET_2C::SaveState( )
{
  SAVE_OUT
  SAVE_CLASS(s_ActName,CY_BDSET_ACT)
	return 0;
}

int CY_BDSET_2C::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
 	REST_OUT
  REST_CLASS(s_ActName,CY_BDSET_ACT)
	return 1;
}

int CY_BDSET_2C::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	MENU("Параметры")
  char Name[32];
  for ( int n = 0; n < _countof(W->DT); n++ )
    {
    sprintf_s ( Name, "#DT%02d", n + 1 );
    PARMS( W->DT[n], Name );
    }
	E_MENU("Параметры")
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\BDSET_2C_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
  PARM( ACT[0], "#ACT.1" );
  PARM( ACT[1], "#ACT.2" );
  PARM( ACT[2], "#ACT.3" );
  PARM( ACT[3], "#ACT.4" );
  PARM( ACT[4], "#ACT.5" );
  PARM( ACT[5], "#ACT.6" );
  PARM( ACT[6], "#ACT.7" );
  PARM( ACT[7], "#ACT.8" );
  PARM( ACT[8], "#ACT.9" );
  PARM( ACT[9], "#ACT.10" );
  PARM( ACT[10], "#ACT.11" );
  PARM( ACT[11], "#ACT.12" );
  PARM( ACT[12], "#ACT.13" );
  PARM( ACT[13], "#ACT.14" );
  PARM( ACT[14], "#ACT.15" );
  PARM( ACT[15], "#ACT.16" );
#include "Yokogawa\BDSET_2C.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_BDSET_2C::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  if( !strcmp(VarName,"ACT") )
  {
    *pVar = ACT;
    *Type = 'I';
    if( pVarSize )
      *pVarSize = sizeof(ACT);
    return true;
  }
  return __super::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
}
