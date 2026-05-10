#include <rsuNoNames2D.h>
#include <stdio.h>
#include <rsuErr.h>
#include "OnDiskAllocator.h"
#include <rsuClipboard.h>
#include <cstring>
#include <hashRSU.h>

static KNoName2D s_Opened[10];
LPCSTR KNoName2D::s_ClassNames[KNoName2D::eClassNamesCount] = { 0 };
tRsuHandleInitData KNoName2D::s_InitDataCalls[KNoName2D::eRsuSupportCount] = { 0 };

static LPCSTR s_pText = "Рекомендуется очистить папку \"Memory\"";
static LPCSTR s_pCaption = "Неустранимая ошибка";

template<typename T, size_t _S>
int ExtendArray( T (&arr)[_S], T pNewCall )
{
  if( pNewCall )
  {
    for( int i=0; i<_S; i++ )
    {
      if( arr[i]==pNewCall )
        return i;
      if( arr[i] )
        continue;
      arr[i] = pNewCall;
      return i;
    }
  }
  return -1;
}

template<typename T, size_t _S, typename A>
int RsuHandleCall( T (&arr)[_S], A a )
{
  for( int i=0; i<_countof(arr); i++ )
  {
    if( !arr[i] )
      continue;
    if( 0==arr[i](a) )
      return 0;
  }
  return 1;
}
//////////////////////////////////////////////////////////////////////////
KNoName2D::KNoName2D()
: pHeader( NULL )
, pOrd( NULL )
, pszString4Ords( NULL )
, pszObjects( NULL )
, m_hVarMapping( NULL )
{
  pfStringComparator = strcmp;
}

KNoName2D::KNoName2D( KNoName2D &src )
{
  this->operator = (src );
}

bool KNoName2D::IsOpened()
{
  return pHeader ? true : false;
}

KNoName2D& KNoName2D::operator = ( const KNoName2D &src )
{
    memcpy(this, &src, sizeof(*this));
  if( pHeader )
    pHeader->clientCounter += 1;
  return *this;
}

KNoName2D::~KNoName2D()
{
  if( pHeader )
    pHeader->clientCounter -= 1;
}

template<typename TL, typename TR>
void Test4NewMem( TL thl, TR thr, bool &res )
{
  if( 0==thl )
    return;
  if( thl!=thr )
    res = true;
}
static LPCSTR s_szMemExt = "noname2D";
KNoName2D *KNoName2D::Create( LPCSTR pszName, bool bMaster, DWORD nNames /*= 800000*/, DWORD nBtsStrings /*= 0xd00000*/, DWORD nBtsObjs /*= 0x0dd00000*/ )
{
  int nEmtpy = -1;
  for( int i=0; i<_countof(s_Opened); ++i )
  {
    if( !s_Opened[i].pHeader )
    {
      if( -1==nEmtpy )
        nEmtpy = i;
      continue;
    }
    if( !strcmp(pszName, s_Opened[i].pHeader->szName) )
      return &s_Opened[i];
  }
  KNoName2D* nn = new KNoName2D();
  if (-1 == nEmtpy)
  {
      return nn;
  }

  DWORD Size = sizeof(*nn->pHeader) + sizeof(*nn->pOrd) * nNames + nBtsStrings + nBtsObjs;
  bool bNewMem = false;
  SNoNameHeader* pHeader = (SNoNameHeader*)OnDiskAllocator( nn->m_hVarMapping, bNewMem, Size, pszName, s_szMemExt, bMaster );
  if (nullptr == pHeader)
  {
      return nn;
  }
  if( bNewMem && !bMaster )
  {
    ASSD( !"как-то криво");
    if (nn->m_hVarMapping != nullptr)
    {
        nn->m_hVarMapping->unmap();
    }

    return nn;
  }
  nn->pHeader = pHeader;

  SNoNameHeader& header = *nn->pHeader;

  if( bMaster )
  {
    Test4NewMem( header.shiftOrds, sizeof(header), bNewMem );
    header.shiftOrds = sizeof(header);
    Test4NewMem( header.countOrds, nNames, bNewMem );
    header.countOrds = nNames;

    Test4NewMem( header.shiftBuff4Ords, header.shiftOrds + header.countOrds*sizeof(*nn->pOrd), bNewMem );
    header.shiftBuff4Ords = header.shiftOrds + header.countOrds*sizeof(*nn->pOrd);
    Test4NewMem( header.nBuff4Ords, nBtsStrings, bNewMem );
    header.nBuff4Ords = nBtsStrings;

    Test4NewMem( header.shiftObjects, header.shiftBuff4Ords + nBtsStrings, bNewMem );
    header.shiftObjects = header.shiftBuff4Ords + nBtsStrings;
    Test4NewMem( header.nBuffObjects, nBtsObjs, bNewMem );
    header.nBuffObjects = nBtsObjs;

    strcpy_s( header.szName, pszName );
    header.clientCounter = 0;
  }

  BYTE *pMem = (BYTE*)nn->pHeader;
  nn->pOrd = (SBuffOrd*)(pMem + header.shiftOrds);
  nn->pszString4Ords = pMem + header.shiftBuff4Ords;
  nn->pszObjects = pMem + header.shiftObjects;

  if( bNewMem )
  {
      memset(nn->pOrd, 0, header.countOrds * sizeof(*nn->pOrd));
      memset(nn->pszString4Ords, 0, header.nBuff4Ords);
      memset(nn->pszObjects, 0, nBtsObjs);
      header.nCurr = sizeof(SBuffRecSeparator);//5
  }
  s_Opened[nEmtpy] = *nn;
  return nn;
}

