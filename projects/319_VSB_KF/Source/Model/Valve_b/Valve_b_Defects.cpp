#include "stdafx.h"
#include "Valve_b.h"
#include "Err.h"

int CValve_b::OnDefect( CDef * pDefect )
{
	SET_BP BreakPoint;
	IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
		K_def = 1.0;
		Осмоление->Fire = 0;
		Omega_Depressurization = 0.0;
		Нарушение_герметичности->Fire = 0;
		nDefect = 0;
	END_IF
	IF_DEFECT(Осмоление, "Осмоление")
		K_def = 1.0 - (Осмоление_Значение->Dbl * 0.009);
	END_IF
	IF_DEFECT(Нарушение_герметичности, "Нарушение герметичности")
		Omega_Depressurization = Omega_max * Нарушение_герметичности_Значение->Dbl * 0.009 * K_Omega * K_def;
	END_IF
	return 0;
}

