#include "stdafx.h"
#include "Pipe.h"
#include "Err.h"
#include "CommProc.h"
#include "Connect.h"

#ifdef N2
extern char ErrN2[256];
#endif 


void CPipe::Calc( double dt )
	{
  SET_BP BreakPoint;

  if ( Reg_P.Value == -2. )
    Reg_P.Value = P;
  if ( Reg_T.Value == -1. )
    Reg_T.Value = T;
  double Sp = 0.;
  double Sm = 0.;
  double Fs = 0.; 
  double Fsol_mol = 0.; 
  double Fsol_kg = 0.; 
//
  int nOut = -1; 
  double FOutMax = 0.;
  for ( int n = 0; n < kF; n++ )
    {
    CFlow * pF = pFlow[n];
    if ( pF == NULL )
      return;
    pShowFlow[n]->Init( this, pF );
    h_F[n] = pF->h_Flow;
    pF -> P = P;
    if ( pF->ID_COMP == 0 )
      continue;
    pComp_in[n] = pF -> Get_Comp( );

    if ( pComp_in[n] && !_finite(pComp_in[n]->Cmol[0]))
      {
      double * pMol = &pComp_in[n]->Cmol[0];
      pComp_in[n]->Set( pN2 );
      }
    //  GetName( pComp_in[n]->ID_NAME, NULL, 0, Name, 256 );
    //  GetName( pF->ID_NAME, NULL, 0, NameF, 256 );
    //  }
    double F = pF -> Flow_mol;
    Flow_kg[n] = F * pF -> To_kg;
    if ( Type[n] == '+' )
      F = -F;
    pComp_in[n] = pF -> Get_Comp( );
    Flow_kg[n] = F * pF -> To_kg;
/*
    if ( pComp_in[n] && UseSolidFlow )
      {
      double Csol = 0.;
      double Csol_kg = 0.; 
      GetCompData( pComp_in[n], (DWORD)_Solid_, Csol, Csol_kg );
      if ( F > 0. )
        {
        Fsol_mol += F * Csol; 
        Fsol_kg += F * Csol_kg; 
        }
      }
*/
    Flow_mol[n] = F;
    Fs += F;
    if ( F > 0. )
      {
      if ( pComp_in[n] == NULL )
        continue;
      Sp +=  F;
      }
    else
      {
      if ( UseEtalon )
        {
        if ( fabs ( F ) > FOutMax )
          {
          nOut = n;
          FOutMax = fabs ( F );
          }
        }
      Sm -= F;
      }
    }
  if ( UseEtalon )
    nEtalonFlow = nOut;
  //if ( UseSolidFlow )
  //  {
  //  _F_Solid_mol = Fsol_mol;
  //  _F_Solid_kg = Fsol_kg;
  //  }
  _Flow_Summ = Fs;
  F_mol = Sp;
  if ( F_mol > MAX_FLOW )
    F_mol = MAX_FLOW;
  if ( F_mol < -MAX_FLOW )
    F_mol = -MAX_FLOW;
  if ( P < 0.0001 )
    P = 0.0001;
  if ( P > MAX_PRESS )
    P = MAX_PRESS;
//  if ( P_Calc_State_Z > 0. )
//    P_Calc_State = P_Calc_State_Z;
//  else
  P_Calc_State = P;
////////////////////
  MixFlow( dt );
  if ( Delete_N2 && pComp_out->IsComp[G_N2] )
    {
    if ( pComp_out->Cmol[G_N2] < 0.1 )
      {
      pComp_out->IsComp[G_N2] = false;
      pComp_out->Cmol[G_N2] = 0.;
      Norm ( pComp_out );
      }
    }
#ifdef N2
  if ( Show_N2 && pComp_out->IsComp[G_N2] && pComp_out->Cmol[G_N2] * 100. > Limit_N2 )
    {
    if ( !IsN2 )
      {
//??      Pause();
      lstrcpy ( ErrN2, ObjName.Str );
      }
    IsN2 = true;
    }
  else
    IsN2 = false;
#endif
  OutFlow( );
//
//  _F_mol = F_mol;
//  _F_nm3 = F_mol	* M3;
//  _F_kg = F_mol	* To_kg;
  FlowSumm.Flow_mol = F_mol;
  FlowSumm.P = P;   
  FlowSumm.T = T;   
  FlowSumm.pComp = pComp_out;
  FlowSumm.ID_COMP = ID_Comp_out;
  FlowSumm.h_Flow = h_Flow; 
  //if ( pSolid )
  //  {
  //  _F_Solid_mol = pSolid->Flow_in_mol;
  //  _F_Solid_kg = pSolid->Flow_in_kg;
  //  _F_kg = F_mol	* To_kg * ( 1. - pSolid->pSolidComp->C );
  //  }
  //else
//  Поток_кг = _F_kg;
  if ( Razgerm )
    {
	  SetState( );
    }
const double Eps = 1e-10;
  if ( ComprComp )
    {
    ComprComp = false;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( Cmol_Out[c] < Eps )
        {
        Cmol_Out[c] = 0.;
        IsComp_In[c] = false;;
        }
      }
    Norm ( IsComp_In, Cmol_Out );
    }
  }

