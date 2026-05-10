#include "stdafx.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"
#include "math.h"
#include "Err.h"
#include "SetData.h"


void CBaseCol_NE::InitState( )
  {
  for ( int n = 1; n <= nCub; n++ )
    {
    CStage_NE * pSt = pStage[n];
    memset ( static_cast<CStage_W*>( pSt ), 0, sizeof ( CStage_W ));
    pSt->X[G_N2] = 1.;
    pSt->Y[G_N2] = 1.;
    pSt->P = 1.;
    pSt->M_liq = 0.;
    pSt->T_gas = pSt->T_liq = 15.;
    for ( int c = 0; c < LAST_GAS; c++ )
      pSt->Gamma[c] = 1.;
    pSt->Init();
    }
  CLEAR(IsComp)
  IsComp[G_N2] = true;
  }

void CBaseCol_NE::InitCol(  int kStage )
  {
  pStage = NewArr(CStage_NE*,(kStage+1));
  pCommonStage = NewArr(bool,(kStage+1));
  Flow_Stage_In_mol = NewArr(double,(kStage+1));
  Flow_Stage_Out_mol = NewArr(double,(kStage+1));
  Flow_Stage_Balance_mol = NewArr(double,(kStage+1));
  Flow_Stage_Balance_kg = NewArr(double,(kStage+1));
  Heat_Stage_In_mol = NewArr(double,(kStage+1));
  Heat_Stage_Out_mol = NewArr(double,(kStage+1));
  Heat_Stage_Balance_mol = NewArr(double,(kStage+1));
  Режим = NewArr(double,(kStage+1));
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
		pStage[n] = new	CStage_NE ( Name, this );
		strcpy_s ( pStage[n]->Name, 32, Name );
//
    if ( n == nCub )
      {
			pStage[n]->h_st = h_cub;
      pStage[n]->ЭтоКуб = true;
      }
    pStage[n]->pCol = this;
    pStage[n]->pCalcTom3 = &CalcTom3;
    pStage[n]->Numb = n;///nCub - n;
		pStage[n]->IsComp = IsComp;
		pStage[n]->Henry = Henry;
		pStage[n]->Alfa_Comp = Alfa_Comp;
    pStage[n]->Init();
    }
  for ( int n = 1; n <= nCub; n++ )
    {
    if ( n > 1 )
      pStage[n]->pUp = pStage[n-1];
    if ( n < nCub )
      pStage[n]->pDown = pStage[n+1];
    pStage[n]->Init();
		}
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    Henry[n] = 1.;
    Alfa_Comp[n] = 100.;
    }
  SetРежим ( false );
//  pParamsStage = pStage[1];
  ShowVariantParams = 0;
	}


void CBaseCol_NE::Step( double dt )
	{
  SET_BP BreakPoint;
//  TT.Start();
	Проблемы = 0;
	kStep++;
///////////////////////////////////////////////////////////////////////////
  Utils_1( );
///////////////////////////////////////////////////////////////////////////
  if ( N_BreakPoint )
    {
    if ( N_BreakPoint >= 1 && N_BreakPoint <= nCub )
      pStage[N_BreakPoint]->BreakPoint = true;
    else
      {
      int N = -N_BreakPoint;
      if ( N >= 1 && N <= nCub )
        pStage[N]->BreakPoint = false;
      }
    }
///////////////////////////////////////////////////////////////////////////
  for ( int n = 1; n <= nCub; n++ )
	  {
    if ( pStage[n]->FullStage )
      break;
//    pStage[n]->BreakPoint = BreakPoint;
    pStage[n]->CalcH( );
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
      pH = pH->pNextHeat;
      }
    }
  dT_abs_med = 0.;
  dT_abs_max = 0.;
//  Static_Fix_L = Static_Calc_Mix;
/////////////////////////////////////////////
  for ( int n = nCub; n >= 1; n-- )
	  {
    if ( pStage[n]->FullStage )
      continue;
    pStage[n]->_P = Show_Press( pStage[n]-> P );
    pStage[n]->Model_Cond( dt );
	  }
