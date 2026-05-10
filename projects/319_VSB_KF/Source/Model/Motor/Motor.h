#pragma once

#include "BaseModel.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "Data.h"
#include "IntReg.h"

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

struct I_Ток
  {
	virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual void Init() = 0;
  virtual bool Calc( double dt, double Oborot, double Pow, double & Ток, double & Мощность, bool & Stop ) = 0;
  };

struct I_BaseТок : public I_Ток
  {
  double _Ток, _Мощность;
  double B_Ток;// "B тока", 100. );
  double A_Ток;// "A тока", 0. );
  double U_work;// "Рабочее напряжение, кВ", 9.5 );
  double k_Ток;// "Коэффициент тока", 0. );
  //
  double k_Pow;// "Коэффициент мощности", 0. );
  double PuskТок;// "Пусковой ток", 0. );
  double Avar_Ток;// "Аварийный ток", 2400. ); 
  int kStep;
  int kAvar;
  //
	void GetParams( IBaseModel * pMain );
  void Init();
  bool Calc( double dt, double Oborot, double Pow, double & Ток, double & Мощность, bool & Stop );
  };

struct I_StartТок : public I_Ток
  {
  double _Ток, _Мощность;
  double _Pow;// "#Pow : Текущая расчетная мощность", 0. );
  double k_Pow;// "К: Мощность = K * Pow", 1. );
  double Ток_1;// "К1: Ток1 = K1 * Pow1", 0. );
  double Pow_1;// "Pow1 - расчетная мощность 1", 0. );
  double Ток_2;// "К2: Ток2 = K2 * Pow2", 0. );
  double Pow_2;// "Pow2 - расчетная мощность 2", 0. );
  double PuskТок;// "Пусковой ток", 0. );
  int kStep;
  int kAvar;
	void GetParams( IBaseModel * pMain );
  void Init();
  bool Calc( double dt, double Oborot, double Pow, double & Ток, double & Мощность, bool & Stop );
  };


struct _W_Motor_b
{
	double Oborot;
	bool On;
	_W_Motor_b();
};

#define MAX_DRIVE 8
class MOTOR_API CMotor : public IBaseModel, public IUniModel, public IElectroModel, public _W_Motor_b
	{
//
public:
	CMotor( char * ObjName, IBaseModel* pMain = NULL );
	virtual ~CMotor();
// Интерфейс IBaseModel
  int Init( int );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams(  char * StrName );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( CDef * pDefect );
  int SrcMM( char * DataName, struct IMM * pMM );// Обращение к источнику сигнала
  int ResetVariable();
  ///////////////////////////////////////////////////
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    // Переменные на входах
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  virtual void ElectroOn();
  virtual void ElectroOff();
  void DrawObj ( struct CDrawObjData * pDraw );
  NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "Motor_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
  enum TypeDef { Исправен, ОтказДвигателя, ПадениеОборотов, ОтказАппаратуры, АварияДвигателя };
  TypeDef Defect;
  double Defect_Pow;
  int kAvar;
  int kStep;
//  I_Ток * pCalcTок_1;
//  I_Ток * pCalcTок_2;
  CMotorData MotorData;
  double* DrivePow[MAX_DRIVE];
  double Po;
/////////////////////////////////////////////////
//
  void Calc ( double dt );
  void CalcТок ( double dt, double Pow );
  void CalcПускТок ( double dt, double Pow );
  bool IsElectro;
};
