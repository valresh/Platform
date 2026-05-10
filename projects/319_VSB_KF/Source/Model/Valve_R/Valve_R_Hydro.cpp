#include "stdafx.h"
#include "Valve_R.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"
#include "TestNodes.h"

NODE_LIST
NODE_IN  ( 1, 0, "i","Входной фланец")
NODE_OUT ( 1, 1, "o","Выходной фланец")
//
END_LIST

STD_TEST


int CValve_R::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  int Res = ::StdTestNodes( (char*)ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  return 0;
  }
#include "SetMatr.h"

int CValve_R::SetMatr(  void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  double * Eq = A;
  if ( Forvard )
    {
    double O = Omega_Fwd * 10000.;//Перепад.Omega;
    Eq[P_0] = O;
    Eq[P_1] = -O;
    Eq[F_0] = -1.;
    Eq += 4;
    Eq[F_0] = 1;
    Eq[F_1] = -1.;
    }
  else
    {
    if ( Omega_Back <= 0. )
      {
      double Ksi = 1.;
      double O = 1.;
      Eq[P_0] = -O;
      Eq[F_0] = 1. + O * Ksi;
      B[0] = -O * Pin;
      Eq += 4;
      Eq[P_1] = O;
      Eq[F_1] = 1. + O * Ksi;
      B[1] = O * Pout;
      }
    else
      {
      double O = Omega_Back;
      Eq[P_0] = O;
      Eq[P_1] = -O;
      Eq[F_0] = -1.;
      Eq += 4;
      Eq[F_0] = 1;
      Eq[F_1] = -1.;
      }
    }
  return 0;
  }

#include "SetVar.h"

int CValve_R::SetVar( void * pExternals, int PntGroup, double dt, 
                             // Переменные на входах
                             double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  P_In = Var[P_0] - 1.;
  P_Out = Var[P_1] - 1.;
  F = Var[F_0];
  F_nm3 = F * M3;
  _dP = dP = P_In - P_Out;
  if ( Reverse )
    dP = -dP;
  CFlow * pF_0 = (CFlow *)pVar[0];
  CFlow * pF_1 = (CFlow *)pVar[1];
  pFlow_In = pF_0;
  if ( Var[P_0] > 0. )
    *pF_1 = *pF_0;
  else
    *pF_0 = *pF_1;
  if ( !_finite(pF_0->Flow_mol ))
    pF_0->Flow_mol = 0.;
  if ( !_finite(pF_1->Flow_mol ))
    pF_1->Flow_mol = 0.;
  F_kg = pF_0->Flow_kg();
  if ( Forvard || Omega_Back > 0. )
    {
    Pin = Var[P_0];
    Pout = Var[P_1];
    }
  else
    {
    double Ksi = k_Ksi;
    Pin += LimdX( Ksi * pF_0->Flow_mol, 0.5 );
    Pout -= LimdX( Ksi * pF_1->Flow_mol, 0.5 );
    }
  return 0;
  }

