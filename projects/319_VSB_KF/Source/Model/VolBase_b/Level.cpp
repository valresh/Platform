#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 

#define K_CYL 50
double V_cyl[K_CYL+1];
double H_cyl[K_CYL+1];
double N_cyl = (double)K_CYL;
double V_sphera[K_CYL+1];
bool SetVH = true;

double CVolBase::CalcLev ( double Vol_liq )
  {
  if ( Vol_liq <= 0. )
    return 0.;
  if ( Vol_liq >= 1. )
    return 1.;
  double H;
  switch ( Тип )
    {
    case 0:
      {
      // цилиндр
      int N = K_CYL - 1;
      while ( N > 0 )
        {
        if ( Vol_liq > V_cyl[N] )
          break;
        N--;
        }
      H = H_cyl[N] + ( H_cyl[N+1] - H_cyl[N] ) * ( Vol_liq - V_cyl[N] ) / ( V_cyl[N+1] - V_cyl[N] );
      }
      break;
    case 2:
      {
      // сфера
      int N = K_CYL - 1;
      while ( N > 0 )
        {
        if ( Vol_liq > V_sphera[N] )
          break;
        N--;
        }
      H = H_cyl[N] + ( H_cyl[N+1] - H_cyl[N] ) * ( Vol_liq - V_sphera[N] ) / ( V_sphera[N+1] - V_sphera[N] );
      }
      break;
    default:
      H = Vol_liq;
      break;
    }
  return H;
  }

double CVolBase::CalcVol ( double Lev_liq )
  {
  if ( Lev_liq <= 0. )
    return 0.;
  if ( Lev_liq >= 1. )
    return Liq_Vol;
  int N = (int)( Lev_liq * N_cyl );
  double V = Lev_liq;
  if ( Тип == 0 )
    V = V_cyl[N] + ( V_cyl[N+1] - V_cyl[N] ) * ( Lev_liq - H_cyl[N] ) / ( H_cyl[N+1] - H_cyl[N] );
  if ( Тип == 2 )
    V = V_sphera[N] + ( V_sphera[N+1] - V_sphera[N] ) * ( Lev_liq - H_cyl[N] ) / ( H_cyl[N+1] - H_cyl[N] );
  return V * Liq_Vol;
  }

void CVolBase::SetVH(  )
  {
  double dH = 1. / (double)K_CYL;
  V_cyl[0] = 0.;
  H_cyl[0] = 0.;
  V_sphera[0] = 0.;
  V_cyl[K_CYL] = 1.;
  H_cyl[K_CYL] = 1.;
  V_sphera[K_CYL] = 1.;
  for ( int n = 1; n < K_CYL; n++ )
    {
    double H = dH * n;
    double Fi = 2. * acos ( 1. - 2. * H );
    double V = ( Fi - sin ( Fi )) * 0.15915494309189533576888376337251;
    V_cyl[n] = V;
    H_cyl[n] = H;
    // Сфера
    //   pi * h * h * ( R - h / 3 )
    V = M_PI * H * H * ( 1. - H / 3. );
    V_sphera[n] = V; 
    }
  }

//////////////////////////////////////////////////////////////////////////////

