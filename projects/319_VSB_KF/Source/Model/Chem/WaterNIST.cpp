#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#include "math.h"
#include "Data.h"


class CWater_NIST : public CReal
  {
  public:
    //
  CWater_NIST();
  virtual void    Init();
  virtual double  C_IG ( double TC );
  virtual double  C_Liq ( double TC );
  virtual double  H_IG ( double TC );
  virtual double  H_Chem ( double TC );
  virtual void    SetS_G( double S_298 );
  virtual double  S_IG( double P, double TC );
  virtual void    S_IG( double P, double TC, double & S, double & dSdT );
  virtual double  Pz ( double Tc );
  virtual double  dPz_dT ( double Tc );
  virtual double  H_v ( double TC );
  virtual double  H_l ( double TC );
  NEW
  };


CWater_NIST Water_NIST;
CReal * pWater_NIST = &Water_NIST;

double wCp_A = 30.09200;
double wCp_B = 6.832514;
double wCp_C = 6.793435;
double wCp_D = -2.534480;
double wCp_E = 0.082139;
double wCp_F = -250.8810;
double wCp_G = 223.3967;
double wCp_H = -241.8264;
//////////////
double wCp_A_liq = -203.6060;
double wCp_B_liq = 1523.290;
double wCp_C_liq = -3196.413;
double wCp_D_liq = 2474.455;
double wCp_E_liq = 3.855326;
double wCp_F_liq = -256.5478;
double wCp_G_liq = -488.7163;
double wCp_H_liq = -285.8304;

double wH_A_liq;
double wH_B_liq;
double wH_C_liq;
double wH_D_liq;
double wH_E_liq;
double wH_F_liq;
double wH_G_liq;
double wH_H_liq;
double Hp_F_liq;
double Hp_F_0_liq;

CWater_NIST::CWater_NIST()
  {
  }
void    CWater_NIST::Init()
  {
  Hp_A = wCp_A;
  Hp_B = wCp_B / 2.; 
  Hp_C = wCp_C / 3.;
  Hp_D = wCp_D / 4.;
  Hp_E = wCp_E / 5.;
  Hp_F = wCp_F;
  wH_A_liq = wCp_A;
  wH_B_liq = wCp_B / 2.; 
  wH_C_liq = wCp_C / 3.;
  wH_D_liq = wCp_D / 4.;
  wH_E_liq = wCp_E / 5.;
  wH_F_liq = wCp_F;
  wH_H_liq = wCp_H;
  S_A = wCp_A;
  S_B = wCp_B;
  S_C = wCp_C / 2.;
  S_D = wCp_D / 3.; 
  S_E = wCp_E / 2.;
  S_G = wCp_G;
  double H_25 = H_IG( 25. );
//
//  H298 = wCp_H * 1e3;
  double Hst = H298;
  Hp_F = Hst - H_25;
  Hp_F_0 = -H_25;
//
  }
///////////////////////////
double  CWater_NIST::C_IG ( double TC )
  {
  double T = ( TC + TK ) * 1e-3;
  return (( wCp_D * T + wCp_C ) * T + wCp_B ) * T + wCp_A + wCp_E / ( T * T ) ;
  }

double  CWater_NIST::H_IG ( double TC )
  {
  double T = ( TC + TK ) * 1e-3;
  double H = ((( Hp_D * T + Hp_C ) * T + Hp_B ) * T + Hp_A ) * T - Hp_E / T;
  if ( CalcChem_H )
    return 1000. * H + Hp_F;
  return 1000. * H + Hp_F_0;
  }

double  CWater_NIST::H_v ( double TC )
  {
  double T = ( TC + TK ) * 1e-3;
  double H = ((( Hp_D * T + Hp_C ) * T + Hp_B ) * T + Hp_A ) * T - Hp_E / T;
  if ( CalcChem_H )
    return 1000. * H + Hp_F;
  return 1000. * H + Hp_F_0;
  }
///////////////////////////
double  CWater_NIST::H_Chem ( double TC )
  {
  double T = ( TC + TK ) * 1e-3;
  double H = ((( Hp_D * T + Hp_C ) * T + Hp_B ) * T + Hp_A ) * T - Hp_E / T;
  return 1000. * H + Hp_F;
  }
///////////////////////////
void CWater_NIST::SetS_G( double S_298 )
  {
  }

double  CWater_NIST::S_IG( double P, double TC )
  {
  double T = ( TC + TK ) * 1e-3;
  //A*ln(t) + B*t + C*t2/2 + D*t3/3 - E/(2*t2) + G
  double dS = (( S_D * T + S_C ) * T + S_B ) * T + 
    S_A * log ( T ) -
    S_E / ( T * T ) + 
    S_G;
  dS -= RG * log ( P );
  return dS;
  }

void    CWater_NIST::S_IG( double P, double TC, double & S, double & dSdT )
  {
  double T = ( TC + TK ) * 1e-3;
  double dS = ((( S_E * T + S_D ) * T + S_C ) * T + S_B ) * T + S_A * log ( T ) +  S_G;
  dS -= RG * log ( P );
  S = dS;
  double ddS = ( 3. * S_D * T + 2. * S_C ) * T + S_B + 
    S_A /  T +
    2. * S_E / ( T * T * T );
  dSdT = ddS * 1e-3;
  }
///////////////////////////
double wA_A = 3.55959;
double wA_B = 643.748;
double wA_C = -198.043;
const double To_Ln = log ( 10. );

double  CWater_NIST::Pz ( double Tc )
  {
  double T = Tc + TK;
  double L_10 = wA_A - wA_B / ( T + wA_C );
  double Ln = L_10 * To_Ln;
  double P = exp ( Ln );
  if ( P > Max_Pz )
    P = Max_Pz + LimdX( P - Max_Pz, 50. );
  return P * GlobalGamma;
  }

double  CWater_NIST::dPz_dT ( double Tc )
  {
  double T = Tc + TK;
  double L_10 = wA_A - wA_B / ( T + wA_C );
  double dL_10_dT = -A_B / ( ( T + wA_C ) * ( T + wA_C ) );
  double Ln = L_10 * To_Ln;
  double dLn_dT = dL_10_dT * To_Ln;
  double P = GlobalGamma * exp ( Ln );
  double dP_dT = P * dLn_dT;
  return dP_dT;
  }

///////////////////////////
double  CWater_NIST::C_Liq ( double TC )
  {
  double T = ( TC + TK ) * 1e-3;
  return (( wCp_D_liq * T + wCp_C_liq ) * T + wCp_B_liq ) * T + wCp_A_liq + wCp_E_liq / ( T * T );
  }


double  CWater_NIST::H_l ( double TC )
  {
  return H_IG ( TC ) - H_vap ( TC );
//  double T = ( TC + TK ) * 1e-3;
//  double H = ((( wH_D_liq * T + wH_C_liq ) * T + wH_B_liq ) * T + wH_A_liq ) * T + wH_E_Liq / ( T * T );
  }
