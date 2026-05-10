#pragma once
#include "Valve_b.h"

#define DEFECT_FULL_OPEN			(1 << 1)
#define DEFECT_FULL_CLOSE			(1 << 2)
#define DEFECT_LIMIT_STOCK			(1 << 3)
#define DEFECT_FIXED_POSITION		(1 << 4)


struct _W_Valve_A_b
{
	double Task;
	double Old_Position;
	double Dubler;
	bool bZacep;
	_W_Valve_A_b();
};

class CValve_A_b :	public CValve_b, public IElectroModel, public _W_Valve_A_b
{
public:
	CValve_A_b( char * _ObjName, char* _Type );
	virtual ~CValve_A_b(void);
	virtual int SaveState( );
	virtual int RestoreState( char * StrName );
	int OnDefect( CDef * pDefect );
	virtual void ElectroOn();
	virtual void ElectroOff();
	virtual void Drive(double dt);
	virtual void VerifyBlk(double _Task);
	bool ControlNoKip(double dt);
	bool ControlNoElectro(double dt);
	virtual void Calc(double dt);
	bool IsHydroTask();
	virtual bool Defect_Calc( double dt );
	virtual void Control(double dt) = 0;	
	virtual void DrawObj ( struct CDrawObjData * pDraw );
	virtual int ShowParams( DWORD DataType, struct CShowData * pSD );
	virtual int SetData( int TypeData, void * pData );
	virtual int GetParams( char * );
	virtual bool ControlDubler(double dt);
  virtual void Sbros(){};
	bool bIgnoreBlk;
	enum Actions
	{
		Открывается,
		Закрывается,
		Не_меняется,
		Не_влияет,
	}KIP, Electro;
	double Dubler_old;
	bool IsElectro;
		/////////////////////////////////////////////////
		//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Valve_A_b_Pnt.h"
#include "Valve_A_b_Defects_Pnt.h"
#include "ACS_Create.h"
#include "Valve_A_b_ACS.h"
};
