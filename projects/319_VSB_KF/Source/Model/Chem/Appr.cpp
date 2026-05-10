#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "Err.h"

extern bool UsePseudoAppr;
#if 0 

#undef IF
#define IF( APPR ) 
//if ( strcmpi ( Appr, APPR ) == 0 )

void Prt( int Numb, void * F, double Calc, double Etalon )
  {
  Fprintf ( F, "%d;", Numb );
  double Err = 100. * ( Calc - Etalon ) / Etalon;
  Fprintf ( F, "%3.1lf;", Err );
  }


double CPseudo::Vis_liq_2 ( double TC )
  {
  //  Viscosity of Liquids: Theory, Estimation, Experiment, and Data
  // Авторы: Dabir S. Viswanath, Tushar K. Ghosh, Dasika H.L. Prasad, Nidamarty V.K. Dutt,
  double b = -5.745 - ( 40.468 / nC ) + 0.616 * log ( (double)nC );
  double T = TC + TK;
  double ll = 100 * pow ( 0.01 * T, b );
  double l = pow ( 10., ll ) - 0.8;
  double Eta = pow ( 10., l ) * 1e-6;
  return Eta * SG * 1000.;  // cSt
  }

// Eta[Pa*s] = 46.1 * Tr
double CPseudo::Vis_gas_2 ( double P, double T_C )
  {
  // Lee correlation
  // reader.elsevier.com/reader/sd/pii/S235285401730075X?token=160C2887C868AC31662005A63DE209EA558587571480900DE4B27410DE00322B2F19E0334C29B8977B82A04C362E7F88
  double T = T_C + TK;
  double T18 = 1.8 * T;
  double X = 3.448 + 986.4 / T18 + 0.01009 * Mw;
  double Y = 2.447 - 0.2224 * X;
  double K = ( 9.379 + 0.01607 * Mw ) * T18 * sqrt ( T18 )/
    ( 209.2 + 19.26 * Mw + T18 );
  // P Mw / RT = M/V
  double Ro = P * Mw / ( RP * T ) * 1e-3;
  double Mu = 1e-4 * K * exp ( X * pow ( Ro, Y )); // mPa s    cp ???
  return Mu * 1e-3;
//
/* 
// Perry's Chemical Engineering  page 508
// Yoon-Thodos method
  double Tr = ( TC + TK ) / Tcr;
  double Eta = ( 46.1 * pow ( Tr, 0.618 ) - 20.4 * exp ( -0.449 * Tr ) + 19.4 * exp ( -4.058 * Tr ) + 1. ) * Mw * pow ( Pcr * Pascal, 2. / 3. ) /
               ( 2.173424e11 * pow ( Tcr, 1. / 6 ));
  return Eta; // Pa * s
*/
  }

double CPseudo::Lam_gas_2 ( double P, double TC )
  {
  //W / m K
  // page 354
  double A = 0.00231 + 0.42624 / Mw + 1.9891 / ( Mw * Mw );
  double B = 1.0208e-4 + 1.3047e-4 / Mw + 0.00574 / ( Mw * Mw );
  double T = TC + TK;
  double k = A + B * ( T - 255.4 );
  return k; // W / m K
  }

/*
double CPseudo::Lam_liq ( double TC )
  {
//  Baroncini method. 
// Perry's Chemical Engineering  page 513
  double A = 0.035;
  double Alfa = 1.2;
  double Beta = 1.;
  double Gamma = 0.167;
  double Tr = ( TC + TK ) / Tcr;
  double Lam = A * pow ( TbK + TK, 1.2 ) / ( Mw * pow ( Tcr, Gamma )) * pow ( 1. - Tr, 0.38 ) * pow ( Tr, -1. / 6. );
  return Lam; // W / m K
  }
*/

