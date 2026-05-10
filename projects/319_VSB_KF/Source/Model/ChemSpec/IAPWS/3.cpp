#include "stdafx.h"
#include "math.h"
#include "Chem.h"
#include "IAPWS.h"


void CIF_97::SetXY_3( double T_K, double Ro_kg_m3 )
  {
  LastError = 0;
  RT = Rkg * T_K;
	Pi = Ro_kg_m3 / 322.;
	Tau = TP / T_K;
  }

CIAPWSApprData Data_3[39] = {
//0,0,0.10658070028513e1,
0,0,-0.15732845290239e2,
0,1,0.20944396974307e2,
0,2,-0.76867707878716e1,
0,7,0.26185947787954e1,
0,10,-0.28080781148620e1,
0,12,0.12053369696517e1,
0,23,-0.84566812812502e-2,
1,2,-0.12654315477714e1,
1,6,-0.11524407806681e1,
1,15,0.88521043984318,
1,17,-0.64207765181607,
2,0,0.38493460186671,
2,2,-0.85214708824206,
2,6,0.48972281541877e1,
2,7,-0.30502617256965e1,
2,22,0.39420536879154e-1,
2,26,0.12558408424308,
3,0,-0.27999329698710,
3,2,0.13899799569460e1,
3,4,-0.20189915023570e1,
3,16,-0.82147637173963e-2,
3,26,-0.47596035734923,
4,0,0.43984074473500e-1,
4,2,-0.44476435428739,
4,4,0.90572070719733,
4,26,0.70522450087967,
5,1,0.10770512626332,
5,3,-0.32913623258954,
5,26,-0.50871062041158,
6,0,-0.22175400873096e-1,
6,2,0.94260751665092e-1,
6,26,0.16436278447961,
7,2,-0.13503372241348e-1,
8,26,-0.14834345352472e-1,
9,2,0.57922953628084e-3,
9,26,0.32308904703711e-2,
10,0,0.80964802996215e-4,
10,1,-0.16557679795037e-3,
11,26,-0.44923899061815e-4,
};

CIAPWSAppr Appr_3( 39, Data_3 );

static double n1 = 0.10658070028513e1;

double CIF_97::G_3 ( double T_K, double Ro_kg_m3 )
	{
  double G = n1 * log ( Pi ) + Appr_3.Appr ( Pi, Tau );
	return G;
	}

double CIF_97::G_3p ( double T_K, double Ro_kg_m3 )
	{
  double G = n1 / Pi + Appr_3.Appr_x( Pi, Tau );
	return G;
	}

double CIF_97::G_3pp ( double T_K, double Ro_kg_m3 )
	{
  double G = -n1 / ( Pi * Pi ) + Appr_3.Appr_xx( Pi, Tau );
	return G;
	}

double CIF_97::G_3t ( double T_K, double Ro_kg_m3 )
	{
  double G = Appr_3.Appr_y( Pi, Tau );
	return G;
	}

double CIF_97::G_3tt ( double T_K, double Ro_kg_m3 )
	{
  double G = Appr_3.Appr_yy( Pi, Tau );
	return G;
	}

double CIF_97::G_3pt ( double T_K, double Ro_kg_m3 )
	{
  double G = Appr_3.Appr_xy( Pi, Tau );
	return G;
	}

double CIF_97::P_3 ( double T_K, double Ro_kg_m3 )
  {
  SetXY_3( T_K, Ro_kg_m3 );
  double P = Ro_kg_m3 * 1e-3 * RT * Pi * G_3p ( T_K, Ro_kg_m3 );
  return P;
  }

double CIF_97::U_3 ( double T_K, double Ro_kg_m3 )
  {
  SetXY_3( T_K, Ro_kg_m3 );
  double U = RT * Tau * G_3t ( T_K, Ro_kg_m3 );
  return U;
  }

