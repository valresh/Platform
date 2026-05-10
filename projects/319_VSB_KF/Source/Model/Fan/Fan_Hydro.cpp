#include "stdafx.h"
#include "Fan.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"
#include "TestNodes.h"

NODE_LIST

NODE_IN(1, 0, "i","Вход")
NODE_OUT(1, 1,"o","Выход")

NODE_IN(2, 0,"in_oil","Вход смазки")
NODE_OUT(2, 1,"out_oil","Выход смазки")

MOTOR_IN ( 3, "i_motor", "Cоединение c валом")
MOTOR_OUT ( 4, "o_motor", "Cоединение c датчиком")


//
END_LIST

//
/*
NODE_IN(1,"i","Вход")
NODE_OUT(1,"o","Выход")
//
NODE_IN(1,"in_oil","Вход смазки")
NODE_OUT(1,"out_oil","Выход смазки")
*/

#undef IF
#define IF(Node) if ( lstrcmp ( Node, Name ) == 0 )
extern "C"
EXP int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
	int kIO = 0;
	int kOil = 0;
	int kErr = 0;
	int kSens = 0;
	int nGroup = 0;
	for ( int n = 0; n < kNodes; n++ )
	{
		CObjectPoint * pOP = ppNodes[n];
		pOP->Type = eГидравлика;
		char * Name = pOP->PntName;
		IF("i")
		{ kIO++; continue; };
		IF("o")
		{ kIO++; continue; };
		IF("in_oil")
		{ kOil++; continue; };
		IF("out_oil")
		{ kOil++; continue; };
		IF("o_motor")
			continue;
		IF("i_motor")
			continue;
		ModMsg ( "У вентилятора '%s' ошибочный вход '%s'", ObjName, Name ), kErr++;
	}
	if ( kIO != 2 )
		ModMsg ( "У вентилятора '%s' неправильно заданы вход/выход", ObjName ), kErr++;
	if ( kOil != 2 && kOil != 0 )
		ModMsg ( "У вентилятора '%s' неправильно заданы вход/выход смазки", ObjName ), kErr++;
	return kErr;
}

int CFan::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::TestNodes( ObjName, kNodes, ppNodes );
	if ( Res )
		return Res;
	for ( int n = 0; n < kNodes; n++ )
	{
		CObjectPoint * pOP = ppNodes[n];
		pOP->Type = eГидравлика;
		char * Name = pOP->PntName;
		IF("i")
		{ 
			pOP->PntGroup = 1;  
			pOP->NumbInGroup = 0;
			pOP->IO = '+';
			pOP->Type = eГидравлика;
			continue; 
		};
		IF("o")
		{
			pOP->PntGroup = 1;  
			pOP->NumbInGroup = 1;
			pOP->IO = '-';
			pOP->Type = eГидравлика;
			continue; 
		};
		IF("in_oil")
		{ 
			pOP->PntGroup = 2;  
			pOP->NumbInGroup = 0;
			pOP->IO = '+';
			pOP->Type = eГидравлика;
			continue; 
		};
		IF("out_oil")
		{
			pOP->PntGroup = 2;  
			pOP->NumbInGroup = 1;
			pOP->IO = '-';
			pOP->Type = eГидравлика;
			continue; 
		};
		IF("i_motor")
		{
			pOP->Type = eMotor;
			pOP->PntGroup = 3;
			pOP->NumbInGroup = 0;
			pOP->IO = '+';
			continue;
		}
		IF("o_motor")
		{
			pOP->Type = eMotor;
			pOP->PntGroup = 4;
			pOP->NumbInGroup = 0;
			pOP->IO = '-';
			continue;
		}
		ASS(0)
	}
	return 0;
}

#include "SetMatr.h"

// Q(F) = A * F + C      F - моль
void CFan::Спад ( double Flow_mol, double & A, double & C )
{
	double Q_m3 = Qmax * 1e3;
	double M_Flow = Q_m3 * Oborot;
	if ( M_Flow < 1. )
	{
		A = 0.;
		C = 1.;
		return;
	}
	double Flow_M3 = Flow_mol * To_m3_in;
	// 
	if ( Flow_M3 < M_Flow ) // Везде M3
	{
		A = 0.;
		C = 1.;
		return;
	}
	double Surge_stock = Flow_M3 / M_Flow;
	double F = Surge_stock - 1.;
	while ( F < Поток[pos_Х] && pos_Х > 0 )
		pos_Х--;
	if ( pos_Х >= kПХ )
		pos_Х = kПХ - 1;
	while ( F > Поток[pos_Х+1] && pos_Х < kПХ - 2 )
		pos_Х++;
	double F0 = Поток[pos_Х];
	double F1 = Поток[pos_Х+1];
	double P0 = Характеристика[pos_Х];
	double P1 = Характеристика[pos_Х+1];
	// Q = P0 + ( F - F0 ) * ( P1 - P0 ) / ( F1 - F0 )
	// Q = a * F + c
	// F = Fcompr * M3 / M_Flow - 1.
	// Q = a * Fcompr * M3 / M_Flow + c - a
	double N = ( P1 - P0 ) / ( F1 - F0 );
	double a = N;
	double c = P0 - F0 * N;
	A = a * To_m3_in / M_Flow;      //    A ~ .../mol
	C = c - a;
}

