#include <BlockCreator.h>
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_TCOREAO : public KFlexBlockBase
{
  S_TCOREAO *W;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  K_TCOREAO()
  {
    m_nullOrderExec = exnOutputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _AO("TCoreAO", K_TCOREAO::Create);

KFlexBlockBase *K_TCOREAO::Create()
{
  return new K_TCOREAO;
}

#include <_InitInfo.h>
DECLARE_SHARED_VARSINFO( S_TCOREAO, 7 )
//////////////////////////////////////////////////////////////////////////
void K_TCOREAO::InitParm()
{
#include <FlexBlocks/TCoreAIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TCOREAO::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_TCOREAO::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( pIn->m_szSink=="IN" )
      (*pIn) >> W->PV;
  }
}

KFlexBlockBase::SReturn K_TCOREAO::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  return sr;
}
