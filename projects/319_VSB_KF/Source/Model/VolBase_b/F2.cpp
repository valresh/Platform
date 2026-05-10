#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 

// Двухфазность 
void CVolBase::Calc2F( )
  {
  if ( Type2F == 1 )
    {
    Calc2F_Comp( );
    return;
    }
  if ( Eps_W_in_O < 1e-4 )
    Eps_W_in_O = 1e-4;
  double Gw1 = 1. / ( Eps_W_in_O * 0.01 );
  double Gw2 = 1.;
  if ( Eps_O_in_W < 1e-4 )
    Eps_O_in_W = 1e-4;
  double Go1 = 1.;
  double Go2 = 1. / ( Eps_O_in_W * 0.01 );
  double Kw = Gw2 / Gw1;
  if ( fabs ( Kw - 1. ) < 1e-3 )
    Kw = 1.001;
  double Ko = Go2 / Go1;
  if ( fabs ( Ko - 1. ) < 1e-3 )
    Ko = 1.001;
  double Zw = 0.;
  if ( IsComp[G_H2O] )
    Zw = Cmol_Liq[G_H2O];
  double Zo = 1. - Zw;
  Eps_1F = Zo / ( 1. - Kw ) + Zw / ( 1. - Ko );
  if ( Eps_1F >= 1. || Eps_1F <= 0. )
    {
    memmove ( Cmol_1F, Cmol_Liq, CMOL_GAS );
    memmove ( Cmol_2F, Cmol_Liq, CMOL_GAS );
    Eps_1F = 1.;
    Is_2F = false;
    return;
    }
  Is_2F = true;
  double Nw = 1. / (( Kw - 1. ) * Eps_1F + 1. );
  double No = 1. / (( Ko - 1. ) * Eps_1F + 1. );
  double W2 = Zw * Nw;
  double O2 = Zo * No;
  double W1 = Kw * W2;
  double O1 = Ko * O2;
  double Summ_1 = 0.;
  double Summ_2 = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      if ( c == G_H2O )
        {
        Cmol_1F[c] = Cmol_Liq[c] * W1;
        Cmol_2F[c] = Cmol_Liq[c] * W2;
        Gamma_mix[c] = Gw1 * Kw * Nw;
        Summ_1 += Cmol_1F[c];
        Summ_2 += Cmol_2F[c];
        continue;
        }
      Cmol_1F[c] = Cmol_Liq[c] * O1;
      Cmol_2F[c] = Cmol_Liq[c] * O2;
      Gamma_mix[c] = Go1 * Ko * No;
      Summ_1 += Cmol_1F[c];
      Summ_2 += Cmol_2F[c];
      }
    }
  double N1 = 1. / Summ_1;
  double N2 = 1. / Summ_2;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      Cmol_1F[c] *= N1;
      Cmol_2F[c] *= N2;
      }
    }
  }

void CVolBase::Calc2F_Comp( )
  {
  if ( M <= 1e-5 )
    {
    memmove ( Cmol_1F, Cmol_Liq, LAST_GAS << 3 );
    memmove ( Cmol_2F, Cmol_Liq, LAST_GAS << 3 );
    return;
    }
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
	{
	  if(G1_G2[c] < 1e-9)
		  G1_G2_Q[c] = 1e-9 - 1.0;
	  else
		  G1_G2_Q[c] = G1_G2[c] - 1.;
	}
    }
  double N = ::Norm(IsComp, Cmol_Liq );
  if( fabs ( N - 1. ) > 1e-6 )
    KKK();
  double Eps_2F = 1. - Eps_1F;
  Eps_2F = CalcEps( Eps_2F );
  Eps_1F = 1. - Eps_2F;
  if ( Eps_2F >= 1. )
    {
    memmove ( Cmol_1F, Cmol_Liq, LAST_GAS << 3 );
    memmove ( Cmol_2F, Cmol_Liq, LAST_GAS << 3 );
    Is_2F = false;
    }
  else
    {
    if ( Eps_2F <= 0. )
      {
      memmove ( Cmol_1F, Cmol_Liq, LAST_GAS << 3 );
      memmove ( Cmol_2F, Cmol_Liq, LAST_GAS << 3 );
      Is_2F = false;
      }
    else
      {
      for ( int n = 0; n < LAST_GAS; n++ )
        {
        if ( IsComp[n] )
          {
          Cmol_1F[n] = Cmol_Liq[n] / ( G1_G2_Q[n] * Eps_2F + 1.);
          Cmol_2F[n] = Cmol_1F[n] * ( G1_G2_Q[n] + 1. );
          }
        }
      Is_2F = true;
      }
    }
  N = ::Norm(IsComp, Cmol_1F );
  //ASS(fabs ( N - 1. ) < 1e-6 )
  N = ::Norm(IsComp, Cmol_2F );
  //ASS(fabs ( N - 1. ) < 1e-6 )
  }


