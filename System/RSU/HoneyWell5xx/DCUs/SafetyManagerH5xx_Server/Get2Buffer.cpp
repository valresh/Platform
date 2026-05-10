#include "Get2Buffer.h"
#include <macros/AutoCloser.h>
#include <rsuErr.h>
#include <UniBuffer.h>

void* Get2Buffer( LPCSTR pszFile, DWORD &size )
{
  size = 0;
  std::ifstream file(pszFile, std::ios::binary | std::ios::ate);
  if (!file.is_open())
  {
      return nullptr;
  }
  size = file.tellg();
  file.seekg(0, std::ios::beg);

  char* pData = new char[size];
  if (!file.read(pData, size))
  {
      ASSD(false);
      delete(pData);
      return nullptr;
  }
  file.close();
  return pData;
}

void ClearBuffer( void *pMem )
{
  delete pMem;
}

BOOL GetAttr( const char* name, FILETIME& attr  )
{
    std::error_code errorCode;
    attr = ToFILETIME(std::filesystem::last_write_time(name, errorCode));
  return !errorCode;
}
