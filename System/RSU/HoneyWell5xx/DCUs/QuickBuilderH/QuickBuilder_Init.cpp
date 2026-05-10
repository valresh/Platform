#include "QuickBuilder.h"
#include <chrono>
//#include <SetData.h>

int KQuickBuilder::InitL()
{
  OutputDebugString( "QuickBuilder::Init...\n" );
  auto t = std::chrono::high_resolution_clock::now();

  e_RestResult res = RestoreProject();
  switch( res )
  {
  case err_DatVerGreaterSrcVer:
    {
#ifdef _WIN32
      if( IDYES!=MessageBox( NULL, "Использовать текущую версию будете на свой страх и риск.\nПродолжить?"
        , "Устаревшая версия QuickBuilder. Обновите QB.", MB_TOPMOST|MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2) )
#endif
      {
        exit( 1 );
      }
    }
  case err_WrongKey:
  case err_WrongVer:
  case err_CantOpen:
    if( ReadProject() )
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
  sprintf_s( szDebug, "время инициализации QuickBuilder %lld ms (%d точек)\n", d, m_nPoints );
  OutputDebugString( szDebug );
  return 0;
}

int KQuickBuilder::Step0L()
{
  return 0;
}

int KQuickBuilder::Step1L()
{
  OutputDebugString( "KQuickBuilder::Step1 " );
  OutputDebugString( m_szObjName );
  OutputDebugString( " ...\n" );
  auto t = std::chrono::high_resolution_clock::now();
  if ( !m_bStep1 )
  {
    for( DWORD n=0; n<m_nPoints; ++n )
    {
      m_ppPoints[n]->Link();
    }
	m_bStep1 = true;
  }

  for( DWORD n=0; n<m_nPoints; ++n )
  {
    m_ppPoints[n]->AfterRestoreState();
  }
  
  auto d = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t).count();
  char szDebug[128*4];
  sprintf_s( szDebug, "время связывания объетов QuickBuilder %lld ms\n", d );
  OutputDebugString( szDebug );

  return 0;
}
/*
int KQuickBuilder::SetData( int TypeData, void * pData )
{
  if( sd_SetRsuLinkCsv==TypeData )
  {
    m_bCreateData = true;
  }
  return 0;
}
*/