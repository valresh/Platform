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
	ComponentControl(const char* szCompName);
	char szName[MAX_COMP_NAME];
	//
	virtual void Release();
	virtual void Norm();
	virtual void Calculate();
	// Concentration Mol
	virtual double GetConcentrationMol(DWORD n);
	virtual void SetConcentrationMol(DWORD n, double C);
	// Concentration Mass
	virtual double GetConcentrationMass(DWORD n);
	virtual void SetConcentrationMass(DWORD n, double W);
	// Tb
	virtual double GetTb(DWORD n);
	virtual const char * GetComponentName(DWORD n);
	virtual ComponentData GetComponentData(DWORD n, TypeComponent Type);
	//-----------------------------
	void MolToMass();
	void MassToMol();
	CComp Mol;
	CComp Mass;
	TypeComponent Type;
	char ** ComponentName;
};
