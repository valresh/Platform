#include "stdafx.h"
#include "VBR.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "TestNodes.h"
#include "SetMatr.h"
#include "SetVar.h"
#include "Work.h"


NODE_LIST
NODE_IN(1, 0, "i", "Вход полипропилена")
NODE_OUT(1, 1, "o", "Выход полипропилена")
END_LIST

STD_TEST

STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
	return ::StdTestNodes( (char*)ObjName, kNodes, ppNodes );
}


int CVBR::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::TestNodes(ObjName, kNodes, ppNodes);
	if ( Res )
		return Res;
	return 0;
}


int CVBR::SetMatr(  void * pExternals, int PntGroup,  double dt, 
						 double A[/* kExkV */], double B[ /*kE*/ ] )
{
	double * Eq = A;
	Eq[P_0] = Omega;
	Eq[P_1] = -Omega;
	Eq[F_0] = -1.;
	Eq += 4;
  if ( !_finite( k_Flow_IO ))
     k_Flow_IO = 1.; 
	Eq[F_0] = k_Flow_IO;
	Eq[F_1] = -1.;     //  k_Flow * F_in - F_out = 0
	return 0;
}

int CVBR::SetVar( void * pExternals, int PntGroup, double dt, 
						// Переменные на входах
						double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	IO_2(pFlow_in,pFlow_out)
		return 0;
}

