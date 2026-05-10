#pragma once

#include "BaseInt.h"
#include "Data.h"
#include "EpsVol.h"

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#undef IN_DLL
#ifdef DLL_PressDrop
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif

struct _W_PressDrop
  {
  double Omega;
  double Xi_Flow;
  double k_Omega;
  _W_PressDrop();
  };

class IN_DLL CPressDrop : public IBaseModel, public _W_PressDrop
	{
//
public:
	CPressDrop( char * ObjName, IBaseModel * pMain, CSEps * pEps );
	virtual ~CPressDrop();
// Интерфейс IBaseModel
	int Init( int Regim );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams( char * );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
	NEW
/////////////////////////////////////////////////
#include "IO_Create.h"
#include "PressDrop_Pnt.h"
/////////////////////////////////////////////////
//
  CSEps * pEps;
//
  double D;
  double S;
//
  double Re;
//
  double Q;
//
  void SetD( double D/*мм*/, double S = 0./*м2*/ );
  void CalcCoef( CFlow * pFlow );
  void SetGasEtalon( CComp * pComp, double Flow, double P, double dP, double T );
  void SetLiqEtalon( CComp * pComp, double Flow, double dP, double T );
//
  void CalcLiqCoef   ( CFlow * pFlow, double & Xi_Rel );
  void CalcGasCoef   ( CFlow * pFlow, double & Xi_Rel );
  void CalcMixCoef   ( CFlow * pFlow, double & Xi_Rel );
//
	};
