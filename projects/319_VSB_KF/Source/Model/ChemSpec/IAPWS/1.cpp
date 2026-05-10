#include "stdafx.h"
#include "math.h"
#include "Chem.h"
#include "IAPWS.h"

void CIF_97::SetXY_1( double T_K, double P_MPa )
  {
  LastError = 0;
  RT = Rkg * T_K;
	Pi = P_MPa / 16.53;
	Tau = 1386. / T_K;
	x = 7.1 - Pi;
  y = Tau - 1.222;
  }

CIAPWSApprData Data_1[34] = {
0,-2,0.14632971213167,
0,-1,-0.84548187169114,
0,0,-0.37563603672040e1,
0,1,0.33855169168385e1,
0,2,-0.95791963387872,
0,3,0.15772038513228,
0,4,-0.16616417199501e-1,
0,5,0.81214629983568e-3,
1,-9,0.28319080123804e-3,
1,-7,-0.60706301565874e-3,
1,-1,-0.18990068218419e-1,
1,0,-0.32529748770505e-1,
1,1,-0.21841717175414e-1,
1,3,-0.52838357969930e-4,
2,-3,-0.47184321073267e-3,
2,0,-0.30001780793026e-3,
2,1,0.47661393906987e-4,
2,3,-0.44141845330846e-5,
2,17,-0.72694996297594e-15,
3,-4,-0.31679644845054e-4,
3,0,-0.28270797985312e-5,
3,6,-0.85205128120103e-9,
4,-5,-0.22425281908000e-5,
4,-2,-0.65171222895601e-6,
4,10,-0.14341729937924e-12,
5,-8,-0.40516996860117e-6,
8,-11,-0.12734301741641e-8,
8,-6,-0.17424871230634e-9,
21,-29,-0.68762131295531e-18,
23,-31,0.14478307828521e-19,
29,-38,0.26335781662795e-22,
30,-39,-0.11947622640071e-22,
31,-40,0.18228094581404e-23,
32,-41,-0.93537087292458e-25,
};

CIAPWSAppr Appr_1( 34, Data_1 );

double CIF_97::G_1 ( double T_K, double P_MPa )
	{
  double G = Appr_1.Appr ( x, y );
	return G;
	}

double CIF_97::G_1p ( double T_K, double P_MPa )
	{
  double G = -Appr_1.Appr_x( x, y );
	return G;
	}

double CIF_97::G_1pp ( double T_K, double P_MPa )
	{
  double G = Appr_1.Appr_xx( x, y );
	return G;
	}

double CIF_97::G_1t ( double T_K, double P_MPa )
	{
  double G = Appr_1.Appr_y( x, y );
	return G;
	}

double CIF_97::G_1tt ( double T_K, double P_MPa )
	{
  double G = Appr_1.Appr_yy( x, y );
	return G;
	}

double CIF_97::G_1pt ( double T_K, double P_MPa )
	{
  double G = -Appr_1.Appr_xy( x, y );
	return G;
	}

double CIF_97::Ro_1 ( double T_K, double P_MPa )
  {
  SetXY_1( T_K, P_MPa );
  double Ro = P_MPa * 1e3 / ( RT * Pi * G_1p ( T_K, P_MPa ));
  return Ro;
  }

double CIF_97::U_1 ( double T_K, double P_MPa )
  {
  SetXY_1( T_K, P_MPa );
  double U = RT * 
    ( Tau * G_1t ( T_K, P_MPa ) - Pi * G_1p ( T_K, P_MPa ));
  return U;
  }

double CIF_97::S_1 ( double T_K, double P_MPa )
  {
  SetXY_1( T_K, P_MPa );
  double S = Rkg * ( Tau * G_1t ( T_K, P_MPa ) - G_1 ( T_K, P_MPa ));
  return S;
  }

double CIF_97::H_1 ( double T_K, double P_MPa )
	{
  SetXY_1( T_K, P_MPa );
	double Gamma = G_1t ( T_K, P_MPa );
	double H = Rkg * T_K * Tau * Gamma;
	return H;
	}

double CIF_97::Cp_1 ( double T_K, double P_MPa )
	{
  SetXY_1( T_K, P_MPa );
  double Cp = -Rkg * Tau * Tau * G_1tt ( T_K, P_MPa );
	return Cp;
	}

double CIF_97::Cv_1 ( double T_K, double P_MPa )
  {
  SetXY_1( T_K, P_MPa );
  double Gamma = G_1p( T_K, P_MPa ) - Tau * G_1pt( T_K, P_MPa );
  double Cv = Rkg * 
    ( -Tau * Tau * G_1tt ( T_K, P_MPa ) + 
      Gamma * Gamma / G_1pp ( T_K, P_MPa ));
	return Cv;
  }

CIAPWSApprData Data_1h[20] = {
0,0,-0.23872489924521e3,
0,1,0.40421188637945e3,
0,2,0.11349746881718e3,
0,6,-0.58457616048039e1,
0,22,-0.15285482413140e-3,
0,32,-0.10866707695377e-5,
1,0,-0.13391744872602e2,
1,1,0.43211039183559e2,
1,2,-0.54010067170506e2,
1,3,0.30535892203916e2,
1,4,-0.65964749423638e1,
1,10,0.93965400878363e-2,
1,32,0.11573647505340e-6,
2,10,-0.25858641282073e-4,
2,32,-0.40644363084799e-8,
3,10,0.66456186191635e-7,
3,32,0.80670734103027e-10,
4,32,-0.93477771213947e-12,
5,32,0.58265442020601e-14,
6,32,-0.15020185953503e-16,
};

CIAPWSAppr Appr_1h( 20, Data_1h );

double CIF_97::T_1h ( double P_MPa, double H_kJ_kg )
  {
  double x = P_MPa;
  double y = H_kJ_kg / 2500. + 1.;
//
  double T = Appr_1h.Appr ( x, y );
  return T;  
  }
CIAPWSApprData Data_1s[20] = {
0,0,0.17478268058307e3,
1,12,0.35672110607366e-9,
0,1,0.34806930892873e2,
1,31,0.17332496994895e-23,
0,2,0.65292584978455e1,
2,0,0.56608900654837e-3,
0,3,0.33039981775489,
2,1,-0.32635483139717e-3,
0,11,-0.19281382923196e-6,
2,2,0.44778286690632e-4,
0,31,-0.24909197244573e-22,
2,9,-0.51322156908507e-9,
1,0,-0.26107636489332,
2,31,-0.42522657042207e-25,
1,1,0.22592965981586,
3,10,0.26400441360689e-12,
1,2,-0.64256463395226e-1,
3,32,0.78124600459723e-28,
1,3,0.78876289270526e-2,
4,32,-0.30732199903668e-30};

CIAPWSAppr Appr_1s( 20, Data_1s );

double CIF_97::T_1s ( double P_MPa, double S )
  {
  double x = P_MPa;
  double y = S + 2.;
//
  double T = Appr_1s.Appr ( x, y );
  return T;  
  }
