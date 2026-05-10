#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include <_CALCU.h>
#include <time.h>

CAlg * CAlg::pFirst = NULL;
CAlg * CAlg::pLast = NULL;

CAlg::CAlg( char * _Name )
: m_pOwner( NULL )
{
  Name = _Name;
  if ( pFirst == NULL )
    pFirst = this;
  if ( pLast )
    pLast->pNext = this;
  pLast = this;
  pNext = NULL;
  }

CAlg * CAlg::Find( char * Name )
  {
  CAlg * pAlg = pFirst;
  while ( pAlg )
    {
    if ( strcmp ( Name, pAlg->Name ) == 0 )
      return pAlg;
    pAlg = pAlg->pNext;
    }
  return NULL;
  }

void CAlg::GetParam( KYBridge2SysParam &params )
  {
  }

void CAlg::Work( struct CCALCU * W )
  {
  ASS(FALSE)
  }

void CAlg::Work( struct C_SFCSW * W, double dt )
{
  ASSD(FALSE)
}
void CAlg::Work( struct C_SFCPB * W, double dt )
{
  ASSD(FALSE)
}
void CAlg::Work( struct C_SFCAS * W, double dt )
{
  ASSD(FALSE)
}
void CAlg::Work( struct COPSFC * W, double dt )
{
  ASSD(FALSE)
}
void CAlg::Work( struct COPSBL * W, double dt )
{
  ASSD(FALSE)
}
void CAlg::Work( struct C_UTPB * W, double dt )
{
  ASSD(FALSE)
}
void CAlg::Work( struct C_UTSW * W, double dt )
{
  ASSD(FALSE)
}
void CAlg::OnStatusChange( struct C_SFCSW * W, double dt )
{
}
void CAlg::OnStatusChange( struct C_SFCPB * W, double dt )
{
}
void CAlg::OnStatusChange( struct C_SFCAS * W, double dt )
{
}
void CAlg::OnStatusChange( struct COPSFC * W, double dt )
{
}
void CAlg::OnStatusChange( struct COPSBL * W, double dt )
{
}
void CAlg::OnStatusChange( struct C_UTPB * W, double dt )
{
}
void CAlg::OnStatusChange( struct C_UTSW * W, double dt )
{
}
void CAlg::SetStep( int stepNo )
{
}
double CAlg::fabs( CRefAny &v )
{
  return ::fabs((double)v);
}
double CAlg::fabs( double &v )
{
  return ::fabs(v);
}
double CAlg::power( double v, double y )
{
  return ::pow( v, y );
}
double CAlg::dlimit( double v, double low, double hi )
{
  if( v < low )
    v = low;
  if( v > hi )
    v = hi;
  return v;
}
double CAlg::llimit( double v, double low, double hi )
{
  if( v < low )
    v = low;
  if( v > hi )
    v = hi;
  return v;
}
double CAlg::lmin( double l, double r)
{
  if( l < r )
    return l;
  return r;
}

double CAlg::dmax( int n, ... )
{
  double ma = -DBL_MAX;
  va_list vl;
  va_start( vl, n );
  for( n;n>0; --n )
  {
    CRefAny any = va_arg( vl, CRefAny);
    if( any > ma )
      ma = any;
  }
  va_end( vl );
  return ma;
}

double CAlg::lmax( int n, ... )
{
  double ma = -DBL_MAX;
  va_list vl;
  va_start( vl, n );
  for( n;n>0; --n )
  {
    CRefAny any = va_arg( vl, CRefAny);
    if( any > ma )
      ma = any;
  }
  va_end( vl );
  return ma;
}

double CAlg::dmin( int n,... )
{
  double mi = DBL_MAX;
  va_list vl;
  va_start( vl, n );
  for( n;n>0; --n )
  {
    CRefAny any = va_arg( vl, CRefAny);
    if( any < mi )
      mi = any;
  }
  va_end( vl );
  return mi;
}

double CAlg::dmax( double v1, double v2 )
{
  return __max( v1, v2 );
}

double CAlg::dmin( double v1, double v2 )
{
  return __min( v1, v2 );
}
double CAlg::lmax( double v1, double v2 )
{
  return __max( v1, v2 );
}

double CAlg::dmax( double v1, double v2, double v3 )
{
  double v12 = __max( v1, v2 );
  return __max( v12, v3 );
}

int CAlg::notInt( int v )
{
  return !v;
}

double CAlg::snum( CRefAny &v )
{
  double r = 0;
  r = atof( v.GetText() );
  return r;
}

