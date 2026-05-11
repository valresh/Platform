#include "stdafx.h"
#include "Valve_A_b.h"

void CValve_A_b::VerifyBlk(double _Task)
{
	if(pSys->IsBlk || LocalBlk)
		Task = _Task;
	else
		bIgnoreBlk = (Position != _Task);
	if(bIgnoreBlk)
		lstrcpy ( Err_BlkInObj, ObjName );

}

bool CValve_A_b::IsHydroTask()
{	
	if(Reg_Position.Use)
		eHydroTask =  HYDRO_TASK_NORM;
	if(!Reg_Position.Use)
	{
		eHydroTask =  HYDRO_TASK_NO;
		return false;
	}
	bIgnoreBlk = false;
	if ( Reg_Position.Use && Reg_Position >= 0. )
		Task = Position = Reg_Position;
	return true;
}


bool CValve_A_b::ControlNoKip( double dt )
{
	if( Press_KIP[nKIP] <= PminKIP && KIP >= Открывается && KIP <= Не_влияет)
	{
		if(KIP == Не_меняется)
			Fixed_Position = true;
		else
		{
			Fixed_Position = false;
			switch(KIP)
			{
			case Открывается:
				Task = 100.;
				break;
			case Закрывается:
				Task = 0.0;
				break;
			case Не_влияет:
				return false;
			}
		}			
		return true;
	}
	else
		Fixed_Position = false;
	return false;
}

bool CValve_A_b::ControlNoElectro( double dt )
{
	if( !IsElectro && Electro >= Открывается && Electro <= Не_влияет)
	{
		if(Electro == Не_меняется)
			Fixed_Position = true;
		else
		{
			Fixed_Position = false;
			switch(Electro)
			{
			case Открывается:
				Task = 100.;
				break;
			case Закрывается:
				Task = 0.0;
				break;
			case Не_влияет:
				return false;
			}
		}			
		return true;
	}
	else
		Fixed_Position = false;
	return false;
}

bool CValve_A_b::ControlDubler(double dt)
{
	double dDubler = Dubler - Dubler_old;
	if(bZacep && !(nDefect & DEFECT_FIXED_POSITION))
	{
		Position += dDubler;
		Task = Position;
	}
	Dubler_old = Dubler;
	return bZacep;
}

void CValve_A_b::Drive(double dt)
{
	double Ksi = (4.605 / Tau_Open) * dt * 3600.0;
	if(Position > Task)
	{
		if(Position <= Task + 0.01)
			SetPosition(Task);
		else
			SetPosition((Position + Ksi * Task) / (1 + Ksi));
	}
	else
	{
		if(Position >= Task - 0.01)
			SetPosition(Task);
		else
			SetPosition((Position + Ksi * Task) / (1 + Ksi));
	}	
}


bool CValve_A_b::Defect_Calc( double dt ) 
{
	if(nDefect & DEFECT_FULL_OPEN)
	{
		Task = 100.0;
		Tau_Open = Время_открытия->Dbl;
	}
	if(nDefect & DEFECT_FULL_CLOSE)
	{
		Task = 0.0;
		Tau_Open = Время_закрытия->Dbl;
	}
	if(nDefect & DEFECT_LIMIT_STOCK)
	{
		if(Position < Минимальная_позиция->Dbl)
		{
			Task = Минимальная_позиция->Dbl;
			Tau_Open = Время_открытя_закрытия->Dbl;
		}
		else if(Position > Максимальная_позиция->Dbl)
		{
			Task = Максимальная_позиция->Dbl;
			Tau_Open = Время_открытя_закрытия->Dbl;
		}		
	}
	return CValve_b::Defect_Calc(dt);
}

void CValve_A_b::Calc( double dt )
{	
	SET_BP BreakPoint;
	Old_Position = Position;
  if ( Reg_On )
    {
    Control(dt);
    SbrosReg = true;
    }
  else
    {
    SbrosReg = false;
	  if(Defect_Calc(dt));
	  else if(IsHydroTask());
	  else if(ControlDubler(dt));
	  else Control(dt);
	  if(!bZacep)
		  Drive(dt);
    }
	CValve_b::Calc(dt);

	Положение = Position;
	Обратное_положение = 100.0 - Position;
	if ( Position > 99. )
		Открыт = 1;
	else
		Открыт = 0;
	if ( Position < 1. )
		Закрыт = 1;
	else
		Закрыт = 0;
	if(Old_Position > Position)
		Закрытие = 1;
	else if (Old_Position < Position)
		Открытие = 1;
	else
		Закрытие = Открытие = 0;
	
}
