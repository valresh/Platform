#include "stdafx.h"
#include "Volumetric.h"
#include "Err.h"

DWORD CVolumetric::ClassRootNode = 0;

int CVolumetric::OnDefect( CDef * pDefect )
{
	SET_BP BreakPoint;
	IF_DEFECT(Исправен,"Исправен")
		FlowQ = 0.0;		
	END_IF
	IF_DEFECT(Загазован,"Загазованность")
		FlowQ = def_Загазован->Dbl * 0.01;
	END_IF
	return 0;
}
