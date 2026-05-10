#include "stdafx.h"
#include "IO.h"
#include "Err.h"
#include "CommProc.h"
#include "Data.h"


#include "SetMatr.h"

void CIO::Matr_1_In( double dt, double * A, double * B )
  {
  double * Eq = A;
  // Приемник
  Pizb = Pizb;
  double O = Omega * Omega_Defect * mOmega;
  if ( OK && Flow_mol_in_out < 0. )
    O = 1e-6;
  if ( Reg_Flow_Z_m3.IsOn() && SetFlow )
    {
    Eq[F_0] = 1.;
    B[0] = Reg_Flow_Z_m3 * iM3;
    return;
    }
  if ( Reg_Flow_Z_kg.IsOn() && SetFlow )
    {
    Eq[F_0] = 1.;
    if ( pFlow_in )
      Mw_src = _Mw = ::Mw ( pFlow_in->Get_Comp());
    else
      Mw_src = 18.;
    if ( Mw_src < 2. )
      Mw_src = 18.;
    B[0] = Reg_Flow_Z_kg / Mw_src;
    return;
    }
  //if ( Reg_Flow_Z_Sol_kg.On() && SetFlow && pSolid )
  //  {
  //  Eq[F_0] = 1.;
  //  double Flow_Sol_mol = Reg_Flow_Z_Sol_kg / pSolid->Mw;
  //  double C = pSolid->C;
  //  if ( C < 0.01 )
  //    C = 0.01;
  //  B[0] = Flow_Sol_mol / C;
  //  return;
  //  }
  Eq[P_0] = O;
  Eq[F_0] = -1.;
  B[0] = O * ( Pizb + 1. );
  }

void CIO::Matr_1_Out( double dt, double * A, double * B )
  {
  double * Eq = A;
  // Источник
  if ( Pizb < -1. )
    Pizb = Pizb;
  double O = Omega * Omega_Defect * mOmega;
  if ( OK && Flow_mol_in_out < 0. )
    O = 1e-6;
  if ( Reg_Flow_Z_m3.IsOn() && SetFlow )
    {
    Eq[F_0] = 1.;
    B[0] = Reg_Flow_Z_m3 * iM3;
    return;
    }
  if ( Reg_Flow_Z_kg.IsOn() && SetFlow )
    {
    Eq[F_0] = 1.;
    B[0] = Reg_Flow_Z_kg / Mw_src;
    return;
    }
  Eq[P_0] = O;
  Eq[F_0] = 1.;
  double _Pizb_out = Pizb;
  B[0] = O * ( _Pizb_out + 1. );
  }


void CIO::Reg_In( )
  {
  if ( kStep < 3 )
    return;
  // Приемник
  // Входит
  double kOmega_Z = k_Omega_Z * 1e-2; 
  if ( Reg_Flow_Z_kg.IsOn() && !SetFlow )
    {
    double Flow = fabs ( Flow_kg_in_out );
    double D = Reg_Flow_Z_kg - Flow;
    double K; 
    if ( Flow < 0.1 * Reg_Flow_Z_kg )
      K = 1.5;
    else
      {
      if ( Flow > 10. * Reg_Flow_Z_kg )
        K = 0.8;
      else
        K = LimdX ( kOmega_Z * ( Reg_Flow_Z_kg / Flow - 1. ), 0.1 ) + 1.;
      }
    Omega *= K;
    if ( Omega < Omega_min_Z )
      Omega = Omega_min_Z;
    if ( Omega > Omega_max_Z )
      Omega = Omega_max_Z;
    }
  //if ( Reg_Flow_Z_Sol_kg.On() && SetFlow && pSolid )
  //  {
  //  double Flow = fabs ( Flow_mol_in_out ) * pSolid->Mw * pSolid->C;
  //  double D = Reg_Flow_Z_kg - Flow;
  //  double K; 
  //  if ( Flow < 0.1 * Reg_Flow_Z_kg )
  //    K = 1.5;
  //  else
  //    {
  //    if ( Flow > 10. * Reg_Flow_Z_kg )
  //      K = 0.8;
  //    else
  //      K = LimdX ( kOmega_Z * ( Reg_Flow_Z_kg / Flow - 1. ), 0.1 ) + 1.;
  //    }
  //  Omega *= K;
  //  if ( Omega < Omega_min_Z )
  //    Omega = Omega_min_Z;
  //  if ( Omega > Omega_max_Z )
  //    Omega = Omega_max_Z;
  //  }

  if ( Reg_Flow_Z_m3.IsOn() && !SetFlow )
    {
    double Flow = fabs ( Flow_m3_in_out );
    double K; 
    if ( Flow < 0.1 * Reg_Flow_Z_m3 )
      K = 1.5;
    else
      {
      if ( Flow > 10. * Reg_Flow_Z_m3 )
        K = 0.8;
      else
        K = LimdX ( kOmega_Z * ( Reg_Flow_Z_m3 / Flow - 1. ), 0.1 ) + 1.;
      }
    Omega *= K;
    if ( Omega < Omega_min_Z )
      Omega = Omega_min_Z;
    if ( Omega > Omega_max_Z )
      Omega = Omega_max_Z;
    }
  if ( Reg_P_in_Z.IsOn() && Type == In )
    {
    double dP_in = ( P_in - 1. ) - Pizb;  // Все (и)
    double dP_in_Z = Reg_P_in_Z - Pizb;
    double K;
    if ( dP_in > 10. * dP_in_Z )
      K = 1.5;
    else
      {
      if ( dP_in < 0.1 * dP_in_Z )
        K = 0.8;
      else
        K = LimdX ( kOmega_Z * ( dP_in / dP_in_Z - 1. ), 0.1 ) + 1.;
      }
    Omega *= K;
    if ( Omega < Omega_min_Z )
      Omega = Omega_min_Z;
    if ( Omega > Omega_max_Z )
      Omega = Omega_max_Z;
    }
  }

void CIO::Reg_Out(  )
  {
  if ( kStep < 3 )
    return;
// Источник
  //double F_Z_kg = Flow_Z_kg;
  //if ( Control.Used )
  //  {
  //  F_Z_kg = Control_flow;
  //  }
  double kOmega_Z = k_Omega_Z * 1e-3; 
  if ( Reg_Flow_Z_kg.IsOn() && !SetFlow )
    {
    double Flow = fabs ( Flow_kg_in_out );
    double K; 
    if ( Flow < 0.1 * Reg_Flow_Z_kg )
      K = 1.5;
    else
      {
      if ( Flow > 10. * Reg_Flow_Z_kg )
        K = 0.8;
      else
        K = LimdX ( kOmega_Z * ( Reg_Flow_Z_kg / Flow - 1. ), 0.1 ) + 1.;
      }
    Omega *= K;
    if ( Omega < Omega_min_Z )
      Omega = Omega_min_Z;
    if ( Omega > Omega_max_Z )
      Omega = Omega_max_Z;
    }
  if ( Reg_Flow_Z_m3.IsOn()&& !SetFlow )
    {
    double Flow = fabs ( Flow_m3_in_out );
    double K; 
    if ( Flow < 0.1 * Reg_Flow_Z_m3 )
      K = 1.5;
    else
      {
      if ( Flow > 10. * Reg_Flow_Z_m3 )
        K = 0.8;
      else
        K = LimdX ( kOmega_Z * ( Reg_Flow_Z_m3 / Flow - 1. ), 0.1 ) + 1.;
      }
    Omega *= K;
    if ( Omega < Omega_min_Z )
      Omega = Omega_min_Z;
    if ( Omega > Omega_max_Z )
      Omega = Omega_max_Z;
    }
  }

