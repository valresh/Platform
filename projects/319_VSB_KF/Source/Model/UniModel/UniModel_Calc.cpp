#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
#include "CommProc.h"

//const double F_min = 0.1;

void CUniModel::Calc( double dt )
  {
  SET_BP BreakPoint;
  pCurrentModel->Calc ( dt );
  };

#if 0
void CUniModel::Calc( double dt )
	{
  SET_BP BreakPoint;
  if( Defect != 2)
	  Уровень = УровеньZ;
  if ( pFlow_in == NULL || pFlow_in->ID_COMP == 0 )
    return;
  if ( pFlow_in && pFlow_out )
    {
    P_In = pFlow_in->P - 1.;
    P_Out = pFlow_out->P - 1.;
    dP = P_In - P_Out;
    Flow = pFlow_in -> Flow_mol;
    if ( pFlow_in -> Flow_mol > 0. )
      {
      *pFlow_out = *pFlow_in;
      In.PH( pFlow_in );
      Температура = In.T;
      if ( Тип == 8 || Тип == 9 || Тип == 10 )
        SetOutComp( pFlow_in, pFlow_out ); 
      else
        {
        CComp * pComp = pFlow_in->Get_Comp();
        Mw = ::Mw ( pComp );
        F_kg = Flow * Mw;
        }
      }
    else
      {
      *pFlow_in = *pFlow_out;
      if ( Температура.Use())
        {
        if ( fabs ( pFlow_out->Flow_mol ) > F_min )
          In.PH( pFlow_out );
        else
          {
          In.T += 0.1 * ( T_air - In.T );
          }
        Температура = In.T;
        }
      if ( Тип == 8 || Тип == 9 || Тип == 10 )
        SetOutComp( pFlow_out, pFlow_in );
      else
        {
        CComp * pComp = pFlow_out->Get_Comp();
        Mw = ::Mw ( pComp );
        F_kg = Flow * Mw;
        }
      }
    T = In.T;
    }
  switch ( Тип )
    {
    case 0: //Дроссель
    case 3: //Уплотнения
    case 4: //Фильтр
      Resist( dt );
      T_Out = T_In;
    break;
    case 2://Нагреватель
      Heater( dt );
    break;
    case 5://Очистка
      Cleaner( dt );
    break;
    case 6://Очистка
      Luk( dt );
    break;
    case 7://Очистка
      Produvka( dt );
    break;
    case 1://Гидростат
    break;
    case 8://
      //Script( dt );
      CalcMEA( dt );
    break;
    case 10://Фильтр состава
    break;
    case 12:
      HeaterdT( dt );
    break;
    case 13:
      ElectroHeat( dt );
    break;
    }
	}

void CUniModel::Resist( double dt )
  {
  if ( Flow_nom_kg > 0. )
    {
    double F_mol = Flow_nom_kg / Mw;
    Flow_nom_m3 = F_mol * M3;
    Omega = F_mol / dP_nom * K_Rel;
    return;
    }
  if ( Flow_nom_m3 > 0. )
    {
    Omega = Flow_nom_m3 / ( M3 * dP_nom ) * K_Rel;
    return;
    }
  if ( dP_Z > 0. )
    {
    double absF = fabs ( Flow );
    Omega = 0.9 * Omega + 0.1 * absF / dP_Z;
    return;
    }
  if ( Flow_Z_kg > 0. )
    {
    double absF = fabs ( Flow );
    double F_kg = absF * Mw;
    if ( Omega * Flow_Z_kg < Omega_max * F_kg )
      Omega *= ( 0.9 + 0.1 * Flow_Z_kg / F_kg );
    else
      Omega = Omega_max;
    return;
    }
  if ( Flow_Z_m3 > 0. )
    {
    double absF = fabs ( Flow );
    double F_m3 = absF * M3;
    if ( Omega * Flow_Z_m3 < Omega_max * F_m3 )
      Omega *= ( 0.9 + 0.1 * Flow_Z_m3 / F_m3 );
    else
      Omega = Omega_max;
    return;
    }
  }

void CUniModel::HeaterdT( double dt )
  {
  //
  double Alfa = ( T - T_start_heat ) / ( T_max_heat - T_start_heat );
  if ( Alfa < 0. )
    Alfa = 0.;
  if ( Alfa > 1. )
    Alfa = 1.;
  if ( pFlow_in->ID_COMP == 0 )
    *pFlow_in = *pN2Flow;
  double dT = Alfa * dT_max;
  Out.PT( pFlow_in->P, T + dT, pFlow_in->Get_Comp());
  pFlow_out->h_Flow = Out.h;
  pFlow_out->T = Out.T;
  double F = pFlow_out->Flow_mol;
  _Q = Out.h * F;
  _q = Out.h;
  _T = Out.T;
  _Eps = Out.E;
  }

