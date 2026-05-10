#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "GydroData.h"
#include "Data.h"
#include "Chem.h"
#include "EpsVol.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "tbp.h"
#include "Control.h"
#include "Extensions.h"


struct _W
  {
  double Val_old;
  double Value;//"Показания с учетом размерности")
  double Phys;//"Показания в исходной размерности")
 _W();
  };

/*
struct CSensCalc : public CFunctions
  {
  virtual int StdFunc( char * Name );
  virtual int CallStdFunc( int nFunc, 
    int kVarStack,
    CVar * VarStack[256],
    int nLine, bool Imitation );
//
  int MolC( int kVarStack,
    CVar * VarStack[256],
    int nLine, bool Imitation);
  int MassC( int kVarStack,
    CVar * VarStack[256],
    int nLine, bool Imitation);
  };
*/
#define MAX_ANALYZ_POINTS 40
#define K_APPR 8
#undef SET_DEF
#define SET_DEF(DefectVar,Comment) CDef * DefectVar;
#undef SET_PARM
#define SET_PARM(Var, vName, vDef, vMin, vMax ) CDefParam * Var;

enum eDefect { eИсправен = 0, eПоказания_не_изменяются, eЗавышение_показаний, eЗанижение_показаний,
eУход_показаний_на_максимум, eУход_показаний_на_начало_шкалы, eСамопроизвольное_изменение_показаний,
eХаотическое_изменение_показаний,
eСигнализация_HH, eСигнализация_H, eСигнализация_L, eСигнализация_LL,
eСигнализация_IOPp, eСигнализация_IOPm,
  };


struct CSensDefects
  {
  eDefect Defect;
  double Alfa, Start, Finish;
  double CurrTime, Time, p1, p2, Prop;
  SET_DEF(Исправен, "Исправен" );
  SET_DEF(Показания_не_изменяются, "Показания не изменяются" );
  SET_DEF(Завышение_показаний, "Завышение показаний" );
  SET_PARM( parm_Завышение_показаний, "Завышение", 1.2, 1., 10. )
    SET_DEF(Занижение_показаний, "Занижение показаний" );
  SET_PARM( parm_Занижение_показаний, "Занижение", 1.2, 1., 10. )
    SET_DEF(Уход_показаний_на_максимум, "Уход показаний на максимум" );
  //		  SET_PARM( time_Уход_показаний_на_максимум, "Время изменения с", 0., 0., 3600. )
  SET_DEF(Уход_показаний_на_начало_шкалы, "Уход показаний на начало шкалы" );
  //		  SET_PARM( time_Уход_показаний_на_начало_шкалы, "Время изменения с", 0., 0., 3600. )
  SET_DEF(Самопроизвольное_изменение_показаний, "Самопроизвольное изменение показаний" );
  SET_PARM( parm_Самопроизвольное_изменение_показаний, "Конечное значение", 0., -100., 10000. )
    SET_PARM( time_Самопроизвольное_изменение_показаний, "Время изменения с", 0., 0., 3600. )
    SET_DEF(Хаотическое_изменение_показаний, "Хаотическое изменение показаний" );
  SET_PARM( min_Хаотическое_изменение_показаний, "Минимальное значение", 0., -100., 10000. )
    SET_PARM( max_Хаотическое_изменение_показаний, "Максимальное значение", 0., -100., 10000. )
  CSensDefects()
    {
    memset ( this, 0, sizeof ( *this ));
    }
  };

