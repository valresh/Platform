#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"

static double Cp_corp = 100.;
static double Lam_stal = 30.;
static double d_stal = 5e-3;// Толщина стенок труб
static double k_norm = 600e3;// Нормальная теплоотдача Сталь->нефть Вт/м2 К


CRadTube::CRadTube(  char * _ObjName, IBaseModel * pPech ) : IBaseModel( _ObjName, pPech )
, In ( "Вход", this )
, Out ( "Выход", this )
, OutZ ( "ВыходZ", this )
  {
  ObjName = _ObjName;
  Model = "RadTube";
  T_in = 50.;
  T_out = 300.;
  Cp_Tube = 10000.;
  //
  pPech = NULL;
  pFlow_in = NULL;
  pFlow_out = NULL;
  TZ = 0.;
  Calc_H = false;
  F_Прогар = 0.;
  O_Прогар = 0.;
  pPyrolysis = NULL;
  pBasePyrolysis = NULL;
  InitPyrolysis = true;
  }

CRadTube_W::CRadTube_W()
  {
  memset ( this, 0, sizeof ( CRadTube_W ));
  kFlow = 1.;
  }

void CRadTube::InitTube()
  {
  InitPyrolysis = false;
  if ( pPyrolysis )
    {
    pPyrolysis->T_in = &T_in;
    pPyrolysis->T_out = &T_out;
    pPyrolysis->Q_Fire = &Q_Rad;
    pPyrolysis->kFlow = &kFlow;
    pPyrolysis->pFlow_in = pFlow_in;
    pPyrolysis->pFlow_out = pFlow_out;
    }
  }

void CRadTube::CalcT ( double dt )
  {
  double T_max_out = pPech->T_max_out;
	In.T_max = T_max_out;
	Out.T_max = T_max_out;
	In.PH ( pFlow_in );
	T_in = In.T;
	double h_In = In.h;
	double Q_in = Q_Rad;
  if ( pPech->Calc_T_Wall )
    Q_in = Q_wall;
  else
    Q_wall = Q_in;
	*pFlow_out = *pFlow_in;
  if ( pPyrolysis )
    {
    if ( InitPyrolysis )
      InitTube();
    pPyrolysis->pFlow_in = pFlow_in;
    pPyrolysis->pFlow_out = pFlow_out;
    pPyrolysis->Reactions( dt );
    return;
    }
  kFlow = 1.;
  if ( Calc_H )
    {
	  double F = fabs ( pFlow_in -> Flow_mol );
    double h; 
    if ( F > 0.1 )
  	  h = h_In + Q_in / F;
    else
  	  h = h_In;
    Out.T_max = T_out + 2.;
    Out.T_min = T_out - 2.;
    double Cp = 100. * Cp_Tube;
    Out.PH_mass( pFlow_out->P, h, pFlow_in->Get_Comp(), Cp, Cp * T_out, T_out );
    double D = h - Out.h;
    h = Out.h;
    pFlow_out->h_Flow = h;
	  Out.PH( pFlow_out );
    h_Out_Gas = h;
    T_out = Out.T;
    }
  else
    {
	  Out.PT( pFlow_out->P, T_out, pFlow_in->Get_Comp());
    if ( TZ > 0. )
      {
	    OutZ.PT( pFlow_out->P, TZ, pFlow_in->Get_Comp());
      hOutZ = OutZ.h;
      }
    else
      hOutZ = 0.;
//	T_out = Out.T;
  	h_Out_Gas = pFlow_out->h_Flow = Out.h;
	  double F = fabs ( pFlow_in -> Flow_mol );
  	double dh =  Q_in + ( h_In - h_Out_Gas) * F;
	  double dT = LimdX ( dh * dt / Cp_Tube, 1. );
	  T_out += dT;
    }
  if ( T_out > T_max_out )
    T_out = T_max_out;
	if ( T_out < T_air )
	 T_out = T_air;
  }



void CRadTube::Step( double dt )
  {
  CalcT ( dt );
  }

#include "IO_Parms.h"

int CRadTube::SaveState( )
	{
	S_CLASS("RadTube_W",CRadTube_W)
	return 0;
  }


int CRadTube::RestoreState( char * StrName )
	{
  COPY_FIRST
	R_CLASS("RadTube_W",CRadTube_W)
	return 1;
  }

int CRadTube::GetParams( char *  )
  {
  TAB("Режим",1)
  PARM(T_in, "#T_in" )
  PARM(T_out, "#T_out" )
  PARM(Cp_Tube, "Cp_Tube" )
  ETAB
	return 0;
  }
