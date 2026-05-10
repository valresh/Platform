#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>
#include <RsuX.h>

struct S_RsuAO_P
{
#include "PrivateVars/RsuAO.h"
};


class K_RsuAO 
  : public KFlexBlockBase
  , protected S_RsuAO_P
{
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  KRsuX m_pnt;
  bool bNeedConnect;
public:
  K_RsuAO()
  {
    m_nullOrderExec = exnOutputsIO;
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

static KBlockCreator _RsuAO("RsuAO", K_RsuAO::Create);

KFlexBlockBase *K_RsuAO::Create()
{
  return new K_RsuAO;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_RsuAO, 4 )
//////////////////////////////////////////////////////////////////////////
void K_RsuAO::InitParm()
{
#include "PrivateVars/RsuAO.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_RsuAO::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
}

void K_RsuAO::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( !strcmp(pIn->m_szSink, "IN") )
      (*pIn) >> in;
  }
}

KFlexBlockBase::SReturn K_RsuAO::StepCalc( SCallParams &params )
{
  SReturn sr;
  if( bNeedConnect )
  {
    bNeedConnect = false;
    m_pnt.LinkTo( m_szVisibleName, "AI", ID );
    if( m_pnt.IsConnected() )
      strcpy_s( Message, "Точки соединились");
    else
      strcpy_s( Message, "Не соединиться");
  }
  if( !m_pnt.IsConnected() )
  {
    sr.r = 1;
    return sr;
  }
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( pIn->GetData() )
      m_pnt = in;
  }
  return sr;
}
