#include "stdafx.h"
#include "VEP.h"
#include "Err.h"
#include "CommProc.h"
#include "SetMatr.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"

int StdTestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes );

int CVEP::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::StdTestNodes(ObjName, kNodes, ppNodes);
	if(Res)
		return Res;
	return CValve_b::TestNodes(kNodes, ppNodes);
}


double CVEP::Calc_Omega(Characteristic eCharacteristic, double _Position)
{
	double _Omega = 0.0; 
	if( Plug == false )
		_Omega = CValve_M_b::Calc_Omega(eCharacteristic, _Position);		
	return _Omega;

}
