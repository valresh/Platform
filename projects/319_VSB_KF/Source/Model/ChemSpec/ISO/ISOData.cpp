#include "stdafx.h"
#include "Chem.h"
#include "ISO.h"
#include "Data.h"

//Natural gas — Calculation of thermodynamic properties — Part 2: Single-phase properties (gas, liquid, and dense fluid) for extended ranges of application

/*
ISO 20765-2:2015
Газ природный. Расчет термодинамических свойств. Часть 2. Свойства в однофазном состоянии (газа, жидкости и плотного флюида) для расширенных диапазонов применения
*/

struct CCoef58
  {
  int n;
  double a, b, c, k, u, g, q, f, s, w;
  };

static CCoef58 Coef58[58] = {
1,0.153832600,1,0,0,0.0,0,0,0,0,0,
2,1.341953000,1,0,0,0.5,0,0,0,0,0,
3,-2.998583000,1,0,0,1.0,0,0,0,0,0,
4,-0.048312280,1,0,0,3.5,0,0,0,0,0,
5,0.375796500,1,0,0,-0.5,1,0,0,0,0,
6,-1.589575000,1,0,0,4.5,1,0,0,0,0,
7,-0.053588470,1,0,0,0.5,0,1,0,0,0,
8,0.886594630,1,0,0,7.5,0,0,0,1,0,
9,-0.710237040,1,0,0,9.5,0,0,0,1,0,
10,-1.471722000,1,0,0,6.0,0,0,0,0,1,
11,1.321850350,1,0,0,12.0,0,0,0,0,1,
12,-0.786659250,1,0,0,12.5,0,0,0,0,1,
13,2.291290e-9,1,1,3,-6.0,0,0,1,0,0,
14,0.157672400,1,1,2,2.0,0,0,0,0,0,
15,-0.436386400,1,1,2,3.0,0,0,0,0,0,
16,-0.044081590,1,1,2,2.0,0,1,0,0,0,
17,-0.003433888,1,1,4,2.0,0,0,0,0,0,
18,0.032059050,1,1,4,11.0,0,0,0,0,0,
19,0.024873550,2,0,0,-0.5,0,0,0,0,0,
20,0.073322790,2,0,0,0.5,0,0,0,0,0,
21,-0.001600573,2,1,2,0.0,0,0,0,0,0,
22,0.642470600,2,1,2,4.0,0,0,0,0,0,
23,-0.416260100,2,1,2,6.0,0,0,0,0,0,
24,-0.066899570,2,1,4,21.0,0,0,0,0,0,
25,0.279179500,2,1,4,23.0,1,0,0,0,0,
26,-0.696605100,2,1,4,22.0,0,1,0,0,0,
27,-0.002860589,2,1,4,-1.0,0,0,1,0,0,
28,-0.008098836,3,0,0,-0.5,0,1,0,0,0,
29,3.150547000,3,1,1,7.0,1,0,0,0,0,
30,0.007224479,3,1,1,-1.0,0,0,1,0,0,
31,-0.705752900,3,1,2,6.0,0,0,0,0,0,
32,0.534979200,3,1,2,4.0,1,0,0,0,0,
33,-0.079314910,3,1,3,1.0,1,0,0,0,0,
34,-1.418465000,3,1,3,9.0,1,0,0,0,0,
35,-5.99905e-17,3,1,4,-13.0,0,0,1,0,0,
36,0.105840200,3,1,4,21.0,0,0,0,0,0,
37,0.034317290,3,1,4,8.0,0,1,0,0,0,
38,-0.007022847,4,0,0,-0.5,0,0,0,0,0,
39,0.024955870,4,0,0,0.0,0,0,0,0,0,
40,0.042968180,4,1,2,2.0,0,0,0,0,0,
41,0.746545300,4,1,2,7.0,0,0,0,0,0,
42,-0.291961300,4,1,2,9.0,0,1,0,0,0,
43,7.294616000,4,1,4,22.0,0,0,0,0,0,
44,-9.936757000,4,1,4,23.0,0,0,0,0,0,
45,-0.005399808,5,0,0,1.0,0,0,0,0,0,
46,-0.243256700,5,1,2,9.0,0,0,0,0,0,
47,0.049870160,5,1,2,3.0,0,1,0,0,0,
48,0.003733797,5,1,4,8.0,0,0,0,0,0,
49,1.874951000,5,1,4,23.0,0,1,0,0,0,
50,0.002168144,6,0,0,1.5,0,0,0,0,0,
51,-0.658716400,6,1,2,5.0,1,0,0,0,0,
52,0.000205518,7,0,0,-0.5,0,1,0,0,0,
53,0.009776195,7,1,2,4.0,0,0,0,0,0,
54,-0.020487080,8,1,1,7.0,1,0,0,0,0,
55,0.015573220,8,1,2,3.0,0,0,0,0,0,
56,0.006862415,8,1,2,0.0,1,0,0,0,0,
57,-0.001226752,9,1,2,1.0,0,0,0,0,0,
58,0.002850908,9,1,2,0.0,0,1,0,0,0 };



