#include "stdafx.h"
#define DLL_UniHeat
#include "UniHeat.h"
#include "math.h"
#include "Err.h"
#include "SR.h"
#include "SetData.h"


CHeat_1_flow::CHeat_1_flow( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain ),
S_Work_In("Work_In",this),
S_Const_In("Const_In",this),
S_Work_Out("Work_Out",this),
S_Const_Out("Const_Out",this)
  {
  Model = "Heat_1_flow";
  kGas_Work = 1.;
  kGas_Const = 1.;
  kCp_Work = 1.;
  kCp_Const = 1.;
  Cp_Corp_Work = 1.;
  Cp_Corp_Const = 1.;
  pComp_Work = NULL;
  pComp_Const = NULL;
  Static = false;
  CalcdTdFc = false;
//
  }

CHeat_1_flow_W::CHeat_1_flow_W()
  {
  memset ( this, 0, sizeof ( CHeat_1_flow_W ));
  T_in_Work = T_in_Const = T_out_Work = T_out_Const = 15.;
  }


void CHeat_1_flow::Calc( double dt )
  {
  //	*pOut_Work = *pIn_Work;
  //	*pOut_Const = *pIn_Const;
  if ( pOut_Work )
    {
    pOut_Work ->To_m3 = pIn_Work->To_m3;
    pOut_Work ->To_kg = pIn_Work->To_kg;
    pOut_Work ->ID_COMP = pIn_Work->ID_COMP;
    pOut_Work ->Flow_mol = pIn_Work->Flow_mol;
    }
  //
  if ( pOut_Const )
    {
    pOut_Const ->To_m3 = pIn_Const->To_m3;
    pOut_Const ->To_kg = pIn_Const->To_kg;
    pOut_Const ->ID_COMP = pIn_Const->ID_COMP;
    pOut_Const ->Flow_mol = pIn_Const->Flow_mol;
    }

  if ( pOut_Work == NULL || pOut_Const == NULL )
    {
    if ( pOut_Work )
      *pOut_Work = *pIn_Work;
    if ( pOut_Const )
      *pOut_Const = *pIn_Const;
    return;
    }
  //
  pComp_Work = pIn_Work->Get_Comp();
  pOut_Work ->pComp = pIn_Work->pComp = pComp_Work;
  pComp_Const = pIn_Const->Get_Comp();
  pOut_Const ->pComp = pIn_Const->pComp = pComp_Const;
  if ( pComp_Work == NULL || pComp_Const == NULL )
    return ;
  //
  S_Work_In.PH( pIn_Work->P, pIn_Work->h_Flow, pComp_Work );
  T_in_Work = S_Work_In.T;
  double ks_Work = kGas_Work * S_Work_In.E + 1. - S_Work_In.E;
  //
  double h_in_Work = pIn_Work->h_Flow;
  double h_in_Const = pIn_Const->h_Flow;
  S_Const_In.PH( pIn_Const->P, pIn_Const->h_Flow, pComp_Const );
  T_in_Const = S_Const_In.T;
  double ks_Const = kGas_Const * S_Const_In.E + 1. - S_Const_In.E;
  double ks = 2. * KS / ( 1. / ks_Work + 1. / ks_Const );
  //
  bool OK = false;
  if ( CalcdTdFc )
    {
    Newton( dt );
    }
  else
    {
    Newton( dt );
    }
  Section( dt );
  BiSection( dt );
  T_in_Work = S_Work_In.T;
  T_out_Work = S_Work_Out.T;
  T_out_Const = S_Const_Out.T;
  //if ( !Newton( dt ) )
  //  {
  //  if ( !Section( dt ))
  //    {
  //    ASS(BiSection( dt ))
  //    }
  //  }
  //
/***************
  if ( Static )
    {
    T_out_Work = T_out_new_trub;
    dT_Corp_Work = 0.;
    T_out_Const = T_out_new_vol;
    dT_Corp_Const = 0.;
    }
  else
    {
    Ksi_Work = 1000.;
    double Cp_trub = Cp_Corp_Work;
    if ( Cp_trub < 0. )
      {             
      Ksi_Work = 1000.;
      Cp_trub = -Cp_trub;
      }
    if ( Fcp_Work * dt * Ksi_Work > Cp_trub )
      Ksi_Work = Cp_trub / ( Fcp_Work * dt );
    T_out_Work = ( T_out_new_trub + Ksi_Work * T_out_old_trub ) / ( 1. + Ksi_Work );
    dT_Corp_Work = Ksi_Work * ( T_out_Work - T_out_old_trub );
    //
    Ksi_Const = 10000.;
    if ( Fcp_Const * dt * Ksi_Const > Cp_Corp_Const )
      Ksi_Const = Cp_Corp_Const / ( Fcp_Const * dt );
    T_out_Const = ( T_out_new_vol + Ksi_Const * T_out_old_vol ) / ( 1. + Ksi_Const );
    dT_Corp_Const = Ksi_Const * ( T_out_Const - T_out_old_vol );
    }
  //
  pOut_Work->T = T_out_Work;
  if ( T_out_Const > 1100. )
    T_out_Const = 1100.;
  pOut_Const->T = T_out_Const;
********************/
  //
  }

