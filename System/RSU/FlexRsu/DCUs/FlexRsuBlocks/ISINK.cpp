#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_ISINK_P
{
#include "PrivateVars/ISINK.h"
};


class K_ISINK 
  : public KFlexBlockBase
  , protected S_ISINK_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;

  SPageConnector* m_pPageCon;
  bool bInitedPC;
public:
  K_ISINK()
  {
    m_nullOrderExec = exnInputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual void OnProjectReady( IProjectContainer *pCont );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _ISINK("PageConnector:ISINK", K_ISINK::Create);

KFlexBlockBase *K_ISINK::Create()
{
  return new K_ISINK;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_ISINK, 4 )
//////////////////////////////////////////////////////////////////////////
void K_ISINK::InitParm()
{
#include "PrivateVars/ISINK.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_ISINK::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
}

void K_ISINK::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
}

void K_ISINK::OnProjectReady( IProjectContainer *pCont )
{
  m_pPageCon = pCont->RegisterPageConnector( REF );
}

KFlexBlockBase::SReturn K_ISINK::StepCalc( SCallParams &params )
{
  if( !bInitedPC )
  {
    bInitedPC = true;
    if( m_pPageCon )
    {
      for( DWORD i=0; i<m_nOuts; ++i )
      {
        KFlexConnection *pOut = m_ppOut[i];
        switch( m_pPageCon->typeSrcData )
        {
        case evtAnalog32:
          (*pOut) << m_pPageCon->flt32Val;
          break;
        case evtDigital32:
          (*pOut) << m_pPageCon->dig32Val;
          break;
        }
      }
    }
  }

  SReturn sr;
  if( m_pPageCon && m_pPageCon->flagSrcValid )
  {
    for( DWORD i=0; i<m_nOuts; ++i )
    {
      KFlexConnection *pOut = m_ppOut[i];
      pOut->DataReady2Send();
    }
  }
  return sr;
}
