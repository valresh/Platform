#pragma once

#include "Interface/CompInterface.h"
#include "Data.h"
class ComponentControl : public ComponentInterface
{
	enum
	{
		MAX_COMP_NAME = 128,
	};
public:
	ComponentControl(const char* szCompName, TypeComponent TypeDefault, TypeSource Load);
	//
	virtual bool Read(const char * Name, TypeSource source);
	virtual void Write(const char * Name);
	virtual void Norm();
	virtual void Calculate();
	virtual void Release();
	virtual DWORD GetNumberLastComponent();
	// Concentration
	virtual double GetConcentration(DWORD n);
	virtual void SetConcentration(DWORD n, double C);
	virtual void SetTypeConcentration(TypeComponent Type);
	// Tb
	virtual double GetTb(DWORD n);
	virtual void GetVT(DWORD size, double * V, double * T, bool ** IS, double ** Values, int ** SortedComponent);
	virtual double GetPortionV(double T);
	virtual const char * GetComponentName(DWORD n);
	virtual ComponentData GetComponentData(DWORD n, TypeComponent Type);
	virtual DWORD GetComponentNumber(const char *);
private:
	CComp Mol;
	CComp Mass;
	char ** ComponentName;
	TypeSource Source;
	// Указатели
	bool * PointIs;
	double * PointMol;
	double Out[K_GAS];
	bool OutIs[K_GAS];
	int SortedComponent[K_GAS];
	// Функции
	double * UniversalPoint();
	double Norm(double * Values, DWORD size);
	void MolToMass(DWORD Count, double * SourceMol, double * DestignationMass);
	void MassToMol(DWORD Count, double * SourceMass, double * DestignationMol);
};
