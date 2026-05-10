#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol.h"

extern CGas::eTypeComp Type[K_GAS];

CStage_Ptr::CStage_Ptr()
	{
	memset ( this, 0, sizeof ( CStage_Ptr ));
	}

CStage_W::CStage_W()
	{
	memset ( this, 0, sizeof ( CStage_W ));
	Pz = 1.;
	}

CStage_P::CStage_P()
	{
	memset ( this, 0, sizeof ( CStage_P ));
	Gamma_1_in_2 = 50.;
	Gamma_2_in_1 = 50.;
	ClearComp = false;
 	CalcTom3 = false;
  Tau_SS = 30000.;
  H_SS = 0.;
  Eff = 1.;
  ExtHeat = 0.;
	}

CStage::CStage( char * _Name, IBaseModel * pMain ) :	IBaseModel ( _Name, pMain )
	{
	pUp = NULL;
	pDown = NULL;
	Yin = NULL;
  CLEAR(Name)
	CLEAR(Type)
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		ASS(pGas[n]->Type != CGas::None );
		Type[n] = pGas[n]->Type;
		}
  CLEAR(Name)
	Level = 0.;
	Flow_gas_in = 0.;
	Flow_liq_in = 0.;
//
	CLEAR(F_gas)
	CLEAR(F_liq)
	CLEAR(P_z)
	CLEAR(Vc_Comp)
	CLEAR(h_gas)
	CLEAR(h_liq)
	CLEAR(h_gas_T_liq)
	CLEAR(Xnew)
	CLEAR(P_comp)
	CLEAR(P_X)
	CLEAR(P_PzX)
//
  P = 1.;
	Pz = 1.;
	T_In = T_liq = T_gas = 15.;
	Vol_Stage = 2.;
  S_Stage = 5.;
	double RT = RP * ( T_gas + TK );
	M_gas = P * Vol_Stage / RT;
	Y[G_N2] = 1.;
	M_liq = 0.;
	h_st = 0.05;
	k_L = 3000.;
	Cp_gas = 30.;
	Cp_liq = 50.;
	Cp_korp = 2000.;
	P_src = 0.;
	Omega = 1e5;
	Alfa_Stage = 100.;
	HenryStage = 1.;
  Eps_In.HenryStage = &HenryStage;
  k_GL = 1e6;
	Lev_min = 0.;
	Lev_max = 100.;
  Hvap_Stage = 1.;
  T_liq_Z = 0.;
  k_Cool = 0.;
	dH_liq_Z = 0.;
	dH_gas_Z = 0.;
  SetdH = false;
  dT_liq_dH = 0.;
  dT_gas_dH = 0.;
	Fin_Bal = 0.;
	Fout_Bal = 0.;
	dFdt_Bal = 0.;
  UseGydroIndP = true;
	dP_Hydro_Static = 0.;
	dP_Hydro_Level = 0.;
	To_m3 = 0.;
	k_Eps = 1.;
//	pRegLev = NULL;
	M_min = 0.1;
  Gamma_H2S_In = 1.;
  Eps_In.T = &T_In;
  Eps_In.Gamma_H2S = &Gamma_H2S_In;
  lstrcpy ( DllName, "Stage" );
  kSrc = 0;
  kOut = 0;
  kExtLout = 0.;
  ExtLout = 0.;
	}

const double Cmin = 1e-9;
void CStage::CompressXY( )
  {
  int k = 0;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( IsComp[n] )
			{
      if ( X[n] < Cmin && Y[n] < Cmin )
        {
        X[n] = 0.;
        Y[n] = 0.;
        continue;
        }
      k++;
			}
		}
  if ( k == 0 )
    {
    CLEAR( IsComp )
    CLEAR( X )
    CLEAR( Y )
		IsComp[G_N2] = true;
		Y[G_N2] = 1.;
		X[G_N2] = 1.;
    }
  Norm ( IsComp, X );
  Norm ( IsComp, Y );
  }

void CStage::Add ( CExtSrc * pSrc )
  {
  ASS(kSrc < MAX_IN)
  Src[kSrc++] = pSrc;
  }

void CStage::Add ( COutput * pOut )
  {
  ASS(kOut < MAX_OUT)
  Out[kOut++] = pOut;
  }

int CStage::SaveState( )
  {
	S_CLASS("W",CStage_W);
	return 0;
	}