double CVolBase::Summa( double Eps )
  {
  double S = 0.;
  double Sp = 0.;
  double Sm = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double q = G1_G2_Q[n];
      double Zn = 1. / ( 1. + Eps * q );
      if ( q > 0. )
        Sp += q * Cmol_Liq[n] * Zn;
      else
        Sm -= q * Cmol_Liq[n] * Zn;  
      }
    }
  if ( Sp > 1. )
    S += log ( Sp );
  else
    S += Sp - 1.;
  if ( Sm > 1. ) 
    S -= log ( Sm );
  else
    S -= Sm - 1.;
  return S;
  }

void CVolBase::SdS( double Eps, double &S, double &dSdE )
  {
  double Sp = 0.;
  double Sm = 0.;
  double dSp = 0.;
  double dSm = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double q = G1_G2_Q[n];
      double Zn = 1. / ( 1. + Eps * q );
      double C = q * Cmol_Liq[n] * Zn;
      if ( q > 0. )
        {
        Sp += C;
        dSp -= C * Zn * q;
        }
      else
        {
        Sm -= C;
        dSm += C * Zn * q;
        }
      }
    }
  S = 0.;
  dSdE = 0.;
  if ( Sp > 1. )
    {
    S = log ( Sp );
    dSdE = dSp / Sp;
    }
  else
    {
    S = Sp - 1.;
    dSdE = dSp;
    }
  if ( Sm > 1. ) 
    {
    S -= log ( Sm );
    dSdE -= dSm / Sm;
    }
  else
    {
    S -= Sm - 1.;
    dSdE -= dSm;
    }
  }

double CVolBase::CalcEps( double Eps0 )
  {
  bool All_plus = true;
  bool All_minus = true;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if ( All_plus && G1_G2_Q[n] < 0. )
        All_plus = false;
      if ( All_minus && G1_G2_Q[n] > 0. )
        All_minus = false;
      }
    }
  if ( All_plus )
    {
    return 1.;
    }
  if ( All_minus )
    {
    return 0.;
    }
  double Eb = Eps0;
  double Ee = Eps0;
  double Sb = Summa ( Eb );
  double Sb1 = Summa ( 0 );
  double Sb2 = Summa ( 0.5 );
  double Sb3 = Summa ( 1. );
  double Se = Sb;
  int kIter = 0;
  while ( Sb < 0. )
    {
    kIter++;
    Ee = Eb;
    Se = Sb;
    if ( Eb == 0. )
      return 0.;
    Eb -= 0.01;
    if ( Eb < 0. )
      Eb = 0.;
    Sb = Summa ( Eb );
    if ( Sb > 0. )
      break;
    }
  while ( Se > 0. )
    {
    kIter++;
    Eb = Ee;
    Sb = Se;
    if ( Ee == 1. )
      return 1.;
    Ee += 0.01;
    if ( Ee > 1. )
      Ee = 1.;
    Se = Summa ( Ee );
    if ( Se < 0. )
      break;
    }
  if ( Eps0 < Eb )
    Eps0 = Eb;
  if ( Eps0 > Ee )
    Eps0 = Ee;

  double Eps = Eps0;
  double d_Eps = 1e10;
  double S, dSdE;
  bool OK = true;
  while ( fabs ( d_Eps ) > 1e-9 )
    {
    kIter++;
    SdS( Eps, S, dSdE );
    if ( fabs ( dSdE ) < 1e-9 )
      {
      OK = false;
      break;
      }
    d_Eps = -S / dSdE;
    Eps += d_Eps;
    if ( !_finite( Eps ) || Eps < 0. || Eps > 1. || kIter > 20 )
      {
      OK = false;
      break;
      }
    }
  if ( OK )
    return Eps;
  //	double S;
  //	int kIter = 0;
  Eps = 0.5 * ( Ee + Eb );
  while ( Ee - Eb > 1e-9 )
    {
    kIter++;
    Eps = 0.5 * ( Ee + Eb );
    S = Summa ( Eps );
    if ( S < 0. )
      Ee = Eps;
    else
      Eb = Eps;
    }
  return Eps;
  }
