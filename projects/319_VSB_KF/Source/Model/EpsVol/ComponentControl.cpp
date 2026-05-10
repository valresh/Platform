#include "stdafx.h"
#include "ComponentControl.h"
#include "Data.h"
#include "SetData.h"

int CompComp(const void * p1, const void * p2)
{
	int n1 = *(int*)p1;
	int n2 = *(int*)p2;
	if(pGas[n1]->Tb < pGas[n2]->Tb) return -1;
	if(pGas[n1]->Tb > pGas[n2]->Tb) return 1;
	return 0;
}
ComponentControl::ComponentControl(const char* Name, TypeComponent TypeDefault, TypeSource source)
{
	CountComponent = LAST_GAS;
	Type = TypeDefault;
	PointMol = NULL;
	PointIs = NULL;
	ComponentName = NULL;
	Read(Name, source);
	memset(SortedComponent, 0, sizeof(SortedComponent));
	for(DWORD i = 0; i < CountComponent; i++ ) SortedComponent[i] = i;
	qsort(SortedComponent, size_t(CountComponent), sizeof(int), CompComp);
}

bool ComponentControl::Read(const char * Name, TypeSource source)
{
	Source = source;
	switch(Source)
	{
		case FILE:
			{
				bool readed = Mol.Read((char*)Name);
				// Если не удалось прочитать файл, то выходим, ничего не меняя
				if(!readed) return false;
				for(DWORD i = 0; i < CountComponent; i++) Mol.IsComp[i] = (Mol.Cmol[i] > 0.0);
				PointMol = Mol.Cmol;
				PointIs = Mol.IsComp;
			}break;
		case MODEL:
			{
				CExtCompInfo Info;
				IBaseModel * object = IBaseModel::Find((char*)Name);
				if(object)
				{
					int s = object->SetData(sd_GetCompInfo, &Info);
					PointMol = Info.Cmol;
					PointIs = Info.IsComp;
				} // иначе - не меняем указатели на моли при типе источника - модель
			}break;
		default: break;
	}
	// Если нужно отобразить как массовые доли(а читаем только как мольные доли
	if(Type == MASS)
	{
		Type = MOL;
		SetTypeConcentration(MASS);
	}
	return true;
}

void ComponentControl::Write(const char * Name)
{
	bool massexist = (Type == MASS);
	if(massexist) SetTypeConcentration(MOL);
	switch(Source)
	{
		case FILE:
		{
			for(size_t i = 0;i<LAST_GAS;i++) Mol.IsComp[i] = (Mol.Cmol[i] > 0.0);
			Mol.Write((char*)Name);
		}break;
		case MODEL:
		{
			CComp Comp;
			memcpy(Comp.Cmol, PointMol, sizeof(Comp.Cmol));
			for(DWORD i = 0; i < CountComponent; i++) Comp.IsComp[i] = (Comp.Cmol[i] > 0.0);
			Comp.Norm(0.0);
			Comp.Write((char*)Name);
		}break;
		default: return;
	}
	if(massexist) SetTypeConcentration(MASS);
}

void ComponentControl::Release()
{
	delete this;
}

DWORD ComponentControl::GetNumberLastComponent()
{
	return LAST_GAS;
}

void ComponentControl::Norm()
{
	if(!PointMol) return;
	double Summa = 0.0;
	// Создаем универсальный указатель
	double * Point = UniversalPoint();
	if(!Point) return;
	//-----------------------------------------------------------
	// Нормировка по универсальному указателю
	for(DWORD i = 0; i < CountComponent; i++)
	{
		if(Point[i] > 0.0) Summa += Point[i];
	}
	// Нормировка
	for(DWORD i = 0; i < CountComponent; i++) if(Point[i] > 0.0) 
	{
		Point[i] /= Summa;
	}
}

void ComponentControl::Calculate()
{
	//
}


// Concentration
double ComponentControl::GetConcentration(DWORD n)
{
	double * Point = UniversalPoint();
	if(!Point) return 0.0;
	if(n >= CountComponent) return 0.0;
	return Point[n];
}

void ComponentControl::SetConcentration(DWORD n, double value)
{
	double * Point = UniversalPoint();
	if(!Point) return;
	if(n >= CountComponent) return;
	Point[n] = value;
}

void ComponentControl::SetTypeConcentration(TypeComponent type)
{
	if(Type == type) return;
	Type = type;
	switch(Type)
	{
		case ComponentInterface::MASS:
		{
			MolToMass(CountComponent, PointMol, Mass.Cmol);
		}break;
		case ComponentInterface::MOL:
		{
			MassToMol(CountComponent, Mass.Cmol, PointMol);
		}break;
		default: break;
	}
}

double ComponentControl::GetTb(DWORD n)
{
	if(n >= CountComponent) return 0.0;
	return  pGas[n]->Tb;
}

