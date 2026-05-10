#include "stdafx.h"
#include "AirDry.h"
#include "Err.h"
#include "CommProc.h"
#include "EventLogTypes.h"
#include "Names.h"

DWORD CAirDry::ClassRootNode = 0;

int CAirDry::OnDefect( struct CDef * pDefect )
{
	SET_BP BreakPoint;
	IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
		FlowQ = 0;
	END_IF
	IF_DEFECT(Загазован,"Загазованность")
		FlowQ = def_Загазован->Dbl * 0.01;
	END_IF
		return 0;
}

