#include "stdafx.h"
#include "Valve_M_b.h"

int CValve_M_b::OnDefect( CDef * pDefect )
{
	SET_BP BreakPoint;
	IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
		Valve = Position;
		Fixed_Position = false;
		Заклинивание_штока->Fire = 0;
	END_IF
	IF_DEFECT(Несанкционированное_открытие, "Несанкционированное открытие")
		Несанкционированное_открытие->Fire = 0;
		Valve = Position = 100.0;
	END_IF
	IF_DEFECT(Несанкционированное_закрытие, "Несанкционированное закрытие")
		Несанкционированное_закрытие->Fire = 0;	
		Valve = Position = 0.0;
	END_IF
	IF_DEFECT(Заклинивание_штока, "Заклинивание штока")
		Fixed_Position = true;
	END_IF
	return CValve_b::OnDefect(pDefect);
}

