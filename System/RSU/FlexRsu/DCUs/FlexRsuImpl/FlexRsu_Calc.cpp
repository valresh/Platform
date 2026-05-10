#include <crossplatform.h>
#include "FlexRsu.h"
#include <DataTypes_.h>

int KFlexRsu::StepCalc( double dtS, bool addDt )
{
  if( addDt && m_pFlexSystem )
  {
    m_pFlexSystem->dModelT += dtS;
    m_pFlexSystem->StartStatus = CFlexSystemA::STATUS_START;
  }
  ++m_nStep;
  KFlexBlockBase::SCallParams params;
  params.dtS = (tAnalog32)dtS;
  params.nStep = m_nStep;
  params.pszPrjObjName = m_szObjName;
  params.pszAuditDir = m_szAuditDir;
#ifdef _WIN32
  params.bSkipSimIO = s_bSkipSimIO;
#else
  params.bSkipSimIO = true;
#endif
  if( m_pFlexSystem )
    params.modelTime = m_pFlexSystem->TimeM();
  for( DWORD n=0; n<m_nDR; ++n )
    m_ppDR[n]->StepCalc( params );

  return 0;
}

int KFlexRsu::TCoreStepCalc( double dtS, __time64_t curMoment )
{
  if( m_pFlexSystem )
  {
    m_pFlexSystem->dModelT = curMoment - m_pFlexSystem->startTime;
    m_pFlexSystem->StartStatus = CFlexSystemA::STATUS_START;
  }
  ++m_nStep;
  KFlexBlockBase::SCallParams params;
  params.dtS = (tAnalog32)dtS;
  params.nStep = m_nStep;
  params.pszPrjObjName = m_szObjName;
  params.pszAuditDir = m_szAuditDir;
  
  params.modelTime = curMoment;
  
  for( DWORD n=0; n<m_nDR; ++n )
    m_ppDR[n]->StepCalc( params );

  return 0;
}
