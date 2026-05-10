#ifndef DEFECTS_H
#define DEFECTS_H
#pragma once
#define __int64 qint64

#include "CommProc.h"

#undef IN_DLL
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

#define RECOVERY_DEFECT_NAME "Восстановление работоспособности"

void IN_DLL CopyStr( char* szDest, size_t MaxLen, char* szSource );

//////////////////////////////////////////////////////////////////////////////////
struct CDef
{
    DWORD Parent;
    char Name[128];
    char ObjName[128];
    union
    {
        struct IBaseModel * pModel;
        __int64 o1;
    };
    DWORD Flags;
    enum
    {
      Defects =  0x00000001,
      Events =   0x00000002,
      Resource = 0x00000004,
      Params =   0x00000008,
      //
      Defect = 0x00000100,
      Node =   0x00000200,
      //
      Active = 0x00000400,
      //
      No_Log = 0x00000800,
      Was_Select = 0x00001000,
    };
    DWORD Param;// 1-й параметр
    BYTE Fire;
    BYTE Level;
    bool Show;
    DWORD Work;
    int ScriptID;
    int NumbProject;
    union
    {
        void * hItem;
        __int64 o2;
    };
    union
    {
        CDef * pNextDefect;
        __int64 o3;
    };
    void Init()
    {
      memset ( this, 0, sizeof ( CDef ));
    }
    void On()
    {
      Flags |= Active;
    }
    void Off()
    {
      Flags &= ~Active;
    }
    void Off_Log()
    {
      Flags |= No_Log;
    }
};

#undef _Bool
struct CDefParam
{
    union
    {
        double Dbl;	int Int; bool Bool;
    };
    union
    {
        double _Dbl; int _Int; bool _Bool;
    };
    float Min, Max;
    enum EType
    {
      tUnknown = 0,
      tDbl,
      tInt,
      tBool,
    };
    char Name[64];
    EType Type;
    DWORD NextParam;
    DWORD Flags;
    enum
    {
      Change   = 0x01,
      Refresh  = 0x02,
      ReadOnly = 0x04,
      Hide     = 0x08,
    };
    void Init()
    {
      memset ( this, 0, sizeof ( CDefParam ));
    }
    operator double ( ) { return Dbl; };
};

struct CDefHead
{
    DWORD Version;
    DWORD Flags;
    enum
    {
      Created =  0x00000001,
    };
    //
    DWORD PosDefect;
    DWORD kDefects;
    //
    DWORD PosParam;
    DWORD kParams;
    //
    DWORD kFire;
    int NumbProject;
};

IN_DLL CDef * GetDefect( DWORD Addr );
IN_DLL CDef * GetFirstDefect( );
IN_DLL DWORD DefectAddr( void * pAddr );
IN_DLL DWORD NewDefect( );
IN_DLL void PrtDefects( );
IN_DLL CDefParam * NewDefectPram( );
IN_DLL CDefParam * NewDefectPram( DWORD Defect );
IN_DLL CDefParam * GetDefectParam( DWORD Addr );
IN_DLL CDefParam * GetDefectParam( CDef * pDef, const char * ParamName );
IN_DLL void AddDefectPram( CDef * pDef, CDefParam * pDefParam );
IN_DLL bool CreateDefectMem( );
// VM: (01.07.2020) Добавлен необязательный параметр для возможности последующей очистки вирт. памяти отказов
//IN_DLL bool GetDefectMem(_HANDLE *phVarMapping = NULL);
//IN_DLL void ClearDefects(_HANDLE hVarMapping);
// ...
IN_DLL DWORD GetRootDefects( );
IN_DLL DWORD GiveClassRoot( char * Name, DWORD Type = CDef::Defects );
IN_DLL DWORD GetRootEvents( );
IN_DLL DWORD GetRootResource( );
IN_DLL DWORD GetRootParams( );
IN_DLL void DefectsCreated( );
IN_DLL bool FireDefect( CDef * pDef, BYTE Mask );
IN_DLL bool FireDefect( int kDefects, CDef * pDef[] );
IN_DLL CDefHead * GetDefHead();
IN_DLL CDef * GetDefect( const char * ObjName, const char * DefectName );
IN_DLL bool FireDefect( const char * ObjName, const char * DefectName,  CDefParam * pDefParam );
IN_DLL bool FireDefect( const char * ObjName, const char * DefectName,  CDef * pSrcParams = NULL );
IN_DLL bool SetDefectParam( CDef * pDef, const char * ParamName, double & Value );
IN_DLL bool SetDefectParam( CDef * pDef, const char * ParamName, int & Value );
IN_DLL CDef * GiveDefect( const char * ClassName, const char * ObjName, const char * DefectName );
IN_DLL void Defect_Params(IBaseModel* pModel);
extern IN_DLL bool SimDef;

#define ON_DEFECT(Class) \
DWORD Class::ClassRootNode = 0;\
  int Class::OnDefect( struct CDef * pDefect )




#define NO_RAZGERM(Flow,Press) \
{\
    FILE * F;\
    if ( fopen_s ( &F, "C:\\NoSetDefOmega.txt", "at" ) == 0 )\
  {\
      fprintf ( F, "%s;%4.2lf;%4.3lf\n", ObjName, Flow, Press );\
      fclose ( F );\
  }\
}

#define RAZGERM_1(Flow,Press,Omega) \
{\
    FILE * F;\
    if ( fopen_s ( &F, "C:\\SetDefOmega.txt", "at" ) == 0 )\
  {\
      fprintf ( F, "%s;%4.2lf;%4.3lf;%4.3lf\n", ObjName, Flow, Press, Omega );\
      fclose ( F );\
  }\
}

#define RAZGERM_2(Flow,Press,Omega1,Omega2) \
{\
    FILE * F;\
    if ( fopen_s ( &F, "C:\\SetDefOmega.txt", "at" ) == 0 )\
  {\
      fprintf ( F, "%s;%4.2lf;%4.3lf;%4.3lf;%4.3lf\n", ObjName, Flow, Press, Omega1, Omega2 );\
      fclose ( F );\
  }\
}

#define DEF_ROOT(Class) DWORD Class::ClassRootNode = 0;
#endif // DEFECTS_H
