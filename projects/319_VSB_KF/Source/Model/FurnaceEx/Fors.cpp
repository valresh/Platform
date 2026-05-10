#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"
#include "SysDataTypes.h"

CComp * CRadFors::pComp_Progar = NULL;

CRadFors::CRadFors( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
  {
  ObjName = Name;
  Model = "RadFors";
  double T_Air = ( 15. + TK ) * 0.01;
  H_Smoke = 0.;
  Flow_Smoke = 0.;
  T0 = 15.;
  T = 15.; 
  Flow_gas = 0.;
  Flow_air = 0.; 
  Q = 0.;
  T_in = 15.;
  H_Smoke_0 = 0.;
  Q_fire = 0.;
  IsProgar = false;
  F_Progar = 0.;
  T_max = 3000.;
  IsCondensat = false;
  kCondensat = 1.;
  GasAsMasut = false;
  //
  Smoke.Set( Smoke_Cmol, IsComp );
  Topl.Set( Topl_Cmol, IsComp );
  KPD = 70.;
  To_kg_mazut = 0.;
  pComp_Gas = NULL;
  pComp_Mazut = NULL;
  }

void CRadFors::Init( )
  {
  }

void CRadFors::CalcT ( double dt )
  {
  SET_BP
  if ( pPech->Calc_T_Wall )
    {
// Расчет через теплопередачу на стены
    double H0 = Smoke.H ( T0 );
    double dH = Flow_Smoke * ( H0 - H_Smoke ) - Q_wall;
    if ( pPech->Tau_fire < 1. )
      pPech->Tau_fire = 1.;
    double Ksi = dt * 60. / pPech->Tau_fire;
    double dT = LimdX ( pPech->V_fors * dH * Ksi, 5. );
    T += dT;
    H_Smoke += 0.05 * ( Smoke.H ( T ) - H_Smoke );
    Q += 0.05 * ( dH - Q );
    return;
    }
// Макс энергия
  double H0 = Smoke.H ( T0 );
  H_Smoke_0 = H0;
  double Hair = Smoke.H ( T_air );
  double Alfa = ( 100. - KPD ) * 0.01;
  if ( pPech->Use_KPD_F )
    {
    double k_Smoke = Flow_Smoke / F_Smoke_Nom_Mol;
    if ( k_Smoke < 1. )
      {
      if ( k_Smoke <= 0. )
        k_Smoke = 0.;
      else
        k_Smoke = pow ( k_Smoke, pPech->Pow_Smoke );
      double Alfa1 = ( 1. - Alfa ) * k_Smoke;
      Alfa = 1. - Alfa1;
      }
    }
  double H_out = H0 - Alfa * ( H0 - Hair ); 
//  double Ksi = dt * 60. / pPech->Tau_fire;
//  double Tf = Smoke.TH ( H_out, T );
  // kH * H + kT * T + C = 0;
  //  T = ( T + Ksi * Tf ) / ( 1. + Ksi );
  T = TH ( Smoke.IsComp, Smoke.Cmol, dt * Flow_Smoke, pPech->Cp_Pereval, 
    -pPech->Cp_Pereval * T - dt * Flow_Smoke * H_out, T );
  H_Smoke = Smoke.H ( T );
  Q += 0.05 * ( Flow_Smoke * ( H0 - H_out ) - Q );
  Q_wall = Q;
  }

void CRadFors::dH_Cp( bool IsComp[], double Cmol[], double T_C, double & H, double & Cp, bool CalcCp )
  {
  double h = 0.;
  double m = 0.;
  double cp = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double c = Cmol[n];
      h += c * pGas[n] -> H_IG ( T_C );
      m += c * pGas[n] ->Mw;
      if ( CalcCp )
        cp += c * pGas[n] -> C_IG ( T_C );
      }
    }
  H = h;
  if ( CalcCp )
    Cp = cp;
  }

