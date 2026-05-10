#include "stdafx.h"
#include "Collector.h"
#include "Err.h"
#include "CommProc.h"


double dH_io = 0.;
void CCollector::Calc( double dt )
  {
	SET_BP BreakPoint;
	double Cmol_in[K_GAS];
	if ( !Use_Comp )
	  {
		memmove ( Cmol_in, Cmol, LAST_GAS * 8 );
		To_kg = ::Mw( IsComp, Cmol );
	  }
	double Flow_summ_out = 0.0;
	double Flow_summ = 0.;
	double Flow_summ_no_io = 0.;
	double Flow_in_kg = 0.;
	double Flow_out_kg = 0.;
	if ( !_finite( h_Flow ))
		h_Flow = 0.;
	double H = 0.;
  double dH = 0.;
	if (!_finite(T))
		T = T_air;
	double F_m = 0.;
//  if ( strstr (ObjName, "ОВП") && Pio < 0. )
//    KKK();
  dH_io = 0.;
	for ( int n = 0; n < kIO; n++ )
	  {
		if ( pFlow[n] == NULL )
			continue;
		double F = pFlow[n]->Flow_mol;
		double aF = fabs ( F );
		if ( aF > F_m )
			F_m = aF;
		if ( F > 0 )
		  {
			double Mw = pFlow[n]->To_kg;
			Flow_kg[n] = F * Mw;
			Flow_in_kg += F * Mw;
		  }
		else
		  {
			Flow_kg[n] = F * To_kg;
			Flow_out_kg += F * To_kg;
		  }
		if ( n != Summ_In - 1 && n != Summ_Out - 1 )
			Flow_summ_no_io += F;
    else
      KKK();
		Flow_summ += F;
		if ( F < 0. )
		  {
			// Выходит
			pFlow[n]->ID_COMP = ID_Comp;
      CComp * pC = Get_Comp ( ID_Comp );
			pFlow[n]->To_kg = To_kg;
			pFlow[n]->h_Flow = h_Flow;
			pFlow[n]->T = T;
      dH_io += F * h_Flow;
//      double TT = Calc_H( &State ) - h_Flow; 
			continue;
		  }
		else
		  {
			// Входит
			CSEps * pS = pStateIn[n];
			if ( kStep % 4 == 0 )
			  {
				if ( !Use_Tz )
					pS->PH( pFlow[n] );
				else
					pS->T = T;
			  }
      double dM = F * dt;
		  Flow_summ_out += F;
			H += pFlow[n]->h_Flow * F;
      dH_io += F * pFlow[n]->h_Flow;
      double dHn = Calc_dH( pS );
      dH += F * dHn;
//      double TT = Calc_H( pS ) - pFlow[n]->h_Flow; 
//      KKK();
		  } 
	  }
//  _dH_io = dH_io;
	Flow_Ctrl = F_m;
  if ( F_m < 1. )
    F_m = 1.;
// Составы
  if ( !Use_Comp )
    {
 //   double N = dt * 3600 / ( F_m * Tau_C );
    for ( int n = 0; n < kIO; n++ )
      {
      if ( pFlow[n] == NULL )
        continue;
      double F = pFlow[n]->Flow_mol;
      if ( F < 0. )
        continue;
      CSEps * pS = pStateIn[n];
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( pS->IsComp[c] )
          {
          if ( !IsComp[c] )
            {
            State.h_Gas[c] = 0.;
            State.h_Liq[c] = 0.;
            }
          IsComp[c] = true;
          Cmol_in[c] += F * pS->Z_W[c];
          }
        }
      }
    }
	//
	_F_in = Flow_in_kg;
	_dF_in = Flow_in_kg + Flow_out_kg;
	//  _F_out = Flow_out;
	//  _dF = _F_in + _F_out;
	if ( Use_Comp )
	  {
		pComp->Set( pCompZ );
		Use_Tz = true;
		memmove ( IsComp, pComp->IsComp, sizeof ( IsComp ));
		memmove ( Cmol, pComp->Cmol, sizeof ( Cmol ));
 //   pComp->Set( IsComp, Cmol );
	  }
	else
	  {
		Norm ( IsComp, Cmol_in );
		memmove ( Cmol, Cmol_in, LAST_GAS * 8 );
		pComp->Set( IsComp, Cmol );
	  }
	///////////////////////////////////////////////////////
	Flow_io += k_SummIn * ( -Flow_summ_no_io - Flow_io );
  if ( strstr ( ObjName, "Коллектор ОВП" ))
    KKK();
	//double Fio = -Flow_summ_no_io;
	//Flow_io += kFlow * ( Fio - Flow_io );
	//Flow_iter = 0.95 * Flow_iter + 0.05 * ( Flow_io + Oi * P );
	///////////////////////////////////////////////////////
	if ( Use_Pz )
    {
		P = Pz + 1.;
    dP_io = Pio - P;
    }
	else
	  {
		if ( UseOut && ( Summ_In > 0 || Summ_Out > 0 ))
		  {
      double Ksi = 1. / kPress;
      dP_io = Pio - P;
			double dP = LimdX( Ksi * dP_io, 0.1 );
			P += 0.5 * ( dP + dP_old );
			dP_old = dP;
		  }
		else
		  {
      double SummF = 0.;
      for ( int n = 0; n < kIO; n++ )
        {
        double F = pFlow[n]->Flow_mol;
        FlowSumm[n] = -F;
        SummF += F;
        }
      double Ksi = dt / kPress;
      double dP = LimdX( Ksi * SummF, 0.1 );
      P += 0.5 * ( dP + dP_old );
      dP_old = dP;
      for ( int n = 0; n < kIO; n++ )
        {
        FlowSumm[n] += SummF;
        }
		  }
		if ( P < P_min )
			P = P_min;
		if ( P > P_max )
			P = P_max;
	  }
	_P = P - 1.;
	if ( Use_Tz )
	  {
		T = Tz;
		State.PT( P, T, pComp );
		h_Flow = State.h;
	  }
	else
	  {
    if ( 0 )
      {
      if ( Flow_summ_out > 1. )
        h_Flow = H / Flow_summ_out;
      State.PH( P, h_Flow, pComp);
      T = State.T;
      Set_h_summ( &State, h_summ );
      }
    else
      {
      T += LimdX ( dH * dt / Cp, 1. );
      State.PT( P, T, IsComp, Cmol );
      h_Flow = State.h;
//      double h_Test = H / Flow_summ_out;
//      double TT = h_Flow - h_Test;
      Set_h_summ( &State, h_summ );
//      double Ht = Calc_H( &State ) - h_Flow;
      }
		To_kg = ::Mw ( pComp );
		if ( State.E > 0.2 )
			To_m3 = M3;
		else
			To_m3 = To_m3_liq ( T, pComp );
	  }
  if ( T < T_min )
    T = T_min;
  if ( T > T_max )
    T = T_max;
  if ( _P + 1. < P_min )
    {
    P = P_min;
    _P = P - 1.;
    }
  if ( _P + 1 > P_max )
    {
    P = P_max;
    _P = P - 1.;
    }
	_T = T;
  }

