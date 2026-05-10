#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include "GetObj.h"
#include <macros/StrHelps.h>
#include "hardwarebp.h"

FILE * fopen_err ( char * File, char * Mode );

class KAutoDecrement
{
  int &val;
public:
  KAutoDecrement( int &v ) : val(v)
  {}
  ~KAutoDecrement()
  {
    --val;
    ASS( val>=0 );
  }
};

static CY_Create ST16 ( "ST16", CY_ST16::Create, CY_ST16::ST16 );
static CY_Create ST16E( "ST16E", CY_ST16::Create, CY_ST16::ST16E );

CY_ST16_W::CY_ST16_W()
{
  ZeroMemory( this, sizeof(*this) );
}

CY_ST16::CY_ST16( char * Name, int _Type, int number, int domain ) 
: CY_Base()
, pNextEx( NULL )
, pPrevEx( NULL )
, pOrgn( NULL )
, bRestoredState( false )
, m_LastWorkStep( 0 )
, m_nWorkIf0( 0 )
{
  ZeroMemory( nStepSaExec, sizeof(nStepSaExec) );
  Type = (eType)_Type; 
  switch( Type )
  {
  case CY_ST16::ST16:
    W = (CST16*)KMemoryServiceY::Instance().CreateObjectsStruct ( Name, CST16::TypeID, number, domain );
    break;
  case CY_ST16::ST16E:
    W = (CST16E*)KMemoryServiceY::Instance().CreateObjectsStruct ( Name, CST16E::TypeID, number, domain );
    break;
  default:
    ASS( FALSE );
    break;
  }
  
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	kOut = 0;
	CLEAR(Cond)
	CLEAR(Act)
	kOut = 0;
//
  CLEAR(MatrCond)
  CLEAR(MatrAct)
  CLEAR(bHasCondOnColumn)
  UseMatr = true;
  ActOnlyOnChange = false;
  //memset( ActState, ' ', sizeof(ActState) );
  CLEAR( YesAction );
  CLEAR(STEP);
  CLEAR(THEN);
  CLEAR(STEL);
  CLEAR(NEXT);
  CLEAR(PREV);
  CLEAR(ORGN);
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_ST16,CST16,30,50)
void SortST16Cond();

void CY_ST16::InitParm()
  {
    _W.mode.M = SMode::AUT;
#include "Yokogawa\ST16_P.hpp" 
#include "Yokogawa\ST16.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  SortST16Cond();
  }

#include <SR_DCU.h>

bool CY_ST16::SaveParms( HANDLE hFile )
{
  LPCSTR pLastStepVal = STEP[0];
  if( pLastStepVal[0] )
  {
    memcpy( curSTEP, pLastStepVal, sizeof(curSTEP) );
    for( int n=1; n<_countof(STEP); ++n )
    {
      if( !STEP[n][0] )
        memcpy( STEP[n], pLastStepVal, sizeof(STEP[n]) );
      else
        pLastStepVal = STEP[n];
    }
  }
  DWORD Lw;
  WRT_KEY(eYKeyST16begin)
//
  WRT_ARR(Cond)
  WRT_ARR(CondComm)
  WRT_ARR(MatrCond)
  WRT_ARR(Act)
  WRT_ARR(ActComm)
  WRT_ARR(MatrAct)
  WRT_VAR(TimeType)
  WRT_ARR(STEP)
  WRT_ARR(THEN)
  WRT_ARR(STEL)
  WRT_ARR(NEXT)
  WRT_ARR(PREV)
  WRT_ARR(ORGN)
  WRT_ARR(curSTEP);
  WRT_KEY(eYKeyST16end)
  return false;
}

bool CY_ST16::RestParms( HANDLE hFile )
{
  DWORD Lr;
  READ_KEY(eYKeyST16begin, false)
//
  READ_ARR(Cond)
  READ_ARR(CondComm)
  READ_ARR(MatrCond)
  READ_ARR(Act)
  READ_ARR(ActComm)
  READ_ARR(MatrAct)
  READ_VAR(TimeType)
  READ_ARR(STEP)
  READ_ARR(THEN)
  READ_ARR(STEL)
  READ_ARR(NEXT)
  READ_ARR(PREV)
  READ_ARR(ORGN)
  READ_ARR(curSTEP);
  READ_KEY(eYKeyST16end, false)
  return false;
}

