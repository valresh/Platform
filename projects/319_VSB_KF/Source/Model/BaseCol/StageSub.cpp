#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol.h"

//??extern CGas::eTypeComp Type[K_GAS];

bool CStage_P::ShowX = false;
bool CStage_P::ShowY = false;
bool CStage_P::ShowPz = false;
bool CStage_P::ShowP_Y = false;
bool CStage_P::ShowPzX = false;
bool CStage_P::ShowVc_Comp = false;
bool CStage_P::ShowX_In = false;
bool CStage_P::ShowY_In = false;

CStage_Ptr::CStage_Ptr()
{
  memset ( this, 0, sizeof ( CStage_Ptr ));
}
const double Cmin = 1e-9;

void CStage::CompressXY( bool Used[K_GAS] )
{
  int k = 0;
  for ( int n = 0; n < LAST_GAS; n++ )
  {
    if ( IsComp[n] )
    {
      if ( X[n] > Cmin || Y[n] > Cmin )
      {
        k++;
        Used[n] = true;
      }
    }
  }
  if ( k == 0 )
  {
    IsComp[G_N2] = true;
    Y[G_N2] = 1.;
    X[G_N2] = 1.;
  }
}

void CStage::Add ( CExtIO * pSrc )
{
  ASS(kSrc < MAX_IN)
  Src[kSrc++] = pSrc;
}

int CStage::SaveState( )
{
  S_CLASS("W",CStage_W);
  return 0;
}

int CStage::RestoreState( char * StrName )
{
  //  return 1;
  COPY_FIRST
    R_CLASS("W",CStage_W);
  return 1;
}

int CStage::ShowParams( DWORD DataType, struct CShowData * pSD )
{
  switch ( DataType )
  {
    case sp_Actions:
      return 1;
  }
  return 0;
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
  CalcTom3 = false;
  Tau_SS = 30000.;
  H_SS = 0.;
  Eff = 1.;
  ExtHeat = 0.;
  Omega_IO = 10000.;
  Ksi_R_Rel = 1.;
  nHenry = 0;
  ЭтоКуб = false;
  kVoutIO = 1.;
  dP_nom = 0.01;
  V_nom = 1000.;
}

CStage::CStage( char * _Name, IBaseModel * pMain ) :	IBaseModel ( _Name, pMain )
{
  ObjName = _Name;
  Model = "Stage Colum";
  pUp = NULL;
  pDown = NULL;
  Yin = NULL;
  CLEAR(Name)
  //??	CLEAR(Type)
  //for ( int n = 0; n < LAST_GAS; n++ )
  //	{
  //	ASS(pGas[n]->Type != CGas::None );
  //	Type[n] = pGas[n]->Type;
  //	}
  CLEAR(Name)
  Level = 0.;
  Lev_old = 0.;
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
  CLEAR(P_Y)
  CLEAR(P_PzX)
  CLEAR(X)
  CLEAR(Y)
  CLEAR(Y_In)
  //
  P = 1.;
  Pz = 1.;
  T_In = T_liq = T_gas = 15.;
  Vol_Stage = 2.;
  S_Stage = 5.;
  Real_S_Stage = 10.;
  Real_H_Stage = 0.7;
  Real_Vol_Stage = 7.;
  double RT = RP * ( T_gas + TK );
  M_gas = P * Vol_Stage / RT;
  Y[G_N2] = 1.;
  int G = G_N2;
  M_liq = 0.;
  h_st = 0.05;
  k_L = 3000.;
  k_LV = 0.;
  Cp_gas = 30.;
  Cp_liq = 50.;
  Cp_korp = 2000.;
  P_src = 0.;
  Omega = 1e5;
  Alfa_Stage = 100.;
  k_GL = 1e6;
  Lev_min = 0.;
  Lev_max = 2.;
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
  //	pRegLev = NULL;
  M_min = 0.1;
  Eps_In.T = &T_In;
  Model = "Stage";
  kSrc = 0;
  kExtLout = 0.;
  FlowStopSlivm3 = 100.;
  Trace = false;
  Lev_Z = 0.;
  dLev_dt_max = 0.;
  Doliv = 0.;
  Sliv = 0.;
  Lev_Opt = 0.;
  Max_Flow_Add = 10000.;
  nStageBalance = -1;
  StageBalance = 0.;
  NameStageBalance = "";
  ConstStageBalance = 0.;
  FlowGasOut = 0.;
  FullStage = false;
  HenryStage = 1.;
  Eps_In.HenryStage = &HenryStage;
  Eps_In.dH_liq = &dH_liq;
  Eps_In.Gamma = Gamma;
  Eps_In.A_T_ext_heat = 0.;  // Q_ext =  A_T_ext_heat * T - B_T_ext_heat;
  Eps_In.B_T_ext_heat = 0.;
  Eps_In.T_ext_heat = 0.;
  for ( int n = 0; n < K_GAS; n++ )
    Gamma[n] = 1.;
  h_In = 2.0;
  k_Hydro_statatic = 0.0;
}