ComponentInterface::ComponentData ComponentControl::GetComponentData(DWORD n, ComponentInterface::TypeComponent Type)
{
	ComponentData Data = {};
	if(!PointMol[n]) return Data;
	Data.ComponentName = GetComponentName(n);
	Data.Tb = GetTb(n);
	Data.C = GetConcentration(n);
	return Data;
}

DWORD ComponentControl::GetComponentNumber(const char * Name)
{
	int id = GetGasID((char*)Name);
	return DWORD(id);
}

double ComponentControl::Norm(double * Values, DWORD size)
{
	double S = 0.0;
	for(DWORD i=0; i < size;i++) if(Values[i] > 0.0) S += Values[i];
	if(S > 0.0) for(DWORD i=0; i < size;i++) if(Values[i] > 0.0) Values[i] /= S;
	return S;
}

// Y[i] = Nu[i]/Nu
// m = Mu*Nu
// C[i] = m[i]/m
// m[i] = Nu[i]*Mu[i] = Nu[i]*Y[i]*Nu = m*C[i]
// m = Summa(m[i]) = Summa(Nu[i]*Mu[i])
// Итого: C[i] = Y[i] * Mu[i]*Nu/m

void ComponentControl::MolToMass(DWORD Count, double * SourceMol, double * DestignationMass)
{
	// Ищем суммарное значение моли(Nu)
	double SummaMol = 0.0;	
	for(DWORD i = 0; i < Count; i++) if(SourceMol[i] > 0.0) SummaMol += SourceMol[i];
	// Ищем суммарное значение массы(m)
	double SummaMass = 0.0;
	for(DWORD i = 0; i < Count; i++) if(SourceMol[i] > 0.0) SummaMass += SourceMol[i] * pGas[i]->Mw;
	//--------------------------
	for(DWORD i = 0; i < Count; i++) if(SourceMol[i] > 0.0)
	{
		double K = pGas[i]->Mw * SummaMol / SummaMass;
		DestignationMass[i] = SourceMol[i] * K;
		PointIs[i] = true;
	}else
	{
		DestignationMass[i] = 0.0;
		PointIs[i] = false;
	}
}

void ComponentControl::MassToMol(DWORD Count, double * SourceMass, double * DestignationMol)
{
	// Ищем суммарное значение моли(Nu)
	double SummaC = 0.0;	
	for(DWORD i = 0; i < Count; i++) if(SourceMass[i] > 0.0) SummaC += SourceMass[i];

	// Ищем суммарное значение K[i] = C[i]/Mu[i], summa(K[i]) = summa(Y[i]) * Nu / m = Nu / m
	double SummaK = 0.0;
	for(DWORD i = 0; i < Count; i++) if(SourceMass[i] > 0.0) SummaK += SourceMass[i] / pGas[i]->Mw;
	// SummaK = Nu / m
	for(DWORD i = 0; i < Count; i++) if(SourceMass[i] > 0.0)
	{
		double Nu = SourceMass[i] / pGas[i]->Mw / SummaK * SummaC;
		DestignationMol[i] = Nu;
	}else DestignationMol[i] = 0.0;
	for(DWORD i=0;i<CountComponent;i++) PointIs[i] = (DestignationMol[i] > 0.0);
}

double * ComponentControl::UniversalPoint()
{
	switch(Type)
	{
		case MOL: return PointMol;
		case MASS: return Mass.Cmol;
		default: return NULL;

	}
}


//====================================================================================
// Всё для GetVT
char * ListNoUsed[] =
{
	"OXYGEN",
	"NITROGEN",
	"Вода",
	"HYDROGEN",
	"HYDROGEN SULFIDE",
	"AMMONIA",
	"CARBON MONOXIDE",
	"CARBON DIOXIDE",
	NULL
};
#define CPY(Dst,Src) 	memcpy ( Dst, Src, sizeof ( Dst ));
#define CLEAR(Var) memset(Var,0,sizeof(Var));
bool InList(char * Name)
{
	int n = 0;
	while(ListNoUsed[n])
	{
		if(lstrcmp(Name, ListNoUsed[n]) == 0) return true;
		n++;
	}
	return false;
}

