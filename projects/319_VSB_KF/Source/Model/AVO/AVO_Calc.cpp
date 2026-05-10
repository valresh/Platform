#include "stdafx.h"
#include "AVO.h"
#include "Err.h"
#include "CommProc.h"


bool CAVO::Set_One_Jalousie( double dt, int & Ctrl_Jal, double & Jalousie,
                            CAnalog & Жалюзи, CDiscrete & Жалюзи_открыты, CDiscrete & Жалюзи_закрыты,
                            CDiscrete & Закрыть_жалюзи, CDiscrete & Открыть_жалюзи  )  
  {
  bool Use = false;
  /*if ( Жалюзи.Use())		
    {
    Jalousie = Жалюзи * 0.01;		//Это выходная точка зачем это делать???
    return true;
    }*/
  if ( Закрыть_жалюзи.Use() && Открыть_жалюзи.Use() )
    {
    if ( !Imp_Ctrl_Jal )
      Ctrl_Jal = 0;
    Use = true;
    if ( Закрыть_жалюзи.On() )
      {
      Ctrl_Jal = -1;
      }
    if ( Открыть_жалюзи.On() )
      {
      Ctrl_Jal = 1;
      }
    if ( Ctrl_Jal < 0 )
      {
      double dJal = 1.1;
      if ( Time_Jal > 0. )
        dJal = dt * 3600. / Time_Jal;
      Jalousie -= dJal;
      if ( Jalousie < 0. )
        {
        Ctrl_Jal = 0;
        Jalousie = 0.;
        }
      }
    if ( Ctrl_Jal > 0 )
      {
      double dJal = 1.1;
      if ( Time_Jal > 0. )
        dJal = dt * 3600. / Time_Jal;
      Jalousie += dJal;
      if ( Jalousie > 1. )
        {
        Ctrl_Jal = 0;
        Jalousie = 1.;
        }
      }
    Жалюзи = Jalousie * 100.;
    if ( Jalousie >= 0.99 )
      Жалюзи_открыты = 1;
    else
      Жалюзи_открыты = 0;
    if ( Jalousie <= 0.01 )
      Жалюзи_закрыты = 1;
    else
      Жалюзи_закрыты = 0;
    }
  return Use;
  }

void CAVO::Set_Jalousie( double dt )
  {
  //Авария_жалюзей_1 = 0;
  //Авария_жалюзей_2 = 0;
  //Авария_жалюзей_3 = 0;
  //Авария_жалюзей_4 = 0;
  //bool Use[4] ;
  //Use[0] = Set_One_Jalousie( dt, Ctrl_Jal_1, Jalousie_1, Жалюзи_1, 
  //               Жалюзи_открыты_1, Жалюзи_закрыты_1,
  //               Закрыть_жалюзи_1, Открыть_жалюзи_1 );  
  //Use[1] = Set_One_Jalousie( dt, Ctrl_Jal_2, Jalousie_2, Жалюзи_2, 
  //               Жалюзи_открыты_2, Жалюзи_закрыты_2,
  //               Закрыть_жалюзи_2, Открыть_жалюзи_2 );  
  //Use[2] = Set_One_Jalousie( dt, Ctrl_Jal_3, Jalousie_3, Жалюзи_3, 
  //               Жалюзи_открыты_3, Жалюзи_закрыты_3,
  //               Закрыть_жалюзи_3, Открыть_жалюзи_3 );  
  //Use[3] = Set_One_Jalousie( dt, Ctrl_Jal_4, Jalousie_4, Жалюзи_4, 
  //               Жалюзи_открыты_4, Жалюзи_закрыты_4,
  //               Закрыть_жалюзи_4, Открыть_жалюзи_4 );  
  //double Jal_Summ = 0.;
  //double kJal_Summ = 0.;
  //for ( int n = 0; n < 4; n++ )
  //  {
  //  if ( Use[n] )
  //    {
  //    Jal_Summ += *pJal[n];
  //    kJal_Summ++;
  //    }
  //  }
  //if ( kJal_Summ > 0. )
  //  Jalousie_Eff = Jal_Summ / kJal_Summ;
  //else
  //  Jalousie_Eff = 1.;
  //for ( int n = 0; n < 4; n++ )
  //  {
  //  if ( !Use[n] )
  //    *pJal[n] = Jalousie_Eff;
  //  }
  }

