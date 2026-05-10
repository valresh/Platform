#include "stdafx.h"
#include "Drainage.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"

NODE_LIST
  NODE_IN( 1, 0, "g", "Сброс газа")
  NODE_IN( 2, 0, "l", "Дренаж жидкости")
  NODE_IN( 2, 0, "i", "Дренаж жидкости")
END_LIST

STD_TEST


int CDrainage::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  int Res = ::StdTestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  if ( kNodes != 1 )
    {
    ModMsg ( "У дренажа '%s' неправильные входы", ObjName.Str );
    return 1;
    }
  CObjectPoint * pOP = ppNodes[0];
  pOP->Type = eГидравлика;
  if ( pOP->Point[0] == 'g' )
    {
    Type = Газ;
    return 0;
    }
  if ( pOP->Point[0] == 'l' ||  pOP->Point[0] == 'i' )
    {
    Type = Жидкость;
    return 0;
    }
  ModMsg ( "У дренажа '%s' неправильные входы", ObjName.Str );
  return 1;
  }

#include "SetMatr.h"

int CDrainage::SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] )   // A * X = B
  {
  switch ( PntGroup )
    {
    case 1:
	case 2:
      {
		  double O;
		  if(Ventil <= 0.0)
			 O = K_Drain * К_Reg * 1e-9 * 0.01;
		  else 
			  O = K_Drain * К_Reg * Ventil * 0.01; 
		  A[P_0] = O;
		  A[F_0] = -1.;
		  B[0] = O; 
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

#include "SetVar.h"

int CDrainage::SetVar( void * pExternals, int PntGroup, double dt, 
    // Переменные на входах
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  switch ( PntGroup )
    {
    case 1:
      {
      P_In = Var[P_0] - 1.;
      Flow_mol = F = Var[F_0];
      if ( Flow_mol < 0. )
        {
        CFlow * pFlow = (CFlow*)pVar[0];
        *pFlow = *pAirFlow;
        }
      }
    return 0;
    case 2:
      {
      P_In = Var[P_0] - 1.;
      Flow_mol = F = Var[F_0];
      if ( Flow_mol < 0. )
        {
        CFlow * pFlow = (CFlow*)pVar[0];
        *pFlow = *pAirFlow;
        }
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

