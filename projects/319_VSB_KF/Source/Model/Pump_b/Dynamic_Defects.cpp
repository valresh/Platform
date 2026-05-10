
#include "stdafx.h"
#include "Dynamic.h"
#include "Err.h"

DWORD CDynamic::ClassRootNode = 0;

int CDynamic::OnDefect( CDef * pDefect )
{
	SET_BP BreakPoint;
	IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
		defContamination->Fire = 0;
		defReset->Fire = 0;
		nDefect = 0;		
		FlowQ = 0.0;	
		kDef = 1.0;
		bRest = false;
	END_IF
	IF_DEFECT(defContamination,"Загазованность")
		FlowQ = _defContamination->Dbl * 0.01;
	END_IF
	IF_DEFECT(defReset, "Сброс насоса")
		bRest = true;
	END_IF
	IF_DEFECT(Падение_мощности_насоса, "Падение мощности насоса")
		kDef = Падение_мощности_насоса_Значение->Dbl * (-0.005) + 1.0;
	END_IF
	return 0;
}