// kH * H + kT * T + C = 0;
double CRadFors::TH (  bool IsComp[], double Cmol[], double kH, double kT, double C, double T_0 )
  {
  double T = T_0;
  double dT = 1e10;
  double h, cp;
  int kIter = 0;
  while ( fabs ( dT ) > 1e-5 && kIter++ < 20 )
    {
    dH_Cp( IsComp, Cmol, T, h, cp, true );
    double f = kH * h + kT * T + C;
    double df = kH * cp + kT;
    dT = -f / df;
    if ( dT > 10. )
      dT = 10.;
    T += dT;
    if ( T < T_min )
      {
      T = T_min;
      break;
      }
    if ( T > T_max )
      {
      T = T_max;
      break;
      }
    }
  return T;
  }

void CRadFors::NoFire( )
  {
  T = T0;
  Flow_Smoke = Flow_gas + Flow_air;
  memset(Smoke.Cmol, 0, sizeof(double) * K_GAS);
  Smoke.IsComp[G_O2] = true;
  Smoke.IsComp[G_N2] = true;
  Smoke.Cmol[G_O2] = 0.21;
  Smoke.Cmol[G_N2] = 0.79;
  Q_fire = 0.;
  H_Smoke = Flow_Smoke * Smoke.H ( T );
  }

void CRadFors::CalcQfire( )
  {
  CLEAR(Topl_Cmol)
  CLEAR(IsComp)
  double F_air = Flow_air;
  if ( F_air < 0.1 )
    F_air = 0.1;
  Topl.Cmol[G_O2] = F_air * 0.21;
  Topl.Cmol[G_N2] = F_air * 0.79;
  Topl.IsComp[G_O2] = true;
  Topl.IsComp[G_N2] = true;
  Topl.Mazut_liq = true;
  Flow_gas_eff = Flow_gas;
  if ( IsCondensat )
    Flow_gas_eff *= kCondensat;
//
  if ( Flow_mazut > 0. && pComp_Mazut )
    {
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( pComp_Mazut->IsComp[n])
        {
        IsComp[n] = true;
        Topl.Cmol[n] += Flow_mazut * pComp_Mazut->Cmol[n];
        }
      }
    }
  if ( Flow_gas_eff > 0. && pComp_Gas )
    {
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( pComp_Gas->IsComp[n])
        {
        IsComp[n] = true;
        Topl.Cmol[n] += Flow_gas_eff * pComp_Gas->Cmol[n];
        }
      }
    }
  if ( Flow_Smoke_in > 0. && pComp_Smoke_in )
    {
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( pComp_Smoke_in->IsComp[n])
        {
        IsComp[n] = true;
        Topl.Cmol[n] += Flow_Smoke_in * pComp_Smoke_in->Cmol[n];
        }
      }
    }
  if ( Flow_Add > 0. && pComp_Add )
    {
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( pComp_Add->IsComp[n])
        {
        IsComp[n] = true;
        Topl.Cmol[n] += Flow_Add * pComp_Add->Cmol[n];
        }
      }
    }
  if ( Flow_steam > 0. )
    {
    IsComp[G_H2O] = true;
    Topl.Cmol[G_H2O] += Flow_steam;
    }
  if ( F_Progar > 0. && pComp_Progar )
    {
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( pComp_Progar->IsComp[n])
        {
        IsComp[n] = true;
        Topl.Cmol[n] += F_Progar * pComp_Progar->Cmol[n];
        }
      }
    }
//
  Topl.Cmol[G_H2O] += Flow_steam + Flow_Steam_Rad;
  IsComp[G_H2O] = true;
  memmove( Smoke_Cmol, Topl_Cmol, sizeof ( Smoke_Cmol ));
//
  if ( !IsFire )
    {
    double F = 0.;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( IsComp[n])
        {
        F += Topl.Cmol[n];
        }
      }
    Flow_Smoke = Flow_in = F;
    Smoke.Norm();
    T = T0 = T_in;
    Q = 0.;
    Q_fire = 0.;
    return;
    }
