#include "stdafx.h"
#include "Motor.h"
#include "Err.h"


ON_DEFECT(CMotor)
	{
  SET_BP BreakPoint;
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
    Defect = Исправен;
    Defect_Pow = 1.;
  END_IF
  IF_DEFECT(Останов_двигателя,"Останов двигателя")
    Defect = ОтказДвигателя;
  END_IF
  IF_DEFECT(Падение_оборотов_двигателя,"Падение оборотов двигателя")
    Defect = ПадениеОборотов;
  //  DEF_D( def_Omega_Pow, "Степень снижения мощности %", 50., 0., 100. )
    Defect_Pow = 1.0 - def_Pow->Dbl * 0.01;
  END_IF
  //IF_DEFECT(Авария_двигателя,"Авария")
  //  Defect = АварияДвигателя;
  //END_IF
  IF_DEFECT(Отказ_пусковой_аппаратуры,"Отказ пусковой аппаратуры")
    Defect = ОтказАппаратуры;
  END_IF
	return 0;
  }

