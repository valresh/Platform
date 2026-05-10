#include "stdafx.h"
#include "math.h"
#include "Chem.h"
#include "IAPWS.h"
#include "Err.h"


int CIF_97::Region ( double T_K, double P_MPa )
  {
  if ( T_K >= 1073.15 )
    {
    if ( P_MPa > 10. )
      return 6;
    return 5;
    }
  if ( P_MPa > 100. )
    {
    Error ( 2 );
    return -1;
    }
  if ( T_K < 273.15 )
    {
    Error ( 2 );
    return -1;
    }
//
  if ( T_K < 623.15 )
    {
// 1 + 2
    double P = Pz ( T_K );
    if ( P_MPa < P )
      return 2;
    else
      return 1;
    }
	double P = P_23_MPa ( T_K );
  if ( P_MPa < P )
    return 2;
  else
    return 3;
  }

// Однофазные состояния
double CIF_97::Ro ( double T_K, double P_MPa )
  {
  int Reg = Region ( T_K, P_MPa );
  switch ( Reg )
    {
    case 1:
      return Ro_1 ( T_K, P_MPa );
    case 2:
      return Ro_2 ( T_K, P_MPa );
    case 3:
      return Ro_3 ( T_K, P_MPa );
    case 5:
      return Ro_5 ( T_K, P_MPa );
    case 6: // По Клапейрону-Менделееву
      return P_MPa * 1e3 * 18. / RG / T_K;
    }
  return 0.;
  }


double CIF_97::Ro_par( double T_K, double P_MPa )
  {
  int Reg = Region ( T_K, P_MPa );
  switch ( Reg )
    {
    case 1:
      return Ro_gas( T_K );
    case 2:
      return Ro_2 ( T_K, P_MPa );
    case 3:
      return Ro_3 ( T_K, P_MPa );
    case 5:
      return Ro_5 ( T_K, P_MPa );
    case 6: // По Клапейрону-Менделееву (на всякий случай)
      return P_MPa*1e3*18./RG/T_K;
    }
  return 0.;
  }

double CIF_97::Ro_mol ( double T_C, double P_atm )
  {
  return Ro ( T_C + TK, P_atm * iMPa_atm );
  }

double CIF_97::U ( double T_K, double P_MPa )
  {
  int Reg = Region ( T_K, P_MPa );
  switch ( Reg )
    {
    case 1:
      return U_1 ( T_K, P_MPa );
    case 2:
      return U_2 ( T_K, P_MPa );
    case 3:
      return U_3 ( T_K, P_MPa );
    case 5:
      return U_5 ( T_K, P_MPa );
    case 6:
      Error ( 2 );
    }
  return 0.;
  }

double CIF_97::U_mol ( double T_C, double P_atm )
  {
  return U ( T_C + TK, P_atm * iMPa_atm ) * Mw_H2O;
  }

double CIF_97::S ( double T_K, double P_MPa )
  {
  int Reg = Region ( T_K, P_MPa );
  switch ( Reg )
    {
    case 1:
      return S_1 ( T_K, P_MPa );
    case 2:
      return S_2 ( T_K, P_MPa );
    case 3:
      return S_3 ( T_K, P_MPa );
    case 5:
      return S_5 ( T_K, P_MPa );
    case 6:
      Error ( 2 );
    }
  return 0.;
  }

double CIF_97::S_mol ( double T_C, double P_atm )
  {
  return S ( T_C + TK, P_atm * iMPa_atm ) * Mw_H2O;
  }

double CIF_97::H ( double T_K, double P_MPa )
  {
  int Reg = Region ( T_K, P_MPa );
  switch ( Reg )
    {
    case 1:
      return H_1 ( T_K, P_MPa );
    case 2:
      return H_2 ( T_K, P_MPa );
    case 3:
      return H_3 ( T_K, Ro_3 ( T_K, P_MPa ));
    case 5:
      return H_5 ( T_K, P_MPa );
    case 6:
      Error ( 2 );
    }
  return 0.;
  }

