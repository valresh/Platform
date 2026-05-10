#include "stdafx.h"
#include "KO.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
//#include "SetData.h"
#include "SetDataTypes.h"

LPCSTR g_szHelpFileName = "Конденсатоотводчик.pdf";

CKO::CKO( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
, In ("Вход",this)
	{
  UNIMODEL
	lstrcpy ( ObjName, _ObjName );
	#include "IO_Clear.h"
	#include "Points.h"
  pFlow_In = NULL;
  pFlow_Out = NULL;
  pComp_In = NULL;
  pComp_Out = NULL;
  ID_pComp_In = 0;
  ID_pComp_Out = 0;
	}

CKO::~CKO()
	{
	}

int CKO::Init( int Regim )
	{
	int Res = 0;
	#include "IO_Init.h"
	#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
  P = 1.;
  P_Gas = 1.;
  T = 15.;
  AA = 0.;
  BB = 1.;
  P_in = 1.;
  P_out = 1.;
//  CLEAR(X)
//  CLEAR(Y)
//  Y[G_N2] = 1.;
//  IsComp[G_N2] = true;
//  M_gas = P / Ksi_Gas;
//  NewComp ( &ID_pComp_In, &pComp_In, "Вход %s", ObjName );
//  NewComp ( &ID_pComp_Out, &pComp_Out, "Выход %s", ObjName );
//  O_In = 1000.;
//  k_In = 1.;
//  k_Out = 0.;
//  K_dM = 1.;
  Full = false;
	return 0;
	}

int CKO::Step0( )
	{
	return 0;
	}

int CKO::Step1( )
	{
//  M_gas_old = M_gas;
  //CLockParams Lock;
  //GetParams( NULL );
  //void * P = Fopen ( "Params.csv", "wt" );
  //char Tabs[8][128];
  //int Pos = 0;
  //for ( int n = 0; n < CParams::kParams; n++ )
  //  {
  //  CParams & Parm = CParams::Params[n];
  //  if ( Parm.Type == 'T' )
  //    {
  //    lstrcpy ( Tabs[Pos++], Parm.ParamName );
  //    for ( int n = 0; n < Pos; n++ )
  //      {
  //      Fprintf ( P, "%s", Tabs[n] );
  //      if ( n < Pos - 1 )
  //        Fprintf ( P, "/" );
  //      }
  //    Fprintf ( P, ";\n" );
  //    continue;
  //    }
  //  if ( Parm.Type == 'E' )
  //    {
  //    Pos--;
  //    continue;
  //    }
  //  if ( Parm.ParamName[0] == '#')
  //    Fprintf ( P, ";%s;\n", Parm.ParamName + 1 );
  //  else
  //    Fprintf ( P, ";%s;\n", Parm.ParamName );
  //  }
  //Fclose ( P );
	return 0;
	}

int CKO::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CKO::SetData( int TypeData, void * pData )
{
	switch ( TypeData )
	{
	case sd_GetHelpPath:
		{
			lstrcpy((char*)pData, HELP_PATH);
			lstrcat((char*)pData, g_szHelpFileName);
			return 1;
		}
	}
	return 1;
}

int CKO::SaveState( )
  {
	S_CLASS("W",_W)
  return 0;
  }

int CKO::RestoreState( char * StrName )
  {
  IGNORE
	R_CLASS("W",_W)
  return 1;
  }
