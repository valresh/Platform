#include "stdafx.h"
#include "Dynamic.h"
#include "Err.h"
#include "CommProc.h"



void CDynamic::Calc(double dt)
{
	SET_BP BreakPoint;
	if(!pFlow_i || !pFlow_o)
		return;
	
	//pFlow_i->pComp = pFlow_i->Get_Comp();
	if(F_mol > 0.0)
	{
		pFlow_i->pComp = pFlow_i->Get_Comp();
		if(!pFlow_i->pComp)
			return;
		*pFlow_o = *pFlow_i;
		Eps.PH(p_i, pFlow_i->h_Flow, pFlow_i->pComp);
		Eps.Calc_Mw();	
		ro_l = Ro_liq(Eps.T, pFlow_i->pComp);
	}
	else if(F_mol < 0.0)
	{
		pFlow_o->pComp = pFlow_o->Get_Comp();
		if(!pFlow_o->pComp)
			return;
		*pFlow_i = *pFlow_o;
		Eps.PH(p_o, pFlow_o->h_Flow, pFlow_o->pComp);
		Eps.Calc_Mw();
		ro_l = Ro_liq(Eps.T, pFlow_o->pComp);
	}
	
	T = Eps.T;
	M_l = Eps.Mw_liq;
	M_g = Eps.Mw_gas;	
	if(!bH)
	{
		const double ksi = 0.1;
		E = (E + ksi * Eps.E) / (1. + ksi);
		if(E < 1e-7)
			E = 0.0;
		if(E > 1.0 - 1e-7)
			E = 1.0;
		E = Eps.E;
	}
//////////////////////////////////////////////////////////////////////////
	if(p_i > p_o)
			bRest = false;
//////////////////////////////////////////////////////////////////////////
	Перепад = Head;
	power =  Head * AtmToPa * _Flow_Work_m3 / HourToSec / 1000.0;
	double W =  power + W_friction;
	if (W_inert > 0.0)
	{
		double Ksi = (fabs(log((p_nom * Q_nom))) / W_inert) * dt * 3600.0;
		Мощность = (Мощность + Ksi * W) / (1 + Ksi);
	}
	else
		Мощность = W;
}
