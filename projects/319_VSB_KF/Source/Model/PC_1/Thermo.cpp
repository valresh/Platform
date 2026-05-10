#include "stdafx.h"
#include "PC_1.h"
#include "Err.h"
#include "CommProc.h"
#include "Data.h"


CThermo_PC::CThermo_PC( char * Name, IBaseModel * pMain ) :	IBaseModel ( Name, pMain ),
In ( "Вход", this )
  {
  ObjName = Name;
  Model = "Thermo PC";
  IsComp = NULL;
  Cmol = NULL;
  pComp = NULL;
  T_in = 0.;
  T_out_s0 = 20.;
  T_out = 20.;
  P_in = 0.;
  P_out = 0.;
  KPD = 1.;
  kCool = 0.;
  h_Cool = 0.;
  In.T_min = 0.1;
  In.T_max = 300.;
  T_out_min = 0.1;
  T_out_max = 300.;
  Gamma = 1.4;
  //for(USHORT i = 0; i < K_GAS; i++)
  //{
	 // aGamma[i] = 1.4;
  //}
  }


void CThermo_PC::Calc()
  {
  if ( P_in < 0.001 )
    P_in = 0.001;
  In.T_min = T_min;
  In.T_max = T_max;
  In.PH( P_in, H_in, pComp );
  T_in = In.T;
//  Gamma = 1.4;
  if ( P_out < P_in )
    P_out = P_in;
  // Изэнтропийный процесс
  //if( bGamma )
  //  {
	 // double cv_mid = 0.0;
	 // double cp_mid = 0.0;
	 // for(USHORT i = 0; i < LAST_GAS; i++)
	 //   {
		//  if(aGamma[i] != 0.0)
		//    {
		//	  double cp = pGas[i]->C_IG(T_in);
		//	  cp_mid += cp * pComp->Cmol[i];
		//	  cv_mid += cp / aGamma[i] * pComp->Cmol[i];		 
		//    }
	 //   }
	 // if(cv_mid != 0.0)
		//  Gamma = cp_mid / cv_mid;
  //  }
  if ( Gamma < 1. )
    Gamma = 1.;
  if ( Gamma > 1. )
    {
    double I = ( 1. - Gamma ) / Gamma;
    //    double G = Gamma / ( 1. - Gamma );
    //  Pi * Ti^^G = Po * To^^G
    //  To = Ti * (Pi/Po)^^(1/G) 
    double Ti = T_in + TK;
    double To = Ti * pow ( P_in / P_out, I );
    T_out_s0 = To - TK;
    }
  else
    // Изэнтропийный процесс
    {
    double dT = 1e10;
    double S_in, dS_indT; 
    S ( P_in, T_in, S_in, dS_indT );
    double S_out, dS_outdT; 
    int kIter = 0;
    while ( fabs ( dT ) > 1e-5 && kIter++ < 10 )
      {
      S ( P_out, T_out_s0, S_out, dS_outdT );
      double dS = S_in - S_out;
      dT = dS / dS_outdT;
      T_out_s0 += LimdX ( dT, 1. );
      if ( T_out_s0 > T_out_max )
        {
        T_out_s0 = T_out_max;
        break;
        }
      }
    }
//  double T_in_K = T_in + TK;
//  double T_out_s0_K = T_in_K * pow ( P_out / P_in, ( Gamma - 1. ) / Gamma );
//  T_out_s0 = T_out_s0_K - TK;
  //
  double H_in, dH_indT; 
  H ( T_in, H_in, dH_indT );  
  double H_out_s0, dH_out_s0dT; 
  H ( T_out_s0, H_out_s0, dH_out_s0dT );
  // Охлаждение - тем-же газом
  double H_in_cool = ( H_in + kCool * h_Cool ) / ( 1. + kCool );
  // Сжатие - компрессор
  double dH_s0 = H_out_s0 - H_in;
  dH = dH_s0 / KPD;
  H_out = H_in_cool + dH;
  // T_out
  double dT = 1e10;
  double h, dhdT; 
  int kIter = 0;
  while ( fabs ( dT ) > 1e-5 && kIter++ < 10 )
    {
    H ( T_out, h, dhdT );
    double dh = H_out - h;
    dT = LimdX ( dh / dhdT, 1. );
    T_out += dT;
    }
  TEST_FINITE(T_out,T_air)
  }

void CThermo_PC::S ( double P, double T, double &S, double &dSdT )
  {
  double Smix = 0.;
  double dSmixdT = 0.;
  double Sn, dSndT;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( !IsComp[n] )
      continue;
    double C = Cmol[n];
    if ( C == 0. )
      continue;
    pGas[n]-> S_IG( P, T, Sn, dSndT );
    Smix += C * Sn;
    dSmixdT += C * dSndT;
    }
  S = Smix;
  dSdT = dSmixdT;
  }

void CThermo_PC::H ( double T, double &H, double &dHdT )
  {
  double Hmix = 0.;
  double dHmixdT = 0.;
  double Hn, dHndT;
  bool * IsComp = pComp ->IsComp;
  double * Cmol = pComp ->Cmol;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( !IsComp[n] )
      continue;
    double C = Cmol[n];
    if ( C == 0. )
      continue;
    Hn = pGas[n]-> H_IG( T );
    dHndT = pGas[n]-> C_IG( T );
    Hmix += C * Hn;
    dHmixdT += C * dHndT;
    }
  H = Hmix;
  dHdT = dHmixdT;
  }

int CThermo_PC::SaveState ( )
  {
  S_CLASS("Thermo_Parm",CThermo_Parm)
    return 0;
  }

int CThermo_PC::RestoreState( char * StrName )
  {
  COPY_FIRST
  R_CLASS("Thermo_Parm",CThermo_Parm)
    return 1;
  }
