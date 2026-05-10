#include "stdafx.h"
#include "math.h"
#include "Chem.h"
#include "IAPWS.h"
#include "Err.h"
#include "CommProc.h"
#include "Water.h"
#include "ChemBase.h"

#define APPR

CWater __declspec(dllexport) Water;

static CIF_97 W;

double Ro_Water ( double Tc, double P )
  {
  double Ro = Water.Rol ( Tc, P );
  return Ro;
  }


CWater::CWater()
  {
  dH0_chem = dH0_0 = 0.;
  _H = 0.;
	Err = 0;
	Water_appr.pErr = (DWORD*)&Err;
  }

double CWater::Cpv ( double Tc, double P )
  {
	Err = 0;
	return Water_appr.Cp_v_P ( Tc, P );
  }

double CWater::Cpv_97 ( double Tc, double P )
  {
	Err = 0;
  double T_K = Tc + TK;
  double P_MPa = P * iMPa_atm;
  ASS ( P_MPa < 100. )
  ASS ( T_K > 273.15 )
	if ( P_MPa <= 10. )
		{
		ASS ( T_K <= 2273.15 )
		}
	else
		{
		ASS ( T_K <= 1073.15 )
		}
	if ( T_K > 1073.15 )
		{
		return W.Cp_5 ( T_K, P_MPa ) * Mw_H2O;
		}
//
  if ( T_K < 623.15 )
    {
    return W.Cp_2 ( T_K, P_MPa ) * Mw_H2O;
    }
	double P_23 = W.P_23_MPa ( T_K );
  if ( P_MPa < P_23 )
    return  W.Cp_2 ( T_K, P_MPa ) * Mw_H2O;
  else
    {
    double Ro_gas = W.Ro_3_gas ( T_K, P_MPa ); 
    return  W.Cp_3 ( T_K, Ro_gas ) * Mw_H2O;
    }
  }

double CWater::Cpl ( double Tc, double P )
  {
	Err = 0;
	return Water_appr.Cp_l_P ( Tc, P );
  }

double CWater::Cpl_97 ( double Tc, double P )
  {
	Err = 0;
  double T_K = Tc + TK;
  ASS ( T_K <= 1073.15 )
  ASS ( T_K > 273.15 )
  if ( T_K >= TP )
    {
    return Cpv ( Tc, P );
    }
  double P_MPa = P * iMPa_atm;
  if ( T_K < 623.15 )
    return W.Cp_1 ( T_K, P_MPa ) * Mw_H2O;
  double Ro_liq = W.Ro_3_liq ( T_K, P_MPa ); 
  return W.Cp_3 ( T_K, Ro_liq ) * Mw_H2O;
  }

double CWater::Hv_97 ( double Tc, double P )
	{
	Err = 0;
	double Pm = Pz_97 ( Tc );
	if ( P > Pm )
		{
		Err |= ERR_P_MAX;
		P = Pm;
		}
  double T_K = Tc + TK;
  double P_MPa = P * iMPa_atm;
  ASS ( P_MPa < 100. )
	ASS ( T_K > 273.15 )
	if ( P_MPa <= 10. )
		{
		ASS ( T_K <= 2273.15 )
		}
	else
		{
		ASS ( T_K <= 1073.15 )
		}
	if ( T_K > 1073.15 )
		{
		if ( CalcChem_H )
			return W.H_5 ( T_K, P_MPa ) * Mw_H2O + dH0_chem;
		return W.H_5 ( T_K, P_MPa ) * Mw_H2O + dH0_0;
		}
//
  if ( T_K < 623.15 )
    {
		if ( CalcChem_H )
			return W.H_2 ( T_K, P_MPa ) * Mw_H2O + dH0_chem;
		return W.H_2 ( T_K, P_MPa ) * Mw_H2O + dH0_0;
    }
	double P_23 = W.P_23_MPa ( T_K );
  if ( P_MPa < P_23 )
		{
		if ( CalcChem_H )
			return  W.H_2 ( T_K, P_MPa ) * Mw_H2O + dH0_chem;
    return  W.H_2 ( T_K, P_MPa ) * Mw_H2O + dH0_0;
		}
  else
    {
    double Ro_gas = W.Ro_3_gas ( T_K, P_MPa ); 
		if ( CalcChem_H )
			return  W.H_3 ( T_K, Ro_gas ) * Mw_H2O + dH0_chem;
    return  W.H_3 ( T_K, Ro_gas ) * Mw_H2O + dH0_0;
    }
	}

