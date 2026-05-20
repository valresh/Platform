#include "CommProc.h"
#include "SR.h"
#include "../zlib/zlib.h"
#include "../zlib/contrib/minizip/unzip.h"
#include "../zlib/contrib/minizip/zip.h"


CSRFile * pSRFile;
bool WriteToMem = false;

//BYTE CSaveVarData::Buf[MAX_DATA_BUF];

CSRFile::CSRFile()
  : m_szBuff(nullptr)
  , m_dwCurr(0)
  , m_dwSize(0)
  {
  Bytes = 0;
  InvalidLen = Assert;
  //ReadBytes = 0;
//  memsetZeroMemory( m_szFilePath, sizeof(m_szFilePath) );
  WriteToMem = false;
  hFile = NULL;
  }

CSRFile::~CSRFile()
  {
  if ( m_szBuff )
    delete[] m_szBuff;
  m_szBuff = nullptr;
  if ( hFile != INVALID_HANDLE_VALUE )
    {
    ::CloseHandle ( hFile );
  }
}


bool CSRFile::Create( const char* szPath )
  {
  if ( hFile != INVALID_HANDLE_VALUE )
    ::CloseHandle ( hFile );
  //	F = fopen ( "X:\\Write.txt", "wt" );
  hFile = CreateFile( szPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL );
  strcpy_s( m_szFilePath, szPath );
  return hFile != INVALID_HANDLE_VALUE;
  }

_HANDLE GetFileContent( LPCSTR pszFile, char path_buffer[1024], DWORD &fileSize )
  {
//  DWORD dwFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN;

  unzFile uf = unzOpen( pszFile );
  sprintf( path_buffer, "%sTEMP/State.dat", PROJECT_ROOT );
  QFile::remove( path_buffer );
  fileSize = 0;
  if( NULL != uf )
    {
    int err = UNZ_OK;
    char filename_inzip[256];
    unz_file_info file_info;
    err = unzGetCurrentFileInfo (uf,&file_info, filename_inzip,_countof(filename_inzip),NULL,0,NULL,0);
    if( err == UNZ_OK )
      {
      err = unzOpenCurrentFilePassword( uf, NULL );
      FILE* fout = fopen(path_buffer, "wb" );
      if( fout != NULL )
        {
        BYTE buf[8192];
        do
          {
          err = unzReadCurrentFile( uf, buf, 8192 );
          if (err>0)
            {
            if( fwrite( buf, err, 1, fout )!=1)
              {
              err = -1;
              break;
              }
            }
          } while ( err > 0 );
        if( UNZ_OK == err )
          {
          pszFile = path_buffer;
          fclose ( fout );
//          dwFlagsAndAttributes |= FILE_FLAG_DELETE_ON_CLOSE;
          fileSize = file_info.uncompressed_size;
          }
        else
          return INVALID_HANDLE_VALUE;
      }
    }
  }
  else
    return INVALID_HANDLE_VALUE;
  _HANDLE hFile = CreateFile( pszFile,
                            GENERIC_READ,
                            0,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL );
  return hFile;
}

BOOL CSRFile::_WriteFile(
  _HANDLE hFile,
  LPCVOID lpBuffer,
  DWORD nNumberOfBytesToWrite,
  LPDWORD lpNumberOfBytesWritten,
  void *
  )
  {
  if ( WriteToMem )
    {
    if ( m_dwCurr + nNumberOfBytesToWrite > m_dwSize )
      {
      ASS(0)
      return FALSE;
      }
    memmove ( m_szBuff + m_dwCurr, lpBuffer, nNumberOfBytesToWrite );
    *lpNumberOfBytesWritten = nNumberOfBytesToWrite;
    return TRUE;
    }
  return ::WriteFile( hFile, (void*)lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL );
  }

bool CSRFile::Open( const char* szPath )
  {
  if ( m_szBuff )
    delete m_szBuff;
  m_dwCurr = 0;
  m_dwSize = 0;

  char path_buffer[1024];
  hFile = GetFileContent( szPath, path_buffer, m_dwSize );
  szPath = path_buffer;
  if( m_dwSize )
    {
    DWORD read = 0;
    m_szBuff = (char*)new char[m_dwSize+1];
    if( ReadFile( hFile, m_szBuff, m_dwSize, &read, NULL) )
      {
      if( read != m_dwSize )
        {
        delete[] m_szBuff;
        return false;
      }
    }
    else
      delete[] m_szBuff;
    }
  else
    {
    ASS(0);
    //m_szBuff = ::FileToBuff( szPath, m_dwSize );
    }
  if ( m_szBuff == 0 )
    return false;
  if ( m_dwSize == 0 )
    return false;
  return true;
  }

