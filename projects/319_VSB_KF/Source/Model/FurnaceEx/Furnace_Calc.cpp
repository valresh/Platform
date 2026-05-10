#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"
#include "CommProc.h"

int nStep = 0;
void CFurnace::Calc( double dt )
	{
  SET_BP BreakPoint;
  for ( int n = 0; n < kProduct; n++ )
    {
    if ( pTube[n] && pTube[n]->pFlow_out )
      *pTube[n]->pFlow_out = *pTube[n]->pFlow_in;
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
    }
  double Flow_Air_Подсос;
  if ( P_Под < 0. )
    Flow_Air_Подсос = -kAir * P_Под;
  else
    Flow_Air_Подсос = 0.;
  double N_Air = Flow_Air_Подсос / kFors;
  //////////////////////////////////////
  double Steam = 0.;
  if ( pSteam )
    Steam = pSteam->Flow_mol;
  if ( Steam < 0. )
    Steam = 0.;
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
/////////////////////////////////////////////
  double Air_summ = 0.;
  double Topl_Gas = 0.;
#define kPascal 98.0665
  for ( int n = 0; n < kFors; n++ )
    {
    CForsunka * pFors = pFire[n]->Logic.pFors;
    double F_gas = 0.;
    if ( pTopl_Gas && pTopl_Gas[n] )
      F_gas = pTopl_Gas[n]->Flow_mol;
    P_Gas[n] = pTopl_Gas[n]->P - 1.;
    F_Gas_m3[n] = F_gas * M3;
//    double F_add = 0.;
//    if ( pAdd_Gas )
//      F_add = pAdd_Gas->Flow_mol;
    double F_pilot = 0.;
    if ( pPilot && pPilot[n] )
      {
      F_pilot = pPilot[n]->Flow_mol;
      if ( C_pilot_O2 > 0. )
        {
        CComp * pComp = pPilot[n]->Get_Comp();
        if ( !pComp->IsComp[G_O2] || pComp->Cmol[G_O2] < C_pilot_O2 * 0.01 )
          F_pilot = 0.;
        }
      P_Pilot[n] = pPilot[n]->P - 1.;
      F_Pilot_m3[n] = F_pilot * M3;
      }
    double F_mazut = 0.;
    pFire[n]->GasAsMasut = false;
    if ( pMazut && pMazut[n] )
      {
      if ( pMazut[n]->Flow_mol > 0.01 )
        {
        F_mazut = pMazut[n]->Flow_mol;
        double Mw = ::Mw( pMazut[n]->Get_Comp());
        if ( Mw < 100. )
          pFire[n]->GasAsMasut = true;
        F_Mazut_kg[n] = F_mazut * Mw;
        }
      else
        F_Mazut_kg[n] = 0.;
     }
    else
      F_Mazut_kg[n] = 0.;
    double F_steam = 0.;
    if ( pMazutSteam && pMazutSteam[n] )
      {
      F_steam = pMazutSteam[n]->Flow_mol;
      if ( F_steam < 0.01 )
        F_steam = 0.;
      F_Steam_kg[n] = F_steam * Mw_H2O;
      }
    else
      F_Steam_kg[n] = 0.;
    if ( kStep >= 5 )
      {
//////////////////////
      if ( Fire_All > 0 )
        {
        pFire[n]->Logic.SetFirePilot = true;
        pFire[n]->Logic.SetFireMain = true;
//        pFors->setFire = true;
        }
      if ( Fire_All < 0 )
        {
        pFire[n]->Logic.SetFirePilot = false;
        pFire[n]->Logic.SetFireMain = false;
        }
//////////////////////
      if ( Розжиг_пилот && Розжиг_пилот[n] && Розжиг_пилот[n]->Use())
        {
        if ( Розжиг_пилот[n]->On())
          {
          pFire[n]->Logic.SetFirePilot = true;
          }
        else
          {
          if ( pFire[n]->Logic.pFors->setFire )
            pFire[n]->Logic.SetFirePilot = true;
		      else
			      pFire[n]->Logic.SetFirePilot = false;
          }
        }
      else
        {
        if ( pFire[n]->Logic.pFors->setFire )
          pFire[n]->Logic.SetFirePilot = true;
        }
      //if ( pBurnAll )
      //  {
      //  if ( pBurnAll->mOn )
      //    pFire[n]->Logic.SetFireMain = true;
      //  }
      pFire[n]->Logic.MIN_GAS_AIR = MIN_GAS_AIR;
      pFire[n]->Logic.MIN_MAZUT_AIR = MIN_MAZUT_AIR;
      pFire[n]->Logic.MIN_WORK_GAS = MIN_WORK_GAS * iM3;
      pFire[n]->Logic.MIN_WORK_MAZUT = MIN_WORK_MAZUT;
      pFire[n]->Logic.MIN_ZAPAL = MIN_ZAPAL * iM3;
      double Real_gas = F_gas;
      double Real_mazut = F_mazut;
      if ( pFire[n]->GasAsMasut )
        {
        Real_gas += Real_mazut;
        Real_mazut = 0.;
        }
      pFire[n]->Logic.Work (
                pFire[n]->Flow_air,
                Real_gas, 
                F_pilot,
                Real_mazut, 
                F_steam,
                false );
      }
    CRadFors * pF = pFire[n];
    if ( pFors->Is_State ( CForsunka::IS_WORK_FIRE ))
      {
      *Горит[n] = 1;
      double k_T = pF->T0 / T_fire_nom;
      if ( k_T > 1. )
        k_T = 1.;
      *Интенсивность_пламени[n] = 50 * k_T * 100.;
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
      double k_pilot = 10 * F_pilot * M3 / F_pilot_nom;
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
    if ( pTopl_Gas && pTopl_Gas[n] )
      {
      if ( pPilot && pPilot[n] && pPilot[n]->Flow_mol > pTopl_Gas[n]->Flow_mol )
        pF->pComp_Gas = pPilot[n]->Get_Comp();
      else
        pF->pComp_Gas = pTopl_Gas[n]->Get_Comp();
      pF->Flow_gas = F_gas + F_pilot;
      pF->h_gas = pTopl_Gas[n]->h_Flow; 
      Topl_Gas += pF->Flow_gas;
      }
    else
      {
      pF->Flow_gas = 0.;
      pF->h_gas = 0.; 
      }
    if ( pFlow_Smoke_in && pFlow_Smoke_in[n] )
      {
      pF->pComp_Smoke_in = pFlow_Smoke_in[n]->Get_Comp();
      pF->Flow_Smoke_in = pFlow_Smoke_in[n]->Flow_mol;
      pF->h_Smoke_in = pFlow_Smoke_in[n]->h_Flow; 
      }
    else
      {
      pF->Flow_Smoke_in = 0.;
      pF->h_Smoke_in = 0.; 
      }
    if ( pAdd && pAdd[n] )
      {
      pF->pComp_Add = pAdd[n]->Get_Comp();
//  Испарение
	  if(!pF->pComp_Add)
		  return;
      pF->Flow_Add = pAdd[n]->Flow_mol;
      pF->h_Add = H_IG ( pAdd[n]->T, pF->pComp_Add );//pAdd[n]->h_Flow; 
      }
    else
      {
      pF->Flow_Add = 0.;
      pF->h_Add = 0.; 
      }
    if ( pMazut && pMazut[n] )
      {
      pF->pComp_Mazut = pMazut[n]->Get_Comp();
      pF->Flow_mazut_full = F_mazut;
      pF->h_mazut = pMazut[n]->h_Flow;
      }
    else
      {
      pF->Flow_mazut_full = 0.;
      pF->h_mazut = 0.;
      }
    if ( pMazutSteam && pMazutSteam[n] )
      {
      pF->Flow_steam = F_steam;
      pF->h_steam = pMazutSteam[n]->h_Flow;
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
    if ( pAir && pAir[n] )
      {
      P_Air[n] = pAir[n]->P - 1.;
      pF->Flow_air = ( pAir[n]->Flow_mol + N_Air ) * kFlowIn;
      F_Air_m3[n] = pAir[n]->Flow_mol * kFlowIn * M3;
      F_Air_podsos_m3[n] = N_Air * kFlowIn * M3;
      }
    else
      {
      F_Air_podsos_m3[n] = N_Air * kFlowIn * M3;
      pF->Flow_air = N_Air * kFlowIn;
      }
    if ( pF->Flow_air < 0. )
      pF->Flow_air = 0.;
    P_Air[n] = k_Air_Gas * pF->Flow_gas + k_Air_Mazut * pF->Flow_mazut_full;
    Air_summ += pF->Flow_air;
    if ( pAir && pAir[n] )
      pF->h_air = pAir[n]->h_Flow;
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
    if ( pSmoke && pSmoke[n] )
      P_Smoke[n] = pSmoke[n]->P - 1.;
    Pz_mm[n] = P_Под;
    }
  Air_Flow = Air_summ * M3;
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
  T_fire /= kFors;
  _T_Fire = T_fire;
//  double Q_1 = Q_summ / kProduct;
  for ( int n = 0; n < kProduct; n++ )
    {
    if ( pTube[n] == NULL || pTube[n]->pFlow_in == NULL )
      continue;
    pTube[n]->Q_Rad = W_Tube[n] * Q_summ;
    pTube[n]->Step( dt );
    }
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
  Flow_Smoke_Real = 0.;
  if ( MatrixModel )
    {
    CLEAR(Mu);
    double F_Mu[7];
    CLEAR(F_Mu);
    for ( int n = 0; n < kFors; n++ )
      {
      Flow_Smoke_Real += pSmoke[n]->Flow_mol;
#define CALC_MU(ID,pFlow) \
      if ( pFlow && pFlow[n]->ID_COMP && pFlow[n]->Flow_mol > F_Mu[ID] )\
        {\
        F_Mu[ID] = pFlow[n]->Flow_mol;\
        Mu[ID] = Mw ( Get_Comp( pFlow[n]->ID_COMP ));\
        }
      CALC_MU(N_SMOKE,pSmoke)
      CALC_MU(N_GAS,pTopl_Gas)
      CALC_MU(N_PILOT,pPilot)
      CALC_MU(N_MAZUT,pMazut)
      CALC_MU(N_STEAM,pMazutSteam)
      CALC_MU(N_AIR,pAir)
      if ( pFlow_Smoke_in && pFlow_Smoke_in[n]->ID_COMP && pFlow_Smoke_in[n]->Flow_mol > F_Mu[N_SMOKE_IN] )
        {                                    
        F_Mu[N_SMOKE_IN] = pFlow_Smoke_in[n]->Flow_mol;
        Mu_Smoke_in = Mw ( Get_Comp( pFlow_Smoke_in[n]->ID_COMP ));
        }
      }
    if ( kAir <= 0 )
      Mu[N_AIR] = Mw( ::pAir );
    }
  else
    {
    if ( pSmoke[0] )
      Flow_Smoke_Real = pSmoke[0]->Flow_mol;
    else
      Flow_Smoke_Real = 0.;
    }
  Smoke_Flow_Real = Flow_Smoke_Real * M3;
  double N = Norm( IsComp_Smoke, Cmol_Smoke );
  Smoke.Set ( Cmol_Smoke, IsComp_Smoke );
  pCompSmoke->Set ( IsComp_Smoke, Cmol_Smoke );
  nStep++;
  if ( Flow_Smoke > 1. )
    {
    H_Smoke = HF_Smoke / Flow_Smoke;
    double T_New = Smoke.TH ( H_Smoke, T_Pereval );
    double Ksi = dt * 60. / Tau_fire;
    double Old = 1. / ( 1. + Ksi );
    double New = Ksi * Old;
    T_Pereval = Old * T_Pereval + New * T_New;
    }
  else
    {
    //    T_Pereval = 0.99 * T_Pereval + 0.01 * T_air;
    double Ksi = dt * 60. / Tau_fire;
    double Old = 1. / ( 1. + Ksi );
    double New = Ksi * Old;
    T_Pereval = Old * T_Pereval + New * T_air;
    Smoke.Set ( pN2->Cmol );
    H_Smoke = Smoke.H( T_Pereval );
    }
  if ( MatrixModel )
    {
    for( int n = 0; n < kFors; n++ )
      {
      pSmoke[n]->ID_COMP = ID_CompSmoke;
      pSmoke[n]->pComp = pCompSmoke;
      pSmoke[n]->h_Flow = H_Smoke;
      }
    }
  else
    {
    pSmoke[0]->ID_COMP = ID_CompSmoke;
    pSmoke[0]->pComp = pCompSmoke;
    pSmoke[0]->h_Flow = H_Smoke;
    }
  Smoke_Flow = Flow_Smoke * M3;
  T_Smoke_Out = T_Pereval;
//
  if ( Calc_T_Wall )
    CalcT_Wall( dt );
  CalcPress( dt );
//
  double Q_Heat = 0.;
  double Q_HeatZ = 0.;
  for ( int n = 0; n < kProduct; n++ )
    {
    pTube[n]->TZ = T_Out_Z;
    pTube[n]->Calc_H = Calc_H;
    pTube[n]->Cp_Tube = Cp_Trub;
    Q_Heat +=  pTube[n]->pFlow_in->Flow_mol * ( pTube[n]->pFlow_out->h_Flow - pTube[n]->pFlow_in->h_Flow );
    Q_HeatZ +=  pTube[n]->pFlow_in->Flow_mol * ( pTube[n]->hOutZ - pTube[n]->pFlow_in->h_Flow );
    }
  if ( Q_Fire > 1000. )
    KPD = Q_Heat / Q_Fire * 100.;
  else
    KPD = 0.;
//  double KZ = Q_HeatZ / Q_Heat;
//  F_Z = Topl_Gas * KZ * M3;
	T_Pereval_u = T_Pereval * kT_fire;
	Т_на_перевале = T_Pereval_u;
//  Т_на_перевале_2 = T_Pereval;
//  Т_на_перевале_3 = T_Pereval;
    double T_rad = T_air + kT_fire * ( T_fire - T_air );
    Т_в_камере_радиации_1 = T_rad;
    Т_в_камере_радиации_2 = T_rad;
    double D = Delta_T_Pereval * 0.01;
    double Dr = 1. - D;
    double Dr1 = 1. + D;
SET_BP BreakPoint;
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
    double TF_1, TF_2, TF_3, TF_4, TF_5;
    SET_F(Т_на_перевале_1,1,TF_1)
    SET_F(Т_на_перевале_2,3,TF_2)
    SET_F(Т_на_перевале_3,5,TF_3)
    SET_F(Т_на_перевале_4,7,TF_4)
    SET_F(Т_на_перевале_5,9,TF_5)
    KKK();
    }
  else
    {
    SET_T(Т_на_перевале_1)
    SET_T(Т_на_перевале_2)
    SET_T(Т_на_перевале_3)
    SET_T(Т_на_перевале_4)
    SET_T(Т_на_перевале_5)
    SET_T(Т_на_перевале_6)
    }
  /////////////////////////////////////////////
//  double T_Rad = sqrt ( T_Max_Fire * T_Pereval );
  if (Smoke_Flow < 100)
  {
	O2_out = 21.;
	CO_out = 0.;
  }
  else
  {
	O2_out = Smoke.Cmol[G_O2] * 100.;
	CO_out = Smoke.Cmol[G_CO] * 100.;
  }

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
  double dP_Air_mm = 0.;
  if ( MatrixModel )
    {
    double _Pout = 0.;
    double _Pz = 0.;
    for ( int n = 0; n < kFors; n++ )
      {
      _Pout += P_Out[n];
      _Pz += Pz[n];
      }
    _Pout /= kFors;
    _Pz /= kFors;
    P_Верх = _Pout - 1.;
    P_Под = _Pz - 1;
    }
  else
    {
    double dF = Flow_Smoke - Flow_Smoke_Real;
    double dF_Smoke = dF;//Stab.Stab( dF );
    d_Flow_Smoke = dF_Smoke * M3;
    //  d_Flow_Smoke = dF_Smoke * M3;
/*
    P_Верх += k_P * dF_Smoke * dt;  //избыточное
    if ( P_Верх > 100e-4 )
      P_Верх = 100e-4;
*/
    P_Верх_мм = P_Верх * 1e4 ; //избыточное
	P_дыма_Па = P_Верх_мм * 9.80665 ;
  //////////////////////
    dP_Air_mm = H_Рад * ( 1. - ( T_air + TK ) / ( T_fire + TK ));
    P_Под = P_Верх - dP_Air_mm * 1e-4;// + k_dP_Rad * Flow_Smoke
    //
      //Разряжение_в_поде = P_Под_мм;
      //Разряжение_на_перевале = P_Верх_мм;
      //Разряжение_на_перевале_1 = P_Верх_мм;
      //Разряжение_на_перевале_2 = P_Верх_мм;
    }
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
  P_Верх_мм = P_Верх * 1e4;
  P_Под_мм = P_Под * 1e4;
  Разряжение_в_поде = P_Под_мм;
  Разряжение_на_перевале = P_Под_мм;
  Разряжение_на_перевале_1 = P_Под_мм;
  Разряжение_на_перевале_2 = P_Под_мм;
  Отрицательное_разряжение = -P_Под_мм;
  if ( CalcdP )
    {
    double dP = H_Calc * ( T_air + TK ) / ( kdP_Calc * ( T_Calc - T_air ) + T_air + TK );
    dP_Calc = dP - H_Calc;
    }
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
    pT->A_wall = A_trub;
    pT->B_wall = -A_trub * Tt;
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
  T_Wall = ( T_Wall + Ksi * Bq ) / ( 1. - Ksi * Aq );
  Т_стен = T_Wall;
//////////////////
  double Q = 0.;
  for ( int n = 0; n < kFors; n++ )
    {
    CRadFors * pF = pFire[n];
    pF->Q_wall = pF->A_wall * T_Wall + pF->B_wall;
    Q += pF->Q_wall;
    }
  Q_fors = Q;
  Q = 0.;
  for ( int n = 0; n < kProduct; n++ )
    {
    CRadTube * pT = pTube[n];
    pT->Q_wall = pT->A_wall * T_Wall + pT->B_wall;
    Q += pT->Q_wall;
    }
  Q_trub = Q;
  Q_air = Aa * T_Wall + Ba;
  Q_summ = Q_fors - Q_trub - Q_air;
  }
