#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>
#include <RsuX.h>

struct S_RsuDO_P
{
#include "PrivateVars/RsuDO.h"
};


class K_RsuDO 
  : public KFlexBlockBase
  , protected S_RsuDO_P
{
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  KRsuX m_pnt;
  bool bNeedConnect;
public:
  K_RsuDO()
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

static KBlockCreator _RsuDO("RsuDO", K_RsuDO::Create);

KFlexBlockBase *K_RsuDO::Create()
{
  return new K_RsuDO;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_RsuDO, 4 )
//////////////////////////////////////////////////////////////////////////
void K_RsuDO::InitParm()
{
#include "PrivateVars/RsuDO.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_RsuDO::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
}

void K_RsuDO::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( !strcmp(pIn->m_szSink, "IN") )
      (*pIn) >> in;
  }
}

KFlexBlockBase::SReturn K_RsuDO::StepCalc( SCallParams &params )
{
  SReturn sr;
  if( bNeedConnect )
  {
    bNeedConnect = false;
    m_pnt.LinkTo( m_szVisibleName, "DI", ID );
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
