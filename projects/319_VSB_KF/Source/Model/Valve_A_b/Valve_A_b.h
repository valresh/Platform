#pragma once
#include "Valve_b.h"

#define DEFECT_FULL_OPEN			(1 << 1)
#define DEFECT_FULL_CLOSE			(1 << 2)
#define DEFECT_LIMIT_STOCK			(1 << 3)
#define DEFECT_FIXED_POSITION		(1 << 4)

struct KlapReg
{
  bool Reg_On;//, "Включен" )
  double Reg_PV_;//, "#PV=" )
  double Reg_MV_;//, "#MV=" )
  double Reg_dMV_;//, "#MV=" )
  int Reg_PV;//, "PV", "Массовый расход;Объемный расход н.у.;Объемный расход;Давление на входе;Давление на выходе;Ссылка;")
  char Reg_Ref[128];//, "Ссылка", "" )
  double Reg_SP;//, "Задание" )
  double Reg_P;//, "P", 1000. )
  double Reg_I;//, "I", 50. )
  bool Reg_Dir;//, "Прямой", true )
  double Reg_SH;//, "Масштаб PV", 10. )
  double Reg_MH;//, "Масштаб MV", 100. )
  double Reg_Tau;//, "Инерционность PV, c", 0. )
  bool Reg_Vent;//, "Крутить вентиль", true )
  double * pRef;
  class CValve_A_b * pKlap;
  /////////////////////////////
//
  double PV;
  double En, En1;
  KlapReg( class CValve_A_b * pMain );
//
  void Reg(double dt);
  double * GetRef ( char * Ref );
  void GetParams( IBaseModel * pMain );
  void UpdateParam( CParams & Param );
};

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
  virtual int UpdateParam( CParams & Param );
  virtual bool ControlDubler(double dt);
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
  KlapReg Reg;
		/////////////////////////////////////////////////
		//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Valve_A_b_Pnt.h"
#include "Valve_A_b_Defects_Pnt.h"
#include "ACS_Create.h"
#include "Valve_A_b_ACS.h"
};