// Внешний подогрев
/*****
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
******/
  for ( int n = 1; n <= nCub; n++ )
	  {
    if ( pStage[n]->FullStage )
      break;
// ВЫвод газа
	  pStage[n]->CalcGas( dt );
	  }
  for ( int n = 1; n <= nCub; n++ )
	  {
    if ( pStage[n]->FullStage )
      break;
    if ( Static_Fix_L )
// Уровень и масса из L_Summ_out
      pStage[n]->SetLevel_Static( dt );
    else
// Уровень L_Summ_out из Массы
      pStage[n]->CalcLevel( dt );
// Вывод жидкости
	  pStage[n]->CalcLiq( dt );
	  }
////////////////////////////////////////////////////////
  for ( int n = 1; n <= nCub; n++ )
	  {
    if ( pStage[n]->FullStage )
      break;
    pStage[n]->CalcHeat( dt );
	  }
  dT_abs_med /= nCub;
  dT_med = dT_abs_med;
  dT_max = dT_abs_max;
  for ( int n = 1; n <= nCub; n++ )
    {
    if ( pStage[n]->FullStage )
      break;
    pStage[n]->Calc_dP( dt );
    }
//  CalcVolPress( dt );
//
  SetPoints( );
//===============================================================
	CLEAR(Comp_In)
	CLEAR(Comp_Out)
