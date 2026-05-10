#include "stdafx.h"
#include "AirDry.h"
#include "Err.h"
#include "CommProc.h"
#include "Data.h"
#include "SetData.h"

LPCSTR g_szHelpFileName = "Базовый осушитель.pdf";

CAirDry::CAirDry( char * _ObjName ) :	IBaseModel ( _ObjName )
, In ( "Вход", this )
#include "ACS_Constr.h"
#include "ACS.h"
	{
	UNIMODEL
	#include "IO_Clear.h"
	#include "Points.h"
	#include "AirDry_Defects.h"
	FlowQ = 0.0;
	}

CAirDry::~CAirDry()
	{
	}

int CAirDry::Init( int Regim )
	{
	int Res = 0;
	#include "IO_Init.h"
	#include "Points.h"
	char Name[256];
	sprintf_s ( Name, sizeof(Name), "Состав газа из %s", ObjName );
	ID_Gas_out = Get_ID_Data ( Name, CComp::TypeID );
	cGas_out = Get_Comp ( ID_Gas_out );
	cGas_out ->Set ( pN2 );
	return 0;
	}

int CAirDry::Step0( )
	{
	return 0;
	}

int CAirDry::Step1( )
	{
	return 0;
	}

int CAirDry::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CAirDry::SetData( int TypeData, void * pData )
{
	switch ( TypeData )
	{
	case sd_GetOverGasSource:
		*(double**)pData = &FlowQ; 
		return sd_GetOverGasSource;
	case sd_SetInitState:
		T = T_air;
		break;
	case sd_GetHelpPath:
		{
			lstrcpy((char*)pData, HELP_PATH);
			lstrcat((char*)pData, g_szHelpFileName);
			return 1;
		}
	}
	return 1;
}

int CAirDry::SaveState( )
  {
	S_CLASS("W",_W)
  return 0;
  }

int CAirDry::RestoreState( char * StrName )
  {
	R_CLASS("W",_W)
  return 1;
  }
