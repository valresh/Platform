#include "stdafx.h"
#include "H2SCO2.h"
#include "Chem.h"



ADParams::ADParams()
  {
  T0_kS = 40.;
  K0_kS = 0.01;
  T1_kS = 100.;
  K1_kS = 100.;
  T0_kC = 40.;
  K0_kC = 0.01;
  T1_kC = 100.;
  K1_kC = 100.;
  }

void ADParams::CalcCoef()
  {
  if ( K0_kS <= 0. )
    K0_kS = 1e-8;
  if ( K1_kS <= 0. )
    K1_kS = 1e-8;
  if ( T1_kS <= T0_kS )
    T1_kS = T0_kS + 0.01;
  double L0S = log ( K0_kS );
  double L1S = log ( K1_kS );
  double T0S = T0_kS + TK;
  double T1S = T1_kS + TK;
  B_kS = T0S * T1S * ( L1S - L0S ) / ( T0S - T1S );
  A_kS = L0S - B_kS / T0S;
  //////////////////////
  if ( K0_kC <= 0. )
    K0_kC = 1e-8;
  if ( K1_kC <= 0. )
    K1_kC = 1e-8;
  if ( T1_kC <= T0_kC )
    T1_kC = T0_kC + 0.01;
  double L0C = log ( K0_kC );
  double L1C = log ( K1_kC );
  double T0C = T0_kC + TK;
  double T1C = T1_kC + TK;
  B_kC = T0C * T1C * ( L1C - L0C ) / ( T0C - T1C );
  A_kC = L0C - B_kC / T0C;
  }

void ADParams::CalcK( double TC )
  {
  double T = TC + TK;
  double ABS = A_kS + B_kS / T;
  if ( ABS < -20. )
    ABS = -20.;
  if ( ABS > 20. )
    ABS = 20.;
  kS = exp ( ABS );
  //
  double ABC = A_kC + B_kC / T;
  if ( ABC < -20. )
    ABC = -20.;
  if ( ABC > 20. )
    ABC = 20.;
  kC = exp ( ABC );
  }

MDEA_Ads::MDEA_Ads( ADParams * _pP )
  {
  pP = _pP;
  ID_M = ID_S = ID_C = ID_MS = ID_MC = -1;
  pP->CalcCoef();
  }


int MDEA_Ads::Init( IBaseModel * pMain )
  {
  ID_M = GetGasID( "DIETHANOLAMINE" );
  ID_S = GetGasID( "HYDROGEN SULFIDE" ); 
  ID_C = GetGasID( "CARBON DIOXIDE" ); 
  ID_MS = GetGasID( "DEA+H2S" ); 
  ID_MC = GetGasID( "DEA+CO2" );
  pP->CalcCoef();
  return 0;
  }

bool MDEA_Ads::ReadData( struct IBaseModel * pModel )
  {
  Init( pModel );
  return true;
  }

#include "IO_Parms.h"
#define this pMain
void MDEA_Ads::GetParams( IBaseModel * pMain )
  {
  TAB("DEA",1)
  PARM( pP->T0_kS, "T0 H2S" )
  PARM( pP->K0_kS, "K0 H2S" )
  PARM( pP->T1_kS, "T1 H2S" )
  PARM( pP->K1_kS, "K1 H2S" )
//
  PARM( pP->T0_kC, "T0 CO2" )
  PARM( pP->K0_kC, "K0 CO2" )
  PARM( pP->T1_kC, "T1 CO2" )
  PARM( pP->K1_kC, "K1 CO2" )
  ETAB
  }
#undef this


void MDEA_Ads::UpdateParams( struct CParams & Param )
  {
  pP->CalcCoef();
  }
/*
struct IReactionsParam
  {
  DWORD  * Param;
  double dt;
  double * P;
  double * T;
  bool   * IsComp;
  double * X;
  double * Y;
  double * Q;
  double * M_Liq;
  double * R;
  };
*/
#define GET(ID) pParam->IsComp[ID] ? pParam->X[ID] : 0
#define SET(V,ID) if ( V < 0. ) V = 0.; pParam->IsComp[ID] = true; pParam->X[ID] = V;

void MDEA_Ads::Reactions( double dt, IReactionsParam * pParam )
  {
  pGas[ID_MS]->Mw = pGas[ID_M]->Mw + pGas[ID_S]->Mw;
  pP->CalcK( *(pParam->T) );
  double M0 = GET(ID_M);
  double S0 = GET(ID_S);
  double C0 = GET(ID_C);
  double MS0 = GET(ID_MS);
  double MC0 = GET(ID_MC);
  double Min = pParam->X[ID_M] * pGas[ID_M]->Mw + 
               pParam->X[ID_S] * pGas[ID_S]->Mw + 
               pParam->X[ID_MS] * pGas[ID_MS]->Mw;
//
  double kS = pP->kS;
  double kC = pP->kC;
  double a = C0 - M0 - S0 - pP->kS;
  double b = M0 * M0 + 2. * M0 * S0 + C0 * kS + M0 * kC + M0 * kS + MC0 * kC - MS0 * kS + S0 * kC + kC * kS;
  double c = MS0 * kC * kS - M0 * M0 * S0 + C0 * MS0 * kS + M0 * MS0 * kS - M0 * S0 * kC - MC0 * S0 * kC;
  double det = b * b - 4. * a * c;
  if ( det < 0. )
    det = 0.;
  double rS = ( -b + sqrt ( det )) / ( 2. * a );
  double rC = -( C0 * rS - C0 * M0 + MC0 * kC )/( C0 + M0 + kC - rS );
//
  double M = M0 - rS - rC;
  double S = S0 - rS;
  double C = C0 - rC;
  double MS = MS0 + rS;
  double MC = MC0 + rC;
  SET(M,ID_M)
  SET(S,ID_S)
  SET(C,ID_C)
  SET(MS,ID_MS)
  SET(MC,ID_MC)
  double Mout = pParam->X[ID_M] * pGas[ID_M]->Mw + 
               pParam->X[ID_S] * pGas[ID_S]->Mw + 
               pParam->X[ID_MS] * pGas[ID_MS]->Mw;
  double dMw = pGas[ID_M]->Mw + pGas[ID_S]->Mw - pGas[ID_MS]->Mw;
  double dM = Min - Mout;
  Norm(pParam->IsComp,pParam->X);
  *pParam->R = rS + rC;
  }

int MDEA_Ads::SaveState( char * SaveName )
  {
  return 0;
  }

int MDEA_Ads::RestoreState ( char * SaveName, char * StrName )
  {
  return 0;
  }