double CWater::Hv ( double Tc, double P )
  {
	Err = 0;
	if ( CalcChem_H )
		return Water_appr.H_v_P ( Tc, P ) + dH0_chem;
	return Water_appr.H_v_P ( Tc, P ) + dH0_0;
  }


double CWater::Sv ( double Tc, double P )
  {
	Err = 0;
  double T_K = Tc + TK;
  double P_MPa = P * iMPa_atm;
  if ( T_K > 1073.15 )
    T_K = 1073.15;
  ASS ( P_MPa < 100. )
  //ASS ( T_K > 273.15 )
  if(T_K < TK)
	  T_K = TK;
//                           Klavisin46
  if ( T_K < 623.15 )
    {
    return W.S_2 ( T_K, P_MPa ) * Mw_H2O;
    }
	double P_23 = W.P_23_MPa ( T_K );
  if ( P_MPa < P_23 )
    return  W.S_2 ( T_K, P_MPa ) * Mw_H2O;
  else
    {
    double Ro_gas = W.Ro_3_gas ( T_K, P_MPa ); 
    return  W.S_3 ( T_K, Ro_gas ) * Mw_H2O;
    }
  }

double CWater::Hl_97 ( double Tc, double P )
	{
	Err = 0;
///////////////////////////////////////////////////
  double T_K = Tc + TK;
  double P_MPa = P * iMPa_atm;
  ASS ( T_K <= 1073.15 )
  ASS ( P_MPa < 100. )
  ASS ( T_K > 273.15 )
  if ( T_K >= TP )
    {
    return Hv_97 ( Tc, P );
    }
	double Pm = Pz_97 ( Tc );
	if ( P < Pm )
		{
		P = Pm;
		P_MPa = P * iMPa_atm;
		Err |= ERR_P_MIN;
		}
  if ( T_K < 623.15 )
		{
		if ( CalcChem_H )
			return W.H_1 ( T_K, P_MPa ) * Mw_H2O + dH0_chem;
    return W.H_1 ( T_K, P_MPa ) * Mw_H2O + dH0_0;
		}
  double Ro_liq = W.Ro_3_liq ( T_K, P_MPa ); 
	if ( CalcChem_H )
		return W.H_3 ( T_K, Ro_liq ) * Mw_H2O + dH0_chem;
  return W.H_3 ( T_K, Ro_liq ) * Mw_H2O + dH0_0;
	}


double CWater::Hl ( double Tc, double P )
  {
	Err = 0;
	if ( CalcChem_H )
		return Water_appr.H_l_P ( Tc, P ) + dH0_chem;
	return Water_appr.H_l_P ( Tc, P ) + dH0_0;
  }

double CWater::Sl ( double Tc, double P )
  {
	Err = 0;
  double T_K = Tc + TK;
  double P_MPa = P * iMPa_atm;
  ASS ( T_K <= 1073.15 )
  ASS ( P_MPa < 100. )
  //ASS ( T_K > 273.15 )
  if(T_K < TK)
	  T_K = TK;
  if ( T_K >= TP )
    {
    return Sv ( Tc, P );
    }
  if ( T_K < 623.15 )
    return W.S_1 ( T_K, P_MPa ) * Mw_H2O;
  double Ro_liq = W.Ro_3_liq ( T_K, P_MPa ); 
  return W.S_3 ( T_K, Ro_liq ) * Mw_H2O;
  }

double CWater::Rov ( double Tc, double P )
  {
	Err = 0;
  double T_K = Tc + TK;
  double P_MPa = P * iMPa_atm;
  ASS ( T_K <= 1073.15 )
  ASS ( P_MPa < 100. )
  ASS ( T_K > 273.15 )
//
  if ( T_K < 623.15 )
    {
    return W.Ro_2 ( T_K, P_MPa );
    }
	double P_23 = W.P_23_MPa ( T_K );
  if ( P_MPa < P_23 )
    return  W.Ro_2 ( T_K, P_MPa );
  else
    {
    double Ro_gas = W.Ro_3_gas ( T_K, P_MPa ); 
    return Ro_gas;
    }
  }

double CWater::Rol ( double Tc, double P )
  {
	Err = 0;
  double T_K = Tc + TK;
  double P_MPa = P * iMPa_atm;
  ASS ( T_K <= 1073.15 )
  ASS ( P_MPa < 100. )
  if ( T_K < 273.16 )
    T_K = 273.16;
  if ( T_K >= TP )
    {
    return Rov ( Tc, P );
    }
  if ( T_K < 623.15 )
    return W.Ro_1 ( T_K, P_MPa );
  double Ro_liq = W.Ro_3_liq ( T_K, P_MPa ); 
  return Ro_liq;
  }

