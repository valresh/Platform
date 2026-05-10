#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#include "math.h"
#include "Data.h"
#include "SysDataTypes.h"

//const double T_std =  300. - TK;
// Real !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//CReal ** pGas; 
extern CReal * pWater_NIST;
bool CalcChem_H = false;

CReal::CReal( )
  {
  H298 = 0.;
  GlobalGamma = 1.; 
  }

bool CReal::CHO()
  {
//  if ( strstr ( Formula, "(W)" ))
//    pGas[n]->AsWater = true;
  char * C = strchr ( CGas::Formula, 'C' );
  if ( C == NULL )
    k_C = 0.;
  else
    {
    k_C = atof( C + 1 );
    if ( k_C <= 0. )
      k_C = 1;
    }
  char * H = strchr ( CGas::Formula, 'H' );
  if ( H == NULL || CGas::Formula[2] == 'O' )
    k_H = 0.;
  else
    k_H = atof( H + 1 );
  char * O = strchr ( CGas::Formula, 'O' );
  if ( O == NULL )
    k_O = 0.;
  else
    {
    k_O = atof( O + 1 );
    if ( k_O <= 0. )
      k_O = 1.;
    }
    //    Fsend ( "CHO.txt", "%10s:C=%4.0lf,H=%4.0lf,O=%4.0lf\n", pGas[n]->ChemFormula, pGas[p]->k_C, pGas[p]->k_H, pGas[p]->k_O );
//  char * F = CGas::Formula;
//  k_C,k_H,k_O;
  return true;
  }

double TCp_max = 500 + 273.15;

double CReal::C_IG ( double TC )
  {
  if ( pApprCp )
    return pApprCp->C_IG ( TC );
  double T = ( TC + TK );
  if ( T > TCp_max )
    T = TCp_max;
//	Cp=A+B*T+C*T^2+D*T^3+E*T^4
  double Cp = ((( Cp_E * T + Cp_D ) * T + Cp_C ) * T + Cp_B ) * T + Cp_A;
//  ASS( Cp > 0. )
  return Cp;
  }

double CReal::C_Liq( double TC )
  {
  double T = ( TC + TK );
  //	Cp=A+B*T+C*T^2+D*T^3+E*T^4
  return (( Cp_D_liq * T + Cp_C_liq ) * T + Cp_B_liq ) * T + Cp_A_liq;
  }

double CReal::H_IG ( double TC )
  {
  if ( pApprCp )
    return pApprCp->H_IG ( TC );
  double T = TC + TK;
//  double H = ((( Hp_D * T + Hp_C ) * T + Hp_B ) * T + Hp_A ) * T - Hp_E / T;
  double H;
  if ( T > TCp_max )
    {
    double Tm = TCp_max;
    double Hm = (((( Hp_E * Tm + Hp_D ) * Tm + Hp_C ) * Tm + Hp_B ) * Tm + Hp_A ) * Tm;
    double Cpm = C_IG ( Tm );
    H = Hm + Cpm * ( T - Tm );
    }
  else
    {
    H = (((( Hp_E * T + Hp_D ) * T + Hp_C ) * T + Hp_B ) * T + Hp_A ) * T;
    }
  if ( CalcChem_H )
    return H + Hp_F;
  return H + Hp_F_0;
  }

double CReal::H_Chem ( double TC )
  {
//---  double T = ( TC + TK ) * 1e-3;
//---  double H = ((( Hp_D * T + Hp_C ) * T + Hp_B ) * T + Hp_A ) * T - Hp_E / T;
  double T = TC + TK;
  double H = (((( Hp_E * T + Hp_D ) * T + Hp_C ) * T + Hp_B ) * T + Hp_A ) * T;
  return H + Hp_F;
  }

double CReal::dH_Chem ( )
  {
  return Hp_F - Hp_F_0;
  }

void CReal::Set( CReal * pGas )
  {
//  memmove ( static_cast<CGasData*>( this ), static_cast<CGasData*>( pGas ), sizeof ( CGasData ));
  Init();
  }

