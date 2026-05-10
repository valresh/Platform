#include "stdafx.h"
#define DLL_BaseCol
#include "BaseCol.h"
#include "math.h"
#include "Err.h"
#include "SetData.h"


void CBaseCol::InitState( )
  {
  for ( int n = 1; n <= nCub; n++ )
    {
    CStage * pSt = pStage[n];
    memset ( static_cast<CStage_W*>( pSt ), 0, sizeof ( CStage_W ));
    pSt->X[G_N2] = 1.;
    pSt->Y[G_N2] = 1.;
    pSt->P = 1.;
    pSt->T_gas = pSt->T_liq = 15.;
    for ( int c = 0; c < LAST_GAS; c++ )
      pSt->Gamma[c] = 1.;
    }
  CLEAR(IsComp)
  IsComp[G_N2] = true;
  }

void CBaseCol::InitCol(  int kStage, bool WaterCol, bool NH3Col, bool Azeotrope )
  {
  pStage = NewArr(CStage*,(kStage+1));
  nCub = kStage;
	char Name[64];
	CLEAR( IsComp );
	IsComp[G_N2] = true;
  for ( int n = 1; n <= nCub; n++ )
    {
		if ( n == nCub )
			sprintf_s ( Name, 64, "Куб" );
		else
			sprintf_s ( Name, 64, "т.%2d", n );
		pStage[n] = new	CStage ( Name, this );
		strcpy_s ( pStage[n]->Name, 32, Name );
//
    if ( n == nCub )
      {
			pStage[n]->h_st = h_cub;
      pStage[n]->ЭтоКуб = true;
      }
    pStage[n]->pCol = this;
    pStage[n]->Eps_In.pCol = this;
    pStage[n]->Eps_In.pStage = pStage[n];
    pStage[n]->Numb = n;///nCub - n;
		pStage[n]->IsComp = IsComp;
		pStage[n]->Eps_In.IsComp = IsComp;
		//pStage[n]->Eps_In.T_min = T_min;
		//pStage[n]->Eps_In.T_max = T_max;
  //  pStage[n]->Eps_In.pT_min = &T_min;
    //pStage[n]->Eps_In.pT_max = &T_max;
    //pStage[n]->Eps_In.pT_min = &T_min;
    //pStage[n]->Eps_In.pT_max = &T_max;
    pStage[n]->Eps_In.WaterCol = WaterCol;
    pStage[n]->Eps_In.Azeotrope = Azeotrope;
    pStage[n]->Eps_In.NH3Col = NH3Col;
		pStage[n]->Henry = Henry;
	  pStage[n]->Eps_In.Henry = Henry;
		pStage[n]->Alfa_Comp = Alfa_Comp;
//    pStage[n]->A_H2S = &A_H2S;
//    pStage[n]->B_H2S = &B_H2S;
    }
  Vol.Eps_In.WaterCol = WaterCol;
  Vol.pCol = this;
  Vol.Eps_In.pCol = this;
  Vol.Eps_In.pStage = NULL;
  Vol.Numb = 0;
  Vol.IsComp = IsComp;
  Vol.Eps_In.IsComp = IsComp;
  //Vol.Eps_In.T_min = T_min;
  //Vol.Eps_In.T_max = T_max;
  Vol.Eps_In.NH3Col = NH3Col;
  Vol.Henry = Henry;
  Vol.Eps_In.Henry = Henry;
  Vol.Alfa_Comp = Alfa_Comp;
//  Vol.A_H2S = &A_H2S;
//  Vol.B_H2S = &B_H2S;
  strcpy_s ( Vol.Name, 32, "Объем" );
  for ( int n = 1; n <= nCub; n++ )
    {
    if ( n > 1 )
      pStage[n]->pUp = pStage[n-1];
    if ( n < nCub )
      pStage[n]->pDown = pStage[n+1];
		}
//??  CalcH2SCoef( );
	double * pY = pStage[nCub]->Y;
  //for ( int n = 0; n < MAX_HENRY; n++ )
  //  {
  //  for ( int c = 0; c < LAST_GAS; c++ )
  //    {
  //    Henry_Stage[n][c] = 1.;
  //    }
  //  }
  Vol.InitVol( );
/*
  if ( CalcAdsDes )
    {
    switch ( AdsType )
      {
      case AdsH2S_MEA:
        {
        if ( pAdsGas_1 )
          {
          pAdsGas_1->G_GAS = G_H2S;
          pAdsGas_1->G_ABS = G_MDEA;
          }
        }
      break;
      case AdsCO2_MEA:
        {
        if ( pAdsGas_1 )
          {
          pAdsGas_1->G_GAS = G_CO2;
          pAdsGas_1->G_ABS = G_MDEA;
          }
        }
      break;
      case AdsCO2_K2CO3:
        {
        if ( pAdsGas_1 )
          {
          pAdsGas_1->G_GAS = G_CO2;
          pAdsGas_1->G_ABS = G_K2CO3;
          }
        if ( pAdsGas_2 )
          {
          pAdsGas_1->G_GAS = G_CO;
          pAdsGas_1->G_ABS = G_K2CO3;
          }
        }
      break;
      }
    }
*/
	}

