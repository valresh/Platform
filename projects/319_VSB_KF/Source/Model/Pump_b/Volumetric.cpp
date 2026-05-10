#include "stdafx.h"
#include "Volumetric.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

CVolumetric::CVolumetric( char * _ObjName ) :	CPump_b ( _ObjName ) 
#include "ACS_Constr.h"
#include "Volumetric_ACS.h"
{
	UNIMODEL
	ObjName = _ObjName;
	pFlow_i = NULL;
	pFlow_o = NULL;
	pFlow_i_cool = NULL;
	pFlow_o_cool = NULL;
#include "IO_Clear.h"
#include "Volumetric_Pnt.h"
	F_real = F_nom * K_F;
}

CVolumetric::~CVolumetric()
{

}

int CVolumetric::Init( int Regim )
{
	return 0;
}

int CVolumetric::Step0( )
{
	return 0;
}

int CVolumetric::Step1( )
{
	return 0;
}

int CVolumetric::StepT ( double dt )
{
	Calc ( dt );
	return 0;
}

int CVolumetric::SetData( int TypeData, void * pData )
{
	return CPump_b::SetData(TypeData, pData);
}

int CVolumetric::SaveState( )
{
	return CPump_b::SaveState();
	//S_CLASS("W",_W_Volumetric)
}

int CVolumetric::RestoreState( char * StrName )
{
if ( strstr ( ObjName, "C-3001S-P1" ))
  KKK();
	//R_CLASS("W",_W_Volumetric)
	return CPump_b::RestoreState(StrName);
}