bool CY_ST16::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( strstr ( ParmName, "TMNG" ))
  {
	  char Fld[20][256];
    int K = SetFld_X( ParmValue, ':', Fld );
    TimeType.StartTiming = Fld[1][0];
    TimeType.OutputTiming = Fld[2][0];
    if( K>3 )
      TimeType.ControlPeriod = atoi( Fld[3] );
    if( K>4 )
      TimeType.ControlPhase = atoi( Fld[4] );
    return true;
  }
  if ( strcmp ( ParmName, "COND" ) == 0 )
  {
	  char Fld[20][256];
    int K = SetFld_X( ParmValue, ':', Fld, false, false );
		int N = atoi ( Fld[1] ) - 1;
		ASS ( N >= 0 && N < _countof(Cond[N]) );
    if ( N >= _countof(Cond[N]) )
      return false;
		strcpy_s( Cond[N], Fld[2] );
    for( LPSTR p=Cond[N]; *p; ++p )
      *p = toupper(*p);
		strcpy_s( CondComm[N], Fld[3] );
		DWORD F = 0;
		DWORD M = 1;
		char * P = Fld[4];
    if ( UseMatr )
      {
      for ( int n = 0; n < 32; n++ )
        {
        char R = 'X';
        if ( *P == 'Y' )
          R = 'Y';
        if ( *P == 'N' )
          R = 'N';
        if ( *P == '.' )
          R = ' ';
        ASS( R != 'X' );
        MatrCond[N][n] = R;
        if ( n == 31 )
          break;
        P++;
        ASS ( *P == ',' );
        P++;
        }
//		  CondFlags[N] = 0;
      }
    else
      {
      while ( *P )
			  {
			  if ( *P == 'Y' )
				  {
				  F |= M;
          break;
				  }
			  ASS ( *P == '.' );
			  P++;
			  if ( *P == 0 )
				  break;
			  ASS ( *P == ',' );
			  P++;
			  M = M << 1;
			  }
//		  CondFlags[N] = F;
      }
    return true;
  }
  if ( strcmp ( ParmName, "ACTN" ) == 0 )
    {
	  char Fld[20][256];
    int K = SetFld_X( ParmValue, ':', Fld, false, false );
		int N = atoi ( Fld[1] ) - 1;
		ASS ( N >= 0 && N < _countof(Act) );
    if ( N >= _countof(Act) )
      return false;
		strcpy_s( Act[N], Fld[2] );
    for( LPSTR p=Act[N]; *p; ++p )
      *p = toupper(*p);
		strcpy_s ( ActComm[N], _countof(ActComm[N]), Fld[3] );
		DWORD F = 0;
		DWORD M = 1;
		char * P = Fld[4];
    if ( UseMatr )
      {
      for ( int n = 0; n < 32; n++ )
        {
        char R = 'X';
        if ( *P == 'Y' )
          R = 'Y';
        if ( *P == 'N' )
          R = 'N';
        if ( *P == '.' )
          R = ' ';
        ASS( R != 'X' );
        MatrAct[N][n] = R;
        if ( n == 31 )
          break;
        P++;
        ASS ( *P == ',' );
        P++;
        }
//		  ActFlags[N] = 0;
      }
    else
      {
      while ( *P )
			  {
			  if ( *P == 'Y' )
				  {
				  F |= M;
          break;
				  }
			  ASS ( *P == '.' );
			  P++;
			  if ( *P == 0 )
				  break;
			  ASS ( *P == ',' );
			  P++;
			  M = M << 1;
			  }
//		  ActFlags[N] = F;
      }
    return true;
    }
  if ( strcmp ( ParmName, "STEP" ) == 0 )
  {
    char Fld[eST16_Count+1][_countof(STEP[0])];
    int K = SetFld_X( ParmValue, ':', Fld );
    ASS( 1==atoi(Fld[0]) );
    CopyMemory( STEP, Fld[1], sizeof(STEP) );
    return true;
  }
  if ( strcmp ( ParmName, "THEN" ) == 0 )
  {
    char Fld[eST16_Count+1][_countof(THEN[0])];
    int K = SetFld_X( ParmValue, ':', Fld );
    ASS( 1==atoi(Fld[0]) );
    CopyMemory( THEN, Fld[1], sizeof(THEN) );
    return true;
  }
  if ( strcmp ( ParmName, "STEL" ) == 0 )
  {
    char Fld[eST16_Count+1][_countof(STEL[0])];
    int K = SetFld_X( ParmValue, ':', Fld );
    ASS( 1==atoi(Fld[0]) );
    CopyMemory( STEL, Fld[1], sizeof(STEL) );
    return true;
  }
  if ( strcmp ( ParmName, "NEXT" ) == 0 )
  {
    strcpy_s( NEXT, ParmValue );
    return true;
  }
  if ( strcmp ( ParmName, "PREV" ) == 0 )
  {
    strcpy_s( PREV, ParmValue );
    return true;
  }
  if ( strcmp ( ParmName, "ORGN" ) == 0 )
  {
    strcpy_s( ORGN, ParmValue );
    return true;
  }
  return CY_Base::ReadParm( ParmName, ParmValue, pData );
}

CSignal * CY_ST16::GetOUT ( char * Name )
{
	GET_STD_OUT
	return NULL;
}

CSignal ** CY_ST16::GetIN ( char * Name )
{
	return NULL;
}

