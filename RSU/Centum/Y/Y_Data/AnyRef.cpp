#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include <_CALCU.h>
#include "FlagsSupport.h"

template<typename T>
void CRefAny::SAnyType<T>::NotifyOnAssign( CY_Base * pObj )
{
  if( !pVar )
    return;
  pObj->OnAssigned( pVar );
}
////////////////////////////////////////////////////////////////////
CRefAny *CRefAny::pFirst = NULL;
CRefAny *CRefAny::pLast = NULL;

CRefAny::CRefAny( char * _Name )
: m_Type( ' ' )
, m_bNotifyOnAssign( false )
, m_pObj( NULL )
{
  Name = _Name;
  m_Field[0] = 0;
  m_NameObj[0] = 0;
  if ( pFirst == NULL )
    pFirst = this;
  if ( pLast )
    pLast->pNext = this;
  pLast = this;
  pNext = NULL;
  ZeroMemory( &m_Value, sizeof(m_Value) );
}

/*CRefAny::CRefAny( const CRefAny &thr ) 
: Name( NULL )
, pNext( NULL )
, m_Type( thr.m_Type )
, m_bNotifyOnAssign( false )
, m_pObj( NULL )
{
  m_Field[0] = 0;
  CopyMemory( &m_Value, &thr.m_Value, sizeof(m_Value) );
  Name = thr.Name;
  strcpy_s( m_NameObj, thr.m_NameObj );
  strcpy_s( m_Field, thr.m_Field );
}*/

CRefAny::CRefAny( char type )
: Name( NULL )
, pNext( NULL )
, m_Type( type )
, m_bNotifyOnAssign( false )
, m_pObj( NULL )
{
  m_Field[0] = 0;
  m_NameObj[0] = 0;
  ZeroMemory( &m_Value, sizeof(m_Value) );
}

CRefAny CRefAny::G( int x, int y /*= -1*/ )
{
  CRefAny r(*this);
  char szCompField[40] = { 0 };
  if( -1!=y)
    sprintf_s( szCompField, "%s[%d,%d]", m_Field, x, y );
  else
    sprintf_s( szCompField, "%s[%d]", m_Field, x );
  r.ReLinkToField( szCompField );
  return r;
}

CRefAny CRefAny::operator[]( int x )
{
  return this->G( x );
}

CRefAny& CRefAny::AssignToObj( CRefAny obj )
{
  return AssignToObj( obj.GetText() );
}

CRefAny& CRefAny::AssignToObj( LPCSTR pszObj )
{
  LPCSTR p = strchr(pszObj,'.');
  if( !p )
  {
    strcpy_s( m_NameObj, pszObj );
    pszObj = m_NameObj;
    if( !m_Field[0] )
      return *this;
  }
  else
  {
    strcpy_s( m_Field, p+1);
    strncpy_s( m_NameObj, pszObj, p-pszObj );
  }
  ASS( m_Field[0] );
  char Ref[40] = { 0 };
  sprintf_s( Ref, "%s.%s", m_NameObj, m_Field );
  Name = NULL;
  return ReLinkToField( Ref );
}

