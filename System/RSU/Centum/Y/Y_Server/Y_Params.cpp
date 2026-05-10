#include "stdafx.h"
#include "Y_Server.h"
#include <io.h>
#include "StateSer.h"
#include <yMagicKeys.h>
#include <macros/saverHelps.h>
#include <map>
#include <string>
#include <CommonRSUs.h>

static SYParam s_params[9999];

const DWORD KServer::s_dwParmVersion = 0x03000001;

int KServer::ParamsSave( LPCSTR pszPath )
{
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
  IYokoStateSer* pParamSer = saver.GetInterface();

  pParamSer->SimpleWrite( eKeyMagicPR );
  pParamSer->SimpleWrite( s_dwParmVersion );

  pParamSer->SimpleWrite( eKeyObjsCount );
  DWORD posObjCount = pParamSer->GetPosition();
  pParamSer->SimpleWrite( kY_Obj );

  int realCount = 0;
  for( int n=0; n<kY_Obj; ++n )
  {
    CY_Base * pY = pY_Obj[n];

    ZeroMemory( s_params, sizeof(s_params) );
    KYBridge2SysParam paramCont( s_params, _countof(s_params) );
    int r = pY->GetParams( paramCont );
    DWORD nParm2Save = 0;
    for( int n=0, Count(paramCont.Count()); n<Count; ++n )
    {
      SYParam &src = s_params[n];
      ASSD( src.pszName );
      if( '#'==src.pszName[0] )
        continue;
      switch( src.Type )
      {
      case 'T':
        continue;
      case 'D':
      case 'W':
      case 'S':
      case 'I':
      case 'B':
        break;
      default:
        ASSD(!"проверить тип")
      }
      KKK();
      ++nParm2Save;
    }

    if( !nParm2Save )
      continue;

    ++realCount;

    pParamSer->SimpleWrite( eKeyObject );
    pParamSer->WriteStr( pY->szAkaObjName );
    pParamSer->WriteStr( pY->Model );
    DWORD posStart = pParamSer->GetPosition();
    DWORD l = pParamSer->SimpleWrite( posStart );

    pParamSer->SimpleWrite( eKeyVarCountOfObj );
    pParamSer->SimpleWrite( nParm2Save );
    for( int n=0, Count(paramCont.Count()); n<Count; ++n )
    {
      SYParam &src = s_params[n];
      ASSD( src.pszName );
      if( '#'==src.pszName[0] )
        continue;
      switch( src.Type )
      {
      case 'T':
        continue;
      }
      pParamSer->WriteStr( src.pszName );
      pParamSer->SimpleWrite( src.Type );
      pParamSer->SimpleWrite( src.Size );
      pParamSer->SimpleWrite( src.pAddr, src.Size );
    }

    DWORD posEnd = pParamSer->GetPosition();
    DWORD length = posEnd - posStart - l;
    pParamSer->WriteAtPosition( posStart, length );
  }

  pParamSer->WriteAtPosition( posObjCount, realCount );

  saver.Close();
  RsuPackager( path_tmp, pszPath, filenameinzip );

  return 0;
}

typedef std::map<std::string, CY_Base*> tLocalSortObjCont_;
int KServer::ParamsRestore( LPCSTR pszPath )
{
  if( 0!=_access( pszPath, 0) )
    return 1;
  KStateSer saver( RsuExtractor(pszPath), false );
  IYokoStateSer* pParamSer = saver.GetInterface();

  EStateKeys key = sKeyNull;
  DWORD ver = 0;
  if( !ReadWithTest( pParamSer, key, eKeyMagicPR ) )
    return 2;
  ReadWithTest( pParamSer, ver, s_dwParmVersion );

  int savedYobjs = 0;
  if( !ReadWithTest( pParamSer, key, eKeyObjsCount ) )
    return 4;
  Read( pParamSer, savedYobjs );

  tLocalSortObjCont_ objs;
  for( int n=0; n<kY_Obj; ++n )
  {
    CY_Base * pY = pY_Obj[n];
    bool add = objs.insert( tLocalSortObjCont_::value_type(pY->szAkaObjName, pY) ).second;
    ASS( add );
  }

  char szAkaObjName[128];
  char szModel[16];
  DWORD lengthSaved = 0;
  int nLastReadOk = -1;
  for( int s=0; s<savedYobjs; ++s )
  {
    if( !ReadWithTest( pParamSer, key, eKeyObject ) )
    {
      if( !pParamSer->IsData() )
        return 0;
      return 5;
    }
    pParamSer->ReadLenStr( szAkaObjName, _countof(szAkaObjName) );
    pParamSer->ReadLenStr( szModel, _countof(szModel) );
    Read( pParamSer, lengthSaved );
    tLocalSortObjCont_::iterator itO = objs.find( szAkaObjName );
    if( objs.end()==itO )
    {
      pParamSer->Shift( lengthSaved );
      continue;
    }
    CY_Base * pY = itO->second;
    ZeroMemory( s_params, sizeof(s_params) );
    KYBridge2SysParam paramCont( s_params, _countof(s_params) );
    int r = pY->GetParams( paramCont );

    DWORD nParm2Save = 0;
    if( !ReadWithTest( pParamSer, key, eKeyVarCountOfObj ) )
      return 10;
    Read( pParamSer, nParm2Save );
    for( DWORD p=0; p<nParm2Save; ++p )
    {
      char type;
      int size;
      pParamSer->ReadLenStr( szAkaObjName, _countof(szAkaObjName) );
      Read( pParamSer, type );
      Read( pParamSer, size );
      bool bRead = false;
      for( int n=0, Count(paramCont.Count()); n<Count; ++n )
      {
        SYParam &src = s_params[n];
        ASSD( src.pszName );
        if( '#'==src.pszName[0] )
          continue;
        if( strcmp(szAkaObjName, src.pszName) )
          continue;
        ASS( type==src.Type );
        ASS( size==src.Size );
        pParamSer->SimpleRead( src.pAddr, size );
        bRead = true;
        break;
      }
      if( bRead )
        continue;
      pParamSer->Shift( size );
    }
    nLastReadOk = s;
  }

  return 0;
}
