#pragma once
#define _AFXDLL
#include <TrendStr.h>
#include <TrendSrvH.h>
#include <UniBuffer.h>
#include <mio.hpp>
#include <crosslock.h>

class CTrendFile
  {
    std::recursive_mutex m_Sec;
  public:
	  CTrendFile(void);
	  virtual ~CTrendFile(void);
  private:
    //
    mio::mmap_source  m_hHeadMap;
    CTrendHeaderEx* m_pHeadT;
    CTrendHeader mHeader;
    DWORD   m_dwName;
    //
    mio::mmap_source  m_hDataMap;
    //
    bool   OpenTrendHead();
    bool   OpenTrendData();
    STrendId* GetHeader(CTrendHeader* header);
    CTrendHeaderEx* GetHeader();
    DWORD  CloseTrend();
  public:
    int    FromTrend(SToTrend* to, char* from);
  private:
    //
		int BuildHeader();
    //
    SUniName mName;
    SUniChar mChar;
  private:
    void BuildNames(SToTrend* to);
    //
    float* MapTrendFile(uint64_t Addr, uint64_t& dwBegin, uint64_t& dwEnd);
  };
