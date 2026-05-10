#include "stdafx.h"
#include "Temp.h"
#include "Err.h"
#include "CommProc.h"
#include "SetMatr.h"
#include "Work.h"
#include "HydroStruct.h"
#include "SetMatr.h"
#include "SetVar.h"
#include "TestNodes.h"
#include "Data.h"

//NODE_IN(1,"#","Любой одиночный вход")

int CTemp::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    char * Name = pOP->PntName;
    pOP->PntGroup = n + 1;  
    pOP->NumbInGroup = 0;
    pOP->IO = '+';
//	  ModMsg( "#У временной модели: '%s' задан вход: '%s'", ObjName, Name);
    }
  return 0;
  }

int CTemp::SetMatr( void * pExternals, int PntGroup,  double dt, 
                   double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  A[P_0] = Omega;
  A[F_0] = -1.0;
  B[0] = F + Omega * P;
  return 0;
  }

int CTemp::SetVar( void * pExternals, int PntGroup, double dt, 
                  double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  CFlow * pF = (CFlow*)pVar[0];
  if ( pF && Var[F_0] < 0 )
	  *pF = *pN2Flow;
  return 0;
  }

