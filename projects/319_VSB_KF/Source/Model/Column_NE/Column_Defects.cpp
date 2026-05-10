#include "stdafx.h"
#define DLL_COLUMN
#include "Column_NE.h"
#include "Err.h"

ON_DEFECT(CColumn_NE)
	{
  SET_BP BreakPoint;
  IF_DEFECT(Восстановление,"Восстановлена")
    Column.pStage[1]->Razgerm_Gas = false;
    Column.pStage[kStage]->Razgerm_Liq = false;
    Column.pStage[1]->Omega_Razgerm = 0.;
    Column.pStage[kStage]->Omega_Razgerm = 0.;
    FlowQ = 0.;
  END_IF
  IF_DEFECT(Резгерметизация_верха,"Разгерметизация верха")
    Column.pStage[1]->Razgerm_Gas = true;
    Column.pStage[1]->Omega_Razgerm = Omega_Top;
    FlowQ = 10;
  END_IF
  IF_DEFECT(Резгерметизация_низа,"Разгерметизация низа")
    Column.pStage[kStage]->Razgerm_Liq = true;
    Column.pStage[kStage]->Omega_Razgerm = Omega_Bott;
    FlowQ = 10;
  END_IF
//  IF_DEFECT(Переполнение,"Переполнение(+другой отказ)")
//???    Column.CanOverflow = true;
//  END_IF
	return 0;
  }

