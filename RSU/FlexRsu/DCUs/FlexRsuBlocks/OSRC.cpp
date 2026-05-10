#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_OSRC_P
{
#include "PrivateVars/OSRC.h"
};


class K_OSRC 
  : public KFlexBlockBase
  , protected S_OSRC_P
{
  KFlexConnection **m_ppIn;
  DWORD m_nIns;

  SPageConnector* m_pPageCon;
public:
  K_OSRC()
  {
    m_nullOrderExec = exnOutputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual void OnProjectReady( IProjectContainer *pCont );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _OSRC("PageConnector:OSRC", K_OSRC::Create);

KFlexBlockBase *K_OSRC::Create()
{
  return new K_OSRC;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_OSRC, 4 )
//////////////////////////////////////////////////////////////////////////
void K_OSRC::InitParm()
{
#include "PrivateVars/OSRC.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_OSRC::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
}

void K_OSRC::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
}

void K_OSRC::OnProjectReady( IProjectContainer *pCont )
{
  m_pPageCon = pCont->RegisterPageConnector( REF );
  if( !m_pPageCon )
    return;
  m_pPageCon->flagSrcValid = 0;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    m_pPageCon->typeSrcData = pIn->GetSourceType();
    switch( m_pPageCon->typeSrcData )
    {
    case evtAnalog32:
      (*pIn) >> m_pPageCon->flt32Val;
      break;
    case evtDigital32:
      (*pIn) >> m_pPageCon->dig32Val;
      break;
    }
  }
}

KFlexBlockBase::SReturn K_OSRC::StepCalc( SCallParams &params )
{
  SReturn sr;
  if( m_pPageCon )
  {
    for( DWORD i=0; i<m_nIns; ++i )
    {
      KFlexConnection *pIn = m_ppIn[i];
      pIn->GetData();
    }
    m_pPageCon->flagSrcValid = 1;
  }
  return sr;
}
