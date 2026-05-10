#include "stdafx.h"
#include "HeatChange.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"

char * Help = "Базовый теплообменник.pdf";


CHeatChange::CHeatChange( char * _ObjName ) :	IBaseModel ( _ObjName ) 
, Heat("Теплообмен", this )
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
    #include "HeatChange_Defects.h"
  Omega_Defect = false;
  k_Omega = 1.;
  K_Defect = 1.;
  Def_Trub = 0.;
  Перемешивание = false;
  In_Trub_0 = NULL;
  In_Vol_0 = NULL;
  In_Trub = NULL;
	Out_Trub = NULL;
  In_Vol = NULL;
	Out_Vol = NULL;
  dP_Hydro = 0.;
  Def_Zasor = false;
  FlowQ = 0.;
  kAntiFlow = 1.;
//
  K_heat = 0.;
  bRegTask = false;
	}

CEtalon::CEtalon()
  {
  D = 0.04;
  S = 0.01;
  Re_Etalon = 0.;
  Pr_Etalon = 0.;
  Nu_Etalon = 0.;
  Alfa_Etalon = 0.;
  Xi_Etalon = Xi = 1.;
  }

CHeatChange::~CHeatChange()
	{
	}

int CHeatChange::Init( int )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
  In_Vol = new CFlow();
  In_Trub = new CFlow();
  NewComp( &ID_Comp_Vol, &pCompVol, "Состав при смешении в объеме %s", ObjName.Str );
  NewComp( &ID_Comp_Trub, &pCompTrub, "Состав при смешении в трубе %s", ObjName.Str );
	return 0;
	}

int CHeatChange::Step0( )
	{
//  Etalon_Trub.CalcH2OCoef( );
//  Etalon_Vol.CalcH2OCoef( );
//  CalcBaseCoef( );
	return 0;
	}

#undef INT_REG
#define INT_REG(Var,Desc) \
  if ( Var != 0. )\
  Reg_##Var = Var, Reg_##Var.Use = true;\
  else\
  Reg_##Var.Use = false;
int CHeatChange::Step1( )
	{
  //INT_REG( T_Z_Trub, "Заданная T труб" )
  //INT_REG( T_Z_Vol, "Заданная T объема" )
  //INT_REG( dP_trub_Z, "Заданный перепад труб" )
  //INT_REG( dP_vol_Z, "Заданный перепад объема" )
  //INT_REG( F_trub_Z, "Заданный поток труб кг" )
  //INT_REG( F_vol_Z, "Заданный поток в объем кг" )
  //dP_vol_Z = 0.;
  //dP_Hydro = 0.;
  Heat.kAntiFlow = 1. - kAntiFlow;
  Heat.dQ = 5000.;
  Heat.QQQ = 0.;
  //Heat.Работа_через_Cp = true;
	return 0;
	}

#undef INT_REG
#define INT_REG(Var,Desc) \
  if ( Reg_##Var.On())\
    Var = Reg_##Var;\
  else\
    Var = 0.;

int CHeatChange::StepT ( double dt )
	{
  //INT_REG( T_Z_Trub, "Заданная T труб" )
  //INT_REG( T_Z_Vol, "Заданная T объема" )
  //INT_REG( dP_trub_Z, "Заданный перепад труб" )
  //INT_REG( dP_vol_Z, "Заданный перепад объема" )
  //INT_REG( F_trub_Z, "Заданный поток труб кг" )
  //INT_REG( F_vol_Z, "Заданный поток в объем кг" )
  Calc ( dt );
	return 0;
	}
#undef IF
#define IF(Parm) if ( strcmp ( pP->Name, Parm ) == 0 && pP->Value[0] ) {
#undef EIF
#define EIF return 0; }

int CHeatChange::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_GetOverGasSource )
    {
    *(double**)pData = &FlowQ; 
    return sd_GetOverGasSource;
    }
  if ( TypeData == sd_SetEquipParam )
    {
    CEquipParam * pP = (CEquipParam*)pData;
    IF("Площадь")
      double V = atof( pP->Value );
      if ( V > 100. )
        S = V;
    EIF
    return 1;
    }
	if ( TypeData == sd_GetHelpPath )
		{
	  lstrcpy((char*)pData, HELP_PATH );
		lstrcat((char*)pData, Help );
		return 1;
		}
  if ( TypeData == sd_SetInitState )
    {
    T_in_Trub = T_in_Vol = T_air;
    T_out_Trub = T_out_Vol = T_air;
    Heat.T_Corp = T_air;
    Heat.QQQ = Heat.dQ = 0.;
    Heat.T_in_Vol_Air  = T_air;
    Heat.h_out_Trub = Heat.h_out_Vol = 0.;
    Heat.Q_prev = 0.;
    Heat.T_in_Trub = Heat.T_in_Vol = T_air;
    Heat.T_out_Trub = Heat.T_out_Vol = T_air;
    Heat.T_Corp = T_air;
		return 0;
    }
	return 1;
	}

int CHeatChange::SaveState( )
  {
	S_CLASS("W",_W)
  return 0;
  }

int CHeatChange::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W",_W)
  return 1;
  }