void CBaseCol::InitWaterCol( )
  {
  for ( int n = 1; n <= nCub; n++ )
    {
    pStage[n]->Eps_In.WaterCol = true;
    }
  Vol.Eps_In.WaterCol = true;
	}

void CBaseCol::Step( double dt )
	{
  SET_BP BreakPoint;
//  TT.Start();
  bool IsSrc = false;
  for ( int n = 1; n <= nCub; n++ )
    {
    CStage * pS = pStage[n];
    if ( !_finite(pS->X[0]))
      pS->CorrData( );
    }
  for ( int n = 1; n <= nCub; n++ )
    {
    CStage * pS = pStage[n];
    for ( int s = 0; s < pS->kSrc; s++ )
      {
      CFlow * pF = pS->Src[s]->pFlow;
      if ( pF == NULL )
        continue;
      double F = pF->Flow_mol;
      if ( F < 0. )
        continue;
      if ( F >= Flow_src_min )
        {
        IsSrc = true;
        break;
        }
      }
    if ( IsSrc )
      break;
    }
  Work = IsSrc;
	Проблемы = 0;
	kStep++;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
    if ( Henry[n] < 1e-4 )
		  Henry[n] = 1.;
		}
	if ( n_Comp_Balance >= 0 )
		{
// Накопление на тарелках
		for ( int n = 1; n <= nCub; n++ )
			{
			CStage * pS = pStage[n];
			pS->M_Bal_0 = pS->M_liq * pS->X[n_Comp_Balance] + 
										pS->M_gas * pS->Y[n_Comp_Balance];
			}
		}
	if ( MustClear )
		{
//    MustClear = false;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( !ClearComp[c] && CtrlComp[c] <= 0 )
        continue;
		  IsComp[c] = false;
		  for ( int n = 1; n <= nCub; n++ )
			  {
			  CStage * pS = pStage[n];
			  pS->X[c] = 0.;
			  ::Norm ( IsComp, pS->X );
			  pS->Y[c] = 0.;
			  ::Norm ( IsComp, pS->Y );
			  }
//      ClearComp[c] = false;
		  }
    }
  if ( n_Comp_Delete >= 0 )
    {
    int c = n_Comp_Delete;
    IsComp[c] = false;
    for ( int n = 1; n <= nCub; n++ )
      {
      CStage * pS = pStage[n];
      pS->X[c] = 0.;
      ::Norm ( IsComp, pS->X );
      pS->Y[c] = 0.;
      ::Norm ( IsComp, pS->Y );
      }
    }
///////////////////////////////////////////////////////////////////////////
  for ( int n = 1; n <= nCub; n++ )
	  {
    if ( pStage[n]->FullStage )
      break;
    pStage[n]->CalcH_Eps( );
	  }
  // Внешний подогрев
  for ( int nh = 0; nh < MAX_HEAT; nh++ )
    {
    if ( pHeat_In[nh] == NULL )
      continue;
    CExtHeat * pH = pHeat_In[nh]->pNextHeat;
    while ( pH )
      {
      int nS = pH->nStage;
      if ( nS <= 0 )
        break;
      double A = 0., B = 0.;
      ASS( pH->SetCoef( A, B ))
      pStage[nS]->Eps_In.A_T_ext_heat += A;
      pStage[nS]->Eps_In.B_T_ext_heat += B;
      pH = pH->pNextHeat;
      }
    }
  for ( int n = nCub; n >= 1; n-- )
	  {
    if ( pStage[n]->FullStage )
      continue;
	  pStage[n]->FeedFlash( dt );
	  }
  if ( Vol.Work )
    Vol.FeedFlash( dt );
