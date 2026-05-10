#include "stdafx.h"
#include "V.h"
#include "Err.h"

ON_DEFECT(CV)
{
	SET_BP BreakPoint;
	IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
		if(Падение_клина)
			Падение_клина->Fire = 0;
		if(Повреждение_запорного_механизма)
			Повреждение_запорного_механизма->Fire = 0;
	END_IF
	IF_DEFECT(Падение_клина, "Падение клина");
		Position = 10.0;
		Fixed_Position = true;
	END_IF
	return CValve_M_b::OnDefect(pDefect);
}