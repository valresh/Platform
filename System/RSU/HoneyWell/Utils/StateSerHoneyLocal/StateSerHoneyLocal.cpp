#include <StateSerHoneyLocal.h>
#include <rsuErr.h>

KStateSerHoneyLocal::KStateSerHoneyLocal( LPCSTR pszFile, bool bSave )
: m_hFile( NULL )
, m_dwFileSize( 0 )
, m_dwCurr( 0 )
, m_dwLastError( 0 )
, m_BytesCanRead( 0 )
{
  bool res = bSave ? Create(pszFile) : Open(pszFile);
  ASS( res );
  if (pszFile[strlen(pszFile)] != 'x')
      m_bConvert = true;
  m_bRead = !bSave;
}

KStateSerHoneyLocal::KStateSerHoneyLocal(FILE* hFile, bool bSave, bool convert )
: m_hFile( hFile )
, m_dwFileSize( 0 )
, m_dwCurr( 0 )
, m_dwLastError( 0 )
, m_BytesCanRead( 0 )
, m_bConvert (convert)
{
  m_bRead = !bSave;
  auto pos = ftell(m_hFile);
  fseek(m_hFile, 0, SEEK_END);
  m_dwFileSize = ftell(m_hFile);
  fseek(m_hFile, pos, SEEK_SET);

  ASS( m_dwFileSize);
}

KStateSerHoneyLocal::~KStateSerHoneyLocal()
{
  Close();
}

IStateSer* KStateSerHoneyLocal::GetInterface()
{
  return this;
}

bool KStateSerHoneyLocal::NeedConvertUTF()
{
    return m_bConvert;
}

bool KStateSerHoneyLocal::Create( LPCSTR pszFile )
{
    m_hFile = fopen(pszFile, "w+b");
  //m_hFile = CreateFile( pszFile,
  //                      GENERIC_WRITE,
  //                      FILE_SHARE_READ|FILE_SHARE_WRITE,
  //                      NULL,
  //                      CREATE_ALWAYS,
  //                      FILE_FLAG_SEQUENTIAL_SCAN,
  //                      NULL );
  return NULL != m_hFile;
}

bool KStateSerHoneyLocal::Open( LPCSTR pszFile )
{
  m_hFile = fopen(pszFile, "r+b");

  /*DWORD dwFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN;
  m_hFile = CreateFile( pszFile,
                        GENERIC_READ,
                        FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        dwFlagsAndAttributes,
                        NULL );
  if( INVALID_HANDLE_VALUE == m_hFile )
    return false;*/
  fseek(m_hFile, 0, SEEK_END);
  m_dwFileSize = (DWORD)ftell(m_hFile);
  fseek(m_hFile, 0, SEEK_SET);

  return true;
}

bool KStateSerHoneyLocal::Close()
{
  if( NULL==m_hFile )
    return false;
  bool r = fclose(m_hFile) == 0;
  ASS( r );
  m_hFile = NULL;
  return true;
}

void KStateSerHoneyLocal::WriteImpl( const void *pObj, DWORD objSize )
{
  //DWORD Lw = 0;
  fwrite(pObj, objSize, 1, m_hFile);
  //::WriteFile ( m_hFile, pObj, objSize, &Lw, NULL );
}

bool KStateSerHoneyLocal::ReadImpl( void *pObj, DWORD objSize )
{    
  DWORD Lr = fread(pObj, 1, objSize, m_hFile);
  if( Lr != objSize)
  {
    m_dwLastError = ferror(m_hFile);
    //m_dwLastError = GetLastError();
    return false;
  }
  ASSD( Lr==objSize );
  return true;
}
