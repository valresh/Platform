#pragma once
#include <AlarmEntry_.h>
#include <rsu_mio.h>
#include <string>

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
  CAlarmsFile(int nNumber);
  ~CAlarmsFile();

  bool Init();
  void Clear();
  void SaveAlarm( CAlarmEntry& rAlarm, DWORD& nAlarms );
  bool FindAlarm( CAlarmEntry& rAlarm, DWORD& nAlarms );
  const std::string& FileName() { return m_FileName; }
  void Flush();
  //HANDLE HandleT();
  //
protected:
  /*
  BYTE* MapView( HANDLE hHandle, DWORD dwOffset, SIZE_T dwSize )
  {
    return (BYTE*)::MapViewOfFile( hHandle, FILE_MAP_ALL_ACCESS, 0, dwOffset, dwSize );
  }
  */
  bool OpenAlarmData();
  //
};
