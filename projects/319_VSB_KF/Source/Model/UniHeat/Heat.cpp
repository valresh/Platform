#include "stdafx.h"
#define DLL_UniHeat
#include "UniHeat.h"
#include "math.h"
#include "Err.h"
#include "SR.h"
#include "SetData.h"

static double To_MWt =  1e-3 / 3600.;


CHeat::CHeat( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain ),
In( "InHeat", this ),
Out( "OutHeat", this )
	{
  Model = "Heat";
	IterH = true;
  Inverse_Lev = false;
	KS = 1000.;
	Lev_Zm = 0.1;
	k_Cp = 1.;
	Cp = 10.;
  Q_Heat = 0.;
  Q_cal = 0.;
  Q_MWt = 0.;
  Q_HeatZ = 0.;
  k_Q_HeatZ = 1.;
  Use_Q_HeatZ = false;
  pIn = NULL;
  pOut = NULL;
  Alfa = 0.5;
  T_Alfa = 15.;
  pIn = NULL;
  pOut = NULL;
  }

CHeat_W::CHeat_W()
	{
	memset ( this, 0, sizeof ( CHeat_W ));
	}

void CHeat::Init()
  {
  In.T_min = -50;
  In.T_max = 500.;
  In.P_max = 100.;
  Out.T_min = -50;
  Out.T_max = 500.;
  Out.P_max = 100.;
  IterH = true;
  for ( int n = 0; n < K_GAS; n++ )
    {
    In.Henry[n] = 1.;
    Out.Henry[n] = 1.;
    }
  }

void CHeat::Calc( double dt )
	{
  if ( pOut == NULL || pIn == NULL )
    {
    if ( Use_Q_HeatZ )
      dQ_Heat = Q_Heat = Q_HeatZ;
    else
      dQ_Heat = Q_Heat = 0.;
    return;
    }
  double Ps = pOut->P;
	*pOut = *pIn;
	pOut->P = Ps;
  double F = fabs ( pIn->Flow_mol );
  if ( F < 0.01 )
    {
    if ( Use_Q_HeatZ )
      dQ_Heat = Q_Heat = Q_HeatZ;
    else
      dQ_Heat = Q_Heat = 0.;
    return;
    }
//
	CComp * pComp = pIn->Get_Comp();
	if ( pComp == NULL )
    {
    if ( Use_Q_HeatZ )
      dQ_Heat = Q_Heat = Q_HeatZ;
    else
      dQ_Heat = Q_Heat = 0.;
    return;
    }
//
//
	h_in = pIn->h_Flow;
	In.T_min = ::T_min;
	In.T_max = ::T_max;
	In.PH ( pIn->P, h_in, pComp );
	T_in = In.T;
//
// 
	double L = 1.;
  if ( Lev_Zm > 0. )  
    L = Lev / Lev_Zm;
  if ( L > 1. )
    L = 1.;
  if ( Inverse_Lev )
    {
    L = 1. - L;
    }
	double ks = KS * ( 0.95 * L + 0.05 );
  double Fcp = F * Cp;
  Lam = 100.;
  if ( Lam * Fcp > ks )
    Lam = ks / Fcp;
///////////////////////////////////////////////
	double D_in = T_in - T_ext;
	double D_out = D_in * exp ( -Lam );
	T_out = T_ext + D_out;
	double D_alfa = D_in * exp ( -Lam * Alfa );
	T_Alfa = T_ext + D_alfa;
//
  double Q = Fcp * fabs ( T_out - T_in );
//
	if ( IterH )
		{
		if ( T_out > T_in	)
			h_out = h_in + Q / F;
		else
			h_out = h_in - Q / F;
		Out.T_min = ::T_min;
		Out.T_max = ::T_max;
		Out.PH( pOut -> P, h_out, pComp );
		pOut->h_Flow = Out.h;
		double T_out_h = Out.T;
    pOut->T = T_out_h;
	////
		if ( Q > 100. )
			{
			double dT = T_out_h - T_out;
			double kCps; 
			if ( T_out > T_in	)
				kCps = LimdX ( -k_Cp * dT * 0.01, 0.1 );
			else
				kCps = LimdX ( k_Cp * dT * 0.01, 0.1 );
			Cp *= 1. + kCps;
			}
    if ( Cp < 10. )
      Cp = 10.;
    if ( Cp > 100000. )
      Cp = 100000.;
		if ( !_finite ( Cp ) )
			Cp = 10.;
		}
	else
		{
		Out.PT( pOut->P, T_out, pComp );
		h_out = Out.h;
		pOut->h_Flow = h_out;
    pOut->T = T_out;
		if ( Q > 100. )
			{
			double dH = F * fabs ( h_out - h_in );
			double Ks = dH / Q - 1.;
			double kCps = LimdX ( k_Cp * Ks * 0.01, 0.1 );
			Cp *= 1. + kCps;
			if ( Cp < 10. )
				Cp = 10.;
			if ( Cp > 100000. )
				Cp = 100000.;
			}
		if ( !_finite ( Cp ) )
			Cp = 10.;
		}
//
	Q_Heat_F = Fcp * ( T_in - T_out );
  if ( Use_Q_HeatZ )
    {
    dQ_Heat = Q_HeatZ - Q_Heat_F;
    Q_Heat = Q_HeatZ;
    }
  else
    {
    Q_Heat = k_Q_HeatZ * Q_Heat_F;
    dQ_Heat = Q_Heat - Q_Heat_F;
    }
  FINITE(Q_Heat)
	Q_MWt = fabs ( Q_Heat ) * To_MWt;
	Q_cal = fabs ( Q_Heat ) * M_cal;
	}

int CHeat::SaveState( )
	{
	S_CLASS("CHeat_W",CHeat_W)
	return 0;
	}

int CHeat::RestoreState( char * StrName )
	{
	R_CLASS("CHeat_W",CHeat_W)
	return 1;
	}

int CHeat::SetData( int TypeData, void * pData )
	{
	return 0;
	}

int CHeat::GetParams( char * )
	{
	#include "IO_Parms.h"
	PARM( T_in, "#Т на входе" ) 
	PARM( T_out, "#Т на выходе" )
	PARM( T_Alfa, "#Т в середине" )
	PARM( Cp, "#Cp" )
  PARM( Lam, "#Lam" )
  if( pIn )
    {
	  PARM( pIn->Flow_mol, "#Поток 1 моль" )
    }
  PARM( Q_cal, "#Теплопередача Гкал/ч" )
  PARM( Q_MWt, "#Теплопередача МВт" )
  PARM_D( k_Cp, "регулировка теплоемкости", 1. )
//
	PARM_B( IterH, "Расчет по энтальпии", true );
	PARM_D( KS, "Суммарная теплоотдача", 1000. )
	PARM_D( Alfa, "Положение точки в середине", true );
	PARM_D( Lev_Zm, "Уровень змеевиков", 0.1 )
  PARM_B( Inverse_Lev, "Чем ниже уровень, тем больше терлоотдача", false );
	return 0;
	}