//
DWORD CSRFile::SetFilePointer( LONG lDistanceToMove, DWORD dwMoveMethod )
{
  return ::SetFilePointer( hFile, lDistanceToMove, NULL, dwMoveMethod );
}
//
DWORD CSRFile::Skip( LONG Bytes )
{
  m_dwCurr += Bytes;
  return m_dwCurr;
}
//
bool CSRFile::WF( LPVOID lpVoid, DWORD dwSize, bool Txt )
{
  if( !lpVoid )
    return true;
  DWORD Lw = 0;
  // char T[1024];
  // if ( Txt )
  //   {
  //   dwSize = UtfToChar_s( (unsigned char *)T, 1024, (char*)lpVoid, dwSize );
  //   lpVoid = T;
  //   }
  if ( !_WriteFile ( hFile, lpVoid, dwSize, &Lw, NULL ) )
    {
//    dwError = ::GetLastError();
    return false;
    }
  //
  m_dwCurr += dwSize;
  return true;
}
//
bool CSRFile::RF( LPVOID lpVoid, DWORD dwSize, bool Txt )
  {
  if ( (m_dwCurr + dwSize) > m_dwSize )
    {
    ASS(0);
    return false;
    }
  char T[1024];
  if( lpVoid )
    {
    // if ( Txt )
    //   {
    //   int L = CharToUTF_s( T, 1024, (char*)m_szBuff+m_dwCurr, dwSize );
    //   memcpy( lpVoid, T, L );
    //   }
    // else
      memcpy( lpVoid, m_szBuff+m_dwCurr, dwSize );
    }
  m_dwCurr += dwSize;
  return true;
  }

  // Длина + данные
bool CSRFile::WD( LPVOID lpVoid, DWORD dwSize )
  {
  DWORD Lw = 0;
  if ( !_WriteFile ( hFile, &dwSize, sizeof(dwSize), &Lw, NULL ) )
    {
//    dwError = ::GetLastError();
    return false;
    }
  //
  m_dwCurr += sizeof(dwSize);
  return WF(lpVoid, dwSize);
  }
// Длина + данные
bool CSRFile::RD( LPVOID lpVoid, DWORD dwSize )
  {
  if ( (m_dwCurr+sizeof(dwSize)) > m_dwSize )
    {
    ASS(0);
    return false;
    }
  DWORD saveSize = 0;
  memcpy( &saveSize, m_szBuff+m_dwCurr, sizeof(dwSize) );
  m_dwCurr += sizeof(dwSize);
  if( saveSize != dwSize )
    {
    switch ( InvalidLen )
    {
      case Ignore:
        {
          InvalidLen = Assert;
          Skip( saveSize );
        }
        return true;
      case Assert:
        {
          ASS(FALSE)
          InvalidLen = Assert;
          Skip( saveSize );
        }
        return true;
      case CopyFirst:
        {
          InvalidLen = Assert;
          if( saveSize <= dwSize )
            return RF( lpVoid, saveSize);
          bool Res = RF( lpVoid, dwSize );
          Skip( saveSize - dwSize );
          return Res;
        }
    }
  }
  return RF( lpVoid, saveSize );
}
//
bool CSRFile::WS( char * str )
  {
  int L = lstrlen( str );
  if ( !WF( &L, sizeof ( int ) ) )
    return false;
  if ( !WF( str, L ) )
    return false;
  return true;
  }

bool CSRFile::RS( char * str )
  {
  int L = 0;
  if ( !RF( &L, sizeof ( int ) ) )
    return false;
  if ( !RF( str, L ) )
    return false;
  str[L] = 0;
  return true;
  }
//
char * CSRFile::RS( int Len )
  {
  int L = 0;
  if ( !RF( &L, sizeof ( int ) ) )
    return NULL;
  if ( Len > 0 )
    {
    ASS ( L <= Len );
    }
  else
    Len = L;
  char * str = new char[Len+1];
  if ( !RF( str, L ) )
    return NULL;
  str[L] = 0;
  return str;
  }
//
bool CSRFile::WHeadStruct( char * _Name, int Len )
  {
  char Name[256];
  UtfToChar( (unsigned char *)Name, 256, _Name );
  static int Key = 0xE1320724;
  if ( !WF( &Key, sizeof ( int ) ))
    return false;
  int L = lstrlen ( Name );
  if ( !WF( &L, sizeof ( int ) ))
    return false;
  if ( !WF( Name, L ) )
    return false;
  L = Len;
  if ( !WF( &L, sizeof ( int ) ))
    return false;
  return true;
  }

bool CSRFile::WStruct( char * _Name, int Len, void * Addr )
  {
  char Name[256];
  UtfToChar( (unsigned char *)Name, 256, _Name );
  static int Key = 0xE1320724;
  if ( !WF( &Key, sizeof ( int ) ))
    return false;
  int L = lstrlen ( Name );
  if ( !WF( &L, sizeof ( int ) ))
    return false;
  if ( !WF( Name, L ) )
    return false;
  L = Len;
  if ( !WF( &L, sizeof ( int ) ))
    return false;
  if ( L == 0 )
    return true;
  if ( !WF( Addr, L ))
    return false;
  return true;
  };
