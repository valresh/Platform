#include "AlarmsFile.h"
#include <CommonH.h>

//
CAlarmsFile::CAlarmsFile(int nNumber)
: m_nNumber(nNumber)
//, m_hMapData(NULL)
, m_pbBegWnd(NULL)
, m_dwCurrSector(0)
, m_pDataT(NULL)
{
  m_dwAllocKvant = m_RsuMio.page_size();
  m_dwCountKvant = m_dwAllocKvant / sizeof(CAlarmEntry);
  ASS(m_dwAllocKvant == m_dwCountKvant * sizeof(CAlarmEntry));
}

CAlarmsFile::~CAlarmsFile()
{
  Clear();
}

void CAlarmsFile::Clear()
{
    m_RsuMio.close();
    m_pbBegWnd = NULL;
    m_pDataT = NULL;
    m_dwCurrSector = 0;
}

bool CAlarmsFile::Init()
{
  Clear();
  return OpenAlarmData();
}
//
bool CAlarmsFile::OpenAlarmData()
{
    m_RsuMio.close();
    m_FileName = std::string(GetAlarmFileName(m_nNumber == 1));
    fs::path fn(m_FileName);
    m_RsuMio.open(fn.filename().generic_string().c_str(), m_dwAllocKvant, m_dwAllocKvant * m_dwCurrSector, fn.parent_path().generic_string().c_str());
    return m_RsuMio.state();
}
//
void CAlarmsFile::SaveAlarm( CAlarmEntry& rAlarm, int& nAlarms, THoneyWell* pHoney, CCBHSystem* pCBSystem )
{
  if( pHoney && !pHoney->m_pObj )
    return;

  if( (nAlarms % m_dwCountKvant) == 0 )
  {
      OpenAlarmData();
      m_pbBegWnd = (BYTE*)m_RsuMio.data();
      ASS(m_pbBegWnd != NULL);
      m_pDataT = (CAlarmEntry*)(m_pbBegWnd);
      memset(m_pDataT, 0, m_dwAllocKvant);
  }
  //
  // До пятидесятого шага всё должно быть квитировано
  if( pHoney )
  {
    bool bQuqu = 1;
    bQuqu = ( pCBSystem->nStep > eMagicStep );
    rAlarm.btKvit =  bQuqu ? 0:1;
    rAlarm.nTimeS = (int)pCBSystem->timeB;
    rAlarm.nTimeA = (int)pCBSystem->timeM;
    pHoney->m_pObj->notACK = bQuqu ? 1:0;
    pHoney->m_pObj->setACK = 0;
  }
  //
  memcpy( m_pDataT, &rAlarm, sizeof(CAlarmEntry) );
  m_pDataT++;
  nAlarms++;
}
//
bool CAlarmsFile::FindAlarm(CAlarmEntry& rAlarm,int& nAlarms,THoneyWell& rHoney)
{
  CAlarmEntry* a = (CAlarmEntry*)m_pbBegWnd;
  if ( a == NULL )
    return false;
  for ( int N = m_dwCurrSector; N >= 0; N-- )
  {
    for ( int n = m_dwCountKvant-1; n >= 0; n-- )
    {
      if ( a[n].dwName == rAlarm.dwName )// Номер в Names.dat (уникален)
        //    if ( a[n].dwType == rAlarm.dwType )// Тип объекта (хватит dwName )
        if ( a[n].btType == rAlarm.btType )// Тип сигнализации
        {
          a[n].btOnOf = 0;
          rAlarm.btEHLN = a[n].btEHLN;
          return (a[n].btKvit & 0x01) == 0x01;
        }
    }
  }
  return false;
}
//
