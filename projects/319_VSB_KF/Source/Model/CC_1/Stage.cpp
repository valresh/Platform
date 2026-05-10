#include "stdafx.h"
#include "CC_1.h"

static double To_MWt =  1e-3 / 3600.;

double FiV( double V )
  {
  double x = V - 1.;
  if ( x < 1. )
    return 1.;
  double Fi = 0.5 + 0.75 * x - 0.25 * x * x * x;
  if ( Fi < 0. )
    Fi = 0.;
  return Fi;
  }

CStage_СС::CStage_СС( char * Name, IBaseModel * pMain ) : IBaseModel( Name, pMain ),
Thermo ( "Thermo", this )
  {
  ObjName = Name;
  Model = "ComprStage";
  Pow = 0.;
  pFlow_In = NULL;
  pFlow_Out = NULL;
  //
  _T_in = _T_out = 15.;
  _Pow_MWt = _P_in = _P_out = 0.;
  _Flow_kg = _Flow_m3 = 0.;
  _Zapas = 0.;
  Trace = false;
  Razgerm = false;
  OmegaRazgerm = 0.;
  def_k_dP = 1.;
  reg_k_dP = 1.;
  Fix_k_dP = 1.;
  Fix_k = false;
  k_Total = 1.;
  P_Out_Z = 1.;
  dP_Z = 0.;
  F_In_Z = 0.;
#define NO_DEFECT
#include "IO_Clear.h"
#include "Points_Stage.h"
#undef NO_DEFECT
  Thermo.Cp = 1.;
  WorkReg_PT = true;
  Use_P_Out_Z = false;
  Min_kdP = 1.05;
  Max_kdP = 4.;
  Reg_kdP = 0.01;
  Tin_0 = 20.;
  Mu_0 = 28.;
  k_dP_0 = 2.;
  Gamma = 1.4;
  KPD = 90.;
  KPD_M = 90.;
  Mw_gas = 28.;
  GetCurrData = false;
  }

void CStage_СС::InitState()
  {
  _T_in = _T_out = T_air;
  _Pow_MWt = _P_in = _P_out = 0.;
  _Flow_kg = _Flow_m3 = 0.;
  _Zapas = 0.;
  Thermo.InitState();
  }

// От свойств:
// P2/P1 = ( K*O*O*Mu/(R*T)*(g-1)/g + 1 ) ^ g / (g-1) стр 363

double CStage_СС::P2_P1_0( )
  {
  if ( Gamma < 1.01 )
    Gamma = 1.01;
  double GG = Gamma / ( Gamma - 1. );
  double W = K_P2_P1 * Oborot * Oborot * Mw_gas / ( GG * RG * ( _T_in + TK ));
  double P2_P1 = pow ( W + 1., GG );
  return P2_P1;
  }

void CStage_СС::Calc_K_P2_P1( )
  {
  // К-т для номинальных условий
  // P2/P1 = ( K*O*O*Mu/(R*T)*(g-1)/g + 1 ) ^ g / (g-1) стр 363
  if ( Gamma < 1.01 )
    Gamma = 1.01;
  double GG = Gamma / ( Gamma - 1. );
  if ( k_dP_0 < 1. )
    k_dP_0 = 1.;
  double W = pow ( k_dP_0, 1. / GG ) - 1.;
  K_P2_P1 = W * GG / Mu_0 * ( RG * ( Tin_0 + TK ));
  }

