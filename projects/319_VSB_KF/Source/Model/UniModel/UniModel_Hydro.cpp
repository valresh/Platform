#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"

#include "HydroStruct.h"
#include "TestNodes.h"

NODE_LIST
NODE_IN  ( 1, 0, "i","Вход" )
NODE_OUT ( 1, 1, "o","Выход")
END_LIST

STD_TEST

int CUniModel::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( pCurrentModel )
    return pCurrentModel->TestNodes( kNodes, ppNodes );
  return 1;
  }

#include "SetMatr.h"

int CUniModel::SetMatr( void * pExternals, int PntGroup,  double dt, 
                       double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  if ( pCurrentModel )
    return pCurrentModel->SetMatr( pExternals, PntGroup, dt, A, B );
  return 1;
  //if ( !_finite ( Omega ))
  //  Omega = 0.01;
  //if ( !_finite ( Flow_nom_m3 ))
  //  Flow_nom_m3 = 1000.;
  //switch ( Тип )
  //  {
  //  default:
  //  case 0:
  //  case 4:
  //  case 12:
  //    {
  //    double O = Omega;
  //    if ( Defect == 1 )
  //      O *= Omega_def;
  //    MATR_OMEGA(O)
  //    }
  //  break;
  //  case 1:
  //    {
  //    double O = Omega;
  //    if ( Flow < 0. )
  //      O = 1e-5;
  //    double * Eq = A;
  //    Eq[P_0] = O;
  //    Eq[P_1] = -O;
  //    Eq[F_0] = -1.;
  //    B[0] = dP_Hydro;
  //    Eq += 4;
  //    Eq[F_0] = 1.;
  //    Eq[F_1] = -1.;
  //    }
  //  break;
  //  case 2:
  //    {
  //    double * Eq = A;
  //    Eq[P_0] = Omega_2;
  //    Eq[P_1] = -Omega_2;
  //    Eq[F_0] = -1.;
  //    B[0] = dP_Hydro;
  //    Eq += 4;
  //    Eq[F_0] = Alfa;
  //    Eq[F_1] = -1.;
  //    }
  //  break;
  //  case 8:
  //  case 9:
  //    {
  //    double * Eq = A;
  //    Eq[P_0] = Omega;
  //    Eq[P_1] = -Omega;
  //    Eq[F_0] = -1.;
  //    Eq += 4;
  //    Eq[F_0] = 1. + delta_Flow * 0.01;
  //    Eq[F_1] = -1.;
  //    }
  //  break;
  //  case 10:
  //    {
  //    double * Eq = A;
  //    Eq[P_0] = Omega;
  //    Eq[P_1] = -Omega;
  //    Eq[F_0] = -1.;
  //    Eq += 4;
  //    Eq[F_0] = -1.;
  //    Eq[F_1] = k_Flow;
  //    }
  //  break;
  //  case 11:
  //    {
  //    double * Eq = A;
//    Eq[F_0] = 1.;
  //    B[0] = Flow;
  //    Eq += 4;
  //    Eq[F_1] = 1.; 
  //    B[1] = Flow;
  //    }
  //  break;
  //  }
  return 1;
  }

#include "SetVar.h"

int CUniModel::SetVar( void * pExternals, int PntGroup, double dt, 
                      double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  Перепад = Var[P_0] - Var[P_1];
  if ( pCurrentModel )
    return pCurrentModel->SetVar( pExternals, PntGroup, dt, Var,	pVar );
  return 1;
  }