CRefAny& CRefAny::ReLinkToField( LPCSTR pszField /*= NULL*/ )
{
  char Ref[128] = { 0 };
  if( pszField )
  {
    LPCSTR p = strchr(pszField, '.');
    if( p )
    {
      if( strncmp(pszField,"%.", 2) )
        strcpy_s( Ref, pszField );
      else
      {
        if( !m_NameObj[0] )
        {
          strcpy_s( m_NameObj, Name );
          LPSTR p2 = strchr(m_NameObj, '.');
          if( p2 )
            *p2 = 0;
        }
        ASS( m_NameObj[0] );
        sprintf_s( Ref, "%s.%s", m_NameObj, p+1 );
      }
    }
    else
    {
      if( !m_NameObj[0] )
      {
        strcpy_s( m_NameObj, Name );
        LPSTR p2 = strchr(m_NameObj, '.');
        if( p2 )
          *p2 = 0;
      }
      ASS( m_NameObj[0] );
      sprintf_s( Ref, "%s.%s", m_NameObj, pszField );
    }
  }
  else
  {
    if( Name )
      strcpy_s( Ref, Name );
    else
    {
      ASS(0);
    }
  }
  /*if( Name )
    strcpy_s( Ref, Name );
  if( !Ref[0] && pszField )
    strcpy_s( Ref, pszField );*/
  char * P = strchr ( Ref, '.' );
  if( P )
  {
    *P = 0;
    strcpy_s( m_Field, P+1 );
  }
  else
  {
    //ASS( pszField );
    if( !pszField )
      return *this;
  }

  if( m_pObj )
  {
    if( strcmp( m_pObj->Name, Ref ) )
    {
      m_NameObj[0] = 0;
      m_pObj = NULL;
    }
  }
  else
  {
    m_NameObj[0] = 0;
    m_pObj = NULL;
  }
  
  if( !strcmp(Ref,"%") )
  {
    ASSD( 0 );
  }
  else if( !m_pObj )
    m_pObj = (*pFindYObj)( Ref, 0 );

  void *pVar = NULL;
  if( m_pObj == NULL )
  {
    //Fsend ( "RefName.txt", "не найти объект\"%s\"\n", Ref );
    m_Type = ' ';
    pVar = NULL;
    return *this;
  }
  strcpy_s( m_NameObj, Ref );
  void * pTuneVar = NULL;
  CY_Base::eVarType Work = CY_Base::evtP;
  char *pField = (char*)pszField;
  if( P && !pField )
    pField = P+1;
  else
  {
    ASS( pszField );
    LPCSTR point = strchr( pszField, '.' );
    if( point )
      pField = (char*)point + 1;
  }

  m_Field[0] = 0;
  USHORT VarSize = 0;
  bool Res = m_pObj->GetVar( pField, (void**)&pVar, &pTuneVar, &m_Type, &Work, NULL, &VarSize );
  if( !Res && pField && '&'==*pField )
  {
    pField = "CALI";
    Res = m_pObj->GetVar( pField, (void**)&pVar, &pTuneVar, &m_Type, &Work, NULL, &VarSize );
  }
  //ASSD( Res );
  if( !Res )
  {
    //Fsend ( "RefName.txt", "У объекта \"%s\" не найти поле \"%s\"\n", Ref, pField );
    return *this;
  }
  strncpy_s( m_Field, pField, _countof(m_Field)-1 );
  switch( m_Type )
  {
  case 'I':
    m_Value.discret.size = VarSize;
    m_Value.discret.Init( (int*)pVar );
    break;
  case 'D':
    m_Value.analog.size = VarSize;
    m_Value.analog.Init( (double*)pVar );
    break;
  case 'W':
    m_Value.W.size = VarSize;
    m_Value.W.Init( (BYTE*)pVar );
    m_bNotifyOnAssign = true;
    break;
  case 'S':
    m_Value.S.InitC( (LPSTR)pVar, VarSize );
    break;
  default:
    ASS( !"Не веданный до селе тип" );
    m_Type = ' ';
    return *this;
  }
  return *this;
}

void CRefAny::Init()
{
  ReLinkToField();
}

static bool InitRefAny = true;
void CRefAny::InitAll()
{
  if ( !InitRefAny )
    return;
  InitRefAny = false;
  CRefAny * pRef = pFirst;
  while ( pRef )
  { 
    pRef->Init();
    pRef = pRef->pNext;
  }
}

void CRefAny::operator = ( CRefAny Ref )
{
  switch( m_Type )
  {
  case 'I':
    m_Value.discret = (int)Ref;
    break;
  case 'D':
    m_Value.analog= (double)Ref;
    break;
  case 'W':
    m_Value.W = (BYTE)Ref.m_Value.W;
    break;
  case 'S':
    ASSD( 'S'==Ref.m_Type );
    m_Value.S  = Ref.GetText();
    break;
  }
}

bool CRefAny::operator !()
{
  switch( m_Type )
  {
  case 'I':
    return !m_Value.discret;
  case 'D':
    ASSD(0);
    return m_Value.analog != 0 ? 0 : 1;
    break;
  case 'W':
    return !m_Value.W;
  }
  return false;
}

void CRefAny::GetParam( KYBridge2SysParam &params )
{
  char ParmName[128] = { 0 };
  strcpy_s( ParmName, "#" );
  strcat_s( ParmName, Name );
  switch( m_Type )
  {
  case 'I':
    if( m_Value.discret.pVar )
    {
      PARM ( *m_Value.discret.pVar, ParmName );
    }
    else
    {
      PARM ( m_Value.discret.value, ParmName );
    }
    break;
  case 'D':
    if( m_Value.analog.pVar )
    {
      PARM ( *m_Value.analog.pVar, ParmName );
    }
    else
    {
      PARM ( m_Value.analog.value, ParmName );
    }
    break;
  case 'W':
    if( m_Value.W.pVar )
    {
      PARM ( *m_Value.W.pVar, ParmName );
    }
    else
    {
      PARM ( m_Value.W.value, ParmName );
    }
    break;
  case 'S':
    if( m_Value.S.pVar )
    {
      PARM ( m_Value.S.pVar, ParmName );
    }
    break;
  }
}

LPSTR CRefAny::GetText()
{
  static char nullString[20] = "";
  if( 'S'!=m_Type )
  {
    if( 'I'==m_Type && !_strcmpi( m_Field, "MODE") )
    {
      LPCSTR p = GetModeName( m_Value.discret );
      if( !p )
        return nullString;
      return (LPSTR)p;
    }
    return nullString;
  }
  static char s_szFullLen[256] = {};
  size_t l = strlen( (LPSTR)m_Value.S.pVar );
  if( l>=m_Value.S.size )
  {
    strncpy_s( s_szFullLen, (LPSTR)m_Value.S.pVar, m_Value.S.size );
    return s_szFullLen;
  }
  return (LPSTR)m_Value.S.pVar;
}

