#include "stdafx.h"
#include "IO.h"
#include "Err.h"


//int CIO::OnDefect( CDef * pDefect )
ON_DEFECT(CIO)
  {
  SET_BP BreakPoint;
//  if ( Script.IsDefect( pDefect ))
//    return 1;
  IF_DEFECT(Прекращение_подачи,"Прекращение подачи")
    Прекращение_подачи->Off();
    Возобновление_подачи->On();
    Omega_Defect = 1e-6;
  END_IF
  IF_DEFECT(Возобновление_подачи,"Возобновление подачи")
    Прекращение_подачи->On();
    Возобновление_подачи->Off();
    Omega_Defect = 1.;
  END_IF
  IF_DEFECT(Изменение_давления,"Изменение давления")
    Pizb = def_P->Dbl;//, "Подача кг/ч", 50000., 0., 100000. )
    SetComp( );
  END_IF
  IF_DEFECT(Изменение_температуры,"Изменение температуры")
    T = def_T->Dbl;// "Температура", -35., -35., 100. )
    SetComp( );
  END_IF
  IF_DEFECT(Изменение_состава,"Изменение состава")
    CompDefect = 1;
    SetComp( );
  END_IF
  IF_DEFECT(Восстановление_состава,"Восстановление состава")
    CompDefect = 0;
    SetComp( );
  END_IF
  IF_DEFECT(Восстановление_работоспособности,"Восстановление работоспособности")
    Закупорен = false;
  END_IF
  IF_DEFECT(Закупорка,"Закупорка")
    Закупорен = true;
  END_IF
	return 0;
  }