bool CY_ST16::CallSA( LPCSTR pParam )
{
  LPCSTR pField = strchr(pParam,'.');
  LPCSTR pStep = strrchr(pParam,'.');
  if( !pStep )
    return false;
  if( pField!=pStep )
    pField += 1;
  else
    pField = pParam;

  bool bShiftCurStep = true;
  size_t lenF = pStep - pField;
  if( !strncmp(pField, "SA", 2) ){}
  else if( !strncmp(pField, "PV", 2) )
    bShiftCurStep = false;
  else
  {
    ASSD(0);
    return false;
  }

  ++pStep;
  
  if( m_nWorkIf0>=2 )
    return false;
  KAutoDecrement ad( m_nWorkIf0 );
  ++m_nWorkIf0;

  char szActiveCurStep[_countof(curSTEP)] = {};
  char szActivePV[_countof(W->PV)] = {};
  memcpy( szActiveCurStep, curSTEP, sizeof(szActiveCurStep) );
  memcpy( szActivePV, W->PV, sizeof(szActiveCurStep) );

  for( int s=0; s<_countof(STEP); ++s )
  {
    if( 0==memcmp(pStep, STEP[s],2) )
    {
      memcpy( curSTEP, pStep, __min( sizeof(curSTEP), strlen(pStep) ) );
      memcpy( W->PV, curSTEP, __min( sizeof(W->PV), sizeof(curSTEP) ) );
      if( SMode::AUT==W->mode.M || SMode::MAN==W->mode.M )
      {
        if( CY_Base::s_nCalcSteps && nStepSaExec[s] != CY_Base::s_nCalcSteps )
          ExecuteColumns( s, s+1 );
        nStepSaExec[s] = CY_Base::s_nCalcSteps;
        BYTE result = StateCond[s];
        if( result )
        {
          if( THEN[s][0] )
            memcpy( curSTEP, THEN[s], sizeof(curSTEP) );
        }
        else
        {
          if( STEL[s][0] )
            memcpy( curSTEP, STEL[s], sizeof(curSTEP) );
        }
        memcpy( W->PV, curSTEP, __min( sizeof(W->PV), sizeof(curSTEP) ) );
      }
    }
  }
  if( pNextEx )
    pNextEx->CallSA( pParam );
  /*if( !bShiftCurStep )
  {
    memcpy( curSTEP, szActiveCurStep, sizeof(curSTEP) );
    memcpy( W->PV, szActivePV, sizeof(W->PV) );
  }*/
  return true;
}

#ifndef _WIN64
static HardwareBreakpoint _hbp;
#endif

void CY_ST16::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}


  if( eTMNG_PeriodicExecution==TimeType.StartTiming )
  {
    if( W->mode.M != SMode::AUT )
    {
      W->mode.M = SMode::MAN;
      return;
    }
    if( 0==dt )
      return;//вызов, например, из ST16
    if( TimeType.ControlPeriod > 0 )
    {
      switch( ESCA )
      {
      default:
      case CY_Base::ESCA_ScanPeriod_basic:
        if( !CY_Base::s_flagBasicScanControl )
          return;
        if( m_nBasicControlPeriodCounter < TimeType.ControlPeriod )
          return;
        m_nBasicControlPeriodCounter = 0;
        break;
      case CY_Base::ESCA_ScanPeriod_medium:
        break;
      case CY_Base::ESCA_ScanPeriod_high:
        break;
      }
    }
  }
  else if( eTMNG_OneshotProcessing==TimeType.StartTiming )
  {
    if( !W->ACT )
      return;
    W->ACT = 0;
  }
  else if( eTMNG_StartupAtInitialColdStart_Restart==TimeType.StartTiming )
  {
    switch( W->mode.M )
    {
    case SMode::AUT:
      bRestoredState = false;
      break;
    /*case SMode::MAN:
      return;*/
    }
    if( bRestoredState )
      return;
    bRestoredState = true;
    W->mode.M = SMode::MAN;
  }
  else
  {
    return;
  }

	if( W->mode.M < SMode::ROUT )
    {
    Out[0].Status |= CSignal::BAD;
		return ;
    }
  else
    Out[0].Status &= ~CSignal::BAD;

  RefreshConds();

  if( m_nWorkIf0 )
    return;
  KAutoDecrement ad( m_nWorkIf0 );
  ++m_nWorkIf0;

  int m = MaxState;
  if( curSTEP[0] )
  {
    bool bAnyExec = false;
    for( int i=0; i<m; ++i )
    {
      if( '0'==STEP[i][0] && '0'==STEP[i][1] )
      {
        //ASSD(!"надо исполнить этот шаг");//еще его не встречал
        ExecuteColumns( i, i+1 );
        //ASSD( !THEN[i][0] );
        //ASSD( !STEL[i][0] );
        continue;
      }
      else if( curSTEP[0]==STEP[i][0] && curSTEP[1]==STEP[i][1] )
      {
        ExecuteColumns( i, i+1 );
        bAnyExec = true;
        BYTE result = StateCond[i];
        if( result )
        {
          if( THEN[i][0] )
            memcpy( curSTEP, THEN[i], sizeof(curSTEP) );
        }
        else
        {
          if( STEL[i][0] )
            memcpy( curSTEP, STEL[i], sizeof(curSTEP) );
        }
      }
      for( CY_Base* pNext = pOrgn; pNext; pNext = ((CY_ST16*)pNext)->pNextEx )
      {
        void * pVar = NULL;
        char Type = ' ';
        USHORT VarSize = 0;
        bool b = pNext->GetVar( "curSTEP", &pVar, NULL, &Type, NULL, NULL, &VarSize );
        ASS( b && Type=='S' );
        if( b && Type=='S' )
        {
          CopyMemory( pVar, curSTEP, __min(sizeof(curSTEP), VarSize) );
        }
      }
    }
    memcpy( W->PV, curSTEP, __min( sizeof(W->PV), sizeof(curSTEP) ) );
  }
  else
  {
    ExecuteColumns( 0, m );
  }
}