//
bool CSRFile::RStruct( int Len, void * Addr )
  {
  if ( Len == Bytes )
    {
    InvalidLen = Assert;
    return RF( Addr, Len );
    }
  ///////////////////////////////////////////
  // Ошибочная длина
  switch ( InvalidLen )
    {
    case Ignore:
      {
        InvalidLen = Assert;
        Skip( Bytes );
      }
      return true;
    case Assert:
      {
        ASS(FALSE)
        InvalidLen = Assert;
        Skip( Bytes );
      }
      return true;
    case CopyFirst:
      {
        InvalidLen = Assert;
        if ( Bytes < Len )
          return RF( Addr, Bytes );
        bool Res = RF( Addr, Len );
        Skip( Bytes - Len );
        return Res;
      }
    }
  ASS(FALSE)
  InvalidLen = Assert;
  return false;
  };

bool CSRFile::RStruct( int Len, void * Addr, int SkipBegin )
  {
  BYTE * A = (BYTE*)Addr;
  if ( Len == Bytes )
  {
    InvalidLen = Assert;
    Skip( SkipBegin );
    return RF( A + SkipBegin, Len - SkipBegin );
  }
  // Ошибочная длина
  switch ( InvalidLen )
  {
    case Ignore:
      {
        InvalidLen = Assert;
        Skip( Bytes );
      }
      return true;
    case Assert:
      {
        ASS(FALSE)
        InvalidLen = Assert;
        Skip( Bytes );
      }
      return true;
    case CopyFirst:
      {
        InvalidLen = Assert;
        Skip( SkipBegin );
        if ( Bytes < Len )
          return RF( A + SkipBegin, Bytes - SkipBegin );
        bool Res = RF( A + SkipBegin, Len - SkipBegin );
        Skip( Len - Bytes );
        return Res;
      }
  }
  ASS(FALSE)
  InvalidLen = Assert;
  return false;
};

//
DWORD CSRFile::Close()
  {
  DWORD L = 0xFFFFFFFF;
  if ( hFile != INVALID_HANDLE_VALUE )
    {
    L = GetFileSize( hFile, NULL );
    ::CloseHandle ( hFile );
    }
  hFile = INVALID_HANDLE_VALUE;
  char szTmpFile[_MAX_PATH];
  sprintf( szTmpFile, "%sTEMP/SRFile.dat", PROJECT_ROOT );
  QFile::remove( szTmpFile );
  if( QFile::copy(m_szFilePath, szTmpFile ))
    {
    // char fname[_MAX_FNAME];
    // char ext[_MAX_EXT];
    // char filenameinzip[_MAX_FNAME] = { 0 };

    // _splitpath_s( m_szFilePath, NULL, 0, NULL, 0, fname, _countof(fname), ext, _countof(ext) );
    // _makepath_s( filenameinzip, _countof(filenameinzip), NULL, NULL, fname, ext );

    zipFile zf = zipOpen( m_szFilePath, 0);
    if( !zf )
      return 0;
    zip_fileinfo zi;
    unsigned long crcFile = 0;
    const int size_buf = 8192;
//    std::vector<BYTE> buf;
//    buf.resize( size_buf );
    zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
    zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
    zi.dosDate = 0;
    zi.internal_fa = 0;
    zi.external_fa = 0;
    filetime(szTmpFile, &zi.tmz_date,&zi.dosDate);
    char filenameinzip[_MAX_FNAME];
    sprintf( filenameinzip, "%s/TEMP/ZipFile.dat", PROJECT_ROOT );
    QFile::remove( filenameinzip );
    char buf[size_buf];
    int err = getFileCrc( szTmpFile, buf, size_buf, &crcFile);
    int L = 0;
    err = zipOpenNewFileInZip3( zf,filenameinzip,&zi,NULL,0,NULL,0,NULL,Z_DEFLATED,8,0,-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, NULL,crcFile);
      {
      FILE* fin =fopen( szTmpFile,"rb");
      if( fin )
        {
        int size_read = 0;
        do
          {
          err = ZIP_OK;
          size_read = (int)fread( &buf[0],1,size_buf,fin);
          if (size_read < size_buf)
          if (feof(fin)==0)
            {
            err = -1;
            }
          if ( size_read > 0 )
            {
            err = zipWriteInFileInZip (zf, &buf[0],size_read);
            }
        } while ((err == ZIP_OK) && (size_read>0));
        err = zipCloseFileInZip(zf);
      }
    }
    zipClose( zf, NULL);
    DeleteFile( szTmpFile );
  }
  return L;
}
