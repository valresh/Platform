#pragma once
#include "crossplatform.h"

#undef CLIENTDLL_API
#if defined(_WIN32)
    #ifdef QUEUEAPM_EXPORTS
        #define CLIENTDLL_API __declspec(dllexport)
    #else
        #define CLIENTDLL_API __declspec(dllimport)
    #endif
#else
    #ifdef UTILS_EXPORTS
        #define CLIENTDLL_API __attribute((visibility("default")))
    #else
        #define CLIENTDLL_API
    #endif
#endif

struct CLIENTDLL_API INetworkTransport
{
  virtual bool IsConnected() = 0;
  virtual bool CreateTransport( LPCSTR pszServer, LPCSTR pszPointName, USHORT port ) = 0;
  virtual bool CloseTransport() = 0;
  virtual bool ReadData( UINT dwSize, LPVOID pReply, LPDWORD lpNumberOfBytesRead ) = 0;
  virtual bool WriteData(LPVOID req, UINT dwReqSize, LPDWORD lpNumberOfBytesWritten ) = 0;
};
