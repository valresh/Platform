#ifndef PARAM_H
#define PARAM_H
#pragma once

#include "BaseModel.h"
#include "CommProc.h"
#include "stdio.h"
//#include <iostream>
//#include <thread>
#include <mutex>

#define SIZE_PARAMS_BLK 128
#define K_PARAMS_BLK 512
#define MAX__PARAMS 25000

#undef IN_DLL
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define QParams QParams
#define IN_DLL Q_DECL_IMPORT
#endif

struct IN_DLL CLockParams
{
    int * kParams;
    CLockParams();
    ~CLockParams();
};

struct IN_DLL SParmStrVal
{
    char szName[16];
    union
    {
        int iValue;
        BYTE eValue;
    };
};


struct IN_DLL QParamsHelpInfo
{
    static QParamsHelpInfo * pFirst;
    static QParamsHelpInfo * pLast;
    QParamsHelpInfo * pNext;
    char * pVarName;
    CStr ParamName; // char * неудобно при генерации имени параметра
    char * pDescription;
    char * pDllInfo;
    QParamsHelpInfo();
    QParamsHelpInfo( char * Descr );
    void operator = ( const char * Descr )
    {
      pDescription = (char*)Descr;
    }
    operator char * ()
    {
      return pDescription;
    }
};

struct IN_DLL IFreeParam
{
    virtual bool SetValue ( const char * Value ) = 0;
    virtual void ChangeVal( double Delta ) = 0;
    virtual void Out ( char Txt[256] ) = 0;
    virtual bool IsWrite() = 0;
    virtual int Len() = 0;
    virtual void * Data() = 0;
    virtual bool SetData( int Len, const void * pData ) = 0;
};

struct IN_DLL IShowParam
{
    virtual void Show ( struct QParams * Param ) = 0;
    virtual double Show_Dbl ( struct QParams * Param ) = 0;
};

#define MAX_PARM_NAME 128

