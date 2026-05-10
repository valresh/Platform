#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"
#include "CommProc.h"
#include "Control.h"


void CFurnace::Calc( double dt )
	{
  SET_BP BreakPoint;
  for ( int n = 0; n < kProduct; n++ )
    {
    if ( pTube[n] && pTube[n]->pFlow_out )
      *pTube[n]->pFlow_out = *pTube[n]->pFlow_in;
    else
      KKK();
    }
  double Summ_O_Air = 0.;
  double Summ_O_Gas = 0.;
  double Summ_O_Gas2 = 0.;
  double Summ_O_Mazut = 0.;
  double Summ_O_Steam = 0.;
  double Summ_O_Pilot = 0.;
  double F_Smoke_Nom_Mol = Flow_Smoke_Nom / ( M3 * kFors );
  for ( int n = 0; n < kFors; n++ )
    {
    CForsunka * pFors = pFire[n]->Logic.pFors;
    pFire[n]->F_Smoke_Nom_Mol = F_Smoke_Nom_Mol;
    Summ_O_Air += pFors->ShibGas;
    Summ_O_Gas += VentGas( pFors );
    Summ_O_Mazut += pFors->VentMazut;// * ( 1. - V2 * 0.01 );
    Summ_O_Steam += pFors->VentSteam;// + V2;
    Summ_O_Pilot += VentPilot( n, pFors );
    Summ_O_Gas2 += VentGas2 ( pFors );//pFors->ShibMazut;
    if ( pFors->VentGas >= 98. )
      *Конц_откр_газ[n] = 1;
    else
      *Конц_откр_газ[n] = 0;
    if ( pFors->VentGas <= 2. )
      *Конц_закр_газ[n] = 1;
    else
      *Конц_закр_газ[n] = 0;
    }
  if ( Summ_O_Air < 0.01 )
    Summ_O_Air = 0.01;
  if ( Summ_O_Gas < 0.01 )
    Summ_O_Gas = 0.01;
  if ( Summ_O_Gas2 < 0.01 )
    Summ_O_Gas2 = 0.01;
  if ( Summ_O_Pilot < 0.01 )
    Summ_O_Pilot = 0.01;
  if ( Summ_O_Mazut < 0.01 )
    Summ_O_Mazut = 0.01;
  if ( Summ_O_Steam < 0.01 )
    Summ_O_Steam = 0.01;
  double N = 0.01 / kFors;
  O_Air = Summ_O_Air * N;
  O_Gas = Summ_O_Gas * N;
  O_Pilot = Summ_O_Pilot * N;
  O_Mazut = Summ_O_Mazut * N;
  O_Steam = Summ_O_Steam * N;
  O_Add_Gas = Summ_O_Gas2 * N;
  double Flow_Air = 0.;
  if ( pAir )
    {
    Flow_Air = pAir->Flow_mol;
    if ( Flow_air_add > 0. )
      Flow_Air += Flow_air_add;
//    else
//      Flow_Air = 0.;
    }
  else
    {
    if ( Flow_air_add >= 0. )
      Flow_Air = Flow_air_add;
//    else
//      Flow_Air = 0.;
    }
  double Mazut = 0.;
  if ( pMazut )
    {
    Mazut = pMazut->Flow_mol;
    if ( Mazut < 0. )
      Mazut = 0.;
    double To_kg_mazut = 160.;
    if ( Mazut > 0.1 )
      To_kg_mazut = ::Mw( pMazut->Get_Comp());
    Mw_mazut = To_kg_mazut;
    Mazut_Flow = Mazut * To_kg_mazut;
    }
  //////////////////////////////////////
  if ( Mazut_FlowZ > 0. && Mazut_Flow > 1. )
    {
    double K = Mazut_FlowZ / Mazut_Flow;
    double V = pFire[0]->Logic.pFors->VentMazut;
    double kV = LimdX( K - 1., 0.05 ) + 1.;
    V *= kV;
    if ( V > 100. )
      V = 100.;
    O_Mazut *= kV;
    for ( int n = 0; n < kFors; n++ )
      {
      pFire[n]->Logic.pFors->VentMazut = V;
      }
    }
//  double N_Mazut = 0.;//Mazut / Summ_O_Mazut; 
  double MazutSteam = 0.;
  if ( pMazutSteam )
    MazutSteam = pMazutSteam->Flow_mol;
  if ( MazutSteam < 0. )
    MazutSteam = 0.;
// 
  double N_Air = Flow_Air / Summ_O_Air;
  double Steam = 0.;
  if ( pSteam )
    Steam = pSteam->Flow_mol;
  if ( Steam < 0. )
    Steam = 0.;
  Flow_steam = MazutSteam + Steam;
//
  double N_Steam_Rad = Steam / kFors; 
  double F_Progar_1 = 0.;
  double h_Progar = 0.;
  if ( IsProgar )
    {
    double T_m = 0.;
    double F_p = 0.;
    for ( int n = 0; n < kProduct; n++ )
      {
      T_m += pTube[n]->T_in;
      F_p += pTube[n]->F_Прогар;
      }
    CFlow * pFlowПрогар = pTube[nProgar]->pFlow_in;
    CComp * pComp = pFlowПрогар->Get_Comp();
    CRadFors::pComp_Progar = pComp;
    if ( pComp )
      {
      F_Progar_1 = F_p / kFors;
      double T = pTube[nProgar]->In.T;
      h_Progar = pFlowПрогар->h_Flow;
      }
    }
  if ( pTopl_Gas && pTopl_Gas->ID_COMP )
    Mw_gas = Mw( pTopl_Gas->Get_Comp());
  else
    Mw_gas = 16.;
  double To_kg_gas = Mw_gas;
/////////////////////////////////////////////
  double Air_summ = 0.;
  double Topl_Gas = 0.;
#define kPascal 98.0665
  bool Podsos; 
  if ( Flow_air_add > 0. )
    {
    Podsos = true;
    Mw_podsos = Mw_air;
    }
  else
    {
    Podsos = false;
    Mw_podsos = Mw_gas;
    }
  double Flow_air_podsos_1f = Flow_air_add / kFors;
  Flow_topl_gas = 0.;
  EffGas = CalcEff( pTopl_Gas->Flow_mol );
  //if(Зажечь_все.IsConnection)
  //{
	 // if(Зажечь_все.pValue->nVal != 0)
		//  Fire_All = true;
	 // else
		//  Fire_All = false;
  //}
  for ( int n = 0; n < kFors; n++ )
    {
    pFire[n]->To_kg_gas = To_kg_gas;
    CForsunka * pFors = pFire[n]->Logic.pFors;
    double Shib_Air = pFors->ShibGas;
    double Fors_Topl_Gas = 0.;
    if ( Fire_All )
      pFors->setFire = true;
/////////////////
    double F_gas = 0.;
    if ( pTopl_Gas )
      F_gas = pTopl_Gas->Flow_mol * VentGas( pFors ) * EffGas * kFlowIn / Summ_O_Gas;
//    if ( !Podsos )
//      F_gas += Flow_air_podsos_1f;
    if ( F_gas < 0. )
      F_gas = 0.;
    double F_add = 0.;
    if ( pAdd_Gas )
      F_add = pAdd_Gas->Flow_mol * pFors->ShibMazut * kFlowIn / Summ_O_Gas2;
    double F_pilot = 0.;
    if ( pPilot )
      F_pilot = pPilot->Flow_mol * VentPilot( n, pFors ) * kFlowIn / Summ_O_Pilot;
    Flow_topl_gas += F_gas + F_add + F_pilot;
/////////////////
    double F_mazut = 0.;
    if ( pMazut )
      F_mazut = pMazut->Flow_mol * pFors->VentMazut * kFlowIn / Summ_O_Mazut;
    Flow_mazut = F_mazut;
    double F_steam = pFors->VentSteam / Summ_O_Steam; //pMazutSteam->Flow_mol *
    if ( kStep >= 5 )
      {
//      pFors -> setFire = false;
      if ( Розжиг_пилот && Розжиг_пилот[n] && Розжиг_пилот[n]->Use())
        {
        if ( Розжиг_пилот[n]->On())
          pFire[n]->Logic.pFors->setFire = true;
 //       else
 //         pFire[n]->Logic.pFors->setFire = false;
        }
      pFire[n]->Logic.Work (
                pFire[n]->Flow_air,
                F_gas + F_add, 
                F_pilot,
                F_mazut, 
                F_steam,
                NoZapal,
				MinWorkGas
				);
      }
    CRadFors * pF = pFire[n];
    if ( pFors->Is_State ( CForsunka::IS_WORK_FIRE ))
      {
      *Горит[n] = 1;
      double k_T = pF->T0 / T_fire_nom;
      if ( k_T > 1. )
        k_T = 1.;
      *Интенсивность_пламени[n] = k_T * 100.;
//      *FireFors[n] = Fire_Fors[n];
      }
    else
      {
      *Горит[n] = 0;
//      *FireFors[n] = 0.;
      *Интенсивность_пламени[n] = 0.;
      }
    if ( pFors->Is_State ( CForsunka::IS_ZAPAL_FIRE ))
      {
//      *FirePilot[n] = Fire_Pilot[n];
      *Горит_пилот[n] = 1;
      double k_pilot = F_pilot * M3 / F_pilot_nom;
      if ( k_pilot > 1. )
        k_pilot = 1.;
      *Интенсивность_пилот[n] = k_pilot * 100.;
      }
    else
      {
//      *FirePilot[n] = 0.;
      *Горит_пилот[n] = 0;
      *Интенсивность_пилот[n] = 0.;
      }
    pF->IsProgar = false;
    if ( IsFireAll )
      pF->IsFire = true;
    else
      pF->IsFire = pFire[n]->Logic.IsFire;
    if ( pTopl_Gas )
      {
      CRadFors::pComp_Gas;// = pTopl_Gas->Get_Comp();
      if ( pPilot && pPilot->Flow_mol > pTopl_Gas->Flow_mol )
        CRadFors::pComp_Gas = pPilot->Get_Comp();
      else
        CRadFors::pComp_Gas = pTopl_Gas->Get_Comp();
      pF->Flow_gas = F_gas + F_add + F_pilot;
      pF->h_gas = pTopl_Gas->h_Flow; 
      pF->To_kg_gas = pTopl_Gas->To_kg;
      Topl_Gas += pF->Flow_gas;
      }
    else
      {
      CRadFors::pComp_Gas = NULL;
      pF->Flow_gas = 0.;
      pF->h_gas = 0.; 
      pF->To_kg_gas = 0.;
      }
    if ( pMazut )
      {
      CRadFors::pComp_Mazut = pMazut->Get_Comp();
      pF->Flow_mazut_full = F_mazut;
      pF->h_mazut = pMazut->h_Flow;
      pF->To_kg_mazut = Mw_mazut;
      }
    else
      {
      CRadFors::pComp_Mazut = NULL;
      pF->Flow_mazut_full = 0.;
      pF->h_mazut = 0.;
      pF->To_kg_mazut = 0.;
      }
    if ( pMazutSteam )
      {
      pF->Flow_steam = F_steam;
      pF->h_steam = pMazutSteam->h_Flow;
      }
    else
      {
      pF->Flow_steam = 0.;
      pF->h_steam = 0.;
      }
    if ( pSteam )
      {
      pF->Flow_Steam_Rad = N_Steam_Rad;
      pF->h_Steam_Rad = pSteam->h_Flow;
      }
    else
      {
      pF->Flow_Steam_Rad = 0.;
      pF->h_Steam_Rad = 0.;
      }
    //////////////////////////
    //?? + Flow_air_podsos_1f;
    double Air_add = ( k_Air_Gas * pF->Flow_gas + k_Air_Mazut * pF->Flow_mazut_full  + Flow_air_podsos_1f ) * kFlowIn;
    pF->Flow_air = Shib_Air * N_Air * kFlowIn + Air_add;
    Air_summ += Air_add;
    if ( pAir )
      pF->h_air = pAir->h_Flow;
    else
      pF->h_air = pAirFlow->h_Flow;
    pF->Flow_Steam_Rad = 0.;
    pF->h_Steam_Rad = 0.;
    if ( IsProgar && pF->IsFire )
      {
      pF->F_Progar = F_Progar_1;
      pF->h_Progar = h_Progar;
      pF->IsProgar = true;
      }
    }
  Flow_air_fors = Air_summ;
  Air_Flow = Air_summ * M3;
//  TRACE("101-BU",Flow_topl_gas,"Flow_topl_gas")
//  TRACE("101-BU",Air_summ,"Air_summ")
//////////////////////////////////////////////////////////
  double Q_summ = 0.;
  T_fire = 0.;
  for ( int n = 0; n < kFors; n++ )
    {
    pFire[n]->KPD = KPD_fire;
    pFire[n]->Step( dt );
    Q_summ += pFire[n]->Q;
    T_fire += pFire[n]->T0;
    }
//  TRACE("101-BU",Q_summ,"Q_summ")
  T_fire /= kFors;
  _T_Fire = T_fire;
//  double Q_1 = Q_summ / kProduct;
  for ( int n = 0; n < kProduct; n++ )
    {
    if ( pTube[n] == NULL || pTube[n]->pFlow_in == NULL )
      continue;
    if ( !Calc_T4 )
      pTube[n]->Q_ext_Rad = W_Tube[n] * Q_summ;
    pTube[n]->Step( dt );
    *(Температура_на_выходе_труб[n]) = pTube[n]->T_out;
    double Tzm = ( 1. - k_Zm[n] ) * pTube[n]->T_in + k_Zm[n] * T_fire;
    *(Температура_змеевика[n]) = Tzm;
    }
  if ( pTube && pTube[0] )
    Т_змеевика = kТ_змеевика * ( pTube[0]->T_out - T_air ) + T_air;
  else
    Т_змеевика = T_air;
//
  double HF_Smoke = 0.;
  Flow_Smoke = 0.;
  CLEAR(IsComp_Smoke)
  CLEAR(Cmol_Smoke)
  double Q_Fire = 0.;
  for ( int n = 0; n < kFors; n++ )
    {
    CRadFors * pF = pFire[n];
    Q_Fire += pF -> Q_fire;
    Flow_Smoke += pF->Flow_Smoke;
    HF_Smoke += pF->H_Smoke * pF->Flow_Smoke;
    double dM = pF->Flow_Smoke * dt;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( pF->IsComp[c] )
        {
        IsComp_Smoke[c] = true;
        Cmol_Smoke[c] += dM * pF->Smoke_Cmol[c];
        }
      }
    }
  Norm( IsComp_Smoke, Cmol_Smoke );
  Smoke.Set ( Cmol_Smoke, IsComp_Smoke );
  pCompSmoke->Set ( IsComp_Smoke, Cmol_Smoke );
  Mw_smoke = Mw( pCompSmoke );
  if ( pSmoke )
    {
    pSmoke->ID_COMP = ID_CompSmoke;
    pSmoke->pComp = pCompSmoke;
    }
  if ( !_finite ( T_Pereval ))
    T_Pereval = T_air;
  if ( Flow_Smoke > 1. )
    {
    H_Smoke = HF_Smoke / Flow_Smoke;
    T_Pereval = Smoke.TH ( H_Smoke, T_Pereval );
    }
  else
    {
    //    T_Pereval = 0.99 * T_Pereval + 0.01 * T_air;
    double Ksi = dt * 60. / Tau_fire;
    double Old = 1. / ( 1. + Ksi );
    double New = Ksi * Old;
    T_Pereval = Old * T_Pereval + New * T_air;
    H_Smoke = Smoke.H( T_Pereval );
    }
  Smoke_Flow = Flow_Smoke * M3;
  if ( T_Pereval > Max_fire )
    T_Pereval = LimdX ( T_Pereval - Max_fire, 50. ) + Max_fire;
  T_Smoke_Out = T_Pereval;
  if ( pSmoke )
    {
    pSmoke->h_Flow = H_Smoke;
    d_Flow_Smoke = ( Flow_Smoke - pSmoke->Flow_mol ) * M3;
    }
  else
    d_Flow_Smoke = 0.;
