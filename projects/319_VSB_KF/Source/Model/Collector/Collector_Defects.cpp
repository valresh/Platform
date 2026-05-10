#include "stdafx.h"
#include "Collector.h"
#include "Err.h"


ON_DEFECT(CCollector)
	{
  SET_BP BreakPoint;
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
	  Defect = 0;
  END_IF
  IF_DEFECT(Прекращение_подачи,"Прекращение подачи/приема")
    Defect = 1;
  END_IF
   
	return 0;
  }