// Внешний подогрев
  for ( int nh = 0; nh < MAX_HEAT; nh++ )
    {
    if ( pHeat_In[nh] == NULL )
      continue;
    *pHeat_Out[nh]->pFlow = *pHeat_In[nh]->pFlow;
    CExtHeat * pH = pHeat_In[nh]->pNextHeat;
    while ( pH )
      {
      int nS = pH->nStage;
      if ( nS <= 0 )
        break;
      ASS( pH->CalcHeat( pStage[nS]->Eps_In.T_ext_heat ))
      pH = pH->pNextHeat;
      }
    }
  for ( int n = 1; n <= nCub; n++ )
	  {
    if ( pStage[n]->FullStage )
      break;
	  pStage[n]->CalcGas( dt );
	  }
  if ( Vol.Work )
    Vol.CalcGas( dt );
  for ( int n = 1; n <= nCub; n++ )
	  {
    if ( pStage[n]->FullStage )
      break;
	  pStage[n]->CalcLiq( dt );
	  }
  if ( Vol.Work )
    Vol.CalcLiq( dt );
////////////////////////////////////////////////////////
  for ( int n = 1; n <= nCub; n++ )
	  {
	  pStage[n]->H_SS = 0.;
	  }
  for ( int n = 1; n <= nCub; n++ )
	  {
    if ( pStage[n]->FullStage )
      break;
    double K = pStage[n]->Cp_korp * 60. / pStage[n]->Tau_SS;
    if ( Fast_Q_SS ) 
      K *= 1000.;
    if ( n > 1 )
	    pStage[n]->H_SS += K * ( pStage[n-1]->T_liq - pStage[n]->T_liq );
    if ( n < nCub )
	    pStage[n]->H_SS += K * ( pStage[n+1]->T_liq - pStage[n]->T_liq );
    }
  for ( int n = 1; n <= nCub; n++ )
	  {
    if ( pStage[n]->FullStage )
      break;
	  pStage[n]->CalcHeat_Eps( dt );
	  }
  if ( CaclOnVol )
    CalcVolPress( dt );
  else
	  CalcPress( dt );
//
  if ( CanOverflow && !Vol.Work && Vol.FullCub( ) )
    {
// Переход в режим 'объем'
    Vol.InitCopy ( nCub - 1 );
    pStage[nCub-2]->pDown = &Vol;
    Vol.pUp = pStage[nCub-2];
    }
//  TT.Stop();
  SetPoints( );
//===============================================================
	CLEAR(Comp_In)
	CLEAR(Comp_Out)
//=================================================================
  if ( n_Comp_Balance >= 0 )
		{
    strcpy ( Comp_Balance_Name, pGas[n_Comp_Balance]->Name );
		for ( int n = 1; n <= nCub; n++ )
			{
			CStage * pS = pStage[n];
			pS->M_Bal_1 = pS->M_liq * pS->X[n_Comp_Balance] + 
										pS->M_gas * pS->Y[n_Comp_Balance];
			double Fin = 0.;
			if ( pS->pUp )
				Fin += pS->pUp->Lout * pS->pUp->X[n_Comp_Balance];
			if ( pS->pDown )
				Fin += pS->pDown->Vout * pS->pDown->Y[n_Comp_Balance];
			double Fout = 0.;
			Fout += pS->Lout * pS->X[n_Comp_Balance];
      if ( n > 1 )
			  Fout += pS->Vout * pS->Y[n_Comp_Balance];
			int K = pS->kSrc;
			for ( int s = 0; s < K; s++ )
				{
				if ( pS->Src[s]->pFlow == NULL )
					continue;
				double F = pS->Src[s]->pFlow->Flow_mol;
				CComp * pComp = pS->Src[s]->pFlow->Get_Comp();
				if ( pComp == NULL )
					continue;
				if ( !pComp->IsComp[n_Comp_Balance] )
					continue;
				double C = pComp->Cmol[n_Comp_Balance];
				if ( F >= 0. )
					Fin += F * C;
				else
					Fout -= F * C;
				}
			pS->Fin_Bal = Fin;
			pS->Fout_Bal = Fout;
			pS->dFdt_Bal = ( pS->M_Bal_1 - pS->M_Bal_0 ) / dt;
			if ( GetAsyncKeyState ( VK_SHIFT ) & 0x8000 )
				pS->Summ_Bal_mol = ( Fin - Fout - pS->dFdt_Bal ) / Fin * 100.;
			else
				pS->Summ_Bal_mol = Fin - Fout - pS->dFdt_Bal;
			}
		}