//=================================================================
  if ( n_Comp_Balance >= 0 )
		{
		for ( int n = 1; n <= nCub; n++ )
			{
			CStage_NE * pS = pStage[n];
			pS->M_Bal_1 = pS->M_liq * pS->X[n_Comp_Balance] + 
										pS->M_gas * pS->Y[n_Comp_Balance];
			double Fin = 0.;
			if ( pS->pUp )
				Fin += pS->pUp->L_Down * pS->pUp->X[n_Comp_Balance];
			if ( pS->pDown )
				Fin += pS->pDown->Vout * pS->pDown->Y[n_Comp_Balance];
			double Fout = 0.;
			Fout += pS->L_Down * pS->X[n_Comp_Balance];
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
  double F_kg;
  double Flow_Stage_In_kg[200];
  double Flow_Stage_Out_kg[200];
  CLEAR(Flow_Stage_In_kg)
  CLEAR(Flow_Stage_Out_kg)
  memset(Flow_Stage_In_mol,0,8*(nCub+1));
  memset(Flow_Stage_Out_mol,0,8*(nCub+1));
  memset(Heat_Stage_In_mol,0,8*(nCub+1));
  memset(Heat_Stage_Out_mol,0,8*(nCub+1));
  bool Print = false;
	for ( int n = 1; n <= nCub; n++ )
		{
    if ( n == nCub )
      KKK();
		CStage_NE * pSt = pStage[n];
    if ( n != nCub )
      {
      Flow_Stage_Out_mol[n] = pSt->L_Down;
      F_kg = pSt->L_Down * Mw( IsComp, pSt->X );
      Flow_Stage_Out_kg[n] += F_kg;
      Flow_Out_kg += F_kg;
      }
    if ( n != 1 )
      {
      Flow_Stage_Out_mol[n] += pSt->Vout;
      F_kg = pSt->Vout * Mw( IsComp, pSt->Y );
      Flow_Stage_Out_kg[n] += F_kg;
      Flow_Out_kg += F_kg;
      }
    Heat_Stage_Out_mol[n] = pSt->L_Down * pSt->h_liq_mix + pSt->Vout * pSt->h_gas_mix;
    if ( pSt->pDown )
      {
      Flow_Stage_In_mol[n] += pSt->pDown->Vout;
      Heat_Stage_In_mol[n] += pSt->pDown->Vout * pSt->pDown->h_gas_mix;
      F_kg = pSt->pDown->Vout * Mw( IsComp, pSt->pDown->Y );
      Flow_Stage_In_kg[n] += F_kg;
      Flow_In_kg += F_kg;
      }
    if ( pSt->pUp )
      {
      Flow_Stage_In_mol[n] += pSt->pUp->L_Down;
      Heat_Stage_In_mol[n] += pSt->pUp->L_Down * pSt->pUp->h_liq_mix;
      F_kg = pSt->pUp->L_Down * Mw( IsComp, pSt->pUp->X );
      Flow_Stage_In_kg[n] += F_kg;
      Flow_In_kg += F_kg;
      }
//
    int kSrc = pSt->kSrc;
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
      pComp =	pSt->Src[s]->pFlow->Get_Comp();
      if ( pComp == NULL )
        continue;
      if ( F_mol > 0. )
        {
        F_kg = F_mol * Mw( pComp );
        if ( Print )
          Fsend ("Balance.csv","%d;Src;%d;%3.2lf;;\n", n, s, F_kg );
        h = pSt->Src[s]->pFlow->h_Flow;
	      Flow_In_mol += F_mol;
        Flow_Stage_In_mol[n] += F_mol;
        Heat_Stage_In_mol[n] += F_mol * h;
        Flow_Stage_In_kg[n] += F_kg;
	      Flow_In_kg += F_kg;
	      H_In += F_mol * h;
	      bool * IsComp =	pComp->IsComp;
	      double * Cmol =	pComp->Cmol;
	      for ( int c = 0; c < LAST_GAS; c++ )
		      {
		      if ( IsComp[c] )
			      {
			      Comp_In[c] += F_mol * Cmol[c] * pGas[c]->Mw;
			      }
		      }
        }
      else
        {
        F_mol = -F_mol;
        F_kg = F_mol * Mw( pComp );
        if ( Print )
          Fsend ("Balance.csv","%d;Src;%d;;%3.2lf;\n", n, s, F_kg );
        Flow_Out_mol += F_mol;
        Flow_Out_kg += F_kg;
        h = pSt->Src[s]->pFlow->h_Flow;
        Flow_Stage_Out_mol[n] += F_mol;
        Heat_Stage_Out_mol[n] += F_mol * h;
        Flow_Stage_Out_kg[n] += F_mol * Mw( pComp );
        H_Out += F_mol * h;
        CComp * pComp =	pSt->Src[s]->pFlow->Get_Comp();
        if ( pComp == NULL )
          continue;
        bool * IsComp =	pComp->IsComp;
        double * Cmol =	pComp->Cmol;
        for ( int c = 0; c < LAST_GAS; c++ )
          {
          if ( IsComp[c] )
            {
            Comp_Out[c] += F_mol * Cmol[c] * pGas[c]->Mw;
            }
          }
        }
      }
    if ( Flow_Stage_Out_mol[n] != 0. )
      Flow_Stage_Balance_mol[n] = Flow_Stage_In_mol[n] - Flow_Stage_Out_mol[n];
    if ( Heat_Stage_Out_mol[n] != 0. )
      Heat_Stage_Balance_mol[n] = Heat_Stage_In_mol[n] / Heat_Stage_Out_mol[n];
    if ( Flow_Stage_Out_kg[n] != 0. )
      Flow_Stage_Balance_kg[n] = Flow_Stage_In_kg[n] - Flow_Stage_Out_kg[n];
    }
  if ( BAL )
    Fclose ( BAL );
  if ( Flow_Out_mol != 0. )
	  Flow_Balance_mol = Flow_In_mol - Flow_Out_mol;
  if ( Flow_Out_kg != 0. )
	  Flow_Balance_kg = Flow_In_kg - Flow_Out_kg;
  if ( H_Out != 0. )
	  H_Balance = H_In / H_Out;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( Comp_Out[n] > 1e-3 )
			Comp_Balance[n] = Comp_In[n] - Comp_Out[n];
		else
			Comp_Balance[n] = 0.;
		}
	ЕстьПроблемы = Проблемы;
	}


int CBaseCol_NE::SetData( int TypeData, void * pData )
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
    CStage_NE * pS = pStage[nSt];
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