void CAVO::Calc( double dt )     
	{
  SET_BP BreakPoint; 
  if ( Heat.pIn_Trub == NULL || Heat.pOut_Trub == NULL )
		return;
//  Пускатель.Work( Состояние );
//  On = Состояние.On();
  double Угол_атаки = 0.;
  int k = 0;
  int k_w = 0;
  if(kMotor != 0)
	  _Oborot = 0.0;
  for ( int n = 0; n < kMotor ;n++ )
  {
	  if ( pMotors[n] )
		  k++;
	  if ( pMotors[n] && *pMotors[n]->pOn )
	  {
		  _Oborot += *pMotors[n]->pRelOborot;
		  Угол_атаки += pMotors[n]->AngleOfAttack;
		  k_w++;
	  }
  }
  if ( k <= 0 )
    {
    On = true;
    Состояние = 1;
	Угол_атаки = 1;
    }
  else
    {
    On = true;
    _Oborot /= k;
    if ( k_w <= 0 )
      Угол_атаки = 1.;
    else
      Угол_атаки /= k_w;
    Состояние = 1;
    }
  //_Oborot = Oborot;
//
  Угол_атаки = 1;
  if ( Angle_Defect )
    {
    Угол_атаки = Def_Angle;
    }
  _Угол_атаки = Угол_атаки * 100.;
  double O_eff = _Oborot * Угол_атаки;
  if ( Pow_Air != 1. && O_eff > 0. )
    O_eff = pow ( O_eff, Pow_Air );
  O_eff = ( 1. - F_Air_XX ) * O_eff + F_Air_XX;
//////////////////////////////////////////////////////////////
  if ( Положение_жалюзей.Use())
  {
	if ( Положение_жалюзей_1.Use())
	{
		if (Положение_жалюзей_2.Use())
		{
			Jalousie_Eff = ( Положение_жалюзей + Положение_жалюзей_1 + Положение_жалюзей_2) / 300;			
		}
		else
		{
			Jalousie_Eff = ( Положение_жалюзей + Положение_жалюзей_1 ) * 0.005;
		}
	}
	else
	{
	Jalousie_Eff = Положение_жалюзей * 0.01;
	}
  }
Жалюзи_1 = Положение_жалюзей;
Жалюзи_2 = Положение_жалюзей_1;
Жалюзи_3 = Положение_жалюзей_2;
//  else
//    {
//    Set_Jalousie( dt );
//    }
//////////////////////////////////////////////////////////////
  if ( Use_Жалюзи_зад )
    {
    Jalousie_Eff = Жалюзи_зад * 0.01;
    }
  if ( !Use_T_cam )
    {
    double Coef = Angle_0 * ( 1. - Jalousie_Eff ) + Jalousie_Eff; 
    O_eff *= Coef;
    }
//
  double KS;
  if ( Use_Pow_Omega )
    KS = K * S * 3.6 * k_Heat * O_eff;
  else
    KS = K * S * 3.6 * k_Heat;
//  if ( Pow_Omega != 0. && Oborot > 0. )
//    KS *= pow ( Oborot * Угол_атаки, Pow_Omega );
  if ( pIn_Water )
    {
    double F = fabs ( pIn_Water->Flow_mol ) * 18.;
    double Keff = 1. + ( k_Water_Max - 1. ) * ( 1. - exp ( -F / F_Water_Max ));
    KS *= Keff;
    if ( pOut_Water )
      *pOut_Water = *pIn_Water;
    }
  if ( pIn_Water_2 )
    {
    double F = fabs ( pIn_Water_2->Flow_mol ) * 18.;
    double Keff = 1. + ( k_Water_Max - 1. ) * ( 1. - exp ( -F / F_Water_Max ));
    KS *= Keff;
    }
//
  double Flow_air_mol = F_Air_kg / 29.;
  In_Air = *pAirFlow;
  In_Air.Flow_mol = O_eff * Flow_air_mol;
  if ( pIn_Steam )
    {
    HeatAir.KS = Kh * Sh * 3.6;
    HeatAir.Static = Static;
    HeatAir.Calc( dt );
    }
  if ( Use_T_cam )
    {
    double Alfa = ( Alfa_Jal_0 * ( 1. - Jalousie_Eff ) + Alfa_Jal_100 * Jalousie_Eff ) * 0.01;
    double T_air_jal = _T_air_out * ( 1. - Alfa ) + T_air * Alfa;
    Т_в_камере = T_air_jal;
    In_Air.h_Flow = H_IG( T_air_jal, pAir );
    }
  bool Reg = false;
  if ( Reg_T_out.IsOn())
    Reg = true;
  if ( OldHeat )
    {
    Heat.KS = KS;
    if ( Reg )
      Heat.Cp_Corp_Trub = 0.;
    else
      Heat.Cp_Corp_Trub = -M_Corp * 400.;
    Heat.Static = Static;
    Heat.Calc( dt );
    _T_in = Heat.T_in_Trub;
    //  Т_входа = _T_in;
    _T_out = Heat.T_out_Trub;
    //  Т_выхода = _T_out;
    _T_air_out = Heat.T_out_Vol;
    Т_воздуха = _T_air_out;
    Q_cal = Heat.Q_cal;
    _h_in = Heat.pIn_Trub->h_Flow;
    _h_out = Heat.pOut_Trub->h_Flow;
    _Lam = Heat.Lam_Trub;
    _Lam_Air = Heat.Lam_Vol;
    }
  else
    {
    Heat1flow.KS = KS;
    Heat1flow.Static = Static;
    Heat1flow.Calc( dt );
    _T_in = Heat1flow.T_in_Work;
    //  Т_входа = _T_in;
    _T_out = Heat1flow.T_out_Work;
    //  Т_выхода = _T_out;
    _T_air_out = Heat1flow.T_out_Const;
    Т_воздуха = _T_air_out;
    Q_cal = Heat1flow.Q_cal;
    _h_in = Heat1flow.pIn_Work->h_Flow;
    _h_out = Heat1flow.pOut_Work->h_Flow;
    _Lam = 0.;//Heat1flow.Lam_Work;
    _Lam_Air = 0.;//Heat1flow.Lam_Const;
    }
//
  CalcCond( );
//
  if ( Reg )
    {
    double MV = ( K - 1. ) / 99.;  // К = 1-100
    if ( Reg_T_out.Reg( dt, _T_out, &MV ))
      K = MV * 99. + 1.;
    }
  if ( _Lam_Opt > 0. )
    {
    double K = _Lam_Air / _Lam_Opt;
    F_Air_kg *= K;
    }
  if ( dP_Eps )
    {
// Перепад от конденсации
    double E = Heat.S_Trub_Out.E;
    if ( E > 0. )
      {
      U = 1.;
      if ( dP_Lin )
        {
        double k = 0.5 * ( 1. + E );
        kOmega = ( 1. - Alfa_Omega ) * kOmega + Alfa_Omega * k;
        }
      else
        {
        double k = ( E * E + E + 1. ) / 3.;
        double kF = Heat.pIn_Trub->Flow_mol / Flow_Nom;
        kOmega = ( 1. - Alfa_Omega ) * kOmega + Alfa_Omega * k * kF;
        }
      }
    else
      {
// Ищем путь конденсации
      U = CalcCondPath( );
      double L = U;
      if ( dP_Lin )
        {
        double k = 0.5 * L;
        kOmega = ( 1. - Alfa_Omega ) * kOmega + Alfa_Omega * k;
        }
      else
        {
        double k = L / 3.;
        double kF = Heat.pIn_Trub->Flow_mol / Flow_Nom;
        kOmega = ( 1. - Alfa_Omega ) * kOmega + Alfa_Omega * k * kF;
        }
      }
    }
  else
    {
    Flow_Nom = Heat.pIn_Trub->Flow_mol; 
    kOmega = 1.;
    }
	}

