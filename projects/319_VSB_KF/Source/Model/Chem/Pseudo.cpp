#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#include "math.h"
#include "Err.h"

bool UsePseudoAppr = true;

double f_Ash ( double T )
  {
  return 1250. / ( sqrt ( T * T + 108000 ) - 307.6 ) - 1.; 
  }

double df_Ash_dT ( double T )
  {
  double sq = sqrt ( T * T + 108000 );
  double zn = ( sq - 307.6 );
  return -1250. / ( zn * zn ) * T / sq; 
  }

CPseudo::CPseudo( ) : CGas ( )
  {
  }

void SetPseudo ( double Tb, CPseudo * pPseudo );//, void * F )

void CPseudo::Init_appr()
  {
  SetPseudo ( Tb, this );
  Ro = Ro_20_4 + 0.0035 / Ro_20_4;
  sq_Ro = sqrt ( Ro );
  // Cp, Hv
  Kw = pow ( 1.8 * TbK, 1. / 3. ) / SG;
  double A0 = -1.41779 + 0.11828 * Kw; 
  double A1 = -( 6.99724 - 8.69326 * Kw + 0.27715 * Kw * Kw ) * 1e-4;
  double A2 = -2.2582e-6; 
  double B0 = 1.09223 - 2.48245 * Omega; 
  double B1 = -(3.434 - 7.14 * Omega ) * 1e-3; 
  double B2 = -(7.2661 - 9.2561 * Omega ) * 1e-7; 
  double C = ( 12.8 - Kw ) * ( 10 - Kw ) / ( 10. * Omega );
  C = C * C;
  Cp_A2 = Mw * ( A2 - C * B2 );
  Cp_A1 = Mw * ( A1 - C * B1 );
  Cp_A0 = Mw * ( A0 - C * B0 );
  Hv_A2 = Cp_A2 / 3.;
  Hv_A1 = Cp_A1 / 2.;
  Hv_A0 = Cp_A0;
  double T25 = 25. + TK;
  Hv_25 = -((( Hv_A2 * T25 + Hv_A1 ) * T25 + Hv_A0 ) * T25 );
  //
  double Pz_Tc13 = pow ( Tcr, 1.3 );
  double Pz_Tb13 = pow ( TbK, 1.3 );
  Pz_B = log ( Pcr / 1.01325 ) / ( 1. / Pz_Tb13 - 1. / Pz_Tc13 );
  Pz_A = 0.013163 + Pz_B / Pz_Tb13; 
//
  double Cp_L_Kw = pow ( 1.8 * TbK, 1. / 3. ) / SG;
  Cp_L_a = 1.4651 + 0.2302 * Cp_L_Kw;
  Cp_L_b = 0.306469 - 0.16734 * SG;
  Cp_L_c = 0.001467 - 0.000551 * SG;

  double n = (Mw - 2) / 14;
  k_C = n;
  k_H = 2 * n + 2; 
  }

static double Clog = log ( 10. );
//static double Cadd = log ( 0.980665 );
static double CaddPa = log ( 98066.5 / Pstd );
// Тепло
// КДж / Kмоль
static double K = 12.;
void CPseudo::Init( )
  {
  GlobalGamma = 1.;
  if ( Name[0] == 0 )
    sprintf_s ( Name, 64, "Tb %d", (int)Tb );
//
//
  double T = 20. + 273.15;
//  Ro_20_4 = Ro0 * pow ( TbK * 0.01, 0.13 );
  // Ro_15_15
  double Ro0 = 0.65 + 0.09 * 0.8;
  Ro0 *= 867. / 826.68;
  Ro_20_4 = Ro0 * pow ( TbK * 0.01, 0.13 );
  Ro = Ro_20_4 + 0.0035 / Ro_20_4;
  sq_Ro = sqrt ( Ro );
//
//  Mw = 60. + 0.3 * Tb + 0.001 * Tb * Tb;  // Формула Воинова
  //kC =  (int)( Mw / 14. );
 // kH =  2 * kC + 2;
//
  PzA = 7.6715;
  PzB = 2.68;
  PzC = f_Ash ( Tb + TK );
  //	ToSI();
  PzA *= Clog;
  PzB *= Clog;
  PzA -= CaddPa;
  dH_v25 = 0.;
  dH_v25 = H_v ( 25. );
//
  Init_appr();
  };