bool CPseudo::Mw_Tb( char * Appr, double _Tb )
  {
//
#if 0
  Tb = _Tb;                               
  double TbK = Tb + TK;
  IF ( "Variant 1" )// Lee–Kesler   при 140 - дает < 0
    {
// The API gravity varies from less than 10 for very heavy crudes to between 10 and 30 for heavy crudes, to between 30 and
// 40 for medium crudes, and to above 40 for light crudes.
// Иначе в Petroleum & Petrochemical Engineering Journal Volume 2 Issue 1 2018
/*
    Mw = -12272.6 + 9486.4 * SG + ( 8.3741 - 5.99175 * SG ) * TbK +
      + ( 1 - 0.77084 * SG - 0.02058 * SG * SG ) * ( 0.7465 - 222.466 / TbK ) * 1e7 / TbK +  
        ( 1 - 0.80882 * SG - 0.02226 * SG * SG ) * ( 0.3228 - 17.335  / TbK ) * 1e12 / ( TbK * TbK * TbK );
*/
    Mw = -12272.6 + 9486.4 * SG + ( 4.6523 - 3.3287 * SG ) * TbK +
         1e7 / TbK * ( 1. - 0.77084 * SG - 0.02058 * SG * SG ) * ( 1.3437 - 720.79 / TbK ) +
         1e12 / pow( TbK, 3. ) * ( 1 - 0.80882 * SG - 0.02226 * SG * SG ) * ( 1.8828 - 181.98  / TbK );
    //          pow( TbK, 13. )??
//    return true;
    }
  IF ( "Variant 2" )// Twu    --
    {
    double Tc0 = TbK / ( 0.533272 + 0.343838e-3 * TbK + 2.52617e-7 * TbK * TbK - 1.654881e-10 * TbK * TbK * TbK + 4.60773e24 * pow ( TbK, -13. ));
    double Alfa = 1. - TbK / Tc0;
    double SG = 0.843593 - 0.128624 * Alfa - 3.36159 * Alfa * Alfa * Alfa - 13749.5 * pow ( Alfa, 12. );
    double Mw_e = 2000.;
    double Mw_b = 0.;
    double TbK05 = 0.;
    double Mw_05;
    while ( fabs ( TbK05 - TbK ) > 1e-5 )
      {
      Mw_05 = 0.5 * ( Mw_e + Mw_b );
      double Beta = log ( Mw_05 ); //5.12640                                                                     // 13.7512          19.6197
      TbK05 = exp ( 5.71419 + 2.71579 * Beta - 0.28659 * Beta * Beta - 39.8544 / Beta - 0.122488 /( Beta * Beta )) - 24.7522 * Beta + 35.3155 * Beta * Beta;
      if ( TbK05 > TbK )
        Mw_e = Mw_05;
      else
        Mw_b = Mw_05;
      }
    Mw = Mw_05;
//    return true;
    }
  double TbK = Tb;
  IF ( "Variant 3" )// Riazi-Daubert  > C25
    {
    double Mw_ = 4.5673e-5 * pow ( TbK, 2.1962 ) * pow ( SG, -1.0164 );    // +??
    Prt ( 1, F, Mw_, Mw );
//    return true;
    }
  IF ( "Variant 4" )// Riazi-Daubert  > C25
    {
    //!!!
    double Mw_ = 42.965 * exp( 2.097e-4 * TbK - 7.78712 * SG + 2.08476e-3 * TbK * SG ) * pow ( TbK, 1.26007 ) * pow ( SG, 4.98308 );
    Prt ( 2, F, Mw_, Mw );
//    return true;
    }
//Petroleum & Petrochemical Engineering Journal Volume 2 Issue 1 2018
  IF ( "Variant 5" )// Riazi and Daubert
    {
    const double a = 581.96;
    const double b = -0.97476;
    const double c = 6.51274;
    const double d = 0.000543076;
    const double e = 9.53384;
    const double F = 0.00111056;
    double Mw_ = a * pow ( TbK, b ) * pow ( SG, c ) * exp ( d * TbK + e * SG + F * TbK * SG );
    Prt ( 3, F, Mw_, Mw );
//    return true;
    }
  IF ( "Variant 6" )// Sim and Daubert
    {
    //!!!
    double Mw_ = 1.4350476e-5 * pow ( TbK, 2.3776 ) * pow ( SG, -0.9371 );
    Prt ( 4, F, Mw_, Mw );
//    return true;
    }
  IF ( "Variant 7" )// Silva and Rodriguez
    {
    double x = 1.52869 + 0.06486 * log ( TbK / ( 1078 - TbK ));
    double T = 20. + 273.15;
    double Ro_20 = 0.999 * SG - 1e-3 * ( 2.34 - 1.898 * SG ) * ( T - 288.7 );  // CHARACTERIZATION AND PROPERTIES OF PETROLEUM FRACTIONS p 223
    double Mw_ = 0.01077 * pow ( TbK, x ) / Ro_20;
    Prt ( 5, F, Mw_, Mw );
//    return true;
    }
  IF ( "Variant 8" )// Sayed Gomaa
    {
    const double a = 2238.880249;
    const double b = 0.836856;
    const double c = -0.001215;
    const double d = 0.225397;
    double Mw_ = a / pow ( 1. + exp ( b - c * TbK ), 1. / d );
    Prt ( 6, F, Mw_, Mw );
//    return true;
    }
  //!!!
  double Mw_ = 60. + 0.3 * Tb + 0.001 * Tb * Tb;
  Prt ( 7, F, Mw_, Mw );
#endif
  return true;
  }

struct CApprData
  {
  double a, b, c, d, e, f;
  CApprData ( double _a, double _b, double _c, double _d, double _e, double _f )
    {
    a = _a; b = _b; c = _c; d = _d; e = _e; f = _f;
    }
  double Teta ( double Teta1, double Teta2 )
    {
    return a * exp ( b * Teta1 + c * Teta2 + d * Teta1 * Teta2 ) * pow ( Teta1, e ) * pow ( Teta2, f );
    }
  };

struct CApprData2
  {
  double Teta8, a, b, c;
  CApprData2 ( double _Teta8, double _a, double _b, double _c )
    {
    Teta8 = _Teta8; a = _a; b = _b; c = _c;
    }
  double Mw ( double TbK )
    {
    return pow (( a - log ( Teta8 - TbK )) / b, 1. / c ); 
    }
  double Prop ( double Mw )
    {
    return Teta8 - exp ( a - b * pow ( Mw, c ));
    }
  };