// Идеальный напор
void CStage_СС::Calc_dP ( )
  {
  if ( Use_P_Out_Z )
    {
    double K = P_Out_Z / pFlow_Out->P;
    k_dP_0 *=  LimdX( K - 1, Reg_kdP ) + 1.;
    if ( k_dP_0 < Min_kdP )
      k_dP_0 = Min_kdP;
    if ( k_dP_0 > Max_kdP )
      k_dP_0 = Max_kdP;
    Calc_K_P2_P1( );
    }
  if ( kReg_PZ > 0. && pSys->bWorkReg && WorkReg_PT )
    {
    Fix_k = false;
    bool OK = true;
    if ( F_In_Z > 0. )
      {
      double K;
      if ( F_In_Z	> 2. * Flow_nm3 )
        K = 2;
      else
        {
        if ( F_In_Z	< 0.5 * Flow_nm3 )
          K = 0.5;
        else
          K = F_In_Z / Flow_nm3;
        }
      K = LimdX ( kReg_PZ * ( K - 1. ), 0.1 ) + 1.;  
      k_dP_0 *= K;
      if ( k_dP_0 < 1. )
        k_dP_0 = 1.;
      if ( k_dP_0 > 5. )
        k_dP_0 = 5.;
      Calc_K_P2_P1( );
      OK = false;
      }
    if ( OK && dP_Z > 0. )
      {
      double dP = Thermo.P_out - Thermo.P_in;
      double K;
      K = LimdX ( kReg_PZ * ( dP_Z / dP - 1. ), 0.1 ) + 1.;  
      k_dP_0 *= K;
      if ( k_dP_0 < 1. )
        k_dP_0 = 1.;
      if ( k_dP_0 > 5. )
        k_dP_0 = 5.;
      OK = false;
      Calc_K_P2_P1( );
      }
    }
  bool ReCalc = false;
  if ( Mu_0 <= 0. )
    {
    Mu_0 = Mw( pFlow_In->Get_Comp());
    ReCalc = true;
    }
  if ( Tin_0 <= 0. )
    {
    Tin_0 = Thermo.T_in;
    ReCalc = true;
    }
  if ( GetCurrData )
    {
    GetCurrData = false;
    Mu_0 = Mw( pFlow_In->Get_Comp());
    Tin_0 = Thermo.T_in;
    ReCalc = true;
    }
  if ( ReCalc )
    {
    Calc_K_P2_P1( );
    }
  double kP_Cond = P2_P1_0( );
  double W = ( kP_Cond - 1. ) * k_Total * def_k_dP * reg_k_dP;
  double k_dP_new = W + 1.;
  if ( Fix_k )
    {
    double K = Fix_k_dP / k_dP_new;
    k_dP_0 += Reg_kdP * LimdX( K - 1, 0.1 );
    Calc_K_P2_P1( );
    k_dP_new = Fix_k_dP;
    }
  if ( k_dP_new > 1.01 )
    k_dP = 0.9 * k_dP + 0.1 * k_dP_new;
  else
    k_dP = 0.5 * k_dP + 0.5 * k_dP_new;
  }

void CStage_СС::Calc ( double dt )
  {
  if( Trace )
    {
    KKK();
    }
  Flow_mol = pFlow_In -> Flow_mol;
  if ( Flow_mol < 0. )
    Flow_mol = 0.;
  Flow_nm3 = Flow_mol * M3;
  CComp * pComp = pFlow_In -> Get_Comp( );
   if ( pComp == NULL )
    {
    To_Air(Thermo.T_in)
    To_Air(Thermo.T_out)
    Pow = 0.;
    Mw_gas = 28.;
    return;
    }
  //
  if( Trace )
    {
    KKK();
    }
  Calc_dP ( );
  Thermo.pComp = pComp;
  //
  Thermo.H_in = pFlow_In -> h_Flow;
  Thermo.P_in = pFlow_In -> P;
  Thermo.P_out = pFlow_Out-> P;
  Thermo.KPD = KPD * 0.01;
  Thermo.T_Out_Z = T_Out_Z;
  Thermo.Gamma = Gamma;
  *pFlow_Out = *pFlow_In;
  if ( Flow_mol > 1. )
    {
    Thermo.T_out_max = T_out_max;
    Thermo.WorkReg_PT = WorkReg_PT;
    Thermo.Calc( dt );
    Pow = Thermo.dH * Flow_mol;
    pFlow_Out -> T = Thermo.T_out;
    pFlow_Out -> h_Flow = Thermo.H_out;
    _T_in = Thermo.T_in;
    _T_out = Thermo.T_out;
    if ( T_Out_Z > 0. && WorkReg_PT )
      KPD = Thermo.KPD * 100.;
    }
  else
    {
    Pow = 0.;
    To_Air(Thermo.T_in)
    To_Air(Thermo.T_out)
    }
  if( Trace )
    {
    KKK();
    }
  _T_in = Thermo.T_in;
  _T_out = Thermo.T_out;
  Mw_gas = ::Mw(pComp);
  //
  _Flow_kg = Flow_mol * pFlow_In->To_kg;
  _Flow_m3 = Flow_nm3;
  _P_in = pFlow_In -> P - 1.;
  _P_out = pFlow_Out-> P - 1.;
  _Pow_MWt = Pow * To_MWt / ( KPD_M * 0.01 );
  }

int CStage_СС::SaveState( )
  {
  S_CLASS("Stage_W",CStage_W)
  return 0;
  }

int CStage_СС::RestoreState( char * StrName )
  {
  COPY_FIRST
  R_CLASS("Stage_W",CStage_W)
  return 1;
  }


int CStage_СС::GetParams( char * )
  {
#include "IO_Parms.h"
#include "Points_Stage.h"
  return 0;
  }

int CStage_СС::UpdateParam( struct CParams & Param )
  {
  return 0;
  }