int CAlg::len( CRefAny &v )
{
  size_t l = strlen( v.GetText() );
  return (int)l;
}

void CAlg::oneshot( CRefAny &obj )
{
}

void CAlg::REQUEST( ERequest eog, int n )
{
  if( !pSendRQ )
    return;
 pSendRQ( n );
}

void CAlg::OPEGUIDE( EOpeGuides eog, int n, int nFcs /*= -1*/ )
{
  if( !pSendOG )
    return;
  char szFcs[20] = { 0 };
  if( nFcs < 0 )
  {
    if( !m_pOwner || !m_pOwner->pDR )
      return;
    nFcs = m_pOwner->pDR->NumbFCS;
  }
  sprintf_s( szFcs, "FCS%04d", nFcs );
  pSendOG( szFcs, n );
}
void CAlg::OPEGUIDE( LPCSTR pszFormatString, int color, ... )
{
  if( !pSendOGraw || !pszFormatString )
    return;
  LPCSTR pszFcs = NULL;
  if( m_pOwner && m_pOwner->pDR )
    pszFcs = m_pOwner->pDR->FCS;
  char szBuf[512] = {};
  va_list args;
  va_start( args, color );
  int L = vsnprintf_s( szBuf, _countof(szBuf), _countof(szBuf)-1, pszFormatString, args );
  va_end(args);

  pSendOGraw( szBuf, color, pszFcs );
}

void CAlg::DIALOGUE( LPCSTR pszMsg, LPCSTR txt, int color, LPCSTR pszHeader, LPCSTR pszVariable )
{
  if( !pDialogue )
    return;
  pDialogue( pszMsg, txt, color, pszHeader, pszVariable );
}

void CAlg::MESSAGE( LPCSTR pTxt )
{
}

void CAlg::HISTORY( LPCSTR pszFormatString, ... )
{
}

int CAlg::GETTIME( int &HOUR, int &MIN, int &SEC )
{
  struct tm * pT = _localtime64( &CY_Base::s_ModelFullTimeT ); 
  if( !pT )
    return -1;
  SEC = pT->tm_sec;
  MIN = pT->tm_min;
  HOUR = pT->tm_hour;
  int r = HOUR * 60 *60 + MIN * 60 + SEC;
  r *= 1000;
  return r;
}

void CAlg::GETDATE( int &YEAR, int &MONTH, int &DAY)
{
  struct tm * pT = _localtime64( &CY_Base::s_ModelFullTimeT ); 
  if( !pT )
    return;
  DAY = pT->tm_mday;
  MONTH = pT->tm_mon + 1;
  YEAR = pT->tm_year - 100;/*+ 1900*/;
}

void CAlg::LOCALTIME( double gmt, int &YEAR, int &MONTH, int &DAY, int &HOUR, int &MIN, int &SEC )
{
  __time64_t T = gmt;
  struct tm * pT = _localtime64( &T );
  if( !pT )
    return;
  SEC = pT->tm_sec;
  MIN = pT->tm_min;
  HOUR = pT->tm_hour;
  DAY = pT->tm_mday;
  MONTH = pT->tm_mon + 1;
  YEAR = pT->tm_year - 100;
}

double CAlg::TIMEGMT()
{
  return CY_Base::s_ModelFullTimeT;
}

int CAlg::TIMEMS()
{
  struct tm * pT = _localtime64( &CY_Base::s_ModelFullTimeT ); 
  if( !pT )
    return 0;
  int SEC = pT->tm_sec;
  int MIN = pT->tm_min;
  int HOUR = pT->tm_hour;
  int ms = (HOUR * 3600 + MIN * 60 + SEC) * 1000;
  return ms;
}

void CAlg::ExecLC( CRefAny &tag )
{
}

int CAlg::SaveState()
{
  return 0;
}

