#include "stdafx.h"
#include "Dynamic.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

CDynamic::CDynamic( char * _ObjName ) :	CPump_b ( _ObjName ) 
#include "ACS_Constr.h"
#include "Dynamic_ACS.h"
{
	UNIMODEL
	ObjName = _ObjName;
	kDef = 1.0;
	M_g = 28.0;
	M_l = 18.0;	
	T = T_air;
	pGas[G_H2O]->Ro_liq(T, ro_l);	
	bRest = false;
	p_i = 1.0;
	p_o = 1.0;
	Eps.Mw_liq = M_l;
	Eps.Mw_gas = M_g;
#include "IO_Clear.h"
#include "Dynamic_Pnt.h"
#include "Dynamic_Defects.h"
}

CDynamic::~CDynamic()
{

}

int CDynamic::Init( int Regim )
{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	return 0;
}

int CDynamic::Step0( )
{
	return 0;
}

int CDynamic::Step1( )
{
	CalcCharacteristic();
	return CPump_b::Step1();
}

int CDynamic::StepT ( double dt )
{
	Calc ( dt );
	return 0;
}

int CDynamic::SetData( int TypeData, void * pData )
{
	if(TypeData == sd_SetEquipParam)
	{
		CEquipParam* pEP = (CEquipParam*)pData;
		double Value = atof(pEP->Value);
		if(!strcmp(pEP->Name, "Подача"))
			Q_nom = Value;
		else if(!strcmp(pEP->Name, "Напор"))
			p_nom = Value;
		CalcCharacteristic();
	}
	return CPump_b::SetData(TypeData, pData);
}

int CDynamic::SaveState( )
{
	if(CPump_b::SaveState())
		return 1;
	S_CLASS("_W_Dynamic", _W_Dynamic);
	return 0;
}

int CDynamic::RestoreState( char * StrName )
{
	COPY_FIRST;
	if(!lstrcmp(StrName, "_W_Pump_b"))
		return CPump_b::RestoreState(StrName);
	R_CLASS("_W_Dynamic", _W_Dynamic);
	return 1;	
}

int CDynamic::DstMM( char * DataName, struct IMM * pMM )
{
	return CPump_b::DstMM(DataName, pMM);
}

int CDynamic::SrcMM( char * DataName, struct IMM * pMM )
{
	return CPump_b::SrcMM(DataName, pMM);
}



