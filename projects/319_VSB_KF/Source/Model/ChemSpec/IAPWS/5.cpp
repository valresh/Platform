#include "stdafx.h"
#include "math.h"
#include "Chem.h"
#include "IAPWS.h"

void CIF_97::SetXY_5( double T_K, double P_MPa )
  {
  LastError = 0;
  RT = Rkg * T_K;
	Tau = 1000. / T_K;
  Pi = P_MPa;
  }

CIAPWSApprData Data0_5[6] = {
0,0,-0.13179983674201e2,
0,1,0.68540841634434e1,
0,-3,-0.24805148933466e-1,
0,-2,0.36901534980333,
0,-1,-0.31161318213925e1,
0,2,-0.32961626538917,
  };

CIAPWSAppr Appr0_5( 6, Data0_5 );

CIAPWSApprData Data_5[5] = {
1,0,-0.12563183589592e-3,
1,1,0.21774678714571e-2,
1,3,-0.45942820899910e-2,
2,9,-0.39724828359569e-5,
3,3,0.12919228289784e-6,
  };

CIAPWSAppr Appr_5( 5, Data_5 );

double CIF_97::G_5 ( double T_K, double P_MPa )
	{
  double G0 = log ( Pi ) + Appr0_5.Appr( 1., Tau );
  double G = Appr_5.Appr ( Pi, Tau );
	return G0 + G;
	}

double CIF_97::G_5p ( double T_K, double P_MPa )
	{
	double G0 = 1. / Pi;
	double G = Appr_5.Appr_x ( Pi, Tau );
	return G0 + G;
	}

double CIF_97::G_5pp ( double T_K, double P_MPa )
	{
	double G0 = -1. / ( Pi * Pi );
	double G = Appr_5.Appr_xx ( Pi, Tau );
	return G0 + G;
	}

double CIF_97::G_5t ( double T_K, double P_MPa )
	{
	double G0 = Appr0_5.Appr_y( 1., Tau );
	double G = Appr_5.Appr_y ( Pi, Tau );
	return G0 + G;
	}

double CIF_97::G_5tt ( double T_K, double P_MPa )
	{
	double G0 = Appr0_5.Appr_yy ( 1., Tau );
	double G = Appr_5.Appr_yy ( Pi, Tau );
	return G0 + G;
	}

double CIF_97::G_5pt ( double T_K, double P_MPa )
	{
	double G0 = 0.;
	double G = Appr_5.Appr_xy ( Pi, Tau );
	return G0 + G;
	}

double CIF_97::Ro_5 ( double T_K, double P_MPa )
  {
  SetXY_5( T_K, P_MPa );
  double Ro = P_MPa * 1e3 / ( RT * Pi * G_5p ( T_K, P_MPa ));
  return Ro;
  }

double CIF_97::U_5 ( double T_K, double P_MPa )
  {
  SetXY_5( T_K, P_MPa );
  double U = RT * 
    ( Tau * G_5t ( T_K, P_MPa ) - Pi * G_5p ( T_K, P_MPa ));
  return U;
  }

double CIF_97::S_5 ( double T_K, double P_MPa )
  {
  SetXY_5( T_K, P_MPa );
  double S = Rkg * ( Tau * G_5t ( T_K, P_MPa ) - G_5 ( T_K, P_MPa ));
  return S;
  }

double CIF_97::H_5 ( double T_K, double P_MPa )
	{
  SetXY_5( T_K, P_MPa );
	double Gamma = G_5t ( T_K, P_MPa );
	double H = Rkg * T_K * Tau * Gamma;
	return H;
	}

double CIF_97::Cp_5 ( double T_K, double P_MPa )
	{
  SetXY_5( T_K, P_MPa );
  double Cp = -Rkg * Tau * Tau * G_5tt ( T_K, P_MPa );
	return Cp;
	}

double CIF_97::Cv_5 ( double T_K, double P_MPa )
  {
  SetXY_5( T_K, P_MPa );
  double Gamma = G_5p( T_K, P_MPa ) - Tau * G_5pt( T_K, P_MPa );
  double Cv = Rkg * 
    ( -Tau * Tau * G_5tt ( T_K, P_MPa ) + 
      Gamma * Gamma / G_5pp ( T_K, P_MPa ));
	return Cv;
  }

