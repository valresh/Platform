#pragma once

#include "Interface/HydroBasicTypes.h"

// создаётся при чтении, затем переключается на CObjectPoint 
struct CPoolPoint  // один из концов трубы
{
	CStr ObjName;
	CStr PntName;
	CPoolPoint * pNext;
};

struct CConnectionXY  // отрезки трубы
{
	CStr Shema;
	int kXY;
	double * pXY;
	CConnectionXY * pNext;
};


// ќбъект может быть в нескольких группах - ссылки на переменные нужны разные
struct CGroupObject
{
	CStr ObjName;
	CStr Model;
	CStr Shema;
	IBaseModel * pObject;
	//  CObjectRef * pObject;
	CObjectPoint * pFirstGroupPoint;
	CObjectPoint * pLastGroupPoint;
	CObjectPoint * pFirstPoolPoint;
	CObjectPoint * pLastPoolPoint;
	bool MainPool; //модуль управлени¤ пулом
	////////
	// CObjectVariable может содержать 0 ... скал¤рных переменных
	CObjectPoint ** pPoints;      // ”пор¤доченные ссылки на CObjectPoint по пор¤дковым номерам в пределах внутренней группы
	int * kPointsVar;             // „исло скал¤рных переменных у каждой точки
	int kPoints;                  // „исто точек
	int kLinearVar;               // „исто скал¤рных переменных у объекта
	int * NumbVarInGroup/*[kV]*/; // Ќомера скал¤рных переменных в общем списке группы
	double ** AddrVar/*[kV]*/;    // јдреса скал¤рных переменных
	int kLinearEq;                // ќбщее число уравнений дл¤ скал¤рных переменных
	int Order;                    // ѕор¤дковый номер при вызове
	//
	struct CGroupRef * pGroup;
	int PntGroup; 
	double * A/*[kE][kV]*/; // ”р-¤ имеют вид A * V = B 
	int l_A; //@
	int * Ind_A;
	double * B/*[kE];*/; 
	bool IsMatrInd;
	//
	void Prepare();
	void Init();
	void Add( CObjectPoint * pP );
	void AddPntToPool( CObjectPoint * pP );
};

struct CStepActionVar
{
	int Step;
	int Var;
};

struct CObjectPointKey
{
	char * ObjName;
	char * PntName;
};

struct CVarPool // труба с несколькими концами
{
	CStr Name;
	CStr Model;
	ePipeType Type;
	int Numb;
	int nGroup;
	bool Selected;
	//
	int FirstGroupNumbVar; // номер 1 переменной пула в группе
	int kPoints;
	struct CBaseStruct ** pStruct;
	int kScalarVar;
	//
	CObjectPoint * pFirstPoint;
	CObjectPoint * pLastPoint;
	struct CObjProps * pFirstProp;
	struct CObjProps * pLastProp;
	struct CConnectionXY * pFirstXY;
	struct CConnectionXY * pLastXY;
	struct IBaseModel * pThisObject;
	struct CVarPool * pNextPool;
	struct CPoolPoint * pFirstPoolPnt;
	struct CPoolPoint * pLastPoolPnt;
	struct CPipeRef * pPipe;
	///
	void Init( );
	void Add( CPoolPoint * pPnt );
	void Add( CObjectPoint * pPnt );
	void Add( CConnectionXY * pXY );
	void Add( struct CObjProps * pP );
	void SetThisObject( );
	bool HaveMatr( );
};
