#include "stdafx.h"
#include "Valve_b.h"
#include "Err.h"
#include "CommProc.h"


void CValve_b::SetPosition( double _Position )
{
	if(!Fixed_Position)
		Position = _Position;
}

bool CValve_b::IsHydroTask()
{
	if(Reg_dP_Zad.Use || Reg_P_out_Zad.Use || Reg_P_in_Zad.Use || 
		Reg_FlowZad_mol.Use || Reg_FlowZad_m3.Use || Reg_FlowZad_nm3.Use ||
		Reg_FlowZad_kg.Use)
	{
		if(Reg_dP_Zad.Use)
		{
			if(Reg_dP_Zad == 0)
				K_Omega = MAX_K_OMEGA;
			else if(Reg_dP_Zad > 0 && _F_mol < 0.0 || Omega == 0.0)
				K_Omega = 1. / MAX_K_OMEGA; 
			else					
				K_Omega = _F_mol * K_F / Reg_dP_Zad / Omega;
			if(fabs(_Differential - Reg_dP_Zad) > 1e-2 && calcConflict == CALC_OK)
			{
				if(eHydroTask != HYDRO_TASK_CONFLICT)
					ModMsg ( "Конфликт задания по перепаду. Объект: '%s'", ObjName);
				eHydroTask = HYDRO_TASK_CONFLICT;
			}
			else
				eHydroTask = HYDRO_TASK_NORM;			
		}		
		else if(Reg_P_in_Zad.Use)
		{			
			if(Reg_P_in_Zad - _P_Out == 0)
				K_Omega = MAX_K_OMEGA;
			if((_F_mol < 0.0 && Reg_P_in_Zad > _P_Out) || (_F_mol > 0.0 && Reg_P_in_Zad < _P_Out) || Omega == 0.0)
				K_Omega = 1. / MAX_K_OMEGA;
			else
				K_Omega = _F_mol * K_F / (Reg_P_in_Zad - _P_Out) / Omega;
			if(fabs(_P_In - Reg_P_in_Zad) > 1e-2 && calcConflict == CALC_OK)
			{
				if(eHydroTask != HYDRO_TASK_CONFLICT)
					ModMsg ( "Конфликт задания по давлению на входе. Объект: '%s'", ObjName);
				eHydroTask = HYDRO_TASK_CONFLICT;
			}
			else
				eHydroTask = HYDRO_TASK_NORM;
		}
		else if(Reg_P_out_Zad.Use)
		{
			if(_P_In - Reg_P_out_Zad == 0)
				K_Omega = MAX_K_OMEGA;
			if((_F_mol < 0.0 && _P_In > Reg_P_out_Zad) || (_F_mol > 0.0 && _P_In < Reg_P_out_Zad) || Omega == 0.0)
				K_Omega = 1. / MAX_K_OMEGA;
			else
				K_Omega = _F_mol * K_F / (_P_In - Reg_P_out_Zad) / Omega;
			if(fabs(_P_Out - Reg_P_out_Zad) > 1e-2 && calcConflict == CALC_OK)
			{
				if(eHydroTask != HYDRO_TASK_CONFLICT)
					ModMsg ( "Конфликт задания по давлению на выходе. Объект: '%s'", ObjName);
				eHydroTask = HYDRO_TASK_CONFLICT;
			}
			else
				eHydroTask = HYDRO_TASK_NORM;
		}		
		else if ( Reg_FlowZad_mol.Use )
		{
			if((_F_mol >= 0 && _P_In <= _P_Out) || (_F_mol <= 0 && _P_In >= _P_Out))
				K_Omega = MAX_K_OMEGA;
			if(fabs(_F_mol) > Reg_FlowZad_mol)
				K_Omega = Reg_FlowZad_mol * K_F / fabs(_P_In - _P_Out) / Omega;				
			if(fabs(_F_mol - Reg_FlowZad_mol) > 1. && calcConflict == CALC_OK)
			{
				if(eHydroTask != HYDRO_TASK_CONFLICT)
					ModMsg ( "Конфликт задания по потоку. Объект: '%s'", ObjName);
				eHydroTask = HYDRO_TASK_CONFLICT;
			}
			else
				eHydroTask = HYDRO_TASK_NORM;
		}
		else if ( Reg_FlowZad_kg.Use )
		{
			if((Reg_FlowZad_kg >= 0 && _P_In <= _P_Out) || (Reg_FlowZad_kg < 0 && _P_In >= _P_Out) || Omega == 0.0)
				K_Omega = MAX_K_OMEGA;
			else
				K_Omega = Reg_FlowZad_kg / M * K_F / (_P_In - _P_Out) / Omega;
			if(fabs(_F_mol - Reg_FlowZad_kg / M) > 1. && calcConflict == CALC_OK)
			{
				if(eHydroTask != HYDRO_TASK_CONFLICT)
					ModMsg ( "Конфликт задания по потоку. Объект: '%s'", ObjName);
				eHydroTask = HYDRO_TASK_CONFLICT;
			}
			else
				eHydroTask = HYDRO_TASK_NORM;
		}
		else if ( Reg_FlowZad_m3.Use )
		{
			if((Reg_FlowZad_m3 >= 0 && _P_In <= _P_Out) || (Reg_FlowZad_m3 < 0 && _P_In >= _P_Out) || Omega == 0.0)
				K_Omega = MAX_K_OMEGA;
			else
				K_Omega = Reg_FlowZad_m3 * Ro / M * K_F / (_P_In - _P_Out) / Omega;
			if(fabs(_F_mol - Reg_FlowZad_m3 * Ro / M) > 1. && calcConflict == CALC_OK)
			{
				if(eHydroTask != HYDRO_TASK_CONFLICT)
					ModMsg ( "Конфликт задания по потоку. Объект: '%s'", ObjName);
				eHydroTask = HYDRO_TASK_CONFLICT;
			}
			else
				eHydroTask = HYDRO_TASK_NORM;
		}
		else if ( Reg_FlowZad_nm3.Use )
		{
			if((Reg_FlowZad_nm3 >= 0 && _P_In <= _P_Out) || (Reg_FlowZad_nm3 < 0 && _P_In >= _P_Out) || Omega == 0.0)
				K_Omega = MAX_K_OMEGA;
			else
				K_Omega = Reg_FlowZad_nm3 * iM3 * K_F / (_P_In - _P_Out) / Omega;
			if(fabs(_F_mol - Reg_FlowZad_nm3 * iM3) > 1. && calcConflict == CALC_OK)
			{
				if(eHydroTask != HYDRO_TASK_CONFLICT)
					ModMsg ( "Конфликт задания по потоку. Объект: '%s'", ObjName);
				eHydroTask = HYDRO_TASK_CONFLICT;
			}
			else
				eHydroTask = HYDRO_TASK_NORM;
		}
		return true;
	}
	if(eHydroTask == HYDRO_TASK_NORM)
		return true;
	eHydroTask = HYDRO_TASK_NO;
	return false;
	
	/*if(!bRegTask)
		return false;
	return bRegTask;
	return false;
	if(K_def != 1.0 || Omega_Depressurization != 0.0)
		return false;	
	double StopReg = 0.0;
	double K = 0.0;
	if ( Reverse && _Differential < 0. )
		return false;
	if ( Reg_dP_Zad.Use && Reg_dP_Zad > 0. )
	{
		if ( fabs ( Reg_dP_Zad ) < 0.001 )
			return false;
		K = fabs ( _Differential / Reg_dP_Zad );
	}
	else if ( Reg_P_out_Zad.Use && Reg_P_out_Zad > 0. )
	{
		double dP_Z = _P_In - Reg_P_out_Zad;
		if ( dP_Z < 0.1 )
			return false;
		K = fabs ( _Differential / dP_Z );

	}
	else if ( Reg_P_in_Zad.Use && Reg_P_in_Zad > 0. )
	{
		double dP_Z = Reg_P_in_Zad - _P_Out;
		if ( dP_Z < 0.1 )
			return false;
		K = fabs ( _Differential / dP_Z );
	}
	else if ( Reg_FlowZad_mol.Use && Reg_FlowZad_mol > 0. )
	{
 		if ( fabs ( _F_mol ) <= 0. )
			return false;
 		K = fabs ( Reg_FlowZad_mol / _F_mol );
	}
	else if ( Reg_FlowZad_m3.Use && Reg_FlowZad_m3 > 0. )
	{
		if ( fabs ( _Flow_Work_m3 ) <= 0. )
			return false;
		K = fabs ( Reg_FlowZad_m3 / _Flow_Work_m3 );
	}
	else if ( Reg_FlowZad_nm3.Use && Reg_FlowZad_nm3 > 0. )
	{
		if ( fabs ( _Flow_Work_nm3 ) <= 0. )
			return false;
		K = fabs ( Reg_FlowZad_nm3 / _Flow_Work_nm3 );
	}
	else if ( Reg_FlowZad_kg.Use && Reg_FlowZad_kg > 0. )
	{
		if ( fabs ( _Flow_Work_kg ) <= 0. )
			return false;
		K = fabs ( Reg_FlowZad_kg / _Flow_Work_kg );
	}
	else if ( Reg_FlowMax_kg.Use && Reg_FlowMax_kg > 0. )
	{
		double K = fabs ( Reg_FlowMax_kg / _Flow_Work_kg );
		if ( fabs ( _Flow_Work_kg ) <= 0. || K > 0.999)
			return false;		
		double K_opt = K_Omega * K;
		double К_Reg_old = K_Omega;
		K_Omega = dReg * K_opt + ( 1. - dReg ) * K_Omega;
		if ( K_Omega > 1000. )
			K_Omega = 1000.;
		if ( K_Omega < 0.001 )
			K_Omega = 0.001;
		if ( K_Omega > 1.2 * К_Reg_old )
			K_Omega = 1.2 * К_Reg_old;
		if ( K_Omega < 0.8 * К_Reg_old )
			K_Omega = 0.8 * К_Reg_old;
	}
	if ( fabs ( K - 1. ) < 1e-4 )
		return false;
	double K_opt = K_Omega * K;
	K_Omega = dReg * K_opt + ( 1. - dReg ) * K_Omega;
	if ( K_Omega > 10000. )
		K_Omega = 10000.;
	if ( K_Omega < 1e-4 )
		K_Omega = 1e-4;
	return true;*/
}