//=================================================================
	Flow_In_mol = 0.;
	Flow_Out_mol = 0.;
	Flow_In_kg = 0.;
	Flow_Out_kg = 0.;
	H_In = 0.;
	H_Out = 0.;
  void * BAL = NULL;
  if ( WriteBalance )
    {
    BAL = Fopen ( "Баланс.csv", "wt");
    WriteBalance = false;
    Fprintf ( BAL, "Тарелка;Вход;Поток,моль;Поток,кг\n" );
    }
	for ( int n = 1; n <= nCub; n++ )
		{
		CStage * pSt = pStage[n];
    int kSrc = pSt->kSrc;
    if ( kSrc == 0 )
      continue;
    for ( int s = 0; s < kSrc; s++ )
      {
	    CComp * pComp;
	    double F_mol, F_kg, h; 
			if ( pSt->Src[s]->pFlow == NULL )
				continue;
      F_mol = pSt->Src[s]->pFlow->Flow_mol;
//
      if ( BAL )
        {
        Fprintf ( BAL, "%d;%d;%4.2lf;%4.2lf;\n", n, s + 1, F_mol, F_mol * Mw( pSt->Src[s]->pFlow->Get_Comp()));
        }
      if ( F_mol > 0. )
        {
		    pComp =	pSt->Src[s]->pFlow->Get_Comp();
        if ( pComp == NULL )
          continue;
	      F_kg = pSt->Src[s]->pFlow->Flow_kg();
		    h = pSt->Src[s]->pFlow->h_Flow;
	      Flow_In_mol += F_mol;
	      Flow_In_kg += F_kg;
	      H_In += F_mol * h;
	      bool * IsComp =	pComp->IsComp;
	      double * Cmol =	pComp->Cmol;
	      for ( int n = 0; n < LAST_GAS; n++ )
		      {
		      if ( IsComp[n] )
			      {
			      Comp_In[n] += F_mol * Cmol[n];
			      }
		      }
        }
      else
        {
        F_mol = -F_mol;
        F_kg = pSt->Src[s]->pFlow->Flow_kg();
        Flow_Out_mol += F_mol;
        Flow_Out_kg += F_kg;
        h = pSt->Src[s]->pFlow->h_Flow;
        H_Out += F_mol * h;
        CComp * pComp =	pSt->Src[s]->pFlow->Get_Comp();
        if ( pComp == NULL )
          continue;
        bool * IsComp =	pComp->IsComp;
        double * Cmol =	pComp->Cmol;
        for ( int n = 0; n < LAST_GAS; n++ )
          {
          if ( IsComp[n] )
            {
            Comp_Out[n] += F_mol * Cmol[n];
            }
          }
        }
      }
    }
  if ( BAL )
    Fclose ( BAL );
	if(Flow_Out_mol != 0.0)
		Flow_Balance_mol = Flow_In_mol / Flow_Out_mol;
	else
		Flow_Balance_mol = 0.0;
	if(Flow_Out_kg != 0.0)
		Flow_Balance_kg = Flow_In_kg / Flow_Out_kg;
	else
		Flow_Balance_kg = 0.0;
	if(H_Out != 0.0)
		H_Balance = H_In / H_Out;
	else
		H_Balance = 0.0;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( Comp_Out[n] > 1e-3 )
			Comp_Balance[n] = Comp_In[n] / Comp_Out[n];
		else
			Comp_Balance[n] = 0.;
		}
	ЕстьПроблемы = Проблемы;
//??//  Timer >> 0;
	}