struct CCompData
  {
  int c;
  int * ID;
  double M, E, K, G, Q, F, S, W;
  };

static CCompData CompData[K_BASE] = {
1,&G_N2,28.0135,99.737780,0.4479153,0.027815,0.0,0.0,0.0,0.0,
2,&G_CO2,44.010,241.960600,0.4557489,0.189065,0.690000,0.0,0.0,0.0,
3,&G_CH4,16.043,151.318300,0.4619255,0.0,0.0,0.0,0.0,0.0,
4,&G_C2H6,30.070,244.166700,0.5279209,0.079300,0.0,0.0,0.0,0.0,
5,&G_C3H8,44.097,298.118300,0.5837490,0.141239,0.0,0.0,0.0,0.0,
6,&G_C4H10N,58.123,337.638900,0.6341423,0.281835,0.0,0.0,0.0,0.0,
7,&G_C4H10I,58.123,324.068900,0.6406937,0.256692,0.0,0.0,0.0,0.0,
8,&G_C5H12N,72.150,370.682300,0.6798307,0.366911,0.0,0.0,0.0,0.0,
9,&G_C5H12I,72.150,365.599900,0.6738577,0.332267,0.0,0.0,0.0,0.0,
10,&G_C6H14N,86.177,402.636293,0.7175118,0.289731,0.0,0.0,0.0,0.0,
11,&G_C7H16N,100.204,427.722630,0.7525189,0.337542,0.0,0.0,0.0,0.0,
12,&G_C8H18N,114.231,450.325022,0.7849550,0.383381,0.0,0.0,0.0,0.0,
13,&G_C9H20N,128.258,470.840891,0.8152731,0.427354,0.0,0.0,0.0,0.0,
14,&G_C10H22N,142.285,489.558373,0.8437826,0.469659,0.0,0.0,0.0,0.0,
15,&G_H2,2.0159,26.957940,0.3514916,0.034369,0.0,1.0,0.0,0.0,
16,&G_O2,31.9988,122.766700,0.4186954,0.021000,0.0,0.0,0.0,0.0,
17,&G_CO,28.010,105.534800,0.4533894,0.038953,0.0,0.0,0.0,0.0,
18,&G_H2O,18.0153,514.015600,0.3825868,0.332500,1.067750,0.0,1.582200,1.0,
19,&G_H2S,34.082,296.355000,0.4618263,0.088500,0.633276,0.0,0.390000,0.0,
20,&G_HE,4.0026,2.610111,0.3589888,0.0,0.0,0.0,0.0,0.0,
21,&G_AR,39.948,119.629900,0.4216551,0.0,0.0,0.0,0.0,0.0 };


struct CInteract
  {
  int I, J;
  double E, V, K, G;
  };

