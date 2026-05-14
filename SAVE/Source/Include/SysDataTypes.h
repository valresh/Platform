#pragma once

#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

#include "BaseType.h"
#include "Param.h"
#include "DataTypes.h"
//////////////////////////////////////////////////////////////////////////////
// Типы и структуры данных, используемых в Sumulator'е

struct CExternalCondition : public CBase
{
    enum { TypeID = id_ExternalCondition };
    static char * ClassName;
    static void BuildList();
    double T_Air;	// Температура окружающего воздуха//-параметр(2)
    double PressAirKip;//Давление воздуха КИП
    bool WorkUPS;
    bool WorkDCS;
    bool IsElectro_380;
    bool IsElectro_6000;
    bool IsPumpSbros;
    bool IsBlk;
    bool CtrlMinMax;
    bool IsEmptyVol;
    //
    int Init( );
    int GetParams(  );
    int SaveState( );
    int RestoreState( char * StrName );
};

struct CSystemA
{
    //
    int nEvents;// Количество записей в Events.dat
    int nTrends;// Количество записей в Trends.dat
    int nMsgToOperator;// Количество записей в MsgToOperator.dat
};
// 1-ый процессор
struct CSystemB
{
    //
    enum
    {
      STATUS_INIT    = 0,
      STATUS_UNKNOWN = 1,
      STATUS_START   = 2,
      STATUS_PAUSE   = 3,
    };
    //
    __time64_t startTime;  // Для отображения в АРМ-ах модельного времени
    int        m_nStep;    // Количество шагов Не надо обнулять при остановке модели
    BYTE       StartStatus;// Состояние модели
    double     AccelZ;     // Заданное ускорение
    int        nAppRun;    // Откуда запущена модель
    int        kErrors;
    //
    double dModelT;// Модельное время (час)
    double dt;     // Модельный шаг (час)
    double Accel;  // Текущее ускорение
    double Time_real;
    __time64_t TimeB()// Время от начала модели (сек)
    {
      return __time64_t( dModelT * 3600 );
    }
    __time64_t TimeM()// Полное модельное время (сек)
    {
      return startTime + TimeB();
    }
};

struct CSystem_Chem
{
    //
    bool Use_Coke;
    bool Use_S;
    bool WaterIAPWS;
};

#define K_THREADS 8

struct CSystem : public CBase, public CSystemA, public CSystemB, public CSystem_Chem
{
    enum { TypeID = id_System };
    static char * ClassName;
    double T2;// копия T модельного времени для восстановления работы упражнения при bContinue=true из Start(...)
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Параметры не подлежащие сохранению
    int UseProcessors;
    int kProcessors;
    //
    int kCall[K_THREADS];
    void * hThread[K_THREADS];
    DWORD ThreadId[K_THREADS];
    double Used[K_THREADS];
    //
    DWORD DbgProcessorFlag;
    DWORD DbgResetFlag;
    DWORD IdDbg;
    //
    int StepCalcAccel;
    bool   bNoPCYWork;  // True - PCY не работает, False - PCY работает
    //
    bool IsPumpSbros;
    bool IsBlk;
    bool bSaveParams;
    bool bRestoreParams;
    bool bSaveState;
    bool bRestoreState;
    bool bWorkReg;
    bool bInit_0;
    bool Debug;
    bool bModelPause;
    //
    int nSave;
    char Mod_Name[2];
    char FileWriteState[256];
    char FileWriteParams[256];
    char FileReadState[256];
    char FileReadParams[256];
    //
    double Speed;
    //  char FileBlk[256];//описаны состояния блокировок (ON/OFF); используется AcyBlk.dll
    //  ULONGLONG Offset_FirstDefect; // смещение первого отказа относительно pMemory
    //  char FileView[256];//используется Control
    //  int nView;//используется Control
    //
    //	bool IsBlkWork;
    //	bool AutoReg;// При true - возможна самонастройка
    //	bool CtrlStab;// контроль стабильности
    enum eShowParamsLevel { Основной, Полный, Разработчик };
    eShowParamsLevel ShowParamsLevel;
    //
    int Init( );
    int GetParams(  );
    int SaveState( );
    int RestoreState( char * StrName );
    static void BuildList();
};
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

struct CShSystem: public CShBase, public CSystemA, public CSystemB
{
    SH_BASE(CShSystem)
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    int  Model      ;// Номер модели
    bool bNoPCYWork ;// True - PCY не работает, False - работает
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Время старта модели
    __time64_t Start(){ return startTime; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};


struct CSysQuery
{
    enum
    {
      Pump_Off = 1
    };
    int Type_Query;
    char ObjName[MAX_OBJ_NAME+1];//
};

extern IN_DLL IBaseModel * pMainHydro;
extern IN_DLL IBaseModel * pCtrlConn;