/*
void CPipe::MixSolid( double dt )
  {
  SET_BP BreakPoint;
  double Cmol_add[CSolid_W::kSolidComp];
  CLEAR( Cmol_add );
  double FlowS = 0.;
  double Fs = 0.;
  bool WasSolid = false;
  for ( int n = 0; n < kF; n++ )
    {
    if ( pFlow[n]->pExt == NULL )
      continue;
    if ( pFlow[n]->TYPE_EXT != CFlow_Ext::Solid )
      continue;
    WasSolid = true;
    CSolid * pS = (CSolid*)pFlow[n]->pExt;
    double * Cmol_in = pS->Cmol;
    double Flow = pFlow[n]->Flow_mol;
    if ( pNodes[n].Type == '+' )
      Flow = -Flow;
    if ( Flow < 0. )
      continue;
    FlowS += Flow;
    double F = Flow * pS->C;
    Fs += F;
    for ( int c = 0; c < CSolid_W::kSolidComp; c++ )
      Cmol_add[c] += F * Cmol_in[c];
    }
  if ( WasSolid )
    {
    ASS( pSolid )
    }
  else
    return;
  double N = 0.;
  for ( int c = 0; c < CSolid_W::kSolidComp; c++ )
    N += Cmol_add[c];
  if ( N < 1e-10 )
    N = 0.;
  else
    N = 1. / N;
  for ( int c = 0; c < CSolid_W::kSolidComp; c++ )
    pSolid->Cmol[c] = Cmol_add[c] * N;
  if ( FlowS < 1e-10 )
    pSolid->C = 0.;
  else
    pSolid->C = Fs / FlowS;
  pSolid->T = T;
  }
*/

void CPipe::MixFlow( double dt )
  {
  SET_BP BreakPoint;
  CalcHeat ( dt );
  // Смесь
  double Fs = 0.;
  bool IsComp_add[K_GAS];
  double Cmol_add[K_GAS];
  memset ( IsComp_add, 0, K_GAS );
  memset ( Cmol_add, 0, CMOL_GAS );
  double * CmolS = NULL;
  double FlowS = 0.;
  //
  bool WasSolid = false;
  for ( int n = 0; n < kF; n++ )
    {
    CFlow * pF = pFlow[n];
    if ( pComp_in[n] == NULL || pF == NULL )
      continue;
    double F = pF->Flow_mol;
    if ( Type[n] == '+' )
      F = -F;
    if ( F < 0. )
      continue;
    bool * IsComp_in = pComp_in[n]->IsComp;
    double * Cmol_in = pComp_in[n]->Cmol;
    if ( !_finite( pFlow[n] -> h_Flow ))
      {
      continue;
      }
    Fs += F;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp_in[c] )
        {
        IsComp_add[c] = true;
        Cmol_add[c] += F * Cmol_in[c];
        }
      }
    }
