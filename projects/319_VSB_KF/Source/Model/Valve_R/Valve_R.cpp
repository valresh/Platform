#include "stdafx.h"
#include "Valve_R.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

LPCSTR g_szHelpFileName = "Базовый обратный клапан.pdf";

CValve_R::CValve_R( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
 ,Перепад( "Перепад", this, NULL )
	{
  UNIMODEL
  ObjName = _ObjName;
  Model = "Valve_R";
	#include "IO_Clear.h"
	#include "Points.h"
    #include "Valve_R_Defects.h"
  DefOpen = false;
  pFlow_In = NULL;
	}

CValve_R::~CValve_R()
	{
	}

int CValve_R::Init( int Regim )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	return 0;
	}

int CValve_R::Step0( )
	{
	return 0;
	}

int CValve_R::Step1( )
	{
  Перепад.SetD( Dy );
  if ( GasKlap )
    {
    Перепад.SetGasEtalon( pN2, F_gas * iM3, P_gas + 1., dP_gas, T_gas );
    }
  else
    {
    Перепад.SetLiqEtalon( pWater, F_liq * 1000. / Mw_H2O, dP_liq, T_liq );
    }
	return 0;
	}

int CValve_R::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CValve_R::SetData( int TypeData, void * pData )
{
    // switch ( TypeData )
    // {
    // case sd_GetHelpPath:
    // 	{
    // 		lstrcpy((char*)pData, HELP_PATH);
    // 		lstrcat((char*)pData, g_szHelpFileName);
    // 		return 1;
    // 	}
    // }
	return 1;
}

int CValve_R::SaveState( )
  {
	S_CLASS("W",_W_Valve_R)
  return 0;
  }

int CValve_R::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W",_W_Valve_R)
  return 1;
  }
