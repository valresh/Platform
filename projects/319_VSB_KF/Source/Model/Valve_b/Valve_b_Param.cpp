#include "stdafx.h"
#include "Valve_b.h"
#include "Err.h"
#include "CommProc.h"

_W_Valve_b::_W_Valve_b()
{
	memset ( this, 0, sizeof (_W_Valve_b));
}


int CValve_b::GetParams( char * )
{	
	return 0;
}

#define IS_UPDATE(_Param) Param.Addr == &_Param

int CValve_b::UpdateParam( CParams & Param )
{
	if(Gas)
	{
		if(IS_UPDATE(dP_Nom_Gas) || IS_UPDATE(F_Nom_Gas) || IS_UPDATE(Gas))
		{
			Omega_max = F_Nom_Gas / dP_Nom_Gas; 
		}			
		if(IS_UPDATE(Calc_Gas))
		{
			F_Nom_Gas = fabs(_Flow_Work_nm3);
			dP_Nom_Liq = fabs(_P_In - _P_Out);
			P_Nom_Gas = _P_In;
			Mw_Nom_Gas = M;
			T_Nom_Gas = Eps_In.T;
			Omega_max = F_Nom_Gas / dP_Nom_Gas;
			Calc_Gas = false;
		}
	}
	else
	{
		if(IS_UPDATE(dP_Nom_Liq) || IS_UPDATE(F_Nom_Liq) || 
			IS_UPDATE(Ro_Nom_Liq) || IS_UPDATE(Mw_Nom_Liq) || IS_UPDATE(Gas))
		{
			Omega_max = F_Nom_Liq / dP_Nom_Liq;  // А в конструкторе формула другая
		}
		if(IS_UPDATE(Calc_Liq))
		{
			F_Nom_Liq = fabs(_Flow_Work_m3);
			dP_Nom_Liq = fabs(_P_In - _P_Out);
			Ro_Nom_Liq = Ro;
			Omega_max = F_Nom_Liq / dP_Nom_Liq;
			Calc_Liq = false;
			//Omega_max = Omega;
		}
	}
	return 0;
}

#undef IS_UPDATE

//#include "ShowParams.h"

int CValve_b::ShowParams( DWORD DataType, struct CShowData * pSD )
{
	if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
	{
		if (Position != 0. )
			Position = 0.0;
		else
			Position = 100.;
	}
	return 0;
}

COLORREF CValve_b::ColZadv(double _Position)
{
	if ( _Position <= 0. )
		return RGB( 255, 0, 0 );
	if ( _Position <= 10. )
		return RGB( 255, 150, 0 );
	if ( _Position <= 50. )
		return RGB( 255, 255, 0 );
	return RGB( 0, 255, 0 );
}