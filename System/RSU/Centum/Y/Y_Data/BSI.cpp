#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"
#include <SR_DCU.h>

static CY_Create BSI ( "BSI", CY_BSI::Create );

CY_BSI::CY_BSI( CBSI * _W )
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  W->mode.M = SMode::MAN;
  memset( &Alarm.Possible, -1, sizeof(Alarm.Possible) );
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pObj = this;

  W->BSTS.V = CBSI::_BSTS::STOP;

  PV[0] = &W->PV01;
  PV[1] = &W->PV02;
  PV[2] = &W->PV03;

  JMOD = NULL;
  JBST = NULL;
  JALM = NULL;

  SET = NULL;

  INT = NULL;

  Q01 = NULL;
  Q02 = NULL;
  Q03 = NULL;
}

void CY_BSI::ResetAlarms()
{
  Alarm.SetBlk( W );
}

void CY_BSI::Init( )
{
}

int CY_BSI::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\BSI_P.hpp" 
  return 0;
}

int CY_BSI::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;

  Q[0] = Q01;
  Q[1] = Q02;
  Q[2] = Q03;

  return 0;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BSI,CBSI,30,55)

void CY_BSI::InitParm()
{
#include "Yokogawa\BSI_P.hpp" 
#include "Yokogawa\BSI.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

extern int CharToIndexColor( LPCSTR color );

bool CY_BSI::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( strcmp(ParmName, "FPSW") )
    return __super::ReadParm( ParmName, ParmValue, pData );

  char Fld[6][16] = { 0 };
  int K = SetFld_X( ParmValue, ':', Fld );
  ASS( _countof(Fld)==K );
  K = atoi( Fld[0] );
  ASS( K );
  --K;
  ASS( K < _countof(fpsw) );
  SFPSW &fp = fpsw[K];
  if( !lstrcmpi( Fld[1], "LAMPBUTN") ) 
    fp.btnType = LAMPBUTN;
  else if( !lstrcmpi( Fld[1], "LAMP") ) 
    fp.btnType = LAMP;
  else if( !lstrcmpi( Fld[1], "BUTTON") ) 
    fp.btnType = BUTTON;
  else if( !lstrcmpi( Fld[1], "NO") ) 
    fp.btnType = NO;
  else
  {
    ASS(0);
  }
  strcpy_s( fp.szOff, Fld[2] );
  strcpy_s( fp.szOn, Fld[3] );
  fp.clrOff = CharToIndexColor( Fld[4] );
  ASS( -1!=fp.clrOff );
  fp.clrOn = CharToIndexColor( Fld[5] );
  ASS( -1!=fp.clrOn );
  return true;

}

bool CY_BSI::SaveParms( HANDLE hFile )
{
  __super::SaveParms( hFile );
  DWORD Lw;
  WRT_DATA( sizeof(fpsw), &fpsw );

  return false;
}

bool CY_BSI::RestParms( HANDLE hFile )
{
  __super::RestParms( hFile );
  DWORD Lr;
  READ_DATA( sizeof(fpsw), &fpsw );
  return false;
}

int CY_BSI::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  /*pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 6 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV01", oMV01 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV02", oMV02 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV03", oMV03 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV04", oMV04 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV05", oMV05 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Timer", Timer );*/

  return 0;
}

int CY_BSI::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  /*ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 6 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV01", oMV01 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV02", oMV02 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV03", oMV03 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV04", oMV04 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV05", oMV05 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Timer", Timer );*/

  return 0;
}

CSignal * CY_BSI::GetOUT ( char * Name )
{
  return NULL;
}

CSignal ** CY_BSI::GetIN ( char * Name )
{
  GET_IN(JMOD)
  GET_IN(JBST)
  GET_IN(JALM)

  GET_IN(SET)
  GET_IN(INT)
  //
  GET_IN(Q01)
  GET_IN(Q02)
  GET_IN(Q03)

  return NULL;
}

void CY_BSI::Work( double dt )
{
  if( !CY_Base::s_flagBasicScanControl )
    return;
  double deltaT = CY_Base::s_ControlPeriod * m_nBasicControlPeriodCounter;
  m_nBasicControlPeriodCounter = 0;

  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    return;
  }

  if( SET )
    W->SV = *SET;

  if( W->SV>0 && W->SV<=_countof(W->PHAS) )
    strcpy_s( W->PV, W->PHAS[W->SV-1] );

  for( int n = 0; n < nElements; n++ )
  {
    if( Q[n] == NULL )
      continue;
    if( *Q[n] == 0 )
      *PV[n] = 0;
    else
      *PV[n] = 1;
  }

  for( int i=0;i<_countof(fpsw); ++i )
  {
    SFPSW &fp = fpsw[i];
    if( LAMP!=fp.btnType && LAMPBUTN!=fp.btnType )
      continue;
    if( *PV[i] )
    {
      W->SWCR[i] = fp.clrOn;
      strcpy_s( W->SWLB[i], fp.szOn );
    }
    else
    {
      W->SWCR[i] = fp.clrOff;
      strcpy_s( W->SWLB[i], fp.szOff );
    }
  }
}

int CY_BSI::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h" 
/*PARM( W->Type_Action,"#Тип	RADIO,MOMENT,ALTER" )
  PARM( PosZ, "#Позиция(0...4)" )
  PARM( Timer, "#ожидание" )*/
  MENU("Все параметры")
#include "Yokogawa\BSI_P.hpp" 
  E_MENU("Все параметры")
  MENU("Все состояние")
#include "Yokogawa\BSI.hpp"
  E_MENU("Все состояние")
  /*MENU("Входы")
  PARM_IN(Q01)
  PARM_IN(Q02)
  PARM_IN(Q03)
  PARM_IN(Q04)
  PARM_IN(Q05)
  PARM_ALL_OUT
  E_MENU("Входы")*/
  MENU("Base")
  CY_Base::GetParams( params );
  E_MENU("Base")
  return 0;
}

bool CY_BSI::SetLimits( )
{
  return true;
}

bool CY_BSI::SetResetAlrmFromExtern( int alrmNumber, bool bSet )
{
  __int64 alrm = 0x0000000000000001LL << alrmNumber;
  if( bSet )
    Alarm.SEND_ON( alrm, -1. );
  else
    Alarm.SEND_OFF( alrm );
  return true;
}
