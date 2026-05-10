#pragma once

#define REG_LIB_NAME				"CtrlReg"
#define REG_PROC_NAME				"GetInterface"

struct CExtRegOut
{
	enum
	{
		MAX_REG_NAME = 64,
	};
	CExtRegOut* pNextOut;
	char Ref[MAX_REG_NAME];
	double MV_0, MV_1;
	double* pMV;
	virtual CExtRegOut* GetNextOut() = 0;
};

struct CExtReg_W
{
	double SP, PV, OP;
	double Delta;
};

struct CExtReg : public CExtReg_W
{
	enum
	{
		MAX_REG_NAME = 64,
		MAX_GOAL_NAME = 64,
	};
	char Name[MAX_REG_NAME];
	double K, T, TD;
	char szGoal[MAX_GOAL_NAME];
	double Min, Max;
	bool On;
	double TrendScale;
	int TrendStep;
	double * pPV;
	int nTrend;	
	virtual CExtReg* GetNextReg() = 0;
	virtual CExtRegOut* GetFirstOut() = 0;
	virtual CExtRegOut* AddOut(const char* szRef, double MV_0, double MV_1) = 0;
	virtual void DeleteOut(CExtRegOut* pDelete) = 0;
};



class IRegInterface
{
public:
	
	virtual CExtReg* GetFirstReg() = 0;
	virtual CExtReg* AddReg( const char* szRegName, double K, double T, double TD, const char* szGoal, double SP, double Min, double Max, bool On ) = 0;
	virtual void ChangeReg( CExtReg* pReg ) = 0;
	virtual void DeleteReg(CExtReg* pDelete) = 0;
	virtual void Save() = 0;
};

typedef IRegInterface* (*LPGET_REG_INTERFACE)();