double dH_Chem( struct CComp * pComp )
  {
  double * Cmol = pComp->Cmol;
  bool * IsComp = pComp->IsComp;
  double D = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
//      if ( n == G_H2O )
//        D += Cmol[n] * ( Water.dH0_chem - Water.dH0_0 );
//      else
//        if ( Cmol[n] > 1e-7 )
      D += Cmol[n] * pGas[n] -> dH_Chem();
      }
    }
  return D;
  }

double dH_Chem( double Cmol[], bool IsComp[] )
  {
  double D = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
//     if ( n == G_H2O )
//       D += Cmol[n] * ( Water.dH0_chem - Water.dH0_0 );
//      else
//        if ( Cmol[n] > 1e-7 )
      D += Cmol[n] * pGas[n] -> dH_Chem();
      }
    }
  return D;
  }

//#define OLD

double CReal::S_IG( double P, double TC )
  {
#ifdef OLD
  double T = ( TC + TK ) * 1e-3;
  //A*ln(t) + B*t + C*t2/2 + D*t3/3 - E/(2*t2) + G
  double dS = (( S_D * T + S_C ) * T + S_B ) * T + 
    S_A * log ( T ) -
    S_E / ( T * T ) + 
    S_G;
  dS -= RG * log ( P );
#else
  if ( S_G == 0.0 )
    return 0.;
  double T = ( TC + TK );
  double dS = ((( S_E * T + S_D ) * T + S_C ) * T + S_B ) * T + S_A * log ( T ) +  S_G;
  dS -= RG * log ( P );
#endif
  return dS;
  }

void CReal::S_IG( double P, double TC, double & S, double & dSdT )
  {
  //ASS( S_G != 0. )
#ifdef OLD
  double T = ( TC + TK ) * 1e-3;
  double dS = ((( S_E * T + S_D ) * T + S_C ) * T + S_B ) * T + S_A * log ( T ) +  S_G;
  dS -= RG * log ( P );
  S = dS;
  double ddS = ( 3. * S_D * T + 2. * S_C ) * T + S_B + 
    S_A /  T +
    2. * S_E / ( T * T * T );
  dSdT = ddS * 1e-3;
#else
	if ( S_G == 0.0 )
	  {
		S = 0.;
		dSdT = 0.0;
		return;
	  }
  double T = ( TC + TK );
  double dS = ((( S_E * T + S_D ) * T + S_C ) * T + S_B ) * T + S_A * log ( T ) +  S_G;
  dS -= RG * log ( P );
  S = dS;
//  double ddS = ( 3. * S_D * T + 2. * S_C ) * T + S_B + 
//    S_A /  T +
//    2. * S_E / ( T * T * T );
//  dSdT = ddS * 1e-3;
  double dS_dT = (( 4. * S_E * T + 3. * S_D ) * T + 2. * S_C ) * T + S_B + S_A / T;
  dSdT = dS_dT;
#endif
  }


double CReal::G_IG( double P, double TC )
  {
  double T = TC + TK;
  return H_Chem( TC ) - T * S_IG( P, TC );
  }


double CReal::dHobr( double Tc )
  {
  double T = Tc + TK;
  //dHf=A+B*T+C*T^2
  if ( fabs ( Hobr_A ) > 1e10 )
    return 0.;
  double dH = ( Hobr_C * T + Hobr_B ) * T + Hobr_A;
  return dH;
  }

double CReal::dSobr( double Tc )
  {
  double dS = 0.;
//  ASS(0)
  return dS;
  }

double CReal::dGobr( double Tc )
  {
  double T = Tc + TK;
 //dGf=A+B*T+C*T^2
  if ( fabs ( Gobr_A ) > 1e10 )
    return 0.;
  double dG = ( Gobr_C * T + Gobr_B ) * T + Gobr_A;
  return dG;
  }

//double CReal::Vis_gas ( double P, double TC )
//  {
//  double T = TC + TK;
//  double Vis = ( GasVis_C * T + GasVis_B ) * T + GasVis_A;
//  return Vis;
//  }

