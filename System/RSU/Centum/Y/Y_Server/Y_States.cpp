#include "stdafx.h"
#include "Y_Server.h"
#include <io.h>
#include <macros/AutoRestore.h>
#include "StateSer.h"
#include <map>
#include <string>
#include <yMagicKeys.h>
#include <macros/saverHelps.h>
#include <rsuNoNames2D.h>
#include <ConnectY.h>
#include <CommonRSUs.h>
#include <rsuProjectVersion.h>

static const DWORD s_StateVersionWithoutDomain = 0x03000001;
const DWORD KServer::s_dwStateVersion = 0x03000002;

extern KNoName2D *g_yokoIOs;

int KServer::StateSave( LPCSTR pszPath )
{
  ChangedParamsSave( pszPath );

  DeleteFile( pszPath );

  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];
  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];
  char path_tmp[_MAX_PATH] = { 0 };
  char filenameinzip[_MAX_FNAME] = { 0 };

  _splitpath_s( pszPath, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext) );
  _makepath_s( filenameinzip, NULL, NULL, fname, ext );
  strcpy_s( path_tmp, pszPath );
  strcat_s( path_tmp, ".tmp" );

  KStateSer saver( path_tmp, true );
  TAutoRestore<IYokoStateSer*> arSr(pYokoStateSer);
  pYokoStateSer = saver.GetInterface();

  pYokoStateSer->SimpleWrite( eKeyMagicST );
  pYokoStateSer->SimpleWrite( s_dwStateVersion );

  pYokoStateSer->SimpleWrite( eKeyObjsCount );
  pYokoStateSer->SimpleWrite( kY_Obj );

  for( int n=0; n<kY_Obj; ++n )
  {
    pYokoStateSer->SimpleWrite( eKeyObject );
    CY_Base * pY = pY_Obj[n];
    pYokoStateSer->WriteStr( pY->szAkaObjName );
    pYokoStateSer->WriteStr( pY->Model );
    DWORD posStart = pYokoStateSer->GetPosition();
    DWORD l = pYokoStateSer->SimpleWrite( posStart );

    pY->StateSave( pYokoStateSer );

    DWORD posEnd = pYokoStateSer->GetPosition();
    DWORD length = posEnd - posStart - l;
    pYokoStateSer->WriteAtPosition( posStart, length );
  }

  DWORD id = 0;
  CBase* pBase;
  int nIOsCount = 0;
  LPCSTR pszName = NULL;
  while( g_yokoIOs->WhileBase( id, -1, &pBase, &pszName, NULL, NULL ) )
  {
    switch(pBase->ID_CLASS)
    {
    case id__Y:
    case id__Z:
    case id__AN:
    case id__GS:
    case id__OG:
    case id__PR:
    case id__RQ:
    case id__SW:
    case id__WW:
    case id__WB:
    case id__XW:
      break;
    default:
      continue;
    }
    ++nIOsCount;
  }
  pYokoStateSer->SimpleWrite( eKeyIOsCount );
  pYokoStateSer->SimpleWrite( nIOsCount );

  ASS( pYokoStructSaveState );
  int nFcsNumber = 0, domain = 0;
  id = 0;
  while( g_yokoIOs->WhileBase( id, -1, &pBase, &pszName, &nFcsNumber, &domain ) )
  {
    switch(pBase->ID_CLASS)
    {
    case id__Y:
    case id__Z:
    case id__AN:
    case id__GS:
    case id__OG:
    case id__PR:
    case id__RQ:
    case id__SW:
    case id__WW:
    case id__WB:
    case id__XW:
      break;
    default:
      continue;
    }
    pYokoStateSer->SimpleWrite( eKeyIOobj );
    pYokoStateSer->WriteStr( pszName );
    pYokoStateSer->SimpleWrite( pBase->ID_CLASS );
    pYokoStateSer->SimpleWrite( nFcsNumber );
    pYokoStateSer->SimpleWrite( domain );
    DWORD posStart = pYokoStateSer->GetPosition();
    DWORD l = pYokoStateSer->SimpleWrite( posStart );

    int res = pYokoStructSaveState( pBase );
    ASS( !res );

    DWORD posEnd = pYokoStateSer->GetPosition();
    DWORD length = posEnd - posStart - l;
    pYokoStateSer->WriteAtPosition( posStart, length );
  }

  saver.Close();
  RsuPackager( path_tmp, pszPath, filenameinzip );

  DeleteFile( path_tmp );
  
  return 0;
}