void CPseudo::Tc_Pc ( char * Appr, double _Tb )
  {
// HYSYS    
  Tb = _Tb;
  double TbK = Tb + TK;
  IF ( "HYSYS" )
    {
    Tcr = 189.8 + 450.6 * SG + (0.4244 + 0.1174 * SG ) * TbK +(0.1441 - 1.0069 * SG) * 1e5 / TbK;
    Pcr = 5.689 - 0.0566 / SG - ( 0.43639 + 4.1216 / SG + 0.21343 / ( SG * SG )) * 1e-3 * TbK +
      ( 0.47579 + 1.182 / SG + 0.15302 / ( SG * SG ) ) * 1e-6 * TbK * TbK +
      ( 2.4505 + 9.9099 / ( SG * SG )) * 1e-10 * ( TbK * TbK * TbK );
    double Tbr = TbK / Tcr;
    double Tbr6 = pow ( Tbr, 6. );
    Omega = ( -log ( Pcr / 1.01325 ) - 5.92714 + 6.09648 / Tbr + 1.28862 * log ( Tbr ) - 0.169347 * Tbr6 ) /  // Lee-Kesler Method 
      ( 15.2518 - 15.6875 / Tbr - 13.4721 * log ( Tbr ) + 0.43577 * Tbr6 );
//    return;
    }
  IF ( "API" )
    {
    double Tb_rank = 1.8000 * Tb + 491.67;
    double Tcr_rank = 10.6443 * exp( -5.1747e-4 * Tb_rank - 0.54444 * SG + 3.5995e-4 * Tb_rank * SG ) * pow ( Tb_rank, 0.81067 ) + pow ( SG, 0.53691 );
    Tcr = ( Tcr_rank - 491.67 ) / 1.8000 + TK;
    double K = pow ( Tb_rank, 1. / 3. ) / SG;
    double Pcr_psi = 6.162e6 * exp ( -4.725e-3 * Tb_rank - 4.8014 * SG + 3.1939e-3 * Tb_rank * SG ) * pow ( Tb_rank, -0.4844 ) + pow ( SG, 4.0846 ); 
    // 
    Pcr = Pcr_psi * 6.89476;
//    return;
    }
// Characterization of Heavy Oils and Bitumens Energy & Fuels, Vol. 22, No. 1, 2008 
  IF ( "Nji" )
    {
    double a_Tc = 226.50;
    double b_Tc = 6.78;
    double c_Tc = 1.282e-06;
    double d_Tc = 2668;                 
    double a_Pc = 141.20;
    double b_Pc = 45.66e-02;
    double c_Pc = 16.59e-03;
    double d_Pc = 2.1;
    double a_Tb = 477.63;
    double b_Tb = 88.51;
    double c_Tb = 1007.00;
    double d_Tb = 1214.40;
//
    double Mw_e = 2000.;
    double Mw_b = 0.;
    double TbK05 = 0.;
    double Mw_05;
    while ( fabs ( TbK05 - TbK ) > 1e-5 )
      {
      Mw_05 = 0.5 * ( Mw_e + Mw_b );
      TbK05 = a_Tb * log ( ( Mw_05 + b_Tb )/( Mw_05 + c_Tb ) ) + d_Tb;
      if ( TbK05 > TbK )
        Mw_e = Mw_05;
      else
        Mw_b = Mw_05;
      }
    Mw = Mw_05;
    Tcr = a_Tc * log ( ( Mw + b_Tc )/( Mw + c_Tc ) ) + d_Tc;
    Pcr = a_Pc * Mw / pow ( b_Pc + c_Pc * Mw, d_Pc );
    double a_SG = 0.83;
    double b_SG = 89.9513;
    double c_SG = 139.6612;
    double d_SG = 3.2033;
    double e_SG = 1.0564;
    double S = a_SG + b_SG / Mw - c_SG / pow ( Mw + d_SG, e_SG );
//    return;
    }
// C6...C20
//  Tb,SG
    double CH = 5.;// C * 6 / H
//
    CApprData Tc_Tb_SG ( 9.5232, -9.314e-4, -0.54444, 6.48e-4, 0.81067, 0.53691 );  // These correlations were also adopted by the API 
    Tcr = Tc_Tb_SG.Teta ( TbK, SG );
    CApprData Tc_Tb_CH ( 8.6649, 0, 0, 0, 0.67221, 0.10199 );
    Tcr = Tc_Tb_CH.Teta ( TbK, CH );
//
    CApprData Pc_Tb_SG ( 3.195846e5, -8.505e-3, -4.8014, 5.749e-3, -0.4844, 4.0846 );
    Pcr = Pc_Tb_SG.Teta ( TbK, SG );
    CApprData Pc_Tb_CH ( 9.858968, -3.8443e-3, -0.3454, 0, -0.1801, 3.2223 );
    Pcr = Pc_Tb_CH.Teta ( TbK, CH );
// Для > C20
    CApprData Tc_Tb_SG_C20 ( 35.9413, -6.9e-4, -1.4442, 4.91e-4, 0.7293, 1.2771 );  // page 74
    Tcr = Tc_Tb_SG_C20.Teta ( TbK, SG );
    //
    CApprData Pc_Tb_SG_C20 ( 6.9575, -1.35e-2, 0.3129, 9.174e-3, -0.6791, -0.6807 );
    Pcr = Pc_Tb_SG_C20.Teta ( TbK, SG );
//==========
    CApprData Vc_Tb_SG ( 6.049e-2, -2.6422e-3, -0.26404, 1.971e-3, 0.7506, -1.2028 );
    Vcr = Vc_Tb_SG.Teta ( TbK, SG );
    CApprData Vc_Tb_CH ( 1.409e1, -1.6594e-3, 0.05345, 2.6649e-4, 0.1657, -1.4439 );
    Vcr = Vc_Tb_CH.Teta ( TbK, CH );
//
    CApprData Mw_Tb_SG ( 1032.1, 9.78e-4, -9.53384, 2.0e-3, 0.97476, 6.51274 );
    double Mw_ = Mw_Tb_SG.Teta ( TbK, SG );
    CApprData Mw_Tb_CH ( 1.81456e-3, 0, 0, 0, 1.9273, -0.2727 );
    Mw_ = Mw_Tb_CH.Teta ( TbK, CH );
// Mw до 700                                                         
    CApprData Mw_Tb_SG_700 ( 42.965, 2.097e-4, -7.78712, 2.08476e-3, 1.26007, 4.98308 );
    Mw_ = Mw_Tb_CH.Teta ( TbK, SG );
//
    CApprData SG_Tb_CH ( 2.86706e-3, -1.83321e-3, -0.081635, 6.49168e-5, 0.890041, 0.73238 );
    double SG_ = SG_Tb_CH.Teta ( TbK, CH );
//
    CApprData CH_Tb_SG ( 3.47028, 1.4850e-2, 16.94020, -0.012491, -2.72522, -6.79769 );
    double CH_ = CH_Tb_SG.Teta ( TbK, SG );
// C20...C50
    //
    CApprData Tc_Tb_SG_H ( 35.9413, -6.9e-4, -1.4442, 4.91e-4, 0.7293, 1.2771 );
    Tcr = Tc_Tb_SG.Teta ( TbK, SG );
    //
    CApprData Pc_Tb_SG_H ( 6.9575, -0.0135, -0.3129, 9.174e-3, 0.6791, -0.6807  );
    Pcr = Pc_Tb_SG.Teta ( TbK, SG );
    //
    CApprData Vc_Tb_SG_H ( 6.1677e10, -7.583e-3, -28.5524, 0.01172, 1.20493, 17.2074  );
    Vcr = Vc_Tb_SG.Teta ( TbK, SG );
    //
// page 65
//  n-alkanes 
    CApprData2 Tb_M ( 1070, 6.98291, 0.02013, 2./ 3. );
    Mw_ = Tb_M.Mw( TbK );
    CApprData2 Tb_Tc_M ( 1.15, -0.41966, 0.02436, 0.58 );
    double TbTc = Tb_Tc_M.Prop( Mw_ );
    Tcr = TbK / TbTc; 
    CApprData2 SG_M ( 0.85, 92.22793, 89.82301, 0.01  );
    SG_ = SG_M.Prop( Mw_ );
    CApprData2 Pc_M ( 0, 4.65757, 0.13423, 0.5  );
    Pcr = -Pc_M.Prop( Mw_ ); 
    CApprData2 Omega_M ( 0.3, -3.06826, -1.04987, 0.2 );
    Omega = -Omega_M.Prop( Mw_ ); 
//   n-alkylcyclopentanes 
    CApprData2 Tb_M2 ( 1028, 6.95649, 0.02239, 2./ 3. );
    Mw_ = Tb_M2.Mw( TbK );
    CApprData2 Tb_Tc_M2 ( 1.2, 0.06765, 0.13763, 0.35  );
    double TbTc2 = Tb_Tc_M2.Prop( Mw_ );
    Tcr = TbK / TbTc2; 
    CApprData2 SG_M2 ( 0.853, 97.72532, 95.73589, 0.01  );
    SG_ = SG_M2.Prop( Mw_ );
    CApprData2 Pc_M2 ( 0, 7.25857, 1.13139, 0.26  );
    Pcr = -Pc_M2.Prop( Mw_ ); 
    CApprData2 Omega_M2 ( 0.3, -8.25682, -5.33934, 0.08 );
    Omega = -Omega_M2.Prop( Mw_ ); 
//    n-alkylcyclohexane  
    CApprData2 Tb_M3 ( 1100, 7.00275, 0.01977, 2./ 3. );
    Mw_ = Tb_M3.Mw( TbK );
    CApprData2 Tb_Tc_M3 ( 1.032, -0.11095, 0.1363, 0.4  );
    double TbTc3 = Tb_Tc_M3.Prop( Mw_ );
    Tcr = TbK / TbTc3; 
    CApprData2 SG_M3 ( 1.032, -0.11095, 0.1363, 0.4 );
    SG_ = SG_M3.Prop( Mw_ );
    CApprData2 Pc_M3 ( 0, 12.3107, 5.53366, 0.1  );
    Pcr = -Pc_M3.Prop( Mw_ ); 
    CApprData2 Omega_M3 ( 0.6, -5.00861, -3.04868, 0.1  );
    Omega = -Omega_M3.Prop( Mw_ ); 
//  n-alkylbenzenes 
    CApprData2 Tb_M4 ( 1015, 6.91062, 0.02247, 2./ 3. );
    Mw_ = Tb_M4.Mw( TbK );
    CApprData2 Tb_Tc_M4 ( 1.03, -0.29875, 0.06814, 0.5 );
    double TbTc4 = Tb_Tc_M4.Prop( Mw_ );
    Tcr = TbK / TbTc4; 
    CApprData2 SG_M4 ( -0.8562, 224.7257, 218.518, 0.01  );
    SG_ = SG_M4.Prop( Mw_ );
    CApprData2 Pc_M4 ( 0, 9.77968, 3.07555, 0.15 );
    Pcr = -Pc_M4.Prop( Mw_ ); 
    CApprData2 Omega_M4 ( 0., -14.97, -9.48345, 0.08 );
    Omega = -Omega_M4.Prop( Mw_ ); 
// Вязкость
//    return;


/*
  IF ( "Qaiser" )//Characterization of petroleum fractions Dr. Qaiser Al-Assady 
    {
    double SG = 141.5/ ( API + 131.5 ); // Ro_oil / Ro_water at 15.6C
    double k_Mw = 13.03251;
    double a_Mw = 0.81348;
    double b_Mw = 1.332426;
    double k_Tc = 60.7002;
    double a_Tc = 0.2613213;
    double b_Tc = 2.4851934; 
    double k_Pc = 1164.2722;
    double a_Pc = -0.87345;
    double b_Pc = 0.26911;
    double k_Vc = 0.0881418;
    double a_Vc = 0.115785;
    double b_Vc = -0.67066;
    double k_Vm = 44.26796;
    double a_Vm = 0.799526;
    double b_Vm = 0.106324; 
    double k_Lam = 2755.825;
    double a_Lam = 0.412988;
    double b_Lam = 1.4198218;
    double k_Ro = 0.2751615;
    double a_Ro = -0.012786;
    double b_Ro = 1.347846; 
    Mw = k_Mw * exp ( a_Mw * TbK + b_Mw * SG );
    return;
    }
*/
  }