const char * ComponentControl::GetComponentName(DWORD n)
{
	if(n >= CountComponent) return NULL;
	return pGas[n]->Name;
}
#include "EpsVol.h"
void ComponentControl::GetVT(DWORD size, double * VV, double * TT, bool ** IS, double ** Values, int ** sortedcomponent)
{
	const double P = 1.0;
	const double Tcool = 15.0;
	*IS = OutIs;
	*Values = Out;
	*sortedcomponent = SortedComponent;
	if(!PointMol) return;
	CSEps Src("Источник");
	CSEps Dst("Приемник");
	double Copy[K_GAS];
	bool CopyIs[K_GAS];
	double Liquid[K_GAS];
	double Gas[K_GAS];
	double SummDst[K_GAS];
	memcpy(Copy, PointMol, sizeof(Copy));
	//--------------------------------------
	double Tmin = 1000.0;
	double Tmax = -1000.0;
	for(DWORD n = 0; n < CountComponent; n++)
	{
		if(Copy[n] > 0.0)
		{
			if(InList(pGas[n]->Name))				
				Copy[n] = 0.0;
			CopyIs[n] = Copy[n] > 0.0;
		}
	}
	// Нормировка
	double S = Norm(Copy, CountComponent);
	if(S < 1e-3) return;
	memcpy(Out, Copy, sizeof(Copy));
	for(DWORD i=0;i < CountComponent; i++) OutIs[i] = (Out[i] > 0.0);

	for(DWORD i = 0; i < CountComponent; i++) if(Copy[i] > 0.0)
	{
		if(pGas[i]->Tb < Tmin) Tmin = pGas[i]->Tb;
		if(pGas[i]->Tb > Tmax) Tmax = pGas[i]->Tb;
	}
	
	if(Tmin < Tcool) Tmin = Tcool;
	Src.T_min = Tmin;
	Src.T_max = Tmax;
	Dst.T_min = Tmin;
	Dst.T_max = Tmax;
	double dT = (Tmax - Tmin) / (size - 1);
	double Msrc = 1.0;
	CLEAR(SummDst);
	double Mdst = 0.0;
	double Vol0 = 0.0;
	Src.PT(P, Tmin, CopyIs, Copy);
	double eps0 = Src.E;
	if(eps0 > 0.0)
	{
		// Отделяем жидкую часть
		Src.SetCompData(NULL, Liquid, Gas, NULL, NULL);
		memcpy(Copy, Liquid, sizeof(Liquid));
	}
	Norm(Copy, CountComponent);
	for(DWORD i=0;i < CountComponent; i++) CopyIs[i] = (Copy[i] > 0.0);
	double ToM3 = To_m3_liq(Tcool, CopyIs, Copy);
	Vol0 = Msrc * ToM3;
	for(DWORD n = 0; n < size; n++)
	{
		VV[n] = 100.0;
		TT[n] = Tmax;
	}
	for(DWORD n = 0; n < size; n++)
	{
		double T = Tmin + n * dT;
		TT[n] = T;
		Src.PT(P, T, CopyIs, Copy);
		double V = Src.E * Msrc;// Испаренная часть
		if(V <= 1e-7)
		{
			VV[n] = 0.0;
			continue;
		}
		Msrc -= V;
		if(Msrc <= 1e-3) break;
		Src.SetCompData(NULL, Liquid, Gas, NULL, NULL);
		memcpy(Copy, Liquid, sizeof(Liquid));
		for(DWORD c = 0; c < CountComponent; c++)
		{
			if(CopyIs[c])
			{
				SummDst[c] = Mdst * SummDst[c] + V * Gas[c];
			}
		}
		Mdst = Norm(SummDst, CountComponent);
		double ToM3 = To_m3_liq(Tcool, CopyIs, SummDst);
		double Vol = Mdst * ToM3;
		VV[n] = Vol / Vol0 * 100.0;
	}
}


double ComponentControl::GetPortionV(double T)
{
	const double P = 1.0;
	const double Tcool = 15.0;
	CSEps Src("Источник");
	CSEps Dst("Приемник");
	double Liquid[K_GAS];
	double CopyMol[K_GAS];
	bool CopyMolIs[K_GAS];
	double SummDst[K_GAS];
	double Gas[K_GAS];
	double Msrc = 1.;
	// Берем тольео копию компонентов
	memcpy(CopyMol, PointMol, sizeof(CopyMol));
	// Из неё исключаем "особенные" значения: кослород, вода
	for(DWORD n = 0; n < CountComponent; n++)
		if(CopyMol[n] > 0.0)
			if(InList(pGas[n]->Name))
				CopyMol[n] = 0.0;
	// Заполняем булевый массив
	for(DWORD n = 0; n < CountComponent; n++) CopyMolIs[n] = (CopyMol[n] > 0.0);

	Src.PT(P, T, CopyMolIs, CopyMol);
	double V = Src.E * Msrc;// Испаренная часть
	if(V <= 1e-7) return 0.0;
	Msrc -= V;
	if(Msrc <= 1e-3) return 0.0;
	Src.SetCompData(NULL, Liquid, Gas, NULL, NULL);
	memcpy(CopyMol, Liquid, sizeof(Liquid));
	double Mdst = 0.0;
	double Vol0 = 0.0;
	for(DWORD c = 0; c < CountComponent; c++)
	{
		if(PointIs[c])
		{
			SummDst[c] = Mdst * SummDst[c] + V * Gas[c];
		}
	}
	Mdst = Norm(SummDst, CountComponent);
	for(DWORD i=0;i < CountComponent; i++) CopyMolIs[i] = (SummDst[i] > 0.0);
	double ToM3 = To_m3_liq(Tcool, CopyMolIs, SummDst);
	double Vol = Mdst * ToM3;
	return Vol / Vol0 * 100.0;
}
