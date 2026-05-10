#include <crossplatform.h>
#include "FlexRsu.h"
#include <macros/StrHelps.h>

BYTE* KFlexRsu::GetTagField( LPSTR pszPntName, eVarType *pType /*= NULL*/, tVarSizeType_ *pVarSize /*= NULL*/ )
{
  return GetVar( pszPntName, pType, pVarSize );
}

BYTE* KFlexRsu::GetVar( LPSTR pszPntName, eVarType *pType /*= NULL*/, tVarSizeType_ *pVarSize /*= NULL*/ )
{
  LPCSTR pszField = "PV";
  LPSTR p = NULL;
  while( 1 )
  {
    TStringTerminator st( p );
    for( DWORD n=0; n<m_nDR; ++n )
    {
      KFlexDR *pDR = m_ppDR[n];
      KFlexBlockBase *pBlock = pDR->GetBlock( pszPntName );
      if( !pBlock )
        continue;
      BYTE *pMem = pBlock->GetField( pszField, pType, pVarSize );
      return pMem;
    }
    p = strrchr( (LPSTR)pszPntName, '.');
    if( !p )
      break;
    pszField = p+1;
  }
  return NULL;
}

#ifdef _WIN32

bool KFlexRsu::GetFirstACSObject()
{
  m_nAcsFindStep = 0;
  return true;
}

bool KFlexRsu::GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd )
{
  switch( m_nAcsFindStep )
  {
  case 0:
    strncpy( Name, m_szObjName, sn);
    strncpy( Data, "FlexRsu", sd);
    *pData = this;
    m_nAcsFindStep++;
    return true;
  }
  return false;
}

bool KFlexRsu::ShowACSObject( void * pInfo, HWND hMainWnd, LPCSTR Name, void * pData )
{
  if( _strcmpi(Name, m_szObjName) )
    return false;
  SPrjCfg Config;
  Config.ppDR = m_ppDR;
  Config.nDR = m_nDR;
  Config.pSteps = &m_nStep;
  Config.pInitCounter = &m_nInitCounter;
  Config.pSkipSimIO = &s_bSkipSimIO;
  Config.pFlexSystem = m_pFlexSystem;
#ifdef _WIN32
  ShowDebugWnd( m_szObjName, hMainWnd, &Config, pData==(void*)1 ? true : false );
#endif
  return true;
}

void KFlexRsu::CloseAllDebugWnd()
{
  //CloseDebugWnds();
}
#endif