double CWater::Pz ( double Tc )
  {
	Err = 0;
	return Water_appr.P_z ( Tc );
  }

double CWater::Tz ( double P_atm )
  {
	Err = 0;
	return Water_appr.T_z ( P_atm );
  }

double CWater::Pz_97 ( double Tc )
  {
	Err = 0;
 ///////////////////////////////////////
  double T_K = Tc + TK;
  if ( T_K >= TP - 0.1 )
    return 1e10;
  return W.Pz ( T_K ) * MPa_atm;
  }

double CWater::Tz_97 ( double P_atm )
  {
	Err = 0;
  return W.T_4 ( P_atm * iMPa_atm ) - TK;
  }

//double CWater::Ro_liq ( double Tc, double P )
//  {
//  return CIF_97::Ro ( Tc + TK, P * iMPa_atm );
//  }

void CWater::SetH()
  {
#ifdef APPR
	ASS(Water_appr.ReadData())
#endif
	dH0_chem = dH0_0 = 0.;
	double P = Pz ( 25. );
  double Hst_v = Hv ( 25., 0.001 );
  double Hst_l = Hl ( 25., 50. );
	_H = -250.8810;
	dH0_chem = _H * 1e3 - Hst_v;
	dH0_0 =	-Hst_v;
	double R = Hst_v - Hst_l;
  Hst_v = Hv ( 25., Pstd );
	Hst_l = Hl ( 25., Pstd );
	R = Hst_v - Hst_l;
  Hst_v = Hv ( 100., Pstd );
  Hst_l = Hl ( 100., Pstd );
  R = ( Hst_v - Hst_l ) / Mw_H2O;
	R = 0.;
  }

//double CWater::Tv ( double hv, double P, double T0 )
//	{
//	Err = 0;
//	return Water_appr.iH_v_T ( hv, P );
//	}

double CWater::Tv_97 ( double hv, double P, double T0 )
	{
	Err = 0;
	double T = T0;
	double dT = 1e10;
	while ( fabs ( dT ) > 1e-5 )
		{
		double dh = hv - Hv_97 ( T, P );
		if ( Err )
			break;
		double cp = Cpv_97 ( T, P );
		if ( Err )
			break;
		dT = dh / cp;
		T += LimdX ( dT, 5. );
		if ( T < 0.1 )
			{
			T = 0.1;
			Err |= ERR_T_MIN;
			break;
			}
		}
	return T;
	}

//double CWater::Tl ( double hl, double P, double T0 )
//	{
//	Err = 0;
//	return Water_appr.iH_l_T ( hl, P );
//	}

double CWater::Tl_97 ( double hl, double P, double T0 )
	{
	Err = 0;
	double T = T0;
	double dT = 1e10;
	while ( fabs ( dT ) > 1e-5 )
		{
		double dh = hl - Hl_97 ( T, P ); 
		if ( Err )
			break;
		double cp = Cpl_97 ( T, P );
		if ( Err )
			break;
		dT = dh / cp;
		T += LimdX ( dT, 5. );
		if ( T < 0.1 )
			{
			T = 0.1;
			Err |= ERR_T_MIN;
			break;
			}
		}
	return T;
	}


double H_IG_W ( double P, double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double S = 0.;
  double H = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0. )
        Cmol[n] = 0.;
      S += Cmol[n];
      if ( n == G_H2O )
        H += Cmol[n] * Water.Hv( Tc, P );
      else
        H += Cmol[n] * pGas[n]->H_IG ( Tc );
      }
    }
  if ( S < 1e-5 ) 
    {
    return 0.;
    }
  return H / S;
  }

class CGasEx : public CReal
  {
  public:
    virtual  double Cpv ( double Tc, double P ) = 0;
    virtual  double Cpl ( double Tc, double P ) = 0;
    virtual  double Hv ( double Tc, double P ) = 0;
    virtual  double Hl ( double Tc, double P ) = 0;
    virtual  double Sv ( double Tc, double P ) = 0;
    virtual  double Sl ( double Tc, double P ) = 0;
    virtual  double Rov ( double Tc, double P ) = 0;
    virtual  double Rol ( double Tc, double P ) = 0;
  };


