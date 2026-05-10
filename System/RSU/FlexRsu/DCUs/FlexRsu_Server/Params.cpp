#include "Server.h"

int KServer::ParamsSave( LPCSTR pszPath )
{
  return KFlexRsu::SaveParamToDRs();
}

int KServer::ParamsRestore( LPCSTR pszPath )
{
  return 0;
}
