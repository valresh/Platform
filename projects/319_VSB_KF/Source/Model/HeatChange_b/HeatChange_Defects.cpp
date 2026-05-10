#include "stdafx.h"
#include "HeatChange.h"
#include "Err.h"


ON_DEFECT(CHeatChange)
	{
  SET_BP
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
  Omega_Defect = false;
  k_Omega = 1.;
  Def_Zasor = false;
  FlowQ = 0.;
  Перемешивание = false;

  K_Defect = 1.;
  END_IF
  IF_DEFECT(Ухудшение_теплообмена,"Ухудшение теплообмена")
    K_Defect = 1. - k_Defect_Heat->Dbl * 0.01;
  END_IF
  IF_DEFECT(Засорение_труб,"Засорение труб")
    Omega_Defect = true;
    k_Omega = 1.0 - k_Defect_Omega->Dbl * 0.01;
  END_IF
  IF_DEFECT(Внутренняя_разгерметизация,"Внутренняя_разгерметизация")
    Def_Trub = def_in_Trub->Dbl * 0.01;
    Перемешивание = true;
  END_IF
  IF_DEFECT(Загазованность,"Загазованность")
    FlowQ = def_Загазованность->Dbl * 0.01;
  END_IF
	return 0;
  }

