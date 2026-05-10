#include "stdafx.h"
#include "PPK.h"
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

STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes <= 0 )
    {
    ModMsg ( "У ППК '%s' не заданы входы", ObjName );
    return 1;
    }
  if ( kNodes > 2 )
    {
    ModMsg ( "У ППК '%s' неправильные входы", ObjName );
    return 1;
    }
  int Res = ::StdTestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  if ( kNodes == 2 )
    {
    CObjectPoint * pOP_0 = ppNodes[0];
    CObjectPoint * pOP_1 = ppNodes[1];
    pOP_0->Type = eГидравлика;
    pOP_1->Type = eГидравлика;
    bool OK = false;
    if ( pOP_0->Point[0] == 'i' && pOP_1->Point[0] == 'o' )
      {
      OK = true;
      }
    if ( pOP_0->Point[0] == 'o' && pOP_1->Point[0] == 'i' )
      {
      OK = true;
      }
    if ( !OK )
      {
      ModMsg ( "У ППК '%s' неправильные входы ('%s','%s')", ObjName, pOP_0->Point, pOP_1->Point );
      return 2;
      }
    return 0;
    }
  if ( kNodes == 1 )
    {
    CObjectPoint * pOP = ppNodes[0];
    pOP->Type = eГидравлика;
    if ( pOP->Point[0] == 'i' )
      {
      return 0;
      }
    ModMsg ( "У ППК '%s' неправильный вход '%s'", (char*)ObjName, (char*)pOP->Point );
    return 2;
    }
  return 0;
  }

int CPPK::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes <= 0 )
    {
    ModMsg ( "У ППК '%s' не заданы входы", (char*)ObjName );
    return 1;
    }
  if ( kNodes > 2 )
    {
    ModMsg ( "У ППК '%s' неправильные входы", (char*)ObjName );
    return 1;
    }
  if ( kNodes == 2 )
    {
    for ( int n = 0; n < kNodes; n++ )
      {
      CObjectPoint * pOP = ppNodes[n];
      pOP->Type = eГидравлика;
      if ( pOP->Point[0] == 'i' )
        {
        pOP->PntGroup = 1;  // i,o
        pOP->NumbInGroup = 0;
        pOP->IO = '+';
        continue;
        }
      if ( pOP->Point[0] == 'o' )
        {
        pOP->PntGroup = 1;  // i,o
        pOP->NumbInGroup = 1;
        pOP->IO = '-';
        continue;
        }
      ModMsg ( "У ППК '%s' неправильный вход '%s'", (char*)ObjName, (char*)pOP->Point );
      return 0;
      }
    OnlyIn = false;
    return 0;
    }
  if ( kNodes == 1 )
    {
    OnlyIn = true;
    CObjectPoint * pOP = ppNodes[0];
    pOP->Type = eГидравлика;
    if ( pOP->Point[0] == 'i' )
      {
      pOP->PntGroup = 2;  // i
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      OnlyIn = true;
      return 0;
      }
    ModMsg ( "У ППК '%s' неправильный вход '%s'", ObjName, pOP->Point );
    return 2;
    }
  return 1;
  }

#include "SetMatr.h"

