#include "stdafx.h"
#include "Valve_b.h"
#include "Err.h"
#include "CommProc.h"
#include "SetMatr.h"
#include "TestNodes.h"
#include "HydroStruct.h"
//#include "Valve_b_ACS.h"

int CValve_b::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{	
	Atmosphere = (kNodes == 1);
	return 0;
}


double CValve_b::Calc_Omega(Characteristic eCharacteristic, double _Position)
{
	if(_Position == 0.0)
		return 0.0;
	double Omega_min = 0.0;
	double _Omega = Omega_min;
	if(Reverse && _P_In - _P_Out < 0.0)
		eCharacteristic = Характеристика_Обратного_Клапана;
	switch(eCharacteristic)
	{
	case Равнопроцентная_Характеристика:
		{
			Omega_min = 0.0183 * Omega_max;
			if(_Position < min_procent)
			{
				double A = Omega_min * exp(min_procent / 25.0) / pow(min_procent, stepen);
				_Omega = A * pow(_Position, stepen);
				Omega_min = 0.0;
			}		
			else
				_Omega =  Omega_min * exp(4.0 / 100.0 * _Position);
			break;
		}
	case Линейная_Характеристика:
		{
			_Omega = (Omega_max - Omega_min) / 100.0 * _Position  + Omega_min;
			break;
		}
	case Параболическая_Характеристика:
		{
			_Omega = (Omega_max - Omega_min) / (100.0 * 100.0) * _Position * _Position  + Omega_min; 
			break;
		}
	case Характеристика_Обратного_Клапана:
		{
			_Omega = Omega_Reverse;
			break;
		}
	}
	if(_Omega <  Omega_min)
		_Omega = Omega_min;
	return _Omega;
}

int CValve_b::SetMatr( void * pExternals, int PntGroup,  double dt, 
					  double A[/* kExkV */], double B[ /*kE*/ ] )
{
	SET_BP BreakPoint;
	switch ( PntGroup )
	{
	case 1:
		{
			double * Eq = A;
// 			if ( Reg_FlowDirect_nm3.Use )
// 			{
// 				Eq[F_0] = 1.0;
// 				B[0] = Reg_FlowDirect_nm3 * iM3;
// 				Eq += 4;
// 				Eq[F_0] = 1.0;
// 				Eq[F_1] = -1.0;	
// 			}
			Omega = Calc_Omega((Characteristic)Performance, Position);
			if(Gas && Pin > 0)		
				K_F = M3 * sqrt((T_Nom_Gas + TK) / (Eps_In.T + TK) * Pin / (P_Nom_Gas + 1.0) * M / Mw_Nom_Gas );
			else
			{
				if(Ro > 0.0)
					K_F = M /Ro * sqrt(Ro / Ro_Nom_Liq);
				else
					K_F = 0.0;
			}
			if(K_F == 0.0)
				K_F = 1e-7;
			if(K_Omega != MAX_K_OMEGA && K_Omega != 1.0 / MAX_K_OMEGA)
			{
				if(Reg_dP_Zad.Use)
				{
					Eq[P_0] = 1.0;
					Eq[P_1] = -1.0;
					Eq[F_0] = -1e-9;
					B[0] = Reg_dP_Zad;
					Eq += 4;
					Eq[F_0] = 1.0;
					Eq[F_1] = -1.0;
					calcConflict |= CALC_SET_MATR;
					return 0;
				}
				else if(Reg_P_in_Zad.Use)
				{
					Eq[P_0] = 1.0;
					Eq[F_0] = -1e-9;
					B[0] = Reg_P_in_Zad + 1;
					Eq += 4;
					Eq[F_0] = 1.0;
					Eq[F_1] = -1.0;
					calcConflict |= CALC_SET_MATR;
					return 0;
				}
				else if(Reg_P_out_Zad.Use)
				{
					Eq[P_1] = 1.0;
					Eq[F_0] = -1e-9;
					B[0] = Reg_P_out_Zad + 1;
					Eq += 4;
					Eq[F_0] = 1.0;
					Eq[F_1] = -1.0;
					calcConflict |= CALC_SET_MATR;
					return 0;
				}
				else if ( Reg_FlowZad_mol.Use )
				{
					double F_z;
					if(_P_In > _P_Out)
						F_z = Reg_FlowZad_mol;
					else
						F_z = -Reg_FlowZad_mol;
					Eq[P_0] = -1e-9;
					Eq[P_1] = 1e-9;
					Eq[F_0] = 1.0;
					B[0] = F_z;
					Eq += 4;
					Eq[F_0] = 1.0;
					Eq[F_1] = -1.0;
					calcConflict |= CALC_SET_MATR;
					return 0;
				}
				else if ( Reg_FlowZad_kg.Use )
				{
					Eq[P_0] = -1e-9;
					Eq[P_1] = 1e-9;
					Eq[F_0] = 1.0;
					B[0] = Reg_FlowZad_kg / M;
					Eq += 4;
					Eq[F_0] = 1.0;
					Eq[F_1] = -1.0;
					calcConflict |= CALC_SET_MATR;
					return 0;
				}
				else if ( Reg_FlowZad_m3.Use )
				{
					Eq[P_0] = -1e-9;
					Eq[P_1] = 1e-9;
					Eq[F_0] = 1.0;
					B[0] = Reg_FlowZad_m3 * Ro / M;
					Eq += 4;
					Eq[F_0] = 1.0;
					Eq[F_1] = -1.0;
					calcConflict |= CALC_SET_MATR;
					return 0;
				}
				else if ( Reg_FlowZad_nm3.Use )
				{
					Eq[P_0] = -1e-9;
					Eq[P_1] = 1e-9;
					Eq[F_0] = 1.0;
					B[0] = Reg_FlowZad_nm3 * iM3;
					Eq += 4;
					Eq[F_0] = 1.0;
					Eq[F_1] = -1.0;	
					calcConflict |= CALC_SET_MATR;
					return 0;
				}

			}
			calcConflict = CALC_NO;
			double Leak_Omega = 0.;
			if ( Leak > 0.)
				Leak_Omega = Calc_Omega((Characteristic)Performance, 100.0) * 0.01 * Leak;
			if(Omega_Depressurization != 0.0 && Omega == 0.0)
				Omega = 1e-7;
			if ( Omega == 0.)
			{
				if(Atmosphere)
				{
					Eq[F_0] = 1.0;
					return 0;
				}
				else if(Leak_Omega != 0.0)
				{
					Leak_Omega *= K_Omega * K_def;

					Eq[P_0] = -Leak_Omega;
					Eq[P_1] = Leak_Omega;
					Eq[F_1] = K_F;

					Eq += 4;
					Eq[F_0] = 1.0;
					Eq[F_1] = -1.0;	
					return 0;
				}
				else
				{
					if ( fabs ( Pout ) > 1000. )
						Pout = 1.;
					if ( fabs ( Pin ) > 1000. )
						Pin = 1;
					double Ksi = 1.0;
					double O = 1.;
					Eq[P_0] = O;
					Eq[F_0] = -Ksi;
					B[0] = O * Pin;
					Eq += 4;
					Eq[P_1] = O;
					Eq[F_1] = Ksi;
					B[1] = O * Pout;
					return 0;
				}

			}
			if(Omega < Leak_Omega)
				Omega = Leak_Omega;
			Omega *= K_Omega * K_def;
			if(Atmosphere)
			{
				Eq[P_0] = Omega;
				Eq[F_0] = -1.0;
				FINITE(Eq[F_0])
					B[0] = 1.0 * Omega;
				return 0;
			}
			Eq[P_0] = -Omega;
			Eq[P_1] = Omega;
			Eq[F_1] = K_F;
			FINITE(Eq[F_0]);
			Eq += 4;
			Eq[P_0] = -Omega_Depressurization / K_F;
			Eq[F_0] = 1.0;
			Eq[F_1] = -1.0;	
			B[1] = -Omega_Depressurization / K_F;
		}		
		break;
	default:
		ASS(0);
		break;
	}
	return 0;
}