void CBaseCol::CalcVolPress( double dt )
  {
  double Alfa[256];
  double Beta[256];
  double P_old[256];
  CStage * pSt = pStage[1];
  int kSrc = pSt->kSrc;
  ASS( kSrc > 0 )
  double Flow_col_out = 0.;
  for ( int s = 0; s < kSrc; s++ )
    {
    if ( pSt->Src[s]->Gas && pSt->Src[s]->pFlow )
      {
      double F_mol = pSt->Src[s]->pFlow->Flow_mol;
      if ( F_mol < 0. )
        Flow_col_out += F_mol;
      }
    }
  if ( UseGasOut )
    {
    for ( int n = 2; n <= nCub; n++ )
      {
      CStage * pSt = pStage[n];
      Flow_col_out -= pSt->FlowGasOut;
      }
    }
  for ( int n = 1; n < nCub; n++ )
    {
    if ( pStage[n]->dP_nom < 1e-6 )
      pStage[n]->dP_nom = 1e-6;
    pStage[n]->Omega = pStage[n]->V_nom * iM3 / pStage[n]->dP_nom;
    }
///////////
  double Ksi = dt * RP * 298. / pStage[1]->Vol_Stage;//pStage[1]->Ksi_Vol;
  double O = pStage[1]->Omega;
  double Z = 1. / ( 1. + Ksi * O );
  Alfa[2] = Ksi * O * Z;
  Flow_col_out = -Flow_col_out;
  Beta[2] = ( pStage[1]->P + Ksi * ( pStage[1]->V_Cond_Gas - Flow_col_out )) * Z;
  P_old[1] = pStage[1]->P;
  for ( int n = 2; n < nCub; n++ )
    {
// a[i]*P[i-1]+b[i]*P[i]+c[i]*P[i+1] = d[i]
 //   double Ksi = dt * pStage[n]->Ksi_Vol;
    double Oi = pStage[n]->Omega;
    double Oi1 = pStage[n-1]->Omega;
    double a = -Ksi * Oi1;
    double c = -Ksi * Oi;
    double b = 1. + Ksi * ( Oi + Oi1 );
    double d = pStage[n]->P + Ksi * pStage[n]->V_Cond_Gas;
    double Z = 1. / ( a * Alfa[n] + b );
    Alfa[n+1] = -c * Z;
    Beta[n+1] = ( d - a * Beta[n] ) * Z;
    P_old[n] = pStage[n]->P;
    }
  P_old[nCub] = pStage[nCub]->P;
//
//  Ksi = dt * pStage[nCub]->Ksi_Vol;
  double Oi1 = pStage[nCub-1]->Omega;
  double P_Cub = ( pStage[nCub]->P + Ksi * ( pStage[nCub]->V_Cond_Gas + Oi1 * Beta[nCub] )) /
             ( 1. + Ksi * Oi1 * ( 1. - Alfa[nCub] ));
  pStage[nCub]->P = P_Cub;
  for ( int n = nCub; n >= 2; n-- )
    {
    double Oi1 = pStage[n-1]->Omega;
    pStage[n-1]->P = Alfa[n] * pStage[n]->P + Beta[n];
    pStage[n]->Vout = Oi1 * ( pStage[n]->P - pStage[n-1]->P );
    }
  pStage[1]->Vout = Flow_col_out;
  double Test[256];
  double Oi = pStage[1]->Omega;
  double Vi = Oi * ( pStage[2]->P - pStage[1]->P ) + pStage[1]->V_Cond_Gas;
  double Vo = Flow_col_out;
  Test[1] =  pStage[1]->P - ( P_old[1] + Ksi * ( Vi - Vo ));
  for ( int n = 2; n < nCub; n++ )
    {
    // a[i]*P[i-1]-b[i]*P[i]+c[i]*P[i+1] = d[i]
    //   double Ksi = dt * pStage[n]->Ksi_Vol;
    double Oi = pStage[n]->Omega;
    double Oi1 = pStage[n-1]->Omega;
    double Vi = Oi * ( pStage[n+1]->P - pStage[n]->P ) + pStage[n]->V_Cond_Gas;
    double Vo = Oi1 * ( pStage[n]->P - pStage[n-1]->P );
    Test[n] = pStage[n]->P - (P_old[n] + Ksi * ( Vi - Vo ));
    }
  Oi1 = pStage[nCub-1]->Omega;
  Vo = Oi1 * ( pStage[nCub]->P - pStage[nCub-1]->P );
  Vi = pStage[nCub]->V_Cond_Gas;
  Test[nCub] = pStage[nCub]->P - (P_old[nCub] + Ksi * ( Vi - Vo ));
  }