void CFan::СпадРаб( )
{
	double Q_m3 = Qmax * 1e3;
	double M_Flow = Q_m3;
	double Surge_stock = Qwork * 1e3 / M_Flow;
	double F = Surge_stock - 1.;
	int pos_X = 0;
	while ( F < Поток[pos_Х] && pos_Х > 0 )
		pos_Х--;
	if ( pos_Х >= kПХ )
		pos_Х = kПХ - 1;
	while ( F > Поток[pos_Х+1] && pos_Х < kПХ - 2 )
		pos_Х++;
	double F0 = Поток[pos_Х];
	double F1 = Поток[pos_Х+1];
	double P0 = Характеристика[pos_Х];
	double P1 = Характеристика[pos_Х+1];
	_Hwork = P0 + ( F - F0 )* ( P1 - P0 ) / ( F1 - F0 );
}

void CFan::SetРаб( )
{
	double Q_m3 = Qmax * 1e3;
	double M_Flow = Q_m3;
	double Surge_stock = Qwork * 1e3 / M_Flow;
	double F = Surge_stock - 1.;
	if ( F < 0.1 )
		F = 0.1;
	if ( F > 10. )
		F = 10.;
	double Q = _Hwork;
	if ( Q < 0.1 )
		Q = 0.1;
	if ( Q > 0.9 )
		Q = 0.9;
	// Q = 1 - A * X * X
	double A = ( 1. - Q ) / ( F * F );
	double Xmax = sqrt ( 1. / A );
	//
	bool SetNom = true; 
	for ( int n = 0; n < kПХ; n++ )
	{
		Поток[n] = 0.2 * n * Xmax;
		if ( Поток[n] > F && SetNom )
		{
			Поток[n] = F;
			SetNom = false;
		}
		Характеристика[n] = 1. - A * Поток[n] * Поток[n];
	}
}

int CFan::SetMatr( void * pExternals, int PntGroup,  double dt, 
				  double A[/* kExkV */], double B[ /*kE*/ ] )        
{
	SET_BP BreakPoint;
	switch ( PntGroup )
	{
	case 1:
		{
			double Omega = 0.;//1e-6;
			//      if ( Flow_mol < 0. )
			//        Omega = Omega_Inv;
			double * Eq = A;
			if ( On )
			{
				H_real.Set ( dP_work );
				Eq[P_1] = 1.;
				Eq[P_0] = -1.;
				Спад ( Flow_mol, aF, cF );
				Eq[F_0] = -dP_work * aF + Omega;
				B[0] = dP_work * cF;   // P1 - P0 - dP_work * ( A * F + C )
			}
			else
			{
				Eq[P_0] = Omega;
				Eq[P_1] = -Omega;
				Eq[F_0] = -1.;
			}
			Eq += 4;
			Eq[F_0] = -1.;
			Eq[F_1] = 1.;
			return 0;
		}
		return 0;
	case 2:
		{
			ASS(kOilSens>0)
				return Смазка->SetMatr( pExternals, 1,  dt, A, B );
		}
	default:
		ASS(0)
			break;
	}
	return 0;
}

#include "SetVar.h"

int CFan::SetVar( void * pExternals, int PntGroup, double dt, 
				 double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	SET_BP BreakPoint;
	switch ( PntGroup )
	{
	case 1:
		{
			pFlow_in = (CFlow*)pVar[0];
			pFlow_out = (CFlow*)pVar[1];
			double dP = Var[P_1] - Var[P_0];
			double dP2 = pFlow_out->P - pFlow_in->P;
			if ( pFlow_in->Flow_mol >= 0. )
				Х_ка = aF * pFlow_in->Flow_mol + cF;
			else
				Х_ка = 0.;
			H_fact.Set ( dP_work * Х_ка ); 
		}
		return 0;
	case 2:
		{
			return Смазка->SetVar( pExternals, 1, dt, Var,	pVar );
		}
	case 3:
		{
			//pMotor = (CMotorData *)pVar[0];
			break;
		}
	case 4:
		{
			break;
		}
	default:
		break;
	}
	return 0;
}