//////////////////////////////////////////
  double c_s = 0.;
  double M_Gas = M;
  if ( Fs > 1e-8 )
    {
    double iZn = 1. / ( M_Gas + dt * Fs ); 
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp_add[c] || IsComp_In[c] )
        {
        IsComp_In[c] = true;
        double m = M_Gas * Cmol_In[c];
        Cmol_In[c] = ( m + dt * Cmol_add[c] ) * iZn;
        if ( Cmol_In[c] < 0. )
          Cmol_In[c] = 0.;
        c_s += Cmol_In[c];
        }
      }
	if(c_s == 0.0)
      {
      memmove ( IsComp_In, pN2->IsComp, sizeof ( IsComp_In ));
      memmove ( Cmol_In, pN2->Cmol, sizeof ( Cmol_In ));
      }
	else
	{
	 double N = 1. / c_s;
    double Mu = 0.;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp_In[c] )
        {
        Cmol_In[c] *= N;
        Mu += Cmol_In[c] * pGas[c]->Mw;
        }
      else
        {
        Cmol_In[c] = 0.;
        }
      }
    To_kg = Mu;
    }
  }
  FINITE(Cmol_In[3])
  /////////////////////////////////////////
  /////////////////////////////////////////
  //if ( Очищать )  //????
  //  {
  //  IsComp_In[G_Сульфолан] = false;
  //  Cmol_In[G_Сульфолан] = 0.;
  //  IsComp_In[G_Триэтиленгликоль] = false;
  //  Cmol_In[G_Триэтиленгликоль] = 0.;
  //  Norm(IsComp_In,Cmol_In);
  //  To_kg = ::Mw(IsComp_In,Cmol_In);
  //  }
  /////////////////////////////////////////
  /////////////////////////////////////////
//.............. Заданный состав 
  if ( CompZ.Set ( pComp_out ))
    {
    To_kg = ::Mw ( pComp_out );
	  if( !Reg_T.IsOn())
	    {
		  CComp comp;
		  comp.Set(IsComp_In, Cmol_In);
		  S_State.PH( P_Calc_State, h_Flow, &comp );
		  double T_in = S_State.T;
		  S_State.PT( P_Calc_State, T_in, pComp_out);
		  _h_Flow = h_Flow = S_State.h;
		  _T = T_in;
		  _Eps = S_State.E;
	    }
    }
  else 
    {
    if ( pEtalonComp && EtalonToOut > 0 )
      pComp_out->Set ( pEtalonComp->IsComp, pEtalonComp->Cmol );
    else
	    pComp_out->Set ( IsComp_In, Cmol_In );
    }
  if ( pEtalonComp && pComp_out->Cmol[1] == 0. )
    KKK();
/////////////////////////////////////////
  if ( DeleteAir )
    {
    S_State.PH( P_Calc_State, h_Flow, pComp_out );
    T = S_State.T;
    IsComp_In[G_O2] = false;
    IsComp_In[G_N2] = false;
    Cmol_In[G_O2] = 0.;
    Cmol_In[G_N2] = 0.;
    Norm ( IsComp_In, Cmol_In );
    CComp Comp;
    Comp.Set ( IsComp_In, Cmol_In );
    S_State.PT( P_Calc_State, T, &Comp );
    h_Flow = S_State.h;
    }
  // Дренажи
  V_drain_g = 0.;
  V_drain_l = 0.;
  V_drain = 0.;
///////////////////////////////////////////////////////////
  double O_max = 0.;
  kDrain = 0;
  if ( kDrain > 0 )
    {
    for ( int n = 0; n < kDrain; n++ )
      {
			if ( pDrainModel[n]->Type == CDrainage::Газ )
        V_drain_g += pDrainModel[n]->Flow_mol;
      else
        V_drain_l += pDrainModel[n]->Flow_mol;
      }
    V_drain = V_drain_l + V_drain_g;
		Flow_Drain = V_drain;
    Flow_DrainageF_m3 = Flow_Drain * To_m3;
    Flow_DrainageF_kg = Flow_Drain * To_kg;
    }
  else
    Flow_Drain = 0.;
  if ( V_drain == 0. && ( kStep % 4 != 1 ))
    {
 ///////////////////////////////////////////////////////////
    if ( !CompZ.Set ( pComp_out ))
      {
      if ( pEtalonComp && EtalonToOut > 0 )
        pComp_out->Set ( pEtalonComp->IsComp, pEtalonComp->Cmol );
      else
        pComp_out->Set ( IsComp_In, Cmol_In );
      }
    if ( pEtalonComp && pComp_out->Cmol[1] == 0. )
      KKK();
    return ;
    }