void RefreshActionState( CAct_ST16 &VarAct )
{
  if( !VarAct.pVar )
    return;
  switch( VarAct.VarType )
  {
  case 'I':
    {
      if( VarAct.dbgH_Type )
        VarAct.State = (VarAct.Iparam != *(int*)VarAct.pVar) ? 1 : 0;
      else
        VarAct.State = (VarAct.Iparam == *(int*)VarAct.pVar) ? 1 : 0;
    }
    break;
  case 'W':
  case 'B':
    {
      if( VarAct.dbgH_Type )
        VarAct.State = (VarAct.Iparam != *(BYTE*)VarAct.pVar) ? 1 : 0;
      else
        VarAct.State = (VarAct.Iparam == *(BYTE*)VarAct.pVar) ? 1 : 0;
    }
    break;
  case 'D':
    VarAct.State = (abs(VarAct.Dparam - *(double*)VarAct.pVar)<0.0001) ? 1 : 0;
    break;
  default:
    VarAct.State = -1;
  }
}

void CY_ST16::RefreshConds()
{

  for( int c = 0; c < MaxCond; ++c )
  {
    if( VarCond[c].pVar == NULL )
      continue;
    VarCond[c].Cond();
    if( VarCond[c].pFlagUse )
    {
      W->FlagUse |= (*VarCond[c].pFlagUse & CAlarmBase::fuYConnect);
      if( W->FlagUse & CAlarmBase::fuYConnect )
        *VarCond[c].pFlagUse |= CAlarmBase::fuYConnect;
    }
  }
  for ( int a = 0; a < MaxAct; a++ )
  {
    if ( VarAct[a].pVar == NULL )
      continue;
    if( VarAct[a].pFlagUse )
    {
      W->FlagUse |= (*VarAct[a].pFlagUse & CAlarmBase::fuYConnect);
      if( W->FlagUse & CAlarmBase::fuYConnect )
        *VarAct[a].pFlagUse |= CAlarmBase::fuYConnect;
    }
    RefreshActionState( VarAct[a] );
  }
}

