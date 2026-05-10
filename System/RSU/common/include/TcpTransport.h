#pragma once
#include "INetworkTransport.h"

class CLIENTDLL_API KTcpTransport : public INetworkTransport
{
public:
  enum
  {
    defListenPort = 3131,
  };
public:
  KTcpTransport();
  //UINT_PTR GetSocket() { return m_hSocket; }
protected:
  virtual bool IsConnected();
  virtual bool CreateTransport( LPCSTR pszServer, LPCSTR pszPointName, USHORT port );
  virtual bool CloseTransport();
  virtual bool ReadData( UINT dwSize, LPVOID pReply, LPDWORD lpNumberOfBytesRead );
  virtual bool WriteData(LPVOID req, UINT dwReqSize, LPDWORD lpNumberOfBytesWritten );
private:
  UINT_PTR m_hSocket;
  CRITICAL_SECTION c_cCritSect;
  int m_nPort;
};