//==========================
  // Слив в дренажи
  Out_Drain = false;
  if ( V_drain > 0. )
    {
    if ( Flow_Drain > 0. )
      SetDrain( );
    else
      {
      //.............. Заданный состав 
      if ( !CompZ.Set ( pComp_out ))
        pComp_out->Set ( IsComp_In, Cmol_In );
      }
    }
  else
    {
    //.............. Заданный состав 
    if ( !CompZ.Set ( pComp_out ))
      {
      if ( pEtalonComp && EtalonToOut > 0 )
        pComp_out->Set ( pEtalonComp->IsComp, pEtalonComp->Cmol );
      else
        pComp_out->Set ( IsComp_In, Cmol_In );
      }
    }
///
  if ( pEtalonComp && pComp_out->Cmol[1] == 0. )
    KKK();
  }

void CPipe::OutFlow( )
	{
	if ( h_Flow == 0. )
		{
		pComp_out->Set ( pN2 );
		S_State.PT( P_Calc_State, T_air, pComp_out );
		h_Flow = S_State.h;
		}
  _h_Flow = h_Flow;
  CComp * pGas = NULL;
  CComp * pLiq = NULL;
  int ID_Gas = 0;
  int ID_Liq = 0;
	for ( int n = 0; n < kF; n++ )
		{
		CFlow * pF = pFlow[n];
    double F = pF -> Flow_mol;
		if ( Type[n] == '+' )
			F = -F;
    if ( F > 0. )
      continue;
		pF -> P = P;   // давление кг/см2
		pF -> T = T;   // температура в градусах Цельсия
    pF -> To_m3 = To_m3; // перевод в м3
    if ( Out_Drain )
      {
      pF -> To_kg = Mw(pComp_out_drain); // перевод в кг
      pF -> ID_COMP = ID_Comp_out_drain;     // ID состава
      pF -> pComp = pComp_out_drain;
      pF -> h_Flow = h_Out_drain; // кДж/кмоль
      }
    else
      {
      pF -> To_kg = Mw(pComp_out); // перевод в кг
      pF -> ID_COMP = ID_Comp_out;     // ID состава
      pF -> pComp = pComp_out;
      pF -> h_Flow = h_Flow; // кДж/кмоль
      }
		}
	}

