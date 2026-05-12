#include "stdafx.h"
#include "CV.h"
#include "Err.h"
#include "CommProc.h"



void CCV::Control(double dt)
  {
  if ( Reg_On )
    {
    Reg( dt );
    return;
    }
	if(ControlNoKip(dt)) 
		return;
	if(KIP == Не_влияет)
		if(ControlNoElectro(dt))
			return;	
	if(Соленоид.On() && (pSys->IsBlk || LocalBlk))
	{
		if ( KIP == Открывается )
			Task = 100.0;
		else if ( KIP == Закрывается )
			Task = 0.0;
	}
	else if(Задание.Use())
	{
			if(Invert_Task)
				Task = 100.0 - Задание;
			else
				Task = Задание;
			if(Соленоид.On())
			{
				if ( KIP == Открывается )
					bIgnoreBlk = (Position != Соленоид * 100.0);
				else if ( KIP == Закрывается )
					bIgnoreBlk = (Position != (1 - Соленоид) * 100.0);
			}
	}		
}


void CCV::Calc(double dt)
{
	SET_BP BreakPoint;
	CValve_A_b::Calc(dt);
	Сработал_соленоид = Соленоид.Value;
}