//
  if ( Calc_T_Wall )
    CalcT_Wall( dt );
  if ( Calc_T4 )
    CalcT_Rad( dt );
  if ( New_Calc )
    {
    if ( Calc_Omega )
      {
      double dP = P_fors_mm_nom * 1e-4;
      Omega_podsos = F_air_m3_nom * 1e3 * iM3 / dP;
      if ( F_out_smoke_m3_fact > 0. )
        {
        dP = ( P_out_smoke_mm - P_fors_mm_nom ) * 1e-4;
        double K =  F_out_smoke_m3_fact * 1e3 * iM3 / ( dP * Omega_out_smoke );
        K = LimdX ( K - 1, 0.05 ) + 1.;
//        Omega_out_smoke += 0.5 * ( F_out_smoke_m3_fact * 1e3 * iM3 / dP - Omega_out_smoke );
        Omega_out_smoke *= K;
        }
      else
        Omega_out_smoke = 1.;
      }
    }
  else
    {
    CalcPress( dt );
    }
//
  double Q_Heat = 0.;
  double Q_HeatZ = 0.;
  for ( int n = 0; n < kProduct; n++ )
    {
    pTube[n]->TZ = T_Out_Z;
    pTube[n]->Calc_H = Calc_H;
    pTube[n]->Cp_Tube = Cp_Trub;
    Q_Heat +=  pTube[n]->Q_Продукт;
    if ( pTube[n]->pFlow_in )
      Q_HeatZ +=  pTube[n]->pFlow_in->Flow_mol * ( pTube[n]->hOutZ - pTube[n]->pFlow_in->h_Flow );
    }
  if ( Q_Fire > 1000. )
    KPD = Q_Heat / Q_Fire * 100.;
  else
    KPD = 0.;