CExtHeat::CExtHeat( char * _Name, IBaseModel * _pMain ) :	IBaseModel ( _Name, _pMain )
  , Out( "Нагреватель", this )
{
  pPrevHeat = NULL;
  pNextHeat = NULL;
  pComp = NULL;
  nStage = 0;
  nGroup = -1;
  h_in = 0.;
  KS = 1000.;
  Cp = 30.;
}

bool CExtHeat::SetIn ( CFlow * _pFlow )
{
  pFlow = _pFlow;
  if ( pFlow == NULL )
    return false;
  h_in = pFlow->h_Flow;
  Out.PH ( pFlow );
  pComp = pFlow->Get_Comp();
  P = pFlow->P;
  Flow = pFlow->Flow_mol;
  return true;
}

bool CExtHeat::SetOut ( CFlow * _pFlow )
{
  pFlow = _pFlow;
  if ( pFlow == NULL )
    return false;
  if ( pPrev == NULL )
    return false;
  pComp = pPrevHeat -> pComp;
  h_in = pPrevHeat->Out.h;
  pFlow->h_Flow = h_in;
  FINITE(pFlow -> h_Flow)
  Out.PH( pFlow );
  pFlow->T = Out.T;
  return true;
}

/*
bool CExtHeat::CalcHeat( double T_liq, double & dH )
  {
  if ( pPrev == NULL )
    return false;
  pComp = pPrevHeat->pComp;
  P = pPrevHeat->P;
  KS = pPrevHeat->KS;
  h_in = pPrevHeat->Out.h;
  Out.PH_ext( P, h_in, KS, T_liq, pComp, dH );
  return true;
  }
*/

bool CExtHeat::SetCoef( double &A, double &B )
{
  pComp = pPrevHeat->pComp;
  P = pPrevHeat->P;
  KS = pPrevHeat->KS;
  h_in = pPrevHeat->Out.h;
  Flow = pPrevHeat->Flow;
  double FCp = Flow * Cp;
  double T_in = pPrevHeat->Out.T;
  //
  A = KS * FCp / ( FCp + KS );
  B = A * T_in;
  return true;
}

bool CExtHeat::CalcHeat( double T_ext )
{
  double FCp = Flow * Cp;
  double Tin = pPrevHeat->Out.T;
  double Tout = ( KS * T_ext + FCp * Tin ) / ( KS + FCp );
  double dh = Cp * ( Tout - Tin );
  double h_out = h_in + dh;
  Out.PH( P, h_out, pComp );
  //
  double dT = Out.T - Tout;
  double D = Tout - Tin;
  double dC = 0.1 * dT;
  if ( D < 0. )
    Cp += dC;
  else
    Cp -= dC;
  if ( Cp < 1. )
    Cp = 1.;
  if ( Cp > 1000. )
    Cp = 1000.;
  //
  return true;
}

int CExtHeat::SaveState( )
{
  S_CLASS("ExtHeat_W",CExtHeat_W)
  return 0;
}

int CExtHeat::RestoreState ( char * StrName )
{
  R_CLASS("ExtHeat_W",CExtHeat_W)
  return 1;
}

#define STAGE_PARM(Parm,NameParam) PARM( Parm, NameParam );

#include "IO_Parms.h"
//#define PROF if ( pCol->ShowProf ) {
//#define LONG if ( pCol->LongList ) {
#define PROF {
#define LONG {
#define EPROF }
#define ELONG }

