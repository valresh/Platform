#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <TcpTransport.h>
#include "PipeClient.h"
#include "LogFile.h"

//#define RCV_BUFSIZE		1048576
#define RCV_BUFSIZE		32768
#define SND_BUFSIZE		32768

#define NEW_R_LOGIC
#define NEW_W_LOGIC

#define WINSOCK_DEPRECATED_NO_WARNINGS

KTcpTransport::KTcpTransport()
: m_hSocket( INVALID_SOCKET ), m_nPort (0)
{
}

bool KTcpTransport::CreateTransport( LPCSTR pszServer, LPCSTR pszPointName, USHORT port )
{
	// VM: For a shared socket don't create a new one
	if (IsConnected())
		return true;
	// ...
	DWORD dwThread = ::GetCurrentThreadId ();
	CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::CreateTransport_start_0: port %d, dwThread %08lx", port, dwThread);
	m_nPort = (int)port;
  LPCSTR pszServerName = pszServer;
  InitializeCriticalSection (&c_cCritSect);
  SOCKADDR_IN sockAddr;
  memset(&sockAddr,0, sizeof(sockAddr));
  //sockAddr.sin_addr.s_addr = inet_addr(pszServerName);
  inet_pton(AF_INET, pszServerName, &sockAddr.sin_addr.s_addr);
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_port = htons(port);

  if( INADDR_NONE==sockAddr.sin_addr.s_addr )
  {
    addrinfo aiHints;
    addrinfo *aiList = NULL;
    int retVal;
    memset(&aiHints, 0, sizeof(aiHints));
    aiHints.ai_family = AF_INET;
    aiHints.ai_socktype = SOCK_STREAM;
    aiHints.ai_protocol = IPPROTO_TCP;
    char szPort[20];
    sprintf_s( szPort, "%d", port );
	CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::CreateTransport_1: szPort %s, pszServerName %s", szPort, pszServerName);
    if( (retVal = getaddrinfo(pszServerName, szPort, &aiHints, &aiList)) != 0)
    {
		CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::CreateTransport_1: FAILURE");
      return false;
    }
    char host[256];
    getnameinfo(aiList->ai_addr, aiList->ai_addrlen, host, _countof(host), NULL, 0, NI_NUMERICHOST);
    freeaddrinfo(aiList);
 //   sockAddr.sin_addr.s_addr = inet_addr(host);
    inet_pton(AF_INET, pszServerName, &sockAddr.sin_addr.s_addr);
    if( INADDR_NONE==sockAddr.sin_addr.s_addr )
	{
		CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::CreateTransport_2: FAILURE");
      return false;
	}
  }

  m_hSocket = socket(AF_INET, SOCK_STREAM, 0 );
  if( INVALID_SOCKET==m_hSocket )
  {
	  CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::CreateTransport_3: FAILURE");
    return false;
  }

	int nBufSize = 0;
#ifdef NEW_R_LOGIC
	nBufSize = RCV_BUFSIZE;
	if ( setsockopt((SOCKET)m_hSocket, SOL_SOCKET, SO_RCVBUF, (const char *)&nBufSize, sizeof(int)) != 0 )
		CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::setsockopt SO_RCVBUF FAILURE");
#endif // NEW_R_LOGIC
#ifdef NEW_W_LOGIC
	nBufSize = SND_BUFSIZE;
	if ( setsockopt((SOCKET)m_hSocket, SOL_SOCKET, SO_SNDBUF, (const char *)&nBufSize, sizeof(int)) != 0 )
		CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::setsockopt SO_SNDBUF FAILURE");
#endif // NEW_W_LOGIC

  /*int V = 20000;
  setsockopt( m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&V, sizeof(V) );
  setsockopt( m_hSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&V, sizeof(V) );
  setsockopt( m_hSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&V, sizeof(V) );*/

  if(connect(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
  {
		int nError = WSAGetLastError();
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::CreateTransport_4: FAILURE (nError %d)", nError);
		return false;
  }
  CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::CreateTransport_5: SUCCESS");

  return true;
}

bool KTcpTransport::IsConnected()
{
  return m_hSocket != INVALID_SOCKET;
}

bool KTcpTransport::ReadData( UINT dwSize, LPVOID pReply, LPDWORD lpNumberOfBytesRead )
{
	if( lpNumberOfBytesRead )
		*lpNumberOfBytesRead = 0;
	if( !IsConnected() )
		return false;
	DWORD dwThread = ::GetCurrentThreadId ();
	bool bResult = true;
	//EnterCriticalSection (&c_cCritSect);
#ifdef NEW_R_LOGIC
	CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::ReadData_0: m_nPort = %d, dwSize = %d, dwThread = %08lx, m_hSocket = %16lx", m_nPort, dwSize, dwThread, m_hSocket);
	BYTE *pPtr = (BYTE *)pReply;
	int nTotalRead = 0, nCurSize = min (RCV_BUFSIZE, (int)dwSize);
	while ( nTotalRead < (int)dwSize )
	{
		int nCurRead = recv( m_hSocket, (char *)(pPtr + nTotalRead), nCurSize, 0 );
		if ( SOCKET_ERROR == nCurRead )
		{
			int nError = WSAGetLastError();
			CLogFile::LogEx (LOGLEVEL_HIGH, "KTcpTransport::ReadData: FAILURE 1: nError %d, m_nPort %d, dwSize %d", nError, m_nPort, dwSize);
			bResult = false;
			break;
		}

		if ( nCurSize != nCurRead )
		{
			CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::ReadData: FAILURE 2: nCurRead: %d, nCurSize %d, m_nPort %d, CPipeClient::ms_RemoteTcpPort %d", nCurRead, nCurSize, m_nPort, CPipeClient::ms_RemoteTcpPort);
			if ( m_nPort != CPipeClient::ms_RemoteTcpPort )
			{
				nTotalRead += nCurRead;
				break;
			}
		}

		nTotalRead += nCurRead;
		CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::ReadData_3: nCurRead: %d, nTotalRead %d", nCurRead, nTotalRead);

		if( lpNumberOfBytesRead )
			*lpNumberOfBytesRead += nCurRead;


		if ( nCurRead == 0 )
		{
			CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::ReadData: FAILURE 22: nCurRead: %d, nCurSize %d", nCurRead, nCurSize);
			break;
		}

		int nReminder = ((int)dwSize - nTotalRead);
		nCurSize = min (RCV_BUFSIZE, nReminder);
	}
	CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::ReadData_4 SUCCESS: nTotalRead %d", nTotalRead);
	if( lpNumberOfBytesRead )
		*lpNumberOfBytesRead = (DWORD)nTotalRead;
#else
	char *pBuf = (char*)pReply;
	int leftData = dwSize;
	while( leftData )
	{
		int r = recv( m_hSocket, pBuf, leftData, 0 );
		if( SOCKET_ERROR==r )
		{
			int nError = WSAGetLastError();
			CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::ReadData: FAILURE 11: nError %d", nError);
			bResult = false;
			break;
		}
		if( lpNumberOfBytesRead )
			*lpNumberOfBytesRead += r;
		pBuf += r;
		leftData -= r;
	}
#endif // NEW_R_LOGIC
	//LeaveCriticalSection (&c_cCritSect);
	return bResult;
}

bool KTcpTransport::WriteData(LPVOID req, UINT dwReqSize, LPDWORD lpNumberOfBytesWritten )
{
	if( lpNumberOfBytesWritten )
		*lpNumberOfBytesWritten = 0;
	if( !IsConnected() )
		return false;
	DWORD dwThread = ::GetCurrentThreadId ();
	bool bResult = true;
	//EnterCriticalSection (&c_cCritSect);
#ifdef NEW_W_LOGIC
	//if ( dwReqSize > SND_BUFSIZE )
		CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::WriteData: m_nPort %d, dwReqSize %d, dwThread %08lx, m_hSocket %16lx", m_nPort, dwReqSize, dwThread, m_hSocket);
	BYTE *pPtr = (BYTE *)req;
	int nTotalWritten = 0, nCurSize = min (SND_BUFSIZE, dwReqSize);
	while ( nTotalWritten < (int)dwReqSize )
	{
		int nCurWritten = send( m_hSocket, (char*)(pPtr + nTotalWritten), nCurSize, 0 );
		if ( SOCKET_ERROR == nCurWritten )
		{
			int nError = WSAGetLastError();
			CLogFile::LogEx (LOGLEVEL_HIGH, "KTcpTransport::WriteData: FAILURE nError %d, m_nPort %d, dwReqSize %d", nError, m_nPort, dwReqSize);
			bResult = false;
			break;
		}
		//pPtr += nCurSize;
		nTotalWritten += nCurWritten;
		int nReminder = ((int)dwReqSize - nTotalWritten);
		nCurSize = min (SND_BUFSIZE, nReminder);
	}
	if( lpNumberOfBytesWritten )
		*lpNumberOfBytesWritten = (DWORD)nTotalWritten;
	CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::WriteData SUCCESS: nTotalWritten %d", nTotalWritten);
#else
	int s = send( m_hSocket, (char*)req, dwReqSize, 0 );
	if( SOCKET_ERROR==s )
	{
		int nError = WSAGetLastError();
		CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::ReadData: FAILURE 11: nError %d", nError);
		//LeaveCriticalSection (&c_cCritSect);
		return false;
	}
	ASSD( s==dwReqSize );
	if( lpNumberOfBytesWritten )
		*lpNumberOfBytesWritten = s;
	CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::WriteData SUCCESS: s %d", s);
#endif // NEW_W_LOGIC
	//LeaveCriticalSection (&c_cCritSect);
	return bResult;
}

bool KTcpTransport::CloseTransport()
{
	CLogFile::LogEx (LOGLEVEL_LOW, "KTcpTransport::CloseTransport !!! m_nPort %d, m_hSocket %08lx", m_nPort, m_hSocket);
	// VM: Sending 'closing socket' message for the server to remove socket thread
	DWORD NumberOfBytesWritten = 0;
	DWORD dwCloseSize = 0xFFFFFFFF;
	WriteData(&dwCloseSize, 4, &NumberOfBytesWritten );
	// ...
	Sleep(100);
	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
	return true;
}
