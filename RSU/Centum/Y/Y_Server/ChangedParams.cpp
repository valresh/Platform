#include "Y_Server.h"
//#include <io.h>
#include <macros/AutoRestore.h>
#include "StateSer.h"
#include <map>
#include <string>
#include <yMagicKeys.h>
#include <macros/saverHelps.h>
#include <CommonRSUs.h>
#include <rsuProjectVersion.h>

static LPCSTR s_Suffix = "_chp";

void KServer::ChangedParamsSave( LPCSTR pszPath )
{
  ns_rsu::eVersion ver = rsuProjectVersion();
  if( ns_rsu::GOBKK_MSK!=ver )
    return;
  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];
  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];
  char path_tmp[_MAX_PATH] = { 0 };
  char filenameinzip[_MAX_FNAME] = { 0 };
  char path_chp[_MAX_PATH] = { 0 };

  _splitpath_s( pszPath, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext) );
  strcat_s( ext, s_Suffix );

  _makepath_s( path_chp, drive, dir, fname, ext );

  DeleteFile( path_chp );

  _makepath_s( filenameinzip, NULL, NULL, fname, ext );
  strcpy_s( path_tmp, path_chp );
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

    pY->ChangedParamsSave( pYokoStateSer );

    DWORD posEnd = pYokoStateSer->GetPosition();
    DWORD length = posEnd - posStart - l;
    pYokoStateSer->WriteAtPosition( posStart, length );
  }

  saver.Close();
  RsuPackager( path_tmp, path_chp, filenameinzip );

  DeleteFile( path_tmp );
}

typedef std::map<std::string, CY_Base*> tLocalSortObjCont_;
void KServer::ChangedParamsRestore( LPCSTR pszPath )
{
  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];
  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];
  char path_tmp[_MAX_PATH] = { 0 };
  char filenameinzip[_MAX_FNAME] = { 0 };
  char path_chp[_MAX_PATH] = { 0 };

  _splitpath_s( pszPath, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext) );
  strcat_s( ext, s_Suffix );

  _makepath_s( path_chp, drive, dir, fname, ext );

  if( 0!=_access( path_chp, 0) )
    return;
  KStateSer saver( RsuExtractor(path_chp), false );
  TAutoRestore<IYokoStateSer*> arSr(pYokoStateSer);
  pYokoStateSer = saver.GetInterface();

  EStateKeys key = sKeyNull;
  DWORD ver = 0;
  if( !ReadWithTest( pYokoStateSer, key, eKeyMagicST ) )
    return;
  pYokoStateSer->SimpleRead( ver );

  int savedYobjs = 0;
  if( !ReadWithTest( pYokoStateSer, key, eKeyObjsCount ) )
    return;
  Read( pYokoStateSer, savedYobjs );

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
  for( int s=0; s<savedYobjs; ++s )
  {
    if( !ReadWithTest( pYokoStateSer, key, eKeyObject ) )
      return;
    pYokoStateSer->ReadLenStr( szAkaObjName, _countof(szAkaObjName) );
    pYokoStateSer->ReadLenStr( szModel, _countof(szModel) );
    Read( pYokoStateSer, lengthSaved );
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
    int r = pY->ChangedParamsRestore( pYokoStateSer );
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
}
