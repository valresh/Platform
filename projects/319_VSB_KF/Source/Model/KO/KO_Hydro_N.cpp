#include "stdafx.h"
#include "KO.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"

int CKO_N::TestNodes(  int kNodes, struct CObjectPoint ** ppNodes  )
  {
  if ( kNodes != 2 )
    {
    ModMsg ( "У конденсатоотводчика '%s' неправильные входы", ObjName.Str );
    return 1;
    }
  int kN = 0;
  for ( int n = 0; n < 2; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
   if ( pOP->Point == "i" )
      {
      pOP->PntGroup = 1;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      kN++;
      continue;
      }
    if ( pOP->Point == "o" )
      {
      pOP->PntGroup = 1;  
      pOP->NumbInGroup = 1;
      pOP->IO = '-';
      kN++;
      continue;
      }
    }
  if ( kN == 2 )
    return 0;
  ModMsg ( "У конденсатоотводчика '%s' неправильные входы", ObjName.Str );
  return 1;
  }

#include "SetMatr.h"

int CKO_N::SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case 1:
      {
      if ( Full )
        {
        MATR_OMEGA(Omega_Full)
/*
        double * Eq = A;
        double O = Omega_In * 1e3;
        Eq[P_0] = O;
        Eq[F_0] = -1.;
        B[0] = O * B_In;
        Eq += 4;
        Eq[F_0] = 1.;
        Eq[F_1] = -1.;
*/
        }
      else
        {
        double * Eq = A;
        double O = Omega_In * 1e3;
 //       if ( P_in < B_In )
 //         O = 0.;
        Eq[P_0] = O;
        Eq[F_0] = -1.;
        B[0] = O * B_In;
        Eq += 4;
        O = Omega_Out * 1e3 * L;
        Eq[P_1] = O;
        Eq[F_1] = 1.;
        B[1] = O * B_Out;
        }
      }
    break;
    }
  return 0;
  }

#include "SetVar.h"

int CKO_N::SetVar( void * pExternals, int PntGroup,  double dt, 
		double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case 1:
      {
      pFlow_In = (CFlow*)pVar[0];
      P_in = Var[P_0];
      pFlow_Out = (CFlow*)pVar[1];
      P_out = Var[P_1];
      _P_in = P_in - 1.;
      _P_out = P_out - 1.;
      }
    break;
    }
  return 0;
  }