void CUniModel::ElectroHeat( double dt )
{
	double Ksi = dt * 3600. / TauHeat;
	if ( Пуск_Стоп_нагрев.Use() || Пуск_стоп_нагрев_мест.Use() || Пуск_нагрев.Use() || Стоп_нагрев.Use())
	{
		double Qz = 1.;
		if ( Нагрев.Use())
			Qz = Нагрев * 0.01;
		bool Mest = Мест_дист_нагрев.Off();
		if(Mest)
		{
			if (Пуск_стоп_нагрев_мест.On())
			{
				Qh = ( Qh + Ksi * Qz * Q * 3600. ) / ( 1. + Ksi );
				Нагреватель_в_работе = true;
			}
			else
			{
				Qh = Qh / ( 1. + Ksi );
				Нагреватель_в_работе = false;
			}
		}
		else
		{
			if (Пуск_Стоп_нагрев.On())
			{
				Qh = ( Qh + Ksi * Qz * Q * 3600. ) / ( 1. + Ksi );
				Нагреватель_в_работе = true;
			}
			else if(Пуск_Стоп_нагрев.Off())
			{
				Qh = Qh / ( 1. + Ksi );
				Нагреватель_в_работе = false;
			}
			if(Пуск_нагрев.On())
			{
				Qh = ( Qh + Ksi * Qz * Q * 3600. ) / ( 1. + Ksi );
				Нагреватель_в_работе = true;
			}
			if(Стоп_нагрев.On())
			{
				Qh = Qh / ( 1. + Ksi );
				Нагреватель_в_работе = false;
			}
		}		
	}
	else
	{
		Qh = Qh / ( 1. + Ksi );
		Нагреватель_в_работе = false;
	}
	double F = pFlow_out->Flow_mol;
	if ( F < 0.1 )
		F = 0.1;
	pFlow_out->h_Flow = pFlow_in->h_Flow + Qh / F;
	Out.PH( pFlow_out );
    pFlow_out->h_Flow = Out.h;
    pFlow_out->T = Out.T;
    T_Out = Out.T;
}

void CUniModel::Heater( double dt )
  {
  Out.T_max = T_max;
  Out.T_min = T_min;
  if ( Пуск_нагрев.ExtOn())
    IsHeat = true;
  if ( Стоп_нагрев.ExtOn() || Стоп_по_блокировке.ExtOn())
    IsHeat = false;
  Авария = 0;
  if ( Q != 0. )
    {
    if ( IsHeat )
      {
      Нагреватель_в_работе = 1;
      double Qz = 1.;
      if ( Нагрев.Use())
	  {
		  Qz = Нагрев * 0.01;
		  Ток = Нагрев;
	  }
	  else
		  Ток = 100.0;
      double Ksi = dt * 3600. / TauHeat;
      Qh = ( Qh + Ksi * Qz * Q * 3600. ) / ( 1. + Ksi );
      }
    else
      {
      Нагреватель_в_работе = 0;
      double Ksi = dt * 3600. / TauHeat;
      Qh = Qh / ( 1. + Ksi );
	  Ток = 0.0;
      }
    double F = pFlow_out->Flow_mol;
    if ( F < 0.1 )
      F = 0.1;
    pFlow_out->h_Flow = pFlow_in->h_Flow + Qh / F;
    Out.PH( pFlow_out );
    pFlow_out->h_Flow = Out.h;
    pFlow_out->T = Out.T;
    _Q = Out.h * F;
    _q = Out.h;
    _T = Out.T;
    _Eps = Out.E;
    return;
    }
  if ( q != 0. )
    {
    pFlow_out->h_Flow = pFlow_in->h_Flow + q;
    Out.PH( pFlow_out );
    pFlow_out->h_Flow = Out.h;
    pFlow_out->T = Out.T;
    double F = pFlow_out->Flow_mol;
    _Q = Out.h * F;
    _q = Out.h;
    _T = Out.T;
    _Eps = Out.E;
    return;
    }
  if ( T != 0. )
    {
    Out.PT( pFlow_in->P, T, pFlow_in->Get_Comp());
    pFlow_out->h_Flow = Out.h;
    pFlow_out->T = Out.T;
    double F = pFlow_out->Flow_mol;
    _Q = Out.h * F;
    _q = Out.h;
    _T = Out.T;
    _Eps = Out.E;
    return;
    }
  if ( Eps != 0. )
    {
    Out.PE( pFlow_in->P, T, pFlow_in->Get_Comp());
    pFlow_out->h_Flow = Out.h;
    pFlow_out->T = Out.T;
    double F = pFlow_out->Flow_mol;
    _Q = Out.h * F;
    _q = Out.h;
    _T = Out.T;
    _Eps = Out.E;
    return;
    }
  }