void CVolBase::Static_Set_Lev( double Lev )
  {
  if ( Объема_2 && Calc_2F && Vol2F >= 0. )
    {
    //    Level_2Vol( dt );
    if ( Calc_2F )
      {
      if ( kStep % 10 == 0 )
        {
        M3_liq_1	= To_m3 ( T, IsComp, Cmol_1F	);
        M3_liq_2	= To_m3 ( T, IsComp, Cmol_2F	);
        Mw_1F = ::Mw ( IsComp, Cmol_1F	);
        Mw_2F = ::Mw ( IsComp, Cmol_2F	);
        }
      M_Liq_1 = M_Liq * Eps_1F;
      M_Liq_2 = M_Liq - M_Liq_1;
      M_Liq_1_kg = M_Liq_1 * Mw_1F;
      M_Liq_2_kg = M_Liq_2 * Mw_2F;
      V_Liq_1 = M_Liq_1 * M3_liq_1;
      V_Liq_2 = M_Liq_2 * M3_liq_2;
      if ( Is_2F )
        {       
        Lev_liq_2 = Lev;
        Lev_liq_1in2 = 1. - Lev;
        }
      else
        {
        Lev_liq_2 = 0.;
        }
      Lev_liq_2in1 = 0.;
      Lev_liq_1 = Lev_liq  = Lev;
      M_Liq_2 = Lev_liq_2 * Vol2F / M3_liq_2;
      M_Liq_1 = ( Lev_liq_1in2 * Vol2F + CalcVol ( Lev_liq_1 ) * Vol ) / M3_liq_1;
      M_Liq = M_Liq_1 + M_Liq_2;
      }
    else
      {
      if ( kStep % 10 == 0 )
        {
        M3_liq	= To_m3 ( T, IsComp, Cmol_Liq	);
        Mw_liq	= ::Mw ( IsComp, Cmol_Liq	);
        M3_liq_2 = M3_liq_1	= M3_liq;
        }
      Lev_liq_1 = Lev_liq = Lev;
      M_Liq = ( Vol2F + CalcVol ( Lev_liq_1 ) * Vol ) / M3_liq_1; 
	    double M_kg = M_Liq * Mw_liq;
	    _M_liq_kg = M_kg;
      M_Liq_1 = M_Liq;
      M_Liq_2 = 0.;
      Lev_liq_2 = 0.;
	    _Lev_liq	= Lev_liq * 100.;
      }
    }
  else
    {
    //    Level_1Vol( dt );
    if ( Calc_2F )
      {
      if ( kStep % 10 == 0 )
        {
        M3_liq_1	= To_m3 ( T, IsComp, Cmol_1F	);
        M3_liq_2	= To_m3 ( T, IsComp, Cmol_2F	);
        Mw_1F = ::Mw ( IsComp, Cmol_1F	);
        Mw_2F = ::Mw ( IsComp, Cmol_2F	);
        }
      if ( Is_2F )
        {             
        Lev_liq  = Lev;
        Lev_liq_2 = Lev * 0.1;
        Lev_liq_1 = Lev_liq - Lev_liq_2;
        }
      else
        {
        Lev_liq  = Lev;
        Lev_liq_2 = 0.;
        Lev_liq_1 = Lev_liq;
        }
      M_Liq_1 = CalcVol ( Lev_liq_1 ) * Vol / M3_liq_1;
      M_Liq_2 = CalcVol ( Lev_liq_2 ) * Vol / M3_liq_1;
      M_Liq = M_Liq_1 + M_Liq_2;
      }
    else
      {
      if ( kStep % 10 == 0 )
        {
        M3_liq	= To_m3 ( T, IsComp, Cmol_Liq	);
        }
      M3_liq_2	= M3_liq_1	= M3_liq;
      Lev_liq_1 = Lev_liq = Lev;
      M_Liq = CalcVol ( Lev_liq ) * Vol / M3_liq;
      double M_kg = M_Liq * Mw_liq;
      _M_liq_kg = M_kg;
      M_Liq_1 = M_Liq;
      M_Liq_2 = 0.;
      _Lev_liq	= Lev_liq * 100.;
      }
    }
  }

