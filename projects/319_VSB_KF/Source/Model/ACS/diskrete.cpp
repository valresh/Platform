#include "stdafx.h"
#define DLL_ACS
#include "ACS_Def.h"
#include "math.h"
#include "Err.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include "CommProc.h"
#include "Connect.h"

#ifdef LINUX
#define CParams QParams
#endif

CDiscrete::CDiscrete(IBaseModel * _pModel, char * _Point, char * _Type, char * _Descr): CExtern_Pnt( _pModel, _Point, _Type, _Descr )
{
	IsConnection = false;
	Value = 0;
	Trip = false;
	Defect = -1;
}

CDiscrete::CDiscrete()
{
	IsConnection = false;
	Value = 0;
	Trip = false;
	Defect = -1;
}

void CDiscrete::Init( IBaseModel * _pModel, char * _Point, char * _Type, char * _Descr )
{
	CExtern_Pnt::Init( _pModel, _Point, _Type, _Descr );
	Defect = -1;
}

bool CDiscrete::Connect( CConnectInfo * pInfo )
{
	if(!CompTypes(pInfo->Type)) return false;
	pInfo->pSrcAddr = &Value;
	IsConnection = true;
	return true;
}

void CDiscrete::GetParams( )
{
	char Name[256];
	if ( CParams::SaveAllParams )
		return;
	char * RSU[16];
	char * Desc[16];
	int K = 0;
    // if ( pGetAllConnections )
    // {
    // 	K = (*pGetAllConnections)( pModel->ObjName, Point, SD, RSU, Desc );
    // 	if ( K == 0 && IsConnection && Connection.Str && Connection.Str[0] == '#' )
    // 	{
    // 		K = 1;
    // 		SD = 'D';
    // 		RSU[0] = "Задано";
    // 		Desc[0] = "Задается вручную";
    // 	}
    // }
	if ( !IsConnection )
	{
		//    ASS( SD == ' ' )
		if ( CParams::GetParamsList )
		{
			sprintf_s ( Name, 256, "@%s", Point.Str );
		}
		else
		{
			if ( Type[1] == 'I' )
				sprintf_s ( Name, 256, "@%s«-*", Point.Str );
			else
				if ( Type[1] == 'O' )
					sprintf_s ( Name, 256, "@%s–»*", Point.Str );
				else
					sprintf_s ( Name, 256, "@%s", Point.Str );
		}
		CParams & P = CParams::Get();
		P.SetDefaultValue(NULL);
		P.Set( pModel, Name, Value, 4 );
		P.pModelDescr = Descr;
		P.pDefValue = Type;
		P.Class = 'A';
		return;
	}
	else
	{
		//ASS( SD == 'S' || SD == 'D' )
		if(K <= 0)
		{
			K = 1;
			RSU[0] = Connection;
		}
		for ( int n = 0; n < K; n++ )
		{
			if ( SD == 'D' )
				sprintf_s ( Name, 256, "@%s«-(%s)", Point.Str, RSU[n] );
			else
			{
				char * Pnt = Point;
				if ( Defect == 0 )
					Pnt = "0";
				if ( Defect == 1 )
					Pnt = "1";

				sprintf_s ( Name, 256, "@%s–»(%s)", Pnt, RSU[n] );
			}
			CParams & Param = CParams::Get();
			Param.SetDefaultValue(NULL);
			Param.Set( pModel, Name, Value, 4 );
			//Param.pModelDescr =  Desc[n];
			Param.Class = 'A';
		}
	}
}

int CDiscrete::operator = ( struct CAnalog & A )
  {
  Value = (int)A.Value;
  return Value;
  }

void CDiscrete::GetInfo( char _Obj[128], char _Point[64], char _ACS[64] )
  {
  _Obj[0] = 0;
  _Point[0] = 0;
  _ACS[0] = 0;
  if ( !IsConnection )
    return;
  if ( !Trip )
    return;
  if ( pModel )
    {
    strcpy_s ( _Obj, 128, pModel->ObjName );
    char * P = strchr ( _Obj, '/' );
    if ( P )
      *P = 0;
    }
  strcpy_s ( _Point, 64, Point );
  strcpy_s ( _ACS, 64, "" );
//@@  strcpy_s ( _ACS, 64, RSU );
  }