void CreateAllPseudo_Base( int & nGas )
  {
  nFirstPseudo = nGas;
  int nC;
  int n = 0;
  void * F = Fopen ( "Data.csv", "wt" );
  while ( 1 )
    {
    nC = NcData[n].nC;
    if ( nC < 0 )
      break;
    Fprintf ( F, "%d;", nC );
    CPseudo * pPseudo = new CPseudo();
    pPseudo->Numb = nGas;
    pPseudo->nC = nC;
    SET(Mw)
    SET(Tb) 
    SET(SG) 
//    SET(n20) 
//    SET(d20) 
    SET(Tcr) 
    SET(Pcr) 
    SET(Vcr) 
    SET(Zcr) 
    SET(Omega) 
//    SET(Sigma) 
//    SET(Delta)
    ASS(nGas < K_GAS)
    pGas[nGas++] = pPseudo;
    n++;
    Fprintf ( F, "\n" );
    }
  Fclose ( F );
  }




// Dr. Ramgopal Uppaluri 
// Refinery Process Design

struct CEOS  // page 220
  {
  double u1, u2, ac, Alfa, b;
  double A, B, B2, B3, Pcr, Tcr, RT;
  double a1, a2, a3;
  bool Test ( double Z )
    {
    double T = (( Z + a1 * Z ) * Z + a2 ) * Z + a3;
    if ( fabs ( Z ) < 1e-8 )
      return true;
    return false;
    }
  void SetAB ( double _Pcr, double _Tcr )
    {
    Pcr = _Pcr;
    Tcr = _Tcr;
    RT = RG * Tcr;
    B = b * Pcr / RT;
    B2 = B * B;
    B3 = B2 * B;
    }
  virtual void CalcAlfa( double TC ) = 0;
  int Calc ( double TC, double Z[] )  // page 220
    {
    double a = ac * Alfa;
    A = a * Pcr /( RT * RT );
    double a1 = -( 1. + B - u1 * B );
    double a2 = A + u2 * B2 - u1 * ( B + B2 );
    double a3 = -A * B - u2 * ( B2 + B3 );
 //
    double Q = ( 3 * a2 - a1 * a1 ) / 9.;
    double L = ( 9. * a1 * a2 - 27. * a3 - 2. * a1 * a1 * a1 ) / 54.; 
    double D = Q * Q * Q + L * L;
    if ( D > 0. )
      {
      double SqD = sqrt ( D );
      double X = L + SqD;
      double S1;
      if ( X > 0. )
        S1 = pow ( X, 1. / 3. );
      else
        S1 = -pow ( -X, 1. / 3. );
      X = L - SqD;
      double S2;
      if ( X > 0. )
        S2 = pow ( X, 1. / 3. );
      else
        S2 = -pow ( -X, 1. / 3. );
      Z[0] = S1 + S2 - a1 / 3;
      ASS( Test ( Z[0] ));
      return 1;
      }
    ASS ( D < 0. )  // Опускаем D == 0
    double Teta3 = acos ( L / sqrt ( -Q * Q * Q )) / 3.;
    ASS(_finite ( Teta3 ));
    
    double M = 2. * sqrt ( -Q );
    double Z1 =  M * cos ( Teta3 + M_PI * 2. / 3. ) - a1 / 3.;
    double Z2 =  M * cos ( Teta3 + M_PI * 4. / 3. ) - a1 / 3.;
    double Z3 =  M * cos ( Teta3 ) - a1 / 3.;
    ASS( Test ( Z1 ))
    ASS( Test ( Z1 ))
    ASS( Test ( Z1 ))
    Z[0] = Z1;
    if ( Z2 < Z[0] )
      Z[0] = Z2;
    if ( Z3 < Z[0] )
      Z[0] = Z3;
    Z[1] = Z1;
    if ( Z2 < Z[1] )
      Z[1] = Z2;
    if ( Z3 < Z[1] )
      Z[1] = Z3;
    return 2;
    }
  };