int CStage::RestoreState( char * StrName )
  {
	COPY_FIRST
	R_CLASS("W",CStage_W);
	return 1;
  }

int CStage::ShowParams( DWORD DataType, struct CShowData * pSD )
	{
	switch ( DataType )
		{
		case sp_Params:
			pSD->N = 0;
		  GetParams( pSD->N, pSD->Params );
			VisualParams( pSD->N, pSD->Params );
		return 1;
		}
	return 0;
	}

#define STAGE_PARM(Parm) PARM( Parm, "#"#Parm );
#include "IO_Parms.h"

void CStage::VisualParams( int & N, struct CParams Params[] )
	{
//  if ( CParams::SaveAllParams )
//    return;
	TAB_INSTR("Stop")
	PARM ( StopSrc, "#Src" );
	PARM ( StopLiq, "#Liq" );
	PARM ( StopGas, "#Gas" );
	PARM ( StopFlow, "#Flow" );
	PARM ( StopHeat, "#Heat" );
	TAB_INSTR("Параметры")
	STAGE_PARM(Flow_Summ_In)
	STAGE_PARM(L_In)
	STAGE_PARM(H_Summ_In)
	STAGE_PARM(T_liq)
	STAGE_PARM(T_gas)
	STAGE_PARM(P)
	STAGE_PARM(Vin)
	STAGE_PARM(Vout)
	STAGE_PARM(Lin)
	STAGE_PARM(Lout)
	STAGE_PARM(V_Cond)
	STAGE_PARM(Hvap_Stage)
	STAGE_PARM(Eps)
	STAGE_PARM(Pz)
	STAGE_PARM(P_Pz)
	STAGE_PARM(M_gas)
	STAGE_PARM(M_liq)
	STAGE_PARM(Level)
  STAGE_PARM(Doliv)
  STAGE_PARM(To_m3)
	STAGE_PARM(dh_hydro)
	STAGE_PARM(dT_liq)
	STAGE_PARM(dT_gas)
	STAGE_PARM(h_gas_mix)
	STAGE_PARM(h_liq_mix)
	char Name[64];
	TAB_INSTR("/X")
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
				sprintf_s ( Name, 64, "#X - %s", pGas[n]->Name );
				PARM ( X[n], Name );
				}
			}
	TAB_INSTR("/Y")
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
				sprintf_s ( Name, 64, "#Y - %s", pGas[n]->Name );
				PARM ( Y[n], Name );
				}
			}
	TAB_INSTR("/Pz")
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
				sprintf_s ( Name, 64, "#Pz - %s", pGas[n]->Name );
				PARM ( P_z[n], Name );
				}
			}
	TAB_INSTR("/P_X")
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
				sprintf_s ( Name, 64, "#P_X - %s", pGas[n]->Name );
				PARM ( P_X[n], Name );
				}
			}
	TAB_INSTR("/P_PzX")
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
				sprintf_s ( Name, 64, "#P_PzX - %s", pGas[n]->Name );
				PARM ( P_PzX[n], Name );
				}
			}
	TAB_INSTR("/Конденсация")
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
				sprintf_s ( Name, 64, "#Vcond - %s", pGas[n]->Name );
				PARM ( Vc_Comp[n], Name );
				}
			}
	TAB_INSTR("/X_In")
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
				sprintf_s ( Name, 64, "#X_In - %s", pGas[n]->Name );
				PARM ( X_In[n], Name );
				}
			}
	TAB_INSTR("/Y_In")
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
				sprintf_s ( Name, 64, "#Y_In - %s", pGas[n]->Name );
				PARM ( Y_In[n], Name );
				}
			}
	}

