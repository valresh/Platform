#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol.h"


CVol_P::CVol_P()
  {
  memset ( this, 0, sizeof ( *this ));
  Accel_Vol = 10.;
  }

CVol_W::CVol_W()
  {
  memset ( this, 0, sizeof ( *this ));
  }

CVol::CVol( char * _Name, IBaseModel * _pMain ) : CStage ( _Name, _pMain )
, StateSrc( "Источники", this )
  {
  pComp = NULL;
  ID_Comp = 0;
  dMdt_p = dMdt_m = 0.;
  Disbalance = 0.;
  Lev_Cub = 0.;
  Cp_liq = 500.;
  _Flow_Summ_In = 0.;
  AlfaCond = 10000.;
  AlfaPz = 1.05;
  }

#include "IO_Parms.h"

int CVol::GetParams( char *  )
  {
//  TAB("Объем",1)
  PARM( Work, "#Объем заполняется" )
  PARM( Overflow, "#Переполнение" )
  PARM( nStartVol, "#Заполнено снизу до" )
  PARM( T_liq, "#Температура жидкости" )
  PARM( M_liq, "#Масса жидкости" )
  PARM( Ro, "#Плотность жидкости" )
  PARM( P, "#Давление(а)" )
  PARM( Pz, "#Давление насыщенных паров (а)" )
  PARM( P_Pz, "#P/Pz" )
  PARM( Vout, "#Поток испарения" )
  PARM( h_sloj, "#Высота слоя жидкости" )
  PARM( Lev_Cub, "#Уровень, относительно куба" )
  PARM( dMdt_p, "#dMdt налив" )
  PARM( dMdt_m, "#dMdt слив" )
  PARM( _Flow_Summ_In, "#Поступление в колонну" )
  PARM( Disbalance, "#Дисбаланс" )
  PARM( Accel_Vol, "Ускорение при расчете переполнения" )
  PARM( AlfaCond, "Скорость испарения" )
  PARM( AlfaPz, "Снижение Pz" )
  PARM( Cp_liq, "Теплоемкость жидкости" )
  return 0;
  }

int CVol::SaveState( )
  {
  CStage::SaveState( );
  S_CLASS("Vol",CVol_W)
  return 0;
  }

int CVol::RestoreState( char * StrName )
  {
  R_CLASS( "Vol", CVol_W )
  int Res = CStage::RestoreState( StrName );
  return 0;
  }


void CVol::InitCopy ( int nStart )
  {
  pCol->dt_Vol = 10.;
  Work = true;
  nStartVol = nStart;
  CStage * pGas = pCol->pStage[nStart];
  CStage * pLiq = pCol->pStage[nStart+1];
  pGas->FullStage = true;
  pLiq->FullStage = true;
////
  P = pGas->P;
  T_gas = pGas->T_gas;
  T_liq = pLiq->T_liq;
  Pz = pLiq->Pz;
  M_gas = pGas->M_gas;
  M_liq = pLiq->M_liq;
  ////
  Vout = pGas->Vout;
  Lin = pGas->Lin;
  To_m3 = pLiq->To_m3;
  V_Cond = pCol->pStage[1]->Vout;
//
  memcpy( X, pLiq->X, sizeof ( X ));
  memcpy( Y, pGas->Y, sizeof ( Y ));
  CLEAR(P_z)
  CLEAR(Vc_Comp)
//  V_Cond = 0;
  //////////////////////////////////////////////////////
  memcpy( h_gas, pGas->h_gas, sizeof ( h_gas ));
  memcpy( h_liq, pGas->h_liq, sizeof ( h_liq ));
  h_gas_mix = pGas->h_gas_mix;
  h_liq_mix = pLiq->h_liq_mix;
  //////////////////////////////////////////////////////
  memcpy( Gamma, pGas->Gamma, sizeof ( Gamma ));
  }