void CPipe::CalcHeat ( double dt )
	{
  SET_BP BreakPoint;
  if ( !_finite ( pComp_out->Cmol[0] ))
    {
    pComp_out->Set(pN2);
	  memcpy ( IsComp_In, pComp_out->IsComp, sizeof ( IsComp_In ));
	  memcpy ( Cmol_In, pComp_out->Cmol, sizeof ( Cmol_In ));
    }
  if ( kStep % 20000 == 0 )
  {
	  Compress(  );
	  if ( TauCool > 0. )
	  	{
	  	S_Cool.T_min = T_air;
	  	S_Cool.T_max = T_max;
	  	S_Cool.PT( P_Calc_State, T_air, pComp_out );
	  	}
	  }
	if ( !_finite( h_Flow ))
		h_Flow = 0.;
// Смесь
	double Fs = 0.;
	double H_s_in = 0.;
//
	for ( int n = 0; n < kF; n++ )
		{
		if ( pComp_in[n] == NULL )
			continue;
    double F = pFlow[n] -> Flow_mol;
		if ( Type[n] == '+' )
			F = -F;
    if ( F < 0. )
      continue;
		if ( !_finite( pFlow[n] -> h_Flow ))
			{
      continue;
			}
		H_s_in += F * pFlow[n] -> h_Flow;
		Fs += F;
		}
	if ( h_Flow == 0. )
		{
		pComp_out->Set ( pN2 );
		S_State.PT( P_Calc_State, T_air, pComp_out );
		h_Flow = S_State.h;
		}
	if ( TauCool > 0. )
		{
		double FlowCool = M / TauCool;
		H_s_in += FlowCool * S_Cool.h;
		Fs += FlowCool;
		}
	if ( Reg_T.IsOn() || Cool || ( pEtalonComp && EtalonToOut > 0 ))
		{
// T задано
		double Tz;
		if ( Cool )
			{
			Cool = false;
			Tz = T_air;
			}
		else
			Tz = Reg_T;
    double Pz = P_Calc_State;
    if ( pEtalonComp && EtalonToOut > 0 )
      {
      Pz = pEtalonComp->P_et + 1.;
      Tz = pEtalonComp->T_et;
      }
//    EtalonToOut
		T = Tz;
		S_State.PT( Pz, Tz, pComp_out );
		_h_Flow = h_Flow = S_State.h;
    if ( h_Flow == 0. )
      _h_Flow = h_Flow = 1e-6;
		_Eps = S_State.E;
    _T = S_State.T;
		}
	else
		{
// Расчет h, T
		if ( F_mol < F_min )
			{
			To_Air(T)
			if ( kStep % 20 == 1 ) //T - T_air > 1e-3 &&
				{
				S_State.PT( P_Calc_State, T, pComp_out );
				h_Flow = S_State.h;
				}
			_T = T;
			_Eps = S_State.E;
			}
		else
			{
			if(Fs)
				h_Flow = ( M * h_Flow + dt * H_s_in ) / ( M + dt * Fs );
	    if ( CalcT || ( TempCalcT && !CompZ.Use ))
				{
				S_State.T_min = T_min;
				S_State.T_max = T_max;
				S_State.PH( P_Calc_State, h_Flow, pComp_out );
//        if ( T_heater > 0. && S_State.T < T_heater )
//          {
//          T = T_heater;
//          S_State.PT( P_Calc_State, T_heater, pComp_out );
//          h_Flow = S_State.h;
//          }
        _Eps = S_State.E;
        _T = T = S_State.T;
        _h_Flow = h_Flow;
				}
			else
				{
				_Eps = -1.;
				_T = -1.;
				}
			}
		}
  //bool PrtH = false;
  //if ( PrtH )
  //  {
  //  PrtH = false;
  //  void * F = Fopen("Pipe.csv","wt");
  //  double h = 0;
  //  for ( int n = 0; n < LAST_GAS; n++ )
  //    {
  //    if ( S_State.IsComp[n] )
  //      {
  //      Fprintf ( F, "%s;%7.1lf;%7.5lf;\n", pGas[n]->Name, S_State.h_Gas[n], S_State.Y_W[n] );
  //      h += S_State.h_Gas[n] * S_State.Y_W[n];
  //      }
  //    }
  //  Fprintf ( F, "h;%7.3lf;%7.3lf;\n", h, T );
  //  S_State.PT( P_Calc_State, 65., pComp_out );
  //  h = 0;
  //  for ( int n = 0; n < LAST_GAS; n++ )
  //    {
  //    if ( S_State.IsComp[n] )
  //      {
  //      Fprintf ( F, "%s;%7.1lf;%7.5lf;\n", pGas[n]->Name, S_State.h_Gas[n], S_State.Y_W[n] );
  //      h += S_State.h_Gas[n] * S_State.Y_W[n];
  //      }
  //    }
  //  Fprintf ( F, "h;%7.3lf;;\n", h );
  //  Fclose ( F );
  //  }
	//FINITE(h_Flow)
  if ( !_finite( h_Flow ))
    h_Flow = 0.;
  double Tst = T_std;
	if ( CalcT || TempCalcT )
		{
		if ( OnlyLiq )
			{
			To_m3 = To_m3_liq ( T_std, pComp_out );
			}
		else
			{
			if ( _Eps >= 1. )
				{
				To_m3 = M3;
				}
			else
				{
				To_m3 = _Eps * M3 + ( 1. - _Eps ) * To_m3_liq ( T_std, pComp_out );
				}
			}
		if(To_m3 == 0.0)
			To_m3 = 18.0e-3;
		_Ro = To_kg / To_m3;
		}
	else
		_Ro = -1.;
	}

static double EpsCmol = 1e-9;
void CPipe::Compress (  )
  {
  double S = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp_In[c] )
      {
      if ( !_finite( Cmol_In[c]) || Cmol_In[c] < EpsCmol )
        {
        IsComp_In[c] = false;
        Cmol_In[c] = 0.;
        }
      S += Cmol_In[c];
      }
    }
  if ( S < 1e-5 )
    {
    IsComp_In[G_N2] = true;
    Cmol_In[G_N2] = 1.;
    return;
    }
  if ( fabs ( S - 1 ) > 1e-5 )
    KKK();
  }