//
  double Flow_C = 0.;
  double Flow_H = 0.;
  double k_C[K_GAS];
  double k_H[K_GAS];
  CLEAR(k_C)
  CLEAR(k_H)
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] && n != G_H2O && n != G_CO && n != G_CO2  && n != G_H2S )
      {
      k_C[n] = pGas[n]->k_C;
      k_H[n] = pGas[n]->k_H;
      Flow_C += Topl.Cmol[n] * pGas[n]->k_C;
      Flow_H += Topl.Cmol[n] * pGas[n]->k_H;
      }
    }
  double Flow_O2 = Topl.Cmol[G_O2];
  double Flow_Need_O2 = 0.25 * Flow_H + Flow_C;
  double F_H2S = 0.;
  if ( IsComp[G_H2S] )
    F_H2S = Topl.Cmol[G_H2S];
  Flow_Need_O2 += 1.5 * F_H2S;
  Q = 0.;
  if ( Flow_O2 > Flow_Need_O2 || Flow_Need_O2 < 0.01 )
    k_Fire = 1.;
  else
    k_Fire = Flow_O2 / Flow_Need_O2;
  if ( IsProgar && IsFire )
    {
    if ( k_Fire < 1. )
      {
// Добавляем воздуха
      double Add_O2 = Flow_Need_O2 - Flow_O2;
      Smoke_Cmol[G_O2] += Add_O2;
const double k_N2 = 0.79 / 0.21;
      Smoke_Cmol[G_N2] += Add_O2 * k_N2;
      k_Fire = 1.;
      }
    }
//
  double k_NoFire = 1. - k_Fire;
  double Flow_H2O = 0.;
  double Flow_CO2 = 0.;
  double Use_O2 = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n])
      {
      double F = Smoke_Cmol[n] * k_Fire;
      double F_C = pGas[n]->k_C * F;
      double F_H = pGas[n]->k_H * F;
      Q += F_C * pPech -> Q_C;
      Q += F_H * pPech -> Q_H2;
      if ( F_C != 0. || F_H != 0. )
        Smoke_Cmol[n] *= k_NoFire;
      Flow_H2O += 0.5 * F_H;
      Flow_CO2 += F_C;
      Use_O2 += 0.25 * F_H + F_C;
      }
    }
  F_H2S *= k_Fire;
  Q += F_H2S * pPech -> Q_H2S;
  Smoke_Cmol[G_H2S] -= F_H2S;
  if ( Smoke_Cmol[G_H2S] < 0. )
    Smoke_Cmol[G_H2S] = 0.;
  IsComp[pPech -> G_SO2] = true;
  Smoke_Cmol[pPech -> G_SO2] += F_H2S;
  IsComp[G_H2O] = true;
  Smoke_Cmol[G_H2O] += Flow_H2O;
  Smoke_Cmol[G_H2O] += F_H2S;
  IsComp[G_H2O] = true;
  Smoke_Cmol[G_CO2] += Flow_CO2;
  IsComp[G_CO2] = true;
  Smoke_Cmol[G_O2] -= Use_O2;
  if ( Smoke_Cmol[G_O2] < 0. )
    Smoke_Cmol[G_O2] = 0.;
  Q_fire = Q;
  Flow_Smoke = Smoke.Norm();
//
  }