void CStage::VisualParams( char * )
{
  //  if ( CParams::SaveAllParams )
  //    return;
  LONG
    TAB("Stop",3)
    PARM ( StopSrc, "#FF" );
  PARM ( StopLiq, "#Liq" );
  PARM ( StopGas, "#Gas" );
  //	PARM ( StopFlow, "#Flow" );
  PARM ( StopHeat, "#Heat" );
  ETAB
    ELONG
      //
      TAB("Параметры",3)
    LONG
    STAGE_PARM(Flow_Summ_In,"#Суммарный поток на вход")
    STAGE_PARM(L_In,"#Cуммарная жидкость на вход")
    STAGE_PARM(H_Summ_In,"#Cуммарная энтальпия на вход")
    STAGE_PARM(Eps,"#Eps")
    STAGE_PARM(Pz,"#Pz")
    STAGE_PARM(Eps_In.GammaPz,"#GammaPz")
    STAGE_PARM(M_gas,"#M_gas")
    STAGE_PARM(M_liq,"#M_liq")
    STAGE_PARM(Doliv,"#Doliv")
    STAGE_PARM(To_m3,"#To_m3")
    STAGE_PARM(dT_liq,"#dT_liq")
    STAGE_PARM(dT_gas,"#dT_gas")
    STAGE_PARM(h_gas_mix,"#h_gas_mix")
    STAGE_PARM(h_liq_mix,"#h_liq_mix")
    STAGE_PARM(dh_hydro,"#dh_hydro")
    ELONG
      STAGE_PARM(T_liq,"#Температура жидкости")
    STAGE_PARM(T_gas,"#Температура газа")
    STAGE_PARM(P,"#Давление")
    STAGE_PARM(Vin,"#Поток газа на тарелку")
    STAGE_PARM(Vout,"#Поток газа с тарелки")
    STAGE_PARM(Lin,"#Поток жидкости на тарелку")
    STAGE_PARM(Lout,"#Поток жидкости с тарелки")
    STAGE_PARM(V_Cond,"#Поток конденсации")
    //	STAGE_PARM(Hvap_Stage)
    STAGE_PARM(P_Pz,"#Неравновесность")
    STAGE_PARM(Level,"#Уровень на тарелке")
    STAGE_PARM(Lev_Z,"Заданный уровень на тарелке")
    LONG
    if ( G_H2S >= 0 )
  {
    PARM( Gamma[G_H2S], "#Gamma H2S" );
  }
  if ( G_CO2 >= 0 )
  {
    PARM( Gamma[G_CO2], "#Gamma CO2" );
  }
  if ( G_CO >= 0 )
  {
    PARM( Gamma[G_CO], "#Gamma CO" );
  }
  ELONG
    ETAB
    ///////////////////////////////////
    char Name[64];
  TAB("Режим",3)
  TAB("/X",5)
  PARM ( ShowX, "Выводить X" );
  if ( ShowX )
  {
    for ( int n = 0; n < LAST_GAS; n++ )
    {
      if ( IsComp[n] )
      {
        sprintf_s ( Name, 64, "#X - %s", pGas[n]->Name );
        PARM ( X[n], Name );
      }
    }
  }
  ETAB
    TAB("/Y",5)
    PARM ( ShowY, "Выводить Y" );
  if ( ShowY )
  {
    for ( int n = 0; n < LAST_GAS; n++ )
    {
      if ( IsComp[n] )
      {
        sprintf_s ( Name, 64, "#Y - %s", pGas[n]->Name );
        PARM ( Y[n], Name );
      }
    }
  }
  ETAB
    TAB("/Pz",5)
    PARM ( ShowPz, "Выводить Pz" );
  if ( ShowPz )
  {
    for ( int n = 0; n < LAST_GAS; n++ )
    {
      if ( IsComp[n] )
      {
        sprintf_s ( Name, 64, "#Pz - %s", pGas[n]->Name );
        PARM ( P_z[n], Name );
      }
    }
  }
  ETAB
    LONG
      TAB("/P_Y",5)
    PARM ( ShowP_Y, "Выводить P*Y" );
  if ( ShowP_Y )
  {
    for ( int n = 0; n < LAST_GAS; n++ )
    {
      if ( IsComp[n] )
      {
        sprintf_s ( Name, 64, "#P_Y - %s", pGas[n]->Name );
        PARM ( P_Y[n], Name );
      }
    }
  }
  ETAB
    TAB("/P_PzX",5)
    PARM ( ShowPzX, "Выводить Pz*X" );
  if ( ShowPzX )
  {
    for ( int n = 0; n < LAST_GAS; n++ )
    {
      if ( IsComp[n] )
      {
        sprintf_s ( Name, 64, "#P_PzX - %s", pGas[n]->Name );
        PARM ( P_PzX[n], Name );
      }
    }
  }
  ETAB
    ELONG
      TAB("/Конденсация",5)
    PARM ( ShowVc_Comp, "Выводить поток конденсации" );
  if ( ShowVc_Comp )
  {
    for ( int n = 0; n < LAST_GAS; n++ )
    {
      if ( IsComp[n] )
      {
        sprintf_s ( Name, 64, "#Vcond - %s", pGas[n]->Name );
        PARM ( Vc_Comp[n], Name );
      }
    }
  }
  ETAB
    LONG
      TAB("/X_In",5)
    PARM ( ShowX_In, "Выводить X_In" );
  if ( ShowX_In )
  {
    for ( int n = 0; n < LAST_GAS; n++ )
    {
      if ( IsComp[n] )
      {
        sprintf_s ( Name, 64, "#X_In - %s", pGas[n]->Name );
        PARM ( X_In[n], Name );
      }
    }
  }
  ETAB
    TAB("/Y_In",5)
    PARM ( ShowY_In, "Выводить Y_In" );
  if ( ShowY_In )
  {
    for ( int n = 0; n < LAST_GAS; n++ )
    {
      if ( IsComp[n] )
      {
        sprintf_s ( Name, 64, "#Y_In - %s", pGas[n]->Name );
        PARM ( Y_In[n], Name );
      }
    }
  }
  ETAB
    ELONG
    ETAB
}

