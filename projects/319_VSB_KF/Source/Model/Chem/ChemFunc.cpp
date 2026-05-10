#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"
#include "Connect.h"
#include "ChemBase.h"


double Ro_H2O( double TC )
  {
  double rho = 1000 * (1. - ( TC + 288.9414 ) /
    ( 508929.2 * ( TC + 68.12963 ) ) * ( TC - 3.9863 ) * ( TC - 3.9863 ) );
  return rho;
  }

double Ro_HNO3( double TC )
  {
  return 1.513;
  }
//

double Norm ( CComp * pComp )
  {
  bool * IsComp = pComp ->IsComp;
  double * Cmol = pComp ->Cmol;
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

double Norm ( bool IsComp[K_GAS], double Cmol[K_GAS] )
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

double Norm ( int K, double Cmol[K_GAS] )
  {
  double S = 0.;
  for ( int n = 0; n < K; n++ )
    {
    if( Cmol[n] < 0.)
      Cmol[n] = 0.;
    S += Cmol[n];
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
  for ( int n = 0; n < K; n++ )
    {
    Cmol[n] *= Si;
    }
  return S; 
  }

double Mw ( CComp * pComp )
  {
  if ( pComp == NULL )
    return pGas[G_N2]->Mw;
  try
  {
  bool * IsComp = pComp ->IsComp;
  double * Cmol = pComp ->Cmol;
  double S = 0.;
  double M = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      S += Cmol[n];
      M += Cmol[n] * pGas[n]->Mw;
      }
    }
  if ( S < 1e-5 ) 
    {
    return 0.;
    }
  if ( fabs ( S - 1. ) < 1e-8 )
    {
    return M;
    }
  double Si = 1. / S;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      Cmol[n] *= Si;
      }
    }
  return M * Si;
  }
  catch(...)
    {
    KKK();
    ASS(0)
    return 18.;
    }
  }

double Pz ( double Tz, CComp * pComp )
  {
  if ( pComp == NULL )
    return 0.;
  bool * IsComp = pComp ->IsComp;
  double * Cmol = pComp ->Cmol;
  double S = 0.;
  double P = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( !IsComp[n] )
      continue;
    if( Cmol[n] < 0.)
      Cmol[n] = 0.;
    S += Cmol[n];
    P += Cmol[n] * pGas[n]->Pz( Tz );
    }
  if ( S < 1e-5 ) 
    return 0.;
  if ( fabs ( S - 1. ) < 1e-8 )
    return P;
  return P / S; 
  }

double Mw ( bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double S = 0.;
  double M = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      S += Cmol[n];
      M += Cmol[n] * pGas[n]->Mw;
      }
    }
  if ( S < 1e-5 ) 
    {
    return 0.;
    }
  if ( fabs ( S - 1. ) < 1e-8 )
    {
    return M;
    }
  double Si = 1. / S;
  //for ( int n = 0; n < LAST_GAS; n++ )
  //  {
  //  if ( IsComp[n] )
  //    {
  //    Cmol[n] *= Si;
  //    }
  //  }
  return M * Si; 
  }

double H_IG ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double S = 0.;
  double Cp = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      S += Cmol[n];
      Cp += Cmol[n] * pGas[n]->H_IG ( Tc );
      }
    }
  if ( S < 1e-5 ) 
    {
    return 0.;
    }
  return Cp / S;
  }


double H_IG ( double Tc, CComp * pComp )
  {
  bool * IsComp = pComp ->IsComp;
  double * Cmol = pComp ->Cmol;
  double S = 0.;
  double Cp = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      S += Cmol[n];
      Cp += Cmol[n] * pGas[n]->H_IG ( Tc );
      }
    }
  if ( S < 1e-5 ) 
    {
    return 0.;
    }
  return Cp / S;
  }

double Cp_IG ( double Tc, CComp * pComp )
  {
  bool * IsComp = pComp ->IsComp;
  double * Cmol = pComp ->Cmol;
  double S = 0.;
  double Cp = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      S += Cmol[n];
      Cp += Cmol[n] * pGas[n]->C_IG ( Tc );
      }
    }
  if ( S < 1e-5 ) 
    {
    return 0.;
    }
  return Cp / S;
  }

double Cp_IG ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double S = 0.;
  double Cp = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      S += Cmol[n];
      Cp += Cmol[n] * pGas[n]->C_IG ( Tc );
      }
    }
  if ( S < 1e-5 ) 
    {
    return 0.;
    }
  return Cp / S;
  }


double To_m3_liq_Mix ( double TC, CComp * pComp )
  {
  double V = 0.;
  double S = 0.;
  double Ro;
  bool * IsComp = pComp ->IsComp;
  double * Cmol = pComp ->Cmol;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      if ( !pGas[n]->Ro_liq ( TC, Ro ))
        continue;
      if ( Ro <= 0. )
        continue;
      V += Cmol[n] * pGas[n]->Mw / Ro; 
      S += Cmol[n];
      }
    }
  if ( S > 0.01 )
    V /= S;
  else
    V = 0.;
  return V;
  }