bool CHeat_1_flow::Teta ( double h_Work_out, double & Teta )
  {
  double dT_out = S_Work_Out.T - S_Const_In.T;
  double dT_in = S_Work_In.T - S_Const_In.T;
  double Log = log ( dT_in / dT_out );
  if(Log)
	  Teta = ( dT_in - dT_out ) / Log;
  return true;
  }

bool CHeat_1_flow::Finc ( double h_Work_out, double & F )
  {
  S_Work_Out.PH( pOut_Work->P, h_Work_out, pIn_Work->Get_Comp());
  double dT_out = S_Work_Out.T - S_Const_In.T;
  double dT_in = S_Work_In.T - S_Const_In.T;
  if ( dT_out <= 0. && dT_in >= 0. )
    return false;
  if ( dT_out >= 0. && dT_in <= 0. )
    return false;
  double Tet;
  if ( !Teta ( h_Work_out, Tet ))
    return false;
  F = pIn_Work->Flow_mol * ( S_Work_In.h - S_Work_Out.h ) - KS * Tet;
  return true;
  }

bool CHeat_1_flow::Newton( double dt )
  {
  int kIter = 0;
  double dh_out = 1;
  double h_out = h_Work_out;
  while ( fabs ( dh_out ) > 1e-6 && kIter++ < 10 )
    {
    double F0, F1, dF;
    if ( !Finc ( h_out, F0 ))
      return false;
    if ( !Finc ( h_out + 1e-5, F1 ))
      return false;
    dF = ( F1 - F0 ) * 1e5;
    dh_out = -( F1 + F0 ) / ( 2. * dF );
    h_out += dh_out;
    }
  if ( kIter >= 10 )
    return false;
  h_Work_out = h_out;
  S_Work_Out.PH ( pOut_Work->P, h_Work_out, pOut_Work->Get_Comp());
  h_Const_out = S_Const_In.h - pOut_Work->Flow_mol / pOut_Const->Flow_mol * ( h_Work_out - S_Work_In.h );
  S_Const_Out.PH( pOut_Const->P, h_Const_out, pOut_Const->Get_Comp());
  return true;
  }

bool CHeat_1_flow::Section( double dt )
  {
  int kIter = 0;
  double h1 = h_Work_out;
  double F1;
  if ( !Finc ( h1, F1 ))
    return false;
  double F2; 
  double h2;
  if ( F1 > 0. )
    {
    h2 = h1 + 100.;
    if ( !Finc ( h2, F2 ))
      return false;
    }
  else
    {
    h2 = h1 - 100.;
    if ( !Finc ( h2, F2 ))
      return false;
    }
  double h = 0.;
  double F = 0.;
  double dh_min = 10;
  while ( fabs ( dh_min ) > 1e-3  && kIter++ < 10 )
    {
    h = ( F1 * h2 - F2 * h1 ) / ( F1 - F2 );
    if ( !Finc ( h, F ))
      return false;
    double dh1 = fabs ( h - h1 );
    double dh2 = fabs ( h - h2 );
    if ( dh2 < dh1 )
      {
      dh_min = dh1;
      h1 = h;
      F1 = F;
      }
    else
      {
      dh_min = dh2;
      h2 = h;
      F2 = F;
      }
    }
  if ( kIter >= 10 )
    return false;
  h_Work_out = h2;
  S_Work_Out.PH ( pOut_Work->P, h_Work_out, pOut_Work->Get_Comp());
  h_Const_out = S_Const_In.h - pOut_Work->Flow_mol / pOut_Const->Flow_mol * ( h_Work_out - S_Work_In.h );
  S_Const_Out.PH( pOut_Const->P, h_Const_out, pOut_Const->Get_Comp());
  return true;
  }

