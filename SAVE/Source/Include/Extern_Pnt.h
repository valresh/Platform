#pragma once
#include "CommProc.h"

#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

struct CConnectInfo
{
	char * Type;
	char * ConnName;
	void * pSrcAddr;
	char * Param;
	struct IBaseModel * pRefModel;               
};

struct CSavePnt
{
    char Name[64];
	union
	{
		double Dbl;
		int Int;
	};
};


struct IN_DLL CExtern_Pnt
{
	CExtern_Pnt * pNext_Extern_Pnt;
    CExtern_Pnt * pNext_Model_Pnt;
    //===============================
	struct IBaseModel * pModel;
	CStr Point;
	char Type[4];
	CStr Descr;
	//CStr RSU;
	CStr Connection;
	bool IsConnection;
	char SD;
	DWORD Flag;
	int nDefect;
	PADDING(4);
	static CSavePnt * pSave;
	static int kPnt;
	//===============================
	NEW
	CExtern_Pnt(struct  IBaseModel * pModel, char * Point, char * Type, char * Descr );
	CExtern_Pnt();
	void Init(struct IBaseModel * pModel, char * Point, char * Type, char * Descr );
	virtual bool Connect( CConnectInfo * pInfo ) = 0;
	virtual void GetParams( ) = 0;
	bool CompTypes(char * EXT_Type );
	virtual bool Active(){ return false; };
	virtual void GetInfo( char Obj[128], char Point[64], char ACS[64] ){};
	// On - Move = 1, Off - Move = -1,
	virtual void KlapOpenClose( double Vent, bool LocalBlk, int & Move, bool & Err ){};
	// On - Move = 1
	virtual void KlapOpen( double Vent, bool LocalBlk, int & Move, bool & Err ){};
	// On - Move = -1
	virtual void KlapClose( double Vent, bool LocalBlk, int & Move, bool & Err ){};
	virtual void CmdStop( int State, bool LocalBlk, bool & Stop, bool & Err ){};
	virtual void CmdStart( int State, bool LocalBlk, bool & Start, bool & Err ){};
	virtual void SetMes( int Mes, double Scale ){ ; };
	static void SaveAll( );
	static void RestoreAll( );
 	virtual void * GetAddress() = 0;
};

void IN_DLL ACS_Params( char * TabName, struct IBaseModel * pModel );
void IN_DLL ACS_Params( struct IBaseModel * pModel );
void IN_DLL ACS_Analog( struct IBaseModel * pModel );
void IN_DLL ACS_Diskrete( struct IBaseModel * pModel );
void IN_DLL ACS_Connect( struct IBaseModel * pModel );
