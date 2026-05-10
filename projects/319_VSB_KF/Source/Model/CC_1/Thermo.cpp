#include "stdafx.h"
#define DLL_ComprStage
#include "CC_1.h"


CThermo_CC::CThermo_CC( char * Name, IBaseModel * pMain )	: IBaseModel( Name, pMain ),
In ( "In", this ),
Adiabat ( "Adiabat", this ), 
Loose ( "Loose", this )
  {
  ObjName = Name;
  Model = "Thermo CC";
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
  In.T_min = T_min;
  Adiabat.T_min = T_min;
  Loose.T_min = T_min;
  T_out_max = T_max;
  Cp = 100.;
  WorkReg_PT = true;
  }


void CThermo_CC::InitState()
  {
  In.T_min = T_min;
  Adiabat.T_min = T_min;
  Loose.T_min = T_min;
  T_out_max = T_max;
  T_in = T_air;
  T_out_s0 = T_air;
  T_out = T_air;
  P_in = 1.;
  P_out = 1.;
  In.P = 1.;
  In.T = T_air;
  Adiabat.P = 1.;
  Adiabat.T = T_air;
  Loose.P = 1.;
  Loose.T = T_air;
  }

void CThermo_CC::Calc( double dt )
  {
  IsComp = pComp->IsComp;
  Cmol = pComp->Cmol;
  In.PH( P_in, H_in, pComp );
  T_in = In.T;
  if ( Gamma > 1. )
    {
    double I = ( 1. - Gamma ) / Gamma;
    //    double G = Gamma / ( 1. - Gamma );
    //  Pi * Ti^^G = Po * To^^G
    //  To = Ti * (Pi/Po)^^(1/G) 
    double Ti = T_in + TK;
    double To = Ti * pow ( P_in/P_out, I );
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
  //
  double H_in, dH_indT; 
  H ( T_in, H_in, dH_indT );  
  double H_out_s0, dH_out_s0dT; 
  H ( T_out_s0, H_out_s0, dH_out_s0dT );
  // Охлаждение - тем-же газом
  double H_in_cool = ( H_in + kCool * h_Cool ) / ( 1. + kCool );
  //
  if ( P_in > P_out )
    {
    // Расширение - турбина
    double dH_s0 = H_in - H_out_s0;
    dH = dH_s0 * KPD;
    H_out = H_in_cool - dH;
    }
  else
    {
    // Сжатие - компрессор
    double dH_s0 = H_out_s0 - H_in;
    dH = dH_s0 / KPD;
    H_out = H_in_cool + dH;
    }
  double h; 
  if ( T_Out_Z > 0. && pSys->bWorkReg && WorkReg_PT )
    {
    double dhdT;
    T_out = T_Out_Z;
    H ( T_out, h, dhdT );
    double dH = h - H_in_cool;
    double dH_s0 = H_out_s0 - H_in;
    KPD = dH_s0 / dH;
    if ( KPD < 0.1 || KPD > 1. )
      {
      H_out = h;
      return;
      }
    dH = dH_s0 / KPD;
    H_out = H_in_cool + dH;
    }
  // T_out
  double dT = 1e10;
  double dhdT;
  int kIter = 0;
  if ( !_finite(T_out))
    T_out = T_in;
  double Cpdt = Cp / dt;
  double h0 = Cpdt * T_out;
  while ( fabs ( dT ) > 1e-5 && kIter++ < 10 )
    {
    H ( T_out, h, dhdT );
    double dh = H_out - h;
    dh -= Cpdt * T_out - h0;
    dhdT += Cpdt;
    dT = LimdX ( dh / dhdT, 1. );
    T_out += dT;
    if ( T_out < T_min )
      {
      T_out = T_min;
      break;
      }
    }
  H_out = h;
  }


void CThermo_CC::S ( double P, double T, double &S, double &dSdT )
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

void CThermo_CC::H ( double T, double &H, double &dHdT )
  {
  double Hmix = 0.;
  double dHmixdT = 0.;
  double Hn, dHndT;
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

void CThermo_CC::Calc_Steam()
  {
  // Изэнтропийный процесс
  In.PH( P_in, H_in, pComp );
  T_in = In.T;
  Eps_in = 1.;
  //
  Adiabat.PS( In, P_out ); 
  double H_out_s0 = Adiabat.h;
  //
  double dH_s0 = H_in - H_out_s0;
  dH = dH_s0 * KPD;
  H_out = H_in - dH;
  Loose.PH ( P_out, H_out, pComp );
  T_out = Loose.T;
  Eps_out = Loose.E;
  }

int CThermo_CC::SaveState( )
  {
  S_CLASS("W",CThermo_Parm);
  return 0;
  }

int CThermo_CC::RestoreState( char * StrName )
  {
  COPY_FIRST
    R_CLASS("W",CThermo_Parm);
  return 1;
  }
