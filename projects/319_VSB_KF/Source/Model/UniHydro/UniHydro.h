#pragma once

#include "BaseInt.h"
#include "HydroStruct.h"
#include "Interface/HydroGroupsInterface.h"
#include "Connect.h"
#include "Full_Lib.h"

#define OLD_CALC_MODE	3
//////////////////////////////////////////////////////////////////////////
// 0 bit - new(0) or old(1) calculation method
// 1 bit - parallel(0) or sequential(1) all StepT calculation
//
// 0 - New and Parallel
// 1 - Old and Parallel
// 2 - New and sequential
// 3 - Old and sequential
//////////////////////////////////////////////////////////////////////////

struct CGroupRef //: public IHydroCalc
{
	// Реализация виртуальных функций
	virtual int HydroCalc(double dt, bool SetVars);
	virtual void GetObjMatr(double dt);
	virtual int SetAllVars(double dt);
	//protected:
	void Init();
	void Add(CVarPool * pPool);
	CGroupObject & Add(struct CObjectRef * pObj, CObjectPoint * pPnt );
	void PrePare();
	void AddObject(int kPoints, int NumbVar[], int kObjEq, bool LastStep);
	bool HaveCommVar(int kPoints, int NumbVar[]);
	void UseVars(int kPoints, int NumbVar[], int & kDelete, int & Add);
	void PrtA(bool Add);
	void PrtM(char * Name, int K, int N, int * iV, int * nV, double * M, double * V);
	void Dump(int nErr);
	bool Fwd(double dt);
	void SetX(double dt, int x, double Value);
	bool Back(double dt);
	bool Exclude(int nVar_excl);
	void PrtFailed(int nVar);
	void ShowFailed(int nVar);
	void ObjVars(CGroupObject * pObj);
	int  FormMatr(double dt, int nObj, double * M, double * V);
	bool SetVar(double dt);
	int  SetVar(double dt, int nObj);
	//protected:
	int MaxEQ;
	bool UseCS;
	unsigned long kScalarVariables;  // Число скалярных переменных
	unsigned long kObjects;
	unsigned long long Counter;

	void There_and_Back(double dt);
	void There_and_Back_np(double dt);
	double *b_test;
	double *x_test;
	int temp_eq;
	int amount_eq;
	LUsolver *ss_test;
	bool cont;
	int *Arow; 
	int *Acol;
	double *Aval;
	int nnzMaybe;
	int nnzv;
	bool flag;
	bool renum;
	int *t_eq;

	int Numb;
	int kVar;
	CMem<CGroupObject,128,128>Objects;
	//
	double ** ppAddrVars;        // Адреса скалярных переменных
	int * CommonVars;   // Переменная pCommonVars[N] заменяет N
	int kStructVariables; // Число неповторяющихся CBaseVariablesData
	int * OptOrd;
	int Step;
	int StepSave;
	struct CStepActionVar * StepVar_Beg; 
	struct CStepActionVar * StepVar_End; 
	int * nStepStart;  // Номер шага включения переменной
	int * nStepEnd;    // Номер шага исключения переменной
	// Вспом 
	double * A;// Ур-я имеют вид A * X = B  
	double * B;
	double * X; 
	int * Pos_in_CurrVars/*[kVar]*/; // 
	int * ActiveSet;
	int * ObjUseVars;
	int * CurrVars; 
	int * State_Var;
	int * Numb_Var; 
	int kCurrVars;
	int kEQ;
	int MaxVAR;
	int MemSave;
	bool ShowErrors;
	int * BegSt; 
	int * pNumb; 
	int PosInSt; 
	double * pVar;
    // CRITICAL_SECTION CS;
    // DWORD nThread;
	//
	CVarPool * pFirstPool;
	CVarPool * pLastPool;
};

struct _W_UniHydro
{
	double O;
	_W_UniHydro();
};

class CUniHydro : public IBaseModel, public _W_UniHydro, public HydroGroupsInterface
{
//
public:
	CUniHydro(const char * ObjectName);
	virtual ~CUniHydro();
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
	NEW
/////////////////////////////////////////////////
#include "IO_Create.h"
#include "Points.h"
/////////////////////////////////////////////////
	CMem<CObjectRef,256,256>ObjRefs;
	int * SortObjByName;
	CMem<CObjectPoint,512,256>ObjPoints;
	int * SortObjPoints;
	CMem<CObjProps,256,256>ObjProps;
	CMem<CVarPool,256,256>VarPool;
	CMem<CPipeRef,256,256>Pipes;
	int * SortPoolByName;
//  CMem<CConnectionXY,256,256>ConnXY;
	CMem<CGroupRef,64,64>Groups;

	LUsolver *LU, *LU_par;
	int nnz;
	int *Ti; 
	int *Tj;
	double *Tx;
	int nnzM;
	int nrow;
	double *b_rhs;

	struct IProgress * pProgress;
	CDataPtr * pDataPtr;
	int kObjects;
	int Load_Data( char * File );
	int Load_File( char * Path, char * Shema );
	CVarPool * FindCons ( char * Name, int Type );
	int FormStruct();
	void Prepare();
	void CompressObjList();
	void CompressConnectionList();
	int ObrUniModelPoints( CObjectRef & Obj );
	void ExpandGroup();
public:
	virtual DWORD GetCount();
	virtual void SetCounter(DWORD index, unsigned long long Counter);
	virtual unsigned long long  GetCounter(DWORD index);
	virtual DWORD GetScalarVariables(DWORD index);
	virtual DWORD GetObjects(DWORD index);
	virtual DWORD GetMaxEQ(DWORD index);
	virtual DWORD GetMaxVAR(DWORD index);
	virtual void SetA(DWORD index, double * A);
	virtual void SetB(DWORD index, double * B);
	virtual void ClearThread(DWORD index);
	virtual void SetThread(DWORD index, DWORD N);
	virtual int HydroCalc(DWORD index, double dt, bool VarInHydro);
	virtual int SetAllVars(DWORD index, double dt);
	virtual void GetObjectMatrix(DWORD index, double dt);
	virtual void CreateCriticalSection(DWORD index);
	virtual int GetCalculationMethod(){return OLD_CALC_MODE;};
};

//IBaseModel * Load_Obj(char * ObjName, char * Model);
