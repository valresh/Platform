#include "stdafx.h"
#include "ElectricHeater_b.h"
#include "Err.h"

DWORD CElectricHeater_b::ClassRootNode = 0;

int CElectricHeater_b::OnDefect( CDef * pDefect )
{
	SET_BP BreakPoint;
	IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
		Авария = 0;
		Перегорание_тэнов->Fire = 0; 
		Восстановление->Fire = 0;
		nDefect = 0;
	END_IF
	IF_DEFECT(Перегорание_тэнов, "Перегорание тэнов")
		nDefect	|= DEFECT_OVERHEATING;
		Авария = 1;
	END_IF
	return 0;
}

