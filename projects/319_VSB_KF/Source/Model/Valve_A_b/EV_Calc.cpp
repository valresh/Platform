#include "stdafx.h"
#include "EV.h"
#include "Err.h"
#include "CommProc.h"

void CEV::Control(double dt)
{	
	if(ControlNoElectro(dt)) 
	{
		bClose = bOpen = bStop = false;
		return;
	}
	if(Electro == Не_влияет)
		if(ControlNoKip(dt))
		{
			bClose = bOpen = bStop = false;
			return;
		}
	bool _bClose = false, _bOpen = false, _bStop = false;
	switch(eSwitch)
	{
	case SWITCH_DIST:
		{
			Дистанционный = 1;
			if(Закрыть.On())
				_bClose = true;
			if(Открыть.On())
				_bOpen = true;
			if(Стоп.On())
				_bStop = true;
			break;
		}
	case SWITCH_MEST:
		{
			Дистанционный = 0;
			if(bClose)
				_bClose = true;
			if(bOpen)
				_bOpen = true;
			if(bStop)
				_bStop = true;
			break;
		}
	}
	bClose = bOpen = bStop = false;
	if(MoveOnSignal)
	{
		if(_bClose)
			conditionCross = 0.0;
		else if(_bOpen)
			conditionCross = 100.0;
		else
			conditionCross = Position;			
	}
	else
	{
		if(_bStop)
			conditionCross = Position;
		else if(_bClose)
			conditionCross = 0.0;
		else if(_bOpen)
			conditionCross = 100.0; 			
	}
	if((pSys->IsBlk || LocalBlk))
	{
		if(!Fixed_Task)
			Task = conditionCross;
		else 
			conditionCross = Task;
		bIgnoreBlk = false;
	}
	else
		bIgnoreBlk = (Position != conditionCross);
}

void CEV::Calc(double dt)
{
	SET_BP BreakPoint;
	CValve_A_b::Calc(dt);
}
