#include "stdafx.h"
#include "Dynamic.h"
#include "Err.h"
#include "CommProc.h"
#include "SetMatr.h"
#include "Work.h"

#undef IF
#define IF(Node) if ( lstrcmp ( Node, Name ) == 0 )

void CDynamic::CalcCharacteristic()
{
	//_D = H_max / ro_nom;
	//_A = (H_nom * Q_0 - Q_0 * _D * ro_nom + Q_nom * _D * ro_nom) / (Q_0 * pow(Q_nom,3.) * ro_nom - pow(Q_0,3.) * Q_nom * ro_nom);
	double H_max = p_max * AtmToPa / (ro_nom * g);
	double H_nom = p_nom * AtmToPa / (ro_nom * g);
	a = H_max;
	b = (H_max - H_nom) / pow(Q_nom,2.);
	Q_0 = sqrt(a / b);
	ro = 1.23;
	//_C = Q_0 / (ro_nom * (_A * pow(Q_0,3.) + _D)) - omega;
}
int CDynamic::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	return CPump_b::TestNodes(kNodes, ppNodes);
}

double sign(double Val) 
{
	if (Val == 0.)  return 0.0;
	if (Val > 0.)  return 1.0;
	else return -1.0;
}

int CDynamic::SetMatr( void * pExternals, int PntGroup,  double dt, 
					  double A[/* kExkV */], double B[ /*kE*/ ] )
{
	SET_BP BreakPoint;
	double * Eq = A;
	switch ( PntGroup )
	{
	case 1:
		{			
			
			
			double Speed = (*pMotor->pRelOborot);
			if(F_mol < 0.0)
			{
				ro = CPump_b::CalcRo(E, ro_l, M_l, p_o, T, M_g);							
				if(a * ro * g / AtmToPa * pow(Speed,2.) > p_o )
				{
					bRest = true;
				}
			}
			else if(F_mol > 0.0)
			{
				ro = CPump_b::CalcRo(E, ro_l, M_l, p_i, T, M_g);
			}
			if(bRest)
			{
				A[F_0] = 1.0;
				B[0] = 0.0;
				Eq += 4;
				Eq[F_0] = 1.0;
				Eq[F_1] = -1.0;
				break;
			}
			double M = E * M_g + (1.0 - E) * M_l;
			if(M == 0)
				M = 15;
			double H = (a * pow(Speed,2.) - b * sign(F_mol) * pow(((F_mol * M) / ro),2.)) * ro * g / AtmToPa;	
			if(H > p_o - p_i_min)
			{

				double ro_min = 0;
				double ro_max = ro_l > 0.0 ? ro_l : 5000.0;
				double y;
				do 
				{
					ro = (ro_min + ro_max) / 2.0;					
					y = (a * pow(Speed,2.) - b * sign(F_mol) * pow(((F_mol * M) / ro),2.)) * (ro * g) / AtmToPa - (p_o - p_i_min);
					if(y > 0.0)
						ro_max = ro;
					else
						ro_min = ro;
				} while (fabs(ro_min - ro_max) > 1e-3);				
				H = (a * pow(Speed,2.) - b * sign(F_mol) * pow(((F_mol * M) / ro),2.)) * ro * g / AtmToPa;
				E = (M_l * p_i_min - (M_l * p_i_min * ro_l) / ro) / (M_l * p_i_min - RP * ro_l * (T + TK));
				bH = true;
			}
			else
				bH = false;
			double dHdQ = -((2. * F_mol * pow(M,2.) * sign(F_mol)) / (pow(ro,2.) / b)) * ro * g / AtmToPa;
			double _dHdQ = 0.0;
			if(fabs(F_mol) > 1e-5)
				_dHdQ = H / (F_mol - (Speed * sqrt(a) * ro) / (sqrt(b) * M));			
			if(_dHdQ == 0.0)
				_dHdQ = 1e-7;
			double fabs_dHdQ = fabs(_dHdQ);
			if(fabs(dHdQ) < fabs_dHdQ)
				dHdQ = sign(dHdQ) * fabs_dHdQ;
			static double k = 1;
			dHdQ *= k;
			if(dHdQ == 0.0)
				dHdQ = _dHdQ;		
			A[F_0] = 1.0;				
			A[P_0] = 1.0 / dHdQ;
			A[P_1] = -1.0 / dHdQ;
			B[0] = F_mol - H / dHdQ;

			Eq += 4;
			Eq[F_0] = 1.0;
			Eq[F_1] = -1.0;
			break;
		}
	case 2:
		{
			MATR_OMEGA(pCoolObj->Omega)
		}
		break;
	default:
		ASS(0)
			break;
	}
	return 0;
}

#include "SetVar.h"

int CDynamic::SetVar( void * pExternals, int PntGroup, double dt, 
						double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	SET_BP BreakPoint;
	switch ( PntGroup )
	{
	case 1:
		{	
			p_i = Var[P_0];
			if(p_i < p_i_min)
				p_i = p_i_min;
			p_o = Var[P_1];
			_Flow_Work_kg = F_mol * (E * M_g + (1. - E) * M_l);
			_Flow_Work_m3 = _Flow_Work_kg / ro;
		}
	} 	
	return CPump_b::SetVar(pExternals, PntGroup, dt, Var, pVar);
}