void TestAllPseudo(  )
  {
  //void * F = Fopen ( "Test.csv", "wt" );
  //for ( int n = nFirstPseudo; n < LAST_GAS; n++ )
  //  {
  //  CPseudo * pC = (CPseudo*)pGas[n];
  //  pC->Test( F );
  //  }
  //Fclose ( F );
  KKK();
  }

#define PRT(Var) Prt ( __LINE__, F, Var##_, Var );
void CPseudo::Test( void * F )
  {
  //
  Fprintf ( F, "%d;", nC );
  //
  double TbK = Tb;
  double Tcr_, Pcr_, Omega_, SG_, Mw_; 
  //IF ( "Variant 3" )// Riazi-Daubert  > C25
  //  {
  //  Mw_ = 4.5673e-5 * pow ( TbK, 2.1962 ) * pow ( SG, -1.0164 );    // +??
  //  PRT( Mw );
  //  //    return true;
  //  }
  // Лучшие аппроксимации
  //HYSYS
  Tcr_ = 189.8 + 450.6 * SG + (0.4244 + 0.1174 * SG ) * TbK +(0.1441 - 1.0069 * SG) * 1e5 / TbK;
  PRT( Tcr );
  Mw_ = 42.965 * exp( 2.097e-4 * TbK - 7.78712 * SG + 2.08476e-3 * TbK * SG ) * pow ( TbK, 1.26007 ) * pow ( SG, 4.98308 );
  PRT( Mw );
  CApprData2 Tb_M ( 1070, 6.98291, 0.02013, 2./ 3. );
  Mw_ = Tb_M.Mw( TbK );
  PRT( Mw );
  CApprData Pc_Tb_SG ( 3.195846e5, -8.505e-3, -4.8014, 5.749e-3, -0.4844, 4.0846 );
  Pcr_ = Pc_Tb_SG.Teta ( TbK, SG );
  PRT( Pcr );
  double Tbr = TbK / Tcr_;
  double Tbr6 = pow ( Tbr, 6. );
  Omega_ = ( -log ( Pcr / 1.01325 ) - 5.92714 + 6.09648 / Tbr + 1.28862 * log ( Tbr ) - 0.169347 * Tbr6 ) /  // Lee-Kesler Method 
    ( 15.2518 - 15.6875 / Tbr - 13.4721 * log ( Tbr ) + 0.43577 * Tbr6 );
  PRT( Omega );
  CApprData2 SG_M3 ( 1.032, -0.11095, 0.1363, 0.4 );
  SG_ = SG_M3.Prop( Mw_ );
  PRT( SG );
  double Vcr = RP * Tcr * Zcr * 1e+3 / Pcr;
  double Vcr_;
  CApprData Vc_Tb_SG_H ( 6.1677e10, -7.583e-3, -28.5524, 0.01172, 1.20493, 17.2074  );
  Vcr_ = Vc_Tb_SG_H.Teta ( TbK, SG );  // cм3/моль
  PRT( Vcr );
  Fprintf ( F, "\n;", nC );
//
  //Mw_ = -12272.6 + 9486.4 * SG + ( 8.3741 - 5.99175 * SG ) * TbK +
  //  + ( 1 - 0.77084 * SG - 0.02058 * SG * SG ) * ( 0.7465 - 222.466 / TbK ) * 1e7 / TbK +  
  //  ( 1 - 0.80882 * SG - 0.02226 * SG * SG ) * ( 0.3228 - 17.335  / TbK ) * 1e12 / ( TbK * TbK * TbK );
  //PRT( Mw );
 //Petroleum & Petrochemical Engineering Journal Volume 2 Issue 1 2018
  //IF ( "Variant 5" )// Riazi and Daubert
  //  {
  //  const double a = 581.96;
  //  const double b = -0.97476;
  //  const double c = 6.51274;
  //  const double d = 0.000543076;
  //  const double e = 9.53384;
  //  const double f = 0.00111056;
  //  Mw_ = a * pow ( TbK, b ) * pow ( SG, c ) * exp ( d * TbK + e * SG + f * TbK * SG );
  //  PRT( Mw );
  //  //    return true;
  //  }
  //!!!
  //Mw_ = 60. + 0.3 * ( Tb - TK ) + 0.001 * ( Tb - TK ) * ( Tb - TK ); 
  //
  //PRT( Mw );
  //Fprintf ( F, "\n;" );
  /////////
//  IF ( "HYSYS" )
//    {
//
  double CH = 5.;
  CApprData CH_Tb_SG ( 3.47028, 1.4850e-2, 16.94020, -0.012491, -2.72522, -6.79769 );
  double CH_ = CH_Tb_SG.Teta ( TbK, SG );
//////////////////////////////////////////////////////////////////////////////////
  //CApprData Mw_Tb_SG ( 1032.1, 9.78e-4, -9.53384, 2.0e-3, 0.97476, 6.51274 );
  //Mw_ = Mw_Tb_SG.Teta ( TbK, SG );
  //PRT( Mw );
  //CApprData Mw_Tb_CH ( 1.81456e-3, 0, 0, 0, 1.9273, -0.2727 );
  //Mw_ = Mw_Tb_CH.Teta ( TbK, CH );
  //PRT( Mw );
  //CApprData2 Tb_M2 ( 1028, 6.95649, 0.02239, 2./ 3. );
  //Mw_ = Tb_M2.Mw( TbK );
  //PRT( Mw );
  //CApprData2 Tb_M3 ( 1100, 7.00275, 0.01977, 2./ 3. );
  //Mw_ = Tb_M3.Mw( TbK );
  //PRT( Mw );
  //CApprData2 Tb_M4 ( 1015, 6.91062, 0.02247, 2./ 3. );
  //Mw_ = Tb_M4.Mw( TbK );
  //PRT( Mw );
  //Fprintf ( F, "\n;" );
///////////////////////////////////////////////////////////////////////////////////
  //Pcr_ = 5.689 - 0.0566 / SG - ( 0.43639 + 4.1216 / SG + 0.21343 / ( SG * SG )) * 1e-3 * TbK +
  //  ( 0.47579 + 1.182 / SG + 0.15302 / ( SG * SG ) ) * 1e-6 * TbK * TbK -
  //  ( 2.4505 + 9.9099 / ( SG * SG )) * 1e-10 * ( TbK * TbK * TbK );
  //Pcr_ *= 9.81;
  //PRT( Pcr );
  //CApprData Pc_Tb_CH ( 9.858968, -3.8443e-3, -0.3454, 0, -0.1801, 3.2223 );
  //Pcr_ = Pc_Tb_CH.Teta ( TbK, CH );
  //PRT( Pcr );
  //CApprData2 Pc_M ( 0, 4.65757, 0.13423, 0.5  );
  //Pcr = -Pc_M.Prop( Mw_ ); 
  //PRT( Pcr );
  //CApprData2 Pc_M2 ( 0, 7.25857, 1.13139, 0.26  );
  //Pcr = -Pc_M2.Prop( Mw_ ); 
  //PRT( Pcr );
  //CApprData Pc_Tb_SG_H ( 6.9575, -0.0135, -0.3129, 9.174e-3, 0.6791, -0.6807  );
  //Pcr_ = Pc_Tb_SG.Teta ( TbK, SG );
  //PRT( Pcr );
  //CApprData2 Pc_M4 ( 0, 9.77968, 3.07555, 0.15 );
  //Pcr_ = -Pc_M4.Prop( Mw_ ); 
  //PRT( Pcr );
  //CApprData2 Pc_M3 ( 0, 12.3107, 5.53366, 0.1  );
  //Pcr_ = -Pc_M3.Prop( Mw_ ); 
  //PRT( Pcr );
  //Fprintf ( F, "\n;" );
///////////////////////////////////////////////////////////////////////////////////
  //CApprData2 Omega_M ( 0.3, -3.06826, -1.04987, 0.2 );
  //Omega_ = -Omega_M.Prop( Mw_ ); 
  //PRT( Omega );
  //CApprData2 Omega_M2 ( 0.3, -8.25682, -5.33934, 0.08 );
  //Omega_ = -Omega_M2.Prop( Mw_ ); 
  //PRT( Omega );
  //CApprData2 Omega_M3 ( 0.6, -5.00861, -3.04868, 0.1  );
  //Omega_ = -Omega_M3.Prop( Mw_ ); 
  //PRT( Omega );
  //CApprData2 Omega_M4 ( 0., -14.97, -9.48345, 0.08 );
  //Omega = -Omega_M4.Prop( Mw_ );
///////////////////////////////////////////////////////////////////////////////////
  //CApprData SG_Tb_CH ( 2.86706e-3, -1.83321e-3, -0.081635, 6.49168e-5, 0.890041, 0.73238 );
  //SG_ = SG_Tb_CH.Teta ( TbK, CH );
  //PRT( SG );
  //CApprData2 SG_M ( 0.85, 92.22793, 89.82301, 0.01  );
  //SG_ = SG_M.Prop( Mw_ );
  //PRT( SG );
  //CApprData2 SG_M2 ( 0.853, 97.72532, 95.73589, 0.01  );
  //SG_ = SG_M2.Prop( Mw_ );
  //PRT( SG );
  //double a_SG = 0.83;
  //double b_SG = 89.9513;
  //double c_SG = 139.6612;
  //double d_SG = 3.2033;
  //double e_SG = 1.0564;
  //SG_ = a_SG + b_SG / Mw - c_SG / pow ( Mw + d_SG, e_SG );
  //PRT( SG );
  //Fprintf ( F, "\n;" );
///////////////////////////////////////////////////////
  CApprData Vc_Tb_SG ( 6.049e-2, -2.6422e-3, -0.26404, 1.971e-3, 0.7506, -1.2028 );
  Vcr_ = Vc_Tb_SG.Teta ( TbK, SG ) * Mw;  // cм3/г
  PRT( Vcr );
  CApprData Vc_Tb_CH ( 1.409e1, -1.6594e-3, 0.05345, 2.6649e-4, 0.1657, -1.4439 );
  Vcr_ = Vc_Tb_CH.Teta ( TbK, CH ) * Mw;
  PRT( Vcr );
  ///////////////////////////////////////////////////////////////////////////////////
  //
  //
  // page 65
  //  n-alkanes 
  //    n-alkylcyclohexane  
  //CApprData2 Tb_Tc_M3 ( 1.032, -0.11095, 0.1363, 0.4  );
  //double TbTc3 = Tb_Tc_M3.Prop( Mw_ );
  //Tcr_ = TbK / TbTc3; 
  //PRT( Tcr );
  //  n-alkylbenzenes 
  //CApprData2 Tb_Tc_M4 ( 1.03, -0.29875, 0.06814, 0.5 );
  //double TbTc4 = Tb_Tc_M4.Prop( Mw_ );
  //Tcr_ = TbK / TbTc4; 
  //PRT( Tcr );
  //CApprData2 SG_M4 ( -0.8562, 224.7257, 218.518, 0.01  );
  //SG_ = SG_M4.Prop( Mw_ );
  //PRT( SG );
  //
  ///////////////////////////////////////////////////////////////////////////////////
  
   //
    //    return;
//    }
  //IF ( "API" )
  //  {
  //  double Tb_rank = 1.8000 * Tb + 491.67;
  //  double Tcr_rank = 10.6443 * exp( -5.1747e-4 * Tb_rank - 0.54444 * SG + 3.5995e-4 * Tb_rank * SG ) * pow ( Tb_rank, 0.81067 ) + pow ( SG, 0.53691 );
  //  Tcr_ = ( Tcr_rank - 491.67 ) / 1.8000 + TK;
  //  PRT( Tcr );
  //  double K = pow ( Tb_rank, 1. / 3. ) / SG;
  //  double Pcr_psi = 6.162e6 * exp ( -4.725e-3 * Tb_rank - 4.8014 * SG + 3.1939e-3 * Tb_rank * SG ) * pow ( Tb_rank, -0.4844 ) + pow ( SG, 4.0846 ); 
  //  // 
  //  Pcr_ = Pcr_psi * 6.89476;
  //  PRT( Pcr );
  //  //    return;
  //  }
  // Characterization of Heavy Oils and Bitumens Energy & Fuels, Vol. 22, No. 1, 2008 
  IF ( "Nji" )
    {
    double a_Tc = 226.50;
    double b_Tc = 6.78;
    double c_Tc = 1.282e-06;
    double d_Tc = 2668;                 
    double a_Pc = 141.20;
    double b_Pc = 45.66e-02;
    double c_Pc = 16.59e-03;
    double d_Pc = 2.1;
    double a_Tb = 477.63;
    double b_Tb = 88.51;
    double c_Tb = 1007.00;
    double d_Tb = 1214.40;
    //
    double Mw_e = 2000.;
    double Mw_b = 0.;
    double TbK05 = 0.;
    double Mw_05;
    while ( fabs ( TbK05 - TbK ) > 1e-5 )
      {
      Mw_05 = 0.5 * ( Mw_e + Mw_b );
      TbK05 = a_Tb * log ( ( Mw_05 + b_Tb )/( Mw_05 + c_Tb ) ) + d_Tb;
      if ( TbK05 > TbK )
        Mw_e = Mw_05;
      else
        Mw_b = Mw_05;
      }
    Mw = Mw_05;
    //Tcr_ = a_Tc * log ( ( Mw + b_Tc )/( Mw + c_Tc ) ) + d_Tc;
    //PRT( Tcr );
    //Pcr_ = a_Pc * Mw / pow ( b_Pc + c_Pc * Mw, d_Pc );
    //PRT( Pcr );
    //    return;
    }
    // C6...C20
    //  Tb,SG
    //
    //CApprData Tc_Tb_SG ( 9.5232, -9.314e-4, -0.54444, 6.48e-4, 0.81067, 0.53691 );  // These correlations were also adopted by the API 
    //Tcr_ = Tc_Tb_SG.Teta ( TbK, SG );
    //PRT( Tcr );
    //CApprData Tc_Tb_CH ( 8.6649, 0, 0, 0, 0.67221, 0.10199 );
    //Tcr_ = Tc_Tb_CH.Teta ( TbK, CH );
    //PRT( Tcr );
    //
    // Для > C20
    //CApprData Tc_Tb_SG_C20 ( 35.9413, -6.9e-4, -1.4442, 4.91e-4, 0.7293, 1.2771 );  // page 74
    //Tcr_ = Tc_Tb_SG_C20.Teta ( TbK, SG );
    //PRT( Tcr );
    //
    //CApprData Pc_Tb_SG_C20 ( 6.9575, -1.35e-2, 0.3129, 9.174e-3, -0.6791, -0.6807 );
    //Pcr_ = Pc_Tb_SG_C20.Teta ( TbK, SG );
    //PRT( Pcr );
    //==========
    // Mw до 700                                                         
    //CApprData Mw_Tb_SG_700 ( 42.965, 2.097e-4, -7.78712, 2.08476e-3, 1.26007, 4.98308 );
    //Mw_ = Mw_Tb_CH.Teta ( TbK, SG );
    //PRT( Mw );
    //
    // C20...C50
    //
    //CApprData Tc_Tb_SG_H ( 35.9413, -6.9e-4, -1.4442, 4.91e-4, 0.7293, 1.2771 );
    //Tcr_ = Tc_Tb_SG.Teta ( TbK, SG );
    //PRT( Tcr );
    //
    Fprintf ( F, "\n" );
  }
