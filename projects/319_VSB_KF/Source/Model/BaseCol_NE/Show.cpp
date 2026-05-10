#include "stdafx.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"
#include "Data.h"
#include "SysDataTypes.h"
#include "Extensions.h"


void CShowРежим::Show ( struct CParams * Param )
  {
  int SysStep = pSys->m_nStep;
  if ( StepT == SysStep )
    return;
  StepT = SysStep;
  pCol -> SetРежим ( true );
  }

