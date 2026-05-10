#include "stdafx.h"
#include "ElectricHeater_b.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

CElectricHeater_b::CElectricHeater_b( char * _ObjName ) :	IBaseModel ( _ObjName ) 
, In("Вход",this)
, Out("Выход",this)
#include "ACS_Constr.h"
#include "ACS.h"
{
	UNIMODEL
	ObjName = _ObjName;
	pFlow_i = NULL;
	pFlow_o = NULL;
	dReg = 0.1;
	T_i = T_o = T_air;
	nDefect = 0;
	IsElectro = true;
	eSwitch = SWITCH_MEST;
#include "IO_Clear.h"
#include "Points.h"
#include "ElectricHeater_b_Defects.h"
}

CElectricHeater_b::~CElectricHeater_b()
{

}

int CElectricHeater_b::Init( int Regim )
{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	return 0;
}

int CElectricHeater_b::Step0( )
{
	return 0;
}

int CElectricHeater_b::Step1( )
{
	return 0;
}

int CElectricHeater_b::StepT ( double dt )
{
	Calc ( dt );
	return 0;
}

int CElectricHeater_b::SetData( int TypeData, void * pData )
{
	return 1;
}

int CElectricHeater_b::SaveState( )
{
	S_CLASS("W",_W_ElectricHeater_b);
	return 0;
}

int CElectricHeater_b::RestoreState( char * StrName )
{
	COPY_FIRST;
	R_CLASS("W",_W_ElectricHeater_b);
	return 1;
}