// Па*c
double CReal::Vis_gas_2 ( double P, double TC )
  {
// VMG аппроксимация даёт неправильные значения
  double T = TC + TK;
// Lee correlation
// reader.elsevier.com/reader/sd/pii/S235285401730075X?token=160C2887C868AC31662005A63DE209EA558587571480900DE4B27410DE00322B2F19E0334C29B8977B82A04C362E7F88
  double T18 = 1.8 * T;
  double X = 3.448 + 986.4 / T18 + 0.01009 * Mw;
  double Y = 2.447 - 0.2224 * X;
  double K = ( 9.379 + 0.01607 * Mw ) * T18 * sqrt ( T18 )/
             ( 209.2 + 19.26 * Mw + T18 );
// P Mw / RT = M/V
  double Ro = P * Mw / ( RP * T ) * 1e-3;
  double Mu = 1e-4 * K * exp ( X * pow ( Ro, Y )); // mPa s
  return Mu * 1e-3;
  }

double CReal::Vis_gas ( double P, double TC )
  {
  // VMG аппроксимация даёт неправильные значения
  double Ksi = pow ( Tcr, 1. / 6. ) / sqrt ( Mw ) * pow ( 0.987 * Pcr, -2. / 3. );
  //Perry  double Ksi = 2.173424e11 * pow ( Tcr, 1. / 6. ) / sqrt ( Mw ) * pow ( Pcr * Pascal, -2. / 3. );
  double T = TC + TK;
  double Tr = T / Tcr;
  double muKsi = 1 + 46.1 * pow ( Tr, 0.618 ) - 20.4 * exp( -0.449 * Tr ) + 19.4 * exp (-4.058 * Tr ); 
  double Mu = muKsi * 1e-5 / Ksi;
  return Mu * 1e-3; // cP
  }

// Па*c
double CReal::Vis_liq ( double TC )
  {
  double T = TC + TK;
  double Log_Vis = ( LiqVis_D * T + LiqVis_C ) * T + LiqVis_A + LiqVis_B / T;
  return exp ( Log_Vis );
  }

double CReal::Lam_gas ( double P, double TC )
  {
  double T = TC + TK;
  double Lam = ( GasLam_C * T + GasLam_B ) * T  + GasLam_A;
  return Lam; // W / m K
  }

double CReal::Lam_liq ( double TC )
  {
  double T = TC + TK;
  double Lam = ( LiqLam_C * T + LiqLam_B ) * T  + LiqLam_A;
  return Lam; // W / m K
  }


void CReal::Init( )
  {
  GlobalGamma = 1.;
//	Cp=A+B*T+C*T^2+D*T^3+E*T^4
  Hp_A = Cp_A;
  Hp_B = Cp_B / 2.; 
  Hp_C = Cp_C / 3.;
  Hp_D = Cp_D / 4.;
  Hp_E = Cp_E / 5.;
  Hp_F = Hp_F_0 = 0.;
  S_A = Cp_A;
  S_B = Cp_B;
  S_C = Cp_C / 2.;
  S_D = Cp_D / 3.; 
//??  S_D = Cp_C / 3.; 
  S_E = Cp_E / 4.;
//??  S_E = Cp_C / 4.;
//??  S_G = 200. + H_vap ( Tb ) / Tb;
  S_G = 0.;
  double H_25 = H_IG( 25. );
//
  if ( fabs ( H298 ) > 1e10 )
    H298 = 0.;
  double Hst = H298;
  Hp_F = Hst - H_25;
  Hp_F_0 = -H_25;
  }

void CReal::SetS_G( double S_298 )
  {
  S_G = 1e-10;
  double S_std = S_IG( 1., 25. ) - 1e-10;
  S_G = S_298 - S_std;
  }

void SetS_G( int ID, double S_298 )
  {
  if ( ID < 0 || ID > LAST_GAS )
    return;
  pGas[ID]->SetS_G( S_298 );
  }