//  double KZ = Q_HeatZ / Q_Heat;
//  F_Z = Topl_Gas * KZ * M3;
//  Т_на_перевале_1 = T_Pereval;
//  Т_на_перевале_2 = T_Pereval;
//  Т_на_перевале_3 = T_Pereval;
    double T_rad = T_air + kT_fire * ( T_fire - T_air );
    Т_в_камере_радиации_1 = T_rad;
//    Т_в_камере_радиации_2 = T_rad;
    double D = Delta_T_Pereval * 0.01;
    double Dr = 1. - D;
    double Dr1 = 1. + D;
//
#define SET_T(T) \
    {\
    double r = r01();\
    double r1 = 1. - r;\
    T = 0.95 * T + 0.05 * T_Pereval * ( Dr * r + Dr1 * r1 );\
    }
#define SET_F(Tsens,N,TFors) \
    {\
    double r = r01();\
    double r1 = 1. - r;\
    TFors = Alfa_T_Fors * pFire[N]->T + \
            Alfa_T_PM * ( pFire[N+1]->T + pFire[N-1]->T ); \
    double Ts = Alfa_T_Pereval * T_Pereval + ( 1. - Alfa_T_Pereval ) * TFors;\
    Tsens = 0.95 * Tsens + 0.05 * Ts * ( 0.95 * r + 1.05 * r1 );\
    }
  if ( kFors != 11 )
    Распределение_датчиков_перевала = false;
  if ( Распределение_датчиков_перевала )
    {
    double Alfa_T_PM = ( 1. - Alfa_T_Fors ) * 0.5;
    double TF_1;//, TF_2, TF_3, TF_4, TF_5;
    SET_F(Т_на_перевале,1,TF_1)
//    SET_F(Т_на_перевале_2,3,TF_2)
//    SET_F(Т_на_перевале_3,5,TF_3)
//    SET_F(Т_на_перевале_4,7,TF_4)
//    SET_F(Т_на_перевале_5,9,TF_5)
    KKK();
    }
  else
    {
    SET_T(Т_на_перевале)
//    SET_T(Т_на_перевале_2)
//    SET_T(Т_на_перевале_3)
//    SET_T(Т_на_перевале_4)
//    SET_T(Т_на_перевале_5)
//    SET_T(Т_на_перевале_6)
    }
  /////////////////////////////////////////////