void KNoName2D::Reset()
{
  SNoNameHeader &header = *pHeader;
  KNoName2D &nn = *this;
  memset(nn.pOrd, 0, header.countOrds * sizeof(*nn.pOrd));
  memset(nn.pszString4Ords, 0, header.nBuff4Ords);
  memset(nn.pszObjects, 0, header.nBuffObjects);
  header.nCurr = sizeof(SBuffRecSeparator);//5
  header.nMaxPos = 0;
  header.nCount = 0;
#ifdef LINUX
	CLogFile::Log ("KNoName2D::Reset_4");
#endif // LINUX

}

void KNoName2D::PrintStatistic()
{
  if( !pHeader )
    return;
}

int KNoName2D::RegisterClassInNames( DWORD idClass, LPCSTR pszClassName )
{
  ASS( idClass<_countof(s_ClassNames) );
  if( s_ClassNames[idClass] )
  {
    ASS( !_strcmpi(s_ClassNames[idClass], pszClassName) );
    return idClass;
  }
  s_ClassNames[idClass] = pszClassName;
  return idClass;
}

void KNoName2D::RegisterInitCBaseCall( tRsuHandleInitData pInitData )
{
  ExtendArray( s_InitDataCalls, pInitData );
}
//////////////////////////////////////////////////////////////////////////
#include <CBase.h>
CBase* KNoName2D::CreateStruct( LPCTSTR ObjName, DWORD TypeID, int number, int domain )
{
  int ID_PNT_NAME = GetID( TypeID, ObjName, true, number, domain );
  if( ID_PNT_NAME<=0 )
    return NULL;
  CBase *pBase = GetNameData( ID_PNT_NAME );
  if( pBase )
    return pBase;
  return AppendData( ID_PNT_NAME, TypeID );
}

CBase* KNoName2D::CreateStruct( LPCTSTR ObjName, DWORD TypeID, DWORD sizeObj, int number, int domain )
{
  int ID_PNT_NAME = GetID( TypeID, ObjName, true, number, domain );
  if( ID_PNT_NAME<=0 )
    return nullptr;
  CBase *pBase = GetNameData( ID_PNT_NAME );
  if( pBase )
  {
    if( sizeObj!=pBase->size )
    {
#ifdef _WIN32
        rsuTxtToClp( ObjName );
      char szBuf[1024] = {};
      sprintf_s( szBuf, "%s\nпроблемный объект - \"%s\"", s_pText, ObjName );
      MessageBox( NULL, szBuf, s_pCaption, MB_OK|MB_ICONASTERISK );
#endif
    }
    ASS( sizeObj==pBase->size )
      return pBase;
  }
  return AppendData( ID_PNT_NAME, TypeID, sizeObj );
}

