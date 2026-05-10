#include "stdafx.h"
#include "Volumetric.h"
#include "Err.h"
#include "CommProc.h"


void CVolumetric::Calc(double dt)
{
	SET_BP BreakPoint;
	CComp* pComp_i = pFlow_i->Get_Comp();
	if(!pComp_i)
		return;
	//if(F_mol < 0.0)
	//	return; // Если есть 2 параллельных насоса - у второго поток всегда <= 0
	*pFlow_o = *pFlow_i;
	Eps.PH(pFlow_i);
	double Ro_i = Ro_liq(20.0, pComp_i);
	double Mw_i = Mw(pComp_i);
	To_kg = Mw_i;
	// 	if(Ro_i && Mw_i)
	// 		To_m3 = Mw_i / Ro_i;		
	// 	else 
	// 		To_m3 = H2O_To_m3;
	_Flow_Work_m3 = F_mol * To_m3;
	_Flow_Work_kg = F_mol * Mw_i;
	CPump_b::Calc();
	Eps.PH(pFlow_i);
	double Ro;
	if(Eps.E == 0.0)
	{
		double Ro_l = Ro_liq(Eps.T, pFlow_i->pComp);
		Ro = Ro_l;
	}
	else if(Eps.E == 1.0)
	{
		double Ro_g = pFlow_i->P * Mw_i / ( RP * (Eps.T + TK));
		Ro = Ro_g;
	}
	else
	{
		double Ro_l = Ro_liq(Eps.T, pFlow_i->pComp);  // По идее нужно считать плотность по составу жидкой части, а не по полному
		Eps.Calc_Mw();
		double Ro_g = pFlow_i->P * Eps.Mw_gas / ( RP * (Eps.T + TK));		
		if(Eps.T < 0.0)
			Ro = 1000.0;
		else
			Ro = (Eps.Mw_liq * (1. - Eps.E) + Eps.Mw_gas * Eps.E) * Ro_l * Ro_g / (Eps.Mw_liq * (1. - Eps.E) * Ro_g + Eps.Mw_gas * Eps.E * Ro_l);
	}
	if(Ro > 0)
		To_m3 = Mw_i / Ro;
}
