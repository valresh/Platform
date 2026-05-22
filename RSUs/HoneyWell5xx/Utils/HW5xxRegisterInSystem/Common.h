#ifndef COMMON_H
#define COMMON_H
#pragma once

#include "BaseType.h"
#include <stdlib.h>

#undef COMMONOBJ_API
#ifdef UTILS_EXPORTS
#define COMMONOBJ_API Q_DECL_EXPORT
#else
#define COMMONOBJ_API Q_DECL_IMPORT
#endif

COMMONOBJ_API EDataTypes  GetTypeID( const char * Name );
COMMONOBJ_API const char* GetClassName( UINT TypeID );
/*template<size_t _S> //если кому-нибудь нужно, то просто убрать комментарии
void GetClassName( int TypeID, char (&ClassName)[_S] )
{
  LPCSTR pName = GetClassName( TypeID );
  if( !pName )
    strcpy_s( ClassName, "" );
  else
    strcpy_s( ClassName, pName );
}*/
enum eTypeSaveRest { Модели_и_РСУ, Модели, РСУ };

COMMONOBJ_API int InitData( CBase* pObj );
COMMONOBJ_API int GetParams( CBase * pObj );
//
COMMONOBJ_API int SaveParamsToFile ( LPCTSTR FilePath );
COMMONOBJ_API int SavePropsToFile( LPCTSTR FilePath );
COMMONOBJ_API int SaveChangedParams( LPCTSTR FilePath );
COMMONOBJ_API int RestoreChangedParams( LPCTSTR FilePath );
COMMONOBJ_API int GetParamsFileInfo ( LPCTSTR FilePath );
typedef bool (*tRestoreObjectParams)( LPCTSTR ObjName );
COMMONOBJ_API int RestoreParamsFromFile( LPCTSTR FilePath, eTypeSaveRest TypeSaveRest,
                                        tRestoreObjectParams pRestoreObjectParams = NULL );
COMMONOBJ_API int RestorePropsFromFile ( LPCTSTR FilePath );
COMMONOBJ_API int SaveStateToFile      ( LPCTSTR FilePath );
COMMONOBJ_API int RestoreStateFromFile ( LPCTSTR FilePath, eTypeSaveRest TypeSaveRest );
COMMONOBJ_API int GetStateFileInfo ( LPCTSTR Path );
typedef bool (*tRestoreObjectState)( LPCTSTR ObjName );
COMMONOBJ_API int RestoreObjectsStateFromFile ( LPCTSTR FilePath, tRestoreObjectState pRestoreObjectState );
COMMONOBJ_API int SaveState( CBase * pObj );
COMMONOBJ_API int RestoreState( CBase * pObj, char * StrName );
//
COMMONOBJ_API BOOL FindSensorValue(const char* szName, double* pValue); // не модельное имя датчика
typedef
  BOOL (*tFindSensInfo)(const char* szName, double* pValue); // не модельное имя датчика
extern COMMONOBJ_API tFindSensInfo pFindSensInfo;
//
//COMMONOBJ_API void WriteAlarms( struct CAlarmEntry& Entry );
COMMONOBJ_API void WriteEvents( struct CEventEntry& Entry );
COMMONOBJ_API void WriteEvents( LPCTSTR szUser, int ID_MESSAGE, LPCTSTR szMessage );
COMMONOBJ_API void WriteEventLog(LPCTSTR szUser, int ID_MESSAGE, LPCTSTR szFormatMessage, ...);
COMMONOBJ_API bool ReadEvent( int Numb, struct CEventEntry & Entry );
//
//COMMONOBJ_API int SortAndFind( const void * d1, const void * d2 );
//COMMONOBJ_API void  PutModel( LPCTSTR szUser, struct SSendToModel& send );
COMMONOBJ_API void* FillVoid( struct SSendToModel& send, struct CShBase* pBase, const char* pszMess );
COMMONOBJ_API int GetEventString( CEventEntry& Event, char* str, int dl );
//
typedef const char* (*FuncReplName)( const char* pszFrom );
COMMONOBJ_API void SetReplName( FuncReplName fnReplName );
//

// #include "./TagType.h"