double To_m3_liq ( double TC, bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double V = 0.;
  double S = 0.;
  double Ro;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      if ( !pGas[n]->Ro_liq ( TC, Ro ))
        continue;
      if ( Ro <= 0. )
        continue;
      V += Cmol[n] * pGas[n]->Mw / Ro; 
      S += Cmol[n];
      }
    }
  if ( S > 0.01 )
    V /= S;
  else
    V = 0.;
  return V;
  }

double To_m3_liq ( double TC, CComp * pComp )
  {
  if ( pComp == NULL )
    {
    return 0.;
    }
  bool * IsComp = pComp ->IsComp;
  double * Cmol = pComp ->Cmol;
  double V = 0.;
  double S = 0.;
  double Ro;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      if ( !pGas[n]->Ro_liq ( TC, Ro ))
        continue;
      V += Cmol[n] * pGas[n]->Mw / Ro; 
      S += Cmol[n];
      }
    }
  if ( S > 0.01 )
    V /= S;
  else
    V = 0.;
  return V;
  }

double H_Liq ( double T, CComp * pComp )
  {
  bool * IsComp = pComp ->IsComp;
  double * Cmol = pComp ->Cmol;
  double H = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      H += Cmol[n] * pGas[n]-> H_l ( T );
      }
    }
  return H;
  }

double H_Liq ( double T, bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double H = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      H += Cmol[n] * pGas[n]-> H_l ( T );
      }
    }
  return H;
  }

double Cp_Liq ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double Cp = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      Cp += Cmol[n] * pGas[n]-> C_Liq( Tc );
      }
    }
  return Cp;
  }

#define K_RO_OIL 0.995

double Ro_liq( double T, bool * IsComp, double * Cmol )
  {
  double Vs = 0.;
  double Ms = 0.;
  double S = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( !IsComp[n] )
      continue;
    double Ro;
    if ( !pGas[n]->Ro_liq ( T, Ro ))
      continue;
    Vs += pGas[n]->Mw * Cmol[n] / Ro;
    Ms += pGas[n]->Mw * Cmol[n];
    S += Cmol[n];
    }
  if ( fabs ( S ) < 1e-6 )
    return 0.;
  double R = 0.;
  if ( Vs > 0. )
    R = Ms / Vs;
  //R *= K_RO_OIL;
  //R *= 1e-3;
  //if ( T == 20. || R < 0.5 )
  //  return R * 1000.;
  ////  double Al = 0.000903;
  ////  R *= 1. - Al * ( T - 20. );
  //R = 1000. * R - ( 0.58 * ( T - 20. ) / R - 
  //  ( T - 1200. * ( R - 0.68 )) * ( T - 20. ) * 1e-3 ) * 0.7;
  return R;
  }

double Ro_liq( double T, CComp * pComp )
  {
  return Ro_liq( T, pComp->IsComp, pComp->Cmol );
  }


double Lam_Liq ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double l_summ = 0.;
  double summ = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double Lam = pGas[n]->Lam_liq( Tc );
      if ( Lam <= 0. )
        continue;
      double CM = Cmol[n] * pGas[n]->Mw;
      l_summ += CM * Lam;
      summ += CM;
      }
    }
  if ( summ <= 0. )
    return 1e-10;
  double Lam = l_summ / summ;
  return Lam;
  }

#define KENDALL
//#define ARRENIUS
const double C = 0.8;
double Vis_Liq ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double ln_summ = 0.;
  double summ = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double Vis = pGas[n]->Vis_liq( Tc );
      if ( Vis <= 0. )
        continue;
      double CM = Cmol[n] * pGas[n]->Mw;
#ifdef ARRENIUS
// Формула Аррениуса + Вальтера
      ln_summ += CM * log ( log ( Vis + C ));
#endif
#ifdef KENDALL
      ln_summ += CM * pow ( Vis, 1. / 3. );
#endif
      summ += CM;
      }
    }
  if ( summ <= 0. )
    return 1e-10;
#ifdef ARRENIUS
  double Vis = exp ( exp ( ln_summ / summ ) ) - C;
  return Vis;
#endif
#ifdef KENDALL
  double Vis13 = ln_summ / summ;
  double Vis = Vis13 * Vis13 * Vis13;
  return Vis;
#endif
  }

double Lam_Gas ( double P, double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double l_summ = 0.;
  double summ = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double Lam = pGas[n]->Lam_gas( P, Tc ); // Более точная ф-ла Уилке : Смеси вязк и тепл.pdf
      if ( Lam <= 0. )
        continue;
      double CM = Cmol[n];
      l_summ += CM * Lam;
      summ += CM;
      }
    }
  if ( summ <= 0. )
    return 1e-10;
  double Lam = l_summ / summ;
  return Lam;
  }

