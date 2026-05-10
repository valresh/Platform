#include "stdafx.h"
#include "Fan.h"
#include "Err.h"
#include "CommProc.h"


// От свойств:
// P2/P1 = K*O*O*Mu/(R*T)*(g-1)/g + 1 стр 363

double CFan::P2_P1_0( )
  {
  double kOborot;
  if ( Use_ОН )
    kOborot = RelH ( Oborot );
  else
    kOborot = Oborot * Oborot;
  double dP = K_P2_P1 * kOborot * Mw_gas / ( RG * ( Tin + TK ));
  return dP;
  }

void CFan::Calc_K_P2_P1( )
  {
  // К-т для номинальных условий
  // P2/P1 = ( K*O*O*Mu/(R*T)*(g-1)/g + 1 ) ^ g / (g-1) стр 363
  double dP = Hmax.P_atm;
  K_P2_P1 = dP / Mw_0 * ( RG * ( Tin_0 + TK ));
  }

double CFan::RelH ( double O )
  {
  if ( O < Обороты[0] )
    return Напор[0];
  if ( O > Обороты[7] )
    {
    double X = O - Обороты[7];
    return Напор[4] + X * X;
    }
  int n = 1;
  while ( n < 5 )
    {
    if ( O < Обороты[n] )
      break;
    n++;
    }
  double Ht = Напор[n-1] + ( O - Обороты[n-1] ) / ( Обороты[n] - Обороты[n-1] ) * ( Напор[n] - Напор[n-1] );
  return Ht;
  }

