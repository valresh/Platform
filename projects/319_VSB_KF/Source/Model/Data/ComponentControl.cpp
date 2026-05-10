#include "stdafx.h"
#include "ComponentControl.h"
#include "Data.h"
ComponentControl::ComponentControl(const char* szCompName)
{
	lstrcpy(szName, szCompName);
	Mol.Read(szName);
	CountComponent = LAST_GAS;
	Type = TypeComponent::MOL;
}

void ComponentControl::Release()
{
	delete this;
}

void ComponentControl::Norm()
{
	double SummaMass = 0.0;
	switch(Type)
	{
		case TypeComponent::MOL:
		{
			double SummaMol = 0.0;
			for(DWORD i = 0; i < CountComponent; i++)
			{
				if(Mol.Cmol[i] > 0.0) SummaMol += Mol.Cmol[i];
			}
			// Нормировка
			for(DWORD i = 0; i < CountComponent; i++) if(Mol.Cmol[i] > 0.0) Mol.Cmol[i] /= SummaMol;
			MolToMass();
		}break;
		case TypeComponent::MASS:
		{
			double SummaMass = 0.0;
			for(DWORD i = 0; i < CountComponent; i++)
			{
				if(Mass.Cmol[i] > 0.0) SummaMass += Mass.Cmol[i];
			}
			// Нормировка
			for(DWORD i = 0; i < CountComponent; i++) if(Mass.Cmol[i] > 0.0) Mass.Cmol[i] /= SummaMass;
			MassToMol();
		}break;
		default: return;
	}
}

void ComponentControl::Calculate()
{
	//
}

// Concentration Mol
double ComponentControl::GetConcentrationMol(DWORD n)
{
	if(n >= CountComponent) return 0.0;
	return Mol.Cmol[n];
}

void ComponentControl::SetConcentrationMol(DWORD n, double C)
{
	if(n >= CountComponent) return;
	Mol.Cmol[n] = C;
	Mol.IsComp[n] = (Mol.Cmol[n] > 0.0);
}

// Concentration Mass
double ComponentControl::GetConcentrationMass(DWORD n)
{
	if(n >= CountComponent) return 0.0;
	return pGas[n]->Mw;
}

void ComponentControl::SetConcentrationMass(DWORD n, double W)
{
	if(n >= CountComponent) return;
	pGas[n]->Mw = W;
}

double ComponentControl::GetTb(DWORD n)
{
	if(n >= CountComponent) return 0.0;
	return  pGas[n]->Tb;
}

const char * ComponentControl::GetComponentName(DWORD n)
{
	if(n >= CountComponent) return NULL;
	return pGas[n]->Name;
}


ComponentInterface::ComponentData ComponentControl::GetComponentData(DWORD n, ComponentInterface::TypeComponent Type)
{
	ComponentData Data = {};
	if(!Mol.IsComp[n]) return Data;
	Data.ComponentName = GetComponentName(n);
	Data.Tb = GetTb(n);
	switch(Type)
	{
		case ComponentInterface::TypeComponent::MOL:
			{
				Data.C = GetConcentrationMol(n);
			}break;
		case ComponentInterface::TypeComponent::MASS:
			{
				Data.C = GetConcentrationMass(n);
			}break;
		default:
			{
				Data.C = 0.0;
			}break;
	}
	return Data;
}

void ComponentControl::MolToMass()
{
	//
}

void ComponentControl::MassToMol()
{
	//
}
