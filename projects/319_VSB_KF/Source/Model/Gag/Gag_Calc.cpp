#include "stdafx.h"
#include "Gag.h"
#include "Err.h"
#include "CommProc.h"


void CGag::Calc( double dt )
{
	SET_BP;
	// pGag->mOn=true;
	if( bOpen )
	{
		Заглушка = true;
		return;
	}

	if ( Исправна )
	{
		if ( bOpen )
			Задание = 1;
		else
			Задание = 0;
		Заглушка = (Задание != 1);
	}
}