static CInteract InterData[] = {
1,2,1.022740,0.835058,0.982361,0.982746,
1,3,0.971640,0.886106,1.003630,1.0,
1,4,0.970120,0.816431,1.007960,1.0,
1,5,0.945939,0.915502,1.0,1.0,
1,6,0.973384,0.993556,1.0,1.0,
1,7,0.946914,1.0,1.0,1.0,
1,8,0.945520,1.0,1.0,1.0,
1,9,0.959340,1.0,1.0,1.0,
1,15,1.086320,0.408838,1.032270,1.0,
1,16,1.021000,1.0,1.0,1.0,
1,17,1.005710,1.0,1.0,1.0,
1,18,0.746954,1.0,1.0,1.0,
1,19,0.902271,0.993476,0.942596,1.0,
2,3,0.960644,0.963827,0.995933,0.807653,
2,4,0.925053,0.969870,1.008510,0.370296,
2,5,0.960237,1.0,1.0,1.0,
2,6,0.897362,1.0,1.0,1.0,
2,7,0.906849,1.0,1.0,1.0,
2,8,0.859764,1.0,1.0,1.0,
2,9,0.726255,1.0,1.0,1.0,
2,10,0.855134,1.066638,0.910183,1.0,
2,11,0.831229,1.077634,0.895362,1.0,
2,12,0.808310,1.088178,0.881152,1.0,
2,13,0.786323,1.098291,0.867520,1.0,
2,14,0.765171,1.108021,0.854406,1.0,
2,15,1.281790,1.0,1.0,1.0,
2,17,1.500000,0.900000,1.0,1.0,
2,18,1.0,1.0,1.673090,1.673090,
2,19,0.955052,1.045290,1.007790,1.0,
3,5,0.994635,0.990877,1.007619,1.0,
3,6,0.989844,0.992291,0.997596,1.0,
3,7,1.019530,1.0,1.0,1.0,
3,8,0.999268,1.003670,1.002529,1.0,
3,9,1.002350,1.0,1.0,1.0,
3,10,1.107274,1.302576,0.982962,1.0,
3,11,0.880880,1.191904,0.983565,1.0,
3,12,0.880973,1.205769,0.982707,1.0,
3,13,0.881067,1.219634,0.981849,1.0,
3,14,0.881161,1.233498,0.980991,1.0,
3,15,1.170520,1.156390,1.023260,1.957310,
3,17,0.990126,1.0,1.0,1.0,
3,18,0.708218,1.0,1.0,1.0,
3,19,0.931484,0.736833,1.000080,1.0,
4,5,1.022560,1.065173,0.986893,1.0,
4,6,1.013060,1.250000,1.0,1.0,
4,7,1.0,1.250,1.0,1.0,
4,8,1.005320,1.250000,1.0,1.0,
4,9,1.0,1.250000,1.0,1.0,
4,15,1.164460,1.616660,1.020340,1.0,
4,18,0.693168,1.0,1.0,1.0,
4,19,0.946871,0.971926,0.999969,1.0,
5,6,1.004900,1.0,1.0,1.0,
5,15,1.034787,1.0,1.0,1.0,
6,15,1.300000,1.0,1.0,1.0,
7,15,1.300000,1.0,1.0,1.0,
10,19,1.008692,1.028973,0.968130,1.0,
11,19,1.010126,1.033754,0.962870,1.0,
12,19,1.011501,1.038338,0.957828,1.0,
13,19,1.012821,1.042735,0.952441,1.0,
14,19,1.014089,1.046966,0.948338,1.0,
15,17,1.100000,1.0,1.0,1.0,
0, 0, };

static CInteract Inter[K_BASE][K_BASE];

static int Numb[K_GAS];

void InitISO()
  {
  for ( int c = 0; c < K_GAS; c++ )
    {
    Numb[c] = -1;
    }
  for ( int n = 0; n < K_BASE; n++ )
    {
    int c = *CompData[n].ID;
    Numb[c] = n;
    }
  for ( int i = 0; i < K_BASE; i++ )
    {
    for ( int j = 0; j < K_BASE; j++ )
      {
      Inter[i][j].E = 1.;
      Inter[i][j].V = 1.;
      Inter[i][j].K = 1.;
      Inter[i][j].G = 1.;
      }
    }
  int n = 0;
  while ( InterData[n].I > 0 && InterData[n].J > 0 )
    {
    int i = InterData[n].I - 1;
    int j = InterData[n].J - 1;
    Inter[i][j].E = Inter[j][i].E = InterData[n].E;
    Inter[i][j].V = Inter[j][i].V = InterData[n].V;
    Inter[i][j].K = Inter[j][i].K = InterData[n].K;
    Inter[i][j].G = Inter[j][i].G = InterData[n].G;
    n++;
    }
  }