double CAVO::CalcCondPath( )
  {
  Cond.PE( Heat.pIn_Trub->P, 0., Heat.pIn_Trub->Get_Comp());
  double dh_cond = Heat.S_Trub_In.h - Cond.h;
  double dh_cool = Heat.S_Trub_In.h - Heat.S_Trub_Out.h;
  if ( dh_cool < 1. )
    return 1.;
  double U = dh_cond / dh_cool;
  if ( U < 0. )
    U = 0.;
  if ( U > 1. )
    U = 1.;
  return U;
  }

void CAVO::CalcCond( )
  {
  if ( Конденсация )
    KKK();
  Eps_cond = 1. - Heat.S_Trub_Out.E;
  return;
  double X[K_GAS];
  bool IsCompX[K_GAS];
  double Pz = Heat.pOut_Trub->P;
  if ( Eps_cond > 0.05 )
    {
    Heat.S_Trub_Out.SetCompData ( IsCompX, X, NULL, NULL, NULL );
    double S = 0.;
    double P = 0.;
    double Tz = Heat.S_Trub_Out.T;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( !IsCompX[n] )
        continue;
      if( X[n] > 0.)
        {
        S += X[n];
        P += X[n] * pGas[n]->Pz( Tz );
        }
      }
	  if ( S > 1e-5 ) 
      {
      if ( fabs ( S - 1. ) < 1e-8 )
        Pz = P;
      else
        Pz = P / S; 
      }
    }
  P_cond = 0.9 * P_cond + 0.1 * Pz;
  }

