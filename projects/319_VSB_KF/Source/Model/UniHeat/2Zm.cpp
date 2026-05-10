#include "stdafx.h"
#define DLL_UniHeat
#include "UniHeat.h"
#include "math.h"
#include "Err.h"
#include "SR.h"
#include "Data.h"
#include "SetData.h"

C2zmHeat::C2zmHeat( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain ),
In_0( "In_0", this ),
In_1( "In_1", this ),
In_2( "In_2", this ),
Out_0( "Out_0", this ),
Out_1( "Out_1", this ),
Out_2( "Out_2", this )
	{
	}

C2zmHeat_W::C2zmHeat_W()
	{
	memset ( this, 0, sizeof ( C2zmHeat_W ));
	k_Cp_0 = 0.01;
	k_Cp_12 = 0.01; 
	Tau_0 = 0.001; 
	Tau_12 = 0.001;
	Cp_0 = 10.;
	Cp_1 = 10.;
	Cp_2 = 10.;
	}


void C2zmHeat::Calc( double dt )
	{
	double Ps0 = pOut_0->P;
	*pOut_0 = *pIn_0;
	pOut_0->P = Ps0;
	double Ps1 = pOut_1->P;
	*pOut_1 = *pIn_1;
	pOut_1->P = Ps1;
	double Ps2 = pOut_2->P;
	*pOut_2 = *pIn_2;
	pOut_2->P = Ps2;
//
	CComp * pComp_0 = pIn_0->Get_Comp();
	CComp * pComp_1 = pIn_1->Get_Comp();
	CComp * pComp_2 = pIn_2->Get_Comp();
	if ( pComp_0 == NULL || ( pComp_1 == NULL && pComp_2 == NULL ))
		return ;
//
	h_in_0 = pIn_0->h_Flow;
	In_0.PH( pIn_0->P, h_in_0, pComp_0 );
	T_in_0 = In_0.T;
//
	h_in_1 = pIn_1->h_Flow;
	In_1.PH( pIn_1->P, h_in_1, pComp_1 );
	T_in_1 = In_1.T;
//
	h_in_2 = pIn_2->h_Flow;
	In_2.PH( pIn_2->P, h_in_2, pComp_2 );
	T_in_2 = In_2.T;
//
// 
	double F_0 = fabs ( pIn_0->Flow_mol );
  double Fcp_0 = F_0 * Cp_0;
	double F_1 = fabs ( pIn_1->Flow_mol );
  double Fcp_1 = F_1 * Cp_1;
	double F_2 = fabs ( pIn_2->Flow_mol );
  double Fcp_2 = F_2 * Cp_2;
  Lam_0 = 100.;
  if ( Lam_0 * Fcp_0 > KS )
    Lam_0 = KS / Fcp_0;
	else
		KS = Fcp_0 * Lam_0;
  Lam_1 = 100.;
  if ( Lam_1 * Fcp_1 > KS )
    Lam_1 = KS / Fcp_1;
  Lam_2 = 100.;
  if ( Lam_2 * Fcp_2 > KS )
    Lam_2 = KS / Fcp_2;
	if ( fabs ( Lam_1 - Lam_2	) < 0.001 * Lam_1 )
		{
// Убираем вырождение
		double L = 0.5 * ( Lam_1 + Lam_2 ); 
		double dL = 0.001 * L;
		Lam_1 = L + dL;
		Lam_2 = L - dL;
		Fcp_1 = KS / Lam_1;
		Fcp_2 = KS / Lam_2;
		}
///////////////////////////////////////////////
//	double a = 1.;
	double b = Lam_1 + Lam_2 - 2. * Lam_0;
	double c = Lam_1 * Lam_2 - Lam_0 * ( Lam_1 + Lam_2 );
	double det = b * b - 4. * c;
	ASS( det >= 0. )
	det = sqrt ( det );
	double x_0 = 0.;
	double x_1 = 0.5 * ( -b + det );
	double x_2 = 0.5 * ( -b - det );
	double e_1_0, e_1_1;
	double e_2_0, e_2_1;
	if ( x_1 > 0. )
		{
		e_1_0 = 1.;
		e_1_1 = exp ( -x_1 );
		}
	else
		{
		e_1_0 = exp ( x_1 );
		e_1_1 = 1.;
		}
	if ( x_2 > 0. )
		{
		e_2_0 = 1.;
		e_2_1 = exp ( -x_2 );
		}
	else
		{
		e_2_0 = exp ( x_2 );
		e_2_1 = 1.;
		}
// Ур-е для к-тов
  double M[3][3];
  double V[3];
// T0(0)
	M[0][0] = 1.;
	M[0][1] = e_1_0;
	M[0][2] = e_2_0;
	V[0] = T_in_0;
//
	M[1][0] = 1.;
	M[1][1] = Lam_1 * e_1_1 / ( Lam_1 + x_1 );
	M[1][2] = Lam_1 * e_2_1 / ( Lam_1 + x_2 );
	V[1] = T_in_1;
//
	M[2][0] = 1.;
	M[2][1] = Lam_2 * e_1_1 / ( Lam_2 + x_1 );
	M[2][2] = Lam_2 * e_2_1 / ( Lam_2 + x_2 );
	V[2] = T_in_2;
//
	double A[3];
	ASS(LinSolv ( 3, (double*)M, V, A ));
//
	T_out_0 = A[0] + A[1] * e_1_1 + A[2] * e_2_1; 
	T_out_1 = A[0] + A[1] * Lam_1 * e_1_0 / ( Lam_1 + x_1 ) + 
		               A[2] * Lam_1 * e_2_0 / ( Lam_1 + x_2 ); 
	T_out_2 = A[0] + A[1] * Lam_2 * e_1_0 / ( Lam_2 + x_1 ) + 
		               A[2] * Lam_2 * e_2_0 / ( Lam_2 + x_2 ); 
	T_in_0 = A[0] + A[1] * e_1_0 + A[2] * e_2_0; 
	T_in_1 = A[0] + A[1] * Lam_1 * e_1_1 / ( Lam_1 + x_1 ) + 
		              A[2] * Lam_1 * e_2_1 / ( Lam_1 + x_2 ); 
	T_in_2 = A[0] + A[1] * Lam_2 * e_1_1 / ( Lam_2 + x_1 ) + 
		              A[2] * Lam_2 * e_2_1 / ( Lam_2 + x_2 ); 
//
  double Q0 = Fcp_0 * fabs ( T_out_0 - T_in_0 ); // кДж/ч   .
//
  const double ToMVt = 1e-3/3600.;
	Q_dg = fabs ( Q0 ) * ToMVt; // 
	Q_cal = Q_dg * M_cal;
  double Q1 = Fcp_1 * fabs ( T_out_1 - T_in_1 );
  double Q2 = Fcp_2 * fabs ( T_out_2 - T_in_2 );
// Уточнение Cp
	Out_0.PT( pOut_0->P, T_out_0, pComp_0 );
	h_out_0 = Out_0.h;
	pOut_0->h_Flow = h_out_0;
  if ( Q0 > 100. )
    {
		double dH = F_0 * fabs ( h_out_0 - h_in_0 );
    double Ks = dH / Q0 - 1.;
    double kCps = LimdX ( k_Cp_0 * Ks, 0.1 );
    Cp_0 *= 1. + kCps;
    if ( Cp_0 < 10. )
      Cp_0 = 10.;
    if ( Cp_0 > 100000. )
      Cp_0 = 100000.;
    }
  if ( !_finite ( Cp_0 ) )
    Cp_0 = 10.;
//
	h_out_1 = h_in_1 + Cp_1 * ( T_out_1 - T_in_1 );
	Out_1.PH( pOut_1->P, h_out_1, pComp_1 );
	pOut_1->h_Flow = h_out_1;
//
	h_out_2 = h_in_2 + Cp_2 * ( T_out_2 - T_in_2 );
	Out_2.PH( pOut_2->P, h_out_2, pComp_2 );
	pOut_2->h_Flow = h_out_2;
///////////////////////////////////////////////
  if ( Q1 > 100. )
    {
		double dT = T_out_1 - Out_1.T;
    double kCps = LimdX ( k_Cp_12 * dT, 0.1 );
    Cp_1 *= 1. + kCps;
    if ( Cp_1 < 10. )
      Cp_1 = 10.;
    if ( Cp_1 > 100000. )
      Cp_1 = 100000.;
    }
  if ( !_finite ( Cp_1 ) )
    Cp_1 = 10.;
////
  if ( Q2 > 100. )
    {
		double dT = T_out_2 - Out_2.T;
    double kCps = LimdX ( k_Cp_12 * dT, 0.1 );
    Cp_2 *= 1. + kCps;
    if ( Cp_2 < 10. )
      Cp_2 = 10.;
    if ( Cp_2 > 100000. )
      Cp_2 = 100000.;
    }
  if ( !_finite ( Cp_2 ) )
    Cp_2 = 10.;
/*******
	Out_1.PT( pOut_1->P, T_out_1, pComp_1 );
	h_out_1 = Out_1.h;
	pOut_1->h_Flow = h_out_1;
//
	Out_2.PT( pOut_2->P, T_out_2, pComp_2 );
	h_out_2 = Out_2.h;
	pOut_2->h_Flow = h_out_2;
////
  if ( Q1 > 100. )
    {
		double dH = F_1 * fabs ( h_out_1 - h_in_1 );
    double Ks = dH / Q1 - 1.;
    double kCps = LimdX ( k_Cp_12 * Ks, 0.1 );
    Cp_1 *= 1. + kCps;
    if ( Cp_1 < 10. )
      Cp_1 = 10.;
    if ( Cp_1 > 100000. )
      Cp_1 = 100000.;
    }
  if ( !_finite ( Cp_1 ) )
    Cp_1 = 10.;
////
  if ( Q2 > 100. )
    {
		double dH = F_2 * fabs ( h_out_2 - h_in_2 );
    double Ks = dH / Q2 - 1.;
    double kCps = LimdX ( k_Cp_12 * Ks, 0.1 );
    Cp_2 *= 1. + kCps;
    if ( Cp_2 < 10. )
      Cp_2 = 10.;
    if ( Cp_2 > 100000. )
      Cp_2 = 100000.;
    }
  if ( !_finite ( Cp_2 ) )
    Cp_2 = 10.;
*******/
	}

