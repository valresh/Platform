#include "stdafx.h"
#define DLL_COLUMN
#include "Column_NE.h"
#include "Err.h"
#include "CommProc.h"


void CColumn_NE::Calc( double dt )
	{
  dt *= Acc;
  SET_BP BreakPoint;
  CalcPress ( dt );
//??  Doliv = Column.pStage[kStage]->Doliv * Column.pStage[kStage]->Mw_Liq;
//??  Sliv = Column.pStage[kStage]->Sliv * Column.pStage[kStage]->Mw_Liq;
  CStage_NE * pCub = Column.pStage[kStage];
  pCub->dLev_dt_max = dLev_dt_max;
  pCub->Lev_Opt = Lev_Opt;
  pCub->Max_Flow_Add = Max_Flow_Add;
  Column.Static_Calc_Mix = Static_Calc_Mix;
  Static_Fix_L = Column.Static_Fix_L;
  Column.ModelCond = ModelCond;
  Column.BreakPoint = BreakPoint;
  if ( nAir >= 0 )
    {
    Flow_Air = Flow_kg[nAir];
    if ( Flow_Air_Nom <= 0 )
      Flow_Air_Nom = Flow_Air;
    double A = Flow_Air / Flow_Air_Nom;
    if ( A > 1. )
      A = 1. + LimdX ( A - 1., 0.1 );
    Column.k_h_air = ( 1. - A ) * k_Air_st + A;
    if ( Column.k_h_air < 0.5 )
      Column.k_h_air = 0.5;
    Column.k_l_air = ( 1. - A ) * k_Air_L + A;
    if ( Column.k_l_air < 0.5 )
      Column.k_l_air = 0.5;
    }
  Column.Step( dt );
//
  double L_out_cub = 0.;
  for ( int nS = 0; nS < pCub->kSrc; nS++ )
    {
    CExtIO * pSrc = pCub->Src[nS];
    if ( pSrc->pFlow->Flow_mol < 0. )
      L_out_cub -= pSrc->pFlow->Flow_mol;
    }
  Static_dF_Fix_L = pCub->L_Summ_out - L_out_cub;
  if ( Lev_CubZ > 0. )
    {
// Проверяем - не газ ли
    double Ro = Ro_liq ( pCub->T_liq, pCub->IsComp, pCub->X );
    if ( Ro < 100.)
      {
      Lev_CubZ = 0.;
      pCub->M_liq = 0.;
      ModelMsg ( this, "Нельзя задать уровень в кубе - нет жидкости" );
      }
    }
  pCub->Lev_Z = Lev_CubZ;
  if ( Static_Calc_Mix && !Static_Fix_L )
    {
    pCub->M_liq += dt * Static_dF_Fix_L;
    if ( pCub->M_liq < 0. )
      pCub->M_liq = 0.;
    }
  CStage_NE * pSt_1 = Column.pStage[1];
  if ( Comp_N1t >= 0 && Comp_N1t < LAST_GAS )
    Cmol_N1t = pSt_1->Y[Comp_N1t] * 100.;
  if ( Comp_N2t >= 0 && Comp_N2t < LAST_GAS )
    Cmol_N2t = pSt_1->Y[Comp_N2t] * 100.;
  if ( Comp_N1b >= 0 && Comp_N1b < LAST_GAS )
    Cmol_N1b = pCub->X[Comp_N1b] * 100.;
  if ( Comp_N2b >= 0 && Comp_N2b < LAST_GAS )
    Cmol_N2b = pCub->X[Comp_N2b] * 100.;
  P_низа = Column.Show_Press ( pCub->P );
  P_верха = Column.Show_Press ( pSt_1->P );
	T_верха = pSt_1->T_gas;
	T_низа = pCub->T_liq;
  double L = Column.pStage[kStage]->Level;
  L_уровнемер = Lev_0_100( L, 0.1, 0.9 ) * 100.;
  Давление_верха = P_верха;
  Давление_куба = P_низа;
  Температура_верха = T_верха;
  Температура_куба = T_низа;                    
  Уровень_куба = L_уровнемер;
	L_куб = L * 100.;
////////////////////////////////////////
/*
  if ( pLevCtrl )
    {
    if ( pLevCtrl->TypeReg == 2 && nRegCtrl >= 0 )
      {
      CStage_NE * pCub_1 = Column.pStage[kStage-1];
      double FlowIn = pCub_1 -> L_Down - pCub->Vout;
      for ( int n = 0; n < pCub->kSrc; n++ )
        {
        CExtIO * pSrc = pCub->Src[n];
        if ( pSrc->nSrc != nRegCtrl )
          FlowIn += pSrc->pFlow->Flow_mol;
        }
      pLevCtrl->FlowIn = FlowIn;
      }
    pLevCtrl->Control( dt );
    if ( pLevCtrl->TypeReg == 1 )
      {
//Фиксировать
      Column.pStage[kStage]->Lev_Z = pLevCtrl->LevelZ;
      }
    else
      Column.pStage[kStage]->Lev_Z = 0.;
    if ( pLevCtrl->TypeReg == 3 && nRegCtrl >= 0 )
      {
//      Проходимость
      Src[nRegCtrl]->Omega = pLevCtrl->Omega;
      }
    }
*/
//
  /*
  for ( int n = 0; n < kIO; n++ )
    {
    int N = nST[n];
    if ( N <= 0 )
      continue;
    double L = Column.pStage[N]->Level;
    if ( L < 0.1 )
      Hydro_dP[n] = 10. * L * Hydro_dPmax[n];
    else
      Hydro_dP[n] = Hydro_dPmax[n];
    }
  */
	}

void CColumn_NE::CalcPress ( double dt )
  {
  //for ( int s = 1; s <= kStage; s++ )
  //  {
  //  FlowGasOut += Column.pStage[s]->FlowGasOut;
  //  }
  if ( Static_Calc_Mix && Static_Fix_P )
    {
    Column.pStage[1]->P = Static_P_fix + 1.;
    Static_dF_Fix_P = Column.pStage[1]->Vout - FlowGasOut;
    }
  else
    {
// Давление - по дисбалансу потоков
    if ( P_Top_Z != 0. )
      {
      Column.pStage[1]->P = P_Top_Z + 1.;
      Static_dF_Fix_P = Column.pStage[1]->Vout - FlowGasOut;
      }
    else
      {
      double dF = Column.pStage[1]->Vout - FlowGasOut;
      Static_dF_Fix_P = dF;
      double Ksi = RP * ( Column.pStage[1]->T_gas + TK ) / Vol_Col;
      double dP = LimdX ( dt * dF * Ksi, 0.5 );
      Column.pStage[1]->P += dP;
      if ( Column.pStage[1]->P < PressOut && FlowGasOut > 1. ) 
        Column.pStage[1]->P = PressOut;
      if ( Column.pStage[1]->P < 0.001 )
        Column.pStage[1]->P = 0.001;
      }
    }
//
  double P = Column.pStage[1]->P;
  //Column.pStage[kStage-1]->dP_Stage = 0.;
  //Column.pStage[kStage-2]->dP_Stage = 0.;
  for ( int n = 2; n <= kStage; n++ )
	  {
    P += Column.pStage[n-1]->dP_Stage;
    Column.pStage[n]->P += Slow_Press * ( P - Column.pStage[n]->P );
	  }
  }