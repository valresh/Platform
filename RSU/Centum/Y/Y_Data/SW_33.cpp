#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.35
static CY_Create SW_33_H ( "SW-33", CY_SW_33::Create );

CY_SW_33::CY_SW_33( CSW_33 * _W ) : CY_Base()
	{
	W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
// SEL(S11,S12,S13)->S10
	nS10 = NULL;
	nS11 = NULL;
	nS12 = NULL;
	nS13 = NULL;
// SEL(S21,S22,S23)->S30
	nS20 = NULL;
	nS21 = NULL;
	nS22 = NULL;
	nS23 = NULL;
// SEL(S31,S32,S33)->S30
	nS30 = NULL;
	nS31 = NULL;
	nS32 = NULL;
	nS33 = NULL;
// S10->S11,S12,S13
	iS10 = NULL;
	iS11 = NULL;
	iS12 = NULL;
	iS13 = NULL;
// S20->S21,S22,S23
	iS20 = NULL;
	iS21 = NULL;
	iS22 = NULL;
	iS23 = NULL;
// S30->S31,S32,S33
	iS30 = NULL;
	iS31 = NULL;
	iS32 = NULL;
	iS33 = NULL;
//
  Dir10 = esrUnknown;
  Dir20 = esrUnknown;
  Dir30 = esrUnknown;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_SW_33,CSW_33,10,20)