double Vis_Gas ( double P, double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] )
  {
  double l_summ = 0.;
  double summ = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double Vis = pGas[n]->Vis_gas( P, Tc ); // Более точная ф-ла Васильевой : Смеси вязк и тепл.pdf
      if ( Vis <= 0. )
        continue;
      double CM = Cmol[n];
      l_summ += CM * Vis;
      summ += CM;
      }
    }
  if ( summ <= 0. )
    return 1e-10;
  double Vis = l_summ / summ;
  return Vis;
  }


typedef 
class CGas* (*tGetExternComponent)( const char * CompName );

CGas * GetExternComponent( const char * Dll, const char * CompName )
  {
  // _HANDLE hLib = Load_Dll( Dll );
  // if ( hLib == NULL )
  //   return NULL;
  // tGetExternComponent pGetExternComponent = (tGetExternComponent) GetProcAddress( (HMODULE)hLib, "GetExternComponent" );
  // if ( pGetExternComponent == NULL )
  //   return NULL;
  // CGas * pComp = (*pGetExternComponent)( CompName );
  // return pComp;
  return NULL;
  }


struct I_ChemAdditions : public I_AdditionList
  {
  virtual void Init();
  virtual void InitAdditions( void * pData );
  virtual void Add( I_Addition * pAdd );
  virtual void Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq );
  virtual void GetParams( IBaseModel * pMain );
  virtual void UpdateParams( );
  };

//I_ChemAdditions ChemAdditions;

void SetChemAdditions()
  {
//  ChemAdditions.Init();
//  pChemAdditions = &ChemAdditions;
  }

void I_ChemAdditions::Init( )
  {
  pFirst = NULL;
  pLast = NULL;
  }

void I_ChemAdditions::InitAdditions( void * pData )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->Init( pData );
    pAdd = pAdd->pNext;
    }
  }

void I_ChemAdditions::Add( I_Addition * pAdd )
  {
  if ( pFirst == NULL )
    pFirst = pAdd;
  if ( pLast )
    pLast->pNext = pAdd;
  pLast = pAdd;
  pAdd->pNext = NULL;
  }

void I_ChemAdditions::Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->Gamma( pData, P, T, IsComp, Cmol, P_sat_id, Gamma, dH_liq );
    pAdd = pAdd->pNext;
    }
  }

void I_ChemAdditions::GetParams( IBaseModel * pMain )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->GetParams( pMain );
    pAdd = pAdd->pNext;
    }
  }

void I_ChemAdditions::UpdateParams( )
  {

  }

I_Addition * LoadAddition( const char * Dll, const char * AddName )
  {
  // HMODULE hLib = (HMODULE)Load_Dll( Dll, NULL );
  // ASS( hLib )
  // if ( hLib == NULL )
  //   return NULL;
  // tGetAddition pGetAddition = (tGetAddition)GetProcAddress( hLib, "GetAddition" );
  // ASS( pGetAddition )
  // if ( pGetAddition == NULL )
  //   return NULL;
  // return (*pGetAddition) ( AddName );
  return NULL;
  }

CReal * LoadComponent( const char * Dll, const char * CompName )
  {
  // HMODULE hLib = (HMODULE)Load_Dll( Dll, NULL );
  // ASS( hLib )
  // if ( hLib == NULL )
  //   return NULL;
  // tGetRealComp pGetRealComp = (tGetRealComp)GetProcAddress( hLib, "GetRealComp" );
  // if ( pGetRealComp == NULL )
  //   return NULL;
  // return (*pGetRealComp) ( CompName );
  return NULL;
  }

CGas * LoadGasComp( const char * Dll, const char * CompName ) 
  {
  // HMODULE hLib = (HMODULE)Load_Dll( Dll, NULL );
  // ASS( hLib )
  // if ( hLib == NULL )
  //   return NULL;
  // tGetGasComp pGetGasComp = (tGetGasComp)GetProcAddress( hLib, "GetGasComp" );
  // if ( pGetGasComp == NULL )
  //   return NULL;
  // return (*pGetGasComp) ( CompName );
  return NULL;
  }

char * PrefListComp = NULL;
char * ListComp = NULL;

void SetCompList ( char * PreTxt )
  {
  int L = lstrlen( PreTxt ) + 1;
  int Pos = L;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    int c = ID_GasComp[n];
    L = lstrlen( NameGasComp[c] ) + 1;
    Pos += L;
    }
  Pos++;
  PrefListComp = NewArr(char,Pos);
//
  L = lstrlen( PreTxt );
  Pos = 0;
  memmove ( PrefListComp + Pos, PreTxt, L );
  Pos += L;
  PrefListComp[Pos++] = ';';
  PrefListComp[Pos] = 0;
  ListComp = PrefListComp + Pos;
  Pos = 0;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    int c = ID_GasComp[n];
    L = lstrlen( NameGasComp[c] );
    memmove ( ListComp + Pos, NameGasComp[c], L );
    Pos += L;
    ListComp[Pos++] = ';';
    ListComp[Pos] = 0;
    }
  }
