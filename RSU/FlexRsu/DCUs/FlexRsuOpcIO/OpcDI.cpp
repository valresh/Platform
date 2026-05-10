#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_OPCDI : public KFlexBlockBase
{
  S_OPCDI *W;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_OPCDI()
  {
    m_nullOrderExec = exnInputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _DI("OPCDI", K_OPCDI::Create);

KFlexBlockBase *K_OPCDI::Create()
{
  return new K_OPCDI;
}

namespace ns_OPCDI
{
#include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_OPCDI, 2 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_OPCDI;
void K_OPCDI::InitParm()
{
#include <FlexBlocks/OpcDIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_OPCDI::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_OPCDI::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << W->PV;
  }
}

KFlexBlockBase::SReturn K_OPCDI::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
