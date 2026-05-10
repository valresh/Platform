#include "stdafx.h"
#include "CoolObj.h"
#include "Err.h"

ON_DEFECT(CCoolObj)
//int CCoolObj::OnDefect( CDef * pDefect )
{
	SET_BP BreakPoint;
	IF_DEFECT(pRecoveryDef, "Восстановление работоспособности")
		Перегрев->Fire = 0;
		Defect = 0;
		nSens = -1;
		Pow = 0.;
	END_IF
	IF_DEFECT(Перегрев,"Перегрев")
		Defect = 1;
		nSens = -1;
		for ( int n = 0; n < kSens; n++ )
		{
			if ( pSens[n] && pSens[n]->Use())
			{
				nSens = n;
				break;
			}
		}
		Pow = Степень->Dbl * 0.01;
	END_IF
	return 0;
}