//  double T_Rad = sqrt ( T_Max_Fire * T_Pereval );
  // Сделал, чтобы при неработающей печи, кислород показывал как в воздухе
  if (Flow_Smoke <=0.00001)
		O2_out = 20.75;
  else	
		O2_out = Smoke.Cmol[G_O2] * 100.;
  CO_out = Smoke.Cmol[G_CO] * 100.;
  О2_в_дыме = O2_out;
  CO_в_дыме = CO_out;
//
//  О2_в_дыме = O2_out;
//  CO_в_дыме = Smoke.Cmol[G_CO] * 1e2;
//  for ( int n = 0; n < 8; n++ )
//    {
//    *T_R[n] = rndT_R[n].Rnd ( T_Rad );
//    }
  /////////////////////////////////////////////
  CSpecPnt * pZ = pFirstPnt;
  while ( pZ )
    {
    if ( pZ->Type == 1 )
      {
      int n = pZ->n1;
      if ( n > 0 && n <= kProduct )
        {
        double T_Out = pTube[n-1]->T_out;
        double Tz = Alfa_TZm * T_Out + ( 1. - Alfa_TZm ) * T_Pereval;
        *pZ = Tz;
        }
      }
    pZ = pZ->pNext;
    }
/*
  for ( int n = 0; n < kProduct; n++ )
    {
    double T_Out = pTube[n]->T_out;
    double Tz = Alfa_TZm * T_Out + ( 1. - Alfa_TZm ) * T_Pereval;
    int nZm = 0;
    for ( int n = 0; n < 5; n++ )
      {
      *T_Zm[nZm] = Tz * rnd_T_Zm[nZm];
      nZm++;
      if ( nZm > kT_Zm )
        break;
      }
    if ( nZm > kT_Zm )
      break;
    }
*/
	}

