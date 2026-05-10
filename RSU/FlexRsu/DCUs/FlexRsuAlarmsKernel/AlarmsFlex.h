#pragma once
#include "FlexRsuAlarmsKernel.h"
#include "AlarmsFile.h"
#include <DataTypes_.h>
#include <vector>
#include <mutex>

#pragma warning( push )
#pragma warning( disable : 4251)
class FLEXRSUALARMSKERNEL_API KAlarmsFlex
{
  DWORD m_nStep;
  //CRITICAL_SECTION m_csOnContour;
  std::recursive_mutex m_csOnContour;
  CFlexSystem* m_pFlexSystem;
  CAlarmsFile file0,file1;
  bool m_bChangeFile;
  //std::vector<DWORD> m_vOffAlarms;
public:
  KAlarmsFlex();
  virtual ~KAlarmsFlex();
protected:
  int InitL();
  void Reset();
  int Step1L();
  int StepTL( double dtS );
protected:
  virtual CFlexSystem* GetFlexSystemDataL() = 0;
  void WriteAlarmImpl( CAlarmEntry& rAlarm );
  void AckAlarmImpl( DWORD ID_PNT_NAME, __int64 moment, bool bInAlarm );
private:
protected:
  void Analiz( CAlarmsFile& file, DWORD& nAlarm, int nCount, CAlarmEntry* data );
  //void ChackForAcknowledge();
};
#pragma warning( pop )
