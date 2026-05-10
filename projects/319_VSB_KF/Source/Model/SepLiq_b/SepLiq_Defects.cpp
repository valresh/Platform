#include "stdafx.h"
#include "SepLiq.h"
#include "Err.h"


ON_DEFECT(CSepLiq)
  {
  SET_BP BreakPoint;
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
    FlowQ = 0.;
  END_IF
  IF_DEFECT(Опустошение,"Опустошение")
    Reg_L.Set( 0.01 );
  END_IF
    IF_DEFECT(Переполнение,"Переполнение")
    Reg_L.Set( 100.01 );
  END_IF
  IF_DEFECT(Загазованность,"Загазованность")
    FlowQ = def_Утечки->Dbl * 0.01;
  END_IF
  return 0;
  }
