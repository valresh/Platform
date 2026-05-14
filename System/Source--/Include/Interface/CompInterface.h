#pragma once

#define COMPONENT_LIB_NAME				"EpsVol"
#define COMPONENT_GET_NAME				"GetComponentInterface"
#include "crossplatform.h"

struct ComponentInterface
{
	struct ComponentData
	{
		enum
		{
			MAX_COMP_NAME = 64,
			MAX_COMP = 128
		};
		const char * ComponentName;
		double C;
		double Tb;
	};
	enum TypeComponent
	{
		MOL,
		MASS,
		UNKNOWN,
	};
	enum TypeSource
	{
		FILE,
		MODEL,
	};
	ComponentInterface(): CountComponent(0), Type(MOL) {};
	virtual bool Read(const char * Name, TypeSource source) = 0;
	virtual void Write(const char * Name) = 0;
	virtual void Norm() = 0;
	virtual void Calculate() = 0;
	virtual void Release() = 0;
	virtual DWORD GetNumberLastComponent() = 0;
	// Concentration
	virtual double GetConcentration(DWORD n) = 0;
	virtual void SetConcentration(DWORD n, double C) = 0;
	virtual void SetTypeConcentration(TypeComponent Type) = 0;
	// Остальное - set
	// Остальное - get
	virtual void GetVT(DWORD size, double * V, double * T, bool ** IS, double ** Values, int ** SortedComponent) = 0;
	virtual double GetPortionV(double T) = 0;
	virtual double GetTb(DWORD n) = 0;
	virtual const char * GetComponentName(DWORD n) = 0;
	virtual ComponentData GetComponentData(DWORD n, TypeComponent Type) = 0;
	virtual DWORD GetComponentNumber(const char * Name) = 0;
	//
	DWORD CountComponent; // Количество элементов
	TypeComponent GetTypeComponent()
	{
		return Type;
	}
protected:
	TypeComponent Type;
	//
};

typedef ComponentInterface* (*AddressComponentInterface)(const char* szCompName, ComponentInterface::TypeComponent TypeDefault, ComponentInterface::TypeSource Load);