// COMMONOBJ_API STagType*   FindTagType( char* name );
// COMMONOBJ_API SValueDef*  NameToValue( DWORD eData, const char* name );
// COMMONOBJ_API const char* EnumStr( SValueDef* def, BYTE val );
// COMMONOBJ_API BYTE        EnumVal( SValueDef* def, const char* name );
// COMMONOBJ_API const char* EnumStr( UINT nEnum, BYTE val );
// COMMONOBJ_API BYTE        EnumVal( UINT nEnum, const char* name );
// COMMONOBJ_API SValueDef*  TypeLst( EDataTypes eData );
// COMMONOBJ_API UINT        TypeCnt( EDataTypes eData );
//
// typedef char CharMP[_MAX_PATH];
// #define SizeMP sizeof(CharMP)
// //
// inline void SplitPath( const char *path, char *drive = 0, char *dir = 0, char *fname = 0, char *ext = 0 )
// {
//   int a = drive ? sizeof(CharMP) : 0;
//   int b = dir   ? sizeof(CharMP) : 0;
//   int c = fname ? sizeof(CharMP) : 0;
//   int d = ext   ? sizeof(CharMP) : 0;
//   _splitpath_s( path, drive, a, dir, b, fname, c, ext, d );
// }
// inline errno_t MakesPath( char *path, const char *drive = 0, const char *dir = 0, const char *fname = 0, const char *ext = 0 )
// {
//   return _makepath_s( path, SizeMP, drive, dir, fname, ext );
// }
//
struct SSendToModel
{
  SSendToModel()
  {
    memset( this, 0, sizeof(*this) );
    eType = enumValueUnk;
    eData = id_Unknown;
  };
  SSendToModel(SValueDef* def)
  {
    memset( this, 0, sizeof(*this) );
      if ( def )
      {
        eType = def->eVal;
        strcpy( szValue, def->name );
    }
    eData = id_Unknown;
  };
  char szName [128];// Имя устройства
  char szValue[128];// Имя переменной
  union
  {
    EDataTypes eData;// Тип устройства
    DWORD      dwData;
  };
  EValueType eType;// Тип переменной
  SValueDef def;
  bool bMessage;// Флаг посылки сообщения
  union { double dNew; int nNew; char cNew; bool bNew; float fNew; char sNew[64]; };
  union { double dOld; int nOld; char cOld; bool bOld; float fOld; char sOld[64]; };
  void SetOld( void* Old )
  {
    switch ( eType )
    {
      case enumValueDbl:if ( Old ) dOld = *(double*)Old;break;
      case enumValueInt:if ( Old ) nOld = *(int   *)Old;break;
      case enumValueChr:if ( Old ) cOld = *(char  *)Old;break;
      case enumValueBol:if ( Old ) bOld = *(bool  *)Old;break;
      case enumValueFlt:if ( Old ) fOld = *(float *)Old;break;
      case enumValueStr:if ( Old ) strcpy_s( sOld, (char*)Old);break;
    }
  }
  void SetNew( void* New )
  {
    switch ( eType )
    {
      case enumValueDbl:if ( New ) dNew = *(double*)New;break;
      case enumValueInt:if ( New ) nNew = *(int   *)New;break;
      case enumValueChr:if ( New ) cNew = *(char  *)New;break;
      case enumValueBol:if ( New ) bNew = *(bool  *)New;break;
      case enumValueFlt:if ( New ) fNew = *(float *)New;break;
      case enumValueStr:if ( New ) strcpy_s( sNew, (char*)New );break;
    }
  }

  void SetNewArray( char* Name,int nIndex,void* New )
  {
    strcpy(szValue, Name);
      if ((strcmp(Name,"AI")==0)||(strcmp(Name,"AO")==0))
        eType=enumValueDbl;
      else
        eType=enumValueChr;
    switch ( eType )
      {
        case enumValueDbl:if ( New ) dNew = *(double*)New;break;
        case enumValueInt:if ( New ) nNew = *(int   *)New;break;
        case enumValueChr:if ( New ) cNew = *(char  *)New;break;
        case enumValueBol:if ( New ) bNew = *(bool  *)New;break;
        case enumValueFlt:if ( New ) fNew = *(float *)New;break;
        case enumValueStr:if ( New ) strcpy_s( sNew,(char*)New);break;
      }
    nOld=nIndex;
  }
  void Set( EValueType type, void* Old, void* New )
  {
    eType = type;
    SetOld( Old );
    SetNew( New );
  }
  void Set( void* Old, void* New )
  {
    Set( eType, Old, New );
  }
};
//
#define SIMPLE_PARAMS