int CStage::GetParams( char * )
{
  //	TAB("Параметры тарелки")
  PARM ( Name, "#Имя" )
  PARM_L ( StageType, "Тип тарелки", "Колпачковая;Насадка;Ситчатая;Глухая;Куб;")
  PARM ( Vol_Stage, "Объем" )
  PARM ( S_Stage, "Площадь" )
  //  PARM ( Omega, "Проходимость" )
  PARM ( dP_nom,"Номинальный перепад на тарелке")
  PARM ( V_nom,"Номинальный поток через тарелку")
  PARM ( h_st, "Высота перелива" )
  PARM ( k_L, "Скорость слива" )
  PARM ( kExtLout, "Максимальная скорость слива с ситчатой тарелки" );
  PARM ( FlowStopSlivm3, "Поток останова слива")
  LONG
    PARM ( k_LV, "Образование пены" )
    ELONG
      PARM ( Cp_korp, "Теплоёмкость корпуса" )
    PARM_D ( k_GL, "Теплообмен газ-жидкость", 1e6 )
    //  PARM ( Alfa_Stage, "Alfa" );
    PARM ( k_Cool, "Охлаждение тарелки" );
  //  PARM_D ( Gamma_1_in_2, "Gamma_1 во 2", 50. )
  //  PARM_D ( Gamma_2_in_1, "Gamma_2 в 1", 50. )
  PARM_D ( Lev_min, "Минимальный уровень", 0. )
  PARM_D ( Lev_max, "Максимальный уровень", 2. )
  LONG
    PARM_D ( UseGydroIndP, "Учитывать гидростат в перепаде", true )
    ELONG
      PARM_D ( dP_Hydro_Static, "Статический гидростат", 0. )
    PARM_D ( dP_Hydro_Level, "Гидростат от уровня", 0. )
    PARM_D ( Eff, "Заданный КПД тарелки", 1. )
    LONG
      PARM ( Ksi_R_Rel, "Отн. скорость испарения, с" )
    ELONG
      //
      LONG
        TAB( "Дополнение",5 )
    PARM ( P_src, "P источника" )
    //  PARM ( HenryStage, "HenryStage" );
    //  PARM ( Hvap_Stage, "Hvap_Stage" );
    PARM ( Cp_gas, "Теплоемкость газа" )
    PARM ( Cp_liq, "Теплоемкость жидкости" )
    PARM ( T_liq_Z, "Заданныа температура жидкости" );
  //  PARM ( dH_liq_Z, "dH_liq_Z" );
  //  PARM ( dH_gas_Z, "dH_gas_Z" );
  PARM ( dLev_dt_max, "Макс скорость изменения уровня" );
  PARM ( Lev_Opt, "Опт. уровень" );
  PARM ( Max_Flow_Add, "Макс. долив, кг" );
  PARM_D ( Tau_SS, "Теплообмен т-т", 30000. )
  PARM ( nStageBalance, "Номер компоненты для баланса" )
  PARM ( NameStageBalance, "#Компонента для баланса" )
  PARM ( StageBalance, "Баланс для заданной компоненты" )
  PARM ( ConstStageBalance, "Поток в источнике" )
  ETAB
    ELONG
      VisualParams( "" );
  LONG
    Eps_In.GetParams( this );
  ELONG
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
  //bool PrtH = false;
  //if ( PrtH )
  //  {
  //  PrtH = false;
  //  void * F = Fopen("Col.csv","wt");
  //  double h = 0;
  //  for ( int n = 0; n < LAST_GAS; n++ )
  //    {
  //    if ( IsComp[n] )
  //      {
  //      Fprintf ( F, "%s;%7.1lf;%7.5lf;\n", pGas[n]->Name, h_gas[n], Y[n] );
  //      h += h_gas[n] * Y[n];
  //      }
  //    }
  //  Fprintf ( F, "h;%7.3lf;;\n", h );
  //  Fclose ( F );
  //  }
  CalcStageBalance = false;
  StageBalance = SummStageBalance;
  SummStageBalance = 0.;
  nStageBalance = pCol->n_Comp_Balance;
  if ( nStageBalance >= 0 &&  nStageBalance < LAST_GAS )
  {
    CalcStageBalance = true;
    NameStageBalance = pGas[nStageBalance]->Name;
  }
  else
  {
    StageBalance = 0.;
    NameStageBalance = "";
    CalcStageBalance = true;
  }
  H_Summ_In = 0.;
  dH_liq_Summ = 0.;
  Flow_Summ_In = 0.;
  if ( StopSrc )
  {
    //    Fsend ( "FeedFlash.txt", "\n1;%8.1lf;%8.1lf;%8.1lf;\n", Flow_Summ_In, H_Summ_In, dH_liq_Summ );
    BREAK
  }
  CLEAR(Summ_In)
  double T_in_max = -273.;
  double T_in_min = 2000.;
  ///////////////////////////////////////////////////
  // Источники
  if ( pCol->Use_min_max )
    pCol->Calc_Flow_T = true;
  double GasOut = 0.;
  int c_Del = pCol->n_Comp_Delete;
  for ( int s = 0; s < kSrc; s++ )
  {
    CExtIO	* pSrc = Src[s];
    CFlow * pF = pSrc->pFlow;
    if ( pF == NULL )
      continue;
    double F = pF->Flow_mol;
    if ( F <= 0. )
    {
      if ( Numb != 1 && pSrc->Gas )
        GasOut -= F;
      continue;
    }
    double T_in;
    if ( pCol->Calc_Flow_T )
    {
      pSrc->Src.PH( pF );
      T_in = pSrc->Src.T;
    }
    else
      T_in = pF->T;
    if ( T_in_max < T_in )
      T_in_max = T_in;
    if ( T_in_min > T_in )
      T_in_min = T_in;
    CComp * pC = pF->Get_Comp();
    if ( pC == NULL )
      continue;
    Flow_Summ_In += F;
    double h = pF->h_Flow;
    if ( Src[s]->kdH != 0. && T_liq > T_in )
    {
      int K = (int)( Src[s]->kdH * 100. + 0.5 );
      double kdH = (double)( K / 100 );
      double dTm = (double)( K % 100 );
      double dT = T_liq - pF->T;
      if ( dT > dTm )
        dT = dTm;
      h += kdH * dT;
    }
    H_Summ_In += F * h;
    bool * IsCompF = pC->IsComp;
    double * CmolF = pC->Cmol;
    for ( int i = 0; i < LAST_GAS; i++ )
    {
      //if ( pTypeComp[i] & _Solid_ )
      //  {
      //  IsComp[i] = false;
      //  continue;
      //  }
      if ( pCol->CtrlComp[i] )
      {
        if ( pCol->CtrlComp[i] == 1 )
        {
          IsComp[i] = false;
          continue;
        }
        if ( pCol->CtrlComp[i] == 2 )
        {
          IsComp[i] = false;
          ASS( F * CmolF[i] < 0.01 )
          continue;
        }
      }
      if ( IsCompF[i] && i != c_Del )
      {
        IsComp[i] = true;
        Summ_In[i] += F * CmolF[i];
        FINITE(Summ_In[i])
      }
    }
  }
  if ( GasOut > 0. )
    KKK();
  ///////////////////////////////////////////////////
  if ( StopSrc )
  {
    //    Fsend ( "FeedFlash.txt", "2;%8.1lf;%8.1lf;%8.1lf;\n", Flow_Summ_In, H_Summ_In, dH_liq_Summ );
    BREAK
  }
  // Сверху
  double h_u_liq = 0.;
  if ( pUp && pUp->Lout > 0. )
  {
    double F = pUp->Lout;
    dH_liq_Summ += F * pUp->dH_liq;
    double * uX = pUp -> X;
    double * uh_Liq = pUp -> h_liq;
    Flow_Summ_In += F;
    if ( T_in_max < pUp->T_liq )
      T_in_max = pUp->T_liq;
    if ( T_in_min > pUp->T_liq )
      T_in_min = pUp->T_liq;
    for ( int i = 0; i < LAST_GAS; i++ )
    {
      if ( IsComp[i] )
      {
        Summ_In[i] += F * uX[i];
        FINITE(Summ_In[i])
        H_Summ_In += F * uX[i] * uh_Liq[i];
        h_u_liq += uX[i] * uh_Liq[i];
      }
    }
    //    double D_1 = h_u_liq / pUp->h_liq_mix;
    //    double D_2 = h_u_liq / pUp->Eps_In.h_liq;
    //    KKK();
  }
  if ( StopSrc )
  {
    //    Fsend ( "FeedFlash.txt", "3;%8.1lf;%8.1lf;%8.1lf;\n", Flow_Summ_In, H_Summ_In, dH_liq_Summ );
    BREAK
  }
  ///////////////////////////////////////////////////
  // Снизу
  double Ksi = pCol->Ksi_R * Ksi_R_Rel * 1e-2;
  double h_d_gas = 0.;
  if ( pDown && pDown->Vout > 0. )
  {
    double Vout = pDown->Vout;
    Flow_Mix = Vout * Ksi;
    Vout -= Flow_Mix;
    double F = Vout * Eff;
    double * dY = pDown -> Y;
    double * dh_Gas = pDown -> h_gas;
    Flow_Summ_In += F;
    if ( T_in_max < pDown->T_gas )
      T_in_max = pDown->T_gas;
    if ( T_in_min > pDown->T_gas )
      T_in_min = pDown->T_gas;
    for ( int i = 0; i < LAST_GAS; i++ )
    {
      if ( IsComp[i] )
      {
        Summ_In[i] += F * dY[i];
        FINITE(Summ_In[i])
        H_Summ_In += F * dY[i] * dh_Gas[i];
        h_d_gas += dY[i] * dh_Gas[i];
      }
    }
    //double D = h_d_gas / pDown->Eps_In.h_gas;
  }
  // Испарение
  if ( StopSrc )
  {
    //    Fsend ( "FeedFlash.txt", "4;%8.1lf;%8.1lf;%8.1lf;\n", Flow_Summ_In, H_Summ_In, dH_liq_Summ );
    BREAK
  }
  //  Ksi = 0.;
  //  double Ksi = pCol->Ksi_R * Ksi_R_Rel * 1e-2;
  //  double dM_Mix = M_liq * Ksi;
  //  M_liq -= dM_Mix;
  //  Flow_Mix  = dM_Mix / dt;
  if ( Flow_Mix > 0. )
  {
    double F = Flow_Mix;
    Flow_Summ_In += F;
    dH_liq_Summ += F * dH_liq;
    for ( int i = 0; i < LAST_GAS; i++ )
    {
      if ( IsComp[i] )
      {
        Summ_In[i] += F * X[i];
        FINITE(Summ_In[i])
        H_Summ_In += F * X[i] * pGas[i]->H_l( T_liq );
      }
    }
  }
  if ( StopSrc )
  {
    //    Fsend ( "FeedFlash.txt", "5;%8.1lf;%8.1lf;%8.1lf;\n", Flow_Summ_In, H_Summ_In, dH_liq_Summ );
    BREAK
  }
  if ( CalcStageBalance )
  {
    if ( nStageBalance >= 0 )
      ConstStageBalance = SummStageBalance = Summ_In[nStageBalance];
    else
      ConstStageBalance = SummStageBalance = Flow_Summ_In;
  }
  /********/
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
  Norm ( IsComp, Summ_In );
  double h = ( H_Summ_In + dH_liq_Summ ) / Flow_Summ_In;
  if ( StopSrc )
  {
    //    Fsend ( "FeedFlash.txt", "6;%8.1lf;\n", h );
    BREAK
  }
  ///////////////////////////////////////////////////
  // Eps
  double Pe = P;                                                                                              
  if ( pCol->SetPsrc > 0 )
    P_src = P;
  if ( pCol->SetPsrc < 0 )
    P_src = 0.;
  if ( P_src > 0. )
  {
    double dP = P - P_src;
    if( dP < 0. )
      dP *= pCol->kSrcMin;
    else
      dP *= pCol->kSrcPlus;
    Pe = P_src + dP;
    if ( Pe < P_src && P_src > 0.3 )
      Pe = P_src;
  }
  //
  //if ( nHenry < 0 )
  //  nHenry = 0;
  //if ( nHenry > MAX_HENRY )
  //  nHenry = MAX_HENRY;
  Eps_In.Henry = pCol->Henry;
  //if ( nHenry == 0 )
  //Eps_In.Henry = pCol->Henry;
  //else
  //  Eps_In.Henry = pCol->Henry_Stage[nHenry-1];
  if ( Eps_In.WaterCol )
  {
    switch ( pCol->AdsType )
    {
      case AdsH2S_MEA:
        {
          pCol->pAdsGas_1->CalcGamma( T_liq, IsComp, X, Gamma );
          break;
        }
      case AdsCO2_MEA:
        {
          //if ( pCol->UseCO2Appr )
          //  {
          //  int g1 = G_CO2;
          //  int g2 = G_MDEA;
          //  if ( g1 > 0 && g2 > 0 )
          //    Gamma[G_CO2] = CMDEA_CO2::Gamma_CO2 ( T_liq, X[G_CO2], X[G_MDEA] );// * pCol->HenryH2S;
          //  }
          //else
          pCol->pAdsGas_1->CalcGamma( T_liq, IsComp, X, Gamma );
          break;
        }
      case AdsCO2_K2CO3:
        {
          int g1 = G_CO2;
          int g2 = G_K2CO3;
          pCol->pAdsGas_1->CalcGamma( T_liq, IsComp, X, Gamma );
          //        pCol->pAdsGas_2->CalcGamma( T_liq, IsComp, X, Gamma );
          break;
        }
    }
  }
  if ( StopSrc )
    BREAK
      if ( pCalcChemEx )
    {
      double k_Flow, dh;
      (*pCalcChemEx)( dt, *Eps_In.T, P, IsComp, Summ_In, pCalcChemParam, k_Flow, dh );
      Flow_Summ_In *= k_Flow;
      h += dh;
    }
  Eps_In.A_T_ext_heat /= Flow_Summ_In;
  Eps_In.B_T_ext_heat /= Flow_Summ_In;
  //if ( pCol->Use_min_max )
  //  {
  //  Eps_In.pT_max = NULL;
  //  Eps_In.pT_min = NULL;
  //  Eps_In.T_max = T_in_max;
  //  Eps_In.T_min = T_in_min;
  //  }
  Eps_In.PH( Pe, h, Summ_In, T_In );
  Eps = Eps_In.E;
  if ( !pCol->Use_min_max )
  {
    if ( *Eps_In.T > T_in_max || *Eps_In.T < T_in_min )
      KKK();
  }
  if ( StopSrc )
  {
    //    Fsend ( "FeedFlash.txt", "7;%8.4lf;%8.4lf;\n", Eps_In.E, Eps_In.T );
    BREAK
  }
  V_In = Flow_Summ_In * Eps;
  L_In = Flow_Summ_In - V_In;
  memmove ( X_In, Eps_In.X_W, CMOL_GAS );
  memmove ( Y_In, Eps_In.Y_W, CMOL_GAS );
  memmove ( Vc_Comp, X_In, CMOL_GAS );
  memmove ( h_Liq_In, Eps_In.h_Liq, CMOL_GAS );
  memmove ( P_z, Eps_In.P_sat, CMOL_GAS );
  if ( pCalcChem )
    (*pCalcChem) ( dt, this, pCalcChemParam );
  double T_gas_new = T_gas;
  if ( pDown )
  {
    double V_Down = pDown->Vout;
    double W_In = ( 1. - pCol->Eps_Gas ) * V_In;
    double W_Down = pCol->Eps_Gas * V_Down;
    double W = ( W_In + W_Down );
    if ( W > 0.1 )
      T_gas_new = ( W_In * T_In + W_Down * pDown->T_gas ) / W;
    else
      T_gas_new = T_In;
  }
  else
    T_gas_new = T_In;
  //
  double KsiT = dt * 60. / pCol->Tau_gas;
  T_gas = ( T_gas + KsiT * T_gas_new ) / ( 1. + KsiT );
  //  if ( Trace )
  //    {
  //   Fsend ( "Trace.txt", "%d)Pe=%6.4lf;h=%6.0lf;Eps=%6.4lf", Numb, Pe, h, Eps );
  //    }
  ///////////////////////////////////////////////////
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
  //	double V_In = Flow_Summ_In * Eps_In.E;
  double dM = L_In * dt;
  if ( pCol->QuickX )
  {
    for ( int i = 0; i < LAST_GAS; i++ )
    {
      if ( IsComp[i] )
        X[i] = 0.9 * X[i] + 0.1 * X_In[i];
    }
  }
  else
  {
    for ( int i = 0; i < LAST_GAS; i++ )
    {
      if ( IsComp[i] )
        X[i] = M_liq * X[i] + dM * X_In[i];
    }
  }
  Norm ( IsComp, X );
  FINITE(X[0])
  //
  int iNo = G_H2O;
  if ( Eps_In.WaterCol )
    iNo = -1;
  double Ps = 0.;
  double PsE = 0.;
  for ( int i = 0; i < LAST_GAS; i++ )
  {
    if ( IsComp[i] && i != iNo )
    {
      P_PzX[i] = X[i] * Eps_In.P_sat[i];
      Ps += P_PzX[i];
      PsE += X_In[i] * Eps_In.P_sat[i];
    }
  }
  Pz = Ps;
  if ( Eps_In.WaterCol )
  {
    P_Pz = P / Pz;
    double P_PzE = P / PsE;
    KKK();
  }
  else
  {
    if(Pz != 0.0)
      P_Pz = P * ( 1. - Y[G_H2O] ) / Pz;
    else
      P_Pz = 0.0;
    //double P_PzE = P * ( 1. - Y[G_H2O] )/ PsE;
    KKK();
  }
  //////////////////////////////////////////////////////////
  M_liq += dM;
  double Ys = 0.;
  double hs = 0.;
  if ( pDown && pDown->Vout > 0. && Eff < 1. )
  {
    double Eps_D = 1. - Eff;
    double Eps_F = Eff;
    double F_D = pDown->Vout * Eps_D;
    //    V_In += F_D;
    double * h_D = pDown->h_gas;
    double * Y_D = pDown -> Y;
    if ( CalcStageBalance )
    {
      if ( nStageBalance >= 0 )
      {
        ConstStageBalance += F_D * Y_D[nStageBalance];
        SummStageBalance += F_D * Y_D[nStageBalance];
      }
      else
      {
        ConstStageBalance += F_D;
        SummStageBalance += F_D;
      }
    }
    double Ksi = 0.1;
    double Old = 1. / ( 1. + Ksi );
    double New = Ksi * Old;
    double E_s = 1.;
    double E_d = 0.;
    double V_S = V_In + F_D;
    if ( V_S > 1e-3 )
    {
      double N = 1. / ( V_In + F_D );
      E_s = V_In * N;
      E_d = F_D * N;
    }
    V_In = V_S;
    for ( int i = 0; i < LAST_GAS; i++ )
    {
      if ( IsComp[i] )
      {
        Y[i] = Old * Y[i] + New * ( E_s * Eps_In.Y_W[i] + E_d * Y_D[i] );
        h_gas[i] = Old * h_gas[i] + New * ( E_s * Eps_In.h_Gas[i] + E_d * h_D[i] );
        Ys += Y[i];
        hs += Y[i] * h_gas[i];
        FINITE(Y[i])
        P_Y[i] = P * Y[i];
      }
    }
  }
  else
  {
    double Old = 0.5;
    double New = 0.5;
    for ( int i = 0; i < LAST_GAS; i++ )
    {
      if ( IsComp[i] )
      {
        Y[i] = Old * Y[i] + New * Y_In[i];
        h_gas[i] = Old * h_gas[i] + New * Eps_In.h_Gas[i];
        Ys += Y[i];
        hs += Y[i] * h_gas[i];
        FINITE(Y[i])
        P_Y[i] = P * Y[i];
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
  if ( CalcStageBalance )
  {
    //double In = Flow_Summ_In * Summ_In[nStageBalance] + pDown->Vout * ( 1. - Eff ) * pDown->Y[nStageBalance];
    //double L = L_In * X[nStageBalance];
    //double V = V_In * Y[nStageBalance];
    //double D = In - L - V;
    //double L2 = L_In * Eps_In.X_W[nStageBalance];
    //double V_Int = Flow_Summ_In * Eps;
    //double V2 = V_Int * Eps_In.Y_W[nStageBalance];
    //double D2 = Flow_Summ_In * Summ_In[nStageBalance] - L2 - V2;
    //double DL = L - L2;
    //double DV = V - V2;
    //M_liq;
    //double dL = ( L_In - Lout ) * X[nStageBalance];
    KKK();
  }

  h_gas_mix = hs;
  if ( !pCol->CaclOnVol )
    Vout += pCol->kInertVout * ( V_In - Vout - GasOut );
  if ( Vout < 0. )
    Vout = 0.;
  if ( pDown )
  {
    V_Cond = pDown->Vout - Vout;
    V_Cond_Gas = 0.9 * V_Cond_Gas + 0.1 * ( V_In - pDown->Vout );
  }
  else
  {
    V_Cond_Gas = 0.9 * V_Cond_Gas + 0.1 * V_In;
    V_Cond = 0.;
  }
  if ( pDown && pCol->CalckVout )
  {
    double K = Vout / pDown->Vout;
    kVoutIO += 0.01 * ( K - kVoutIO );
  }
  //
  double h_liq_mix = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
  {
    if ( IsComp[n] )
      h_liq_mix += X[n] * h_liq[n];
  }
  //double D = H_Summ_In - Vout * h_gas_mix - L_In * h_liq_mix;
  //double P = D / H_Summ_In;
  if ( CalcStageBalance )
  {
    if ( nStageBalance >= 0 )
    {
      if ( pUp )
        SummStageBalance -= Vout * Y[nStageBalance];
      if ( pDown )
        SummStageBalance -= Lout * X[nStageBalance];
    }
    else
    {
      if ( pUp )
        SummStageBalance -= Vout;
      if ( pDown )
        SummStageBalance -= Lout;
    }
  }
}

void CStage::CorrData( )
{
  bool CorrY = false;
  for ( int n = 0; n < LAST_GAS; n++ )
  {
    if ( !_finite(X[n]) || X[n] < 0. )
      X[n] = 0.;
    if ( !_finite(Y[n]) || Y[n] < 0. )
      Y[n] = 0.;
    else
      CorrY = true;
  }
  if ( !CorrY )
  {
    IsComp[G_N2] = true;
    Y[G_N2] = 1.;
  }
}
