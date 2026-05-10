#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_OPCAO : public KFlexBlockBase
{
  S_OPCAO *W;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  K_OPCAO()
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

static KBlockCreator _AO("OPCAO", K_OPCAO::Create);

KFlexBlockBase *K_OPCAO::Create()
{
  return new K_OPCAO;
}

namespace ns_OPCAO
{
#include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_OPCAO, 2 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_OPCAO;
void K_OPCAO::InitParm()
{
#include <FlexBlocks/OpcAIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_OPCAO::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_OPCAO::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> W->PV;
  }
}

KFlexBlockBase::SReturn K_OPCAO::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  return sr;
}
