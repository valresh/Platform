#include "stdafx.h"
#include "Chem.h"
#include "IAPWS.h"

CIF_97::CIF_97()
  {
	Tau = -1.;
  Pi = -1.;
  LastError = 0;
  RT = 0.;
  }

void CIF_97::Error( int N )
  {
  LastError = N;
  }

static double n_2_3[6] = {
0.,
0.34805185628969e3,
-0.11671859879975e1,
0.10192970039326e-2,
0.57254459862746e3,// Температура, при которой достигается минимальное давление
0.13918839778870e2,// Минимальное давление
};
//(623.15K,16.5292MPa) <-> (863.15K,100MPa).
double CIF_97::P_23_MPa ( double T_K )
	{
	double P_mPa = ( n_2_3[3] * T_K + n_2_3[2] ) * T_K + n_2_3[1];
	return P_mPa;
	}

double CIF_97::T_23_Kp ( double P_MPa )
	{
	double T = n_2_3[4] + sqrt ( ( P_MPa - n_2_3[5] ) / n_2_3[3] );
	return T;
	}

double CIF_97::T_23_Km ( double P_MPa )
	{
	double T = n_2_3[4] - sqrt ( ( P_MPa - n_2_3[5] ) / n_2_3[3] );
	return T;
	}

