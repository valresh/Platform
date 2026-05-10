#pragma once
#include "../FlexRsuCore/FlexDR.h"

#ifdef FLEXRSUDEBUGBRIDGE_EXPORTS
#define FLEXRSUDEBUG_API _EXP
#else
#define FLEXRSUDEBUG_API _IMP
#endif

struct SPrjCfg //FLEXRSUDEBUG_API SPrjCfg
{
  typedef bool tSkipSimIO_;
  SPrjCfg()
  {
    ZeroMemory( this, sizeof(*this) );
  }
  KFlexDR **ppDR;
  DWORD nDR;
  DWORD *pSteps;
  DWORD *pInitCounter;
  tSkipSimIO_ *pSkipSimIO;
  struct CFlexSystem *pFlexSystem;
};

FLEXRSUDEBUG_API void ShowDebugWnd( LPCSTR pszPrjName, HWND hMainWnd, SPrjCfg *pConfig, bool bExitOnClose );
FLEXRSUDEBUG_API void CloseDebugWnds();
