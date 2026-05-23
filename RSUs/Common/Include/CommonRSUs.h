#pragma once
#include <stdlib.h>
#include <rsuErr.h>
#ifdef COMMONRSUS_EXPORTS
#define COMMONRSUS_API Q_DECL_EXPORT        
#else
#define COMMONRSUS_API Q_DECL_IMPORT        
#pragma comment( lib, "CommonRSUs.lib")
#endif

bool COMMONRSUS_API IsThisModuleLoaded( LPCTSTR szClassName = NULL );
// Возвращает полный путь к файлу сообщений
COMMONRSUS_API LPCSTR GetModel2OperatorMessagesFilePath();

int COMMONRSUS_API RsuPackager( LPCSTR pszSrcFile, LPCSTR pszArchive, LPCSTR pszNamePackAs );
HANDLE COMMONRSUS_API RsuExtractor( LPCSTR pszArchive );
bool COMMONRSUS_API RsuExtractor( LPCSTR pszArchive, LPSTR pszOutFile, size_t nFile );

bool COMMONRSUS_API is_valid_utf8( LPCSTR string );
int COMMONRSUS_API convert_utf8_to_windows1251( LPCSTR utf8, LPSTR windows1251, size_t n );

template<size_t _S>
void Utf8ToAnsi( LPCSTR pszUtf8, char (&szOut)[_S] )
{
  // szOut[0] = 0;
  // if( !pszUtf8 )
  //   return;
  // wchar_t wBuf[_S];
  // int nW = MultiByteToWideChar( CP_UTF8, 0, pszUtf8, -1, wBuf, _countof(wBuf) );
  // ASSD( nW < _S );
  // int nSZ = WideCharToMultiByte( CP_ACP, 0, wBuf, -1, szOut, _countof(szOut), NULL, NULL );
  // ASSD( nSZ < _S );
}

COMMONRSUS_API LPCSTR Utf8ToAnsi( LPCSTR pszInUtf8 );


COMMONRSUS_API void Bin2Text( BYTE *pMem, size_t nBytes, char *pszOut, size_t _S );
template<size_t _S>
void Bin2Text( BYTE *pMem, size_t nBytes, char (&szOut)[_S] )
{
  Bin2Text( pMem, nBytes, szOut, _S );
}
COMMONRSUS_API void Text2Bin( BYTE *pMem, size_t nBytes, LPCSTR pszText, size_t L );
