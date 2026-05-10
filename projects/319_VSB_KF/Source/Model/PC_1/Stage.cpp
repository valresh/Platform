#include "stdafx.h"
#include "PC_1.h"
#include "Err.h"
#include "CommProc.h"
#include "SR.h"

#undef M3
#undef iM3
#define M3 22.4273077591 // Моль в нм3 Р = 1.0332, T = 273.16
#define iM3 0.0445884994641965 // нм3 в Моль  Р = 1.0332, T = 273.16

static double To_MWt =  1e-3 / 3600.;
static double T_norm_K = 25. + TK; 

CStage_PC::CStage_PC( char * Name, IBaseModel * pMain ) :	IBaseModel ( Name, pMain )
, Thermo ( "Thermo", this )
, Рубашка ( "Рубашка", this )
  {
  ObjName = Name;
  Model = "Stage PC";
  Pow = 0.;
  Flow_Nom_m3 = 10000.;
  P_Nom = 15.;
  KPD = 90.;
  KPD_mech = 90.;
  Eps = 0.01;
  dP_in = 0.;
  dP_out = 0.;
  k_Flow = 0.1;
  pFlow_In = NULL;
  pFlow_Out = NULL;
  pCompr = NULL;
  //
  _T_in = _T_out = 15.;
  _Pow_MWt = _P_in = _P_out = 0.;
  _Flow_kg = _Flow_m3 = 0.;
  Trace = false;
  Razgerm = false;
  OmegaRazgerm = 0.;
  PowZ = 1.;
  K_heat = 100.;
  S_heat = 10.;
  Рубашка.pIn_Vol = &FlowM;
  }

void CStage_PC::Calc ( double dt )
  {
  if( Trace )
    {
    KKK();
    }
  if ( pFlow_In == NULL )
    return;
  double Flow_mol = pFlow_In->Flow_mol;
//  if ( Flow_mol < -0.01 )
//    *pFlow_In = *pFlow_Out;
//  else
  *pFlow_Out = *pFlow_In;
  if ( Flow_mol <= 0. && Oborot <= 0. )
    {
    Flow_Gas = 0.;
    Pow = 0.;
    CComp * pComp = pFlow_In -> Get_Comp( );
	  if(Рубашка.pOut_Trub && Рубашка.pIn_Trub)
		  *Рубашка.pOut_Trub = *Рубашка.pIn_Trub;
    if ( pComp == NULL )
      return;
    pFlow_Out->h_Flow = H_IG( T_air, pComp );	
    return;
    }
  double Flow_nm3 = Flow_mol * M3;
  CComp * pComp = pFlow_In -> Get_Comp( );
  if ( pComp == NULL )
    {
    Flow_Gas = 0.;
    Pow = 0.;
    return;
    }
  //
  if( Trace )
    {
    KKK();
    }
  Thermo.pComp = pComp;
  Thermo.IsComp = pComp->IsComp;
  Thermo.Cmol = pComp->Cmol;
  //
  double P_in;
  double P_out;
  double RT = RP * ( 20. + TK );
  P_in = pFlow_In -> P;
  P_out = pFlow_Out-> P;
  Thermo.H_in = pFlow_In -> h_Flow;
  Thermo.P_in = P_in;
  Thermo.P_out = P_out;
  Thermo.KPD = KPD * 0.01;
  Thermo.Calc( );
  //
  FlowM = *pFlow_In;
  FlowM.P = pFlow_Out->P; 
  FlowM.Flow_mol = pFlow_In->Flow_mol; 
  FlowM.T = Thermo.T_out; 
  FlowM.h_Flow = Thermo.H_out;
  //
  Рубашка.KS = 3.6 * K_heat * S_heat;
  Рубашка.pIn_Vol = &FlowM;
  if ( FlowM.Flow_mol < 0.1 )
    FlowM.Flow_mol = 0.1; 
  if ( Рубашка.pIn_Trub )
    {
//    if ( Рубашка.pIn_Trub->Flow_mol > 0.2
    Рубашка.Calc( dt );
    }
  else
    {
    Рубашка.T_out_Vol = Thermo.T_out;
    pFlow_Out -> T = Thermo.T_out;
  	pFlow_Out -> h_Flow = Thermo.H_out;
    }
  //
  //
  _T_in = Thermo.T_in;
  _T_out = Рубашка.T_out_Vol;
  if ( Flow_mol > 1. )
    Pow = Thermo.dH * Flow_mol / ( KPD_mech * 0.01 );
  else
    Pow = 0.;
  //
  if ( pCompr->On && pCompr->P_in_Z > 0. && PowZ > 0. )
    {
    //Flow_Gas = pFlow_In->Flow_mol;
    //if ( Flow_Gas > 0. )
    //  Flow_Nom_m3 += k_Flow * ( Flow_Gas * RP / ( PowZ * ( P_in / (_T_in + TK ) - Eps * P_out / (_T_out + TK ))) - Flow_Nom_m3 );
    double dP = pFlow_In->P - pCompr->P_in_Z - 1.;
    double K = 1. + LimdX ( dP * k_Flow, 0.01 );
    Flow_Nom_m3 *= K;
    }
//  else
    {
    double Flow_Gas_new = Flow_Nom_m3 * PowZ * ( P_in / (_T_in + TK ) - Eps * P_out / (_T_out + TK )) / RP;
    if ( Flow_Gas_new < 0. )
      Flow_Gas_new = 0.;
    TEST_FINITE(Flow_Gas_new,0.)
    Flow_Gas = 0.9 * Flow_Gas + 0.1 * Flow_Gas_new;
    }
  //						 
  if( Trace )
    {
    KKK();
    }
  //
  _Flow_kg = Flow_mol * pFlow_In->To_kg;
  _Flow_m3 = Flow_nm3 = Flow_Gas * M3;
  _P_in = pFlow_In -> P - 1.;
  _P_out = pFlow_Out-> P - 1.;
  _Pow_MWt = Pow * To_MWt;
  }

int CStage_PC::SaveState( )
  {
  S_CLASS("Stage_W",CStage_W)
  return 0;
  }

int CStage_PC::RestoreState( char * StrName )
  {
  COPY_FIRST
  R_CLASS("Stage_W",CStage_W)
  return 1;
  }

