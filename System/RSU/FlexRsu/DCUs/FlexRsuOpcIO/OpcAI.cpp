#include <BlockCreator.h>
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_OPCAI : public KFlexBlockBase
{
  S_OPCAI *W;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_OPCAI()
  {
    m_nullOrderExec = exnInputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _AI("OPCAI", K_OPCAI::Create);

KFlexBlockBase *K_OPCAI::Create()
{
  return new K_OPCAI;
}

namespace ns_AI
{
#include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_OPCAI, 2 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_AI;
void K_OPCAI::InitParm()
{
#include <FlexBlocks/OpcAIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_OPCAI::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_OPCAI::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << W->PV;
  }
}

KFlexBlockBase::SReturn K_OPCAI::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
