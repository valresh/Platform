#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>
#include <RsuX.h>

struct S_RsuAI_P
{
#include "PrivateVars/RsuAI.h"
};


class K_RsuAI 
  : public KFlexBlockBase
  , protected S_RsuAI_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KRsuX m_pnt;
  bool bNeedConnect;
public:
  K_RsuAI()
  {
    m_nullOrderExec = exnInputsIO;
    bNeedConnect = true;
    strcpy_s( Message, "Ожидание подключения");
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _RsuAI("RsuAI", K_RsuAI::Create);

KFlexBlockBase *K_RsuAI::Create()
{
  return new K_RsuAI;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_RsuAI, 4 )
//////////////////////////////////////////////////////////////////////////
void K_RsuAI::InitParm()
{
#include "PrivateVars/RsuAI.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_RsuAI::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
}

void K_RsuAI::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << out;
  }
}

KFlexBlockBase::SReturn K_RsuAI::StepCalc( SCallParams &params )
{
  SReturn sr;
  if( bNeedConnect )
  {
    bNeedConnect = false;
    m_pnt.LinkTo( m_szVisibleName, "AO", ID );
    if( m_pnt.IsConnected() )
      strcpy_s( Message, "Точки соединились");
    else
      strcpy_s( Message, "Не соединиться");
  }
  if( !m_pnt.IsConnected() )
    sr.r = 1;
  else
    out = m_pnt;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