CISO::CISO()
  {
  WasCalcFi = false;
  WasCalcF1 = false;
  WasCalcF2 = false;
  WasCalcDelta = false;
  WasCalcTau_dFdTau = false;
  WasCalcTau2_dFdTauTau = false;
  WasCalcUHS = false;
  WasCalcCpCv = false;
  WasCalcMu = false;
  }

int CISO::SetComp( CComp * pComp )
  {
  return SetComp( pComp->IsComp, pComp->Cmol );
  }

#define Xi X[i]
#define Xj X[j]
#define Mi CompData[i].M
#define Ei CompData[i].E
#define Ej CompData[j].E
#define Ki CompData[i].K
#define Kj CompData[j].K
#define Fi CompData[i].F
#define Fj CompData[j].F
#define Qi CompData[i].Q
#define Qj CompData[j].Q
#define Gi CompData[i].G
#define Gj CompData[j].G
#define Si CompData[i].S
#define Sj CompData[j].S
#define Wi CompData[i].W
#define Wj CompData[j].W
#define Gij Inter[i][j].G
#define GGij ( Gij * ( Gi + Gj ) * 0.5 )
#define Eij Inter[i][j].E
#define EEij ( Eij * sqrt ( Ei * Ej ))
#define Vij Inter[i][j].V
#define Kij Inter[i][j].K
/////////////////////////
#define an Coef58[n].a
#define bn Coef58[n].b
#define cn Coef58[n].c
#define gn Coef58[n].g
#define qn Coef58[n].q
#define fn Coef58[n].f
#define kn Coef58[n].k
#define un Coef58[n].u
#define sn Coef58[n].s
#define wn Coef58[n].w

double pow_5_2( double X )
  {
  if ( X == 1. )
    return 1.;
  double X2 = X * X;
  return X2 * sqrt ( X );
  }

double pow_3_2( double X )
  {
  if ( X == 1. )
    return 1.;
  return X * sqrt ( X );
  }

double pow_5( double X )
  {
  if ( X == 1. )
    return 1.;
  double X2 = X * X;
  double X4 = X2 * X2;
  return X4 * X;
  }

int CISO::SetComp( bool IsComp[], double Cmol[] )
  {
  WasCalcFi = false;
  WasCalcF1 = false;
  WasCalcF2 = false;
  WasCalcDelta = false;
  WasCalcTau_dFdTau = false;
  WasCalcTau2_dFdTauTau = false;
  WasCalcUHS = false;
  WasCalcCpCv = false;
  WasCalcMu = false;
  CLEAR(X)
  CLEAR(C)
  int Err = 0;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    int n = Numb[c];
    if ( n < 0 )
      {
      Err = 1;
      continue;
      }
    X[n] += Cmol[c];
    }
  double Sx = 0.;
  for ( int n = 0; n < K_BASE; n++ )
    Sx += X[n];
  if ( fabs ( Sx - 1. ) > 1e-6 )
    {
    if ( Sx < 1e-6 )
      {
      Err = 2;
      Sx = 0.;
      }
    else
      Sx = 1. / Sx;
    for ( int n = 0; n < K_BASE; n++ )
      X[n] *= Sx;
    }