bool CHeat_1_flow::BiSection( double dt )
  {
  double h_b = S_Work_In.h;
  double h_e = H_Liq ( S_Const_In.T, pIn_Work->Get_Comp());
  double h = h_b;
  double F = 0.;
  int kIter = 0;
  while ( fabs ( h_e - h_b ) > 1e-3 )
    {
    kIter++;
    h = 0.5 * ( h_e + h_b );
    if ( !Finc ( h, F ))
      return false;
    if ( h_e > h_b )
      {
      if ( F < 0 )
        h_e = h;
      else
        h_b = h;
      }
    else
      {
      if ( F > 0 )
        h_e = h;
      else
        h_b = h;
      }
    }
  h_Work_out = h;
  S_Work_Out.PH ( pOut_Work->P, h_Work_out, pOut_Work->Get_Comp());
  h_Const_out = S_Const_In.h - pOut_Work->Flow_mol / pOut_Const->Flow_mol * ( h_Work_out - S_Work_In.h );
  S_Const_Out.PH( pOut_Const->P, h_Const_out, pOut_Const->Get_Comp());
  return true;
  }

int CHeat_1_flow::SaveState( )
  {
  S_CLASS("Heat_1_flow_W",CHeat_1_flow_W)
    return 0;
  }

int CHeat_1_flow::RestoreState( char * StrName )
  {
  COPY_FIRST
    R_CLASS("Heat_1_flow_W",CHeat_1_flow_W)
    return 1;
  }

int CHeat_1_flow::SetData( int TypeData, void * pData )
  {
  if ( TypeData == sd_SetInitState )
    {
    T_in_Work = T_in_Const = T_air;
    T_out_Work = T_out_Const = T_air;
    return 1;
    }
  return 0;
  }

int CHeat_1_flow::GetParams( char * )
  {
#include "IO_Parms.h"
  PARM( kGas_Work, "Теплоотдача труб от газа" )
  PARM( kGas_Const, "Теплоотдача объема от газа" )
  PARM( Cp_Corp_Work, "Теплоемкость корпуса труб" )
  PARM( Cp_Corp_Const, "Теплоемкость корпуса объема" )
  PARM( kCp_Work, "Поправка к Cp труб" )
  PARM( kCp_Const, "Поправка к Cp объема" )
  //
  PARM( T_in_Work, "#Т на входе труб" )
  PARM( T_out_Work, "#Т на выходе труб" ) 
  PARM( T_in_Const, "#Т на входе объема" ) 
  PARM( T_out_Const, "#Т на выходе объема" )
  PARM( S_Work_In.E, "#Eps на входе труб" )
  PARM( S_Work_Out.E, "#Eps на выходе труб" )
  PARM( S_Const_In.E, "#Eps на входе объема" )
  PARM( S_Const_Out.E, "#Eps на выходе объема" )
  PARM( pIn_Work->Flow_mol, "#Поток труб моль" )
  PARM( pIn_Const->Flow_mol, "#Поток объема моль" )
  PARM( Ksi_Work, "#Влияние теплоемкости труб" )
  PARM( Ksi_Const, "#Влияние теплоемкости объема" )
  PARM( dT_Corp_Work, "#dT от теплоемкости труб" )
  PARM( dT_Corp_Const, "#dT от теплоемкости объема" )
  PARM( Q_cal, "#Теплопередача Гкал/ч" )
  PARM( Q_dg, "#Теплопередача ГДж/ч" )
  return 0;
  }