const double To_Ln = log ( 10. );
const double Ln_kPa = log ( 98.0665 );
double CReal::Max_Pz = 100.;
double CReal::Pz ( double Tc )
  {
//  log(P) = A + B / T +C log(T)+ D * T + E * T^2
//    T - K
//    P - kPa
//    log = log10
//	ASS(FALSE);
  double T = Tc + TK;
//  if ( T > A_Tmax )
//    return 100.;
  double L_10 = A_A + A_B / T + A_C * log10( T ) +  ( A_E * T + A_D ) * T;
  double Ln = L_10 * To_Ln - Ln_kPa;
  double P = exp ( Ln );
  if ( P > Max_Pz )
    P = Max_Pz + LimdX( P - Max_Pz, 50. );
  return P;// * GlobalGamma;
  }

double CReal::dPz_dT ( double Tc )
  {
  double T = Tc + TK;
  double L_10 = A_A + A_B / T + A_C * log10( T ) +  ( A_E * T + A_D ) * T;
  double dL_10_dT = -A_B / ( T * T ) + A_C / T  +  2. * A_E * T + A_D;
  double Ln = L_10 * To_Ln - Ln_kPa;
  double dLn_dT = dL_10_dT * To_Ln;
  double P = exp ( Ln ); //GlobalGamma 
  double dP_dT = P * dLn_dT;
  return dP_dT;
  }

double CReal::H_v ( double TC )
  {
  return H_IG ( TC );
  }

double CReal::H_l ( double TC )
  {
  return H_IG ( TC ) - H_vap ( TC );
  }


double CReal::dH_1_dTC( double TC )
  {
  double dHVap = 0.;
  if ( Hvap_B > 0. )
    {
    double Arg = 1. - ( TC + TK ) / Hvap_B; 
    if ( Arg > 0. )
      dHVap = Hvap_A * Hvap_N / Hvap_B * pow( Arg, Hvap_N - 1);
    }
	return C_IG(TC) + dHVap;
  }


typedef
double (*tRo_Water) ( double Tc, double P );

__declspec (dllexport) tRo_Water pRo_Water = NULL;
__declspec (dllexport) double Press_For_Ro_Water = 100.;


bool CReal::Ro_liq ( double T_C, double & Ro )
  {
  if ( Numb == G_H2O && pSys->WaterIAPWS && pRo_Water )                              
    {
    Ro = (*pRo_Water) ( T_C, Press_For_Ro_Water );
    return (Ro != 0.0);
    }
//  double CWater::Rol ( double Tc, double P )

//  Ro = A * B^(1-T/Tc)^N
  double T = T_C + TK;
  if ( T >= RoLiq_Tc )
    {
    Ro = 0.;
    return false;
    }
  double Pow = pow ( 1. - T / RoLiq_Tc, RoLiq_N );
  Ro = RoLiq_A * pow ( RoLiq_B, -Pow ); 
  return true;
  }

bool CReal::To_m3_liq ( double TC, double & To_m3 )
  {
  double Ro;
  if ( Ro_liq ( TC, Ro ) )
    {
    To_m3 = Mw / Ro;
    return true;
    }
  return false;
  }

double CReal::H_vap ( double TC )
  {
  double T = TC + TK;
  if ( Hvap_B > 1e50 || T >= Hvap_B )
    return 0.;
  double H = Hvap_A * pow ( 1. - T / Hvap_B, Hvap_N );
  return H;
  }

void CGas::Print ( void * F )
  {

  }

void CGas::SetS_G( double S_298 )
  {
  //ASS(0)
  }

void CReal::Print ( void * F )
  {
  // Fprintf ( F, "%12.8lf;%12.8lf;%12.8lf;%12.8lf;%12.8lf;%12.8lf;%12.8lf;\n",
  //           Hp_A, Hp_B, Hp_C, Hp_D, Hp_E, Hp_F, Hp_F_0 );
  // Fprintf ( F, "%12.8lf;%12.8lf;%12.8lf;%12.8lf;%12.8lf;%12.8lf;%12.8lf;\n",
  //   Hvap_A, Hvap_B, Hvap_N, RoLiq_A, RoLiq_B, RoLiq_N, RoLiq_Tc );

  }

