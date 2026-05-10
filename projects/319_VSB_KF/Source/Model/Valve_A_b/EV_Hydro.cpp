#include "stdafx.h"
#include "EV.h"
#include "Err.h"
#include "CommProc.h"
#include "SetMatr.h"
#include "Work.h"

int VA_StdTestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes );

int CEV::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::VA_StdTestNodes(ObjName, kNodes, ppNodes);
	if(Res)
		return Res;
	return CValve_A_b::TestNodes(kNodes, ppNodes);
}