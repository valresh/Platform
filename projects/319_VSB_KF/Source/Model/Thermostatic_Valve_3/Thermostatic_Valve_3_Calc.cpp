#include "stdafx.h"
#include "Thermostatic_Valve_3.h"
#include "Err.h"
#include "CommProc.h"

void CThermostatic_Valve_3::Control(double dt)
{
	En1 = En;
	En = PV - Task;
	dt *= 3600.;
	double Kp = 100. / P;
	double dEn = En - En1;
	double dMV = Kp * ( dEn + dt / I * En );
	//
	if ( Inverse )
		dMV = -dMV;
	Position += dMV;	
	if(Position < 0.0)
		Position = 0.0;
	else if(Position > 100.0)
		Position = 100.0;
};


void CThermostatic_Valve_3::Calc(double dt)
{
	SET_BP BreakPoint;
	double H = 0;
	double Flow = 0;
	memset(pCompOut->Cmol, 0, sizeof(pCompOut->Cmol));
	for(short i = 0; i < IO_COUNT; i++)
	{
		if(!pFlow[i])
			return;
		if(pFlow[i]->Flow_mol > 0.0)
		{
			Flow += pFlow[i]->Flow_mol;
			H += pFlow[i]->h_Flow * pFlow[i]->Flow_mol;
			CComp * pCompIn = pFlow[i]->Get_Comp();
			if ( pCompIn == NULL )
				continue;
			for(short c = 0; c < LAST_GAS; c++)
				pCompOut->Cmol[c] += pFlow[i]->Flow_mol * pCompIn->Cmol[c];
		}
	}
	for(short c = 0; c < LAST_GAS; c++)
	{
    if ( Flow > 0. )
		  pCompOut->Cmol[c] /= Flow;
		pCompOut->IsComp[c] = pCompOut->Cmol[c] > 0.0;
	}
	for(short i = 0; i < IO_COUNT; i++)
	{
		if(pFlow[i]->Flow_mol < 0.0)
		{
    if ( Flow > 0. )
			pFlow[i]->h_Flow = H / Flow;
			pFlow[i]->pComp = pCompOut;
			pFlow[i]->ID_COMP = ID_CompOut;
		}			
	}
	state_o.PH(pFlow[IO_O]);
	PV = state_o.T;
	Control(dt);
	
}
