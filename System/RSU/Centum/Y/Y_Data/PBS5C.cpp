#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <SR_DCU.h>

static CY_Create PBS5C ( "PBS5C", CY_PBS5C::Create );

CY_PBS5C::CY_PBS5C( CPBS5C  * _W ) : CY_Base()
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
	kOut = 5;
	MV[0] = &W->MV01;
	MV[1] = &W->MV02;
	MV[2] = &W->MV03;
	MV[3] = &W->MV04;
	MV[4] = &W->MV05;
	oMV[0] = &oMV01;
	oMV[1] = &oMV02;
	oMV[2] = &oMV03;
	oMV[3] = &oMV04;
	oMV[4] = &oMV05;
	PV[0] = &W->PV01;
	PV[1] = &W->PV02;
	PV[2] = &W->PV03;
	PV[3] = &W->PV04;
	PV[4] = &W->PV05;
  PosZ = -1;
  SetoMV = false;
  m_timeHolder = 0;
  W->mode.M = SMode::MAN;
  memset( &Alarm.Possible, -1, sizeof(Alarm.Possible) );
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pObj = this;
}

void CY_PBS5C::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_PBS5C,CPBS5C,30,55)

void CY_PBS5C::InitParm()
  {
#include "Yokogawa\PBS5C_P.hpp" 
#include "Yokogawa\PBS5C.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_PBS5C::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_PBS5C::GetIN ( char * Name )
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
//
	GET_IN(INT)
//
	return NULL;
	}

void CY_PBS5C::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
	W->mode.M = SMode::MAN;
  m_timeHolder += dt;
  bExecutedAtStep = false;
  double fullDelta = m_timeHolder;
  if( m_timeHolder >= s_ControlPeriod )
  {
    bExecutedAtStep = true;
    m_timeHolder -= s_ControlPeriod;
  }
  else
    return;

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
/**/
// MV
	if ( W->Type_Action == 0 )//RADIO
		{
/****/
		int Change = -1;
		for ( int n = 0; n < nElements; n++ )
			{
			if ( *MV[n] && *MV[n] != *oMV[n] )
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
    if( W->Timer == 0. )
    {
      int Change = -1;
  		for ( int n = 0; n < nElements; n++ )
			{
			if( *MV[n] && *MV[n] != *oMV[n] )
				{
				Change = n;
				break;
				}
			}
	  	if( Change >= 0 )
			{
			  W->Timer = W->TimeW * 2.;
				*oMV[Change] = *MV[Change];
        for( int n = Change+1; n < nElements; n++ )
          *oMV[n] = *MV[n] = 0;
			}
			return ;
    }
		W->Timer -= fullDelta;
		if ( W->Timer > 0. )
			return ;
		W->Timer = 0.;
		for ( int n = 0; n < nElements; n++ )
		{
      if( !*oMV[n] )
        continue;
			*oMV[n] = *MV[n] = 0;
		}
		return;
	}
	if ( W->Type_Action == 2 )//ALTER
		{
      int Change = -1;
      for( int n = 0; n < nElements; n++ )
      {
        if( *MV[n] && *MV[n] != *oMV[n] )
        {
          Change = n;
          break;
        }
      }
      if( Change>=0 )
      {
        //*MV[Change] = !*oMV[Change];
        for ( int n = 0; n < nElements; n++ )
        {
          if( n!=Change )
            *MV[n] = 0;
          else
            *MV[n] = 1;
        }
      }
      for ( int n = 0; n < nElements; n++ )
      {
        *oMV[n] = *MV[n];
      }
		}
	}

void CY_PBS5C::Init( )
	{
	}

int CY_PBS5C::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\PBS5C_P.hpp" 
	return 0;
	}

int CY_PBS5C::Prepare1()
{
	Q[0] = Q01;
	Q[1] = Q02;
	Q[2] = Q03;
	Q[3] = Q04;
	Q[4] = Q05;
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

int CY_PBS5C::SaveState( )
{
	SAVE_OUT
	SAVE_CLASS("W",CY_PBS5C_W)
	return 0;
}

int CY_PBS5C::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	REST_OUT
	REST_CLASS("W",CY_PBS5C_W)
	return 1;
}

int CY_PBS5C::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 6 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV01", oMV01 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV02", oMV02 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV03", oMV03 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV04", oMV04 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "oMV05", oMV05 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Timer", Timer );

  return 0;
}

int CY_PBS5C::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 6 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV01", oMV01 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV02", oMV02 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV03", oMV03 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV04", oMV04 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "oMV05", oMV05 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Timer", Timer );

  return 0;
}


int CY_PBS5C::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h" 
	PARM( W->Type_Action,"#Тип	RADIO,MOMENT,ALTER" )
  PARM( PosZ, "#Позиция(0...4)" )
  PARM( Timer, "#ожидание" )
	MENU("Все параметры")
#include "Yokogawa\PBS5C_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\PBS5C.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(Q01)
	PARM_IN(Q02)
	PARM_IN(Q03)
	PARM_IN(Q04)
	PARM_IN(Q05)
	PARM_ALL_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_PBS5C::SetLimits( )
  {
  return true;
  }

struct SCharColor 
{ 
  char* name; 
  COLORREF color; DWORD index;
};

static SCharColor arrColors[] =
{
  { "N" , 0x000000, 0x2a },//0
  { "R" , 0x0000ff, 0x2b },//2
  { "G" , 0x00ff00, 0x2c },//4
  { "Y" , 0x00ffff, 0x2d },//6
  { "B" , 0xff0000, 0x2e },
  { "M" , 0xff00ff, 0x2f },
  { "C" , 0xffff00, 0x30 },
  { "W" , 0xffffff, 0x31 },
  { "SB", 0xb48246, 0x32 },
  { "PK", 0xcbc0ff, 0x33 },
  { "SG", 0x7fff00, 0x34 },
  { "OR", 0x00a5ff, 0x35 },
  { "YG", 0x32cd9a, 0x36 },
  { "VO", 0xee82ee, 0x37 },
  { "DB", 0xffbf00, 0x38 },
  { "GR", 0xc0c0c0, 0x41 },
  { NULL, 0x000000,  },
};

int CharToIndexColor( LPCSTR color )
{
  for( int n=0; arrColors[n].name; ++n )
  {
    if( !strcmp( color, arrColors[n].name ) )
      return n;
  }
  return -1;
}

bool CY_PBS5C::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
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

bool CY_PBS5C::SaveParms( HANDLE hFile )
{
  __super::SaveParms( hFile );
  DWORD Lw;
  WRT_DATA( sizeof(fpsw), &fpsw );

  return false;
}

bool CY_PBS5C::RestParms( HANDLE hFile )
{
  __super::RestParms( hFile );
  DWORD Lr;
  READ_DATA( sizeof(fpsw), &fpsw );
  return false;
}

bool CY_PBS5C::SetResetAlrmFromExtern( int alrmNumber, bool bSet )
{
  __int64 alrm = 0x0000000000000001LL << alrmNumber;
  if( bSet )
    Alarm.SEND_ON( alrm, -1. );
  else
    Alarm.SEND_OFF( alrm );
  return true;
}