void CY_ST16::ExecuteColumns( int from, int to )//текущая актуальная версия
{//сперва проверяются условия целиком
  m_LastWorkStep = CY_Base::s_nCalcSteps;

  char HActState[eST16_Count];
  memset( HActState, 0, _countof(HActState) );

  for ( int a = 0; a < MaxAct; a++ )
  {
    if ( VarAct[a].pVar == NULL )
      continue;
    VarAct[a].Active = false;
    if( VarAct[a].dbgH_Type )
      HActState[a] = ' ';
  }
  // Условия
  BYTE NewStateCond[_countof(StateCond)] = { 0 };
  BYTE stateChangedC[_countof(StateCond)] = { 0 };
  char bYonL[ _countof(VarAct) ];
  memset( bYonL, -1, _countof(bYonL) );
  char bConL[ _countof(VarAct) ];
  memset( bConL, -1, _countof(bConL) );
  ASSD( MaxAct <= _countof(bYonL) );

  for( int c = 0; c < MaxCond; ++c )
  {
    if( VarCond[c].pVar == NULL )
      VarCond[c].Yes = false;
    else
      VarCond[c].Cond();
  }

  to = __min( to, MaxState );
  for( int s=from; s<to; ++s )
  {
    char ActState[eST16_Count];
    memset( ActState, ' ', _countof(ActState) );

    char * Column = &MatrCond[0][s];
    StateCond[s] = 1;
    for( int c = 0; c < MaxCond; c++, Column += _countof(MatrCond[0]) )
    {
      if( *Column==' ' )
        continue;
      if( *Column == 'Y' )
      {
        if( VarCond[c].Yes )
        {
          StateCond[s] = VarCond[c].Yes;
          continue;
        }
        StateCond[s] = 0;
        break;
      }
      if ( *Column == 'N' )
      {
        if ( !VarCond[c].Yes )
          continue;
        StateCond[s] = 0;
        break;
      }
    }
    // Действия
    NewStateCond[s] = StateCond[s];
    if( ActOnlyOnChange )
    {
      if ( StateCond[s] == OldStateCond[s] )
        stateChangedC[s] = NewStateCond[s] = 0;
      else
        stateChangedC[s] = 1;
    }

    Column = &MatrAct[0][s];
    for( int a = 0; a < MaxAct; a++, Column += _countof(MatrAct[0]) )
    {
      if( !HActState[a] )
        VarAct[a].Active = false;

      if( !bHasCondOnColumn[s] )
      {
        if( VarAct[a].L_Type && *Column == 'Y' )
        {
          if( 1!=bYonL[a] )
            bYonL[a] = 0;
        }
      }
      else if ( !NewStateCond[s] )
      {
        if( *Column!=' ' && VarAct[a].L_Type )
        {
          if( 1!=bYonL[a] )
            bYonL[a] = 0;
          if( StateCond[s] && *Column == 'Y' )
            bYonL[a] = 1;
          if( ActOnlyOnChange && stateChangedC[s] )
            bConL[a] = 1;
        }
        continue;
      }
      if ( VarAct[a].pVar == NULL )
        continue;

      if( VarAct[a].L_Type )
        bConL[a] = 1;

      if ( *Column == 'Y' )
      {
        ActState[a] = 'Y';
        if( VarAct[a].L_Type )
          bYonL[a] = 1;
        continue;
      }
      if ( *Column == 'N' )
      {
        if ( ActState[a] != 'Y' )
          ActState[a] = 'N';
        continue;
      }
    }

    for( int a = 0; a < MaxAct; a++ )
    {
      if ( VarAct[a].pVar == NULL )
        continue;
      if ( ActState[a] == 'Y' )
      {
        if( HActState[a] )
          HActState[a] = ActState[a];
        if( !VarAct[a].L_Type )
          VarAct[a].Act( true, 0 );
        else if( bConL[a]>0 && bYonL[a]>0 )
        {
          VarAct[a].Act( true, 0 );
          bConL[a] = 0;
        }
      }
      else if ( ActState[a] == 'N' )
      {
        if( !HActState[a]  )
          VarAct[a].Act( false, 0 );
        else if( ' '==HActState[a] || 'N'==HActState[a] )
        {
          HActState[a] = ActState[a];
          VarAct[a].Act( false, 0 );
        }
      }
    }
    OldStateCond[s] = StateCond[s];
    for( int a = 0; a < MaxAct; a++ )
    {
      YesAction[a] = VarAct[a].Active;
    }
  }

  for( int a = 0; a < MaxAct; ++a )
  {
    if( 0==bYonL[a] )
      VarAct[a].SET_0( true );
    else if( bConL[a]>0 && bYonL[a]>0 )
      VarAct[a].Act( true, 0 );
  }

  for ( int a = 0; a < MaxAct; a++ )
  {
    if( !VarAct[a].pVar )
      continue;
    RefreshActionState( VarAct[a] );
  }
}

void CY_ST16::Init( )
{
}

int CY_ST16::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\ST16_P.hpp" 
	return 0;
	}

void GetST16_LC64CondFunc( char * Cond, CCond_ST16_LC64 * pVC, CY_Base *pObj );
void GetST16ActFunc( char * Cond, CAct_ST16 * pVC, CY_Base *pObj );
void CorrectST16ActFunc( char * Act, CAct_ST16 * pVA, CY_Base *pObj );