double CIF_97::H_par ( double T_K, double P_MPa )
  {
  int Reg = Region ( T_K, P_MPa );
  switch ( Reg )
    {
    case 1:
      return H_gas( T_K );
    case 2:
      return H_2 ( T_K, P_MPa );
    case 3:
      return H_3 ( T_K, Ro_3 ( T_K, P_MPa ));
    case 5:
      return H_5 ( T_K, P_MPa );
    case 6:
      Error ( 2 );
    }
  return 0.;
  }

double CIF_97::H_mol ( double T_C, double P_atm )
  {
  return H ( T_C + TK, P_atm * iMPa_atm ) * Mw_H2O;
  }

double CIF_97::Cp ( double T_K, double P_MPa )
  {
  int Reg = Region ( T_K, P_MPa );
  switch ( Reg )
    {
    case 1:
      return Cp_1 ( T_K, P_MPa );
    case 2:
      return Cp_2 ( T_K, P_MPa );
    case 3:
      return Cp_3 ( T_K, P_MPa );
    case 5:
      return Cp_5 ( T_K, P_MPa );
    case 6:
      Error ( 2 );
    }
  return 0.;
  }

double CIF_97::Cp_par ( double T_K, double P_MPa )
  {
  int Reg = Region ( T_K, P_MPa );
  switch ( Reg )
    {
    case 1:
      return Cp_gas( T_K );
    case 2:
      return Cp_2 ( T_K, P_MPa );
    case 3:
      return Cp_3 ( T_K, P_MPa );
    case 5:
      return Cp_5 ( T_K, P_MPa );
    case 6:
      Error ( 2 );
    }
  return 0.;
  }

double CIF_97::Cp_mol ( double T_C, double P_atm )
  {
  return Cp ( T_C + TK, P_atm * iMPa_atm ) * Mw_H2O;
  }

double CIF_97::Cv ( double T_K, double P_MPa )
  {
  int Reg = Region ( T_K, P_MPa );
  switch ( Reg )
    {
    case 1:
      return Cv_1 ( T_K, P_MPa );
    case 2:
      return Cv_2 ( T_K, P_MPa );
    case 3:
      return Cv_3 ( T_K, P_MPa );
    case 5:
      return Cv_5 ( T_K, P_MPa );
    case 6:
      Error ( 2 );
    }
  return 0.;
  }

double CIF_97::Cv_par ( double T_K, double P_MPa )
  {
  int Reg = Region ( T_K, P_MPa );
  switch ( Reg )
    {
    case 1:
      return Cv_gas( T_K );
    case 2:
      return Cv_2 ( T_K, P_MPa );
    case 3:
      return Cv_3 ( T_K, P_MPa );
    case 5:
      return Cv_5 ( T_K, P_MPa );
    case 6:
      Error ( 2 );
    }
  return 0.;
  }

double CIF_97::Cv_mol ( double T_C, double P_atm )
  {
  return Cv ( T_C + TK, P_atm * iMPa_atm ) * Mw_H2O;
  }

// 2-х фазные состояния
double CIF_97::Ro_liq ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return Ro_1 ( T_K, P_MPa );
  double Ro = Ro_3_liq ( T_K, P_MPa );
  return Ro;
  }

double CIF_97::U_liq ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return U_1 ( T_K, P_MPa );
  double Ro = Ro_3_liq ( T_K, P_MPa );
  return U_3 ( T_K, Ro );
  }

double CIF_97::S_liq ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return S_1 ( T_K, P_MPa );
  double Ro = Ro_3_liq ( T_K, P_MPa );
  return S_3 ( T_K, Ro );
  }

double CIF_97::H_liq ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return H_1 ( T_K, P_MPa );
  double Ro = Ro_3_liq ( T_K, P_MPa );
  return H_3 ( T_K, Ro );
  }

double CIF_97::Cp_liq ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return Cp_1 ( T_K, P_MPa );
  double Ro = Ro_3_liq ( T_K, P_MPa );
  return Cp_3 ( T_K, Ro );
  }