typedef std::map<std::string, CY_Base*> tLocalSortObjCont_;
int KServer::StateRestore( LPCSTR pszPath )
{
  ChangedParamsRestore( pszPath );
  if( 0!=_access( pszPath, 0) )
    return 1;
  KStateSer saver( RsuExtractor(pszPath), false );
  TAutoRestore<IYokoStateSer*> arSr(pYokoStateSer);
  pYokoStateSer = saver.GetInterface();

  EStateKeys key = sKeyNull;
  DWORD ver = 0;
  if( !ReadWithTest( pYokoStateSer, key, eKeyMagicST ) )
    return 2;
  pYokoStateSer->SimpleRead( ver );

  int savedYobjs = 0;
  if( !ReadWithTest( pYokoStateSer, key, eKeyObjsCount ) )
    return 4;
  Read( pYokoStateSer, savedYobjs );

  tLocalSortObjCont_ objs;
  for( int n=0; n<kY_Obj; ++n )
  {
    CY_Base * pY = pY_Obj[n];
    bool add = objs.insert( tLocalSortObjCont_::value_type(pY->szAkaObjName, pY) ).second;
    ASS( add );
  }

  bool b147Project = ns_rsu::EP60_PM==rsuProjectVersion();

  char szAkaObjName[128];
  char szModel[16];
  DWORD lengthSaved = 0;
  for( int s=0; s<savedYobjs; ++s )
  {
    if( !ReadWithTest( pYokoStateSer, key, eKeyObject ) )
      return 5;
    pYokoStateSer->ReadLenStr( szAkaObjName, _countof(szAkaObjName) );
    pYokoStateSer->ReadLenStr( szModel, _countof(szModel) );
    Read( pYokoStateSer, lengthSaved );
    if( b147Project )
    {
      if( !strncmp(szAkaObjName,"FCS0211", 7) || !strncmp(szAkaObjName,"FCS0212", 7) )
        szAkaObjName[4] = '1', szAkaObjName[5] = '0';
    }
    tLocalSortObjCont_::iterator itO = objs.find( szAkaObjName );
    if( objs.end()==itO )
    {
      pYokoStateSer->Shift( lengthSaved );
      continue;
    }
    CY_Base * pY = itO->second;
    if( strcmp(pY->Model, szModel) )
    {
      pYokoStateSer->Shift( lengthSaved );
      continue;
    }

    DWORD start = pYokoStateSer->GetPosition();
    int r = pY->StateRestore( pYokoStateSer );
    ASS( !r );
    DWORD end = pYokoStateSer->GetPosition();
    DWORD delta = end - start;
    if( delta!=lengthSaved )
    {
      if( delta < lengthSaved )
        pYokoStateSer->Shift( lengthSaved-delta );
      else
      {
        ASS(!"отработать");
      }
    }
  }

  ASS( pYokoStructRestoreState );

  int savedIOobjs = 0;
  if( !ReadWithTest( pYokoStateSer, key, eKeyIOsCount ) )
    return 6;
  Read( pYokoStateSer, savedIOobjs );

  int nFcsNumber = 0, domain = 0;
  DWORD idClass;
  for( int n=0; n<savedIOobjs; ++n )
  {
    if( !ReadWithTest( pYokoStateSer, key, eKeyIOobj ) )
      return 7;
    pYokoStateSer->ReadLenStr( szAkaObjName, _countof(szAkaObjName) );
    Read( pYokoStateSer, idClass );
    Read( pYokoStateSer, nFcsNumber );
    if( s_StateVersionWithoutDomain!=ver )
      Read( pYokoStateSer, domain );
    Read( pYokoStateSer, lengthSaved );

    CBase *pIO = g_yokoIOs->FindStruct( szAkaObjName, idClass, nFcsNumber, domain );
    if( b147Project )
    {
      if( 11==nFcsNumber )
        nFcsNumber = 1;
      if( 12==nFcsNumber )
        nFcsNumber = 2;
      if( !pIO )
        pIO = g_yokoIOs->FindStruct( szAkaObjName, idClass, nFcsNumber, 1 );
    }
    if( !pIO )
    {
      pYokoStateSer->Shift( lengthSaved );
      continue;
    }
    pYokoStructRestoreState( pIO, szAkaObjName );
  }

  return 0;
}
