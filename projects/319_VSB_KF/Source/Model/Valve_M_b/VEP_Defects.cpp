#include "stdafx.h"
#include "VEP.h"
#include "Err.h"

DWORD CVEP::ClassRootNode = 0;

int CVEP::OnDefect( CDef * pDefect )
{
	SET_BP BreakPoint;
	IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
		Падение_клина->Fire = 0;
	END_IF
	IF_DEFECT(Падение_клина, "Падение клина");
		Position = 10.0;
		Fixed_Position = true;
	END_IF;
	return CValve_M_b::OnDefect(pDefect);
}