void CY_ST16::GetAllRef()
{
  for ( int n = 0; n < _countof(Cond); n++ )
  {
    if ( Cond[n][0] == 0 )
      continue;
		char * A = Cond[n];
    char * P = strchr ( A, '.' );
    ASS(P)
    *P = 0;
    CY_Base * pObj = GetObj ( A, this );
    *P = '.';
    if ( pObj == NULL )
      {
      VarCond[n].Name = Cond[n];
      VarCond[n].pVar = NULL;
      VarCond[n].VarType = ' ';
      VarCond[n].pFlagUse = NULL;
      continue;
      }
    P++;
    char sep = '.';
    char * Q = strchr ( P, sep );
    if( !Q)
    {
      sep = '=';
      Q = strchr ( P, '=' );
      ASS( Q );
      if( !Q )
        continue;
    }
    ASS(Q)
    *Q = 0;
    void * pVar = NULL;
    void * pTuneVar = NULL; 
    char Type = ' ';
    CY_Base::eVarType Work = CY_Base::evtP;
    CAlarmBase::tFlagUse_ *pFlagUse = NULL;
    ASS(pObj->GetVar ( P, &pVar, &pTuneVar, &Type, &Work, &pFlagUse ));
    bool bActOfCalc = false;
    if( !pFlagUse )
      pFlagUse = pObj->GetFlagUse();
    if( id_CALCU==pObj->Y_Type && !_strcmpi(P,"ACT") )
      bActOfCalc = true;
    *Q = sep;
    VarCond[n].Name = Cond[n];
    VarCond[n].pVar = pVar;
    VarCond[n].VarType = Type;
    VarCond[n].pFlagUse = pFlagUse;
    if( id__AN==pObj->Y_Type && pFlagUse )
      *pFlagUse |= CAlarmBase::fuYConnect;
    GetST16_LC64CondFunc( P, &VarCond[n], pObj );
    if( bActOfCalc && 1==VarCond[n].Iparam && &CCond_ST16_LC64::CompareInt==VarCond[n].pCIP )
      VarCond[n].pCIP = &CCond_ST16_LC64::CompareIntCalcuActOn;
  }

  for ( int n = 0; n < _countof(Act); n++ )
  {
    if ( Act[n][0] == 0 )
      continue;
    USHORT varSize = 0;
    CY_Base * pObj = NULL;
		char * A = Act[n];
    char * P = strchr ( A, '.' );
    ASS(P);
    {
      TStringTerminator st(P);
      pObj = GetObj ( A, this );
    }
    if ( pObj == NULL )
    {
      VarAct[n].Name = Act[n];
      VarAct[n].pVar = NULL;
      VarAct[n].VarType = ' ';
      VarAct[n].pFlagUse = NULL;
      continue;
    }
    P++;
    char * Q = strchr ( P, '.' );
    if( Q )
      *Q = 0;
    else
    {//xxx.PV=XCAL
      VarAct[n].Name = Act[n];
      VarAct[n].pVar = NULL;
      VarAct[n].VarType = ' ';
      VarAct[n].pFlagUse = NULL;
      continue;
    }
    void * pVar = NULL;
    void * pTuneVar = NULL; 
    char Type = ' ';
    CY_Base::eVarType Work = CY_Base::evtP;
    if( strcmp( P, "SA" ) == 0 )
    {
// Page D3-97, 
      pVar = NULL;
      Type = 'R'; // Run
    }
    else if( !strcmp( P, "PV" ) && (id_ST16==pObj->Y_Type || id_ST16E==pObj->Y_Type)  )
    {
      Type = 'R'; // Run
    }
    else
    {
      CAlarmBase::tFlagUse_ *pFlagUse = NULL;
      bool b = pObj->GetVar ( P, &pVar, &pTuneVar, &Type, &Work, &pFlagUse, &varSize );
      ASSD( b );
      if( b && !pFlagUse )
        pFlagUse = pObj->GetFlagUse();
      VarAct[n].pFlagUse = pFlagUse;
      if( id__AN==pObj->Y_Type && pFlagUse )
        *pFlagUse |= CAlarmBase::fuYConnect;
    }
    if( Q )
    {
      *Q = '.';
      Q++;
    }
    VarAct[n].Name = Act[n];
    VarAct[n].pVar = pVar;
    VarAct[n].VarType = Type;
    VarAct[n].varSize = varSize;
    GetST16ActFunc( P, &VarAct[n], pObj );
    if( VarAct[n].pPreActionApply==this )
      VarAct[n].pPreActionApply = NULL;
    CorrectST16ActFunc( P, &VarAct[n], pObj );
    if( Q && strcmp ( Q, "L" ) == 0 )
      VarAct[n].L_Type = true;
    else
      VarAct[n].L_Type = false;
  }
}

int CY_ST16::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if( !curSTEP[0] )
  {
    for( int n=0; n<_countof(STEP); ++n )
    {
      if( STEP[n][0] )
      {
        strcpy_s( W->PV, STEP[n] );
        break;
      }
    }
    if( !W->PV[0] )
      strcpy_s( W->PV, "00" );
  }
  if( eTMNG_OutputOnlyWhenConditionsChange==TimeType.OutputTiming )
    ActOnlyOnChange = true;
  if( eTMNG_OneshotProcessing==TimeType.StartTiming || eTMNG_StartupAtInitialColdStart_Restart==TimeType.StartTiming )
  {
    W->mode.M = SMode::MAN;
  }
  FindNearestSTs();
  
  GetAllRef();
  int MaxStateInActions = 0;
  MaxState = 0;
  MaxCond = 0;
  bool tHasCondOnColumn[_countof(bHasCondOnColumn)] = { false };
  for ( int c = 0; c < _countof(Cond); c++ )
  {
    if ( Cond[c][0] == 0 )
      continue;
    MaxCond = c + 1;
    for ( int s = 0; s < _countof(MatrCond[c]); s++ )
    {
      if ( MatrCond[c][s] == 'Y' || MatrCond[c][s] == 'N' )
      {
        if ( MaxState < s + 1 )
          MaxState = s + 1;
        tHasCondOnColumn[s] = true;
      }
    }
  }
  for ( int c = 0; c < _countof(Act); c++ )
  {
    if ( Act[c][0] == 0 )
      continue;
    for ( int s = 0; s < _countof(MatrAct[c]); s++ )
    {
      if ( MatrAct[c][s] == 'Y' || MatrAct[c][s] == 'N' )
      {
        if ( MaxStateInActions < s + 1 )
          MaxStateInActions = s + 1;
      }
    }
  }
  MaxState = __max( MaxState, MaxStateInActions );
  CopyMemory( bHasCondOnColumn, tHasCondOnColumn, sizeof(bHasCondOnColumn) );
  MaxAct = 0;
  for ( int a = 0; a < _countof(VarAct); a++ )
    {
    if ( VarAct[a].pVar == NULL )
      continue;
    MaxAct = a + 1;
    }
  for ( int c = 0; c < _countof(Cond[c]); c++ )
    {
    if ( Cond[c][0] == 0 )
      continue;
    if ( VarCond[c].pVar == NULL )
      {
      VarCond[c].pC = NULL;
      VarCond[c].Yes = false;
      continue;
      }
    VarCond[c].Cond();
    }
	return 0;
}

