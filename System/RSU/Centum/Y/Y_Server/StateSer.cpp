#include "stdafx.h"
#include "StateSer.h"
#include <rsuErr.h>

KStateSer::KStateSer( LPCSTR pszFile, bool bSave )
: m_hFile( INVALID_HANDLE_VALUE )
, m_dwFileSize( 0 )
, m_dwInitialFileSize( 0 )
, m_dwCurr( 0 )
, m_dwLastError( 0 )
, m_BytesCanRead( 0 )
{
  bool res = bSave ? Create(pszFile) : Open(pszFile);
  ASS( res );
  m_bRead = !bSave;
}

KStateSer::KStateSer( HANDLE hFile, bool bSave )
: m_hFile( hFile )
, m_dwFileSize( 0 )
, m_dwInitialFileSize( 0 )
, m_dwCurr( 0 )
, m_dwLastError( 0 )
, m_BytesCanRead( 0 )
{
  m_bRead = !bSave;
  DWORD H = 0;
  m_dwInitialFileSize = m_dwFileSize = GetFileSize( m_hFile, &H );
  ASS( !H );
}

KStateSer::~KStateSer()
{
  Close();
}

bool KStateSer::Create( LPCSTR pszFile )
{
  m_hFile = CreateFile( pszFile,
                        GENERIC_WRITE,
                        FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_FLAG_SEQUENTIAL_SCAN,
                        NULL );
  return INVALID_HANDLE_VALUE != m_hFile;
}

bool KStateSer::Open( LPCSTR pszFile )
{
  DWORD dwFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN;
  m_hFile = CreateFile( pszFile,
                        GENERIC_READ,
                        FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        dwFlagsAndAttributes,
                        NULL );
  if( INVALID_HANDLE_VALUE == m_hFile )
    return false;

  DWORD H = 0;
  m_dwInitialFileSize = m_dwFileSize = GetFileSize( m_hFile, &H );
  ASS( !H );

  return true;
}

bool KStateSer::Close()
{
  if( INVALID_HANDLE_VALUE==m_hFile )
    return false;
  BOOL r = CloseHandle( m_hFile );
  ASS( r );
  m_hFile = INVALID_HANDLE_VALUE;
  return true;
}

void KStateSer::OnWrote( DWORD size )
{
  m_dwCurr += size;
  m_dwFileSize += size;
}

void KStateSer::OnRead( DWORD size )
{
  ASSD( m_dwFileSize >= size );
  m_dwFileSize -= size;
  m_dwCurr += size;
}

IYokoStateSer* KStateSer::GetInterface()
{
  return this;
}
//////////////////////////////////////////////////////////////////////////
DWORD KStateSer::Shift( LONG lDistanceToMove )
{
  DWORD r = SetFilePointer( m_hFile, lDistanceToMove, NULL, FILE_CURRENT );
  m_dwCurr = r;
  m_dwFileSize = m_dwInitialFileSize - r;
  return r;
}

bool KStateSer::IsData()
{
  ASSD( m_bRead );
  if( m_bRead )
    return m_dwFileSize > 0;
  return true;
}

DWORD KStateSer::BytesCanRead()
{
  return m_BytesCanRead;
}

void KStateSer::SetReadyBytes( DWORD count )
{
  m_BytesCanRead = count;
}

DWORD KStateSer::SimpleRead( LPVOID Addr, DWORD dwSize )
{
  ASSD( m_dwFileSize >= dwSize );
  if( !m_dwFileSize )
    return 0;
  DWORD Lr = 0;
  if( !::ReadFile( m_hFile, Addr, dwSize, &Lr, NULL ) )
  {
    m_dwLastError = GetLastError();
    return 0;
  }
  ASSD( Lr==dwSize );
  OnRead( dwSize );
  return dwSize;
}

DWORD KStateSer::ReadLenStr( LPSTR str, DWORD dwSize )
{
  DWORD len = 0;
  if( !SimpleRead( &len, sizeof(len)) )
    return 0;
  ASS( len<dwSize );
  if( len>=(dwSize-1) )
    return 0;
  if( !SimpleRead( str, len ) )
    return 0;
  str[len] = 0;
  return len+sizeof(len);
}

DWORD KStateSer::SimpleWrite( LPCVOID Addr, DWORD dwSize )
{
  DWORD Lw = 0;
  if( !::WriteFile ( m_hFile, Addr, dwSize, &Lw, NULL ) )
  {
    m_dwLastError = GetLastError();
    return 0;
  }
  ASSD( Lw==dwSize );
  OnWrote( dwSize );
  return dwSize;
}

DWORD KStateSer::WriteStr( LPCSTR str )
{
  DWORD L = (DWORD)strlen( str );
  if( !SimpleWrite( &L, sizeof(L) ) )
    return 0;
  if( L && !SimpleWrite( str, L ) )
    return 0;
  return L + sizeof(L);
}

DWORD KStateSer::GetPosition()
{
  return m_dwCurr;
}

DWORD KStateSer::WriteAtPosition( LONG shiftFromBegin, LPCVOID Addr, DWORD objSize )
{
  DWORD &curPos = m_dwCurr;
  DWORD r = SetFilePointer( m_hFile, shiftFromBegin, NULL, FILE_BEGIN );
  curPos = shiftFromBegin;
  DWORD l = SimpleWrite( Addr, objSize );
  r = SetFilePointer( m_hFile, 0, 0, FILE_END );
  m_dwFileSize = m_dwCurr = r;
  return l;
}