CBase* KNoName2D::FindStruct( LPCTSTR ObjName, DWORD TypeID, int number, int domain )
{
  int ID_PNT_NAME = GetID( TypeID, ObjName, false, number, domain );
  if( ID_PNT_NAME<=0 )
    return nullptr;
  CBase *pBase = GetNameData( ID_PNT_NAME );
  if( pBase )
    return pBase;
  return nullptr;
}

int KNoName2D::GetID( LPCSTR szClass, LPCSTR szEntry, bool bAdd, int number, int domain )
{
  if( !szClass )
    return 0;
  if( !szEntry )
    return 0;
  if( !pHeader )
    return 0;

  const DWORD classCrc = m_bCompareByCrc ? CreateHASH32(szClass, m_bCrc2Upper) : 0,
      entryCrc = m_bCompareByCrc ? CreateHASH32(szEntry, m_bCrc2Upper) : 0;
  int K = pHeader->nCount;
  int nb = -1, ne = K, no, n, Res;
  UINT nClass = 0;
  UINT nEntry = 0;
  while ( ne - nb > 1 )
  {
    n = ( nb + ne ) >> 1;
    no = pOrd[n].nOrd;
    LPCSTR pTestClass = Class( pOrd[no] );

    LPCSTR pTestEntry = NULL;
    Res = pfStringComparator( pTestClass, szClass );
    if( Res == 0 )
    {
      pTestEntry = Entry( pOrd[no] );
      Res = pfStringComparator( pTestEntry, szEntry );
      nClass = pOrd[no].nClass;
    }
    if( Res == 0 )
    {
      nEntry = pOrd[no].nEntry;
      if( number>0 )
      {
        int n = Number(pOrd[no]);
        Res = n==number ? 0 : (n<number ? -1 : 1);
      }
      else
        KKK();
    }              
    if( Res == 0 )
    {
      if( domain>0 )
      {
        int n = Domain(pOrd[no]);
        Res = n==domain ? 0 : (n<domain ? -1 : 1);
      }
      else
        KKK();
    }
    if( Res == 0 )
    {
      return no + 1;
    }
    if ( Res < 0 ) 
      ne = n;
    else          
      nb = n;
  }
  if ( !bAdd )
  {
    return 0;
  }
  // Новое имя сразу за nb
  ASS( K < pHeader->countOrds /*MAX_NAMES*/ );
  //
  if ( nClass == 0 )
  {
    pOrd[K].nClass = pHeader->nCurr;
    SetBuffer( szClass );
  }
  else 
    pOrd[K].nClass = nClass;
  //
  if( nEntry == 0 )
  {
    pOrd[K].nEntry = pHeader->nCurr;
    SetBuffer( szEntry );
  }
  else 
    pOrd[K].nEntry = nEntry;

  pOrd[K].nData  = -1;
  pOrd[K].nNumber = std::max(0, number);
  pOrd[K].nDomainNumber = std::max(0, domain);
  //
  if ( nb < K - 1 )
  {
    for ( int n = K - 1; n > nb; --n )
      pOrd[n+1].nOrd = pOrd[n].nOrd;
  }
  pOrd[nb+1].nOrd = K;
  ++K;
  pHeader->nCount = K;
  return K;
}

int KNoName2D::GetID( DWORD TypeID, LPCSTR szEntry, bool bAdd, int number, int domain )
{
  if( TypeID > sizeof(s_ClassNames) )
    return -1;
  return GetID( s_ClassNames[TypeID], szEntry, bAdd, number, domain );
}

LPCSTR KNoName2D::Class( SBuffOrd &_ord )
{
  if( !pszString4Ords )
    return nullptr;
  SBuffRecSeparator *buf = (SBuffRecSeparator*)(pszString4Ords+_ord.nClass);
  return buf->str; 
}

LPCSTR KNoName2D::Entry( SBuffOrd &_ord )
{
  if( !pszString4Ords )
    return NULL;
  SBuffRecSeparator *buf = (SBuffRecSeparator*)(pszString4Ords+_ord.nEntry);
  return buf->str; 
}

int KNoName2D::Number( SBuffOrd &_ord )
{
  return _ord.nNumber; 
}

int KNoName2D::Domain( SBuffOrd &_ord )
{
  return _ord.nDomainNumber; 
}

LPCSTR KNoName2D::Class( DWORD ID )
{
  ASSD( ID < pHeader->countOrds );
  ASSD( ID < pHeader->nCount );
  return Class( pOrd[ID] );
}

