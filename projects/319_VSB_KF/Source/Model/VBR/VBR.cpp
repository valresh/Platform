#include "stdafx.h"
#include "VBR.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

//..\..\..\..\..\SYSTEM\EXE\Simulator.exe
//-n -s-1 -pВБ -r101_KU1_UNPZ -mModels

CVBR::CVBR( char * _ObjName ) :	IBaseModel ( _ObjName ) 
, In ( "Вход", this )
, Out_Реактор_0 ( "Выход реактора T0", this )
, Out_Реактор ( "Выход реактора T", this )
#include "ACS_Constr.h"
#include "ACS.h"
{
	UNIMODEL
		ObjName = _ObjName;
#include "IO_Clear.h"
#include "Points.h"
	pFlow_in = NULL;
	pFlow_out = NULL;
	pCompSumm = NULL;
	ID_CompSumm = 0;
	CLEAR(pComp);
	CLEAR(MwComp);
	CLEAR(ID_Comp);
	k_Gas = k_P_1 = k_P_2 = k_Flow = 1.;
	CLEAR(Cin)
	CLEAR(Cout)
	CLEAR(S)
	CLEAR(Cmol_No_Conv)
	CLEAR(IsComp_No_Conv)
	Flow_No_Conv = 0.;
	CLEAR(Cmol_Conv)
	CLEAR(IsComp_Conv)
	Flow_Conv = 0.;
	CLEAR(No_React)
	CLEAR(kComp)
	for ( int c = 0; c < LAST_GAS; c++ )
		kComp[c] = 1.;
	kCompMax = 10.;
	CLEAR(CmolOpt)
	Change_k = false;
	Change_C = false;
	kStep = 0;
	CLEAR(Etalon)
	strcpy_s(Etalon,"После печей");
	CLEAR(OutToEtalon)
	SetOptKoef = false;
}

CVBR::~CVBR()
	{
	}

int CVBR::Init( int Regim )
	{
	int Res = 0;
  NewComp ( &ID_CompSumm, &pCompSumm, "Состав из %s", ObjName );
  for ( int n = 0; n < 6; n++ )
    {
    NewComp ( &ID_Comp[n], &pComp[n], "Состав %d из %s", n, ObjName );
    }
  pComp[0]->Read ( "С1-С4" );
  pComp[1]->Read ( "С5-90" );
  pComp[2]->Read ( "90-170" );
  pComp[3]->Read ( "170-350" );
  pComp[4]->Read ( "350-550" );
  pComp[5]->Read ( "550+" );
  for ( int n = 0; n < 6; n++ )
    {
    MwComp[n] = ::Mw ( pComp[n] );
    }
	return 0;
	}

int CVBR::Step0( )
	{
	return 0;
	}

int CVBR::Step1( )
	{
  CompEtalon.Read ( Etalon );
  for ( int c = 0; c < LAST_GAS; c++ )
    kComp[c] = 1.;
	return 0;
	}

int CVBR::StepT ( double dt )
	{
  Calc ( dt );
  kStep++;
	return 0;
	}

int CVBR::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CVBR::SaveState( )
  {
	S_CLASS("W",_W_VBR)
  return 0;
  }

int CVBR::RestoreState( char * StrName )
  {
	R_CLASS("W",_W_VBR)
  return 1;
  }
