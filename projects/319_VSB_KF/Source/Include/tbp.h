#pragma once

#include "EpsVol.h"
#include "Data.h"

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#undef IN_DLL
#ifdef DLL_TBP
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif

class IN_DLL CTBP 
	{
	public:
	double P;
	double T_cool;
	double Eps_0;
	CSEps Src;
	CSEps Dst;
	bool IsComp[K_GAS];
	double Liq[K_GAS];
	double Gas[K_GAS];
	double SummDst[K_GAS];
	double SummSrc[K_GAS];
	bool Calc ( CComp * pComp, int kPnt, double V[], double T[] );
	CTBP();
	};