const double P_max = 500. * 1e-4;

void CFurnace::CalcPress( double dt )
  {
  SET_BP
  if ( pSmoke == NULL )
    return;
  double dF = Flow_Smoke - pSmoke->Flow_mol;
  double dF_Smoke = dF;//Stab.Stab( dF );
  d_Flow_Smoke = dF_Smoke * M3;
//  d_Flow_Smoke = dF_Smoke * M3;
//--  P_Верх += k_P * dF_Smoke * dt;  //избыточное
  if ( P_Верх > 100e-4 )
    P_Верх = 100e-4;
  P_Верх_мм = P_Верх * 1e4 ; //избыточное
//////////////////////
  double dP_Air_mm = H_Рад * ( 1. - ( T_air + TK ) / ( T_fire + TK ));
  P_Под = P_Верх - dP_Air_mm * 1e-4;// + k_dP_Rad * Flow_Smoke
  P_Под_мм = P_Под * 1e4;
  if ( P_Под_мм > P_Под_мм_start )
    {
    P_Под_мм = P_Под_мм_start + LimdX ( P_Под_мм - P_Под_мм_start, P_Под_мм_max - P_Под_мм_start );
    P_Под = P_Под_мм * 1e-4;
    P_Верх = P_Под + dP_Air_mm * 1e-4;
    }
  if ( P_Под_мм > P_Out_мм_start )
    {
    if ( P_Под_мм > P_Out_мм_max )
      kFlowIn = 0.;
    else
      kFlowIn = ( P_Out_мм_max - P_Под_мм ) / ( P_Out_мм_max - P_Out_мм_start );
    }
  else
    kFlowIn = 1.;
//
  Разряжение_в_поде = P_Под_мм;
  Разряжение_на_перевале = P_Верх_мм;
//  Разряжение_на_перевале_1 = P_Верх_мм;
//  Разряжение_на_перевале_2 = P_Верх_мм;
  Отрицательное_разряжение_в_поде = -P_Под_мм;
  Отрицательное_разряжение_на_перевале = -P_Верх_мм;
  if ( CalcdP )
    {
    double dP = H_Calc * ( T_air + TK ) / ( kdP_Calc * ( T_Calc - T_air ) + T_air + TK );
    dP_Calc = dP - H_Calc;
    }
  }