bool CValve_b::Defect_Calc( double dt ) 
{
	if(IS_DEFECT(DEFECT_FALLING_WEDGE))
		return false;
	return (nDefect != 0);
}

void CValve_b::Calc(double dt)
{
	SET_BP BreakPoint;
	if(IS_DEFECT(DEFECT_FALLING_WEDGE))
	{
		if(Position < 10.0)
			SetPosition(10.0);
	}
	if(Position < 1e-4)
		Position = 0.0;
	if(Position > 100.0)
		Position = 100.0;
	CValve_b::IsHydroTask();
	if ( fabs ( _F_mol ) <  1e-6 )
	{
		_Flow_Work_kg = 0.;
		_Flow_Work_m3 = 0.; 
		_Flow_Work_nm3 = 0.;
		Eps_In.T = T_air;
		Ro = 1000.;
	}
	else
	{
		pFlow_i->pComp = pFlow_i->Get_Comp();
		if(!pFlow_i->pComp)
			return;
		if ( kStep++ % 10 == 0 )
			Eps_In.PH(pFlow_i);
		double ksi = 0.1;
		Eps = (Eps + ksi * Eps_In.E) / (1. + ksi);
		if(Eps < 1e-5)
			Eps = 0.0;
		if(Eps > 0.9999)
			Eps = 1.0;
		M = Mw(pFlow_i->pComp);	
		if(Eps == 0.0)
		{
			double Ro_l = Ro_liq(Eps_In.T, pFlow_i->pComp);
			if(Ro_l == 0.0)
				Ro_l = 100.0;
			Ro = Ro_l;
		}
		else if(Eps == 1.0)
		{
			double Ro_g = pFlow_i->P * M / ( RP * (Eps_In.T + TK));
			Ro = Ro_g;
		}
		else
		{
			double Ro_l = Ro_liq(Eps_In.T, pFlow_i->pComp);  // По идее нужно считать плотность по составу жидкой части, а не по полному
			Eps_In.Calc_Mw();
			double Ro_g = pFlow_i->P * Eps_In.Mw_gas / ( RP * (Eps_In.T + TK));		
			Ro = (Eps_In.Mw_liq * (1. - Eps) + Eps_In.Mw_gas * Eps) * Ro_l * Ro_g / (Eps_In.Mw_liq * (1. - Eps) * Ro_g + Eps_In.Mw_gas * Eps * Ro_l);
		}
		//////////////////////////////////////////////////////////////////////////
		_Flow_Work_kg = _F_mol * M;
		if(Ro)
			_Flow_Work_m3 = _Flow_Work_kg / Ro; 
		_Flow_Work_nm3 = _F_mol * M3;
	}
	Положение = Position;
}