struct IN_DLL QParams
    {
    struct IBaseModel * pModel;
    QParams * pNextParam;
    char ParamName[MAX_PARM_NAME+1]; // char * неудобно при генерации имени параметра
    char Type;
    bool AbsAddr;
    QParams * pNextChanged;
    static QParams * pFirstChanged;
    //
    void * Addr;
    int   Len;
    void * pVarName;// Имя переменной
    void * pDefValue;//Указатель на значение по умолчанию
    char * pModelDescr;//Описание
    char * pExternDescr;
    QParamsHelpInfo * pHelpInfo;//Описание
    IShowParam * pShowParam;
    bool  * Used; // Признак того, что параметр выводится и его нужно считать
    bool  m_bNoSave;//Явный указатель ненужности сохранения
    bool  Property; // Признак того, что параметр - свойство
    char Class;
    int Visible;
    //
    SParmStrVal * pextVals;
    int extCount;
    enum Flag
    {
      FLAG_TABLE = 0x00000001,
      FLAG_STATE = 0x00000002,
      FLAG_PARAM = 0x00000004,
    };
    DWORD dwFlags;
    //
  public:
    //  static CMem<QParams,SIZE_PARAMS_BLK,K_PARAMS_BLK> * pParams;
    static QParams Params[MAX__PARAMS];
    static int kParams;
    static bool SaveAllParams;
    static bool ReadProps;
    static bool Test_Default;
    static std::recursive_mutex CS;
    static char * TabPrefix;
    static bool GetParamsList;
  public:
    static void LockParams( );
    static void FreeParams( );
    static void Reset( );
    static QParams & Get();
    static QParams & GetLast();
    static bool AddParmToList( const char * Name );
    static bool AddPropToList( const char * Name );
    static bool LastIsTab();
    //
    QParams();
    ~QParams();
    QParams( struct IBaseModel * pModel,
            char * _ParamName, QParams& par );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, double & pD, int Len  );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, float & pF, int Len  );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, short & pS, int Len  );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, int & pI, int Len  );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, __int64 & pI, int Len  );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, unsigned int & pI, int Len  );
    void SetE( struct IBaseModel * pModel,
              char * _ParamName, unsigned int & pI, int Len, int C = 0, SParmStrVal *pVals = NULL  );
    void SetE( struct IBaseModel * pModel,
              char * _ParamName, unsigned char & pE, int Len, int C = 0, SParmStrVal *pVals = NULL  );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, bool & pB, int Len  );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, char * pStr, int Len );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, unsigned char& pU, int Len );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, char & pC, int Len );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, struct SModePole & mode, int Len );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, CStr & Str );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, IFreeParam & IParam, int Len );
    void Set ( struct IBaseModel * pModel,
             char * _ParamName, struct CGrafParams & Param, int Len );
    void SetAddr ( struct IBaseModel * pModel,
                 char * _ParamName,
                 void * Addr, char Type, int Len );
    void SetDefect ( struct IBaseModel * pModel,
                   char * _ParamName,
                   void * pDefect );
    // Для структур
    void Set ( char * _ParamName, char * ClassName, struct CEnums * pEnum );
    void Set ( char * _ParamName, double & pD );
    void Set ( char * _ParamName, int & pD );
    void Set ( char * _ParamName, unsigned char & pD );
    void Set ( char * _ParamName, double * pD, int Len );
    void Set ( char * _ParamName, int * pD, int Len );
    void Set ( char * _ParamName, BYTE * pD, int Len );
    //
    bool Out ( char Txt[256] );
    bool OutNoM ( char Txt[256] );
    bool SetValue ( char Value[] );
    bool ChangeVal( double Delta );
    void WrtParam( );
    void SafeCopy ( const char * Name );
    void SafeCopyTab ( const char * Name );
    //
    static bool IsFixed(void* addr);
    bool IsWrite ( );
    bool IsRead ( );
    inline void SetDefaultValue(void* def,bool bSave=false)
    {
      m_bNoSave = bSave;
      pDefValue = def;
      dwFlags = 0;
    }
    inline bool ItYou( void* ptr )
    {
      return (Addr == ptr);
    }
    int Copy(BYTE* mem,void*& addr);
    void WasChanged( );
    void operator += ( QParams & Param );
    //
};

struct CSetParam
{
    struct IBaseModel * pModel;
    CSetParam( struct IBaseModel * _pModel )
    {
      pModel = _pModel;
    }
    void operator()( double & V, char * Name )
    {
      QParams & __P = QParams::Get();
      __P.SetDefaultValue( NULL );
      __P.Set( pModel, Name, V, 8 );
      __P.pModelDescr = NULL;
    }

    void operator()( double & V, char * Name, char * Descr )
    {
      QParams & __P = QParams::Get();
      __P.SetDefaultValue( NULL );
      __P.Set( pModel, Name, V, 8 );
      __P.pModelDescr = Descr;
    }

    void operator()( double & V, char * Name, double Default )
    {
      QParams & __P = QParams::Get();
      __P.SetDefaultValue( &Default );
      __P.Set( pModel, Name, V, 8 );
      __P.pModelDescr = NULL;
    }

    void operator()( double & V, char * Name, double Default, char * Descr )
    {
      QParams & __P = QParams::Get();
      __P.SetDefaultValue( &Default );
      __P.Set( pModel, Name, V, 8 );
      __P.pModelDescr = Descr;
    }
};

struct CSelfParam : public QParams
{
    int I;
    CStr Str;
    double D;
};

struct CCopyParam : public QParams
{
    struct {
        int I;
        CStr Str;
        double D;
    } Old;
    struct {
        int I;
        CStr Str;
        double D;
    } New;
};