void CBaseCol::CalcPress( double dt )
  {
  Flow_1 = pStage[1]->Vout;
  if ( !GasColumn )
    {
    //double FlowGasOut = 0.;
    //for ( int n = 1; n <= nCub; n++ )
    //  {
    //  CStage * pSt = pStage[n];
    //  FlowGasOut += pSt->FlowGasOut;
    //  }
    double Flow_col_out = 0.;
    //FlowGasOut = 0.;
    if ( UseGasOut )
      {
      for ( int n = 2; n <= nCub; n++ )
        {
        CStage * pSt = pStage[n];
        Flow_col_out -= pSt->FlowGasOut;
        }
      }
    CStage * pSt = pStage[1];
    int kSrc = pSt->kSrc;
    for ( int s = 0; s < kSrc; s++ )
      {
      if ( pSt->Src[s]->Gas && pSt->Src[s]->pFlow )
        {
        double F_mol = pSt->Src[s]->pFlow->Flow_mol;
        if ( F_mol < 0. )
          Flow_col_out += F_mol;
        }
      }
    Flow_out = Flow_col_out;
    if ( P_Top_Z > 0. )
      P_top = P_Top_Z;
    else
      {
      double F_out = Flow_1;
      if ( Vol.Overflow )
        {
        F_out = Vol.Disbalance;
        }
      double dF = F_out + Flow_out; // - FlowGasOut
      double dP = LimdX ( dt / dt_Vol * dF, 0.1 );
      P_top += dP;
      if ( P_top < P_min )
        P_top = P_min;
      if ( Razgerm )
        {
        double F = OmegaRazgerm * ( P_top - 1. );
        double Ksi = dt / dt_Vol * OmegaRazgerm;
        P_top = ( P_top + Ksi ) / ( 1. + Ksi );
        }
      }
    }
  /////////////////////////////////////////////////
  if ( P_Top_Z > 0. )
    {
    Проблемы |= PROB_FIXED_P;
    P_top = P_Top_Z;
    }
  if ( P_top < P_min )
    {
    Проблемы |= PROB_MIN_P;
    P_top = P_min;
    }
  if ( P_top > P_max )
    {
    Проблемы |= PROB_MAX_P;
    P_top = P_max;
    }
  if ( Разгерметизация_Верха )
    P_top = 1.;
  double P = P_top;
  double _old = 1. - k_P_stage;
  double _new = k_P_stage;
  for ( int n = 1; n <= nCub; n++ )
    {
    //    FINITE(P)
    if ( pStage[n]->FullStage )
      break;
    pStage[n]->P = _old * pStage[n]->P + _new * P;
    FINITE(pStage[n]->P)
    if ( n < nCub )
      {
//	    pStage[n]->V_nom = pStage[n+1]->Vout * M3;
//      pStage[n]->dP_nom = pStage[n+1]->Vout / pStage[n]->Omega;
//      P = pStage[n]->P + pStage[n]->dP_nom;
      if ( pStage[n]->dP_nom < 1e-6 )
        pStage[n]->dP_nom = 1e-6;
      pStage[n]->Omega = pStage[n]->V_nom * iM3 / pStage[n]->dP_nom;
      P = pStage[n]->P + pStage[n+1]->Vout / pStage[n]->Omega;
      }
    if ( P > P_max )
      P = P_max;
    //		if ( pStage[n]->UseGydroIndP	)
    //			P += pStage[n]->dh_hydro;
    FINITE(P)
    }
  Vol.P = P;
  }

int CBaseCol::SetData( int TypeData, void * pData )
  {
  if ( TypeData == sd_GetComp )
    {
    CGetComp * pC = (CGetComp*)pData;
    int nSt = pC->nSt;
    if ( nSt < 1 )
      nSt = 1;
    if ( nSt > nCub )
      nSt = nCub;
    pC->nSt = nSt;
    CStage * pS = pStage[nSt];
    memmove ( pC->IsComp, pS->IsComp, sizeof ( pC->IsComp ));
    memmove ( pC->Cmol, pS->X, sizeof ( pC->Cmol ));
    memmove ( pC->Henry, pS->Henry, sizeof ( pC->Henry ));
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( pS->IsComp[n] )
        pC->Faza[n] = 1;
      }
    pC->P = pS->P;
    pC->T = pS->T_liq;
    return sd_GetComp;
    }
  return 0;
  }