int CPPK::SetMatr( void * pExternals, int PntGroup,  double dt, 
                  double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case 1:
      {
      double P_O = P_PPK;
      if ( DefOpen )
        {
        _IsOpen = Open = true;
        double Omega_Fin = Calc_Omega( );
        double * Eq = A;
        Eq[P_0] = Omega_Fin;
        Eq[P_1] = -Omega_Fin;
        Eq[F_0] = -1.;
        Eq += 4;
        Eq[F_0] = 1.;
        Eq[F_1] = -1.;
        return 0;
        }
      if ( dP > P_O )
        {
        lstrcpy ( ErrObj, ObjName );
        if ( WorkPPK )
          _IsOpen = Open = true;
        if ( !WorkPPK )
          _IsOpen = Open = false;
        }
      if ( VintPPK )
        {
        if ( dP > P_O )
          {
          double * Eq = A;
          Eq[P_0] = OmegaVintPPK;
          Eq[P_1] = -OmegaVintPPK;
          Eq[F_0] = -1.;
          B[0] = OmegaVintPPK * P_O;
          Eq += 4;
          Eq[F_0] = 1.;
          Eq[F_1] = -1.;
          return 0;
          }
        else
          _IsOpen = Open = false;
        }
      if ( dP < P_O * P_lev_Off )
        _IsOpen = Open = false;
      if ( Open )
        {
//        Fsend ( "Сработали ППК.txt", "%s\n", ObjName );
        double Omega_Fin = Calc_Omega( );//Omega_Open_PPK;
        //  F = O * ( P0 - P1 - Pz )
        double * Eq = A;
        Eq[P_0] = Omega_Fin;
        Eq[P_1] = -Omega_Fin;
        Eq[F_0] = -1.;
        B[0] = Omega_Fin * P_O * P_lev_Min;
        Eq += 4;
        Eq[F_0] = 1.;
        Eq[F_1] = -1.;
        }
      else
        {
			double * Eq = A;
			Eq[P_0] = 1.0;
			Eq[F_0] = -1.0;
			B[0] = Pin;
			Eq += 4;
			Eq[P_1] = 1.0;
			Eq[F_1] = 1.0;
			B[1] = Pout;
        }
      }
    return 0;
    case 2:
      {
      if ( DefOpen )
        {
        _IsOpen = Open = true;
        double Omega_Fin = Calc_Omega( );
        A[0] = Omega_Fin;
        A[1] = -1.;
        B[0] = Omega_Fin;
        return 0;
        }
      double P_O = P_PPK;
      if ( dP > P_O )
        {
        lstrcpy ( ErrObj, ObjName );
        if ( WorkPPK )
          _IsOpen = Open = true;
        if ( !WorkPPK )
          _IsOpen = Open = false;
        }
      if ( dP < P_O * P_lev_Off )
        _IsOpen = Open = false;
      if ( Open )
        {
        double Omega_Fin = Calc_Omega( );//Omega_Open_PPK;
        //  F = O * ( P - 1 - Pz )
        A[0] = Omega_Fin;
        A[1] = -1.;
        B[0] = Omega_Fin * ( P_O + 1. ) * P_lev_Min;
        }
      else
        {
        A[1] = 1.;
        return 0;
        }
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

#include "SetVar.h"

  int CPPK::SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  switch ( PntGroup )
    {
    case 1:
      {
	  if(Open)
	  {
		  Pin = Var[P_0];
		  Pout = Var[P_1];
	  }
	  else
	  {
		  double dP_in = LimdX ( 1.0 * Var[F_0], 1.0 );
		  Pin += dP_in;
		  double dP_out = LimdX ( 1.0 * Var[F_1], 1.0 );
		  Pout -= dP_out;		  
	  }
	  P_In = Pin - 1.0;
	  P_Out = Pout - 1.0;
      Flow_in = Var[F_0];
      OutIsGas = true;
      pFlow_in = (CFlow*)pVar[0];
      F_kg = pFlow_in->Flow_kg();
      CFlow * pFlow_out = (CFlow*)pVar[1];
      if ( Flow_in > 0. )
        *pFlow_out = *pFlow_in;
      if ( Flow_in < 0. )
        *pFlow_in = *pFlow_out;
      P = P_In;
      _dP = dP = P_In - P_Out;
      T =  pFlow_in->T;
      Mw = pFlow_in->To_kg;
      }
    return 0;
    case 2:
      {
      P_In = Var[P_0] - 1.;
      P_Out = 0.;
      Flow_in = Var[F_0];
      OutIsGas = true;
      pFlow_in = (CFlow*)pVar[0];
      F_kg = pFlow_in->Flow_kg();
      P = P_In;
      _dP = dP = P_In - P_Out;
      T =  pFlow_in->T;
      Mw = pFlow_in->To_kg;
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }
