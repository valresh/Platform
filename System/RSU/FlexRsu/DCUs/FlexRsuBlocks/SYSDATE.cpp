#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <rsu_time.h>

class K_SYSDATE : public KFlexBlockBase
{
  S_SYSDATE *W;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_SYSDATE()
  {
    m_nullOrderExec = exnInputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  SReturn StepCalc( const SCallParams &params );
  static void InitParm();
};

static KBlockCreator _SYSDATE("SYSDATE", K_SYSDATE::Create);

KFlexBlockBase *K_SYSDATE::Create()
{
  return new K_SYSDATE;
}

namespace ns_SYSDATE
{
#include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_SYSDATE, 5 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_SYSDATE;
void K_SYSDATE::InitParm()
{
#include <FlexBlocks/SYSDATE.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_SYSDATE::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_SYSDATE::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[0] )
    {
    case 'D':
      (*pOut) << W->DAY;
      break;
    case 'Y':
      (*pOut) << W->YEAR;
      break;
    case 'M':
      (*pOut) << W->MONTH;
      break;
    default:
      ASSD(!"такого выхода нет");
      break;
    }
  }
}

KFlexBlockBase::SReturn K_SYSDATE::StepCalc( const SCallParams &params )
{
  SReturn sr;

  tm TM;
  bool ret = rsu_localtime( &TM, params.modelTime ); 
  W->YEAR = TM.tm_year+1900;
  W->MONTH = TM.tm_mon+1;
  W->DAY = TM.tm_mday;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
