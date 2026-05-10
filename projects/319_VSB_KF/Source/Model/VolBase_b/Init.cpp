#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 


CVolBase_W::CVolBase_W()
  {
  memset ( this, 0, sizeof ( CVolBase_W ));
  }

extern bool SetVH;


CVolBase::CVolBase( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain ),
StateVol ( "Vol", this )
  {
  //
  TypeVol = 0;
  k_Node = 0;
  Trace = false;
  FullInitN2 = false;
  KSi_P = 1.;
  LevMin = 0.;
  LevZ = 0.;
  LevZ_2 = 0.;
  K_F1 = 1.;
  K_F2 = 1.;
  LevZabs = 0.;
  S.Clear();
#define NO_CLEAR_DEFECT
#include "IO_Clear.h"
#undef DEFECT
#define DEFECT(Defect,Comment) Defect = NULL;
#undef DEF_D
#define DEF_D(Var, vName, vDef, vMin, vMax ) Var = NULL;
#include "volbasepoints.h"
  EmptyVol = false;
  FullVol = false;
  Destroy = false;
  Defect = 0;
  kToAir = 0.;
  kStep = 0;
  F_lost = 0.;
  pVolHeat = NULL;
  kStepTz = 0;
  Q_ext = 0.;
  pAddComp = NULL;
  ЕстьЗадания = false;
  CLEAR(IsDelete)
    CLEAR(F_Del)
    Delete = false;
  k_Delete = 0.1;
  Model = "VolBase";
  Lev_0 = 0.1;
  Lev_100 = 0.9;
  Restore = true;
  PseudoWater = false;
  Flow_Water_In = 0.;
  OverFlow = false;
  Calc_2F = false;
  Eps_W_in_O = 0.05;
  Eps_O_in_W = 0.1;
  Type2F = 0;
  for ( int c = 0; c < LAST_GAS; c++)
    G1_G2[c] = 1.;
  P_Pz = 0.;
  if ( ::SetVH )
    {
    ::SetVH = false;
    SetVH();
    }
  H_node = NULL;
  Vol_Conn = NULL;
  pFlow = NULL;
  IsFlow = NULL;
  State_in = NULL;
  Flow_kg = NULL;
  Flow_Gas_mol = NULL;
  ID_Out = NULL;
  pComp_Out = NULL;
  kStep = 0;
  Slow_Mgas = 1.;
  Ro_liq = 1000.;
  Mw_1F = 18.; 
  M3_liq_1 = Mw_1F / Ro_liq;
  Mw_2F = 18.; 
  M3_liq_2 = Mw_2F / Ro_liq;
  Mw_liq = 18.;
  M3_liq = Mw_liq / Ro_liq;
//
  ReactionsParam.Param = NULL;
  ReactionsParam.P = &P;
  ReactionsParam.T = &T;
  ReactionsParam.IsComp = IsComp;
  ReactionsParam.X = Cmol_Liq;
  ReactionsParam.Y = Cmol_Gas;
  ReactionsParam.Q = &Q_ext;
  ReactionsParam.M_Liq = &M_Liq;
  pReactions = NULL;;
  Summ_Flow_kg = 0.;
  }

#define ZERO(Arr,Len) memset ( Arr, 0, Len);
void CVolBase::InitData( int kIO )
  {
  k_Node = kIO;
  pFlow = NewArr(CFlow*,(kIO+1));
  IsFlow = NewArr(bool,(kIO+1));
  State_in = NewArr(CSEps*,(kIO+1));
  Flow_kg = NewArr(double,(kIO+1));
  Flow_Gas_mol = NewArr(double,(kIO+1));
  ID_Out = NewArr(int,(kIO+1));
  pComp_Out = NewArr(CComp *,(kIO+1));
  }

CVolBase::~CVolBase()
  {
  }

int CVolBase::Step1()
  {
  if ( M3_liq == 0. )
    M3_liq = 0.018;
  if ( M3_liq_1 == 0. )
    M3_liq_1 = 0.018;
  if ( M3_liq_2 == 0. )
    M3_liq_2 = 0.018;
  return 0;
  }

void CVolBase::VolInit ( CComp * pComp0, double P0, double T0 )
  {
  //	#include "IO_Init.h"
  //	#include "volbasepoints.h"
  OverFlow = false;
  CompVol.Set( pComp0 );
  P = P0;
  T = T0;
  M = M_Gas = P * Vol * KSi_P * Gas_Vol / ( RP * ( T + TK ));
  M_Liq = 0.;
  M_Liq_1 = 0.;
  M_Liq_2 = 0.;
  M_Liq_1_kg =  0.;
  M_Liq_2_kg =  0.;
  V_Liq_1 =  0.;
  V_Liq_2 =  0.;
  Vcond = 0.;
// Азот
  CLEAR( Cmol_Gas );
	CLEAR( Cmol_Liq );
  CLEAR( Cmol_1F );
  CLEAR( Cmol_2F );
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    Gamma_mix[c] = 1.;
    }
	CLEAR( h_Gas );
	CLEAR( h_Liq );
	CLEAR( IsComp );
  Cmol_Gas[G_N2] = 1.;
  IsComp[G_N2] = true;
  h_Gas[G_N2] = pGas[G_N2]->H_IG ( T );
  //StateVol.Vol = Vol;
  //StateVol.PT ( P, T, pComp0 );
  //M_Gas = StateVol.V;
  //M_Liq = StateVol.L;
  //FINITE(M_Gas)
  //FINITE(M_Liq)
  //M = M_Gas + M_Liq;
  //StateVol.SetCompData ( IsComp, Cmol_Liq, Cmol_Gas, 
  //	                     h_Liq, h_Gas );
  Lev_liq = 0.;	
  Lev_liq_1 = 0.;
  Lev_liq_2 = 0.;
  Lev_liq_2in1 = 0.;
  int kFlow = k_Node;
  AddFlow.Init();
  pAddComp = new CNewComp ( "Долив", this );
  pFlow[kFlow] = NULL;
  char NameEps[32];
  sprintf_s ( NameEps, sizeof(NameEps), "Состояние долива" );
  State_in[kFlow] = new CSEps ( NameEps, this );
  /*
  */
  for ( int n = 0; n < kFlow; n++ )
    {
    sprintf_s ( NameEps, sizeof(NameEps), "In%02d", n );
    State_in[n] = new CSEps ( NameEps, this );
    }
  StateVol.T_min = T_min;
  StateVol.T_max = T_max;
  StateVol.NoInLiq[G_N2] = true;
  }