double CIF_97::S_3 ( double T_K, double Ro_kg_m3 )
  {
  SetXY_3( T_K, Ro_kg_m3 );
  double S = Rkg * ( Tau * G_3t ( T_K, Ro_kg_m3 ) - G_3 ( T_K, Ro_kg_m3 ));
  return S;
  }

double CIF_97::H_3 ( double T_K, double Ro_kg_m3 )
	{
  SetXY_3( T_K, Ro_kg_m3 );
	double H = RT * ( Tau * G_3t ( T_K, Ro_kg_m3 ) + Pi * G_3p ( T_K, Ro_kg_m3 ));
	return H;
	}

double CIF_97::Cv_3 ( double T_K, double Ro_kg_m3 )
	{
  SetXY_3( T_K, Ro_kg_m3 );
  double Cv = -Rkg * Tau * Tau * G_3tt ( T_K, Ro_kg_m3 );
	return Cv;
	}

double CIF_97::Cp_3 ( double T_K, double Ro_kg_m3 )
  {
  SetXY_3( T_K, Ro_kg_m3 );
  double Gamma = Pi * ( G_3p( T_K, Ro_kg_m3 ) - Tau * G_3pt( T_K, Ro_kg_m3 ));
  double Cp = Rkg * 
    ( -Tau * Tau * G_3tt ( T_K, Ro_kg_m3 ) + 
      Gamma * Gamma / 
     ( 2. * Pi * G_3p ( T_K, Ro_kg_m3 ) + Pi * Pi * G_3pp ( T_K, Ro_kg_m3 )));
	return Cp;
  }

double CIF_97::Ro_3 ( double T_K, double P_MPa )
  {
  if ( T_K < TP )
    {
// 2-х фазная область
    double P = Pz ( T_K );
    if ( P_MPa < P )
      return Ro_3_gas ( T_K, P_MPa );
    else
      return Ro_3_liq ( T_K, P_MPa );
    }
  double Rob = 0.;
  double Roe = 1000.;
  double Ro; 
  while ( Roe - Rob > 1e-3 )
    {
    Ro = 0.5 * ( Rob + Roe );
    double P = P_3 ( T_K, Ro );
    if ( P > P_MPa )
      Roe = Ro;
    else
      Rob = Ro;
    }
  Ro = 0.5 * ( Rob + Roe ); 
  return Ro;
  }

double CIF_97::Ro_3_gas ( double T_K, double P_MPa )
  {
  double Roe = 50;
  double P = P_3 ( T_K, Roe );
  if ( P > P_MPa )
    Error ( 3 );
  double Rob = 0.;
  while ( P < P_MPa )
    {
    Rob = Roe;
    Roe += 5.;
    if ( Roe > 322. )
      return 322.;
    P = P_3 ( T_K, Roe );
    }
  double Ro; 
  while ( Roe - Rob > 1e-3 )
    {
    Ro = 0.5 * ( Rob + Roe );
    P = P_3 ( T_K, Ro );
    if ( P > P_MPa )
      Roe = Ro;
    else
      Rob = Ro;
    }
  Ro = 0.5 * ( Rob + Roe ); 
  return Ro;
  }

double CIF_97::Ro_3_liq ( double T_K, double P_MPa )
  {
  double Rob = 650.;
  double P = P_3 ( T_K, Rob );
  if ( P < P_MPa )
    Error ( 3 );
  double Roe = 650.;
  while ( P > P_MPa )
    {
    Roe = Rob;
    Rob -= 5.;
    if ( Rob < 322. )
      return 322.;
    P = P_3 ( T_K, Rob );
    }
  double Ro; 
  while ( Roe - Rob > 1e-3 )
    {
    Ro = 0.5 * ( Rob + Roe );
    P = P_3 ( T_K, Ro );
    if ( P > P_MPa )
      Roe = Ro;
    else
      Rob = Ro;
    }
  Ro = 0.5 * ( Rob + Roe ); 
  return Ro;
  }