#define Strupr(Txt) _strupr_s ( Txt, sizeof ( Txt ) );
#define Strlwr(Txt) _strlwr_s ( Txt, sizeof ( Txt ) );
#define Sprintf(Txt,...) sprintf_s ( Txt, sizeof ( Txt ), __VA_ARGS__ );
#define Strcpy(Txt,From) strcpy_s ( Txt, sizeof ( Txt ), From );
#define Strcat(Txt,From) strcat_s ( Txt, sizeof ( Txt ), From );

void SetMODE ( BYTE & M );
/*
//
typedef UINT (*LPAcyType) ( const char * szName );
extern COMMONOBJ_API LPAcyType g_pAcyType;// Тип тэга по имени
//
typedef const char* (*LPAcyComm)( const char * szName );
extern COMMONOBJ_API LPAcyComm g_pAcyComm;// Комментарий тэга по имени
//
typedef UINT (*LPAcyNumb) ( const char * szName );
extern COMMONOBJ_API LPAcyNumb g_pAcyNumb;// Номер тэга по имени
*/
typedef int    (*tRsuHandleInitData)( struct CBase* pObj );
typedef int    (*tRsuHandleGetParams)( struct CBase * pObj  );
typedef int    (*tRsuHandleSaveState)( struct CBase * pObj );
typedef int    (*tRsuHandleRestoreState)( struct CBase * pObj, char * StrName );

typedef int (*tExtendCommonBehaviours)(tRsuHandleInitData, tRsuHandleGetParams, tRsuHandleSaveState, tRsuHandleRestoreState);
extern COMMONOBJ_API tExtendCommonBehaviours g_pExtendCommonBehaviours;

typedef LPCSTR (*tRsuHandleGetClassName)( UINT TypeID );
typedef void (*tExtendClassInfo)( tRsuHandleGetClassName );
extern COMMONOBJ_API tExtendClassInfo g_pExtendClassInfo;

COMMONOBJ_API bool Create_1_2_Data( const char * File );
COMMONOBJ_API struct IBaseModel * Get_Line_2_Object( struct IBaseModel * pLine_1 );
extern COMMONOBJ_API bool ReadSecondLine;
//
extern COMMONOBJ_API int NumbProject;
extern COMMONOBJ_API bool CreateCommonFiles;
//extern COMMONOBJ_API bool TraceCommunications;
//extern COMMONOBJ_API void TRACER( char * Fmt, ... );

// .........................................................................................
// Далее следует описание сообщений и типов данных для связи нового Инструктора с Сервером.
// Связь осуществляется с помощью сообщения WM_COPYDATA.
// тип сообщения указывается в переменной dwData структуры COPYDATASTRUCT
// .........................................................................................

// (1). VM: 27.05.2021
// Генерация события ЦПУ (Event)
// Тип сообщения (GENERATE_EVENT_ID)
#define GENERATE_EVENT_ID		(DWORD)0xFEFEC0C1
// Структура данных
typedef struct EXTEVENTDATA
{
  char szUser[_MAX_PATH];
  SSendToModel send;
} EXTEVENTDATA, *PEXTEVENTDATA;
// ...
// (2). VM: 22.11.2021
// Передача состояния упражнения

// Флаги состояний
#define EXRCSTATE_STARTED		(DWORD)0x00000001
#define EXRCSTATE_STOPPED		(DWORD)0x00000002
#define EXRCSTATE_TRAINMODE		(DWORD)0x00000004
// Тип сообщения (SET_EXRCSTATE_ID)
#define SET_EXRCSTATE_ID		(DWORD)0xFEFEC0C2

// (3). VM: 22.11.2021
// Передача ссылки на подсказку оператору в режиме обучения
// Тип сообщения (SET_ACTION_TIP_ID)
#define SET_ACTION_TIP_ID		(DWORD)0xFEFEC0C3
// Типы файлов подсказки
enum TIP_FILE_TYPE
{
  TIP_FILE_HTML,
  TIP_FILE_IMAGE,
  TIP_FILE_AUDIO,
  TIP_FILE_VIDEO
};
// Структура данных описания подсказки
typedef struct ACTIONTIPDATA
{
  char szUser[64];
  char szTipFile[_MAX_PATH];
} ACTIONTIPDATA, *PACTIONTIPDATA;
// ...

#endif // COMMON_H
