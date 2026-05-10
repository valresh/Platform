#include "stdafx.h"
#include "HeatAntiFlow.h"
#include "Err.h"
#include "CommProc.h"
#include "Data.h"
#include "SetData.h"


CHeatAntiFlow::CHeatAntiFlow( char * _ObjName ) :	IBaseModel ( _ObjName ) 
, Heat("Теплообмен", this )
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
  Heat_Defect = false;
  k_Heat = 1.;
  Omega_Defect = false;
  k_Omega = 1.;
  In_Trub = NULL;
	Out_Trub = NULL;
  In_Vol = NULL;
	Out_Vol = NULL;
  Def_Trub = 0.;
  Def_outTrub = 0.;
  FlowQ = 0.;
  kFlow_1 = 1.;
  kFlow_2 = 1.;
  kDefectFlow = 0.;
  DefectMix = false;
  Comp_Vol = NULL;
  Comp_Trub = NULL;
  ID_Comp_Vol = 0;
  ID_Comp_Trub = 0;
  Переток_Trub = 0.;
  Переток_Vol = 0.;
  P_Аневризма = 1.;
  HaveProperties = true;
	}

CHeatAntiFlow::~CHeatAntiFlow()
	{
	}

int CHeatAntiFlow::Init( int )
	{
	int Res = 0;
  NewComp( &ID_Comp_Vol, &Comp_Vol, "Состав объема %s", ObjName );
  NewComp( &ID_Comp_Trub, &Comp_Trub, "Состав %s", ObjName );
  Comp_Vol->Set( pN2 );
  Comp_Trub->Set( pN2 );
  Char<128>Name;
  Name.Prt( "Поток на вход труб %s", ObjName );
  ID_In_Trub = Get_ID_Data ( Name, CFlow::TypeID );
  In_Trub = Get_Flow( ID_In_Trub );
  *In_Trub = *pN2Flow;
  Name.Prt( "Поток с выхода труб %s", ObjName );
  ID_Out_Trub = Get_ID_Data ( Name, CFlow::TypeID );
  Out_Trub = Get_Flow( ID_Out_Trub );
  *Out_Trub = *pN2Flow;
  Name.Prt( "Поток на вход объема %s", ObjName );
  ID_In_Vol = Get_ID_Data ( Name, CFlow::TypeID );
  In_Vol = Get_Flow( ID_In_Vol );
  *In_Vol = *pN2Flow;
  Name.Prt( "Поток с выхода объема %s", ObjName );
  ID_Out_Vol = Get_ID_Data ( Name, CFlow::TypeID );
  Out_Vol = Get_Flow( ID_Out_Vol );
  *Out_Vol = *pN2Flow;
  Name.Prt( "Состав смешивапния в %s", ObjName );
  ID_Comp_Mix = Get_ID_Data ( Name, CComp::TypeID );
  Comp_Mix = Get_Comp( ID_Comp_Mix );
  *Comp_Mix = *pN2;
  Name.Prt( "Состав смеси в %s", ObjName );
  ID_Comp_Аневризма = Get_ID_Data ( Name, CComp::TypeID );
  Comp_Аневризма = Get_Comp( ID_Comp_Аневризма );
  *Comp_Аневризма = *pWater;
  h_Аневризма = pWaterFlow->h_Flow;
	return 0;
	}

int CHeatAntiFlow::Step0( )
	{
	T_out_Vol = T_air;
	T_out_Trub = T_air;
 // Fsend ( "Опасные объекты.csv", "%s;Да;\n", ObjName );
 // Fsend ( "Теплообменники.csv", "%s;1000.;1000.;Да;Да;\n", ObjName );
	return 0;
	}

#undef INT_REG
#define INT_REG(Var,Desc) \
  if ( Var != 0. )\
    Reg_##Var = Var, Reg_##Var.Use = true;\
  else\
    Reg_##Var.Use = false;
int CHeatAntiFlow::Step1( )
	{
  //INT_REG( T_Z_Trub, "Заданная T труб" )
  //INT_REG( T_Z_Vol, "Заданная T объема" )
  //INT_REG( dP_trub_Z, "Заданный перепад труб" )
  //INT_REG( dP_vol_Z, "Заданный перепад объема" )
  //INT_REG( F_trub_Z, "Заданный поток труб кг" )
  //INT_REG( F_vol_Z, "Заданный поток в объем кг" )
  //INT_REG( Lam_trub_Z, "Заданная Lam труб")
  //INT_REG( Lam_vol_Z, "Заданная Lam объема")
	return 0;
	}

#undef INT_REG
#define INT_REG(Var,Desc) \
  if ( Reg_##Var.On())\
  Var = Reg_##Var;\
  else\
  Var = 0.;

int CHeatAntiFlow::StepT ( double dt )
	{
  //INT_REG( T_Z_Trub, "Заданная T труб" )
  //INT_REG( T_Z_Vol, "Заданная T объема" )
  //INT_REG( dP_trub_Z, "Заданный перепад труб" )
  //INT_REG( dP_vol_Z, "Заданный перепад объема" )
  //INT_REG( F_trub_Z, "Заданный поток труб кг" )
  //INT_REG( F_vol_Z, "Заданный поток в объем кг" )
  //INT_REG( Lam_trub_Z, "Заданная Lam труб")
  //INT_REG( Lam_vol_Z, "Заданная Lam объема")
  Calc ( dt );
	return 0;
	}

#undef IF
#define IF(Parm) if ( strcmp ( pP->Name, Parm ) == 0 && pP->Value[0] ) {
#undef EIF
#define EIF return 0; }

int CHeatAntiFlow::SetData( int TypeData, void * pData )
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
      if ( V > 10. )
        S = V;
    EIF
    return 1;
    }
	return 1;
	}

int CHeatAntiFlow::SaveState( )
  {
	S_CLASS("W_HC",_W)
  return 0;
  }

int CHeatAntiFlow::RestoreState( char * StrName )
  {
	IGNORE
	R_CLASS("W_HC",_W)
  return 1;
  }