void CRefAny::TestAssignMode( LPCSTR pszMode )
{
  if( ('I'==m_Type && !_strcmpi( m_Field, "MODE")) )
  {
    int m = GetModeNumber( pszMode, NULL );
    if( -1==m )
      return;
    m_Value.discret = m;
    return;
  }
  int t = GetEnumValue( m_Field, pszMode, m_pObj );
  ASSD( t>=0 );
  if( t < 0 )
    return;
  *this = t;
}

bool CRefAny::CompareNoString( LPCSTR thr )
{
  if( 'I'==m_Type && !_strcmpi( m_Field, "MODE") )
  {
    int m = GetModeNumber( thr, NULL );
    if( -1==m )
      return false;
    return this->operator == ( m );
  }
  int t = GetEnumValue( m_Field, thr, m_pObj );
  ASSD( t>=0 );
  return this->operator == ( t );
}

void CRefAny::ExecLC()
{
  CY_Base *pObj = (*pFindYObj)( Name, 0 );
  if( !pObj )
    return;
  pObj->Work( -1 );
}
//////////////////////////////////////////////////////////////////////////
double operator * ( double thl, CRefAny &thr )
{
  thl *= (double)thr;
  return thl;
}

double operator * ( CRefAny &thl, double thr )
{
  double l = thl;
  return l * thr;
}

double operator * ( CRefAny &thl, CRefAny &thr )
{
  double l = thl;
  return l * (double)thr;
}

bool operator >= ( CRefAny &thl, double thr )
{
  return (double)thl >= thr;
}

bool operator >= ( CRefAny &thl, CRefAny &thr )
{
  return (double)thl >= (double)thr;
}

bool operator >= ( double thl, CRefAny &thr )
{
  return thl >= (double)thr;
}

bool operator > ( double thl, CRefAny &thr )
{
  return thl > (double)thr;
}

bool operator < ( double thl, CRefAny &thr )
{
  return thl < (double)thr;
}

bool operator <= ( CRefAny &thl, double thr )
{
  return (double)thl <= thr;
}

bool operator <= ( CRefAny &thl, CRefAny &thr )
{
  return (double)thl <= (double)thr;
}

bool operator <= ( double thl, CRefAny &thr )
{
  return thl <= (double)thr;
}

CRefAny operator + ( CRefAny &thl, CRefAny &thr )
{
  CRefAny r(thl.m_Type);
  r = (double)thl + (double)thr;
  return r;
}

double operator + ( double thl, CRefAny &thr )
{
  double r = thr;
  thl += r;
  return thl;
}

CRefAny operator + ( CRefAny &thl, double thr )
{
  CRefAny r(thl.m_Type);
  r = (double)thl + thr;
  return r;
}

double operator - ( double thl, CRefAny &thr )
{
  thl -= (double)thr;
  return thl;
}

double operator - ( CRefAny &thl, double thr )
{
  return (double)thl - thr;
}

CRefAny operator - ( CRefAny &thl, CRefAny &thr )
{
  CRefAny r(thl.m_Type);
  r = (double)thl - (double)thr;
  return r;
}

double operator / ( CRefAny &thl, double thr )
{
  double r = thl;
  if( 0!=thr )
    r /= thr;
  return r;
}

double operator / ( double thl, CRefAny &thr )
{
  if( thl!=(double)thr )
    thl /= (double)thr;
  return thl;
}

CRefAny operator / ( CRefAny &thl, CRefAny thr )
{
  CRefAny r(thl.m_Type);
  r = operator / ( thl, double(thr) );
  return r;
}

CRefAny operator & ( CRefAny &thl, int thr )
{
  CRefAny r(thl.m_Type);
  r = (int)thl & (int)thr;
  return r;
}

CRefAny operator | ( CRefAny &thl, int thr )
{
  CRefAny r(thl.m_Type);
  r = (int)thl | (int)thr;
  return r;
}

bool operator != ( int thl, CRefAny &thr )
{
  return thl != (int)thr;
}

bool operator != ( double thl, CRefAny &thr )
{
  switch( thr.m_Type )
  {
  case 'I':
    return (int)thl != (int)thr;
  case 'W':
    return (BYTE)thl != (BYTE)thr;
  }
  return thl != (double)thr;
}

bool operator == ( double thl, CRefAny &thr )
{
  switch( thr.m_Type )
  {
  case 'I':
    return (int)thl == (int)thr;
  case 'W':
    return (BYTE)thl == (BYTE)thr;
  }
  return thl == (double)thr;
}

bool operator && ( CRefAny &thl, bool thr )
{
  CRefAny r(thl.m_Type);
  r = (int)thl && (int)thr;
  return r;
}

bool operator || ( CRefAny &thl, bool thr )
{
  CRefAny r(thl.m_Type);
  r = (int)thl || (int)thr;
  return r;
}
