#include "H_Server.h"
//#include <io.h>
#include <CommonRSUs.h>
#include "StateSerHoneyLocal.h"
#include <rsuErr.h>
#include <map>
#include <string>
#include <crossstring.h>

static SHParam s_params[19999];

enum EKeys
{
  eKeyNULL     = 0,
  kKeyVersion    = 0x9abc6def,
};

static const DWORD s_parmVer = 1;

int KServer::ParamsSave( LPCSTR pszPath )
{
    fs::remove(pszPath);
  //DeleteFile( pszPath );
    fs::path path(pszPath);
    std::string filenameinzip = path.filename().string();
    std::string path_tmp(pszPath);
    path_tmp += ".tmp";

  KStateSerHoneyLocal saver( path_tmp.c_str(), true);
  IStateSer* pSaver = saver.GetInterface();

  ZeroMemory( s_params, sizeof(s_params) );
  KHBridge2SysParam paramCont( s_params, _countof(s_params) );
  int r = KControlBuilder::GetParams( paramCont );

  saver.Write( kKeyVersion );
  saver.Write( s_parmVer );
  saver.Write( paramCont.Count() );

  for( int i=0, C(paramCont.Count()); i<C; ++i )
  {
    SHParam &src = s_params[i];
    saver.Write( src.ParamName );
    saver.Write( src.Type );
    saver.Write( src.Size );
    pSaver->WriteImpl( src.pAddr, src.Size );
    KKK();
  }

  saver.Close();
  RsuPackager( path_tmp.c_str(), pszPath, filenameinzip.c_str());

  fs::remove(path_tmp);

  return 0;
}

typedef std::map<std::string, int> tLocalSortObjCont_;

int KServer::ParamsRestore( LPCSTR pszPath )
{
    std::string path(pszPath);
    if (!fs::exists(path))
    {
        path[path.length()-1] = 0; // Пробуем старый формат param без x
        if (!fs::exists(path))
            return 1;
    }

  ZeroMemory( s_params, sizeof(s_params) );
  KHBridge2SysParam paramCont( s_params, _countof(s_params) );
  int r = KControlBuilder::GetParams( paramCont );

  tLocalSortObjCont_ objs;

  for( int i=0, C(paramCont.Count()); i<C; ++i )
  {
    SHParam &src = s_params[i];
    bool add = objs.insert( tLocalSortObjCont_::value_type(src.ParamName, i) ).second;
    ASS( add );
  }

  bool convert = path[path.length()-1] != 'x';
  KStateSerHoneyLocal saver( RsuExtractor(path.c_str()), false, convert );
  IStateSer* pParamSer = saver.GetInterface();

  EKeys key = eKeyNULL;
  DWORD ver = 0;

  pParamSer->Read( key );
  ASS( kKeyVersion==key );
  pParamSer->Read( ver );
  ASS( s_parmVer==ver );
  int count= 0;
  pParamSer->Read( count );

  char parmName[128*4];
  BYTE type = 0;
  int size = 0;
  for( int i=0; i<count; ++i )
  {
    pParamSer->Read( parmName, _countof(parmName) );
    if (pParamSer->NeedConvertUTF())
        cross::string::convert_to_utf8(1251, parmName, parmName, sizeof(parmName));
    pParamSer->Read( type );
    pParamSer->Read( size );
    tLocalSortObjCont_::iterator itO = objs.find( parmName );
    if( objs.end()==itO )
    {
      ASS( size<_countof(parmName) );
      pParamSer->Read( size, parmName );
      if (pParamSer->NeedConvertUTF())
          cross::string::convert_to_utf8(1251, parmName, parmName, sizeof(parmName));
      continue;
    }
    int index = itO->second;
    ASS( index < paramCont.Count() );
    SHParam &src = s_params[index];
    ASS( (char)type==src.Type );
    ASS( size==src.Size );
    pParamSer->Read( size, src.pAddr );
  }


  return 0;
}