void CVolBase::Level_1Vol( double dt )
  {
  if ( Calc_2F )
    {
    if ( kStep % 10 == 0 )
      {
      M3_liq_1	= To_m3 ( T, IsComp, Cmol_1F	);
      M3_liq_2	= To_m3 ( T, IsComp, Cmol_2F	);
      }
    M_Liq_1 = M_Liq * Eps_1F;
    M_Liq_2 = M_Liq - M_Liq_1;
    Mw_1F = ::Mw ( IsComp, Cmol_1F	);
    Mw_2F = ::Mw ( IsComp, Cmol_2F	);
    M_Liq_1_kg = M_Liq_1 * Mw_1F;
    M_Liq_2_kg = M_Liq_2 * Mw_2F;
    V_Liq_1 = M_Liq_1 * M3_liq_1;
    V_Liq_2 = M_Liq_2 * M3_liq_2;
    if ( Is_2F )
      {             
      Lev_liq  = CalcLev ( ( V_Liq_1 + V_Liq_2 ) / Vol );
      Lev_liq_2 = CalcLev ( V_Liq_2 / Vol );
      Lev_liq_1 = Lev_liq - Lev_liq_2;
      }
    else
      {
      Lev_liq  = CalcLev ( V_Liq_1 / Vol );
      Lev_liq_2 = 0.;
      Lev_liq_1 = Lev_liq;
      }
    }
  else
    {
    if ( kStep % 10 == 0 )
      M3_liq	= To_m3 ( T, IsComp, Cmol_Liq	);
    M3_liq_2	= M3_liq_1	= M3_liq;
    double M_kg = M_Liq * Mw_liq;
    _M_liq_kg = M_kg;
    M_Liq_1 = M_Liq;
    M_Liq_2 = 0.;
    k_M_Lev = M3_liq / Vol;
    V_Liq_1 = M_Liq * M3_liq;
    double K = V_Liq_1 / Vol;
    if ( K > 1.05 )
      {
      double M = M_Liq_1 * 1.05 / K; 
      New_M_liq ( 1, M );
      }
    if ( OverFlow && K < 1. && K > 0.01 )
      {
      double M = M_Liq_1 * 1.05 / K; 
      New_M_liq ( 1, M );
      }
    V_Liq_2 = 0.;
    Lev_liq_1 = Lev_liq = CalcLev ( M_Liq * k_M_Lev );

    Lev_liq_2 = 0.;
    _Lev_liq	= Lev_liq * 100.;
    }
  //
  if ( VolZ > 0. )
    {
    double K = VolZ / Vol;
    M_Gas *= K;
    M = M_Liq + M_Gas;
    H = M_Liq * h_liq + M_Gas * h_gas;
    Vol = VolZ;
    VolZ = 0.;
    LevZabs = Lev_liq;
    }
  if ( EmptyVol )
    {
    EmptyVol = false; 
    LevZabs = 0.01;
    }
  if ( FullVol && Lev_liq > 0.001 )
    {
    FullVol = false;
    LevZabs = 1.;
    }
  //
  if ( Lev_liq < LevMin )
    {
    LevZabs = LevMin * 0.01;
    ЕстьЗадания = true;
    }
  //
  if ( Lev_liq > LevMax )
    {
    LevZabs = LevMax * 0.01;
    ЕстьЗадания = true;
    }
  //
//  if ( strstr ( ObjName, "Е-101") )
//    KKK();
  if ( OverFlow )
    {
    if ( V_Liq_1 > V_Liq_2 )
      {
      double Delta = Vol - V_Liq_1 - V_Liq_2;
      New_M_liq ( 1, ( V_Liq_1 + Delta ) / M3_liq_1 );
      }
    else
      {
      double Delta = Vol - V_Liq_1 - V_Liq_2;
      New_M_liq ( 2, ( V_Liq_2 + Delta ) / M3_liq_2 );
      }
    }
  //
  if ( LevZabs > 0. && Lev_liq > 0.01 )
    {
    double Eps1 = Lev_liq_1 / Lev_liq;
    double Eps2 = 1. - Eps1;
    ЕстьЗадания = true;
    Lev_liq = LevZabs;
    if ( Is_2F )
      {
      New_M_liq ( 1, CalcVol( Eps1 * Lev_liq ) * Vol / M3_liq_1 );
      New_M_liq ( 2, CalcVol( Eps2 * Lev_liq ) * Vol / M3_liq_2 );
      }
    else
      New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq );
    LevZabs = 0.;
    }
  if ( LevZ > 0. && Lev_liq > 0.01 )
    {
    double Eps1 = Lev_liq_1 / Lev_liq;
    double Eps2 = 1. - Eps1;
    ЕстьЗадания = true;
    Lev_liq = Lev_0 + ( Lev_100 - Lev_0 ) * LevZ * 0.01;
    if ( Is_2F )
      {
      New_M_liq ( 1, CalcVol( Eps1 * Lev_liq ) * Vol / M3_liq_1 );
      New_M_liq ( 2, CalcVol( Eps2 * Lev_liq ) * Vol / M3_liq_2 );
      }
    else
      New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq );
    LevZ = 0.;
    }
  if ( LevZ < 0. && Lev_liq > 0.01)
    {
    ЕстьЗадания = true;
    double Eps1 = Lev_liq_1 / Lev_liq;
    double Eps2 = 1. - Eps1;
    Lev_liq = Lev_0 + ( Lev_100 - Lev_0 ) * -LevZ * 0.01;
    if ( Is_2F )
      {
      New_M_liq ( 1, CalcVol( Eps1 * Lev_liq ) * Vol / M3_liq_1 );
      New_M_liq ( 2, CalcVol( Eps2 * Lev_liq ) * Vol / M3_liq_2 );
      }
    else
      New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq );
    }
  if ( LevZ_2 > 0. && Is_2F && Lev_liq_2 > 0.01 )
    {
    Lev_liq_2 = Lev_0 + ( Lev_100 - Lev_0 ) * LevZ_2 * 0.01;
    New_M_liq ( 2, Lev_liq_2 * Vol / M3_liq_2 );
    LevZ_2 = 0.;
    }
  if ( LevZ_2 < 0. && Is_2F && Lev_liq_2 > 0.01 )
    {
    Lev_liq_2 = Lev_0 + ( Lev_100 - Lev_0 ) * -LevZ_2 * 0.01;
    New_M_liq ( 2, Lev_liq_2 * Vol / M3_liq_2 );
    }
  }

