#pragma once

#include "Valve_A_b.h"


#define STRAPPING_IN		0x00000001
#define STRAPPING_OUT		0x00000002
#define STRAPPING_BAYPASS	0x00000004

struct _W_CV
{
	double Zadv_In;
	double Zadv_Out;
	double Zadv_Bypass;
	_W_CV();
};

class CCV : public CValve_A_b, public _W_CV
{
	//
public:
	CCV( char * ObjName, char * Type );
	virtual ~CCV();
	// Интерфейс IBaseModel
	int GetParams( char * );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
	int SaveState ( );
	int RestoreState ( char * StrName );
	int SetData( int TypeData, void * pData );
	int OnDefect( CDef * pDefect );
	void DrawObj ( struct CDrawObjData * pDraw );
	///////////////////////////////////////////////////
	int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
	NEW
	/////////////////////////////////////////////////
	//#include "IO_Conn.h"
	#include "IO_Create.h"
	#include "CV_Pnt.h"
	#define NO_CREATE_DEFECTS
	#include "CV_Defects.h"
	#undef NO_CREATE_DEFECTS
	#include "ACS_Create.h"
	#include "CV_ACS.h"
	/////////////////////////////////////////////////
	//
	void Calc ( double dt );
	double Calc_Omega(Characteristic eCharacteristic, double _Position);
	void Control(double dt);
	int Strapping;
};

