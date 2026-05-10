#pragma once

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef DLL_CTRLREG_EXPORTS
#define DLL_CTRLREG_API Q_DECL_EXPORT
#else
#define DLL_CTRLREG_API Q_DECL_IMPORT
#endif
#else
#ifdef DLL_CTRLREG_EXPORTS
#define DLL_CTRLREG_API __declspec(dllexport)
#else
#define DLL_CTRLREG_API __declspec(dllimport)
#endif
#endif

#include "BaseInt.h"
#include "Interface/RegInterface.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "SelVar.h"
//#include "Data.h"
//#include "Chem.h"
//#include "Defects.h"
//#include "ACS_Def.h"

class CExtRegOutEx : public CExtRegOut
{
	CExtRegOut* GetNextOut();	
};

struct CExtRegEx_W 
{
	double dV, dVm;
};

class DLL_CTRLREG_API CExtRegEx : public CExtReg, public CExtRegEx_W
{
public:
	CExtRegEx();
	CExtRegOut* pFirstOut;
	CExtRegEx* pNextReg;
	CExtRegOut* GetFirstOut();
	CExtReg* GetNextReg();
	CExtRegOut* AddOut(const char* szRef, double MV_0, double MV_1);
	void DeleteOut(CExtRegOut* pDelete);
	int StepT(double dt);
};

struct _W_CtrlReg
{
	_W_CtrlReg();
};


class DLL_CTRLREG_API CCtrlReg : public IRegInterface, public IBaseModel, public _W_CtrlReg
{
	//
public:
	enum RegCsv
	{
		COLUMN_TYPE,
		COLUMN_NAME,
		COLUMN_K,
		COLUMN_T,
		COLUMN_TD,
		COLUMN_GOAL,
		COLUMN_TASK,
		COLUMN_MIN,
		COLUMN_MAX,
		COLUMN_COUNT
	};
	CCtrlReg( char * ObjName );
	virtual ~CCtrlReg();
	// Интерфейс IBaseModel
	int Init( int Regim );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams( char * );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
	int UpdateParam( struct CParams & Param );
	int SaveState ( );
	int RestoreState ( char * StrName );
	int SetData( int TypeData, void * pData );
	int OnDefect( CDef * pDefect );
	///////////////////////////////////////////////////
	NEW
	/////////////////////////////////////////////////
	//#include "IO_Conn.h"
	//#include "IO_Create.h"
	//#include "Points.h"
	//#include "ACS_Create.h"
	//#include "ACS.h"
	/////////////////////////////////////////////////
	//
	CExtRegEx* pFirstReg;
	CExtRegEx* pLastReg;
	CExtReg* GetFirstReg();
	CExtReg* AddReg( const char* szRegName, double K, double T, double TD, const char* szGoal, double SP, double Min, double Max, bool On );
	void ChangeReg( CExtReg* pReg );
	void DeleteReg(CExtReg* pDelete);
	void Save();
	void Calc (double dt);	
	int kStep;
};