struct CWater_IAPWS : public CGasEx
  {
  CWater_IAPWS();
  void Init(); //0 
  double C_IG ( double TC );	//Функция нахождения теплоемкости при постоянном давлении
  double H_IG ( double TC );	//Функция нахождения энтальпии
  double H_l ( double TC );
  double Pz ( double Tc );
  double H_vap ( double TC );
  bool   Ro_liq ( double T_C, double & Ro );
  bool   To_m3_liq ( double TC, double & To_m3 );
  
  double S_IG( double P, double TC );	
  void   S_IG( double P, double TC, double & S, double & dSdT ) { ASS(0); S = 0.; dSdT = 0.; }
  double G_IG( double P, double TC );
  
  double dHobr( double Tc ) { return 0.; };
  double dSobr( double Tc ) { return 0.; };
  double dGobr( double Tc ) { return 0.; };
  
  double H_Chem ( double TC ) { return 0.; };
  double dH_Chem ( ) { return 0.; };
#ifdef LINUX
  int ReadData( QFile & hFile );
#else
  int ReadData( _HANDLE hFile );
#endif

//
  double Cpv ( double Tc, double P );
  double Cpl ( double Tc, double P );
  double Hv ( double Tc, double P );
  double Hl ( double Tc, double P );
  double Sv ( double Tc, double P );
  double Sl ( double Tc, double P );
  double Rov ( double Tc, double P );
  double Rol ( double Tc, double P );
  NEW
  };

CWater_IAPWS::CWater_IAPWS()
  {
  Init();
  }

void CWater_IAPWS::Init()
  {
  Function = eHv|eHl|ePz|eRo|eS|eChem;
  GlobalGamma = 1.;
  Mw = Mw_H2O;
  Tb = 100.;
  Tcr = 647.;
  Pcr = 220.64; // bar
  Vcr = 0.; 
  Rocr = 17.9; // Моль/л
  Zcr = 1.;
  Omega = 1.;
  lstrcpy ( CGas::Name, "WATER" );
  lstrcpy ( CGas::Alt_Name, "Вода" );
  lstrcpy ( CGas::Formula, "H2O" );
  AsWater = true;
  Numb = -1;
  k_C = 0.;
  k_H = 2.;
  k_O = 1.;
  }

CGas * Create_Water_IAPWS()
  {
  return new CWater_IAPWS();
  }

const double Pvap = 1.;
const double Pliq = 100.;

double CWater_IAPWS::C_IG ( double TC )
  {
  return Water.Cpv ( TC, Pvap );
  }

double CWater_IAPWS::H_IG ( double TC )
  {
  return Water.Hv ( TC, Pvap );
  }
double CWater_IAPWS::H_l ( double TC )
  {
  return Water.Hl ( TC, Pliq );
  }

double CWater_IAPWS::Pz ( double Tc )
  {
  return GlobalGamma * Water.Pz ( Tc );
  }

double CWater_IAPWS::H_vap ( double TC )
  {
  return H_IG ( TC ) - H_l ( TC );
  }

bool CWater_IAPWS::Ro_liq ( double T_C, double & Ro )
  {
  Ro = Water.Rol ( T_C, Pliq );
  return true;
  }

bool CWater_IAPWS::To_m3_liq ( double TC, double & To_m3 )
  {
  double Ro = Water.Rol ( TC, Pliq );
  To_m3 = Mw / Ro;
  return true;
  }

double CWater_IAPWS::S_IG( double P, double TC )
  {
  return Water.Sv ( TC, P );
  }

double CWater_IAPWS::G_IG( double P, double TC )
  {
  return H_IG ( TC ) - ( TC + TK ) * S_IG ( P, TC );
  }

#ifdef LINUX
  int CWater_IAPWS::ReadData( QFile & hFile )
#else
  int CWater_IAPWS::ReadData( _HANDLE hFile )
#endif
  {
  return 1;
  }

double CWater_IAPWS::Cpv ( double Tc, double P )
  {
  return Water.Cpv (  Tc,  P );
  }
double CWater_IAPWS::Cpl ( double Tc, double P )
  {
  return Water.Cpl (  Tc,  P );
  }
double CWater_IAPWS::Hv ( double Tc, double P )
  {
  return Water.Hv (  Tc,  P );
  }
double CWater_IAPWS::Hl ( double Tc, double P )
  {
  return Water.Hl (  Tc,  P );
  }
double CWater_IAPWS::Sv ( double Tc, double P )
  {
  return Water.Sv (  Tc,  P );
  }
double CWater_IAPWS::Sl ( double Tc, double P )
  {
  return Water.Sl (  Tc,  P );
  }
double CWater_IAPWS::Rov ( double Tc, double P )
  {
  return Water.Rov (  Tc,  P );
  }
double CWater_IAPWS::Rol ( double Tc, double P )
  {
  return Water.Rol (  Tc,  P );
  }