void CVolBase::Level_2Vol( double dt )
  {
  //if ( strstr ( ObjName, "Е-101") )
  //  KKK();
  double V_Liq_1in2 = 0.;
  double V_Liq_2in1 = 0.;
  if ( Calc_2F )
    {
    if ( kStep % 10 == 0 )
      {
      M3_liq_1	= To_m3 ( T, IsComp, Cmol_1F	);
      M3_liq_2	= To_m3 ( T, IsComp, Cmol_2F	);
      }
    M_Liq_1 = M_Liq * Eps_1F;
    M_Liq_2 = M_Liq - M_Liq_1;
    Mw_1F = ::Mw ( IsComp, Cmol_1F	);
    Mw_2F = ::Mw ( IsComp, Cmol_2F	);
    M_Liq_1_kg = M_Liq_1 * Mw_1F;
    M_Liq_2_kg = M_Liq_2 * Mw_2F;
	if ( true )
	{
		V_Liq_1 = M_Liq_1 * M3_liq_1;
		V_Liq_2 = M_Liq_2 * M3_liq_2;
	}
	else
	{
		V_Liq_1 = M_Liq_1 * M3_liq;
		V_Liq_2 = 0;
	}
    if ( true )
      {             
      if ( Vol2F <= 0 )
        Vol2F = 1e-5;
      if ( V_Liq_2 > Vol2F )
        {
        Lev_liq_2 = 1.;  //!!
        Lev_liq_1in2 = 0.;
        V_Liq_2in1 = V_Liq_2 - Vol2F;
        V_Liq_1in2 = 0.;
        }
      else
        {
        Lev_liq_2 = V_Liq_2 / Vol2F; //!!
        V_Liq_2in1 = 0.;
        V_Liq_1in2 = Vol2F - V_Liq_2;
        if ( V_Liq_1in2 > V_Liq_1 )
          {
          V_Liq_1in2 = V_Liq_1;
          V_Liq_1 = 0;
          Lev_liq_1in2 = V_Liq_1in2 / Vol2F;
          }
        else
          {
          Lev_liq_1in2 = V_Liq_1in2 / Vol2F;
          V_Liq_1 -= V_Liq_1in2;
          }
        }
      }
    else
      {
      Lev_liq_2 = 0.;
      }
    Lev_liq_2in1 = CalcLev ( V_Liq_2in1 / Vol );
    double K = V_Liq_1 / Vol;
    if ( K > 1.05 )
      {
      double M = M_Liq_1 * 1.05 / K; 
      New_M_liq ( 1, M );
      }
    if ( OverFlow && K < 1. && K != 0.0)
      {
      double M = M_Liq_1 * 1.05 / K; 
      New_M_liq ( 1, M );
      }
    Lev_liq  = CalcLev ( ( V_Liq_1 + V_Liq_2in1 ) / Vol );
    Lev_liq_1 = Lev_liq - Lev_liq_2in1;
    }
  else
    {
    if ( kStep % 10 == 0 )
      M3_liq	= To_m3 ( T, IsComp, Cmol_Liq	);
    M3_liq_2	= M3_liq_1	= M3_liq;
	  double M_kg = M_Liq * Mw_liq;
	  _M_liq_kg = M_kg;
    M_Liq_1 = M_Liq;
    M_Liq_2 = 0.;
	  k_M_Lev = M3_liq / Vol;
    //V_Liq_1 = M_Liq * M3_liq;
    //double K = V_Liq_1 / Vol;
    //if ( K > 1.05 )
    //  {
    //  double M = M_Liq_1 * 1.05 / K; 
    //  New_M_liq ( 1, M );
    //  }
	  Lev_liq_1 = Lev_liq = CalcLev ( M_Liq * k_M_Lev );
    Lev_liq_2 = 0.;
	  _Lev_liq	= Lev_liq * 100.;
    }
//
  if ( VolZ > 0. )
    {
    double K = VolZ / Vol;
    M_Gas *= K;
    M = M_Liq + M_Gas;
    H = M_Liq * h_liq + M_Gas * h_gas;
    Vol = VolZ;
    VolZ = 0.;
    if ( Lev_liq > 0.01 )
      LevZ = ( Lev_liq - Lev_0 ) / ( Lev_100 - Lev_0 ) * 100.;
    }
  if ( OverFlow )
    {
    //if ( V_Liq_1 > V_Liq_2 )
    //  {
    //  double Delta = Vol - V_Liq_1 - V_Liq_2in1;
    //  New_M_liq ( 1, ( V_Liq_1 + Delta + V_Liq_1in2 ) / M3_liq_1 );
    //  }
    //else
    //  {
    //  double Delta = Vol + Vol2F - V_Liq_1 - V_Liq_2;
    //  New_M_liq ( 2, ( V_Liq_2 + Delta ) / M3_liq_2 );
    //  }
    }
  if ( K_F1 != 1. )
    {
    New_M_liq ( 1, M_Liq_1 * K_F1 );
    K_F1 = 1.;
    }
  if ( K_F2 != 1. )
    {
    New_M_liq ( 2, M_Liq_2 * K_F2 );
    K_F2 = 1.;
    }
  if ( LevZ > 0. && Lev_liq > 0.01 )
    {
    Lev_liq = Lev_0 + ( Lev_100 - Lev_0 ) * LevZ * 0.01;
    if ( Is_2F )
      New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq_1 );
    else
      New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq );
    LevZ = 0.;
    }
  if ( LevZ_2 > 0. && Is_2F && Lev_liq_2 > 0.01 )
    {
    Lev_liq_2 = Lev_0 + ( Lev_100 - Lev_0 ) * LevZ_2 * 0.01;
    New_M_liq ( 2, Lev_liq_2 * Vol2F / M3_liq_2 );
    LevZ_2 = 0.;
    }
  if ( LevZ_2 < 0. && Is_2F && Lev_liq_2 > 0.01 )
    {
    Lev_liq_2 = Lev_0 + ( Lev_100 - Lev_0 ) * -LevZ_2 * 0.01;
    New_M_liq ( 2, Lev_liq_2 * Vol2F / M3_liq_2 );
    }
  if ( EmptyVol )
    {
    Lev_liq = 0.01;
    if ( Is_2F )
      M3_liq	= To_m3 ( T, IsComp, Cmol_1F	);
    else
      M3_liq	= To_m3 ( T, IsComp, Cmol_Liq	);
    New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq );
    EmptyVol = false; 
    }
  if ( FullVol && Lev_liq > 0.001 )
    {
    Lev_liq = 1.;
    if ( Is_2F )
      M3_liq	= To_m3 ( T, IsComp, Cmol_1F	);
    else
      M3_liq	= To_m3 ( T, IsComp, Cmol_Liq	);
    New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq );
    FullVol = false;
    }
  if ( LevZ < 0. && Lev_liq > 0.01)
    {
    ЕстьЗадания = true;
    Lev_liq = Lev_0 + ( Lev_100 - Lev_0 ) * -LevZ * 0.01;
    if ( Is_2F )
      New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq_1 );
    else
      New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq );
    }
  if ( Lev_liq < LevMin )
    {
    ЕстьЗадания = true;
    if ( kStep % 10 == 0 )
      {
      if ( Is_2F )
        M3_liq	= To_m3 ( T, IsComp, Cmol_1F	);
      else
        M3_liq	= To_m3 ( T, IsComp, Cmol_Liq	);
      }
    Lev_liq = LevMin * 0.01;
    New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq );
    }
  if ( Lev_liq > LevMax )
    {
    ЕстьЗадания = true;
    double M_old = M_Liq;
    Lev_liq = LevMax;
    if ( kStep % 10 == 0 )
      {
      if ( Is_2F )
        M3_liq	= To_m3 ( T, IsComp, Cmol_1F	);
      else
        M3_liq	= To_m3 ( T, IsComp, Cmol_Liq	);
      }
    New_M_liq ( 1, CalcVol( Lev_liq ) * Vol / M3_liq );
    double M_new = M_Liq;
    _Sliv_kg = ( M_old - M_new ) / dt * Mw_liq;
    }
  }


void CVolBase::New_M_liq ( int Faza, double new_M )
  {
  if ( Trace )
    {
    KKK();
    }
  if ( Is_2F )
    {
    if ( Faza == 1 )
      {
      double dM = new_M - M_Liq_1;
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( IsComp[c] )
          {
          Cmol_Liq[c] = M_Liq * Cmol_Liq[c] + dM * Cmol_1F[c];
          }
        }
      M_Liq_1 += dM;
      }
    else
      {
      double dM = new_M - M_Liq_2;
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( IsComp[c] )
          {
          Cmol_Liq[c] = M_Liq * Cmol_Liq[c] + dM * Cmol_2F[c];
          }
        }
      M_Liq_2 += dM;
      }
    M_Liq = Norm ( IsComp, Cmol_Liq );
    KKK();
    }
  else
    {
    M_Liq = new_M;
    }
  M = M_Liq + M_Gas;
  H = M_Liq * h_liq + M_Gas * h_gas;
  }
