#include "stdafx.h"
#include "AVO.h"
#include "Err.h"


ON_DEFECT(CAVO)
	{
  SET_BP BreakPoint;
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
	k_Heat = 1.0;
    Heat_Defect = false;
    Trub_Defect = false;
    Def_Omega_Trub = 0.;
    Angle_Defect = false;
    FlowQ = 0.;
  END_IF
  IF_DEFECT(Ухудшение_теплообмена,"Ухудшение теплообмена")
    k_Heat = k_Defect_Heat->Dbl * 0.01;
  if ( k_Heat < 0.01 )
    k_Heat = 0.01;
    Heat_Defect = true;
  END_IF
//   IF_DEFECT(Разгерметизация,"Разгерметизация")
//     double F = fabs ( Heat.pIn_Trub->Flow_mol );
//     if ( F < 1. )
//       return 0;
//     double P = Heat.pIn_Trub->P - 1.;
//     if ( P < 0.1 )
//       return 0;
//     Trub_Defect = true;
//     Def_Omega_Trub = def_Omega->Dbl * 0.01 * F / P;
//     FlowQ = def_Загазован_2->Dbl * 0.01;
//   END_IF
//   IF_DEFECT(Утечка_фланца,"Утечка фланца")
//     FlowQ = def_Загазован_1->Dbl * 0.01;
//   END_IF
  IF_DEFECT(Загазован,"Загазованность")
    FlowQ = def_Загазован_3->Dbl * 0.01;
  END_IF
  IF_DEFECT(Отказ_регулятора_угла_атаки,"Отказ регулятора угла атаки")
    Angle_Defect = true;
    Def_Angle = def_Angle->Dbl * 0.01;//DEF_D(def_Angle,"Фиксированный угол атаки, %", 50., 0., 100.)
  END_IF
	return 0;
  }