LPCSTR KNoName2D::Entry( DWORD ID )
{
  ASSD( ID < pHeader->countOrds );
  ASSD( ID < pHeader->nCount );
  return Entry( pOrd[ID] );
}

int KNoName2D::Number( DWORD ID )
{
  ASSD( ID < pHeader->countOrds );
  ASSD( ID < pHeader->nCount );
  return Number( pOrd[ID] );
}

int KNoName2D::Domain( DWORD ID )
{
  ASSD( ID < pHeader->countOrds );
  ASSD( ID < pHeader->nCount );
  return Domain( pOrd[ID] );
}

void KNoName2D::SetBuffer( LPCSTR text )
{
  if( !pHeader )
    return;
  UINT L = (UINT)strlen(text);
  UINT N = pHeader->nCurr + L + sizeof(SBuffRecSeparator);
  ASS( N < pHeader->nBuff4Ords/*MAX_BUFFER*/ );
  if( N > pHeader->nBuff4Ords )
  {
    exit(1);
  }
  SBuffRecSeparator *rec = (SBuffRecSeparator*)(pszString4Ords + pHeader->nCurr);
  rec->strLength = L;
  memcpy( rec->str, text, L+1 );
  pHeader->nCurr = N;
}

CBase* KNoName2D::GetNameData( int ID )
{
  if( !pHeader )
    return NULL;
  ID--;
  ASS ( ID < pHeader->nCount );
  ASSD( ID < pHeader->countOrds );
  if ( pOrd[ID].nData != -1 )
  {
    CBase* pBase = (CBase*)(pszObjects + pOrd[ID].nData);
#ifdef _WIN32
    if( !pBase->size )
      MessageBox( NULL, s_pText, s_pCaption, MB_OK|MB_ICONASTERISK );
#endif
    ASS( pBase->size > 0 );
    return pBase;
  }
  return NULL;
}

CBase* KNoName2D::AppendData( int ID, DWORD TYPE, DWORD size/* = 0*/ )
{
  if( !pHeader || !pszObjects )
    return NULL;
  CBase* pBase = (CBase*)( pszObjects + pHeader->nMaxPos );
  pBase->ID_PNT_NAME = ID;
  pBase->ID_CLASS = TYPE;

  if( 0==size )
  {
    if( InitData( pBase ) != 0 )
      return nullptr;
  }
  else
    pBase->size = size;

  ASS( pBase->size > sizeof(*pBase));
  ID--;
  ASSD( ID < pHeader->nCount );
  ASSD( ID < pHeader->countOrds );
  if( ID >= pHeader->countOrds )
  {
    exit(1);
  }
  pOrd[ID].nData = pHeader->nMaxPos;
  pHeader->nMaxPos += pBase->size; 
  ASS(pHeader->nMaxPos < pHeader->nBuffObjects /*MEM_SIZE*/);
  if( pHeader->nMaxPos >= pHeader->nBuffObjects )
  {
#ifdef _WIN32
      ::MessageBox( NULL,
                  "Нехватка памяти под объекты\nобратитесь к разработчику",
                  pHeader->szName,
                  MB_ICONSTOP );
#endif
      exit(1);
  }
  return pBase;
}

size_t KNoName2D::GetNAddr( DWORD ID )
{
  ID--;
  ASS( ID >= 0 );
  ASSD( ID < pHeader->nCount );
  ASSD( ID < pHeader->countOrds );
  return pOrd[ID].nData;
}

CBase* KNoName2D::GetBase( size_t AddrBase )
{
  ASSD( AddrBase < pHeader->nMaxPos );
  if( AddrBase > pHeader->nMaxPos )
    return NULL;
  CBase* pBase = (CBase*)( pszObjects + AddrBase );
  ASSD( pBase->ID_PNT_NAME-1 < pHeader->nCount );
  ASSD( pBase->ID_PNT_NAME-1 < pHeader->countOrds );
  ASSD( pOrd[pBase->ID_PNT_NAME-1].nData==AddrBase );
  return pBase;
}

int KNoName2D::InitData( CBase* pObj )
{
  return RsuHandleCall( s_InitDataCalls, pObj );
}

LPCSTR KNoName2D::GetEmpty()
{
  if( !pszString4Ords )
    return nullptr;
  return (LPCSTR)pszString4Ords;
}

