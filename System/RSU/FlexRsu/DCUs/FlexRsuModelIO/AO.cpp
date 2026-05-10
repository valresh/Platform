#include <cstddef>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_AO : public KFlexBlockBase
{
  S_AIO *W;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  K_AO()
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

static KBlockCreator _AO("AO", K_AO::Create);

KFlexBlockBase *K_AO::Create()
{
  return new K_AO;
}

namespace ns_AO
{
  #include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_AIO, 2 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_AO;
void K_AO::InitParm()
{
#include <FlexBlocks/AIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_AO::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_AO::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> W->PV;
  }
}

KFlexBlockBase::SReturn K_AO::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  return sr;
}
