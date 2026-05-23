#include "stdafx.h"
#include <CommonRSUs.h>
//#include <Psapi.h>
#include <string>
#include <rsuPaths.h>

#include <macros/AutoCloser.h>
#include <vector>
#include <contrib/minizip/unzip.h>
#include <contrib/minizip/zip.h>
#include <zlibH.h>


template<size_t _SIZE>
void RemoveUpDirPoints( char (&pszPath)[_SIZE] )
{
  using namespace std;
  string szPath( pszPath );
  string::size_type pos = szPath.find( ".." );
  while( string::npos != pos )
  {
    string prevPath = szPath.substr( 0, pos-1 );
    string::size_type posPrev = prevPath.find_last_of( "\\/" );
    szPath = prevPath.substr( 0, posPrev+1) + szPath.substr( pos+3 );
    pos = szPath.find( ".." );
  }
  strcpy_s( pszPath, szPath.c_str() );
}

bool IsThisModuleLoaded( LPCTSTR szClassName )
{
  DWORD dwCount = 0;
  DWORD dwSize = 1024;  // for 256 processes
  LPDWORD id = new DWORD[dwSize];
  if ( id != NULL )
  {
    while ( ::EnumProcesses( id, dwSize*sizeof(DWORD), &dwCount ) )
    {
      if( dwSize*sizeof(DWORD) > dwCount )
      {
        dwCount /= sizeof( DWORD );
        break;
      }
      delete [] id;
      dwCount = 0;
      dwSize *= 2;
      id = new DWORD[ dwSize ];
      if ( id == NULL )
        break;
    }
  }
  DWORD dwError = ::GetLastError();
  //
  bool bReturn = false;
  //
  char szPath[_MAX_PATH];
  char szProc[_MAX_PATH];
  ::GetModuleFileName( NULL, szPath, sizeof(szPath) );
  RemoveUpDirPoints( szPath );
  //
  for ( DWORD n = 0; n < dwCount; n++ )
  {
    if  ( id[n] != ::GetCurrentProcessId() )
    {
      HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE, id[n] );
      if ( NULL != hProcess )
      {
        HMODULE hMod;
        DWORD cbNeeded;
        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
        {
          GetModuleFileNameEx( hProcess, hMod, szProc, sizeof(szProc) );
          RemoveUpDirPoints( szProc );
          if ( _stricmp( szPath, szProc ) == 0 )
            bReturn = true;
        }
        CloseHandle( hProcess );
      }
      if ( bReturn ) break;
    }
  }
  //
  if ( id != NULL ) delete [] id;
  //
  if ( szClassName != NULL && bReturn )
  {
    HWND hWnd = ::FindWindow(szClassName,NULL);
    if(::IsWindow(hWnd))
    {
      ::SetForegroundWindow( hWnd );
    }
  }
  //
  return bReturn;
}

LPCSTR GetModel2OperatorMessagesFilePath()
{
  static TCHAR szPath[_MAX_PATH] = {0};
  if( *szPath == CHAR('\0') )
    sprintf_s( szPath, "%s_%s", rsuGetMemoryPath(), "Model2OperatorMessages.dat" );
  return szPath;
}

int RsuPackager( LPCSTR pszSrcFile, LPCSTR pszArchive, LPCSTR pszNamePackAs )
{
  zipFile zf = zipOpen( pszArchive, 0);
  if( !zf )
    return -1;

  zip_fileinfo zi;
  unsigned long crcFile=0;
  int size_buf = 8192;
  std::vector<BYTE> buf;
  buf.resize( size_buf );

  zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
    zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
  zi.dosDate = 0;
  zi.internal_fa = 0;
  zi.external_fa = 0;
  filetime( pszSrcFile,&zi.tmz_date,&zi.dosDate);

  int err = getFileCrc( pszSrcFile, &buf[0], size_buf, &crcFile);

  err = zipOpenNewFileInZip3( zf, pszNamePackAs, &zi,NULL,0,NULL,0,NULL,Z_DEFLATED,8,0,-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, NULL,crcFile);

  {
    KAutoCloser<FILE*> fin( fopen(pszSrcFile,"rb"), fclose);
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
            err = ZIP_ERRNO;
          }

          if (size_read>0)
          {
            err = zipWriteInFileInZip (zf, &buf[0],size_read);
          }
      } while ((err == ZIP_OK) && (size_read>0));
      err = zipCloseFileInZip(zf);
    }
  }
  zipClose( zf, NULL);
  return 0;
}

