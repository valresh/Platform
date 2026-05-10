#include "stdafx.h"
#include "SepLiq.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"
#include "SetVar.h"


NODE_LIST
  NODE_IN (1, 0, "i|in_gas","Подключение входа двухфазного потока")
  NODE_IN (1, 1, "o_gas|out_gas","Подключение выхода газового потока")
  NODE_IN (1, 2, "o_liq|out_liq", "Подключение выхода жидкостного потока")
  NODE_IN (2, 0, "i_heat|in_heat","Подключение входа в змеевик")
  NODE_OUT(2, 1, "o_heat|out_heat","Подключение выхода из змеевика")
END_LIST

STD_TEST

/*
IF("вода")
  {
  PseudoWater = true;
  pOP->PntGroup = 3;  
  pOP->NumbInGroup = 0;
  pOP->IO = '+';
  continue; 
  };
*/
int CSepLiq::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  int Res = ::StdTestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  return 0;
  }

#include "SetMatr.h"

int CSepLiq::SetMatr( void * pExternals, int PntGroup,  double dt, 
                      double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  if ( strstr ( ObjName, "Сепаратор_ОГ_301"))
    KKK();
  switch ( PntGroup )
    {
    case 1:
      {
      double * Eq = A;
      if ( Разгерметизация )
        {
        Eq[P_0] = 1.;
        B[0] = 1.;
        Eq += 6;
        Eq[P_1] = 1.;
        B[1] = 1.;
        Eq += 6;
        Eq[P_2] = 1.;
        B[2] = 1.;
        return 0;
        }
      if ( !_finite ( T_gas ))
        T_gas = 15.;
      if ( !_finite ( P ))
        P = 1.;
      Ksi = dt * RP * ( T_gas + TK ) / Volume;
      double O_Ksi = Omega_Gas * Ksi;
      memmove(Eps_h, Eps, sizeof ( Eps_h));
// 0
//
      Eq[P_0] = -Omega_Gas;
      Eq[F_0] = 1. + O_Ksi * Eps_h[0];
      Eq[F_1] = O_Ksi * Eps_h[1];
      Eq[F_2] = O_Ksi * Eps_h[2];
      B[0] = -Omega_Gas * P;
      //
      Eq += 6;
      Eq[P_1] = -Omega_Gas;
      Eq[F_1] = 1. + O_Ksi * Eps_h[1];
      Eq[F_0] = O_Ksi * Eps_h[0];
      Eq[F_2] = O_Ksi * Eps_h[2];
      B[1] = -Omega_Gas * P;
      //
      Eq += 6;
      double O = Omega_Liq;
      if ( !_finite ( Level ))
        Level = 0.;
      if ( Close_Out_liq && Level < 0.1 )
        O *= Level * 10.;
      O_Ksi = O * Ksi;
      Eq[P_2] = -O;
      Eq[F_2] = 1. + O_Ksi * Eps_h[2];
      Eq[F_0] = O_Ksi * Eps_h[0];
      Eq[F_1] = O_Ksi * Eps_h[1];
      _dP_hydro = Hydro_L * Level + Hydro_C;
      double P_out = P + _dP_hydro;
      B[2] = -O * P_out;
      }
    return 0;
    case 2:
      {
      MATR_OMEGA(Omega_Heat)
      }
    return 0;
    case 3:
      {
      //if ( pPW )
      //  {
      //  double P_hydro = pPW->Hydro_Water * pPW->Level_Water;
      //  double P_out = P + P_hydro;
      //  pPW->Matr( P_out, Mp, Mf, V );
      //  }
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

#include "SetVar.h"

int CSepLiq::SetVar( void * pExternals, int PntGroup, double dt, 
                    // Переменные на входах
                    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case 1:
      {
      pFlow[0] = (CFlow*)pVar[0];
      pFlow[1] = (CFlow*)pVar[1];
      pFlow[2] = (CFlow*)pVar[2];
      _dP_gas = Var[P_0] - Var[P_1];
      Перепад = _dP_gas;
      double Pnew = 0.5 * ( Var[P_0] + Var[P_1] );
      double Pnew_1 = Var[P_1] - Var[F_1] / Omega_Gas;
      P += Ksi * ( Eps_h[0] * Var[F_0] + Eps_h[1] * Var[F_1] + Eps_h[2] * Var[F_2] );
      _P = P - 1.;
      _Flow_in = Var[F_0] * M3;
      _Flow_gas_out = Var[F_1] * M3;
      _Flow_liq_out = Var[F_2] * ::Mw(pFlow[2]->Get_Comp());
      //double T0 = Omega_Gas * ( Var[P_0] - P ) - Var[F_0];
      //double T1 = Omega_Gas * ( Var[P_1] - P ) - Var[F_1];
      //double T2 = Omega_Liq * ( Var[P_2] - P ) - Var[F_2];
      //KKK();
      //if ( pPW )
      //  {
      //  pPW->Calc( dt, pFlow[0] );
      //  }
      }
    return 0;
    case 2:
      {
      IO_2(pInHeat,pOutHeat)
      Heat.pIn = pInHeat;
      Heat.pOut = pOutHeat;
      }
    return 0;
    case 3:
      {
      //CFlow * pFW = Get_Flow ( ID_Flow[0] );
      //if ( pPW )
      //  {
      //  pPW->Out( dt, T_liq, pFW );
      //  }
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

