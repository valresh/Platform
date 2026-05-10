#include <rsuErr.h>
#include "ControlBuilder.h"
#include <macros/AutoCloser.h>
#include <locale.h>
#include "CBPool.h"
#include <macros/rsuINI_C.h>
#include <chrono>

CONTROLBUILDERH5XX_API KCBPool g_Pool;

KControlBuilder::KControlBuilder( LPCSTR ObjName )
: m_entryModule( NULL )
, m_nAcsFindStep( 0 )
, g_throughNumb( 0 )
, pszSelectedModule( NULL )
, m_bStep0( false )
, m_readNewProject( 0 )
, m_usePhases( false )
{
  char tranformName[64*4];
  ZeroMemory( m_szEntrySystem, sizeof(m_szEntrySystem) );

  strcpy_s( tranformName, ObjName );
  strcpy_s( m_szPrjName, ObjName );
  strcpy_s( m_szObjName, ObjName );
  LPSTR p = strchr(tranformName,'(');
  if( p )
  {
    m_szPrjName[ p-tranformName ] = 0;
    *p = 0;
    ++p;
    strcpy_s( m_szEntrySystem, p );
    p = strrchr( m_szEntrySystem, ')');
    if( p )
      *p = 0;
  }
  /*else
    strcpy_s( m_szEntrySystem, tranformName );*/

  g_Pool.RegisterCB( this );
}

KControlBuilder::~KControlBuilder()
{
}

int KControlBuilder::Step0L()
{
  if( m_bStep0 )
    return 0;
  m_bStep0 = true;
  OutputDebugString( "KControlBuilder::Step0 " );
  OutputDebugString( m_szPrjName );
  OutputDebugString( " ...\n" );
  auto t = std::chrono::high_resolution_clock::now();

  SH_Block::s_bProjectLoaded = false;
  if( m_entryModule )
  {
    //char szPath[_MAX_PATH];
    char name[128*4];
    sprintf_s( name, "%s%s", GetCsvPrefix(), m_szObjName );
    auto szPath = rsu_cp::makepath_with_ext_cp(rsuGetMemoryPath(), name, "csv");
    //_makepath_s( szPath, NULL, rsuGetMemoryPath(), name, "csv" );
    KAutoCloser<FILE*> fAI;
    bool bAny = false;
#ifdef _DEBUG
    //bAny = true;
#endif
    if( 0x00123400==m_readNewProject || bAny )
    {
      fAI.Assign( fopen( szPath.generic_string().c_str(), "wt"), fclose);
      if( NULL!=fAI )
        fprintf( fAI, "ED#Точка;ED#Alt;TypeDV#Тип;ST#Описание;LE,Objects#Модель;LS,Type#ТипМодели;Points#ACS;ED#Формула;ST#EngUnit;\n" );
    }
    LinkConnections( m_entryModule );
    AssignToContainers( m_entryModule, fAI );
  }
  SH_Block::s_bProjectLoaded = true;

  if( m_entryModule )
    m_entryModule->StepBeforeRestoreState( NULL, m_entryModule, NULL );

  auto duration = std::chrono::high_resolution_clock::now() - t;
  char szDebug[128*4];
  sprintf_s( szDebug, "время связывания объетов ControlBuilder %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
  OutputDebugString( szDebug );

  return 0;
}

int KControlBuilder::Step1L()
{
  char res[16] = { 0 };
  rsuINI_C( "USE_PHASES", "0", res );
  m_usePhases = atoi(res) ? true : false;
  if( m_entryModule )
    m_entryModule->StepAfterRestoreState();
  m_nFirstSteps = 0;
  return 0;
}
