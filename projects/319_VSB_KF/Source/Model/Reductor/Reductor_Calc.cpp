#include "stdafx.h"
#include "Reductor.h"
#include "Err.h"
#include "CommProc.h"


void CReductor::Calc( double dt )
{
	SET_BP BreakPoint;
	if(Mest)
		P_Z = P_Pult * 0.01 * P_Max;
	if ( IsCtrlGas && pGas )
	{
		P_Gas = pGas->P - 1.;
		P_Z = P_Gas + dP_Gas_In;
		_P_out_Z = false;
	}
	if(Заданное_давление.Use())
		P_Z = Заданное_давление;
	if(_dP)
		P_rab = P_in - P_out;
	else if(_P_out_Z)
		P_rab = _P_out;		
	else
		P_rab = _P_in;
	P_rab *= Scale( MesZ );
}
