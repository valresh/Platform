#include <BlockCreator.h>
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_TCOREDO : public KFlexBlockBase
{
  S_TCOREDO *W;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  K_TCOREDO()
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

static KBlockCreator _DO("TCoreDO", K_TCOREDO::Create);

KFlexBlockBase *K_TCOREDO::Create()
{
  return new K_TCOREDO;
}

#include <_InitInfo.h>
DECLARE_SHARED_VARSINFO( S_TCOREDO, 7 )
//////////////////////////////////////////////////////////////////////////
void K_TCOREDO::InitParm()
{
#include <FlexBlocks/TCoreDIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TCOREDO::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_TCOREDO::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( pIn->m_szSink=="IN" )
      (*pIn) >> W->PV;
  }
}

KFlexBlockBase::SReturn K_TCOREDO::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  return sr;
}