int CValve_b::SetVar( void * pExternals, int PntGroup, double dt, 
					   double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	SET_BP BreakPoint;
	switch ( PntGroup )
	{
	case 1:
		{
			if(Atmosphere)
			{
				_P_In = Var[P_0] - 1.;
				if(fabs(_P_In) < 1e-4)
					_P_In = 0.0;
				_Differential = fabs(1.0 - _P_In);
				if(_Differential < 1e-4)
					_Differential = 0.0;
				pFlow_i = (CFlow*)pVar[0];
				if(_F_mol < 0.0)
					if(pAtmComp)
					{
						//						pFlow_i->pComp = pAtmComp;
						pFlow_i->ID_COMP = ID_COMP_ATM;
					}
					return 0;

			}
			//Pin = Var[P_0];
			//Pout = Var[P_1];
			double Fin = Var[F_0];
			double Fout = Var[F_1];
			double dP_in = 0.;
			double dP_out = 0.; 
			bool bReg = Reg_dP_Zad.Use || Reg_P_out_Zad.Use || Reg_P_in_Zad.Use || 
				Reg_FlowZad_mol.Use || Reg_FlowZad_m3.Use || Reg_FlowZad_nm3.Use ||
				Reg_FlowZad_kg.Use;
			if ( Omega == 0. && !bReg)
			{
				double Ksi = 1.0;
				dP_in = LimdX ( Ksi * Fin, 1.0 );
				Pin += dP_in;
				dP_out = LimdX ( Ksi * Fout, 1.0 );
				Pout -= dP_out;
			}
			else
			{
				Pin = Var[P_0];
				Pout = Var[P_1];
			}
			_P_In = Pin - 1.;
			if(fabs(_P_In) < 1e-4)
				_P_In = 0.0;
			_P_Out = Pout - 1.;
			if(fabs(_P_Out) < 1e-4)
				_P_Out = 0.0;
			_Differential = fabs(_P_Out - _P_In);
			if(_Differential < 1e-4)
				_Differential = 0.0;
			_F_mol = Var[F_0];
			if(fabs(_F_mol) < 1e-4)
				_F_mol = 0.0;
			pFlow_i = (CFlow*)pVar[0];
			pFlow_o = (CFlow*)pVar[1];
			if(_F_mol >= 0.0)
				*pFlow_o = *pFlow_i;
			else
				*pFlow_i = *pFlow_o;
			if(bReg)
				calcConflict |= CALC_SET_VAR;
		}
		break;
	default:
		ASS(0);
		break;
	}
	return 0;
}
