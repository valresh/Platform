#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"
#include "CommProc.h"


CMixVol::CMixVol( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
  {
  M = 1.;
  CLEAR(CMixVol_W::IsComp)
  CLEAR(CMixVol_W::Cmol)
  }

void CMixVol::Set ( CComp * pComp )
  {
  if(!pComp)
    return;
  memcpy ( IsComp, pComp->IsComp, K_GAS );
  memcpy ( Cmol, pComp->Cmol, K_GAS * 8 );
  }

void CMixVol::Get ( CComp * pComp )
{
	if(!pComp)
		return;
	memcpy ( pComp->IsComp, IsComp, K_GAS );
	memcpy ( pComp->Cmol, Cmol, K_GAS * 8 );
}

void CMixVol::SetAir( )
  {
  Cmol_0( );
  IsComp_0( );
  IsComp[G_N2] = true;
  IsComp[G_O2] = true;
  Cmol[G_N2] = 0.79;
  Cmol[G_O2] = 0.21;
  }

void CMixVol::SetN2( )
  {
  Cmol_0( );
  IsComp_0( );
  IsComp[G_N2] = true;
  Cmol[G_N2] = 1.;
  }

void CMixVol::SetWater( )
  {
  Cmol_0( );
  IsComp_0( );
  IsComp[G_H2O] = true;
  Cmol[G_H2O] = 1.;
  }

void CMixVol::Add ( int ID, double dM )
  {
  double d = fabs ( dM );
  double N = 1. / ( M + d );
  double Ksi = M * N;
  double Ksi1 = 1. - Ksi;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      Cmol[n] *= Ksi;
      }
    }
  IsComp[ID] = true;
  Cmol[ID] += Ksi1;
  }

void CMixVol::Add ( double dM, double * _Cmol )
  {
  double d = fabs ( dM );
  double N = 1. / ( M + d );
  double Ksi = M * N;
  double Ksi1 = 1. - Ksi;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      Cmol[n] = Ksi * Cmol[n] + Ksi1 * _Cmol[n];
      }
    }
  }

void CMixVol::Add ( double dM, bool * _IsComp, double * _Cmol )
  {
  double d = fabs ( dM );
  double N = 1. / ( M + d );
  double Ksi = M * N;
  double Ksi1 = 1. - Ksi;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      Cmol[n] *= Ksi;
    }
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( _IsComp[n] )
      {
      IsComp[n] = true;
      Cmol[n] += Ksi1 * _Cmol[n];
      }
    }
  }

void CMixVol::Add ( double dM, CComp * pComp )
  {
  if(!pComp)
	  return;
  Add ( dM, pComp->IsComp, pComp->Cmol );
  }

double CMixVol::Norm ( )
  {
  double S = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      S += Cmol[n];
      }
    }
  if ( S < 1e-5 ) 
    {
    return S;
    }
  if ( fabs ( S - 1. ) < 1e-8 )
    {
    return S;
    }
  double Si = 1. / S;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      Cmol[n] *= Si;
      }
    }
  return S; 
  }

int CMixVol::SaveState ( )
  {
  S_CLASS ( "MixVol_W", CMixVol_W );
  return 0;
  }

int CMixVol::RestoreState ( char * StrName )
  {
  R_CLASS ( "MixVol_W", CMixVol_W );
  return 1;
  }
