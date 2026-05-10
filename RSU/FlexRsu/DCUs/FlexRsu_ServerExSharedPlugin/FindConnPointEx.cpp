#include "ServerExSharedPlugin_FlexRsu.h"
#include <shdata.h>
#include <rsuNoNames.h>
#include <RsuX.h>
#include <macros/StrHelps.h>
#include <SharedFlexBlocks.h>
#include "RsuF.h"

extern KNoName* g_SharedMemory;

CBase* _FindConnPoint( LPCSTR ObjName, DWORD TypeID, int number )
{
  CBase* pB = NULL;
  LPSTR p = NULL;
  while( 1 )
  {
    TStringTerminator st( p );
    DWORD ID = 0;
    pB = g_SharedMemory->FindStruct( ObjName, TypeID, number );
    if( pB )
    {
      if( p )
        ++p;
      break;
    }
    p = strrchr( (LPSTR)ObjName, '.');
    if( !p )
      break;
  }
  return pB;
}

extern "C" SERVEREXSHAREDPLUGIN_FLEXRSU_API CBase* FindConnPointEx( LPCSTR ObjName, DWORD TypeID )
{
  CBase* pB = _FindConnPoint( ObjName, TypeID, 0 );
  if( !pB )
    return NULL;
  return pB;
}

using namespace nRSUx;

SParamInfo RsuConnectParamInfoTypesImpl( LPCSTR pszPointName, DWORD TypeID )
{
  SParamInfo info;

  if( SFlex::TypeID!=TypeID )
    return info;

  SFlex* pB = NULL;
  LPSTR p = NULL;
  while( 1 )
  {
    TStringTerminator st( p );
    DWORD ID = 0;
    pB = (SFlex*)g_SharedMemory->FindStruct( pszPointName, TypeID, 0 );
    if( pB )
    {
      if( p )
        ++p;
      break;
    }
    p = strrchr( (LPSTR)pszPointName, '.');
    if( !p )
      break;
  }
  if( !pB )
    return info;

  SVarInfo* pV = GetFieldInfo( pB->TypeFlex, p ? p : "PV" );
  if( pV )
  {
    info.pMem = pB->ObjectMem;
    info.size = pB->ObjSize;
    info.bPriorityOnCBase = 1;

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

extern "C" SERVEREXSHAREDPLUGIN_FLEXRSU_API int GetRsuData( DWORD type, CShBase* pSB, KRsuX &pnt )
{
  if( !pnt.IsConnected() || type!=SFlex::TypeID )
    return CShBase::rcNO;

  CShFlex* This = (CShFlex*)pSB;
  ASS( This->m_nSize >= sizeof(*This) );

  KRsuF* rr = (KRsuF*)&pnt;

  nRSUx::EParamType typeMem = (nRSUx::EParamType)rr->GetType();
  switch( typeMem )
  {
  case e_int:
    This->mType = evtDigital32;
    This->Discret = pnt;
    break;
  case e_float:
    This->mType = evtAnalog32;
    This->Analog = pnt;
    break;
  case e_chars:
    This->mType = evtChars;
    This->dataSize = (tVarSizeType_)rr->GetLength();
    strcpy_s( This->String, This->dataSize, (LPCSTR)(*rr) );
    break;
  default:
    This->mType = evtHZ;
    return CShBase::rcNO;
  }

  This->RetCode =  CShBase::rcOK;

  return This->RetCode;
}
