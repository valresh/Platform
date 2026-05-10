#include "ScriptableBlock.h"
#include <FlexRsuConnect.h>
#include <rsuIsType.h>

using namespace ns_translator;
using namespace std;
using namespace ns_Script;

bool KCalcVar::Numb()
{
  if( ::IsInt(szName) )
  {
    intValue = atoi(szName);
    pMem = (BYTE*)&intValue;
    type = evtDigital32;//evtConstInt;
  }
  else if( ::IsFlt( szName ) )
  {
    fltValue = (tAnalog32)_atof(szName);
    pMem = (BYTE*)&fltValue;
    type = evtAnalog32;//evtConstFloat;
  }
  else
  {
    ASSD(0);
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////
bool KScriptableBlock::IsVar( LPCSTR pszVarName )
{
  BYTE* pMem = GetField( pszVarName );
  return pMem ? true : false;
}

void KScriptableBlock::AllocScriptCode( KFlexTranslator &trans )
{
  trans.GetResult( m_scriptParams.cVars, m_scriptParams.cOPs);

  vector<KVar> vVars;
  vector<SOPElement> pOPs;
  if( m_scriptParams.cVars )
    vVars.resize( m_scriptParams.cVars );
  if( m_scriptParams.cOPs )
    pOPs.resize( m_scriptParams.cOPs );

  trans.GetResult( m_scriptParams.cVars ? &vVars[0] : NULL, m_scriptParams.cOPs ? &pOPs[0] : NULL );

  m_rawScriptSize = sizeof(KCalcVar) * m_scriptParams.cVars + sizeof(SOPElement) * m_scriptParams.cOPs;
  DWORD size = m_rawScriptSize;
  if( !m_pRawScript && m_rawScriptSize )
  {
    m_pRawScript = pFlexMemoryAllocator( m_rawScriptSize );
    if( m_scriptParams.cVars )
      m_pVars = new (m_pRawScript) KCalcVar[m_scriptParams.cVars];
    if( m_scriptParams.cVars && m_scriptParams.cOPs )
      m_pOPs = (SOPElement*)(m_pRawScript + sizeof(KCalcVar) * m_scriptParams.cVars);
  }

  if( m_scriptParams.cVars && m_pVars )
  {
    for( DWORD i=0; i<m_scriptParams.cVars; i++ )
    {
      KVar *var = &vVars[i];
      KVar *cv = &m_pVars[i];
      *cv = *var;
    }
  }
  if( m_scriptParams.cVars && m_scriptParams.cOPs && m_pOPs )
  {
    memcpy( m_pOPs, &pOPs[0], sizeof(SOPElement) * m_scriptParams.cOPs );
  }
}

void KScriptableBlock::InitVars()
{
  for( DWORD n=0; n<m_scriptParams.cVars; ++n )
  {
    KCalcVar &var = m_pVars[n];
    if( KCalcVar::Reference==var.Type )
    {
      ::eVarType type = evtHZ;
      var.pMem = GetField( var.szName, &type );
      var.type = type;
    }
    else if( KCalcVar::Const==var.Type )
    {
      bool r = var.Numb();
      if( !r )
      {
        m_nInited = -1;
        break;
      }
      continue;
    }
    else
    {
      ASSD(0);
    }
    ASSD( var.pMem );
    if( !var.pMem )
    {
      m_nInited = -1;
      break;
    }
  }
}
