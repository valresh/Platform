#include "stdafx.h"
#define DLL_NewComp
#include "NewComp.h"
#include "math.h"
#include "Err.h"
#include "SetData.h"


void CNewComp::ChangeComp ( CFlow * pFlow )
	{
  SET_BP
//  ChangeFlow = Change;
	if ( !Change )
    {
    pFlow->T = T_air;
    pFlow->P = 1.;
    pFlow->Flow_mol = 0.;
		return;
    }
	if ( !ChangeFlow && !AddFlow && !AddMass && !AddPress )
    {
    pFlow->T = T_air;
    pFlow->P = 1.;
    pFlow->Flow_mol = 0.;
		return;
    }
	if ( NewComp )
		SetComp();
	double T_Flow, P_Flow;
	if ( T != 0. )
		pFlow->T = T_Flow = T;
	else
		T_Flow = pFlow->T;
	if ( P != 0. )
		pFlow->P = P_Flow = P;
	else
		P_Flow = pFlow->P;
	pFlow->ID_COMP = ID_Comp;
	pFlow->pComp = pComp;

	Out.PT( P_Flow, T_Flow, pComp );
	pFlow->h_Flow = Out.h;
	if ( Flow != 0. )
		pFlow->Flow_mol = Flow;
  pFlow->To_kg = ::Mw ( pComp );
	}

void CNewComp::CalcComp( )
	{
	if ( !Change )
		return;
	if ( NewComp )
		SetComp();
	Out.PT( P, T, pComp );
	h_Flow = Out.h;
	}

CNewComp::CNewComp( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
, Out ( "Выход", this )
  {
  ID_Comp = 0;
  pComp = NULL;
	NewComp = true;
	ChangeC = false;
  Model = "NewComp";
	}


CNewComp_P::CNewComp_P()
	{
//	memset ( this, 0, sizeof ( CNewComp_W ));
  Change = false;
  In_Mass = false;
  ChangeFlow = false;
  AddFlow = false;
  AddMass = false;
  AddPress = false;
	P = 1.;
	T = 15.;
	Flow = 0.;
	h_Flow = 0.;
	CLEAR(Name)
	}

#include "IO_Parms.h"

int CNewComp::GetParams( char * )
	{
	PARM_B ( Change, "Работа", false );
//  PARM_B ( ChangeFlow, "Менять выходной поток", false );
  PARM_B ( AddFlow, "Долив", false );
  PARM_B ( AddMass, "Добавка массы", false );
  PARM_B ( AddPress, "Набор давления", false );
	PARM_D( P, "P(а)", 1. );
	PARM_D( T, "T", 15. );
	PARM_D( Flow, "F моль", 0. );
	PARM_S( Name, 64, "Состав", "Вода" );
	return 0;
	}

int CNewComp::UpdateParam( struct CParams & Param )
	{
	NewComp = true;
	ChangeC = false;
	return 0;
	}

int CNewComp::SetData( int TypeData, void * pData )
	{
	if ( TypeData == sd_SetRegData )
		{
		NewComp = true;
		ChangeC = true;
		return 1;
		}
	return 0;
	}

int CNewComp::SaveState( )
  {
//	S_CLASS("W",CNewComp_W)
  return 0;
  }

int CNewComp::RestoreState( char * StrName )
  {
//	R_CLASS("W",CNewComp_W)
  return 1;
  }

void CNewComp::SetComp()
	{
  ChangeFlow = Change;
  if ( pComp == NULL )
    {
    char NameMix[256];
    sprintf_s ( NameMix, sizeof(NameMix), "Состав %s", (char*)ObjName );
    ID_Comp = Get_ID_Data ( NameMix, CComp::TypeID ); 
    pComp = Get_Comp ( ID_Comp );
    }
	pComp->Read ( Name );
	NewComp = false;
	}