int CStage::GetParams( int & N, struct CParams Params[] )
	{
//	TAB_INSTR("Параметры тарелки")
  PARM ( Name, "#Имя" )
  PARM ( Vol_Stage, "Объем" )
  PARM ( S_Stage, "Площадь" )
  PARM ( Omega, "Проходимость" )
  PARM ( h_st, "Высота перелива" )
  PARM ( k_L, "Скорость слива" )
  PARM ( Cp_korp, "Cp корпуса" )
  PARM_DD ( k_GL, "k Gas-Liq", 1e6 )
//  PARM_DD ( k_Eps, "Поправка к высоте слоя", 1. )
  PARM ( Alfa_Stage, "Alfa" );
  PARM ( k_Cool, "k_Cool" ); 
  PARM_DD ( Gamma_1_in_2, "Gamma_1 во 2", 50. )
  PARM_DD ( Gamma_2_in_1, "Gamma_2 в 1", 50. )
  PARM_DD ( Lev_min, "Минимальный уровень", 0. )
  PARM_DD ( Lev_max, "Максимальный уровень", 100. )
  PARM_DD ( UseGydroIndP, "Учитывать гидростат в перепаде", true )
  PARM_DD ( dP_Hydro_Static, "Статический гидростат", 0. )
  PARM_DD ( dP_Hydro_Level, "Гидростат от уровня", 0. )
  PARM_DD ( Eff, "КПД тарелки", 1. )
  TAB_HIDE( "//Дополнение" )
  PARM ( P_src, "P источника" )
  PARM ( HenryStage, "HenryStage" );
  PARM ( Hvap_Stage, "Hvap_Stage" );
  PARM ( Cp_gas, "Cp газа" )
  PARM ( Cp_liq, "Cp жидкости" )
  PARM ( T_liq_Z, "T_liq_Z" );
  PARM ( dH_liq_Z, "dH_liq_Z" );
  PARM ( dH_gas_Z, "dH_gas_Z" );
  PARM ( kExtLout, "kExtLout" );
  PARM ( ExtLout, "#ExtLout")
  PARM_DD ( Tau_SS, "Теплообмен т-т", 30000. )
  Eps_In.GetParams( this, N, Params );
	return 0;
	}


