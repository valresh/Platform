
#include "stdafx.h"
#include "Gag.h"
#include "Err.h"


ON_DEFECT(CGag)
{
	SET_BP
		NewParams = true;
	IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
		Исправна = 1;
	END_IF
	IF_DEFECT(Закрыт,"Самопроизвольное закрытие")
		Исправна = 0;
		Заглушка = false;
	END_IF
	IF_DEFECT(Полностью_открыт,"Самопроизвольное открытие")
		Исправна = 0;
		Заглушка = true;
	END_IF
	return 0;
}

