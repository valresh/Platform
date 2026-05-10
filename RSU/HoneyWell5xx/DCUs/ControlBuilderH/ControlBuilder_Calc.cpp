#include <rsuErr.h>
#include "ControlBuilder.h"
#include "CBPool.h"
#include <chrono>

extern KCBPool g_Pool;
extern int g_All2MomState;
void KControlBuilder::EnableMomStateDEVCTL()
{
  g_All2MomState = g_Pool.GetCtrlCount();
}

enum
{
  eFirstStepsTest = 50,
};

int KControlBuilder::StepTL(double dt_sec)
{
  if( !m_entryModule )
    return -1;

  std::chrono::high_resolution_clock::time_point t;
  if( m_nFirstSteps < eFirstStepsTest )
      t = std::chrono::high_resolution_clock::now();

  SStepCalcParams parm;
  parm = dt_sec;
  parm.usePhases = m_usePhases;
  m_entryModule->StepT( parm );
  if( g_All2MomState>0 )
    --g_All2MomState;

  if( m_nFirstSteps < eFirstStepsTest )
  {
    auto duration = std::chrono::high_resolution_clock::now() - t;
    char szDebug[128*4];
    sprintf_s( szDebug, "время шага №% d CB (%s) %lldms, при dt_sec=%0.5f ms\n", m_nFirstSteps, m_szPrjName, std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(), dt_sec );
    OutputDebugString( szDebug );
    ++m_nFirstSteps;
  }
  return 0;
}