// On - Move = 1, Off - Move = -1,
void CDiscrete::KlapOpenClose( double Vent, bool LocalBlk, int & Move, bool & Err )
  {
  if ( !IsConnection )
    return;
  // On - Move = 1, Off - Move = -1,
  int Dir = 0;
  if ( On())
    Dir = 1;
  else
    Dir = -1;
  if ( LocalBlk || pSys->IsBlk )
    {
    Move = Dir;
    Trip = false;
    return;
    }
  if ( Dir > 0 && Vent <= 99. )
    {
    Trip = true;
    Err = true;
    //Set_ErrBlkInObj ( pModel->ObjName );
    return;
    }
  if ( Dir < 0 && Vent >= 1. )
    {
    Trip = true;
    Err = true;
    //Set_ErrBlkInObj ( pModel->ObjName );
    return;
    }
  Trip = false;
  }
// On - Move = 1
void CDiscrete::KlapOpen( double Vent, bool LocalBlk, int & Move, bool & Err )
  {
  if ( !IsConnection )
    return;
  // On - Move = 1
  if ( On())
    {
    if ( pSys->IsBlk || LocalBlk )
      {
      Move = 1;
      Trip = false;
      return;
      }
    if ( Vent <= 99. )
      {
      Trip = true;
      Err = true;
      //Set_ErrBlkInObj ( pModel->ObjName );
      return;
      }
    }
  Trip = false;
  }
// On - Move = -1
void CDiscrete::KlapClose( double Vent, bool LocalBlk, int & Move, bool & Err )
  {
  if ( !IsConnection )
    return;
  // On - Move = -1
  if ( On())
    {
    if ( pSys->IsBlk || LocalBlk )
      {
      Move = -1;
      Trip = false;
      return;
      }
    if ( Vent >= 1. )
      {
      Trip = true;
      Err = true;
      //Set_ErrBlkInObj ( pModel->ObjName );
      return;
      }
    }
  Trip = false;
  }
void CDiscrete::CmdStop( int State, bool LocalBlk, bool & Stop, bool & Err )
  {
  if ( !IsConnection )
    return;
  if ( On())
    {
    if ( pSys->IsBlk || LocalBlk )
      {
      Stop = true;
      Trip = false;
      return;
      }
    if ( State )
      {
      Trip = true;
      Err = true;
      //Set_ErrBlkInObj ( pModel->ObjName );
      return;
      }
    }
  Trip = false;
  }

void CDiscrete::CmdStart( int State, bool LocalBlk, bool & Start, bool & Err )
  {
  if ( !IsConnection )
    return;
  if ( On())
    {
    if ( pSys->IsBlk || LocalBlk )
      {
      Start = true;
      Trip = false;
      return;
      }
    if ( State == 0 )
      {
      Trip = true;
      Err = true;
      //Set_ErrBlkInObj ( pModel->ObjName );
      return;
      }
    else
      {
      Trip = false;
      return;
      }
    }
  Trip = false;
  }

void CDiscrete::CmdStartStop( int State, bool LocalBlk, bool & Start, bool & Stop, bool & Err )
  {
  if ( !IsConnection )
    return;
  if ( On())
    {
    if ( pSys->IsBlk || LocalBlk )
      {
      Start = true;
      Trip = false;
      return;
      }
    if ( State == 0 )
      {
      Trip = true;
      Err = true;
      //Set_ErrBlkInObj ( pModel->ObjName );
      return;
      }
    }
  else
    {
    if ( pSys->IsBlk || LocalBlk )
      {
      Stop = true;
      Trip = false;
      return;
      }
    if ( State )
      {
      Trip = true;
      Err = true;
      //Set_ErrBlkInObj ( pModel->ObjName );
      return;
      }
    }
  Trip = false;
  }
