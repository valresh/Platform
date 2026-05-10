#include "stdafx.h"
#define DLL_LS
#include "LS.h"
#include "Err.h"
#include "SetData.h"
//#include "CommProc.h"
//#include "Data.h"
//#include "SetData.h"

CLS * CLS::pFirstLS = NULL;
CLS * CLS::pLastLS = NULL;

CLS::CLS( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
//  pSens = NULL;
  pValue = NULL;
  if ( pFirstLS == NULL )
    pFirstLS = this;
  if ( pLastLS )
     pLastLS->pNextLS = this; 
  pLastLS = this;
  pNextLS = NULL;
  pBaseSens = NULL;
	}

CLS::~CLS()
	{
	}

int CLS::Init( int Regim )
	{
  char * Name = ObjName;
  if ( *Name == '_' )
    Name++;
  //GiveCommData ( Name, CSensor::TypeID, (void**)&pSens );
  //lstrcpy ( pSens->SensorName, ObjName );
  //ASS(pSens)
	return 0;
	}

double * Connect( char * Pnt );
int CLS::Step0( )
	{
  if ( ObjName[0] == '_' )
    pBaseSens = IBaseModel::Find( (char*)(ObjName.Str + 1 ));
  if ( pBaseSens )
    {
    pBaseSens->SetData( sd_GetMes, &MesZ );
    }
  if ( Ref[0] )  //CLS::Step1
    {
    //    Char<256>Name;
    //    Name = Ref;
    pValue = ::Connect( Ref );
    }
	return 0;
	}

double * Connect( char * Pnt )
  {
  if ( Pnt == NULL || Pnt[0] == 0 )
    return NULL;
  char Point[256];
  lstrcpy ( Point, Pnt );
  char * P = strchr ( Point, '.' );
  if ( P == NULL )
    return NULL;
  *P = 0;
  char * Obj = Point;
  IBaseModel * pModel = IBaseModel::Find ( Obj );
  if ( pModel == NULL )
    return NULL;
  char * Param = P + 1;
  if ( Param[0] == '@' )
    {
    // ACS
    Param++;
    CExtern_Pnt * pPnt = (CExtern_Pnt*)::Find( &pModel->Points, Param, false );
    if ( pPnt == NULL )
      return NULL;
    if ( pPnt->Type[0] != 'A' )
      return NULL;
    CAnalog * pA = (CAnalog*)pPnt;
    double * pAddr = (double*)&(pA->Value);
    return pAddr;
    }
  CLockParams Lock;
  pModel->GetParams( NULL );
  for ( int n = 0; n < CParams::kParams; n++ )
    {
    CParams & Parm = CParams::Params[n];
    if ( lstrcmp ( Parm.ParamName, Param ) == 0 && Parm.Type == 'D' )
      {
      double * pAddr = (double*)Parm.Addr;
      return pAddr;
      }
    }
  return NULL;
  }

int CLS::Step1( )
	{
	return 0;
	}

int CLS::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CLS::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CLS::SaveState( )
  {
	S_CLASS("W",_W_LS)
  return 0;
  }

int CLS::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W",_W_LS)
  return 1;
  }
