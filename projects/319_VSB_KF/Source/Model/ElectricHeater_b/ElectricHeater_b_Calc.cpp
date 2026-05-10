#include "stdafx.h"
#include "ElectricHeater_b.h"
#include "Err.h"
#include "CommProc.h"

void CElectricHeater_b::VerifyBlk(int Command)
{
	if(pSys->IsBlk || LocalBlk)
		State = (Command != 0);
	else
		bIgnoreBlk = (State != (Command != 0));
	if(bIgnoreBlk)
		lstrcpy ( Err_BlkInObj, ObjName );
}


bool CElectricHeater_b::DefectCalc()
{
	if(nDefect & DEFECT_OVERHEATING)
		Task = 0.0;
	return (nDefect != 0);	
}

bool CElectricHeater_b::HydroTask()
{	
	bRegTask = Reg_T_Zad.Use;
	if(!Reg_T_Zad.Use)
		return false;
	bIgnoreBlk = false;
	double K = 0.0;
	if ( Reg_T_Zad.Use && Reg_T_Zad >= 0. )
	{
		K = fabs ( Reg_T_Zad.Value / (Flow_i > 0 ? T_o : T_i) );
	}
	if ( fabs ( K - 1. ) >= 1e-4 )
	{
		double K_opt = Power * K;
		Power = dReg * K_opt + ( 1. - dReg ) * Power;
		if ( Power > 10000. )
			Power = 10000.;
		if ( Power < 1e-4 )
			Power = 1e-4;
	}	
	Q = Power;
	return true;
}

void CElectricHeater_b::Control(double dt)
{
	if(Дистанционный.On())
		eSwitch = SWITCH_DIST;
	else if(Дистанционный.Off())
		eSwitch = SWITCH_MEST;
	switch(eSwitch)
	{
	case SWITCH_DIST:
		if(Запрет_пуска.On())
			break;
		if(Стоп.On())
			VerifyBlk(!Стоп);
		else if(Пуск.On())
			VerifyBlk(Пуск);
		else if(Пуск_Стоп.IsConnection)
			VerifyBlk(Пуск_Стоп);	
		else 
			bIgnoreBlk = false;
		break;
	case SWITCH_MEST:			
		if(bPuskStop)
			State = true;
		else
			State = false;
		bIgnoreBlk = false;
		break;
	}
	if(State)
	{
		if(Задание.Use())
			Task = Задание;
		else
			Task = 100.0;
	}
	else
		Task = 0.0;
}

bool CElectricHeater_b::Heat(double dt)
{
	double Ksi = 4.605 * dt * 3600.0 / TauHeat;	
	double _Task = Power *  Task * 0.01;
	if(Q > _Task)
	{
		if(Q <= _Task + 0.01)
			Q = _Task;
		else
			Q = ( Q + Ksi * _Task ) / ( 1.0 + Ksi );
	}
	else
	{
		if(Q >= _Task - 0.01)
			Q = _Task;
		else
			Q = ( Q + Ksi * _Task ) / ( 1.0 + Ksi );
	}
	return (Q != 0.0);
}

void CElectricHeater_b::Calc(double dt)
{
	SET_BP BreakPoint;	
	dP = P_i - P_o;
	if(DefectCalc());
	else if(HydroTask());
	else
		Control(dt);
	if(IsElectro && State)
		Heat(dt);
	else
	{
		Q = 0.0;
		State = false;
	}
	if(Flow_i > 0)
	{
		*pFlow_o = *pFlow_i;
		pFlow_o->h_Flow = pFlow_i->h_Flow + Q * 3600.0  / fabs(Flow_i);
	}
	else if(Flow_i < 0)
	{
		*pFlow_i = *pFlow_o;
		pFlow_i->h_Flow = pFlow_o->h_Flow + Q * 3600 / fabs(Flow_i); 
	}	
	In.PH(pFlow_i);
	Out.PH(pFlow_o);
	Температура_входа = T_i = In.T;
	Температура_выхода = T_o = Out.T;
	Дистанционное_управление = (eSwitch == SWITCH_DIST);
	Состояние = State == true ? 1 : 0; 
	Ток = Q / (U * 1.73205);
}
