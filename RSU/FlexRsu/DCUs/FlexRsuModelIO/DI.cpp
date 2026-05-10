#include <cstddef>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_DI : public KFlexBlockBase
{
  S_DIO *W;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_DI()
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

static KBlockCreator _DI("DI", K_DI::Create);

KFlexBlockBase *K_DI::Create()
{
  return new K_DI;
}

namespace ns_DI
{
  #include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_DIO, 2 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_DI;
void K_DI::InitParm()
{
#include <FlexBlocks/DIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_DI::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_DI::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << W->PV;
  }
}

KFlexBlockBase::SReturn K_DI::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