void CY_SW_33::InitParm()
  {
#include "Yokogawa\SW_33_P.hpp" 
#include "Yokogawa\SW_33.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }
/*
#undef SET_DIR
#define SET_DIR(Node,Dir) \
  if ( strcmp ( Name, #Node ) == 0 )\
    {\
    ASS ( Dir >= 0 );\
    Dir = esr0_from_123;\
    n##Node = pExt;\
    return true;\
    }

#undef SET_INV
#define SET_INV(Node,Dir) \
  if ( strcmp ( Name, #Node ) == 0 )\
    {\
    ASS ( Dir <= 0 );\
    Dir = esr0_to_123;\
    i##Node = pExt;\
    return true;\
    }*/
template<typename T>
bool SetImpl( LPCSTR pszNode, LPCSTR pszTestNode, CY_SW_33::eSignalRoute &Dir, T pExt, T * nS, CY_SW_33::eSignalRoute v )
{
  if( strcmp( pszNode, pszTestNode ) )
    return false;
  ASSD( Dir==v || Dir==CY_SW_33::esrUnknown );
  Dir = v;
  *nS = pExt;
  return true;
}
#undef SET_DIR
#define SET_DIR(Node,Dir) \
  if( SetImpl<CSignal**>( Name, #Node, Dir, pExt, &n##Node, CY_SW_33::esr0_from_123 ) ) return true;

#undef SET_INV
#define SET_INV(Node,Dir) \
  if( SetImpl<CSignal**>( Name, #Node, Dir, pExt, &i##Node, CY_SW_33::esr0_to_123 ) ) return true;

bool CY_SW_33::SetOUT ( char * Name, CSignal ** pExt )
  {
  SET_DIR(S10,Dir10)
  SET_DIR(S20,Dir20)
  SET_DIR(S30,Dir30)
  SET_INV(S11,Dir10)
  SET_INV(S12,Dir10)
  SET_INV(S13,Dir10)
  SET_INV(S21,Dir20)
  SET_INV(S22,Dir20)
  SET_INV(S23,Dir20)
  SET_INV(S31,Dir30)
  SET_INV(S32,Dir30)
  SET_INV(S33,Dir30)
  return false;
  }

#undef SET_DIR
#define SET_DIR(Node,Dir) \
  if( SetImpl<CSignal*>( Name, #Node, Dir, pExt, &n##Node, CY_SW_33::esr0_from_123 ) ) return true;

#undef SET_INV
#define SET_INV(Node,Dir) \
  if( SetImpl<CSignal*>( Name, #Node, Dir, pExt, &i##Node, CY_SW_33::esr0_to_123 ) ) return true;

bool CY_SW_33::SetIN ( char * Name, CSignal * pExt )
  {
  SET_INV(S10,Dir10)
  SET_INV(S20,Dir20)
  SET_INV(S30,Dir30)
  SET_DIR(S11,Dir10)
  SET_DIR(S12,Dir10)
  SET_DIR(S13,Dir10)
  SET_DIR(S21,Dir20)
  SET_DIR(S22,Dir20)
  SET_DIR(S23,Dir20)
  SET_DIR(S31,Dir30)
  SET_DIR(S32,Dir30)
  SET_DIR(S33,Dir30)
  return false;
  }

#undef GET_DIR
#define GET_DIR(Node,Dir) \
  if ( strcmp ( Name, #Node ) == 0 )\
    {\
    ASS ( (int)Dir >= esrUnknown );\
    Dir = esr0_from_123;\
    return &n##Node;\
    }

#undef GET_INV
#define GET_INV(Node,Dir) \
  if ( strcmp ( Name, #Node ) == 0 )\
    {\
    ASS ( (int)Dir <= esrUnknown );\
    Dir = esr0_to_123;\
    return &i##Node;\
    }

CSignal ** CY_SW_33::GetIN ( char * Name )
  {
  GET_INV(S10,Dir10)
  GET_INV(S20,Dir20)
  GET_INV(S30,Dir30)
  GET_DIR(S11,Dir10)
  GET_DIR(S12,Dir10)
  GET_DIR(S13,Dir10)
  GET_DIR(S21,Dir20)
  GET_DIR(S22,Dir20)
  GET_DIR(S23,Dir20)
  GET_DIR(S31,Dir30)
  GET_DIR(S32,Dir30)
  GET_DIR(S33,Dir30)
  return NULL;
  }

void On_IMAN ( CSignal * Sig )
  {
  if ( Sig )
  	Sig->Status |= CSignal::CND;
  }

void Off_IMAN ( CSignal * Sig )
  {
  if ( Sig )
    Sig->Status &= ~CSignal::CND;
  }

void CY_SW_33::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
  W->FlagUse |= CAlarmBase::fuYConnect;//пусть будет всегда доступным

	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
		return ;
    }
//	W->mode.M = SMode::AUT;
  if ( W->SW < 0 )
    W->SW = 0;
  if ( W->SW > 3 )
    W->SW = 3;
//
  if ( Dir10 < 0 )
    {
// Вход S10 -> на 3 выхода
    Transfer1to3( iS10, iS11, iS12, iS13 );
    }
  if ( Dir20 < 0 )
    {
 // Вход S20 -> на 3 выхода
    Transfer1to3( iS20, iS21, iS22, iS23 );
    }
  if ( Dir30 < 0 )
    {
    // Вход S30 -> на 3 выхода
    Transfer1to3( iS30, iS31, iS32, iS33 );
    }
//===================================
  if ( Dir10 > 0 )
    {
    switch ( W->SW )
      {
      case 1:
        if ( nS10 )
          *nS10 = nS11;
        Off_IMAN ( nS11 );
        On_IMAN ( nS12 );
        On_IMAN ( nS13 );
      break;
      case 2:
        if ( nS10 )
          *nS10 = nS12;
        On_IMAN ( nS11 );
        Off_IMAN ( nS12 );
        On_IMAN ( nS13 );
      break;
      case 3:
        if ( nS10 )
          *nS10 = nS13;
        On_IMAN ( nS11 );
        On_IMAN ( nS12 );
        Off_IMAN ( nS13 );
      break;
      }
    }
  if ( Dir20 > 0 )
    {
    switch ( W->SW )
      {
      case 1:
        if ( nS20 )
          *nS20 = nS21;
        Off_IMAN ( nS21 );
        On_IMAN ( nS22 );
        On_IMAN ( nS23 );
      break;
      case 2:
        if ( nS20 )
          *nS20 = nS22;
        On_IMAN ( nS21 );
        Off_IMAN ( nS22 );
        On_IMAN ( nS23 );
      break;
      case 3:
        if ( nS20 )
          *nS20 = nS23;
        On_IMAN ( nS21 );
        On_IMAN ( nS22 );
        Off_IMAN ( nS23 );
      break;
      }
    }
  if ( Dir30 > 0 )
    {
    switch ( W->SW )
      {
      case 1:
        if ( nS30 )
          *nS30 = nS31;
        Off_IMAN ( nS31 );
        On_IMAN ( nS32 );
        On_IMAN ( nS13 );
      break;
      case 2:
        if ( nS30 )
          *nS30 = nS32;
        On_IMAN ( nS31 );
        Off_IMAN ( nS32 );
        On_IMAN ( nS13 );
      break;
      case 3:
        if ( nS30 )
          *nS30 = nS33;
        On_IMAN ( nS31 );
        On_IMAN ( nS32 );
        Off_IMAN ( nS13 );
      break;
      }
    }
	}

void CY_SW_33::Init( )
	{
	}

int CY_SW_33::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\SW_33_P.hpp" 
  W->mode.M = SMode::MAN;
	return 0;
	}

int CY_SW_33::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::MAN;
	return 0;
}

int CY_SW_33::SaveState( )
{
	SAVE_CLASS("W",CY_SW_33_W)
	return 0;
}

int CY_SW_33::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_SW_33_W)
	return 1;
}

int CY_SW_33::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 3 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "OUT_1", OUT_1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "OUT_2", OUT_2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "OUT_3", OUT_3 );

  return 0;
}

int CY_SW_33::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 3 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "OUT_1", OUT_1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "OUT_2", OUT_2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "OUT_3", OUT_3 );

  return 0;
}

#undef PARM_OUT
#define PARM_OUT(Parm) \
	{\
	if ( Parm && *Parm && (*Parm)->pTo[0] )\
		{\
		char Name[256];\
		char Used = ' ';\
		if ( (*Parm)->Used() ) Used = '<';\
		sprintf_s ( Name, sizeof(Name), "#-%c%s", Used, (*Parm)->Name );\
		PARM((*Parm)->pTo[0], Name )\
		}\
	}

int CY_SW_33::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->SW, "#SW" );
	E_MENU("Параметры")
	MENU("Входы")
  if ( Dir10 > 0 )
    {
	  PARM_IN(nS11)
	  PARM_IN(nS12)
	  PARM_IN(nS13)
	  PARM_OUT(nS10)
    }
  if ( Dir10 < 0 )
    {
	  PARM_OUT(iS11)
	  PARM_OUT(iS12)
	  PARM_OUT(iS13)
	  PARM_IN(iS10)
    }
//
  if ( Dir20 < 0 )
    {
  	PARM_OUT(iS21)
	  PARM_OUT(iS22)
	  PARM_OUT(iS23)
	  PARM_IN(iS20)
    }
//
  if ( Dir30 < 0 )
    {
	  PARM_OUT(iS31)
	  PARM_OUT(iS32)
	  PARM_OUT(iS33)
	  PARM_IN(iS20)
    }
//
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\SW_33_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\SW_33.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

void CY_SW_33::Transfer1to3( CSignal * i0, CSignal ** o1, CSignal ** o2, CSignal ** o3 )
{
  switch( W->SW )
  {
  case 1:
    if( o1 )*o1 = i0;
    if( o2 )*o2 = NULL;
    if( o3 )*o3 = NULL;
    break;
  case 2:
    if( o1 )*o1 = NULL;
    if( o2 )*o2 = i0;
    if( o3 )*o3 = NULL;
    break;
  case 3:
    if( o1 )*o1 = NULL;
    if( o2 )*o2 = NULL;
    if( o3 )*o3 = i0;
    break;
  default:
    On_IMAN ( i0 );
  }
}

bool test( LPCSTR pNode, CY_SW_33::eSignalRoute &sr, LPCSTR pCon, char &t )
{
  if( strncmp(pCon, pNode, 2) )
    return false;
  if( CY_SW_33::esrUnknown==sr )
  {
    switch( pCon[2] )
    {
    case '0':
      if( 'O'==t )
        sr = CY_SW_33::esr0_from_123;
      else
        sr = CY_SW_33::esr0_to_123;
      break;
    default:
      if( 'I'==t )
        sr = CY_SW_33::esr0_from_123;
      else
        sr = CY_SW_33::esr0_to_123;
      break;
    }
  }
  else
  {
    switch( pCon[2] )
    {
    case '0':
      if( 'O'==t )
      {
        ASS( sr == CY_SW_33::esr0_from_123 );
      }
      else
      {
        ASS( sr == CY_SW_33::esr0_to_123 );
      }
      break;
    default:
      if( 'I'==t )
      {
        ASS( sr == CY_SW_33::esr0_from_123 );
      }
      else
      {
        if( sr != CY_SW_33::esr0_to_123 )
          t = 'I';
      }
      break;
    }
  }
  return true;
}

char CY_SW_33::TestCNCT( LPSTR pConn )
{
  char t = __super::TestCNCT(pConn);
  if( !test( "S10", Dir10, pConn, t) )
  {
    if( !test( "S20", Dir20, pConn, t) )
    {
      if( !test( "S30", Dir30, pConn, t) )
      {
        LPCSTR pPP = strchr(pConn, ':');
        size_t shift = pPP - pConn;
        if( 2==shift )
        {}
        else if( shift>2 )
        {
#pragma message( "!!!! отработать присвоение на примере 131 112TIC018SS\n")
          ASSD( '.'==pConn[2] );
          int val = atoi( pConn+3 );
          ASSD( val>0 && val<4 );
        }
        else
        {
          ASSD(!strncmp(pConn,"SW:", 3));
        }
      }
    }
  }
  return t;
}