HANDLE RsuExtractor( LPCSTR pszArchive )
{
  DWORD dwFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN;

  KAutoCloser<unzFile> uf( unzOpen( pszArchive ), unzClose );
  char path_buffer[_MAX_PATH] = { 0 };

  if( NULL!=uf )
  {
    strcpy_s( path_buffer, pszArchive );
    strcat_s( path_buffer, ".tmp" );

    int err = UNZ_OK;

    char filename_inzip[256];
    unz_file_info file_info;
    err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,_countof(filename_inzip),NULL,0,NULL,0);
    if( err==UNZ_OK )
    {
      err = unzOpenCurrentFilePassword( uf, NULL );
      KAutoCloser<FILE*> fout( fopen(path_buffer,"wb"), fclose);
      if( fout!=NULL )
      {
        std::vector<BYTE> buf;
        buf.resize( 8192 );
        do
        {
          err = unzReadCurrentFile( uf, &buf[0], (unsigned int)buf.size() );
          if (err>0)
          {
            if( fwrite( &buf[0], err, 1, fout )!=1)
            {
              err = UNZ_ERRNO;
              break;
            }
          }
        }while (err>0);
        if( UNZ_OK==err )
          pszArchive = path_buffer, dwFlagsAndAttributes |= FILE_FLAG_DELETE_ON_CLOSE;
      }
    }
  }


  HANDLE hFile = CreateFile(pszArchive,
                            GENERIC_READ,
                            FILE_SHARE_READ|FILE_SHARE_WRITE,
                            NULL,
                            OPEN_EXISTING,
                            dwFlagsAndAttributes,
                            NULL );
  return hFile;
}

bool RsuExtractor( LPCSTR pszArchive, LPSTR pszOutFile, size_t nFile )
{
  pszOutFile[0] = 0;

  KAutoCloser<unzFile> uf( unzOpen( pszArchive ), unzClose );
  char path_buffer[_MAX_PATH] = { 0 };

  if( NULL!=uf )
  {
    strcpy_s( path_buffer, pszArchive );
    strcat_s( path_buffer, ".tmp" );

    int err = UNZ_OK;

    char filename_inzip[256];
    unz_file_info file_info;
    err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,_countof(filename_inzip),NULL,0,NULL,0);
    if( err==UNZ_OK )
    {
      err = unzOpenCurrentFilePassword( uf, NULL );
      KAutoCloser<FILE*> fout( fopen(path_buffer,"wb"), fclose);
      if( fout!=NULL )
      {
        std::vector<BYTE> buf;
        buf.resize( 8192 );
        do
        {
          err = unzReadCurrentFile( uf, &buf[0], (unsigned int)buf.size() );
          if (err>0)
          {
            if( fwrite( &buf[0], err, 1, fout )!=1)
            {
              err = UNZ_ERRNO;
              break;
            }
          }
        }while (err>0);
        if( UNZ_OK==err )
        {
          strcpy_s( pszOutFile, nFile, path_buffer );
          return true;
        }
      }
    }
  }
  return false;
}


static char const hex_chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void Bin2Text( BYTE *pMem, size_t nBytes, char *pszOut, size_t _S )
{
  size_t O = 0;
  for( size_t i=0; i<nBytes; ++i )
  {
    BYTE b = pMem[i];
    pszOut[O++] = hex_chars[ ( b & 0xF0 ) >> 4 ];
    pszOut[O++] = hex_chars[ ( b & 0x0F ) >> 0 ];
    ASSD( O<_S );
    if( O==_S )
      break;
  }
  pszOut[O] = 0;
  ASSD( O<_S );
}

void Text2Bin( BYTE *pMem, size_t nBytes, LPCSTR pszText, size_t L )
{
  size_t O = 0;
  ASSD( 0==(L%2) );
  if( 0!=(L%2) )
    return;
  for( size_t i=0; i<L; i+=2, ++O )
  {
    BYTE hn = pszText[i] > '9' ? pszText[i] - 'A' + 10 : pszText[i] - '0';
    BYTE ln = pszText[i+1] > '9' ? pszText[i+1] - 'A' + 10 : pszText[i+1] - '0';
    pMem[O] = (hn << 4 ) | ln;
    ASSD( O<nBytes );
  }
}
