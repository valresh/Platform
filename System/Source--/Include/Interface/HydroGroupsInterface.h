#pragma once
#include "crossplatform.h"

struct HydroGroupsInterface
{
	virtual DWORD GetCount() = 0;
	virtual void SetCounter(DWORD index, unsigned long long Counter) = 0;
	virtual unsigned long long  GetCounter(DWORD index) = 0;
	virtual DWORD GetScalarVariables(DWORD index) = 0;
	virtual DWORD GetObjects(DWORD index) = 0;
	virtual DWORD GetMaxEQ(DWORD index) = 0;
	virtual DWORD GetMaxVAR(DWORD index) = 0;
	virtual void SetA(DWORD index, double * A) = 0;
	virtual void SetB(DWORD index, double * B) = 0;
	virtual void ClearThread(DWORD index) = 0;
	virtual void SetThread(DWORD index, DWORD N) = 0;
	virtual int HydroCalc(DWORD index, double dt, bool VarInHydro) = 0;
	virtual int SetAllVars(DWORD index, double dt) = 0;
	virtual void GetObjectMatrix(DWORD index, double dt) = 0;
	virtual void CreateCriticalSection(DWORD index) = 0;
	virtual int GetCalculationMethod(){return 3;};
	/*
	virtual int HydroCalc(double dt, bool SaveVars) = 0;
	virtual void GetObjMatr(double dt) = 0;
	virtual int SetAllVars(double dt) = 0;
	virtual void InitializeCritical() = 0;
	virtual void LeaveCritical() = 0;
	int MaxEQ;
	bool UseCS;
	unsigned long kScalarVariables;  // Число скалярных переменных
	unsigned long kObjects;
	unsigned long long Counter;*/
};

typedef HydroGroupsInterface * (*AddressHydroGroupsInterface)();