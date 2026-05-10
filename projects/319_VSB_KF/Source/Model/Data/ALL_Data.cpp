
#include "stdafx.h"
#include "ALL_Data.h"
#define ALL_DATA
#include "Data.h"
#include "Err.h"
#include "CommProc.h"


int ID_Air;
CComp * pAir;
CFlow AirFlow;
CFlow * pAirFlow = &AirFlow;
int ID_N2;
CComp * pN2;
CFlow N2Flow;
CFlow * pN2Flow = &N2Flow;
int ID_Water;
CComp * pWater;
CFlow WaterFlow;
CFlow * pWaterFlow = &WaterFlow;


CALL_Data::CALL_Data( char * _ObjName ) : IBaseModel ( _ObjName )
{
ObjName = _ObjName;
Model = "Data";
ModelFlags = Flag_NoAll;
}

CALL_Data::~CALL_Data()
{
}

void SetStdComp();

int CALL_Data::Init( int )
{
  int Res = 0;
  SetStdComp();
  return 0;
}

int CALL_Data::Step0( )
{
  return 0;
}

int CALL_Data::Step1( )
{
  /////////////////////////////////////////////////////
  return 0;
}


void NormComp();
static int kStep = 0;
int CALL_Data::StepT ( double dt )
{
  kStep++;
  if ( kStep % 50 == 0 )
    NormComp();
  return 0;
}

int CALL_Data::SetData( int TypeData, void * pData )
{
  return 1;
}

void SaveData( );

int CALL_Data::SaveState( )
{
  SaveData( );
  return 0;
}

void RestData( );

extern char * DataName;
int CALL_Data::RestoreState( char * StrName )
{
  ASS( lstrcmp ( StrName, DataName ) == 0 );
  RestData( );
  SetStdComp();
  return 0;
}

