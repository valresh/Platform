#include "stdafx.h"
#include "KO.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"


//NODE_LIST
//NODE_IN  ( 1, 0, "i","Вход" )
//NODE_OUT ( 2, 0, "o","Выход")
//END_LIST

//NODE_LIST
extern "C"
__declspec(dllexport) CObjVariablesData VariablesData[] = {

{ "i", eГидравлика, 1, 0, '+', "Вход" },
{ "o", eГидравлика, 2, 0, '-', "Выход" },
//NODE_IN  ( 1, 0, "i","Вход" )
//NODE_OUT ( 2, 0, "o","Выход")
 };
extern "C"\
__declspec(dllexport) int kVariablesData = sizeof ( VariablesData ) / sizeof ( CObjVariablesData );


int CKO_O::TestNodes(  int kNodes, struct CObjectPoint ** ppNodes  )
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
      pOP->PntGroup = 2;  
      pOP->NumbInGroup = 0;
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

int CKO_O::SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case 1:
      {
      double * Eq = A;
      Eq[P_0] = -Omega;
      Eq[F_0] = 1. + Omega * AA;
      B[0] = -Omega * ( BB - dP_In );
      }
    break;
    case 2:  // Жидкость
      {
      double * Eq = A;
      double P_max = P_in * 2.;
      if ( P_out < P_max )
        {
        Eq[F_0] = 1.;
        B[0] = Flow_liq;
        }
      else
        {   
        // F + O * P = Fl + O * Pmax
        double O = 1e4;
        Eq[F_0] = 1.;
        Eq[P_0] = O;
        B[0] = Flow_liq + O * P_max; 
        }
      }
    break;
    }
  return 0;
  }

#include "SetVar.h"

int CKO_O::SetVar( void * pExternals, int PntGroup,  double dt, 
		double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  //pFlow_In = (CFlow*)pVar[0];
  //pFlow_Out = (CFlow*)pVar[1];
  //P_in = Var[P_0] - 1.;
  //P_out = Var[P_1] - 1.;
  switch ( PntGroup )
    {
    case 1:
      {
      pFlow_In = (CFlow*)pVar[0];
      P_in = Var[P_0];
      _P_in = P_in - 1.;
      }
    break;
    case 2:  // Жидкость
      {
      pFlow_Out = (CFlow*)pVar[0];
      P_out = Var[P_0];
      _P_out = P_out - 1.;
      }
    break;
    }
//  P = ( Eta_Gas * P + dt * ( O_In * k_In * P_in + O_Out * k_Out * P_out )) * Z;
//  M_gas = Eta_Gas * P;
  return 0;
  }

