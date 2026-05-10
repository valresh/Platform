#include "stdafx.h"
#include "Valve_M_b.h"



void CValve_M_b::Control(double dt)
{
	if(Поворот.Use())
		Valve = Поворот;
	if(Положение_Задание.Use())
	{
		if(Inver_Task_Position)
			Valve = 100.0 - Положение_Задание;
		else
			Valve = Положение_Задание;
	}
}

void CValve_M_b::Calc( double dt )
{	
	SET_BP BreakPoint;
	if(Defect_Calc(dt));
	else Control(dt);
	double dValve = Valve - Valve_old;
	SetPosition(Position + dValve);
	Valve_old = Valve;
	CValve_b::Calc(dt);	
}
