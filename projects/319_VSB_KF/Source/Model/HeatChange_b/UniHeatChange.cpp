#include "stdafx.h"
#include "stdafx.h"
#include "UniHeatChange.h"
#include "math.h"
#include "Err.h"
#include "SR.h"
#include "Data.h"
#include "SetData.h"

//const double M_cal = 2.38845896627496E-1;
const double To_MWt =  1e-3 / 3600.;

CUniHeatChange::CUniHeatChange( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
, S_Trub_In("Trub_In",this)
, S_Vol_In("Vol_In",this)
, S_Trub_Out("Trub_Out",this)
, S_Vol_Out("Vol_Out",this)
, S_Trub_Lim("Trub_Lim",this)
, S_Vol_Lim("Vol_Lim",this)
  {
  Model = "UniHeatChange";
  M_Trub = 10.;
  M_Vol = 100.;
  pComp_Trub = NULL;
  pComp_Vol = NULL;
  Static = false;
  kAntiFlow = 0.;
  dQ = 1000.;
  Tau_Q = 1.;
  Teta = 0.;
  _dH_Trub = _dH_Vol = 0.;
  Slow_1 = 0.;
  Slow_2 = 0.;
  Расчет_через_Cp = false;
  Работа_через_Cp = true;
  Прямоток = false;
  WasLimit = false;
  EpsQ = 0.;
  EpsQfin = 0.;
  dT_Trub = 0.;
  dT_Vol = 0.;
  kIterQ = 0;
  dTdop = 0.01;
  kQ = 0.1;
  KS_Large = false;
  }

CUniHeatChange_W::CUniHeatChange_W()
  {
  memset ( this, 0, sizeof ( CUniHeatChange_W ));
  T_in_Trub = T_in_Vol = T_in_Vol_Air =  
    T_out_Trub = T_out_Vol = 15.;
  First_Step = true;
  }


bool CalcTeta ( double T1_in, double T1_out, double T2_in, double T2_out, double kPr, double & Teta )
  {
  TestTemp ( T1_in, T2_in, T1_out, T2_out );
  double dT1 = T1_in - T1_out;
  double dT2 = T2_in - T2_out;
  double dT = sqrt ( ( dT1 + dT2 ) * ( dT1 + dT2 ) - 4 * kPr * dT1 * dT2 );
  if ( dT < 1e-8 )
    {
    Teta = 0.;
    return true;
    }
  double Sch = T1_in + T1_out - T2_in - T2_out + dT;
  double Zn = T1_in + T1_out - T2_in - T2_out - dT;
  if ( fabs ( Zn ) < 1e-10 )
    Zn = 1e-10;
  if ( fabs ( Sch ) < 1e-10 )
    Sch = 1e-10;
  double Arg = fabs ( Sch / Zn );
  Teta = fabs ( dT / log ( Arg ));
  if ( !_finite ( Teta ))
    ASS( 0 );
  return true;
  }

bool CalcTeta ( double T1_in, double T1_out, double T2_in, double T2_out, double kPr, double & Teta, double & dTeta_T1, double & dTeta_T2 )
  {
  TestTemp ( T1_in, T2_in, T1_out, T2_out );
  double dT1 = T1_in - T1_out;
  double dT2 = T2_in - T2_out;
  double ddT2_T2 = -1.;
  double dT = sqrt ( ( dT1 + dT2 ) * ( dT1 + dT2 ) - 4 * kPr * dT1 * dT2 );
  double dT_T2 =   0.5 / dT * ( 2. * ( dT1 + dT2 ) - 4 * kPr * dT1 ) * ddT2_T2;
  if ( dT < 1e-8 )
    {
    Teta = 0.;
    return true;
    }
  double Sch = T1_in + T1_out - T2_in - T2_out + dT;
  double dSch_T2 = -1. + dT_T2;
  double Zn = T1_in + T1_out - T2_in - T2_out - dT;
  double dZn_T2 = -1. - dT_T2;
  if ( fabs ( Zn ) < 1e-10 )
    {
    Teta = 0.;
    dTeta_T1 = dTeta_T2 = 0.;
    return false;
    }
  if ( fabs ( Sch ) < 1e-10 )
    {
    Teta = 0.;
    dTeta_T1 = dTeta_T2 = 0.;
    return false;
    }
  double Min = false;
  if ( Sch < 0. )
    Min = !Min;
  if ( Zn < 0. )
    Min = !Min;
  double Arg = fabs ( Sch / Zn );
  double dArg_T2 = ( dSch_T2 * Zn - dZn_T2 * Sch ) / ( Zn * Zn );
  if ( Min )
    {
    dArg_T2 = -dArg_T2;
    }
  double logA = log ( Arg );
  double dlogA_T2 = dArg_T2 / Arg;
  Min = false;
  if ( dT < 0. )
    Min = !Min;
  if ( logA < 0. )
    Min = !Min;
  Teta = fabs ( dT / logA );
  dTeta_T2 = ( dT_T2 * logA - dlogA_T2 * dT ) / ( logA * logA );
  if ( Min )
    {
    dTeta_T2 = -dTeta_T2;
    }
  dTeta_T1 = -dTeta_T2;
  return true;
  }


void CUniHeatChange::CalcHeat( double dt )
  {
  double F_trub = fabs ( pIn_Trub->Flow_mol );
  double F_vol = fabs ( pIn_Vol->Flow_mol );
  if ( F_trub < 0.02 && F_vol < 0.01 )
    {
    T_out_Vol = T_out_Trub = 0.9 * T_out_Trub + 0.1 * T_air;
    S_Trub_Out.PT( pOut_Trub->P, T_out_Trub, pComp_Trub );
    S_Vol_Out.PT( pOut_Vol->P, T_out_Vol, pComp_Vol );
    Teta = 0.;
    Q_MWt = 0.;
    return;
    }
  if ( F_trub < 0.02 )
    {
    S_Vol_In.PH( pIn_Vol->P, pIn_Vol->h_Flow, pComp_Vol );
    T_in_Vol = S_Vol_In.T;
    T_out_Vol = T_out_Trub = S_Vol_In.T;
    S_Trub_Out.PT( pOut_Trub->P, T_out_Trub, pComp_Trub );
    S_Vol_Out.PH( pOut_Vol->P, S_Vol_In.h, pComp_Vol );
    Teta = 0.;
    Q_MWt = 0.;
    return;
    }
  if ( F_vol < 0.02 )
    {
    S_Trub_In.PH( pIn_Trub->P, pIn_Trub->h_Flow, pComp_Trub );
    T_in_Trub = S_Trub_In.T;
    T_out_Vol = T_out_Trub = S_Trub_In.T;
    S_Trub_Out.PH( pOut_Trub->P, S_Trub_In.h, pComp_Trub );
    S_Vol_Out.PT( pOut_Vol->P, T_out_Vol, pComp_Vol );
    Teta = 0.;
    Q_MWt = 0.;
    return;
    }
//
  ////
  S_Trub_In.PH( pIn_Trub->P, pIn_Trub->h_Flow, pComp_Trub );
  T_in_Trub = S_Trub_In.T;
  S_Vol_In.PH( pIn_Vol->P, pIn_Vol->h_Flow, pComp_Vol );
  T_in_Vol = S_Vol_In.T;
/////////////////////////////////////////////////////////////////////////////////////////
  if ( T_Trub_Z != 0. && !Err_Tz )
    {
    if ( Работа_через_Cp )
      Calc_Q_Trub_Z( );
    else
      CalcTrubZ( dt, T_Trub_Z, pIn_Vol, pIn_Trub, pOut_Vol, pOut_Trub,  
               &S_Vol_In,  &S_Trub_In, &S_Vol_Out, &S_Trub_Out );
//    if ( !Err_Tz )
      {
      T_out_Trub = S_Trub_Out.T;
      h_out_Trub = S_Trub_Out.h;
      T_out_Vol = S_Vol_Out.T;
      h_out_Vol = S_Vol_Out.h;
      return;
      }
    }
  if ( T_Vol_Z != 0. && !Err_Tz )
    {
    if ( Работа_через_Cp )
      Calc_Q_Vol_Z( );
    else
      CalcVolZ( dt, T_Vol_Z, pIn_Vol, pIn_Trub, pOut_Vol, pOut_Trub,  
              &S_Vol_In,  &S_Trub_In, &S_Vol_Out, &S_Trub_Out );
//    if ( !Err_Tz )
      {
      T_out_Trub = S_Trub_Out.T;
      h_out_Trub = S_Trub_Out.h;
      T_out_Vol = S_Vol_Out.T;
      h_out_Vol = S_Vol_Out.h;
      return;
      }
    }
//  if ( strstr ( ObjName, "T-31" ))
//    KKK();
  if ( T_in_Trub > T_in_Vol )
    {
    if ( Работа_через_Cp )
      CalcTemp_Q( dt );
    else
      {
      Params.dt = dt;
      Params.pHot_in = pIn_Trub;
      Params.pCool_in = pIn_Vol;
      Params.pHot_out = pOut_Trub;
      Params.pCool_out = pOut_Vol;
      Params.S_Hot_In = &S_Trub_In;
      Params.S_Cool_In = &S_Vol_In;
      Params.S_Hot_Out = &S_Trub_Out;
      Params.S_Cool_Out = &S_Vol_Out;
      CalcTemp( dt );
      }
    }
  else
    {
    if ( Работа_через_Cp )
      CalcTemp_Q( dt );
    else
      {
      Params.dt = dt;
      Params.pHot_in = pIn_Vol;
      Params.pCool_in = pIn_Trub;
      Params.pHot_out = pOut_Vol;
      Params.pCool_out = pOut_Trub;
      Params.S_Hot_In = &S_Vol_In;
      Params.S_Cool_In = &S_Trub_In;
      Params.S_Hot_Out = &S_Vol_Out;
      Params.S_Cool_Out = &S_Trub_Out;
      CalcTemp( dt );
      }
    }
  if ( Расчет_через_Cp )
    {
    CalcTemp_Q( dt );
    //CalcTemp_Cp( dt );
    }
/////////////////////////////////////////////////////////////////////////////////////////
  T_out_Trub = S_Trub_Out.T;
  h_out_Trub = S_Trub_Out.h;
  T_out_Vol = S_Vol_Out.T;
  h_out_Vol = S_Vol_Out.h;
//
  }


void CUniHeatChange::Calc( double dt )
  {
  //	*pOut_Trub = *pIn_Trub;
  //	*pOut_Vol = *pIn_Vol;
  if ( pOut_Trub )
    {
    pOut_Trub ->To_m3 = pIn_Trub->To_m3;
    pOut_Trub ->To_kg = pIn_Trub->To_kg;
    pOut_Trub ->ID_COMP = pIn_Trub->ID_COMP;
    pOut_Trub ->Flow_mol = pIn_Trub->Flow_mol;
    }
  //
  if ( pOut_Vol )
    {
    pOut_Vol ->To_m3 = pIn_Vol->To_m3;
    pOut_Vol ->To_kg = pIn_Vol->To_kg;
    pOut_Vol ->ID_COMP = pIn_Vol->ID_COMP;
    pOut_Vol ->Flow_mol = pIn_Vol->Flow_mol;
    }

  if ( pOut_Trub == NULL || pOut_Vol == NULL )
    {
    if ( pOut_Trub )
      *pOut_Trub = *pIn_Trub;
    if ( pOut_Vol )
      *pOut_Vol = *pIn_Vol;
    return;
    }
  //
  pComp_Trub = pIn_Trub->Get_Comp();
  pOut_Trub ->pComp = pIn_Trub->pComp = pComp_Trub;
  pComp_Vol = pIn_Vol->Get_Comp();
  pOut_Vol ->pComp = pIn_Vol->pComp = pComp_Vol;
  if ( pComp_Trub == NULL || pComp_Vol == NULL )
    return ;
  //                                                                
  CalcHeat( dt );
  pOut_Trub->T = T_out_Trub;
  pOut_Trub->h_Flow = S_Trub_Out.h;
  pOut_Vol->T = T_out_Vol;
  pOut_Vol->h_Flow = S_Vol_Out.h;
  KKK();
  }

int CUniHeatChange::SaveState( )
  {
  S_CLASS("TubeVolHeat_W",CUniHeatChange_W)
    return 0;
  }

int CUniHeatChange::RestoreState( char * StrName )
  {
  COPY_FIRST
  R_CLASS("TubeVolHeat_W",CUniHeatChange_W)
  return 1;
  }

int CUniHeatChange::SetData( int TypeData, void * pData )
  {
  if ( TypeData == sd_SetInitState )
    {
    T_in_Trub = T_in_Vol = T_air;
    T_out_Trub = T_out_Vol = T_air;
    return 1;
    }
  return 0;
  }

int CUniHeatChange::GetParams( char * )
  {
#include "IO_Parms.h"
    PARM( M_Trub, "Масса труб, кг" )
    PARM( M_Vol,  "Масса корпуса, кг" )
    PARM( T_in_Trub, "#Т на входе труб" )
    PARM( T_out_Trub, "#Т на выходе труб" ) 
    PARM( T_in_Vol, "#Т на входе объема" ) 
    PARM( T_out_Vol, "#Т на выходе объема" )
    PARM( S_Trub_In.E, "#Eps на входе труб" )
    PARM( S_Trub_Out.E, "#Eps на выходе труб" )
    PARM( S_Vol_In.E, "#Eps на входе объема" )
    PARM( S_Vol_Out.E, "#Eps на выходе объема" )
    PARM( pIn_Trub->Flow_mol, "#Поток труб моль" )
    PARM( pIn_Vol->Flow_mol, "#Поток объема моль" )
    PARM( Q_MWt, "#Теплопередача, МВт" )
    return 0;
  }