void CPipe::SetDrain(  )
  {
  if ( F_mol > 0. )
    {
    // 
    double F_liq = F_mol * ( 1. - S_State.E );
    double F_gas = F_mol - F_liq;
    //
    if ( V_drain < 1e-2 )
      return;
    double F_liq_drain = V_drain_l * 0.99;
    double F_gas_drain = V_drain_g * 0.99;
    //
    double F_out = F_mol - V_drain_l - V_drain_g; // Пойдет на выход
/////
    double F_gas_out = 0.;
    double F_liq_out = 0.;
    if ( F_gas_drain <= F_gas && F_liq_drain <= F_liq )
      {
      F_gas_out = F_gas - F_gas_drain;
      F_liq_out = F_liq - F_liq_drain;
      }
    else
      {
      if ( F_gas_drain > F_gas )
        {
        F_gas_out = 0.;
        F_liq_drain += F_gas_drain - F_gas;
        F_gas_drain = F_gas;
        ASS( F_liq_drain <= F_liq)
        F_liq_out = F_liq - F_liq_drain;
        }
      else
        {
        ASS( F_liq_drain > F_liq)
        F_gas_drain += F_liq_drain - F_liq;
        F_liq_drain = F_liq;
        ASS( F_gas_drain <= F_gas)
        F_liq_out = 0.;
        F_gas_out = F_gas - F_gas_drain;;
        }
      }
    if ( F_gas_out < 0. )
      F_gas_out = 0.;
    if ( F_liq_out < 0. )
      F_liq_out = 0.;
    double F_summ = F_gas_out + F_liq_out;
    // Формирование состава
    double Cmol_X[K_GAS], Cmol_Y[K_GAS];
    S_State.SetCompData ( IsComp_In, Cmol_X, Cmol_Y, NULL, NULL );
    S_State.Calc_Mw();
    Flow_DrainageF_m3 = F_gas_drain * M3;
    Flow_DrainageF_kg = F_liq_drain * S_State.Mw_liq;
    double h_Liq = S_State.h_liq;
    double h_Gas = S_State.h_gas;
    double Eps_out_1; 
    if ( F_summ > 0.01 )
      Eps_out_1 = F_liq_out / F_summ;
    else
      {
      Eps_out_1 = 1.;
      }
    Out_Drain = true;
    double Eps_out = 1. - Eps_out_1;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp_In[c] )
        Cmol_Out[c] = Cmol_X[c] * Eps_out_1 + Cmol_Y[c] * Eps_out;
      else
        Cmol_Out[c] = 0.;
      }
    h_Out_drain = h_Liq * Eps_out_1 + h_Gas * Eps_out;
    pComp_out_drain->Set ( IsComp_In, Cmol_Out );
    }
  else
    {
    Out_Drain = false;
    }
  }


void CPipe::SetState( )
  {
	if ( !Razgerm || pComp_out == NULL )
	{
    return;
	}
	//
	//Тип потока и Поток, для трубы по 3D должен появиться. 
	// Flags записывается тип потока 
	//  ( 0-нет,1-газ прозрачный (звук только),
	//  2-газ горящий(огонь), 3-пар облако,
	// 4 -жидкость прозрачная,
	//  5 - жидкость темная,  6-жидкость испаряющаяся+ облако, )
	//  , в dVal значение потока  
	//  pState -> eType = enumValueDbl;
	double F_Razgerm = Omega_Razgerm * ( P - 1. );
	double _F_razgerm_kg = F_Razgerm * To_kg;
	if ( _F_razgerm_kg > F_Razgerm_max )
		F_Razgerm_max = _F_razgerm_kg;
	double F_show = _F_razgerm_kg;
	if ( F_show < 0.02 * F_Razgerm_max )
	  {
		return;
	  }
	bool * Is = pComp_out->IsComp;  
	double * C = pComp_out->Cmol;
	//////////////////
	S_Razgerm.PH( P, h_Flow, pComp_out );
	if ( S_Razgerm.E > 0.7 )
	{
		// Газ
		if ( Is[G_H2O] && C[G_H2O] > 0.9 )
		{
			return;
		}
		if ( Is[G_CH4] && C[G_CH4] > 0.5 )
		{
			return;
		}
		return;
	}

	if ( Is[G_H2O] && C[G_H2O] > 0.9 )
	{
		return;
	}
	double C_l = 0;
	for ( int c = 0; c <= G_C10H22N; c++ )
	{
		if ( Is[c])
			C_l += C[c];
	}
	if ( C_l > 0.5 )
	{
		return;
	}
	//
	return;
}

