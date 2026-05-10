#include "stdafx.h"
#include "Fan.h"
#include "Err.h"


ON_DEFECT(CFan)
	{
  SET_BP BreakPoint;
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
    Defect = 0;
    Defect_Pow = 1.;
  END_IF
  IF_DEFECT(Останов_вентилятора,"Останов двигателя")
    Defect = 2;
  END_IF
  IF_DEFECT(Падение_мощности_вентилятора,"Падение мощности вентилятора")
    Defect = 3;
  //  DEF_D( def_Omega_Pow, "Степень снижения мощности %", 50., 0., 100. )
    Defect_Pow = ( 100. - def_Pow->Dbl ) * 0.01;
  END_IF
  //IF_DEFECT(Авария_вентилятора,"Авария")
  //  Defect = 1;
  //END_IF
  //IF_DEFECT(Загорание_вентилятора,"Загорание вентилятора")
		//Faza_Fire = 1;
  //END_IF
  IF_DEFECT(Выход_двигателя_из_строя,"Выход двигателя из строя")
    Defect = 4;
  END_IF
	return 0;
  }

