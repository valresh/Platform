#include "stdafx.h"
#include "Ejector.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"

CEjector::CEjector( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
, In ( "Вход", this )
, Out ( "Выход", this )
, Add ( "Подсос", this )
	{
  UNIMODEL
	lstrcpy ( ObjName, _ObjName );
	#include "IO_Clear.h"
	#include "Points.h"
    #include "Ejector_Defects.h"
  p_In = NULL;
  p_Podsos = NULL;
  p_Out = NULL;
  FlowQ = 0.;
  Defect = 0;
  O_Razgerm = 0.;
  Mw_Mix = 18.;
  Cp_H2O = 2000.;
  Cp_Add = 2000.;
  Sp = 5e-2;
  Gamma = 1.3;
	}

CEjector::~CEjector()
	{
	}

int CEjector::Init( int Regim )
	{
	int Res = 0;
	#include "IO_Init.h"
	#include "Points.h"
  char Name[256];
  sprintf_s ( Name, sizeof(Name), "Выход %s", ObjName.Str );
  ID_CompOut = Get_ID_Data ( Name, CComp::TypeID );
  pCompOut = Get_Comp ( ID_CompOut );
  pCompOut->Set ( pN2 );
  h_max = ( ( P_out_nom + 1. ) - P_cam_min ) / ( ( P_in_nom + 1. ) - P_cam_min );
	return 0;
	}

int CEjector::Step0( )
	{
//  Fsend ( "Опасные объекты.csv", "%s;Да;\n", ObjName );
	return 0;
	}

int CEjector::Step1( )
	{
/* */
  CLockParams Lock;
  GetParams( NULL );
  void * P = Fopen ( "Ejector.csv", "wt" );
  char Tabs[8][128];
  int Pos = 0;
  for ( int n = 0; n < CParams::kParams; n++ )
    {
    CParams & Parm = CParams::Params[n];
    if ( Parm.Type == 'T' )
      {
      lstrcpy ( Tabs[Pos++], Parm.ParamName );
      for ( int n = 0; n < Pos; n++ )
        {
        Fprintf ( P, "%s", Tabs[n] );
        if ( n < Pos - 1 )
          Fprintf ( P, "/" );
        }
      Fprintf ( P, ";\n" );
      continue;
      }
    if ( Parm.Type == 'E' )
      {
      Pos--;
      continue;
      }
    if ( Parm.ParamName[0] == '#')
      Fprintf ( P, ";%s;\n", Parm.ParamName + 1 );
    else
      {
      char Txt[256];
      Parm.Out ( Txt );
      Fprintf ( P, ";%s;%s;\n", Parm.ParamName, Txt );
      }
    }
  Fclose ( P );
/**/
	return 0;
	}

int CEjector::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CEjector::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_GetOverGasSource )
    {
    *(double**)pData = &FlowQ; 
    return sd_GetOverGasSource;
    }
	return 1;
	}

int CEjector::SaveState( )
  {
	S_CLASS("W",_W)
  return 0; 
  }

int CEjector::RestoreState( char * StrName )
  {
	COPY_FIRST
	R_CLASS("W",_W)
  return 1;
  }
