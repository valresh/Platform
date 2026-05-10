#pragma once
#include "Mnemo.h"
#include "AlmGr.h"
#include <rsu_mio.h>

class CAlarmsFile
{
  int m_nNumber;
  DWORD m_dwAllocKvant;
  DWORD m_dwCountKvant;
  DWORD m_dwCurrSector;
  //
  //HANDLE m_hMapData;
  std::string m_FileName = "";
  rsu_cp::RsuMio m_RsuMio;
  BYTE*  m_pbBegWnd;
  CAlarmEntry* m_pDataT;
public:
  enum
  {
    eMagicStep = 50,
  };
public:
  CAlarmsFile(int nNumber);
  ~CAlarmsFile();

  bool Init();
  void SaveAlarm(CAlarmEntry& rAlarm,int& nAlarms,THoneyWell* pHoney,CCBHSystem* pCBSystem);
  bool FindAlarm(CAlarmEntry& rAlarm,int& nAlarms,THoneyWell& rHoney);
  const std::string& FileName() { return m_FileName; }
  //HANDLE HandleT();
  //
protected:
  /*BYTE* MapView(HANDLE hHandle, DWORD dwOffset, SIZE_T dwSize)
  {
    return (BYTE*)::MapViewOfFile( hHandle, FILE_MAP_ALL_ACCESS, 0, dwOffset, dwSize );
  }*/
  bool OpenAlarmData();
  void Clear();
  //
};
