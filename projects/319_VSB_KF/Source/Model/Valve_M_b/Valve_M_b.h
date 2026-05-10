#pragma once
#include "Valve_b.h"

struct _W_Valve_M_b
{
	double Valve;
	_W_Valve_M_b();
};


class CValve_M_b :	public CValve_b, public _W_Valve_M_b
{
public:
	CValve_M_b( char * _ObjName, char* _Type );
	virtual ~CValve_M_b(void);
	int OnDefect( CDef * pDefect );
	virtual int Step1( );
	virtual void Calc(double dt);
	virtual void Control(double dt);	
	virtual int SetData( int TypeData, void * pData );
	virtual int GetParams( char * );
	virtual int SaveState( );
	virtual int RestoreState( char * StrName );
	virtual int ShowParams( DWORD DataType, struct CShowData * pSD );
	double Valve_old;
	/////////////////////////////////////////////////
		//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Valve_M_b_Pnt.h"
#include "Valve_M_b_Defects_Pnt.h"
#include "ACS_Create.h"
#include "Valve_M_b_ACS.h"
};
