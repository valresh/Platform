#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include "Data.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "ComprStage.h"
#include "Oborot.h"
#include "IntReg.h"
#include "vector"

#define MAX_EXTERNAL 8

struct _W
{
	double Oborot;
	bool On;
	double ComprFlow;
	double P_in, P_out;
	int pos_Характеристики;
	COborot ExtOborot[MAX_EXTERNAL];
	double T_in, T_out;
	double To_m3_in;
	_W();
};

typedef char char16[16];

class CCC_1 : public IBaseModel, public IUniModel, public _W
{
	//
public:
	CCC_1( char * ObjName, char * Param );
	virtual ~CCC_1();
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
	int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
	int SetMatr( void * pExternals, int PntGroup,  double dt, 
		double A[/* kExkV */], double B[ /*kE*/ ] );
	int SetVar( void * pExternals, int PntGroup, double dt, 
		double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	///////////////////////////////////////////////////
	int DstMM( char * DataName, struct IMM * pMM );// Вызов получателя сигнала
	int SrcMM( char * DataName, struct IMM * pMM );
	void DrawObj ( struct CDrawObjData * pDraw );
	NEW
		/////////////////////////////////////////////////
		//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "CC_1_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
		CStage_СС ST_1;
	//  CPumpData * pPump;
	enum TypeDef { Исправен, ОтказДвигателя, ОтказАппаратуры, ОтказСвязи };
	TypeDef Defect;
	//  CDef * Восстановление_работоспособности;
	//  CDef * Отказ_двигателя;
	//  CDef * Отказ_пусковой_аппаратуры;
	//	CDef * Отказ_связи;
	//	CDefParam * def_Conn;
	//
	IBaseModel * pAnalogPipe;
	bool CanPuskStop;
	double FlowQ; 
	bool DefectStop;
	bool RazGerm;
	double OmegaRazGerm;
	double aF, cF;
	double * F_Характеристики;
	double * P_Характеристики;
	char16 * S_Характеристики;
	int kExternal;
	int k_Pnt_Q;
	/////////////////////////////////////////////////
	//
	void Напор ( double Flow_M3, double & A, double & C );
	void Calc ( double dt );
	void Logic ( double dt );
	typedef std::vector<CDef*> TDefVector;
	TDefVector defVector;
	CMotorData MotorData;
	CMotorData* pMotor;
};
