#pragma once

#include "BaseInt.h"
#include "EpsVol.h"
#include "Chem.h"
#include "Data.h"

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#undef IN_DLL
#ifdef DLL_NewComp
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif

struct IN_DLL CNewComp_P
  {
  bool Change;
  bool ChangeFlow;
  bool AddFlow;
  bool AddMass;
  bool AddPress;
  bool In_Mass;
//
	double P, T;
	double Flow, h_Flow;
	char Name[64];
  CNewComp_P();
  };

// 1. Должен рождаться только по new в ф-ии Init моделей
// 2. В pFlow должна быть рассчитана T
// 3. pFlow дальше не использовать
class IN_DLL CNewComp	: public IBaseModel, public CNewComp_P
	{
	public:
	CSEps Out;
	int ID_Comp;
	CComp * pComp;
	bool NewComp;
	bool ChangeC;
  NEW
	CNewComp( char * Name, IBaseModel * pMain );
	int GetParams( char * );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
	void SetComp();
  int SetData( int TypeData, void * pData );
	void ChangeComp ( CFlow * pFlow );
	void CalcComp( );
	};

