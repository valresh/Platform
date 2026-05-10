#pragma once

#include "BaseModel.h"
#include "DataTypes.h"
#include "Extern_Pnt.h"

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#undef IN_DLL
#ifdef DLL_ACS
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif



// Дискретые точки
//struct IN_DLL CDiscrete_W 
//  {
//  int Value;
//  };
struct IN_DLL CDiscrete : /*public CDiscrete_W,*/ public CExtern_Pnt
{
public:
//	CValue * pValue;	//Временный параметр для скриптов, потом нужно удалить
	bool Trip;
	int Defect;
	int Value;
	int operator = (int V)
	{
		if(Defect == 0)
		{
			Value = 0;
			return 0;
		}

		if(Defect == 1)
		{
			Value = 1;
			return 1;
		}
		Value = V;
		return V;
    }

	inline bool Use()
	{
		return IsConnection;
	}

	bool On()
	{
		if (!IsConnection) return false;
		return Value != 0;
	}

	bool Push()
	{
		if(!IsConnection) return false;
		if(Value != 0)
		{
			Value = 0;
			return true;
		}
		return false;
	}

	bool Off()
	{
		if(!IsConnection) return false;
		return Value == 0;
	}

	bool ExtOn()
	{
		if(!IsConnection) return false;
		return Value != 0;
	}

	bool ExtOff()
	{
		if(!IsConnection) return false;
		return Value == 0;
	}

	operator int()
	{
		return Value;
	}

	int operator = ( CDiscrete & D )
	{
		if(Defect == 0)
		{
			Value = 0;
			return 0;
		}
		if ( Defect == 1 )
		{
			Value = 1;
			return 1;
		}
		Value = D.Value;
		return Value;
	}

	int operator = ( struct CAnalog & A );
	int Is( )
	{
		if(Value != 0) return 1;
		else return 0;
	}

	NEW
	CDiscrete(IBaseModel * pModel, char * Point, char * Type, char * Descr = "");
	CDiscrete();
	void Init(IBaseModel * pModel, char * Point, char * Type, char * Descr = "");
	virtual bool Connect(CConnectInfo * pInfo);
	virtual void GetParams();
	bool Active(){return Trip;};
	void GetInfo(char Obj[128], char Point[64], char ACS[64]);
	// On - Move = 1, Off - Move = -1,
	void KlapOpenClose( double Vent, bool LocalBlk, int & Move, bool & Err);
	// On - Move = 1
	void KlapOpen( double Vent, bool LocalBlk, int & Move, bool & Err);
	// On - Move = -1
	void KlapClose(double Vent, bool LocalBlk, int & Move, bool & Err);
	void CmdStop(int State, bool LocalBlk, bool & Stop, bool & Err);
	void CmdStart(int State, bool LocalBlk, bool & Start, bool & Err);
	void CmdStartStop(int State, bool LocalBlk, bool & Start, bool & Stop, bool & Err);

	virtual void * GetAddress()
	{
		return (void*)&Value;
	};
};

// Аналоговые точки
//struct IN_DLL CAnalog_W
//  {
//  double Value;
//  };

struct IN_DLL CAnalog : /*public CAnalog_W,*/ public CExtern_Pnt
{
public:
//	CValue * pValue;		//Временный параметр для скриптов, потом нужно удалить
	double * pSrcAddr;
	double A, B;
	int Mes;
	double Scale;
	double Value;
	double operator = ( double V )
	{
		if ( Mes >= 0 )
		{
			int Flag = Mes / 100;
			if ( Flag & 0x01 )
			V += 1.;
			Value = ( A * ( V - B ) + B ) * Scale;
		}
		else Value = A * ( V - B ) + B;
		return Value;
	}

	inline bool Use()
	{
		return IsConnection;
	}

	operator double()
    {
		return Value;
    }

	double operator = ( CAnalog & A )
	{
		Value = A.Value;
		return Value;
	}

	double operator = ( CDiscrete & D )
	{
		Value = D.Value;
		return Value;
	}

	NEW
	CAnalog(IBaseModel * pModel, char * Point, char * Type, char * Descr = "");
	virtual bool Connect(CConnectInfo * pInfo);
	virtual void SetMes(int Mes, double Scale);
	virtual void GetParams();
	virtual void * GetAddress()
	{
		return (void*)&Value;
	};
};

//-------------------------------------------------------
struct IN_DLL CAnalogD : public CAnalog
{
public:
	double SH, SL;
	double HH, PH, LL, PL;
	int Defect;
	struct CDef * Исправен;
	double CurrSensorData;
	struct CDef * Показания_не_изменяются;
	struct CDef * Завышение_показаний;
	struct CDefParam * parm_Завышение_показаний;
	struct CDef * Занижение_показаний;
	struct CDefParam * parm_Занижение_показаний;
	struct CDef * Уход_показаний_на_максимум;
	struct CDefParam * time_Уход_показаний_на_максимум;
	struct CDef * Уход_показаний_на_начало_шкалы;
	struct CDefParam * time_Уход_показаний_на_начало_шкалы;
	struct CDef * Самопроизвольное_изменение_показаний;
	struct CDefParam * parm_Самопроизвольное_изменение_показаний;
	struct CDefParam * time_Самопроизвольное_изменение_показаний;
	struct CDef * Хаотическое_изменение_показаний;
	struct CDefParam * min_Хаотическое_изменение_показаний;
	struct CDefParam * max_Хаотическое_изменение_показаний;
	struct CDef * Сигнализация_HH;
	struct CDef * Сигнализация_H;
	struct CDef * Сигнализация_L;
	struct CDef * Сигнализация_LL;
	struct CDef * Сигнализация_IOPp;
	struct CDef * Сигнализация_IOPm;
	double Alfa, Start, Finish;
	double CurrTime, Time, p1, p2, Prop;
	bool UseCoef;
	double Coef;
    //
    //
	CAnalogD( IBaseModel * pModel, char * Point, char * Type, char * Descr = "" );
	double operator = (double V)
	{
		double Vconv = CAnalog::operator = (V);
		Value = SetDefect(Vconv);
		return V;
	}

	inline bool Use()
	{
		return IsConnection;
	}

	operator double()
	{
		return Value;
	}

	virtual bool Connect(CConnectInfo * pInfo);
	virtual void GetParams();
	int SetDefect(struct CDef * pDefect);
	double SetDefect(double V);
	NEW
};


struct _W_CACS
{
	double O;
	_W_CACS();
};

class CACS : public IBaseModel, public _W_CACS
{
public:
	CACS( char * ObjName );
	virtual ~CACS();
	// Интерфейс IBaseModel
	int Init( int Regim );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams( char * );
	int UpdateParam( struct CParams & Param );
	int SaveState ( );
	int RestoreState ( char * StrName );
	int SetData( int TypeData, void * pData );
	NEW
};