void CVol::FeedFlash( double dt )
  {
  dt *= Accel_Vol;
  if ( StopSrc )
    {
    BREAK
    if ( StopSrc > 0 )
      StopSrc--;
    }
  H_Summ_In = 0.;
  dH_liq_Summ = 0.;
  Flow_Summ_In = 0.;
  CLEAR(X_In)
  double dh_In[K_GAS];
  CLEAR(dh_In)
///////////////////////////////////////////////////
// Источники
  for ( int nS = nStartVol; nS <= pCol->nCub; nS++ )
    {
    CStage * pS = pCol->pStage[nS];
    for ( int s = 0; s < pS->kSrc; s++ )
      {
      CFlow * pF = pS->Src[s]->pFlow;
      if ( pF == NULL )
        continue;
      double F = pF -> Flow_mol;
      if ( F <= 0. )
        continue;
      CComp * pC = pF->Get_Comp();
      if ( pC == NULL )
        continue;
      StateSrc.PH( pF, pF->T );
      bool IsCompF[K_GAS];
      double XF[K_GAS];
      double YF[K_GAS];
      double h_gas_F[K_GAS];
      double h_liq_F[K_GAS];
      StateSrc.SetCompData ( IsCompF, XF, YF, h_liq_F, h_gas_F );
      Flow_Summ_In += F;
      double h = pF->h_Flow;
      H_Summ_In += F * h;
      for ( int i = 0; i < LAST_GAS; i++ )
        {
        if ( IsCompF[i] )
          {
          IsComp[i] = true;
          X_In[i] += F * ( StateSrc.E * YF[i] + ( 1. - StateSrc.E ) * XF[i] );
          double dh_str = StateSrc.E * YF[i] * ( h_gas_F[i] - h_liq[i] ) + ( 1. - StateSrc.E ) * XF[i] * ( h_liq_F[i] - h_liq[i] );
          dh_In[i] += F * dh_str;
          }
        }
      }
    }
  ///////////////////////////////////////////////////
  if ( StopSrc )
    BREAK
  // Сверху
  if ( pUp && pUp->Lout > 0. )
    {
    double F = pUp->Lout;
    dH_liq_Summ += F * pUp->dH_liq;
    double * uX = pUp -> X;
    double * uh_Liq = pUp -> h_liq;
    Flow_Summ_In += F;
    for ( int i = 0; i < LAST_GAS; i++ )
      {
      if ( IsComp[i] )
        {
        X_In[i] += F * uX[i];
        H_Summ_In += F * uX[i] * uh_Liq[i];
        double dh_str = uX[i] * ( uh_Liq[i] - h_liq[i] );
        dh_In[i] += F * dh_str;
        }
      }
    }
  if ( StopSrc )
    BREAK
  ///////////////////////////////////////////////////
  if ( Flow_Summ_In < 0.01 )
    {
    // Нет подачи
    L_In = 0.;
    V_Cond = 0.;
    Vout = 0.;
    CLEAR(X_In)
    CLEAR(Y_In)
    return;
    }
  ///////////////////////////////////////////////////
  if ( StopSrc )
    BREAK
///
//  if ( pCalcChem )
//    (*pCalcChem) ( dt, this, pCalcChemParam );
  for ( int i = 0; i < LAST_GAS; i++ )
    {
    if ( IsComp[i] )
      X[i] = M_liq * X[i] + dt * X_In[i];
    }
  double F = 0.;
  for ( int i = 0; i < LAST_GAS; i++ )
    {
    if ( IsComp[i] )
      F += X_In[i];
    }
  if ( !Overflow )
    {
    double M_old = M_liq;
    M_liq = Norm ( IsComp, X );
    dMdt_p = ( M_liq - M_old ) / dt;
    }
  else
    {
    Norm ( IsComp, X );
    dMdt_p = 0.;
    }
  pComp->Set( IsComp, X );
  _Flow_Summ_In = Flow_Summ_In;
/////////// нагрев
  double dH_s = 0.;
  for ( int i = 0; i < LAST_GAS; i++ )
    {
    if ( IsComp[i] )
      {
      dH_s += dh_In[i];
      }
    }
  double Cp = M_liq * Cp_liq + Cp_korp;
  double dT = dH_s * dt / Cp;
  T_liq += dT;
  T_gas = T_liq;
//////////////////////////////////////////////////////////
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( !IsComp[n] )
      continue;
    char * Name = pGas[n]->Name;
    Eps_In.P_sat[n] = Henry[n] * Gamma[n] * pGas[n]->Pz ( T_liq );
    Y[n] = X[n] * Eps_In.P_sat[n];
    h_liq[n] = pGas[n]->H_l( T_liq );
    h_gas[n] = pGas[n]->H_IG( T_liq );
    }
  Pz = Norm( IsComp, Y );