double CIF_97::Cv_liq ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return Cv_1 ( T_K, P_MPa );
  double Ro = Ro_3_liq ( T_K, P_MPa );
  return Cv_3 ( T_K, Ro );
  }

double CIF_97::Ro_gas ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return Ro_2 ( T_K, P_MPa );
  double Ro = Ro_3_gas ( T_K, P_MPa );
  return Ro;
  }

double CIF_97::U_gas ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return U_2 ( T_K, P_MPa );
  double Ro = Ro_3_gas ( T_K, P_MPa );
  return U_3 ( T_K, Ro );
  }

double CIF_97::S_gas ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return S_2 ( T_K, P_MPa );
  double Ro = Ro_3_gas ( T_K, P_MPa );
  return S_3 ( T_K, Ro );
  }

double CIF_97::H_gas ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return H_2 ( T_K, P_MPa );
  double Ro = Ro_3_gas ( T_K, P_MPa );
  return H_3 ( T_K, Ro );
  }

double CIF_97::Cp_gas ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return Cp_2 ( T_K, P_MPa );
  double Ro = Ro_3_gas ( T_K, P_MPa );
  return Cp_3 ( T_K, Ro );
  }

double CIF_97::Cv_gas ( double T_K )
  {
  if ( T_K > TP )
    {
    Error ( 1 );
    return 0.;
    }
  double P_MPa = P_4 ( T_K );
  if ( T_K <= 623.15 )
    return Cv_2 ( T_K, P_MPa );
  double Ro = Ro_3_gas ( T_K, P_MPa );
  return Cv_3 ( T_K, Ro );
  }

double CIF_97::Pz ( double T_K )
  {
  return P_4 ( T_K );
  }

// Сложные состояния
double CIF_97::XH ( double T_K, double P_MPa, double dPar )
  {
  ASS(FALSE)
  //if ( T_K >= TP ) dPar = 1.0;
  //if ( dPar >= 1.0 )
  //  return Water.H_par( T_K, P_MPa );
  //if ( dPar <= 0.0 )
  //  return Water.H (T_K,P_MPa);
  //double g = Water.H_gas( T_K );
  //double l = Water.H_liq( T_K );
  //double r = (dPar*g+(1.-dPar)*l);
  //return r;
  return 0.;
  }
//
double CIF_97::XRo( double T_K, double P_MPa, double dPar )
  {
  ASS(FALSE)
  //if ( T_K >= TP ) dPar = 1.0;
  //if ( dPar >= 1.0 )
  //  return Water.Ro_par( T_K, P_MPa );
  //if ( dPar <= 0.0 )
  //  return Water.CIF_97::Ro( T_K, P_MPa );
  //double g = Water.Ro_gas( T_K );
  //double l = Water.Ro_liq( T_K, P_MPa * MPa_atm );
  //double r = (dPar*g+(1.-dPar)*l);
  //return r;
  return 0.;
  }
//
double CIF_97::XCv( double T_K, double P_MPa, double dPar )
  {
  ASS(FALSE)
  //if ( T_K >= TP ) dPar = 1.0;
  //if ( dPar >= 1.0 )
  //  return Water.Cv_par( T_K, P_MPa );
  //if ( dPar <= 0.0 )
  //  return Water.Cv(T_K,P_MPa);
  //double g = Water.Cv_gas( T_K );
  //double l = Water.Cv_liq( T_K );
  //double r = (dPar*g+(1.-dPar)*l);
  //return r;
  return 0.;
  }
//
double CIF_97::XCp( double T_K, double P_MPa, double dPar )
  {
  ASS(FALSE)
  //if ( T_K >= TP ) dPar = 1.0;
  //if ( dPar >= 1.0 )
  //  return Water.Cp_par( T_K, P_MPa );
  //if ( dPar <= 0.0 )
  //  return Water.Cp(T_K,P_MPa);
  //double g = Water.Cp_gas( T_K );
  //double l = Water.Cp_liq( T_K );
  //double r = (dPar*g+(1.-dPar)*l);
  //return r;
  return 0.;
  }
//