static LPCSTR s_Name = "W";
int CY_ST16::SaveState( )
{
	SAVE_OUT
	SAVE_CLASS( s_Name, CY_ST16_W )
	return 0;
}

int CY_ST16::RestoreState ( char * StrName )
{
  bRestoredState = true;
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	REST_OUT
	REST_CLASS( s_Name, CY_ST16_W )
	return 1;
}

int CY_ST16::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "OldStateCond", OldStateCond );
  WriteFullStruct( pStateSer, eKeyAnyVar, "curSTEP", curSTEP );
  return 0;
}

int CY_ST16::StateRestore( IYokoStateSer* pStateSer )
{
  bRestoredState = true;
  __super::StateRestore( pStateSer );
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "OldStateCond", OldStateCond );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "curSTEP", curSTEP );
  return 0;
}

int CY_ST16::GetParams( KYBridge2SysParam &params )
{
  char Name[256];

#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM( TimeType.StartTiming, "#StartTiming" );
  {
    static SYParam::SYParamStrVal vals[] = {{"Startup Restart", eTMNG_StartupAtInitialColdStart_Restart},
                                            {"PeriodicExecuti", eTMNG_PeriodicExecution},
                                            {"OneshotProcessi", eTMNG_OneshotProcessing},
                                            {"RestricInitlExe", eTMNG_RestrictedInitialExecution} };
    params.Add( TimeType.StartTiming, "#@StartTiming", _countof(vals), vals );
  }
  if( TimeType.OutputTiming > 0 )
  {
    PARM( TimeType.OutputTiming, "#OutputTiming" );
    {
      static SYParam::SYParamStrVal vals[] = {{"OutEchTiCndSati", eTMNG_OutputEachTimeConditionsSatisfied},
                                              {"OutOnlyCondChan", eTMNG_OutputOnlyWhenConditionsChange}};
      params.Add( TimeType.OutputTiming, "#@OutputTiming", _countof(vals), vals );
    }
  }
  PARM( TimeType.ControlPeriod, "#Control Period")
  PARM( m_LastWorkStep, "#Шаг Срабатывания")
  PARM( curSTEP, "#curSTEP" );

	MENU("Условия")
	for ( int n = 0; n < _countof(VarCond); n++ )
  {
    if ( VarCond[n].pVar == NULL )
      continue;
    sprintf_s( Name, "#%s", VarCond[n].Name );
    PARM( VarCond[n].Yes, Name )
	}
	E_MENU("Условия")
  PARM ( ActOnlyOnChange, "Действовать при изменениях" );
	MENU("Действия")
	for ( int n = 0; n < _countof(VarAct); n++ )
		{
    if ( VarAct[n].pVar == NULL )
      continue;
    sprintf_s( Name, "#%s", VarAct[n].Name );
	  PARM ( VarAct[n].Active, Name )
		}
	E_MENU("Действия")
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\ST16_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\ST16.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_ST16::SetLimits( )
 {
  return true;
 }

bool CY_ST16::Draw ( CDrawData * pDD, int x, int y )
{
	hWnd = pDD->hWnd;
	return true;
}

bool CY_ST16::CreatePSMGraf( struct CGrafParm ** GP )
{
	if ( pCreateST16Wnd )
		(*pCreateST16Wnd) ( Name, hWnd, this );	
	return true;
}

bool CY_ST16::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool bCommon = CY_Base::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( bCommon )
    return bCommon;
  if( !_strcmpi(VarName,"SA") )
  {
    *pVar  = &StateCond;
    *Type = 'W';
    if( pVarSize )
      *pVarSize = sizeof(StateCond);
    return true;
  }
  if( !_strnicmp(VarName,"SA.", 3) )
  {
    LPCSTR pszStep = VarName + 3;
    for( int s=0; s<_countof(STEP); ++s )
    {
      if( memcmp(pszStep, STEP[s],2) )
        continue;
      *pVar  = &StateCond[s];
      *Type = 'W';
      if( pVarSize )
      {
        for( int j=s; j<_countof(STEP); ++j )
        {
          if( memcmp(pszStep, STEP[j],2) )
            break;
          *pVarSize += 1;
        }
      }
      return true;
    }
    if( !pNextEx )
      FindNearestSTs();
    if( pNextEx )
      return pNextEx->GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
    OutputDebugString("!!!!! ST16 "); OutputDebugString( Name ); OutputDebugString(" нет переменной \""); OutputDebugString(VarName); OutputDebugString("\"\n");
    int Index = atoi( pszStep );
    if( Index < 1 || Index > _countof(StateCond) )
      return false;
    *pVar  = &StateCond[Index-1];
    *Type = 'W';
    if( pVarSize )
      *pVarSize = 1;
    OutputDebugString("!!!!! ST16 "); OutputDebugString( Name ); OutputDebugString(" подсовываю как индекс\n");
    return true;
  }
  if( !strcmp(VarName,"TMNG") )
  {
    *pVar = &TimeType;
    *Type = 'S';
    if( pVarSize )
      *pVarSize = sizeof(TimeType);
    return true;
  }
  if( !strcmp(VarName,"ActOnlyOnChange") )
  {
    *pVar = &ActOnlyOnChange;
    *Type = 'W';
    if( pVarSize )
      *pVarSize = sizeof(ActOnlyOnChange);
    return true;
  }
  if( !strcmp(VarName,"curSTEP") )
  {
    *pVar = &curSTEP;
    *Type = 'S';
    if( pVarSize )
      *pVarSize = sizeof(curSTEP);
    return true;
  }
  if( !strcmp(VarName,"_bRestoredState") )
  {
    *pVar = &bRestoredState;
    *Type = 'W';
    if( pVarSize )
      *pVarSize = sizeof(bRestoredState);
    return true;
  }
  if( !strcmp(VarName,"_StartTiming") )
  {
    *pVar = &TimeType.StartTiming;
    *Type = 'W';
    if( pVarSize )
      *pVarSize = sizeof(TimeType.StartTiming);
    return true;
  }
  return false;
}

