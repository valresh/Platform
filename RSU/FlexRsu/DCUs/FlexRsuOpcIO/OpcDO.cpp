#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_OPCDO : public KFlexBlockBase
{
  S_OPCDO *W;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  K_OPCDO()
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

static KBlockCreator _DO("OPCDO", K_OPCDO::Create);

KFlexBlockBase *K_OPCDO::Create()
{
  return new K_OPCDO;
}

namespace ns_OPCDO
{
#include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_OPCDO, 2 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_OPCDO;
void K_OPCDO::InitParm()
{
#include <FlexBlocks/OpcDIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_OPCDO::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_OPCDO::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> W->PV;
  }
}

KFlexBlockBase::SReturn K_OPCDO::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  return sr;
}