struct IN_DLL CParmInfo
{
    bool B;
    int I;
    double D;
    CStr Str;
    bool bUpdate;
    //
    char * VarName;
    char * ParmName;
    char Type;
    int Len;
    int Shift;
    char * Default;
    int Show;
    CParmInfo(char * _VarName,
              char * _ParmName,
              char _Type,
              int _Len,
              int _Shift,
              char * _Default,
              int _Show )
    {
      VarName = _VarName;
      ParmName = _ParmName;
      Type = _Type;
      Len = _Len;
      Shift = _Shift;
      Default = _Default;
      Show = _Show;
      bUpdate = false;
    }
    CParmInfo( )
    {
      VarName = "";
      ParmName = "";
      Type = 0;
      Len = 0;
      Shift = 0;
      Default = "";
      Show = 0;
      bUpdate = false;
    }
    void SetValue ( char Value[] );
    void Out( char Txt[256] );
};

struct CAddData
{
    void * pData1;
    void * pData2;
    void * pData3;
    void * pData4;
    void * pData5;
    CAddData()
    {
      pData1 = NULL;
      pData2 = NULL;
      pData3 = NULL;
      pData4 = NULL;
      pData5 = NULL;
    };
};

struct IN_DLL CGrafParams
{
    enum eTypeOut { Текст, Диаграмма, Линия };
    eTypeOut TypeOut;
    char SubType;
    void * Addr;  // Для Текст
    double Min, Max; // Для Диаграмма, Линия
    double Top, Bottom; // Для Линия
    CGrafParams()
    {
      memset ( this, 0, sizeof ( *this ));
    }
};

#define PARM(Val,Name) \
{QParams & __P = QParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.Set( NULL, Name, Val, sizeof ( Val ) );}
#define PARM_L(Val,Name,Def) \
{QParams & __P = QParams::Get();\
    __P.pDefValue = Def;\
    __P.SetAddr ( NULL, Name, &Val, 'L', 4 );}
#define PARM_EX(Val,Name,nDefault) \
{QParams & __P = QParams::Get();\
    __P.SetDefaultValue(nDefault);\
    __P.Set( NULL, Name, Val, sizeof ( Val ) );}
#define PARM_EXT(Val,Name,nDefault) \
{QParams & __P = QParams::Get();\
    __P.SetDefaultValue(nDefault);\
    __P.Set( model, Name, Val, sizeof ( Val ) );}
//-------------------------- Параметры точек --------------------------------
#define PARM_PNT(Val,Name) \
{QParams & __P = QParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.Set( NULL, Name, Val, sizeof ( Val ) );}
#define PARM_ENUM(Val) \
{QParams & __P = QParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.Set( #Val, Val.ClassName, &Val );}
IN_DLL void Sort ( int kParams, QParams Params[] );

/////////////////////////////////////////////////////////////
enum  EShowTypes
{
  sp_Unknown = -1,
  sp_Info = 1,
  sp_Actions = 2,
  sp_Events = 3,
  sp_Regim = 4,
  sp_Refresh = 5,
};

struct CShowData
{
    char Txt[256];
    IBaseModel * pModel;
    //HWND hWndMain;
    enum eCommand { Нет, ВыделитьТрубу, ВыделитьОбъект, ОкноСоставов, СравнитьСоставы, РасчетДавлений, Открыть_Схему  };
    eCommand NewAction;
    CShowData()
    {
      memset ( this, 0, sizeof ( CShowData ));
      NewAction = Нет;
    }
};

// Уровни доступа в TAB
#define LEV0 0
#define LEV2 2
#define LEV4 4
#define LEV6 6
#define LEV8 8

struct RSU_Obj
{
    CStr ObjName;
    void * pBase;
    const char * Model;
    const char * File;
    const char * Ref;
};

IN_DLL void GetRSUPnt ( const char * Filtr, const char * File, const char * TypeObj,
                      CMem<RSU_Obj, 1024, 1024> * pRSU_Pnt);
IN_DLL void GetObjParams( void * pBase, LPCSTR Model, CMem<QParams, 16, 16> * pParamsP, CMem<QParams, 16, 16> * pParamsW );

#endif // PARAMS_H
