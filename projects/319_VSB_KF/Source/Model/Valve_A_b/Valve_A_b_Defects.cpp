#include "stdafx.h"
#include "Valve_A_b.h"

int CValve_A_b::OnDefect( CDef * pDefect )
{
	SET_BP BreakPoint;	
	IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
		Полное_открытие->Fire = 0;
		Полное_закрытие->Fire = 0;
		Ограничение_штока->Fire = 0;
		Повреждение_запорного_устройства->Fire = 0;
		return CValve_b::OnDefect(pDefect);
	END_IF
	IF_DEFECT(Полное_открытие, "Полное открытие")
		nDefect |= DEFECT_FULL_OPEN;
	END_IF
		IF_DEFECT(Полное_закрытие, "Полное закрытие")
		nDefect |= DEFECT_FULL_CLOSE;
	END_IF
		IF_DEFECT(Ограничение_штока, "Ограничение штока")
		nDefect |= DEFECT_LIMIT_STOCK;
	END_IF
	IF_DEFECT(Повреждение_запорного_устройства, "Повреждение запорного устройства")
		nDefect |= DEFECT_FALLING_WEDGE;
	END_IF
	return CValve_b::OnDefect(pDefect);
}