int KNoName2D::CreateRefToData( LPCSTR ObjName, CBase *pBase )
{
  if( !pOrd )
    return -1;
  ASSD( pBase->ID_PNT_NAME-1 < pHeader->nCount );
  ASSD( pBase->ID_PNT_NAME-1 < pHeader->countOrds );
  int number = pOrd[pBase->ID_PNT_NAME-1].nNumber;
  int domain = pOrd[pBase->ID_PNT_NAME-1].nDomainNumber;
  int ID_PNT_NAME = GetID( pBase->ID_CLASS, ObjName, true, number, domain );
  if( ID_PNT_NAME<=0 )
    return -1;
  ASSD( ID_PNT_NAME-1 < pHeader->nCount );
  ASSD( ID_PNT_NAME-1 < pHeader->countOrds );
  pOrd[ID_PNT_NAME-1].nData = pOrd[pBase->ID_PNT_NAME-1].nData;
  return 0;
}

bool KNoName2D::WhileBase( DWORD& ID, const DWORD ClassID, CBase** base, LPCSTR* pszName, int *pFcsNumber, int *pDomain )
{
  if( !pHeader )
    return false;
  const DWORD &nCount = pHeader->nCount;
  for( ; ID < nCount; ++ID )
  {
    CBase * pBase = GetNameData( ID+1 );
    if( !pBase )
      continue;
    if ( ClassID == -1 || ClassID == pBase->ID_CLASS )
    {
      ID++;
      ASS( int(pBase->size) > 0 ); 
      if ( base ) 
        *base = pBase;
      GetNameEx( ID, nullptr, pszName, pFcsNumber, pDomain );
      return true;
    }
  }
  return false;
}

bool KNoName2D::GetNameEx( DWORD ID, const char** pszClass, const char** pszEntry, int *pFcsNumber /*= NULL*/, int *pDomain /*= NULL*/ )
{
  if( !pHeader )
    return false;
  ID--;
  if( ID < pHeader->nCount )
  {
    if( NULL != pszClass ) 
      *pszClass = Class(ID );
    if( NULL != pszEntry )
      *pszEntry = Entry( ID );
    if( NULL != pFcsNumber )
      *pFcsNumber = Number( ID );
    if( NULL != pDomain )
      *pDomain = Domain( ID );
    return true;
  }
  // В начале буфера всегда нули
  if( NULL != pszClass )
    *pszClass = GetEmpty();
  if( NULL != pszEntry )
    *pszEntry = GetEmpty();
  if( NULL != pFcsNumber )
    *pFcsNumber = 0;
  if( NULL != pDomain )
    *pDomain = 0;
  return false;
}

bool KNoName2D::GetNameEx( DWORD ID, DWORD ClassID, LPCSTR* pszClass, LPCSTR* pszEntry, int *pFcsNumber /*= NULL*/, int *pDomain /*= NULL*/ )
{
  if( !pHeader )
    return false;
  ID--;
  if( ID < pHeader->nCount )
  {
    CBase * pBase = GetNameData( ID+1 );
    if( pBase && ClassID==pBase->ID_CLASS )
    {
      if( NULL != pszClass ) 
        *pszClass = Class(ID );
      if( NULL != pszEntry )
        *pszEntry = Entry( ID );
      if( NULL != pFcsNumber )
        *pFcsNumber = Number( ID );
      if( NULL != pDomain )
        *pDomain = Domain( ID );
      return true;
    }
  }
  // В начале буфера всегда нули
  if( NULL != pszClass )
    *pszClass = GetEmpty();
  if( NULL != pszEntry )
    *pszEntry = GetEmpty();
  if( NULL != pFcsNumber )
    *pFcsNumber = 0;
  if( NULL != pDomain )
    *pDomain = 0;
  return false;
}

void KNoName2D::GetName( DWORD ID, char *szClass, size_t _SC, char *szEntry, size_t _SE )
{
  if( !pHeader )
    return;
  ID--;
  ASS( (unsigned int)ID < pHeader->nCount );
  if ( szClass != NULL )
    strcpy_s( szClass, _SC, Class( ID ) );
  if ( szEntry != NULL )
    strcpy_s( szEntry, _SE, Entry( ID ) );
}