//
  F = 0.;
  Q = 0.;
  M = 0.;
  for ( int i = 0; i < K_BASE; i++ )
    {
    F += Xi * Xi * Fi;
    Q += Xi * Qi;
    M += Xi * Mi;
    }
  G = 0.;
  for ( int i = 0; i < K_BASE; i++ )
    {
    G += Xi * Gi;
    for ( int j = i + 1; j < K_BASE; j++ )
      {
      G += Xi * Xj * ( Gij - 1. ) * ( Gi + Gj );
      }
    }
  double S1 = 0.;
  double S2 = 0.;
  for ( int i = 0; i < K_BASE; i++ )
    {
    S1 += Xi * pow_5_2 ( Ei );
    for ( int j = i + 1; j < K_BASE; j++ )
      {
      S2 += Xi * Xj * ( pow_5( Vij ) - 1. ) * pow_5_2 ( Ei * Ej );
      }
    }
  double V5 = S1 * S1 + 2. * S2;
  V = pow ( V5, 0.2 );
//
  S1 = 0.;
  S2 = 0.;
  for ( int i = 0; i < K_BASE; i++ )
    {
    S1 += Xi * pow_5_2 ( Ki );
    for ( int j = i + 1; j < K_BASE; j++ )
      {
      S2 += Xi * Xj * ( pow_5( Kij ) - 1. ) * pow_5_2 ( Ki * Kj );
      }
    }
  double K5 = S1 * S1 + 2. * S2;
  K = pow ( K5, 0.2 );
  K3 = K * K * K;
//
  for ( int n = 12; n < 58; n++ )
    {
    double a = an;
    double g = gn;
    double q = qn;
    double f = fn;
    double u = un;
    C[n] = an * pow( G + 1. - g, g ) * pow ( Q * Q + 1. - q, q ) * pow ( F + 1. - f, f ) *  pow ( V, u );
    }
  return Err;
  }

void CISO::SetPT( double _P, double TC )
  {
  WasCalcFi = false;
  WasCalcF1 = false;
  WasCalcF2 = false;
  WasCalcDelta = false;
  WasCalcTau_dFdTau = false;
  WasCalcTau2_dFdTauTau = false;
  WasCalcUHS = false;
  WasCalcCpCv = false;
  WasCalcMu = false;
  P = _P * 98.0665; // кПА
  T = TC + TK;
  Tau = 1. / T;
  R = 8.314510;
//
  B = 0.;
  for ( int n = 0; n < 18; n++ )
    {
    B += Bn( n ) * pow ( Tau, un );
    }
  }

double CISO::Bnij( int n, int i, int j )
  {
  double B = pow ( GGij + 1. - gn, gn ) * 
             pow ( Qi * Qj + 1. - qn, qn ) *
             pow ( sqrt ( Fi * Fj ) + 1. - fn, fn ) * 
             pow ( Si * Sj + 1. - sn, sn ) *
             pow ( Wi * Wj + 1. - wn, wn );
  return B;
  }

double CISO::Bn( int n )
  {
  double B = 0.;
  for ( int i = 0; i < K_BASE; i++ )
    {
    for ( int j = 0; j < K_BASE; j++ )  ///j = 1 ???
      {
      B += Xi * Xj * Bnij( n, i, j ) * pow ( EEij, un ) * pow_3_2( Ki * Kj );
      }
    }
  return an * B;
  }

double CISO::Press( )
  {
  double S = 1. + B * Delta / K3;
  double S2 = 0.;
  for ( int n = 12; n < 18; n++ )
    S2 += C[n] * pow ( Tau, un );
  S -= Delta * S2;
  S2 = 0.;
  for ( int n = 12; n < 58; n++ )
    {
    double Delta_kn = pow ( Delta, kn );
    S2 += C[n] * pow ( Tau, un ) * pow ( Delta, bn ) * ( bn - cn * kn * Delta_kn ) * exp(-cn * Delta_kn );
    }
  S += S2;
  double p = Delta * R / ( Tau * K3 ) * S;
  return p;
  }

