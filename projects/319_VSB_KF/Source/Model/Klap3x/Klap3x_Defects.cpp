#include "stdafx.h"
#include "Klap3x.h"
#include "Err.h"

ON_DEFECT(CKlap3x)
//int CKlap3x::OnDefect( CDef * pDefect )
	{
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
    //pPos->Off_State ( CKlapan::REG_BROKEN );
    Defect = 0;
    Fixed_Pos = 0;
  END_IF
  IF_DEFECT(Положение_фиксировано,"Положение фиксировано")
    //pPos->On_State ( CKlapan::REG_BROKEN );
    Defect = 1;
    Fixed_Pos = Defect_Pos->Dbl;
  END_IF	//IF_DEFECT(Исправен,"Исправен")
	//END_IF
	return 0;
  }