int C2zmHeat::SaveState( )
	{
	S_CLASS("C2zmHeat_W",C2zmHeat_W)
	return 0;
	}

int C2zmHeat::RestoreState( char * StrName )
	{
	R_CLASS("C2zmHeat_W",C2zmHeat_W)
	return 1;
	}

int C2zmHeat::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetInitState )
    {
		T_in_0 = T_in_1 = T_in_2 = T_air;
		T_out_0 = T_out_1 = T_out_2 = T_air;
    return 1;
    }
	return 0;
	}

int C2zmHeat::GetParams( char * )
	{
	#include "IO_Parms.h"
	PARM( T_in_0, "#Т на входе 0" )
	PARM( T_out_0, "#Т на выходе 0" ) 
	PARM( T_in_1, "#Т на входе 1" ) 
	PARM( T_out_1, "#Т на выходе 1" )
	PARM( T_in_2, "#Т на входе 2" ) 
	PARM( T_out_2, "#Т на выходе 2" )
	PARM( Cp_0, "#Cp 0" ) 
	PARM( Cp_1, "#Cp 1" )
	PARM( Cp_2, "#Cp 2" )
  PARM( Lam_0, "#Lam 0" )
  PARM( Lam_1, "#Lam 1" )
  PARM( Lam_2, "#Lam 2" )
	PARM( pIn_0->Flow_mol, "#Поток 0 моль" )
	PARM( pIn_1->Flow_mol, "#Поток 1 моль" )
	PARM( pIn_2->Flow_mol, "#Поток 1 моль" )
  PARM( Q_cal, "#Теплопередача Гкал/ч" )
  PARM( Q_dg, "#Теплопередача ГДж/ч" )
	return 0;
	}