void CStage::FeedFlash( double dt )
  {
  if ( StopSrc )
    {
    BREAK
    if ( StopSrc > 0 )
      StopSrc--;
    }
	H_Summ_In = 0.;
	Flow_Summ_In = 0.;
	CLEAR(Summ_In)
///////////////////////////////////////////////////
// Источники
	for ( int s = 0; s < kSrc; s++ )
		{
    CFlow * pF = Src[s]->pFlow;
		if ( pF == NULL )
			continue;
    CComp * pC = pF->Get_Comp();
		if ( pC == NULL )
			continue;
		double F = pF -> Flow_mol;
		if ( Src[s]->Output )
			F = -F;
		if ( F <= 0. )
			continue;
    Flow_Summ_In += F;
    H_Summ_In += F * pF->h_Flow;
    bool * IsCompF = pC->IsComp;
    double * CmolF = pC->Cmol;
		for ( int i = 0; i < LAST_GAS; i++ )
		  {
			if ( IsCompF[i] )
			  {
        IsComp[i] = true;
				Summ_In[i] += F * CmolF[i];
				}
			}
		}
///////////////////////////////////////////////////
// Сверху
  double h_u_liq = 0.;
	if ( pUp && pUp->Lout > 0. )
		{
		double F = pUp->Lout;
		double * uX = pUp -> X;
		double * uh_Liq = pUp -> h_liq;
    Flow_Summ_In += F;
		for ( int i = 0; i < LAST_GAS; i++ )
			{
			if ( IsComp[i] )
				{
				Summ_In[i] += F * uX[i];
        H_Summ_In += F * uX[i] * uh_Liq[i];
        h_u_liq += uX[i] * uh_Liq[i];
				}
			}
    double D_1 = h_u_liq / pUp->h_liq_mix;
    double D_2 = h_u_liq / pUp->Eps_In.h_liq;
		}
///////////////////////////////////////////////////
// Снизу
  double h_d_gas = 0.;
	if ( pDown && pDown->Vout > 0. )
		{
		double F = pDown->Vout * Eff;
		double * dY = pDown -> Y;
		double * dh_Gas = pDown -> h_gas;
    Flow_Summ_In += F;
		for ( int i = 0; i < LAST_GAS; i++ )
			{
			if ( IsComp[i] )
        {
				Summ_In[i] += F * dY[i];
        H_Summ_In += F * dY[i] * dh_Gas[i];
        h_d_gas += dY[i] * dh_Gas[i];
        }
			}
    double D = h_d_gas / pDown->Eps_In.h_gas;
		}
  if ( Flow_Summ_In < 0.01 )
    {
// Нет подачи
    L_In = 0.;
    V_Cond = 0.;
    Vout = 0.;
    return;
    }
  Norm ( IsComp, Summ_In );
  double h = H_Summ_In / Flow_Summ_In;
///////////////////////////////////////////////////
// Eps
  double Pe = P;
  if ( P_src > 0. )
    Pe = P_src;
  Eps_In.PH( Pe, h, Summ_In, T_In );
  T_gas = T_In;
  Eps = Eps_In.E;
///////////////////////////////////////////////////
  memmove ( X_In, Eps_In.X_W, CMOL_GAS );
  memmove ( Y_In, Eps_In.Y_W, CMOL_GAS );
  FINITE( X_In[0] )
  memmove ( Vc_Comp, X_In, CMOL_GAS );
  //for ( int i = 0; i < LAST_GAS; i++ )
	//  {
	//  if ( IsComp[i] )
	//	  {
  //    if ( Hvap_Stage == 1. )
  //      h_Liq_In[i] = Eps_In.h_Liq[i];
  //    else
  //      h_Liq_In[i] = Eps_In.h_Gas[i] - Hvap_Stage * ( Eps_In.h_Gas[i] - Eps_In.h_Liq[i] );
  //    }
  //  else
  //    h_Liq_In[i] = 0.;
  //  }
  memmove ( h_Liq_In, Eps_In.h_Liq, CMOL_GAS );
  memmove ( P_z, Eps_In.P_sat, CMOL_GAS );
	double V_In = Flow_Summ_In * Eps_In.E;
	L_In = Flow_Summ_In - V_In;
  double dM = L_In * dt;
  for ( int i = 0; i < LAST_GAS; i++ )
		{
		if ( IsComp[i] )
			{
			X[i] = M_liq * X[i] + dM * X_In[i];
 //     double Z = Eps * Y_In[i] + ( 1. - Eps ) * X_In[i];
			}
		}
  Norm ( IsComp, X );
  double Ps = 0.;
  for ( int i = 0; i < LAST_GAS; i++ )
		{
		if ( IsComp[i] )
			{
			Ps += X[i] * Eps_In.P_sat[i];
			}
		}
  Pz = Ps;
  P_Pz = P / Pz;
  M_liq += dM;
  double Ys = 0.;
  double hs = 0.;
	if ( pDown && pDown->Vout > 0. && Eff < 1. )
    {
    double Eps_D = 1. - Eff;
		double F_D = pDown->Vout * Eps_D;
		V_In += F_D;
    double * h_D = pDown->h_gas;
    double Eps_F = Eff;
		double * Y_D = pDown -> Y;
    double Ksi = V_In * dt * 0.1 / M_gas;
    double Old = 1. / ( 1. + Ksi );
    double New = Ksi * Old;
		for ( int i = 0; i < LAST_GAS; i++ )
			{
			if ( IsComp[i] )
        {
				Y[i] = Old * Y[i] + New * ( Eps_F * Eps_In.Y_W[i] + Eps_D * Y_D[i] );
				h_gas[i] = Old * h_gas[i] + New * ( Eps_F * Eps_In.h_Gas[i] + Eps_D * h_D[i] );
        Ys += Y[i];
        hs += Y[i] * h_gas[i];
        }
			}
    }
  else
    {
    double Ksi = V_In * dt * 0.1 / M_gas;
    double Old = 1. / ( 1. + Ksi );
    double New = Ksi * Old;
		for ( int i = 0; i < LAST_GAS; i++ )
			{
			if ( IsComp[i] )
        {
				Y[i] = Old * Y[i] + New * Eps_In.Y_W[i];
				h_gas[i] = Old * h_gas[i] + New * Eps_In.h_Gas[i];
        Ys += Y[i];
        hs += Y[i] * h_gas[i];
        }
			}
//    memmove ( Y, Eps_In.Y_W, CMOL_GAS );
//	  memmove ( h_gas, Eps_In.h_Gas, CMOL_GAS );
		}
  if ( fabs ( Ys - 1. ) > 1e-4 )
    {
    double N = 1. / Ys;
	  for ( int i = 0; i < LAST_GAS; i++ )
		  {
		  if ( IsComp[i] )
        {
			  Y[i] *= N;
			  h_gas[i] *= N;
        }
		  }
    hs *= N;
    }
  h_gas_mix = hs;
  Vout = 0.95 * Vout + 0.05 * V_In;
  if ( pDown )
	  V_Cond = pDown->Vout - Vout;
  else
    V_Cond = 0.;
//
  double h_liq_mix = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
	  {
		if ( IsComp[n] )
		  h_liq_mix += X[n] * h_liq[n];
    }
  double D = H_Summ_In - Vout * h_gas_mix - L_In * h_liq_mix;
  double P = D / H_Summ_In;
  KKK();
  }

