#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <rsu_time.h>

class K_SYSTIME : public KFlexBlockBase
{
  S_SYSTIME *W;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_SYSTIME()
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

static KBlockCreator _SYSTIME("SYSTIME", K_SYSTIME::Create);

KFlexBlockBase *K_SYSTIME::Create()
{
  return new K_SYSTIME;
}

namespace ns_SYSTIME
{
#include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_SYSTIME, 5 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_SYSTIME;
void K_SYSTIME::InitParm()
{
#include <FlexBlocks/SYSTIME.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_SYSTIME::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_SYSTIME::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[0] )
    {
    case 'H':
      (*pOut) << W->HOURS;
      break;
    case 'M':
      (*pOut) << W->MINUTES;
      break;
    case 'S':
      (*pOut) << W->SECONDS;
      break;
    default:
      ASSD(!"такого выхода нет");
      break;
    }
  }
}

KFlexBlockBase::SReturn K_SYSTIME::StepCalc( SCallParams &params )
{
  SReturn sr;

  tm TM;
  rsu_localtime( &TM, params.modelTime );

  W->HOURS = TM.tm_hour;
  W->MINUTES = TM.tm_min;
  W->SECONDS = TM.tm_sec;


  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
