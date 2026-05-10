#include "stdafx.h"
#include "PPK.h"
#include "Err.h"


ON_DEFECT(CPPK)
	{
  SET_BP BreakPoint;
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
    DefOpen = false;
  //		P_Open = 1000.;
  END_IF
  IF_DEFECT(Срабатывание_ППК,"Срабатывание ППК")
    DefOpen = true;
  //		P_Open = def_P->Dbl;
  END_IF
	return 0;
  }

