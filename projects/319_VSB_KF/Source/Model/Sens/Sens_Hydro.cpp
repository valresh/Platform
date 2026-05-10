#include "stdafx.h"
#include "Sens.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"

//#include "Work.h"


int CSens::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes != 2 )
    {
    ModMsg ( "У датчика '%s' число входов не равно двум", (char*)ObjName );
    return 1;
    }
  Char <32>PntName;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    pOP->NumbInGroup = n;
    pOP->PntGroup = 1;
    }
  ppNodes[0]->IO = '+';
  ppNodes[1]->IO = '-';
  return 0;
  }
#include "SetMatr.h"

int CSens::SetMatr( void * pExternals, int PntGroup,  double dt, 
				   double A[/* kExkV */], double B[ /*kE*/ ] )
{
	SET_BP BreakPoint;
  if ( PntGroup == 2 )
    KKK();
  PntGroup = 1;
	switch ( PntGroup )
	{
	case 1:
		{  
		double * Eq = A;
		Eq[P_0] = -Omega;
		Eq[P_1] = Omega;
		Eq[F_0] = 1.0;
		// 2 ур-e
		Eq += 4;
		Eq[F_0] = 1.0;
		Eq[F_1] = -1.0;
		}
		break;
	default:
		ASS(0)
			break;
	}
	return 0;
}

#include "SetVar.h"

int CSens::SetVar( void * pExternals, int PntGroup, double dt,
				  double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	SET_BP BreakPoint;
	switch ( PntGroup )
	{
	case 1:
		{
			pF = (CFlow*)pVar[0];
			CFlow * pF_1 = (CFlow*)pVar[1];
			F_mol = Var[F_0];
			if ( F_mol > 0. )
				*pF_1 = *pF;
			else
				*pF = *pF_1;
			Pin = Var[P_0];
			Pout = Var[P_1];
			dP = fabs ( Pin - Pout ) * Pascal;

		}
		return 0;
	default:
		ASS(0)
			break;
	}
	return 0;
}
