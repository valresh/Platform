#include "stdafx.h"
#include "CV.h"
#include "Err.h"
#include "CommProc.h"
#include "SetMatr.h"
#include "Work.h"

int VA_StdTestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes );

int CCV::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::VA_StdTestNodes(ObjName, kNodes, ppNodes);
	if(Res)
		return Res;
	return CValve_A_b::TestNodes(kNodes, ppNodes);
}

double CCV::Calc_Omega(Characteristic eCharacteristic, double _Position)
{
	double _Omega = 0.0;
	double Omega_Reg;
	Omega_Reg = CValve_A_b::Calc_Omega((Characteristic)Performance, _Position);
    if ( strstr ( ObjName, "PV-026"))
        KKK();
	if(Omega_Reg != 0.0)
	{
		if(Strapping & STRAPPING_IN &&  Strapping & STRAPPING_OUT)
		{		
			double Omega_Zadv_In = CValve_A_b::Calc_Omega(Линейная_Характеристика, Zadv_In) * K_IO;
			double Omega_Zadv_Out = CValve_A_b::Calc_Omega(Линейная_Характеристика, Zadv_Out) * K_IO;
			if(Omega_Zadv_In != 0.0 && Omega_Zadv_Out != 0.0)
				_Omega = (Omega_Zadv_In * Omega_Reg * Omega_Zadv_Out) / (Omega_Reg * Omega_Zadv_Out + Omega_Zadv_In * Omega_Zadv_Out + Omega_Zadv_In * Omega_Reg);
		}
		else if(Strapping & STRAPPING_IN)
		{
			double Omega_Zadv_In = CValve_A_b::Calc_Omega(Линейная_Характеристика, Zadv_In) * K_IO;
			if(Omega_Zadv_In != 0.0)
				_Omega = (Omega_Reg * Omega_Zadv_In) / (Omega_Reg + Omega_Zadv_In);
		}
		else if(Strapping & STRAPPING_OUT)
		{
			double Omega_Zadv_Out = CValve_A_b::Calc_Omega(Линейная_Характеристика, Zadv_Out) * K_IO;
			if(Omega_Zadv_Out != 0.0)
				_Omega = (Omega_Reg * Omega_Zadv_Out) / (Omega_Reg + Omega_Zadv_Out);
		}
		else
			_Omega = Omega_Reg;
	}
	if(Strapping & STRAPPING_BAYPASS)
	{
		double Omega_Zadv_Bypass = CValve_A_b::Calc_Omega(Равнопроцентная_Характеристика, Zadv_Bypass) * K_BP;
		_Omega += Omega_Zadv_Bypass;
	}	
	return _Omega;
}
