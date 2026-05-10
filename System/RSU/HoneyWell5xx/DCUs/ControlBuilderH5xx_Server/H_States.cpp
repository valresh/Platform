#include "H_Server.h"
#include <CommonRSUs.h>
#ifdef _WIN32
#include <io.h>
#endif
#include "StateSerHoneyLocal.h"

int KServer::StateSave( LPCSTR pszPath )
{
  fs::remove(pszPath);

  fs::path path(pszPath);
  std::string filenameinzip = path.filename().string();
  std::string path_tmp(pszPath);
  path_tmp += ".tmp";

  KStateSerHoneyLocal saver( path_tmp.c_str(), true);

  KControlBuilder::SaveState( saver.GetInterface() );

  saver.Close();
  RsuPackager( path_tmp.c_str(), pszPath, filenameinzip.c_str());

  fs::remove( path_tmp );

  return 0;
}

int KServer::StateRestore( LPCSTR pszPath )
{
  if( 0!= _access( pszPath, 0) )
    return 1;

  bool convert = pszPath[strlen(pszPath)-1] != 'x';
  KStateSerHoneyLocal rest( RsuExtractor(pszPath), false, convert );
  
  return KControlBuilder::RestoreState( rest.GetInterface() );
}