double CFurnace::VentGas ( CForsunka * pFors )
  {
  if ( kGasVent <= 1 )
    return pFors->VentGas;
  double iV;
  if ( pFors->VentGas > 1e-10 )
    iV = 1. / pFors->VentGas;
  else
    iV = 1e10;
  double iV2;
  if ( pFors->VentGas2 > 1e-10 )
    iV2 = 1. / pFors->VentGas2;
  else
    iV2 = 1e10;
  double V = 2. / ( iV + iV2 );
  return V;
  }

double CFurnace::VentPilot ( int n, CForsunka * pFors )
  {
  if ( Открыть_пилот && Открыть_пилот[n] && Открыть_пилот[n]->Use() )
    {
    if ( Открыть_пилот[n]->Off())
      return 0.;
    else
      KKK();
    }
  if ( kPilotVent <= 1 )
    return pFors->VentPilot;
  double iV;
  if ( pFors->VentPilot > 1e-10 )
    iV = 1. / pFors->VentPilot;
  else
    iV = 1e10;
  double iV2;
  if ( pFors->VentPilot2 > 1e-10 )
    iV2 = 1. / pFors->VentPilot2;
  else
    iV2 = 1e10;
  double V = 2. / ( iV + iV2 );
  return V;
  }

double CFurnace::VentGas2 ( CForsunka * pFors )
  {
  if ( kGas2Vent <= 1 )
    return pFors->VentSteam;
  double iV;
  if ( pFors->VentSteam > 1e-10 )
    iV = 1. / pFors->VentSteam;
  else
    iV = 1e10;
  double iV2;
  if ( pFors->VentSteam2 > 1e-10 )
    iV2 = 1. / pFors->VentSteam2;
  else
    iV2 = 1e10;
  double V = 2. / ( iV + iV2 );
  return V;
  }