void CFan::Calc( double dt )
	{
  SET_BP BreakPoint;
  if ( !ExtOboror( dt ))
    IntOboror( dt );
//  Fire( dt );
//  Охлаждение.T_Fire = T_Fire;
//  Смазка.T_Fire = T_Fire;
//
  //if ( pPump->LocalRemote == 1 )
  //  Дистанционное_управление = 1;
  //else
  //  Дистанционное_управление = 0;
//
  if ( On )
    {
//    Стоит = 0;
    Работает = 1;
    }
  else
    {
//    Стоит = 1;
    Работает = 0;
    Oborot = 0.9 * Oborot;
    }
  if ( pFlow_in )
    Flow_mol = 0.9 * Flow_mol + 0.1 * pFlow_in->Flow_mol;
  else
    Flow_mol = 0.9 * Flow_mol + 0.1 * Qmax * Oborot * iM3 * 1e3;
  if ( pFlow_in )
    {
    *pFlow_out = *pFlow_in;
    StateIn.PH ( pFlow_in );
    Tin += Eps_H * ( StateIn.T - Tin );
    Mw_gas = ::Mw ( pFlow_in->Get_Comp());
    *pFlow_out = *pFlow_in;
    double dP = ( pFlow_out->P - pFlow_in->P ) * Pascal; //Па
    double F = pFlow_in->Flow_mol * To_m3_in;   // М3/ч
    P_kdg = dP * F * 1e-3 / ( KPD_M * 0.01 );  // кДж/ч
//      double Tst = dP * F / P_kdg;
    }
  else
    {
    Tin = T_air;
//    Roin = 1.;
    }
  /////////////////////////////////////////////////
  double H_fin;
  Q =	Flow_mol * M3 * 1e-3;
  if ( Учитывать_среду )
    H_fin = P2_P1_0( );
  else
    H_fin = Hmax.P_atm;
  if ( On )
    {
    Фактические_обороты = Oborot * Nom_Oborot;
    Ток = A_Ток * Pow + B_Ток * ( 1. - Oborot );
    static double To_MWt =  1e-3 / 3600.;
    Pow = P_kdg * To_MWt;
    Мощность = Pow;
    Работает = 1;
    //    Стоит = 0;
    //    Работает = 1;
//    double Coef_H = Oborot * Oborot;
//    if ( Use_X )
//      Coef_H = RelH ( Oborot );
//    pPump-> On_State ( CPumpData::Work );
    //		Работает = 1;
//    Status = 1;
//    pPump->dAmper = I;
//    Ном_ток = I_Max;
//    Момент = Oborot * 100.;
//    Мощность = Oborot * Nom_Pow;
    }
  else
    {
    Работает = 0;
    Ток = 0.;
    if (pMotor == NULL)
      Oborot = 0.9 * Oborot;

//    pPump->dAmper = 0.;
//    pPump-> Off_State ( CPumpData::Work );			
//    Момент = 0.;
    Фактические_обороты = 0.;
    Мощность = 0.;
    Oborot = 0.;
//    Status = 0;
//    Ном_ток = I_Max;
    }
// dP = K * O * O * Mu * Pin / ( R * T )   K - из номинала
//  Частота = Oborot_Max * Oborot;
  if ( H_zad.P_atm > 0. )
    {               
    double K = H_zad.P_atm / H_fact.P_atm;
    Hmax.P_atm *= LimdX ( K - 1, 0.01 ) + 1.;
    Calc_K_P2_P1( );
    }
  //
  H += Eps_H * ( H_fin - H );
  dP_work = H;
  if ( Defect == 3 )
    dP_work *= Defect_Pow;
  //if ( Pow_Max < 0. )
  //  {
  //  Pow_Max = 1.2 * dP_work * Q;
  //  }
  //if ( Pow_Max > 0. )
  //  {
  //  double K = Pow / Pow_Max;
  //  double s_Ток = Ток_Max * K;
  //  }
  //////////////////////
  Охлаждение.Oborot = Oborot; 
  Охлаждение.StepT( dt );
  if ( Смазка )
  Смазка->StepT( dt );
////////////////////////
  double Pin = pFlow_in->P;
  To_m3_in = RP * ( Tin + TK ) / Pin;
  Q_m3 = pFlow_in->Flow_mol * To_m3_in * 1e-3;
  Pow = dP_work * Q_m3 * Pascal / ( KPD_M * 0.01 );
  static double To_MWt =  1e-3 / 3600.;
  Мощность = PowMWt = Pow * To_MWt;
  double Ksi = dt * 3600. / Tau;
  double Old = 1. / ( 1. + Ksi );
  double New = Ksi * Old;
  double _O = Oborot / V_Krit;
  double _O_1 = _O - 1;
  double V;
  double Delta2 = 3.;
  if ( _O < 1. )
    V = _O * Delta2 / ( Delta2 + _O_1 * _O_1 );
  else
    V = Delta2 / ( Delta2 + _O_1 * _O_1 );
  for ( int n = 0; n < kSens_Vibr; n++ )
    {
    double Vib = V * rndVibrSens[n] * _def_Вибраций;
    *(pVibro[n]) = Vib;
    if ( r01() > 0.5 )
      rndVibrSens[n] = Old * rndVibrSens[n] + New * Max_Vibration;
    else
      rndVibrSens[n] = Old * rndVibrSens[n] + 0.5 * New * Max_Vibration;
    }
  double I; 
  if ( On )
    I = B_Ток * ( 1. - Oborot ) + A_Ток * Pow; 
  else
    I = 0.;
  Ток = I;
	}

void CFan::Fire( double dt )
  {
#if 0
  double dt_min = dt * 60.;
  switch ( Faza_Fire )
    {
    case 0:	// Нет
      {
      Time_Fire = 0.;
      T_Fire = 0.; 
      }
    break;
    case 1:	// Разгорание
      {
      Time_Fire	+= dt_min;
      double Alfa = Time_Fire / Tau_Start_Fire;
      if ( Alfa > 0.5 )
        {
//        pPump->Off_State ( CPumpData::Work );
        On = false;
        }
      if ( Alfa > 1. )
        {
        Alfa = 1.;
        Faza_Fire = 2;
        Time_Fire = 0.;
        }
      T_Fire = Alfa * T_Max_Fire;
      }
    break;
    case 2:	// Горение
      {
      Time_Fire	+= dt_min;
      if ( Time_Fire > Tau_Fire )
        {
        Faza_Fire = 3;
        Time_Fire = 0.;
        }
      T_Fire = T_Max_Fire;
//      pPump->Off_State ( CPumpData::Work );
      On = false;
      }
    break;
    case 3:	// Остывание
      {
      Time_Fire	+= dt_min;
      double Tau = Time_Fire / Tau_Cool;
      if ( Tau > 50. )
        Tau = 50.;
      T_Fire = T_Max_Fire * exp ( -Tau );
//      pPump->Off_State ( CPumpData::Work );
      On = false;
      }
    break;
    case 4:	// Конец
    break;
    }
#endif
  }
