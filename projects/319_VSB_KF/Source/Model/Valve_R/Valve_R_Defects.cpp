#include "stdafx.h"
#include "Valve_R.h"
#include "Err.h"


ON_DEFECT(CValve_R)
  {
  SET_BP BreakPoint;
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
    DefOpen = false;
  END_IF
    IF_DEFECT(Отказ,"Отказ")
    DefOpen = true;
  END_IF
    return 0;
  }
