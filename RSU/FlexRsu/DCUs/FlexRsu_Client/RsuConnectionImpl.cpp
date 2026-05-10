#include <RsuX.h>
#include <rsuNoNames.h>
#include <macros/StrHelps.h>
#include <SharedFlexBlocks.h>
#include <rsuErr.h>
#include <rsuStr.h>

extern KNoName* g_SharedMemory;

using namespace nRSUx;

SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix )
{
  SParamInfo info;
  CBase *pBase = NULL;
  LPCSTR pszName = NULL;
  int nFcsNumb = 0;
  LPSTR p = NULL;
  while( 1 )
  {
    TStringTerminator st( p );
    DWORD ID = 0;
    while( g_SharedMemory->WhileBase( ID, -1, &pBase, &pszName, &nFcsNumb ) )
    {
      if( _Kstricmp(pszPointName, pszName) )
      {
        pBase = NULL;
        continue;
      }
      break;
    }
    if( pBase )
    {
      if( p )
        ++p;
      break;
    }
    p = strrchr( (LPSTR)pszPointName, '.');
    if( !p )
      break;
  }
  if( !pBase )
    return info;

  SFlex* pB = (SFlex*)pBase;
  
  info.pMem = pB->ObjectMem;
  info.size = pB->ObjSize;
  info.bPriorityOnCBase = 1;

  SVarInfo* pV = GetFieldInfo( pB->TypeFlex, p ? p : "PV" );
  if( pV )
  {
    info.pMem += pV->shift;
    info.size = pV->size;
    switch( pV->evtType )
    {
    case evtDigital32:
      info.type = e_int;
      break;
    case evtAnalog32:
      info.type = e_float;
      break;
    case evtChars:
      info.type = e_chars;
      break;
    }
  }

  return info;
}
