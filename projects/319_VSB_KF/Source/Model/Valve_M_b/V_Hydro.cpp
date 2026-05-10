#include "stdafx.h"
#include "V.h"
#include "Err.h"
#include "CommProc.h"
#include "SetMatr.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"

int ValveStdTestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes );

int CV::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
    int Res = ::ValveStdTestNodes(ObjName, kNodes, ppNodes);
    if(Res)
        return Res;
    return CValve_b::TestNodes(kNodes, ppNodes);
}
