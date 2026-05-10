#include "StateSer.h"
#include <rsuErr.h>
#include <crossstring.h>

enum EStateKeys
{
  eKeyMagicDW = 0x5A8E845E,
};

KStateSer::KStateSer( LPCSTR pszFile, bool bSave )
: m_hFile( NULL )
, m_dwFileSize( 0 )
, m_dwCurr( 0 )
, m_bStartCalled( false )
, m_bCompleteCalled( false )
, m_dwStartPos( 0 )
, m_saverVerFormat( 0 )
{
  bool res = bSave ? Create(pszFile) : Open(pszFile);
  ASS( res );
  if (pszFile[strlen(pszFile)] != 'x')
      m_bConvert = true;
  m_bRead = !bSave;
}

KStateSer::KStateSer( FILE* hFile, bool bSave, bool convert )
: m_hFile( hFile )
, m_dwFileSize( 0 )
, m_dwCurr( 0 )
, m_dwStartPos( 0 )
, m_bConvert( convert )
{
  m_bRead = !bSave;
  auto pos = ftell(m_hFile);
  fseek(m_hFile, 0, SEEK_END);
  m_dwFileSize = ftell(m_hFile);
  fseek(m_hFile, pos, SEEK_SET);
  ASS( m_dwFileSize);
}

KStateSer::~KStateSer()
{
  if( !m_bRead )
  {
    ASS( m_bStartCalled );
    ASS( m_bCompleteCalled );
  }
  Close();
}

bool KStateSer::Close()
{
  if( NULL==m_hFile )
    return false;
  bool r = fclose(m_hFile) == 0;
  ASS( r );
  m_hFile = NULL;
  return true;
}

void KStateSer::OnWrote( DWORD size )
{
  m_dwCurr += size;
  m_dwFileSize += size;
}

void KStateSer::OnRead( DWORD size )
{
  m_dwCurr += size;
}

bool KStateSer::Create( LPCSTR pszFile )
{
    m_hFile = fopen(pszFile, "w+b");
  return NULL != m_hFile;
}

bool KStateSer::Open( LPCSTR pszFile )
{
    m_hFile = fopen(pszFile, "r+b");
  if( NULL == m_hFile )
    return false;

  fseek(m_hFile, 0, SEEK_END);
  m_dwFileSize = (DWORD)ftell(m_hFile);
  fseek(m_hFile, 0, SEEK_SET);
  ASS(m_dwFileSize);

  return true;
}

void KStateSer::StartSave()
{
  m_bStartCalled = true;
  Write( eKeyMagicDW );
  m_dwStartPos = GetPosition();
  Write( m_dwStartPos );
}

void KStateSer::SaveCompleted()
{
  m_bCompleteCalled = true;
  DWORD &curPos = m_dwCurr;
  DWORD r = fseek(m_hFile, m_dwStartPos, SEEK_SET);
  //DWORD r = SetFilePointer( m_hFile, m_dwStartPos, NULL, FILE_BEGIN );
  DWORD length = curPos - m_dwStartPos;
  m_dwCurr -= 4;
  Write( length );
  r = fseek( m_hFile, 0, SEEK_END );
  ASS( m_dwCurr==r );
  m_dwCurr = r;
}

int KStateSer::StartRestore()
{
  bool r = CompareOnRead( eKeyMagicDW );
  ASS( r );
  int legth = 0;
  Read( legth );
  return legth;
}

bool KStateSer::NeedConvertUTF()
{
    return m_bConvert;
}

DWORD KStateSer::WriteImpl( const void *pObj, DWORD objSize )
{
  DWORD start = GetPosition();
  DWORD Lw = 0;
  Lw = fwrite(pObj, objSize, 1, m_hFile);
  ASSD( objSize==Lw );
  OnWrote( Lw );
  DWORD end = GetPosition();
  return end - start;
}

DWORD KStateSer::ReadImpl( void *pObj, DWORD objSize )
{
  DWORD Lr = 0;
  Lr = fread(pObj, 1, objSize, m_hFile);
  if (Lr <= 0 || ferror(m_hFile))
  {
    return 0;
  }
  ASSD( Lr==objSize );
  OnRead( objSize );
  return Lr;
}

DWORD KStateSer::RawRead( void *pOut, DWORD objSize )
{
  return ReadImpl( pOut, objSize );
}

DWORD KStateSer::GetPosition()
{
  return m_dwCurr;
}

DWORD KStateSer::WriteAtPositionImpl( DWORD shiftFromBegin, const void *pObj, DWORD objSize )
{
  DWORD &curPos = m_dwCurr;

  DWORD r = fseek(m_hFile, shiftFromBegin, SEEK_SET);
  curPos = shiftFromBegin;
  DWORD l = WriteImpl( pObj, objSize );
  r = fseek( m_hFile, 0, SEEK_END );
  m_dwCurr = r;
  return l;
}

void KStateSer::Skip( DWORD shift )
{
  m_dwCurr = fseek( m_hFile, shift, SEEK_CUR);
}

void KStateSer::WriteDotV( SDotValue *pV, LPCSTR pszName )
{
  Write( pszName );
  DWORD s = pV ? sizeof(*pV) : 0;
  Write( s );
  if( !pV )
    return ;
  Write( *pV );
}

int KStateSer::ReadDotV( SDotValue *pV, LPCSTR pszName )
{
  char szName[32*4];
  Read( szName );
  if (NeedConvertUTF())
      cross::string::convert_to_utf8(1251, szName, szName, sizeof(szName));
  if( strcmp(szName, pszName) )
  {
    ASSD( 0 );
    return -1;
  }
  DWORD s = 0;
  Read( s );
  if( !pV )
  {
    if( !s )
      return 0;
    Skip( s );
    return 0;
  }
  if( s!=sizeof(*pV) )
    Skip( s );
  else
    Read( *pV );
  return 0;
}

void KStateSer::SetSavedVersionFormat( DWORD v )
{
  m_saverVerFormat = v;
}

DWORD KStateSer::GetSavedVersionFormat()
{
  return m_saverVerFormat;
}