//enum eFunctions { 
//  eHv   = 0x0001, // C_IG, H_IG,
//  eHl   = 0x0002, // H_l, Hvap
//  ePz   = 0x0004, // Pz
//  eRo   = 0x0008, // Ro_liq, To_m3_liq
//  eS    = 0x0010, // S_IG, G_IG 
//  eChem = 0x0020, // H_Chem, dH_Chem
//  eObr  = 0x0040, // dHobr, dSObr, dGobr 
//  };

#define TST(Val,Mask) if ( Val == -12321 ) { ASS(0); MaskErr |= Mask; }

  int CReal::ReadData( _HANDLE hFile )
  {
// Ключ уже прочитан
  DWORD MaskErr = 0;
//  DWORD  Function; // показывает какие функции реализованы 
//
  DWORD Lr;
  ReadFile ( hFile, static_cast<CRealDBData*>(this), sizeof ( CRealDBData ), &Lr, NULL );
  strcpy_s ( CGas::Name, 128, CRealDBData::Name );
#ifdef LINUX
  ToUTF8( CGas::Name, 128 );
#endif
  strcpy_s ( CGas::Alt_Name, 128, CRealDBData::AltName );
#ifdef LINUX
  ToUTF8( CGas::Alt_Name, 128 );
#endif
  strcpy_s ( CGas::Formula, 16, CRealDBData::Formula );
  char * N1 = CRealDBData::Name;
  char * N2 = CGas::Name;
  Cp_A = Poly5IdealGasCpA; TST(Cp_A,eHv)
  Cp_B = Poly5IdealGasCpB; TST(Cp_B,eHv)
  Cp_C = Poly5IdealGasCpC; TST(Cp_C,eHv)
  Cp_D = Poly5IdealGasCpD; TST(Cp_D,eHv)
  Cp_E = Poly5IdealGasCpE; TST(Cp_E,eHv)
  CpTmin = Poly5IdealGasCpTmin;
  CpTmax = Poly5IdealGasCpTmax;
  Cp_A_liq = LiquidHeatCapacityA; TST(Cp_A_liq,eHl)
  Cp_B_liq = LiquidHeatCapacityB; TST(Cp_B_liq,eHl)
  Cp_C_liq = LiquidHeatCapacityC; TST(Cp_C_liq,eHl)
  Cp_D_liq = LiquidHeatCapacityD; TST(Cp_D_liq,eHl)
  CpTmin_liq = LiquidHeatCapacityTmin;
  CpTmax_liq = LiquidHeatCapacityTmax;
  //
  A_A = Antoine5LogA; TST(A_A,ePz)
  A_B = Antoine5LogB; TST(A_B,ePz)
  A_C = Antoine5LogC; TST(A_C,ePz)
  A_D = Antoine5LogD; TST(A_D,ePz)
  A_E = Antoine5LogE; TST(A_E,ePz)
  A_Tmin = Antoine5LogTmin;
  A_Tmax = Antoine5LogTmax;
  //
  Hobr_A = IdealGasEnthalpyOfFormationA; TST(Hobr_A,eObr)
  Hobr_B = IdealGasEnthalpyOfFormationB; TST(Hobr_B,eObr)
  Hobr_C = IdealGasEnthalpyOfFormationC; TST(Hobr_C,eObr)
  Hobr_Tmin = IdealGasEnthalpyOfFormationTmin;
  Hobr_Tmax = IdealGasEnthalpyOfFormationTmax;
  H298 = IdealGasEnthalpyOfFormation_298; TST(H298,eObr)
  if ( fabs ( H298 ) > 1e10 )
    {
    if ( strcmp ( CGas::Name, "HYDROGEN" ) == 0 )
      H298 = 0.;
    if ( strcmp ( CGas::Name, "2-methylbutene" ) == 0 )
      H298 = -3.51E+05;
    if ( strcmp ( CGas::Name, "3,3-dimethylheptane" ) == 0 )
      H298 = -2.33E+05;
    }
//    H298 = 0.;
//  Fsend ( "H.csv", "%s;%s;%12.5le;\n", CGas::Formula, CGas::Name, H298 );
  //
  Gobr_A = IdealGasGibbsFreeEnergyOfFormationA; TST(Gobr_A,eObr)
  Gobr_B = IdealGasGibbsFreeEnergyOfFormationB; TST(Gobr_B,eObr)
  Gobr_C = IdealGasGibbsFreeEnergyOfFormationC; TST(Gobr_C,eObr)
  Gobr_Tmin = IdealGasGibbsFreeEnergyOfFormationTmin;
  Gobr_Tmax = IdealGasGibbsFreeEnergyOfFormationTmax;
  G298 = IdealGasGibbsFreeEnergyOfFormation_298;
  //
  Hvap_A = EnthalpyOfVapourizationA; TST(Hvap_A,eHl)
  Hvap_B = EnthalpyOfVapourizationB; TST(Hvap_B,eHl)
  Hvap_N = EnthalpyOfVapourizationN; TST(Hvap_N,eHl)
  Hvap_Tmin = EnthalpyOfVapourizationTmin;
  Hvap_Tmax = EnthalpyOfVapourizationTmax;
  //
  RoLiq_A = RackettLiquidDensityA; TST(RoLiq_A,eRo)
  RoLiq_B = RackettLiquidDensityB; TST(RoLiq_B,eRo)
  RoLiq_N = RackettLiquidDensityN; TST(RoLiq_N,eRo)
  RoLiq_Tmin = RackettLiquidDensityTmin;
  RoLiq_Tmax = RackettLiquidDensityTmax;
  RoLiq_Tc = RackettLiquidDensityTc;
//
  GasVis_A = GasViscosityA;
  GasVis_B = GasViscosityB;
  GasVis_C = GasViscosityC;
  GasVis_Tmin = GasViscosityTmin;
  GasVis_Tmax = GasViscosityTmax;
  LiqVis_A = LiquidViscosityA;
  LiqVis_B = LiquidViscosityB;
  LiqVis_C = LiquidViscosityC;
  LiqVis_D = LiquidViscosityD;
  LiqVis_Tmin = LiquidViscosityTmin;
  LiqVis_Tmax = LiquidViscosityTmax;
  //
  GasLam_A = GasThermalConductivityA;
  GasLam_B = GasThermalConductivityB;
  GasLam_C = GasThermalConductivityC;
  GasLam_Tmin = GasThermalConductivityTmin;
  GasLam_Tmax = GasThermalConductivityTmax;
  LiqLam_A = LiquidThermalConductivityA;
  LiqLam_B = LiquidThermalConductivityB;
  LiqLam_C = LiquidThermalConductivityC;
  LiqLam_Tmin = LiquidThermalConductivityTmin;
  LiqLam_Tmax = LiquidThermalConductivityTmax;
//
  Mw = MolecularWeight;
  Tb = NormalBoilingPoint - TK;
  Tcr = CriticalTemperature;
  Pcr = CriticalPressure * 1.0197266112955116733203828053699e-2;
  Vcr = CriticalVolume;
  Rocr = CriticalDensity;
  Zcr = CriticalCompressibility;
  Omega = AcentricFactor;
  Function = ~MaskErr;
  CHO();
  return 0;
  }

#define this pMain
#include "IO_Parms.h"

 void CReal::GetParams ( IBaseModel * pMain )
  {
  Char<256> Txt;
  Txt.Prt( "/%s(%s)", CGas::Name, Alt_Name );
  TAB( Txt, 1)
  Txt.Prt( "|%s|T кипения", CGas::Name );
  PARM( Tb, Txt );
  Txt.Prt( "|%s|Мол. вес", CGas::Name );
  PARM( Mw, Txt );
  ETAB
  }
#undef this