//http://thermalinfo.ru/svojstva-gazov/gazy-raznye/dinamicheskaya-vyazkost-gazov-i-parov
void TestCompVis( CGas * pC, void * F, int ID )
  {
  Fprintf ( F, "%s;", pC->Name );
//
  double P = 1.;
  double TC = 20;
  if ( ID >= 13 )
    KKK();
  if ( ID < nFirstPseudo )
    {
    CReal * pR = (CReal*)pC;
    double Vis3 = pR->Vis_gas( 1., 20. );
    double VisL = pC->Vis_liq( 20. );
    Fprintf ( F, "%5.3le;;%5.3le;\n", Vis3, VisL );
    return;
    }
  CPseudo * pP = (CPseudo*)pC;
  double Vis = pP->Vis_gas( 1., 20. );
  double VisL = pP->Vis_liq( 20. );
  Fprintf ( F, "%5.3le;;%5.3le;\n", Vis, VisL );
  }

void TestCompLam( CGas * pC, void * F, int ID )
  {
  Fprintf ( F, "%s;", pC->Name );
  //
  double P = 1.;
  double TC = 20;
  if ( ID >= nFirstPseudo )
    {
    CPseudo * pP = (CPseudo*)pC;
    double LamG = pP->Lam_gas_2( 1., 20. );
    double LamL = pP->Lam_liq( 20. );
//    double LamL_2 = pP->Lam_liq_2( 20. );
    Fprintf ( F, "%5.3le;;%5.3le", LamG, LamL );
    }
  else
    {
    double LamG = pC->Lam_gas( P, 20. );
    double LamL = pC->Lam_liq ( 20. );
    Fprintf ( F, "%5.3le;;%5.3le;", LamG, LamL );
    }
  Fprintf ( F, "\n" );
  }

void TestVis()
  {
  void * F = Fopen ( "TestVis.csv", "wt" );
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    CGas * pC = (CGas*)pGas[n];
    TestCompVis( pC, F, n );
    }
  Fprintf ( F, "##########################################################\n" );
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    CGas * pC = (CGas*)pGas[n];
    TestCompLam( pC, F, n );
    }
  Fclose ( F );
  KKK();
  }
#endif
