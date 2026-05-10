#include "stdafx.h"
#include "Ejector.h"
#include "Err.h"


ON_DEFECT(CEjector)
  {
  SET_BP BreakPoint;
	//IF_DEFECT(Исправен,"Исправен")
	//END_IF
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
    FlowQ = 0.;
    Defect = 0;
    O_Razgerm = 0.;
  END_IF
  IF_DEFECT(Останов,"Останов")
  // DEF_D( def_Утечки, "Макс. загазованность в % ПДК", 50., 0., 200. )
    Defect = -1;
  END_IF
  IF_DEFECT(Разгерметизация,"Разгерметизация")
    FlowQ = 1000;
    Defect = 1;
    if ( OmegaRazgerm > 0 )
      O_Razgerm = OmegaRazgerm;
    else
      if ( p_In->P > 1.1 )
        O_Razgerm = 2. * p_In->Flow_mol / ( p_In->P - 1. );
      else
        O_Razgerm = 0.;
  END_IF
	return 0;
  }