//
  double h_l = 0.;
  double h_g = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( !IsComp[n] )
      continue;
    h_l += X[n] * h_liq[n];
    h_g += Y[n] * h_gas[n];
    }
  h_liq_mix = h_l;
  h_gas_mix = h_g;
  if ( Eps_In.WaterCol )
    P_Pz = P / Pz;
  else
    P_Pz = P * ( 1. - Y[G_H2O] ) / Pz;
//////////////////////////////////////////////////////////
  if ( !Overflow )
    {
    Vout += 0.01 * ( AlfaCond * ( AlfaPz * Pz - P ) - Vout );
    if ( Vout < 0. )
      Vout = 0.;
  //////////////////////////////
    double dM = dt * Vout;
    double dH = 0.;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( !IsComp[n] )
        continue;
      X[n] = M_liq * X[n] - dM * Y[n];
      if ( X[n] < 0. )
        X[n] = 0.;
      dH += dM * Y[n] * ( h_liq[n] - h_gas[n] );
      }
    M_liq = Norm( IsComp, X );
    dT = dH / Cp;
    T_liq += dT;
    T_gas = T_liq;
    }
  }

void CVol::CalcLiq( double dt )
  {
  dt *= Accel_Vol;
  if ( StopLiq )
    {
    BREAK
    if ( StopLiq > 0 )
      StopLiq--;
    KKK();
    }
  double M_old = M_liq;
  double Flow_Summ_Out = 0.;
  int First = nStartVol;
  if ( First == 1 )
    First = 2;
  for ( int nS = First; nS <= pCol->nCub; nS++ )
    {
    CStage * pS = pCol->pStage[nS];
	  for ( int o = 0; o < pS->kSrc; o++ )
		  {
      if ( pS->Src[o]->pFlow == NULL )
        continue;
      CFlow * F_Liq_out = pS->Src[o]->pFlow;
		  double F = F_Liq_out->Flow_mol;
		  if ( F > 0. )
        continue;
      Flow_Summ_Out -= F;
		  }
    }
  if ( !Overflow )
    {
    double dM = Flow_Summ_Out * dt;
    if ( dM > 0.2 * M_liq )
      {
      pCol->Проблемы |= PROB_HI_DM;
      dM = 0.2 * M_liq;
      }
    M_liq -= dM;
    dMdt_m = ( M_liq - M_old ) / dt;
    }
  else
    {
    P = pCol->P_top;
    Disbalance = Flow_Summ_In - Flow_Summ_Out;
    if ( Disbalance > 0. )
      {
      dMdt_m = 0.;
      }
    else
      {
      Overflow = false;
      }
    }
	FINITE(M_liq)
////////////////////////////////////////////////
  Ro = Ro_liq ( T_liq, pComp );
  To_m3 = To_m3_liq ( T_liq, pComp );
  double kH = To_m3 / Real_S_Stage;
  h_sloj = M_liq * kH;
/////////////////////////////
  double h_last = h_sloj; 
//
  int nLast = -1;
  for ( int nS = pCol->nCub; nS >= nStartVol; nS-- )
    {
    if ( h_last <= pCol->pStage[nS]->Real_H_Stage )
      {
      nLast = nS;
      break;
      }
    h_last -= pCol->pStage[nS]->Real_H_Stage;
    }
  if ( Overflow )
    {
    if ( nLast >= 1 )
      {
// Переполнение кончилось
      }
    }
  else
    {
    if ( nLast < 0 )
      {
  // переполнение тарелки nStartVol или сразу нескольких
      while ( nStartVol > 1 ) // nStartVol = 0 - Колонна переполнена
        {
        nStartVol--;
        pCol->pStage[nStartVol]->FullStage = true;
        if ( nStartVol > 1 )
          {
          pCol->pStage[nStartVol-1]->pDown = this;
          pUp = pCol->pStage[nStartVol-1];
          }
        if ( h_last <= pCol->pStage[nStartVol]->Real_H_Stage )
          {
          nLast = nStartVol;
          break;
          }
        h_last -= pCol->pStage[nStartVol]->Real_H_Stage;
        }
      }
    else
      {
      if ( nLast > nStartVol )
        {
    // очередная тарелка освободилась 
        while ( nLast > nStartVol ) // nStartVol == nCub - колонна стала нормальной
          {
          pCol->pStage[nStartVol]->FullStage = false;
          pCol->pStage[nStartVol]->pDown = pCol->pStage[nStartVol+1];
          pUp = pCol->pStage[nStartVol];
          nStartVol++;
          }
        pCol->pStage[nStartVol-1]->pDown = this;
        }
      }
    if ( nStartVol == pCol->nCub )
      {
      Work = false;
      pCol->pStage[pCol->nCub]->FullStage = false;
      return;
      }
    if ( nStartVol <= 1 ) // nStartVol = 0 - Колонна переполнена
      {
      Overflow = true;
      pUp = NULL;
      nStartVol = 1;
      pCol->pStage[nStartVol]->FullStage = true;
      }
    }
///////////////////////////////////////////////
// Давление на выводах
	double dP_h;
  double h_hydro = h_last;
  for ( int nS = nStartVol; nS <= pCol->nCub; nS++ )
    {
    CStage * pS = pCol->pStage[nS];
	  for ( int o = 0; o < pS->kSrc; o++ )
		  {
      if ( pS->Src[o]->pFlow == NULL )
        continue;
      double dL = h_hydro;
      if ( pS->Src[o]->Gas )
        dL -= pS->Real_H_Stage;
      if ( dL < 0. )
        dL = 0.;
		  double Eliq;
		  if ( Level > 0.1 )
			  Eliq = 1.;
		  else
			  Eliq = 10. * Level;
	    dP_h = dL * Ro * 1e-4;
		  *(pS->Src[o]->pPress ) = dP_h + P;
		  }
    }
///////////////////////////////////////////////
  for ( int nS = nStartVol; nS <= pCol->nCub; nS++ )
    {
    CStage * pS = pCol->pStage[nS];
	  for ( int o = 0; o < pS -> kSrc; o++ )
		  {
      CFlow * F_Liq_out = pS->Src[o]->pFlow;
		  if ( F_Liq_out == NULL )
        continue;
      double F = F_Liq_out->Flow_mol;
		  if ( F >= 0. )
        continue;
  // Вывод
		  F_Liq_out->ID_COMP = ID_Comp;
		  F_Liq_out->pComp = pComp;
		  F_Liq_out -> h_Flow = h_liq_mix;
		  F_Liq_out -> T = T_liq;
		  F_Liq_out -> To_kg = ::Mw ( pComp );
		  F_Liq_out -> To_m3 = To_m3;
      FINITE(F_Liq_out -> h_Flow)
      }
    }
  }

void CVol::CalcGas( double dt )
  {
  dt *= Accel_Vol;

  }

void CVol::InitVol( )
  {
  NewComp( &ID_Comp, &pComp, "Состав %s", ObjName );
  pComp->Set( pN2 );
  }

bool CVol::FullCub( )
  {
  CStage * pS = pCol->pStage[pCol->nCub];
  double kH = pS->To_m3 / pS->Real_S_Stage;
  M_liq = pS->M_liq;
  h_sloj = pS->M_liq * kH;
  Lev_Cub = h_sloj / pS->Real_H_Stage * 100.;
  if ( h_sloj >= 1.1 * pS->Real_H_Stage )
    return true;
  return false;
  }