class CSens : public IBaseModel, public IUniModel, public _W, public CSensDefects
{
//
public:
	CSens( char * ObjName );
	virtual ~CSens();
// Интерфейс IBaseModel
	int Init( int );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams(  char * StrName );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
	int UpdateParam( struct CParams & Param );
	int SaveState ( );
	int RestoreState ( char * StrName );
	int SetData( int TypeData, void * pData );
	int OnDefect( CDef * pDefect );
	void DrawObj ( struct CDrawObjData * pDraw );
	double AdditonalComp( CComp * pComp, int nAdd, double Dry  );
	virtual void AddResources (CGDIResourceMgr *pResMgr);
	virtual void DrawObject ( struct CDrawObjData * pDraw, CGDIResourceMgr *pResMgr);
    virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
	virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
		double A[/* kExkV */], double B[ /*kE*/ ] );
	virtual int SetVar( void * pExternals, int PntGroup, double dt,
		double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	int Work( double dt );
  double CalcCoefTBP( double V );
	int TestCtrlPoints( int kNodes, struct CObjectPoint ** _ppNodes );
  virtual bool GetParamsList( struct CParams ** pParams, int * kParams );
  static CParams * pParams;
  static int kParams;
  static DWORD ClassRootNode;
	NEW
/////////////////////////////////////////////////
	//CSensor * pSens;
	CFlow * pF;
	int kStep;
	bool WasCalcT;
	CSEps SensT;
	double Pin, Pout;
	double F_mol;
  static bool МассовыеДоли;
  bool КривыеКипения;
  static bool UseAnalysComp[K_GAS];
  CAnalogControl * pControlVal;
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
#include "CtrlVar.h"
/////////////////////////////////////////////////
//  CCalcu * pCalc;
//  bool UseCalc;
	CGasAnalyz * pAnalyz;
	double k_Analyz[MAX_ANALYZ_POINTS];
	int * CalcAnalyz;
	int kAnalyzData;
	//CTBP * pTBP;
  double * CoefTBP;
  int kCoefTBP; 
  bool AddCoefTBP;
	bool AnalysInit;
//	CScript Script;
  double CycleTime;
	double dt_hour;
  bool InvalidMin;
  bool InvalidMax;
  IGamma * pGamma;
//
 
  static char* pCompName;
  enum
  {
	  Нет_типа = 0,
	  Тип_Температура,
	  Тип_Поток,
	  Тип_Давление_и,
	  Тип_Давление_а,
	  Тип_Перепад,
	  Тип_Плотность,
	  Тип_Концентрация,
	  Тип_Уровень,
	  Тип_Молекулярный_вес
  };
  //////////////////////////////////////////////////////////////////////////
	void Calc ( double dt );
	void DrawState ( void * pData );
	double Calc_Ro( );
	double Calc_T( double dt );
	double Calc_F( );
  double Calc_Ro_Gas_Std( );
  double Calc_D( );
	double Calc_P( bool Abs );
	double Slow( double Value, double dt );
	void Analyz( );
  void NewAnalyz( );
	void PrepareAnalyz( );
	void SetAnalyzData( );
	double T_PROC( double Proc );
	double V_PROC( double Proc );
  void NewPrepareAnalyz( );
  //void NewSetAnalyzData( );
  //double Calc_k_Ro ( double T );
  void CreateDefects( );
  int SetDefect ( struct CDef * pDefect );
  double SetDefect( double V );
};


template <int kCol, int max_size>
int Split ( char * Str, char * Cols/*[kCol][max_size]*/ )
  {
  int L = lstrlen( Str );
  if ( Str[L-1] == 0x0d || Str[L-1] == 0x0a )
    {
    L--;
    Str[L] = 0;
    }
  int K = 0;
  memset( Cols, 0, kCol * max_size );
  char * Pnext = NULL;
  char * P = Str;
  char * Out = Cols;
  while ( K < kCol )
    {
    Pnext = strchr ( P, ';' );
    if ( Pnext == NULL )
      {
      if ( *P == 0 )
        return K + 1;
      int L = lstrlen ( P );
      if ( L >= max_size )
        P[max_size-1] = 0;
      lstrcpy ( Out, P );
      return K+1;
      }
    *Pnext = 0;
    int L = lstrlen ( P );
    if ( L >= max_size )
      P[max_size-1] = 0;
    lstrcpy ( Out, P );
    Out += max_size;
    P = Pnext + 1;
    K++;
    }
  return K;
  }

template <int kCol, int max_size>
int Split ( FILE * F, char * Cols /*[kCol][max_size]*/)
  {
  char Str[256];
  if ( fgets ( Str, 255, F ) == NULL )
    return -1;
  return Split<kCol,max_size>( Str, Cols );
  }
