#include "stdafx.h"
#include "OpenInGraph.h"
#include "../../FlexDcuEditor/FlexRsuDebugBridgeToGUI/memoryDescriptor.h"
#include <rsus/rsuPaths.h>
#include <FlexRsuCommon.h>
#include <vector>
#include <rsuErr.h>

using namespace n_GUI;
using namespace std;

KOpenInGraph::KOpenInGraph() : m_hGui( NULL )
{
}

typedef std::vector<SSignalDescriptor> tSignalsCont_;

void KOpenInGraph::Show( LPCSTR pszPrjName, LPCSTR pszFileName, KFlexDR *pDR )
{
  char szDir[_MAX_PATH * 4];
  std::string szPath = "";
  if( !m_hGui )
  {
    fs::path p = fs::path(rsuGetExecutionPath()) / fs::path("FlexRsuDebugBridgeToGUI.dll");
    szPath = p.generic_string();
    m_hGui = LoadLibrary( szPath.c_str() );
    if( !m_hGui )
    {
      strcpy_s( szDir, "Не загрузилась \"" );
      strcat_s( szDir, szPath.c_str() );
      strcat_s( szDir, "\"" );
      MessageBox( NULL, szDir, "Работать не будет", MB_OK|MB_ICONERROR );
      return;
    }
    CoInitialize(NULL);
  }

  ns_FlexRsu::GetPath2Logic( szDir, pszPrjName );
  std::string ext = std::string(".") + std::string(ns_FlexRsu::GetLogicFileExtension());
  szPath = rsu_cp::makepath_with_ext_cp(szDir, pszFileName, ext).generic_string();

  tSignalsCont_ links;

  lpfnOpenDebugGUI call = (lpfnOpenDebugGUI)GetProcAddress( m_hGui, "OpenDebugGUI" );
  lpfnOpenDebugGuiWithInfo callWithInfo = (lpfnOpenDebugGuiWithInfo)GetProcAddress( m_hGui, "OpenDebugGuiWithInfo" );

  if( pDR->m_nCons )
  {
    links.resize( pDR->m_nCons );
    for( DWORD n=0; n<pDR->m_nCons; ++n )
    {
      KFlexConnection &conSrc = *pDR->m_ppConnections[n];
      tSignalsCont_::value_type &dst = links.at(n);
      dst.srcMem.objID = conSrc.m_SourceID;
      dst.srcMem.type = conSrc.GetSourceType();
      dst.srcMem.pszFieldName = conSrc.m_szSource;
      dst.srcMem.memAddr = conSrc.GetSrcRawMem();

      dst.destMem.objID = conSrc.m_SinkID;
      dst.destMem.type = conSrc.GetSinkType();
      dst.destMem.pszFieldName = conSrc.m_szSink;
      dst.destMem.memAddr = conSrc.GetDstRawMem();
    }
    if( callWithInfo )
    {
      int r = callWithInfo( szPath.c_str(), pszFileName, &links[0], pDR->m_nCons);
      if( !r )
        return;
    }
  }

  if( call )
    call( szPath.c_str(), pszFileName);
}
