#pragma once
#include "Valve_A_b.h"

enum Switch
{
	SWITCH_DIST,
	SWITCH_MEST,
	SWITCH_DUAL,
};

struct _W_EV
{
	bool bOpen;
	bool bClose;
	bool bStop;
	Switch eSwitch;
	_W_EV();
};

class CEV : public CValve_A_b, public _W_EV
{
	//
public:
	CEV( char * ObjName, char* Type );
	virtual ~CEV();
	// Интерфейс IBaseModel
	int GetParams( char * );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
	int SaveState ( );
	int RestoreState ( char * StrName );
	int OnDefect( CDef * pDefect );
	int Step1( );
	///////////////////////////////////////////////////
	int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
	NEW
	/////////////////////////////////////////////////
	//#include "IO_Conn.h"
	#include "IO_Create.h"	
	#include "EV_Pnt.h"
	#define NO_CREATE_DEFECTS
	#include "EV_Defects.h"
	#undef NO_CREATE_DEFECTS
	#include "ACS_Create.h"
	#include "EV_ACS.h"
	/////////////////////////////////////////////////
	//
	void Calc ( double dt );
	void Control(double dt);
	double conditionCross;
	bool Fixed_Task;
};