void CRadFors::CalcT0( )
  {
  SET_BP
//
  if ( !GasAsMasut )
    {
    if ( To_kg_mazut <= 0. )
      To_kg_mazut = Mw(pComp_Mazut);
	  double Mazut_kg = Flow_mazut_full * To_kg_mazut;
	  double Steam_kg = Flow_steam * Mw_H2O;
    if ( Steam_kg < 0. )
      Steam_kg = 0.;
	  double Alfa_Mazut = 1.;
	  double Steam_min = Mazut_kg * pPech->kSteam;
	  if ( Steam_kg < Steam_min && Mazut_kg > 0.1 )
		  {
		  Alfa_Mazut = Steam_kg / Steam_min; 
      if ( Alfa_Mazut > 1. )
        Alfa_Mazut = 1.;
		  }
	  Flow_mazut = Flow_mazut_full * Alfa_Mazut;
    }
  if ( Flow_air < 0. )
    Flow_air = 0.;
  if ( Flow_Steam_Rad < 0. )
    Flow_Steam_Rad = 0.;
  if ( Flow_gas < 0. )
    Flow_gas = 0.;
  if ( Flow_Add < 0. )
    Flow_Add = 0.;
  if ( Flow_mazut < 0. )
    Flow_mazut = 0.;
  if ( Flow_Smoke_in < 0. )
    Flow_Smoke_in = 0.;
  if ( Flow_steam < 0. )
    Flow_steam = 0.;
	double H_Topl = 
	            Flow_gas * h_gas + 
							Flow_mazut * h_mazut +
              Flow_Smoke_in * h_Smoke_in +
              Flow_Add * h_Add +
							Flow_steam * h_steam +
			        Flow_air * h_air +
							Flow_Steam_Rad * h_Steam_Rad;
	if ( IsProgar )
		H_Topl += F_Progar * h_Progar;
//  pPech->b_CalcT_In = true;
//	if( pPech->b_CalcT_In )
	  {
		Flow_in = 
			Flow_gas + 
			Flow_mazut +
      Flow_Smoke_in +
			Flow_steam +
			Flow_air +
			Flow_Steam_Rad;
		if ( IsProgar )
			Flow_in += F_Progar;
	  }
//
  CalcQfire( );
  Flow_in = Topl.Norm();
  if ( Flow_in > 0.01 )
    T_in = Topl.TH( H_Topl/Flow_in, T_in );
  else
    T_in = T_air;
//
  if ( !IsFire )
    {
    H_Smoke = H_Smoke_0 = H_Topl/Flow_in;
    T = T0 = T_in;
    return;
    }
  //
//
  SET_BP
//
// Температура
	double C_O2 = Smoke.Cmol[G_O2] / 0.05;
	if ( C_O2 > 1. )
		C_O2 = 1.;
	double k_CO = pPech->k_CO_min * C_O2 * C_O2 + pPech->k_CO_max * ( 1. - C_O2 ) * ( 1. - C_O2 );
	double d_CO = Smoke.Cmol[G_CO2] * k_CO;
	Smoke.Cmol[G_CO2] -=	d_CO;
	Smoke.Cmol[G_CO] =	d_CO;
  Smoke.IsComp[G_CO] = true;
//
	Smoke.Mazut_liq = false;
	H_Smoke_0 = Smoke.H( T_in );
  if ( Flow_Smoke < 0.1 )
    {
		T0 = T_in;//Smoke.T( Flow_Smoke, H_Smoke_0, T0 );
		Q = 0.;
		Q_fire = 0.;
    return ;
    }
// Горение
  H_Smoke_0 += Q_fire / Flow_Smoke;
  Smoke.T_max = T_max;
	T0 = Smoke.TH ( H_Smoke_0, T0 );
  if ( T0 < T_air )
    T0 = T_air;
  FINITE(T0)
  }

const double G_gas_min = 0.1;

void CRadFors::Step( double dt )
  {
  if ( Flow_gas + Flow_air + Flow_mazut_full + Flow_Smoke_in < G_gas_min )
		{
    NoFire();
    return ;
		}
//
  CalcT0();
  CalcT( dt );
  }

int CRadFors::SaveState( )
	{
	S_CLASS("RadFors_W",CRadFors_W)
	return 0;
  }

int CRadFors::RestoreState( char * StrName )
	{
  IGNORE
	R_CLASS("RadFors_W",CRadFors_W)
	return 1;
  }

#include "IO_Parms.h"
int CRadFors::GetParams( char * )
  {
  TAB("Режим",1)
  PARM(IsFire, "#Горит" )
  PARM(T0, "#T0" )
  PARM(T, "#T" )
  PARM(Flow_gas, "#Газ" );
  PARM(Flow_mazut_full, "#Мазут" );
  PARM(Flow_air, "#Воздух" );
//  PARM(K_Fire, "Теплоотдача" )
//  PARM(T_eff, "#T_eff" )
  PARM(Smoke.Cmol[G_O2], "#O2" );
  ETAB
	return 0;
  }