double CCollector::Calc_dH( CSEps * pS )
  {
  double hs_summ[K_GAS];
  Set_h_summ( pS, hs_summ );
  bool * _IsComp = pS->IsComp;
  double dH = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !_IsComp[c] )
      continue;
    dH += pS->Z_W[c] * ( hs_summ[c] - h_summ[c] );
    }
  return dH;
  }

double CCollector::Calc_H( CSEps * pS )
  {
  double hs_summ[K_GAS];
  Set_h_summ( pS, hs_summ );
  bool * _IsComp = pS->IsComp;
  double H = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !_IsComp[c] )
      continue;
    H += pS->Z_W[c] * hs_summ[c];
    }
  return H;
  }

void CCollector::Set_h_summ( CSEps * pS, double * h_summ )
  {
  bool _IsComp[K_GAS];
  double X[K_GAS], Y[K_GAS], h_Liq[K_GAS], h_Gas[K_GAS];
  pS->SetCompData ( _IsComp, X, Y, h_Liq, h_Gas );
  double * Z = pS->Z_W;
  double h_gas = 0.;
  double h_liq = 0.;
  double h_s = 0.;
  double E = pS->E;
  double E1 = 1. - E;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !_IsComp[c] || Z[c] < 1e-8)
      {
      h_summ[c] = 0.;
      continue;
      }
    double h = 0.;
    if ( E > 0. )
      {
      double hy = Y[c] * h_Gas[c];
      h = E * hy / Z[c];
      h_gas += hy;
      }
    if ( E1 > 0. )
      {
      double hx = X[c] * h_Liq[c];
      h_liq += hx;
      h += E1 * hx / Z[c];
      }
    h_summ[c] = h;
    h_s += pS->Z_W[c] * h;
    }
  h_eps = E * h_gas + E1 * h_liq;
  }
