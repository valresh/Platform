#include "stdafx.h"
#include "Pump_b.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"
#include "MM.h"

LPCSTR g_szHelpFileName = "Базовый насос.pdf";

const double CPump_b::H2O_To_m3 = 0.017968722658731808;
const double CPump_b::AtmToPa = 101325.0;
const double CPump_b::HourToSec = 3600.0;
const double CPump_b::kMolToMol = 1000.0;
const double CPump_b::g = 9.81;
const double CPump_b::p_i_min = 1e-5;

CPump_b::CPump_b( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "Pump_b_ACS.h"
, Eps("Вход", this )
{
	UNIMODEL;
		//////////////////////////////////////////////////////////////////////////
	_P_In = 0.0;
	_Flow_Work_m3 = 0.0;
	_Flow_Work_kg = 0.0;
	K_F = 1.0;
	Номинальный_ток = 100.0;
	Use_Motor = true;
	//////////////////////////////////////////////////////////////////////////
	ObjName = _ObjName;
	pFlow_i = NULL;
	pFlow_o = NULL;
	pFlow_i_cool = NULL;
	pFlow_o_cool = NULL;
	FlowQ = 0.0;
	pMotor = NULL;
	bIgnoreBlk = false;
	nDefect = 0;
	static CMotorData freeMotor;
    pMotor = &freeMotor;
	bZapret = false;
	IsElectro = true;
    if ( strstr ( ObjName, "Р-3002S1" ))
        KKK();
	//ClassRootNode = pMain->C
}

CPump_b::~CPump_b()
{

}

int CPump_b::Init( int Regim )
{
	return 0;
}

int CPump_b::Step0( )
{
	return 0;
}

int CPump_b::Step1( )
{
 	return 0;
}

int CPump_b::SetData( int TypeData, void * pData )
{
	switch ( TypeData )
	{
	case sd_GetOverGasSource:
		{
			*(double**)pData = &FlowQ; 
			return sd_GetOverGasSource;
		}
	case sd_GetHelpPath:
		{
			lstrcpy((char*)pData, HELP_PATH);
			lstrcat((char*)pData, g_szHelpFileName);
			break;
		}
	case sd_SetInitState:
		break;
	}
	return 1;
}

int CPump_b::SaveState( )
{
	S_CLASS("_W_Pump_b",_W_Pump_b);
	return 0;
}

int CPump_b::RestoreState( char * StrName )
{
	COPY_FIRST;
	R_CLASS("_W_Pump_b",_W_Pump_b);
	return 1;
}

int CPump_b::DstMM( char * DataName, struct IMM * pMM )
{
    if ( strstr ( ObjName, "Р-3002S1" ))
        KKK();
    if (!lstrcmp( DataName, "Motor"))
	{
		ASS( pMM->ID == CMotorData::TypeID );
		pMotor = (CMotorData*)pMM->pData;
		if(pMotor->ppPower)
			*pMotor->ppPower = &power;
		return 0;
	}
	return 1;
}

int CPump_b::SrcMM( char * DataName, struct IMM * pMM )
{
    if ( strstr ( ObjName, "Р-3002S1" ))
        KKK();
    if ( lstrcmp( DataName, "Motor" ) == 0 )
	{
		pMM->ID = CMotorData::TypeID;
		pMM->pData = &_pMotor->MotorData;;
		pMM->ObjName = ObjName;
		return 0;
	}
	return 1;
}
