#include "stdafx.h"
#include "Drainage.h"
#include "Err.h"

ON_DEFECT(CDrainage)
	{
  SET_BP BreakPoint;
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
  END_IF
  IF_DEFECT(Закрыт,"Несанкционированное закрытие")
    Ventil = 0.0;
  END_IF
  IF_DEFECT(Полностью_открыт,"Несанкционированное открытие")
    Ventil = 100.0;
  END_IF
	return 0;
  }

