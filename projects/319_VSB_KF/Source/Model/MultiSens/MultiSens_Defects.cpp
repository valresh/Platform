
#include "stdafx.h"
#include "MultiSens.h"
#include "Err.h"

DWORD CMultiSens::ClassRootNode = 0;
int CMultiSens::OnDefect( CDef * pDefect )
{
	SET_BP BreakPoint;
	IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
		eDefect = DEFECT_NO;
		kDefectSens = -1;
	END_IF
	IF_DEFECT(Рост_температуры_подшипников, "Рост температуры подшипников")
		kDefectSens = (int)(r01() * (kTSens - 1));
		*pT[kDefectSens] = Температура_подшипников->Dbl;
	END_IF
	IF_DEFECT(Рост_вибрации, "Рост вибрации")
		for ( int n = 0; n < kVibroSens; n++ )
		{
			*(pVibro[n]) = Вибрация->Dbl;
		}
		eDefect = DEFECT_VIBRO;
	END_IF
	IF_DEFECT(Рост_сдвига, "Рост сдвига")
		for ( int n = 0; n < kShiftSens; n++ )
		{
			*(pShift[n]) = Сдвиг->Dbl;
		}
		eDefect = DEFECT_SHIFT;
	END_IF
	return 0;
}