double CPseudo::Pz ( double Tc )
  {
  if ( UsePseudoAppr )
    return GlobalGamma * Pz_appr( Tc );
  double T = Tc + TK;
  double lP = PzA - PzB * f_Ash ( T ) / PzC;
  return GlobalGamma * exp ( lP );
  }

// page 266, 323 Korsten
double CPseudo::Pz_appr ( double Tc )
  {
  double T = Tc + TK;
  double T13 = pow ( T, 1.3 );
  double lP = Pz_A - Pz_B / T13;
  return GlobalGamma * exp ( lP );
  }

double CPseudo::Pz_appr_2 ( double TC )
  {
  double T = TC + TK;
  double Tbr = TbK / Tcr;
  double Tr = T / Tcr;
  double logPr = log ( Pcr / 1.01325 ) * Tbr / ( 1. - Tbr ) * ( 1. - 1. / Tr );
  return exp ( logPr ) * Pcr;
  }

//double f_Ash ( double T )
//  {
//  return 1250. / ( sqrt ( T * T + 108000 ) - 307.6 ) - 1.; 
//  }

double CPseudo::dPz_dT ( double Tc )
  {
  if ( UsePseudoAppr )
    {
    ASS(0)
    }
  double T = Tc + TK;
  double T2 = T * T + 108000;
  double sqT2 = sqrt ( T2 );
  double Zn =  sqT2 - 307.6;
  double f = 1250. / Zn - 1.; 
  double df_dT = -1250. * T / ( Zn * Zn * sqT2 );
  double lP = PzA - PzB * f / PzC;
  double dlP_dT = -PzB * df_dT / PzC;
  double P = GlobalGamma * exp ( lP );
  double dP_dT = P * dlP_dT;
  ASS( dP_dT > 0.)
  return dP_dT;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double CPseudo::H_l ( double T )
  {
  if ( UsePseudoAppr )
    return H_IG_appr( T ) - H_vap_appr ( T, 2 );
  //			T += TK;
  // Крэг 157
  //			double H = // кДж/кг
  //				(( 0.7616 + 0.00167 * T ) * T * ( 2.563 - 0.343 * K + 0.0177 * K * K ) - 334.27 ) / sq_Ro;
  double H = // кКал/кг
    ( 0.403 + 0.000405 * T ) * T / sq_Ro;
  return 4.1868 * H * Mw - dH_v25;	// кДж/кМоль
  }


double CPseudo::dH_1_dTC( double TC )
{
	if ( UsePseudoAppr )
	{
		double Tbr = TbK / Tcr;
		double dHvap_Tb = RG * Tcr * Tbr * ( 3.978 * Tbr - 3.958 + 1.555 * log( Pcr ) )/ ( 1.07 - Tbr );
		return C_IG_appr( TC ) - TbK == 0.0 ? 0.0 : dHvap_Tb * 0.38 / TbK * pow(1.0 / (1.0 - TbK / Tcr) - (TC + TK) / TbK, -0.62);
	}
	return 4.1868 * Mw * (0.00081* TC + 0.403 ) / sq_Ro;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double CPseudo::C_Liq ( double T )
  {
  if ( UsePseudoAppr )
    return C_Liq_appr ( T );
  //			T += TK;
  // Крэг	( стр. 63, 85 )
  //			double C =  // кДж/кг
  //				( 0.7616 + 0.00334 * T ) * ( 2.563 - 0.343 * K + 0.0177 * K * K )/ sq_Ro;
  double C = // кКал/кг
    ( 0.403 + 0.00081 * T )/ sq_Ro;
  return 4.1868 * C * Mw;	// кДж/кМоль
  }

//Kesler and Lee page 334
double CPseudo::C_Liq_appr ( double TC )
  {
  double T = TC + TK;
  double Cp = Cp_L_a * ( Cp_L_b + Cp_L_c * T );
  return Cp * Mw;
  }
// API page 335
double CPseudo::C_Liq_appr_2 ( double TC )
  {
  double T = TC + TK;
  double Kw = pow ( 1.8 * TbK, 1. / 3. ) / SG;
  double A1 = -4.90383 + ( 0.099319 + 0.104281 * SG ) * Kw +  ( 4.81407 - 0.194833* Kw ) / SG;
  double A2 =  (7.53624 + 6.214610 * Kw ) * ( 1.12172 - 0.27634 / SG ) * 1e-4;
  double A3 = -(1.35652 + 1.11863 * Kw ) * ( 2.9027 - 0.70958 / SG ) * 1e-7;
  double Cp =  ( A3 * T + A2 ) * T + A1;
  return Cp * Mw;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double CPseudo::H_IG ( double TC )
  {
  if ( UsePseudoAppr )
    return H_IG_appr( TC );
  return H_v ( TC );
  }

double CPseudo::H_v ( double T )
  {
  if ( UsePseudoAppr )
    return H_IG_appr( T );
  //			T += TK;
  // Крэг(стр.85)
  //			double H =  // кДж/кг
  //				(( 0.7616 + 0.00167 * T ) * T * ( 2.563 - 0.343 * K + 0.0177 * K * K ) - 334.27 ) / sq_Ro;
  //			H *= M;	// кДж/кМоль
  // Кистяковский R (стр. 72, 85)
  //			double R = // кДж/кМоль
  //				T * ( 36.63 + 19.138 * log10( T ) );
  //			return H + R;
  double H =  // кКал/кг
    ( 50.2 + 0.109 * T + 0.00014 * T * T ) * ( 4 - Ro ) - 73.8;
  return 4.1868 * H * Mw - dH_v25;	// кДж/кМоль
  }

double CPseudo::H_vap ( double TC )
  {
  if ( UsePseudoAppr )
    return H_vap_appr ( TC, 2 );
  return H_v ( TC ) - H_l ( TC );
  }

// Рекомендации page 340
double CPseudo::H_vap_appr ( double TC, int Variant )
  {
  double T = TC + TK;
  if ( T >= Tcr - 0.1 )
    return 0.;
  double Tbr = TbK / Tcr;
  double Tr = T / Tcr;
  double dHvap_Tb;
// Riedel
  switch ( Variant )
    {
    case 1:
      dHvap_Tb = 1.095 * RG * Tcr * Tbr * ( log ( Pcr ) - 1.013 )/ ( 0.93 - Tbr );
    break;
    case 2:
      dHvap_Tb = RG * Tcr * Tbr * ( 3.978 * Tbr - 3.958 + 1.555 * log( Pcr ) )/ ( 1.07 - Tbr );
    break;
    case 3:
      dHvap_Tb = 37.32315 * pow ( TbK, 1.14086 ) * pow ( SG, 9.77089e-3 );
    break;
    }
  double dHvap = dHvap_Tb * pow ( ( 1. - T / Tcr )/( 1. - TbK / Tcr ), 0.38 );
  return dHvap;
  }

bool CPseudo::Ro_liq ( double T_C, double & _Ro )
  {
  _Ro = Ro_liq ( T_C );
//
  return true;
  }

double  CPseudo::Ro_liq ( double T_C )
  {
//  double Ro_20 = 0.983719e3 * pow ( TbK, 0.002016 ) * pow ( SG, 1.0055 ); // page 80
//  double Ro_20 = 0.995e3 * SG;
  double Ro = 0.999 * SG - LimdX( 1e-3 * ( 2.34 - 1.898 * SG ) * ( T_C - 15.5 ), 0.3 );  // CHARACTERIZATION AND PROPERTIES OF PETROLEUM FRACTIONS p 223
  return Ro * 1000.;
  }

bool CPseudo::To_m3_liq ( double TC, double & To_m3 )
  {
  double _Ro = Ro_liq ( TC );
  To_m3 = Mw / _Ro;
  return true;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double l10 = log ( 10. );
double CPseudo::C_IG ( double T )
  {
  if ( UsePseudoAppr )
    return C_IG_appr ( T );
  //			T += TK;
  // Крэг	( стр. 63, 85 )
  //			double C = // кДж/кг
  //				( 0.7616 + 0.00334 * T ) * ( 2.563 - 0.343 * K + 0.0177 * K * K )/ sq_Ro;
  // Крэг	( стр. 67 )
  //			C -= 0.3768 / Ro;
  //			C *= M;	// кДж/кМоль
  // Кистяковский R (стр. 72, 85)
  //			double dR = ( 36.63 + 19.138 * log10( T ) ) + 19.138 / l10;
  //			return C + dR;
  double C =  // кКал/кг K
    ( 0.109 + 0.00028 * T ) * ( 4 - Ro );
  return 4.1868 * C * Mw;	// кДж/кМоль K
  }


double CPseudo::C_IG_appr ( double TC )
  {
// Kesler and Lee page 257
  //double Kw = pow ( 1.8 * TbK, 1. / 3. ) / SG;
  //double A0 = -1.41779 + 0.11828 * Kw; 
  //double A1 = -( 6.99724 - 8.69326 * Kw + 0.27715 * Kw * Kw ) * 1e-4;
  //double A2 = -2.2582e-6; 
  //double B0 = 1.09223 - 2.48245 * Omega; 
  //double B1 = -(3.434 - 7.14 * Omega ) * 1e-3; 
  //double B2 = -(7.2661 - 9.2561 * Omega ) * 1e-7; 
  //double C = ( 12.8 - Kw ) * ( 10 - Kw ) / ( 10. * Omega );
  //C = C * C;
  //double T = TC + TK;
  //double Cp = Mw * ( ( A2 * T + A1 ) * T + A0 - C * ( ( B2 * T + B1 ) * T + B0 ));
  double T = TC + TK;
  double Cp = ( Cp_A2 * T + Cp_A1 ) * T + Cp_A0;
  return Cp;
  }

double  CPseudo::H_IG_appr ( double TC )
  {
  double T = TC + TK;
  double H = (( Hv_A2 * T + Hv_A1 ) * T + Hv_A0 ) * T + Hv_25;
  return H;
  }


double CPseudo::Vis_gas ( double P, double TC )
  {
  // Yoon-Thodos Perry 548
  // page 345
  double Ksi = pow ( Tcr, 1. / 6. ) / sqrt ( Mw ) * pow ( 0.987 * Pcr, -2. / 3. );
//Perry  double Ksi = 2.173424e11 * pow ( Tcr, 1. / 6. ) / sqrt ( Mw ) * pow ( Pcr * Pascal, -2. / 3. );
  double T = TC + TK;
  double Tr = T / Tcr;
  double muKsi = 1 + 46.1 * pow ( Tr, 0.618 ) - 20.4 * exp( -0.449 * Tr ) + 19.4 * exp (-4.058 * Tr ); 
  double Mu = muKsi * 1e-5 / Ksi;
  return Mu * 1e-3;  // Pa s
  }

double CPseudo::Vis_liq ( double TC )
  {
  double a1 = 14.69 * pow ( TbK, 0.0684 ) * pow ( SG, 0.267 );
  double T = TC + TK;
  double a2 = 3.682 * log ( T );
  double ll = a1 - a2;
  double l = exp ( ll ) - 0.8;
  double Eta = exp ( l ) * 1e-6;
  return Eta * SG * 1000.;
  }

double CPseudo::Lam_gas ( double P, double TC )
  {
//W / m K
// Perry's Chemical Engineering  page 513
  double Cv = C_IG( TC ) - RG;
  double EtaCv = Vis_gas ( P, TC );
  double k = ( 1.15 + 2.033 * ( RG / Cv )) * EtaCv / Mw;
  return k; // W / m K
  }



double CPseudo::Lam_liq ( double TC )
  {
  //  Сато
  // Perry's Chemical Engineering  page 513
  double Lam_Tb =  1.11 / sqrt ( Mw );
  double T = TC + TK;
  double Tr = T / Tcr;
  double Tbr = TbK / Tcr;
  double Lam = Lam_Tb * ( 3. + 20. * pow ( 1. - Tr, 2. / 3. )) / ( 3. + 20. * pow ( 1. - Tbr, 2. / 3. ));
  return Lam; // W / m K
  }

double CPseudo::dHobr( double Tc )
  {
  ASS(0)
  return 0.;
  }
double CPseudo::dSobr( double Tc )
  {
  ASS(0)
  return 0.;
  }
double CPseudo::dGobr( double Tc )
  {
  ASS(0)
  return 0.;
  }
double CPseudo::H_Chem ( double TC )
  {
  ASS(0)
  return 0.;
  }
double CPseudo::dH_Chem ( )
  {
  return 0.;
  }

double CPseudo::S_IG( double P, double TC )
  {
 // ASS(0)
  return 0.;
  }

void CPseudo::S_IG( double P, double TC, double & S, double & dSdT )
  {

//  ASS(0)
  S = 0.;
  dSdT = 0.;

  }

double CPseudo::G_IG( double P, double TC )
  {
  ASS(0)
  return 0.;
  }

void CPseudo::Print ( void * F )
  {
//  Fprintf ( F, "%12.8lf;%12.8lf;%12.8lf;%12.8lf;\n",
//            Mw, Tb, Ro, sq_Ro );
  }

  int CPseudo::ReadData( _HANDLE hFile )
  {
  CPseudoDBData Data;
  DWORD Lr;
  ReadFile ( hFile, &Data, sizeof ( Data ), &Lr, NULL );
  Tb = Data.Tb;
  TbK = Tb + TK;
  char AltName[64];
  ReadFile ( hFile, AltName, sizeof ( AltName ), &Lr, NULL );
  strcpy ( Alt_Name, AltName );
#ifdef LINUX
  ToUTF8( Alt_Name, 128 );
#endif
  char Appr[64];
  ReadFile ( hFile, Appr, sizeof ( Appr ), &Lr, NULL );
  strcpy ( Name, Appr );
#ifdef LINUX
  ToUTF8( Name, 128 );
#endif
///////////////////////////////////////////////
//  ASS( Mw_Tb( Appr, TbK ));
  Init();
  return 0;
  }

#define this pMain
#include "IO_Parms.h"
void CPseudo::GetParams ( IBaseModel * pMain )
  {
  Char<256> Txt;
  Txt.Prt( "/%s(%s)", CGas::Name, Alt_Name );
  TAB( Txt, 1)
  Txt.Prt( "|%s|T кипения", Name );
  PARM( Tb, Txt );
  Txt.Prt( "|%s|Мол. вес", Name );
  PARM( Mw, Txt );
  ETAB
  }
#undef this

extern int nFirstPseudo;

void TestPseudo ( )
  {
//   int n = 0;
//   double P = 1.;
//   double TC = 200.;
//   void * F = Fopen ( "Pseudo.csv", "wt" );
//   for ( int n = nFirstPseudo; n < LAST_GAS; n++ )
//     {
//     CPseudo * pP = (CPseudo*)pGas[n];
//     Fprintf ( F, "%3.0lf;", pP->Tb );
// //
//     double Cp0 = pP->C_IG( TC );
//     double Cp1 = pP->C_IG_appr( TC );
//     Fprintf ( F, "CpG;%5.3lf;%5.3lf;", Cp0, Cp1 );
// //
//     double Hv0 = pP->H_IG( TC );
//     double Hv1 = pP->H_IG_appr( TC );
//     Fprintf ( F, "Hv;%5.3lf;%5.3lf;", Hv0, Hv1 );
// //
//     Cp0 = pP->C_Liq( TC );
//     Cp1 = pP->C_Liq_appr( TC );//!!!
//     double Cp2 = pP->C_Liq_appr_2( TC );
//     Fprintf ( F, "CpL;%5.3lf;%5.3lf;%5.3lf;", Cp0, Cp1, Cp2 );
// //
//     double Pz0 = pP->Pz ( TC );
//     double Pz1 = pP->Pz_appr ( TC );//!!!
//     double Pz2 = pP->Pz_appr_2 ( TC );
//     Fprintf ( F, "Pz;%7.5le;%7.5le;%7.5le;", Pz0, Pz1, Pz2 );
// //
//     double Ro0 = pP->Ro_20_4;
//     double T = 20. + TK;
//     double Ro1 = 0.999 * pP->SG - 1e-3 * ( 2.34 - 1.898 * pP->SG ) * ( T - 288.7 );;
//     Fprintf ( F, ";Ro20;%5.3lf;%5.3lf;", Ro0, Ro1 );
// //
//     double Mw0 = 60. + 0.3 * pP->Tb + 0.001 * pP->Tb * pP->Tb;;
//     double Mw1 = pP->Mw;
//     Fprintf ( F, "Mw;%5.3lf;%5.3lf;", Mw0, Mw1 );
// //
//     double Hvap0 = pP->H_vap ( TC + TK );
//     double Hvap1 = pP->H_vap_appr ( TC, 1 );
//     double Hvap2 = pP->H_vap_appr ( TC, 2 ); //!!!!!!!!!
//     double Hvap3 = pP->H_vap_appr ( TC, 3 );
//     Fprintf ( F, "Hvap;%5.3lf;%5.3lf;%5.3lf;%5.3lf;", Hvap0, Hvap1, Hvap2, Hvap3 );
// //
//     double Hl0 = pP->H_l ( TC );
//     double Hl1 = pP->H_IG_appr( TC ) - pP->H_vap_appr ( TC, 2 );
//     Fprintf ( F, "Hl;%5.3lf;%5.3lf;", Hl0, Hl1 );
// //
//     Fprintf ( F, "\n" );
//     }
//   Fclose ( F );
  KKK();
  }

struct CNcData  // page 176
  {
  int  nC;
  double Mw, Tb, SG, n20, Ro_20_4, Tcr, Pcr, Vcr, Zcr, Omega, Sigma, Delta;
  double Tb_Nc ( double Nc )
    {
    return 1090 - exp ( 6.9955 - 0.11193 * pow ( Nc, 2. / 3. ));
    }
  double SG_Nc ( double Nc )
    {
    return 1.07 - exp ( 3.65097 - 3.8864 * pow ( Nc, 0.1 ));
    }
  };

CNcData NcData[] = {
6,   84, 337, 0.690, 1.395, 0.686, 510.3, 34.4, 0.241, 0.275, 0.255, 18.6, 7.25, 
7,   95, 365, 0.727, 1.407, 0.723, 542.6, 31.6, 0.245, 0.272, 0.303, 21.2, 7.41, 
8,  107, 390, 0.749, 1.417, 0.743, 570.2, 29.3, 0.246, 0.269, 0.346, 23.0, 7.53, 
9,  121, 416, 0.768, 1.426, 0.762, 599.0, 26.9, 0.247, 0.265, 0.394, 24.4, 7.63, 
10, 136, 440, 0.782, 1.435, 0.777, 623.7, 25.0, 0.251, 0.261, 0.444, 25.4, 7.71, 
11, 149, 461, 0.793, 1.442, 0.790, 645.1, 23.5, 0.254, 0.257, 0.486, 26.0, 7.78, 
12, 163, 482, 0.804, 1.448, 0.802, 665.5, 21.9, 0.256, 0.253, 0.530, 26.6, 7.83, 
13, 176, 500, 0.815, 1.453, 0.812, 683.7, 20.6, 0.257, 0.249, 0.570, 27.0, 7.88, 
14, 191, 520, 0.826, 1.458, 0.822, 700.9, 19.6, 0.262, 0.245, 0.614, 27.5, 7.92, 
15, 207, 539, 0.836, 1.464, 0.831, 716.5, 18.5, 0.267, 0.241, 0.661, 27.8, 7.96, 
16, 221, 556, 0.843, 1.468, 0.839, 732.1, 17.6, 0.269, 0.237, 0.701, 28.1, 7.99, 
17, 237, 573, 0.851, 1.472, 0.847, 745.6, 16.7, 0.274, 0.233, 0.746, 28.3, 8.02, 
18, 249, 586, 0.856, 1.475, 0.852, 758.8, 15.9, 0.274, 0.229, 0.779, 28.5, 8.05, 
19, 261, 598, 0.861, 1.478, 0.857, 771.1, 15.2, 0.275, 0.226, 0.812, 28.6, 8.07, 
20, 275, 611, 0.866, 1.481, 0.862, 782.7, 14.7, 0.278, 0.222, 0.849, 28.8, 8.09, 
21, 289, 624, 0.871, 1.484, 0.867, 793.8, 14.0, 0.281, 0.219, 0.880, 28.9, 8.11, 
22, 303, 637, 0.876, 1.486, 0.872, 804.9, 13.5, 0.283, 0.215, 0.914, 29.0, 8.13, 
23, 317, 648, 0.881, 1.489, 0.877, 814.2, 13.0, 0.287, 0.212, 0.944, 29.1, 8.15, 
24, 331, 660, 0.885, 1.491, 0.880, 824.1, 12.5, 0.289, 0.209, 0.977, 29.2, 8.17, 
25, 345, 671, 0.888, 1.493, 0.884, 833.3, 12.0, 0.291, 0.206, 1.007, 29.3, 8.18, 
26, 359, 681, 0.892, 1.495, 0.888, 841.7, 11.7, 0.295, 0.203, 1.034, 29.3, 8.20, 
27, 373, 691, 0.896, 1.497, 0.891, 850.2, 11.3, 0.298, 0.200, 1.061, 29.4, 8.21, 
28, 387, 701, 0.899, 1.499, 0.895, 858.2, 10.9, 0.301, 0.197, 1.091, 29.4, 8.22, 
29, 400, 710, 0.902, 1.501, 0.898, 865.5, 10.6, 0.303, 0.194, 1.116, 29.5, 8.24, 
30, 415, 720, 0.905, 1.503, 0.901, 873.5, 10.2, 0.306, 0.191, 1.146, 29.5, 8.25, 
31, 429, 728, 0.909, 1.504, 0.904, 880.1, 10.0, 0.310, 0.189, 1.169, 29.6, 8.26, 
32, 443, 737, 0.912, 1.506, 0.906, 887.4, 9.7, 0.312, 0.187, 1.195, 29.6, 8.27, 
33, 457, 745, 0.915, 1.507, 0.909, 894.0, 9.5, 0.316, 0.184, 1.218, 29.7, 8.28, 
34, 471, 753, 0.917, 1.509, 0.912, 900.2, 9.2, 0.319, 0.182, 1.244, 29.7, 8.29, 
35, 485, 760, 0.920, 1.510, 0.914, 906.1, 9.0, 0.323, 0.180, 1.263, 29.7, 8.30, 
36, 499, 768, 0.922, 1.511, 0.916, 912.2, 8.8, 0.325, 0.177, 1.289, 29.8, 8.31, 
37, 513, 775, 0.925, 1.512, 0.918, 917.7, 8.6, 0.328, 0.175, 1.311, 29.8, 8.32, 
38, 528, 782, 0.927, 1.514, 0.920, 923.1, 8.3, 0.332, 0.173, 1.333, 29.8, 8.33, 
39, 542, 789, 0.929, 1.515, 0.922, 928.6, 8.2, 0.335, 0.171, 1.355, 29.8, 8.34, 
40, 556, 795, 0.931, 1.516, 0.924, 933.4, 8.0, 0.338, 0.169, 1.374, 29.9, 8.35, 
41, 570, 802, 0.933, 1.517, 0.926, 938.8, 7.8, 0.341, 0.167, 1.396, 29.9, 8.35, 
42, 584, 808, 0.934, 1.518, 0.928, 943.6, 7.7, 0.344, 0.165, 1.415, 29.9, 8.36, 
43, 599, 814, 0.936, 1.519, 0.930, 948.4, 7.5, 0.348, 0.164, 1.434, 29.9, 8.36, 
44, 614, 820, 0.938, 1.520, 0.932, 952.5, 7.4, 0.353, 0.163, 1.448, 29.9, 8.37, 
45, 629, 826, 0.940, 1.521, 0.933, 956.9, 7.2, 0.356, 0.160, 1.470, 29.9, 8.38, 
46, 641, 831, 0.941, 1.522, 0.935, 961.6, 7.1, 0.358, 0.159, 1.489, 30.0, 8.38, 
47, 656, 836, 0.943, 1.523, 0.936, 965.7, 7.0, 0.362, 0.158, 1.504, 30.0, 8.39, 
48, 670, 841, 0.944, 1.524, 0.938, 969.4, 6.9, 0.366, 0.156, 1.522, 30.0, 8.39, 
49, 684, 846, 0.946, 1.524, 0.939, 973.5, 6.8, 0.369, 0.155, 1.537, 30.0, 8.40, 
50, 698, 851, 0.947, 1.525, 0.940, 977.2, 6.6, 0.372, 0.153, 1.555, 30.0, 8.40,
-1,  };



#undef SET 
#define SET(Var) \
  pPseudo->Var = NcData[nB].Var + ( NcData[nE].Var - NcData[nB].Var ) / ( NcData[nE].Tb - NcData[nB].Tb ) * ( T - NcData[nB].Tb );
//  Fprintf ( F, "%5.3lf;", pPseudo->Var );

void SetPseudo ( double Tb, CPseudo * pPseudo )//, void * F )
  {
  double T = Tb + TK;
  int nE = -1;
  int n = 0;
  while ( 1 )
    {
    int nC = NcData[n].nC;
    if ( nC < 0 )
      break;
    if ( NcData[n].Tb >= T )
      {
      nE = n;
      break;
      }
    n++;
    }
  int nB;
  if ( nE < 0 )
    nE = n - 1;
  if ( nE == 0 )
    nE = 1;
  nB = nE -1;
  SET(nC)
  if ( UsePseudoAppr )
    {
    SET(Mw)
    }
//  SET(Tb) 
  SET(SG) 
  //    SET(n20) 
  if ( UsePseudoAppr )
    {
    SET(Ro_20_4)
    }
  SET(Tcr) 
  SET(Pcr) 
  SET(Vcr) 
  SET(Zcr) 
  SET(Omega) 
  //    SET(Sigma) 
  //    SET(Delta)
  pPseudo->Tb = Tb;
  pPseudo->TbK = Tb + TK;
  pPseudo->Ro_20_4 *= 1000.;
  }

#undef SET 
#define SET(Var) \
  pPseudo->Var = NcData[n].Var;\
  Fprintf ( F, "%5.3lf;", pPseudo->Var );

int nFirstPseudo;
void CreateAllPseudo ( int & nGas )
  {
  nFirstPseudo = nGas;
  int n = 0;
//  void * F = Fopen ( "Data_Tb.csv", "wt" );
  for ( double Tb = 160; Tb <= 580; Tb += 20. )
    {
//    Fprintf ( F, "%3.0lf;", Tb );
    CPseudo * pPseudo = new CPseudo();
    pPseudo->Numb = nGas;
    SetPseudo ( Tb, pPseudo );//, F );
    pPseudo->Init( );
    sprintf_s ( pPseudo->Name, 64, "Tb %3.0lf", pPseudo->Tb );
    pGas[nGas++] = pPseudo;
    n++;
//    Fprintf ( F, "\n" );
    }
//  Fclose ( F );
  }

