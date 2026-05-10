#include <cstddef>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_DO : public KFlexBlockBase
{
  S_DIO *W;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  K_DO()
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

static KBlockCreator _DO("DO", K_DO::Create);

KFlexBlockBase *K_DO::Create()
{
  return new K_DO;
}

namespace ns_DO
{
  #include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_DIO, 2 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_DO;
void K_DO::InitParm()
{
#include <FlexBlocks/DIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_DO::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_DO::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> W->PV;
  }
}

KFlexBlockBase::SReturn K_DO::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  return sr;
}