void CISO::Tau_dFdTau( )
  {
  if ( WasCalcTau_dFdTau )
    return;
  WasCalcTau_dFdTau = true;
  double S = Tau * dF0dTau( );
  double S2 = 0.;
  for ( int n = 0; n < 18; n++ )
    S2 += un * Bn( n ) * pow ( Tau, un );
  S += S2 * Delta / K3;
//
  S2 = 0.;
  for ( int n = 12; n < 18; n++ )
    S2 += un * C[n] * pow ( Tau, un );
  S -=  Delta * S2;
  S2 = 0.;
  for ( int n = 12; n < 58; n++ )
    {
    double Delta_kn = pow ( Delta, kn );
    S2 += un * C[n] * pow ( Tau, un ) * pow ( Delta, bn ) * exp(-cn * Delta_kn );
    }
  S += S2;
  T_dFdT = S;
  }

void CISO::Tau2_dFdTauTau( )
  {
  if ( WasCalcTau2_dFdTauTau )
    return;
  WasCalcTau2_dFdTauTau = true;
  double S = Tau * Tau * dF0dTauTau( );
  double S2 = 0.;
  for ( int n = 0; n < 18; n++ )
    S2 += ( un - 1. ) * un * Bn( n ) * pow ( Tau, un );
  S += S2 * Delta / K3;
//
  S2 = 0.;
  for ( int n = 12; n < 18; n++ )
    S2 += ( un - 1. ) * un * C[n] * pow ( Tau, un );
  S -=  Delta * S2;
//
  S2 = 0.;
  for ( int n = 12; n < 58; n++ )
    {
    double Delta_kn = pow ( Delta, kn );
    S2 += ( un - 1. ) * un * C[n] * pow ( Tau, un ) * pow ( Delta, bn ) *  exp(-cn * Delta_kn );
    }
  S += S2;
  T2_dFdTT = S;
  }

void CISO::CalcF1()
  {
  if ( WasCalcF1 )
    return;
  WasCalcF1 = true;
  double S = 1. + 2. * B * Delta / K3;
  double S2 = 0.;
  for ( int n = 12; n < 18; n++ )
    S2 += C[n] * pow ( Tau, un );
  S -= 2. * Delta * S2;
  S2 = 0.;
  for ( int n = 12; n < 58; n++ )
    {
    double Delta_kn = pow ( Delta, kn );
    double bn_kn = ( bn - cn * kn * Delta_kn );
    S2 += C[n] * pow ( Tau, un ) * pow ( Delta, bn ) * 
      ( bn - ( 1. + kn ) * cn * kn * Delta_kn + bn_kn * bn_kn ) * 
      exp(-cn * Delta_kn );
    }
  S += S2;
  F1 = S;
  }

void CISO::CalcFi()
  {
  if ( WasCalcFi )
    return;
  WasCalcFi = true;
  double S = F0( );
  S += B * Delta / K3;
//
  double S2 = 0.;
  for ( int n = 12; n < 18; n++ )
    S2 += C[n] * pow ( Tau, un );
  S -= Delta * S2;
//
  S2 = 0.;
  for ( int n = 12; n < 58; n++ )
    {
    double Delta_kn = pow ( Delta, kn );
    S2 += C[n] * pow ( Tau, un ) * pow ( Delta, bn ) *  exp(-cn * Delta_kn );
    }
  S += S2;
  FiS = S;
  }

void CISO::CalcF2()
  {
  if ( WasCalcF2 )
    return;
  WasCalcF2 = true;
  double S2 = 0.;
  for ( int n = 0; n < 18; n++ )
    S2 += ( 1. - un ) * Bn( n ) * pow ( Tau, un );
  double S = 1. + S2 * Delta / K3;
//
  S2 = 0.;
  for ( int n = 12; n < 18; n++ )
    S2 += ( 1. - un ) * C[n] * pow ( Tau, un );
  S -=  Delta * S2;
//
  S2 = 0.;
  for ( int n = 12; n < 58; n++ )
    {
    double Delta_kn = pow ( Delta, kn );
    double bn_kn = ( bn - cn * kn * Delta_kn );
    S2 += ( 1. - un ) * C[n] * pow ( Tau, un ) * pow ( Delta, bn ) * bn_kn *  exp(-cn * Delta_kn );
    }
  S += S2;
  F2 = S;
  }