void CFurnace::CalcT_Wall( double dt )
  {
// Энергия от форсунок
// Q = Af * Fsmoke^Kf * ( Tf - Twall ); 
// Q = Afors * Twall + Bfors 
// Нагрев труб
// Q = At * ( Twall - Tout ) 
// Q = Atrub * Twall + Btrub 
// Потери
// Q = Al * ( Twall - Tair ) 
// Q = Alost * Twall + Blost 
  double F_Nom = Flow_Smoke_Nom / ( M3 * kFors );
  double Af = 0.;
  double Bf = 0.;
  for ( int n = 0; n < kFors; n++ )
    {
    CRadFors * pF = pFire[n];
    double Tf = pF->T;
    double Fsmoke = pF->Flow_Smoke / F_Nom;
    double M_smoke = 0.;
    if ( Fsmoke > 1e-6 )
      M_smoke = pow ( Fsmoke, Pow_Smoke );
    pF->A_wall = -A_fors * M_smoke;
    pF->B_wall = A_fors * M_smoke * Tf;
    Af += pF->A_wall;
    Bf += pF->B_wall;
    }
//
  double At = 0.;
  double Bt = 0.;
  for ( int n = 0; n < kProduct; n++ )
    {
    CRadTube * pT = pTube[n];
    double Tt = pT->T_out;
    pT->A_wall = W_Tube[n] * A_trub;
    pT->B_wall = -W_Tube[n] * A_trub * Tt;
    At += pT->A_wall;
    Bt += pT->B_wall;
    }
//
  double Aa = A_air;
  double Ba = -A_air * T_air;
//
  double Aq = Af - At - Aa;
  double Bq = Bf - Bt - Ba;
// dTw/dt = Q / Tau
// Tw = _Tw + Ksi * Aq * Tw +  Ksi * Bq
  double Ksi = 60. * dt / Tau_T_Wall;
  if ( !_finite(T_Wall))
    T_Wall = T_air;
  T_Wall = ( T_Wall + Ksi * Bq ) / ( 1. - Ksi * Aq );
//////////////////
  double Q = 0.;
  for ( int n = 0; n < kFors; n++ )
    {
    CRadFors * pF = pFire[n];
    pF->Q_wall = pF->A_wall * T_Wall + pF->B_wall;
    FINITE(pF->Q_wall)
    Q += pF->Q_wall;
    }
  Q_fors = Q;
  Q = 0.;
  for ( int n = 0; n < kProduct; n++ )
    {
    CRadTube * pT = pTube[n];
    pT->Q_wall = pT->A_wall * T_Wall + pT->B_wall;
    FINITE(pT->Q_wall)
    Q += pT->Q_wall;
    }
  Q_trub = Q;
  Q_air = Aa * T_Wall + Ba;
  Q_summ = Q_fors - Q_trub - Q_air;
  }


void CFurnace::CalcCoef( )
  {
  if ( a_ft < 0.01 )
    a_ft = 0.01; 
  if ( a_ft > 0.99 )
    a_ft = 0.99;
  a_fw = 1. - a_ft;
  a_ff = 0.;
//
  a_tf = S_Fors * a_ft / S_Trub;
  a_tt = 1. - a_tf - a_tw;
//
  a_wf = S_Fors * a_fw / S_Wall;
  a_wt = S_Trub * a_tw / S_Wall;
  a_ww = 1. - a_wf - a_wt;
  }

double CFurnace::CalcEff( double FlowGas )
  {
  FlowGas *= M3 * 1e-3;
  if ( FlowGas <= Fuel[0] )
    return Eff[0];
  if ( FlowGas >= Fuel[3] )
    return Eff[3];
  int n = 0;
  while ( FlowGas > Fuel[n+1] )
    n++;
  double E = Eff[n] + ( FlowGas - Fuel[n] ) / ( Fuel[n+1] - Fuel[n] ) * ( Eff[n+1] - Eff[n] );
  return E;
  }