int CAlg::RestoreState( char * StrName )
{
  return 1;
}
int CAlg::StateSave( IYokoStateSer* pStateSer )
{
  return 0;
}
int CAlg::StateRestore( IYokoStateSer* pStateSer )
{
  return 0;
}
int CAlg::StateSaveImpl( IYokoStateSer* pStateSer, void *pW, DWORD size )
{
  pStateSer->SimpleWrite( eKeyLocalStruct );
  pStateSer->WriteSized( pW, size );
  return 0;
}
int CAlg::StateRestoreImpl( IYokoStateSer* pStateSer, void *pW, DWORD size )
{
  EStateKeys key = sKeyNull;
  pStateSer->SimpleRead( key );
  if( eKeyLocalStruct==key )
  {}
  else
  {
    pStateSer->Shift( -LONG(sizeof(key)) );
    return 0;
  }
  DWORD savedSize = 0;
  pStateSer->SimpleRead( savedSize );
  ASSD( savedSize==size );
  if( savedSize!=size )
  {
    pStateSer->Shift( savedSize );
    return 0;
  }
  pStateSer->SimpleRead( pW,size );
  return 0;
}
////////////////////////////////////////////////////////////////////
CCond * CCond::pFirst = NULL;
CCond * CCond::pLast = NULL;

struct CTypeCond
  {
  char * Name;
  int ID;
  };

CTypeCond TypeCond[] = {
"MODE.MAN", 1,
"MODE.AUT", 2, 
"ALRM.IOP-", 3,
"PV.L", 4,
"ALRM.TRIP", 5,
"ALRM.IOP", 6,
"PV.0", 7,
"PSW.1", 8,
"PSW.2", 9,
"PV.1", 10,
NULL, 0,
  };

CCond::CCond( char * _Name )
  {
  Name = _Name;
  if ( pFirst == NULL )
    pFirst = this;                 
  if ( pLast )
    pLast->pNext = this;
  pLast = this;
  pNext = NULL;
  }

const int IOPM = 0x00000001 << 3;
CCond::operator int()
  {
  switch ( ID )
    {
    case 1:
      if ( *(BYTE*)pVar == 2 )
        return 1;
    return 0;
    case 2:
      if ( *(BYTE*)pVar == 1 )
        return 1;
    return 0;
    case 3:
      if ( *(BYTE*)pVar == 3 )
        return 1;
    return 0;
    case 4:
      if ( *(BYTE*)pVar == 1 )
        return 1;
    return 0;
    case 5:
      if ( *(BYTE*)pVar )
        return 1;
    return 0;
    case 6:
    return 0;
    case 7:
      if ( *(BYTE*)pVar == 0 )
        return 1;
    return 0;
    case 10:
      if ( *(BYTE*)pVar == 1 )
        return 1;
      return 0;
    }
  return 0;
  }

void CCond::operator = ( BYTE V )
  {
  switch ( ID )
    {
    case 1:
      *(BYTE*)pVar = 2;
    return;
    case 2:
      *(BYTE*)pVar = 1;
    return;
    case 4:
      if ( V )
      *(BYTE*)pVar = 1;
    return;
    case 5:
      ASS(0)
    return;
    case 6:
      ASS(0)
    return;
    case 7:
      *(BYTE*)pVar = 0;
    return;
    case 10:
      *(BYTE*)pVar = 1;
      return;
    }
  }

void CCond::Init()
  {
  char Cond[128];
  strcpy_s( Cond, Name );
  char * P = strchr ( Cond, '.' );
  ASS(P)
  *P = 0;
  char * P2 = strchr ( P + 1, '.' );
  ASS(P2)
  *P2 = 0;
  CY_Base * pObj = (*pFindYObj)( Cond, 0 );
  if ( pObj == NULL )
    {
    pVar = NULL;
    TypeVar = ' ';
    return;
    }
  pVar = NULL;
  void * pTuneVar = NULL;
  CY_Base::eVarType Work = CY_Base::evtP;
  TypeVar = ' ';
  bool Res = pObj->GetVar ( P + 1, &pVar, &pTuneVar, &TypeVar, &Work );
  ASS(Res)
  ASS(TypeVar == 'I' || TypeVar == 'D' || TypeVar == 'W' )
  *P2 = '.';
  ID = 0;
  int n = 0;
  while ( TypeCond[n].Name )
    {
    if ( lstrcmp ( TypeCond[n].Name, P + 1 ) == 0 )
      {
      ID = TypeCond[n].ID;
      break;
      }
    n++;
    }
  ASS( ID > 0 );
  switch ( ID )
    {
    case 1:
    case 2:
      ASS ( TypeVar == 'W' );
    break;
    case 3:
      ASS ( TypeVar == 'W' );
    break;
    case 8:
    case 9:
      ASS ( TypeVar == 'I' );
    break;
    }
  }

static bool InitCond = true;
void CCond::InitAll()
  {
  if ( !InitCond )
    return;
  InitCond = false;
  CCond * pCond = pFirst;
  while ( pCond )
    {
    pCond->Init();
    pCond = pCond->pNext;
    }
  }
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

