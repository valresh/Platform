#pragma once
#include <PipeClient.h>

#ifdef FLEXMNEMOEXTEND_EXPORTS
#define FLEXMNEMOEXTEND_API _EXP
#else
#define FLEXMNEMOEXTEND_API _IMP
#endif


class FLEXMNEMOEXTEND_API KFlexPipeClient : public CPipeClient
{
  int m_LocalSystem;
public:
  KFlexPipeClient( LPCSTR pszDebugName, KTcpTransport *pTcpTransport = NULL );
  virtual ~KFlexPipeClient();

  static CShBase* GetShRs(UINT eType);
  int AddLocalSystem();
  struct CShFlexSystem* GetLocalSystem();
  static void SetConfigId(int id);
public:
	static int m_nConfigId;
};