bool CY_ST16::EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj )
{
  if( !ppName )
    return false;
  if( ppObj )
    *ppObj = NULL;
  *ppName = NULL;

  WORD con = HIWORD( pos );
  WORD act = LOWORD( pos );
  for( con; con<_countof(Cond); ++con )
  {
    if( !Cond[con][0] )
      continue;
    *ppName = Cond[con];
    ++con;
    pos = MAKELONG( act, con);
    return true;
  }
  for( act; act<_countof(Act); ++act )
  {
    if( !Act[act][0] )
      continue;
    *ppName = Act[act];
    ++act;
    pos = MAKELONG( act, con);
    return true;
  }
  return false;
}

void CY_ST16::FindNearestSTs()
{
  if( NEXT[0] )
    pNextEx = GetObj( NEXT, this );
  if( PREV[0] )
    pPrevEx = GetObj( PREV, this );
  if( ORGN[0] )
    pOrgn = GetObj( ORGN, this );
  if( pNextEx || pPrevEx )
  {
    ASSD( pOrgn );
  }
  if( Y_Type==id_ST16 )
  {
    CY_Base* pNext = pNextEx;
    while( pNext )
    {
      ASS( id_ST16E==pNext->Y_Type );
      if( id_ST16E!=pNext->Y_Type)
        break;
      void * pVar = NULL;
      char Type = ' ';
      USHORT VarSize = 0;
      bool b = pNext->GetVar( "TMNG", &pVar, NULL, &Type, NULL, NULL, &VarSize );
      ASS( b && Type=='S' && VarSize > 1 );
      if( b && Type=='S' )
      {
        CopyMemory( pVar, &TimeType, __min(sizeof(TimeType), VarSize) );
      }
      b = pNext->GetVar( "ActOnlyOnChange", &pVar, NULL, &Type, NULL, NULL, &VarSize );
      ASS( b && Type=='W' );
      if( b && Type=='W' )
      {
        *(bool*)pVar = ActOnlyOnChange;
      }
      b = pNext->GetVar( "curSTEP", &pVar, NULL, &Type, NULL, NULL, &VarSize );
      ASS( b && Type=='S' );
      if( b && Type=='S' )
      {
        CopyMemory( pVar, curSTEP, __min(sizeof(curSTEP), VarSize) );
      }

      LPSTR pszNext = ((CY_ST16*)pNext)->NEXT;
      if( !*pszNext )
        break;
      pNext = GetObj( pszNext, this );
    }
  }
}
