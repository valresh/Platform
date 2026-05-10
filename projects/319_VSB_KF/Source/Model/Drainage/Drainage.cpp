#include "stdafx.h"
#include "Drainage.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"

LPCSTR g_szHelpFileName = "Базовый дренаж.pdf";

CDrainage::CDrainage( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
    #include "Drainage_Defects.h"
  Defect = 0;
	}

CDrainage::~CDrainage()
	{
	}

int CDrainage::Init( int Regim )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
	return 0;
	}

int CDrainage::Step0( )
	{
	return 0;
	}

int CDrainage::Step1( )
	{
  CalcK( );
  return 0;
	}

int CDrainage::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CDrainage::SetData( int TypeData, void * pData )
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

int CDrainage::SaveState( )
  {
	S_CLASS("W",_W_Drain)
  return 0;
  }

int CDrainage::RestoreState( char * StrName )
  {
	R_CLASS("W",_W_Drain)
  return 1;
  }

/*
Замены для перехода на новую версию
#include "SetMatr.h"
double * Eq = A;
CObjectPoint  CObjectPoint 
Press[0] Var[P_0]
Press[1] Var[P_1]
Flow[0]  Var[F_0]
Var[F_1]  Var[F_1]
V[0] B[0]
V[1] B[1]
Mp[0] Eq[P_0]
Mp[1] Eq[P_1]
Mf[0] Eq[F_0]
Mf[1] Eq[F_1]
Mf[2] Eq[F_0]  Eq += 4 
Mf[3] Eq[F_1]  Eq += 4
MP[0][0] Eq[P_0]
MP[0][1] Eq[P_1]
MP[1][0] Eq[P_0]
MP[1][1] Eq[P_1]
MF[0][0] Eq[F_0]
MF[0][1] Eq[F_1]
MF[1][0] Eq[F_0]
MF[1][1] Eq[F_1]
Get_Flow( ID_Flow[0] )  (CFlow*)pVar[0] 
Get_Flow( ID_Flow[1] )  (CFlow*)pVar[1]
*/