void CISO::CalcDelta()
  {
  if ( WasCalcDelta )
    return;
  WasCalcDelta = true;
  Delta = P * Tau * K3 / R;
  double Pd = Press( );
  double Db = Delta;
  double De = Delta;
  if ( Pd < P )
    {
    while ( Pd < P )
      {
      Db = De;
      De *= 1.05;
      Delta = De;
      Pd = Press( );
      }
    }
  else
    {
    while ( Pd > P )
      {
      De = Db;
      Db *= 0.95;
      Delta = Db;
      Pd = Press( );
      }
    }
  int kIter = 0;
  while ( kIter++ < 100 )
    {
    Delta = 0.5 * ( Db + De );
    Pd = Press( );
    if ( fabs ( Pd - P ) < 1e-6 )
      break;
    if ( Pd < P )
      Db = Delta;
    else
      De = Delta;
    }
  Ro = Delta / K3;
  D = Ro * M;
  Z = P * Tau * K3 / ( Delta * R ); 
  }

void CISO::CalcUHS()
  {
  if ( WasCalcUHS )
    return;
  WasCalcUHS = true;
  CalcDelta();
  Tau_dFdTau( );
  double RT = R * T;
  U = T_dFdT * RT;
  H = U + Z * RT;
  CalcFi();
  S = U / T - FiS * R;
  }

void CISO::CalcCpCv()
  {
  if ( WasCalcCpCv )
    return;
  WasCalcCpCv = true;
  CalcDelta();
  Tau2_dFdTauTau( );
  Cv = -R * T2_dFdTT;
  CalcF1();
  CalcF2();
  Cp = Cv + R * F2 * F2 / F1;
  }

void CISO::CalcMu()
  {
  if ( WasCalcMu )
    return;
  WasCalcMu = true;
  CalcDelta();
  CalcCpCv();
  CalcF1();
  CalcF2();
  Mu = ( F2 / F1 - 1. ) / ( Cp * Ro );
  Mu *= 1e3;// K/МПа
  }

static CISO ISO;

static bool IsComp[K_GAS];
static double Cmol[K_GAS];
#define SET(G,C) Cmol[G] = C; IsComp[G] = true;

void CalcISO()
  {
  InitISO();
  CLEAR(IsComp)
  CLEAR(Cmol)
  SET(G_N2,0.003)
  SET(G_CO2,0.006)
  SET(G_CH4,0.965)
  SET(G_C2H6,0.018)
  SET(G_C3H8,0.0045)
  SET(G_C4H10N,0.001)
  SET(G_C4H10I,0.001)
  SET(G_C5H12N,0.0003)
  SET(G_C5H12I,0.0005)
  SET(G_C6H14N,0.0007)
//
  int Res = ISO.SetComp( IsComp, Cmol );
 
  ISO.SetPT( 0.101325/0.0980665, 298.15 - TK );
  ISO.CalcDelta();
  ISO.DeltaN = ISO.Delta;
//  ISO.SetPT( 10./0.0980665, 350 - TK );
  ISO.SetPT( 15./0.0980665, 250 - TK );
  ISO.CalcDelta();
  ISO.CalcUHS();
  ISO.CalcCpCv();
  ISO.CalcMu();
  double U = ISO.U / ISO.M;
  double H = ISO.H / ISO.M;
  double S = ISO.S / ISO.M;
  double Cp = ISO.Cp / ISO.M;
  double Cv = ISO.Cv / ISO.M;
  KKK();
  }

#undef  Xi
#undef Xj
#undef Mi
#undef Ei
#undef Ej
#undef Ki
#undef Kj
#undef Fi
#undef Fj
#undef Qi
#undef Qj
#undef Gi
#undef Gj
#undef Si
#undef Sj
#undef Wi
#undef Wj
#undef Gij
#undef GGij
#undef Eij 
#undef EEij
#undef Vij 
#undef Kij 
/////////////////////////
#undef an
#undef bn
#undef cn
#undef gn
#undef qn
#undef fn
#undef kn
#undef un
#undef sn
#undef wn