#include "SafetyManager.h"
#include <chrono>

int g_chVer = 0x3500000C; //0x3500000B;

KSafetyManager::KSafetyManager()
: m_Ready( FALSE )
{
}

int KSafetyManager::Initialize( LPCSTR pszPrj )
{
  OutputDebugString( "KSafetyManager::Init " );
  OutputDebugString( pszPrj );
  OutputDebugString( " ...\n" );
  auto t = std::chrono::high_resolution_clock::now();

  e_RestResult res = RestoryData( pszPrj );
  switch( res )
  {
  case err_DatVerGreaterSrcVer:
    {
#ifdef _WIN32
      if( IDYES!=MessageBox( NULL, "Использовать текущую версию будете на свой страх и риск.\nПродолжить?"
                                 , "Устаревшая версия SafetyManager. Обновите SM.", MB_TOPMOST|MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2) )
      {
        exit( 1 );
      }
#else
      return 2;
#endif
    }
  case err_WrongKey:
  case err_WrongVer:
  case err_CantOpen:
    ReadProject( pszPrj );
    if( err_Ok != RestoryData( pszPrj ) )
      return 2;
    break;
  default:
  case err_CommonErr:
    return 1;
  case err_Ok:
    break;
  }

  auto d = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t).count();
  char szDebug[64*4];
  sprintf_s( szDebug, "время инициализации SafetyManager %lld ms\n", d );
  OutputDebugString( szDebug );

  return 0;
}

#ifdef _WIN32
void KSafetyManager::FscFileDraw( CMyFont& font, UINT nFile, UINT nSelect, bool bNumber )
{
  mFile.Draw( this, nFile, font, nSelect, bNumber );
}
#endif