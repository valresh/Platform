#include "stdafx.h"
#include "Electro.h"
#include "Err.h"

DWORD CElectro::ClassRootNode = 0;

int CElectro::OnDefect( CDef * pDefect )
{
	SET_BP BreakPoint;
	IF_DEFECT(Восстановление_работоспособности, "Восстановление работоспособности")
		for(int i = 0; i < nModel; i++)
			if(aModel && aModel[i])
				aModel[i]->ElectroOn();
		Прекращение_подачи_электроэнергии->Fire = 0;
		Восстановление_работоспособности->Fire = 0;
	END_IF
	IF_DEFECT(Прекращение_подачи_электроэнергии, "Прекращение подачи электроэнергии")
	for(int i = 0; i < nModel; i++)
		if(aModel && aModel[i])
			aModel[i]->ElectroOff();
	END_IF
	return 0;
}