void CUniModel::Script( double dt )
  {

  }

void CUniModel::Cleaner( double dt )
  {
  if ( pFlow_in == NULL || pFlow_out == NULL )
    return;
  CComp * pComp_in = pFlow_in->Get_Comp();
  if ( pComp_in == NULL )
    return;
  pCompOut->Set ( pComp_in );
  pCompOut->IsComp[G_H2S] = false;
  pCompOut->Cmol[G_H2S] = 0.;
  pCompOut->IsComp[G_CO2] = false;
  pCompOut->Cmol[G_CO2] = 0.;
  Norm ( pCompOut );
  In.PH( pFlow_in );
  Out.PT( pFlow_out->P, In.T, pCompOut );
  pFlow_out->ID_COMP = ID_CompOut;
  pFlow_out->pComp = pCompOut;
  pFlow_out->T = Out.T;
  pFlow_out->h_Flow = Out.h;
  pFlow_out->To_kg = ::Mw ( pCompOut );
  pFlow_out->To_m3 = To_m3_liq ( Out.T, pCompOut );
  }

void CUniModel::Produvka( double dt )
  {
  if ( Воздух )
    {
    Воздух = false;
    С_O2 = 0.209476;
    C_N2 = 1. - С_O2;
    }
  N2 = C_N2 * 100.;
  O2 = С_O2 * 100.;
  if ( !Продувка.On())
    return;
  double Ksi = dt * 60. / Tau_Produvka;
  С_O2 /= 1. + Ksi;
  C_N2 = 1. - С_O2;
  }


void CUniModel::CalcMEA( double dt )
  {
  In.PH( pFlow_in );
  pCompOut->Set ( &MEA );
  Out.PT( pFlow_out->P, In.T, pCompOut );
  pFlow_out->ID_COMP = ID_CompOut;
  pFlow_out->pComp = pCompOut;
  pFlow_out->h_Flow = Out.h;
  pFlow_out->T = Out.T;
  }

void CUniModel::SetComp( )
  {
  if ( Mass )
    {
    bool IsComp[K_GAS];
    double Cmol[K_GAS];
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( MEA.Cmol[c] >= 0. )
        {
        IsComp[c] = true;
        Cmol[c] = MEA.Cmol[c] / pGas[c]->Mw;
        }
      else
        {
        IsComp[c] = false;
        Cmol[c] = 0.;
        }
      }
    Norm(IsComp,Cmol );
    pCompOut->Set ( IsComp, Cmol );
    }
  else
    pCompOut->Set ( &MEA );
  }

void CUniModel::SetOutComp( CFlow * pFlow_i,  CFlow * pFlow_o )
  {
  if ( MustSetComp )
    {
    MustSetComp = false;
    SetComp( );
    }
  if ( Тип == 10 )
    FiltrComp( pFlow_i, pFlow_o );
  Out.PT( pFlow_o->P, In.T, pCompOut );
  pFlow_o->ID_COMP = ID_CompOut;
  pFlow_o->pComp = pCompOut;
  pFlow_o->h_Flow = Out.h;
  pFlow_o->T = Out.T;
  Mw = ::Mw ( pCompOut );
  pFlow_o->To_kg = Mw;
  F_kg = Flow * Mw;
  }

void CUniModel::FiltrComp( CFlow * pFlow_i,  CFlow * pFlow_o )
  {
  CComp * Comp_in = pFlow_i->Get_Comp();
  bool * IsComp_in = Comp_in->IsComp;
  double * Cmol_in = Comp_in->Cmol;
  bool * IsComp_out = pCompOut->IsComp;
  double * Cmol_out = pCompOut->Cmol;
  memmove ( IsComp_out, IsComp_in, LAST_GAS );
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp_in[n] )
      {
      Cmol_out[n] = Cmol_in[n] * k_Comp[n];
      }
    }
  Norm( pCompOut );
  double MW_in = ::Mw( Comp_in );
  double MW_out = ::Mw( pCompOut );
  k_Flow = 0.9 * k_Flow + 0.1 * MW_out / MW_in;
  }

#endif