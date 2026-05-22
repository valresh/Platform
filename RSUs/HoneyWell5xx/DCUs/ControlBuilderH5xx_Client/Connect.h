#ifndef CONNECT_H
#define CONNECT_H
#pragma once
//#include <commproc.h>

#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif


typedef void (*tSysMsg)( char * Fmt, ... );
IN_DLL extern tSysMsg pSysMsg;
#define SysMSG if ( pSysMsg ) (*pSysMsg)

typedef void (*tShowGroup)( char * Group, char * ObjName );
IN_DLL extern tShowGroup pShowGroup;

typedef void (*tShowIBaseModelParams)( struct IBaseModel * pModel, class CWnd * pParent );
extern IN_DLL tShowIBaseModelParams pShowIBaseModelParams;

typedef bool (*tGetIBaseModelParams)( struct IBaseModel * pModel, char ** ObjName, char ** Model );
extern IN_DLL tGetIBaseModelParams pGetIBaseModelParams;

typedef int (*tShowAddWindow )( struct IBaseModel * pST, class CWnd * pParent );
extern IN_DLL tShowAddWindow pShowAddWindow;

typedef struct IBaseModel * (*tFindACSModel)( char * ObjName );
extern IN_DLL tFindACSModel pFindACSModel;

typedef bool (*tGetFirstACSObject)( );
extern IN_DLL tGetFirstACSObject pGetFirstACSObject;

typedef bool (*tGetNextACSObject)( DWORD & Type, void ** pData, char Name[256], char Info[256] );
extern IN_DLL tGetNextACSObject pGetNextACSObject;

typedef bool (*tGetVarsList )( char * ProgramName, int & kVarInfo, struct CVariable ** ppVar );
extern IN_DLL tGetVarsList pGetVarsList;
typedef bool (*tGetGlobalVarsList )( char * ProgramName, int & kVarInfo, struct CVariable ** ppVar );
extern IN_DLL tGetGlobalVarsList pGetGlobalVarsList;

typedef bool (*tTestConnection)( void * pData, char Name[256] );
typedef bool (*tGetFirstConnection)( );
typedef bool (*tGetNextConnection)( void ** pData, char Src[256], char Dst[256] );
extern IN_DLL tTestConnection pTestConnection;
extern IN_DLL tGetFirstConnection pGetFirstConnection;
extern IN_DLL tGetNextConnection pGetNextConnection;

typedef bool (*tShowACSObject)( void * pInfo, HWND hMainWnd, char * Name, void * pData );
extern IN_DLL tShowACSObject pShowACSObject;

typedef void (*tRegisterFinderACSobj)(tGetFirstACSObject pGetF, tGetNextACSObject pGetN, tShowACSObject pShowACS);
extern IN_DLL tRegisterFinderACSobj pRegisterFinderACSobj;

typedef bool (*tFindAndShowACSObj) ( HWND hWndMain, char * FullName );
extern IN_DLL tFindAndShowACSObj pFindAndShowACSObj;

typedef bool (*tGetACSObjectValue)( char * Name, void ** ppPV, char * Type );
extern IN_DLL tGetACSObjectValue pGetACSObjectValue;

typedef int (*tGetAllConnections)( char * ObjName, char * PntName, char & IO, char * RSU[16],  char * Desc[16] );
extern IN_DLL tGetAllConnections pGetAllConnections;

typedef void (*tAddNewNonStable)( bool Add, IBaseModel * pModel );
extern IN_DLL tAddNewNonStable pAddNewNonStable;

IN_DLL extern struct CExtern_Pnt * pFirst_Extern_Pnt;
IN_DLL extern struct CExtern_Pnt * pLast_Extern_Pnt;

typedef void (*tContinue)();
extern IN_DLL tContinue pContinue;

typedef void (*tPause)();
extern IN_DLL tPause pPause;

typedef bool (*tTestNoUsed)( const char * Name );
extern IN_DLL tTestNoUsed pTestNoUsed;
typedef bool (*tExcludeFromListNoUsed)( const char * Name );
extern IN_DLL tExcludeFromListNoUsed pExcludeFromListNoUsed;
typedef bool (*tSetConnectionsList)( );
extern IN_DLL tSetConnectionsList pSetConnectionsList;

typedef
  void (*tConvert_1_2 )(bool *aIsComp, double *aCmol, bool bReplaceComp);
extern IN_DLL tConvert_1_2 pConvert_1_2;

typedef
  void (*tAddToVarList )( int Lab, const char * Name, int eVT, void * pVar );
extern IN_DLL tAddToVarList pAddToVarList;

//////////////////////////////////////////////////////////////
IN_DLL extern struct I_NH3_H2O * pNH3_H2O;
IN_DLL extern struct I_NH3_H2O_Appr * pNH3_H2O_Appr;
IN_DLL extern struct I_NH3Helmholtz * pNH3Helmholtz;
IN_DLL extern struct I_AdditionList * pChemAdditions;
IN_DLL extern bool StartInAutoMode;
IN_DLL extern int StepStop;
IN_DLL extern bool UseStepStop;
IN_DLL extern bool UseKlapDefault;
IN_DLL extern bool ShortParamsList;
IN_DLL extern bool UseAlt_CompNames;

IN_DLL extern void * pConnectionsList;


struct CShowInfo   // ABB
{
  enum eActions { NoAction, UpdateParams, TestConnection, GetConnectedObjects };
  eActions Action;
  char * ParamName;
  void * pAddr;
};


IN_DLL int bTest;

struct IN_DLL CUniConnect
{
  static CUniConnect * pFirst;
  CUniConnect * pNextConnect;
  CUniConnect();
  virtual int Service( DWORD Key, const char * Name, DWORD Param, void * pData, void ** pInterface ) = 0;
  static int Get( DWORD Key, const char * Name, DWORD Param, void * pData, void ** pInterface );
};

extern IN_DLL struct IBaseControl * pFirstControl;
extern IN_DLL struct IBaseControl * pLastControl;
extern IN_DLL struct IBaseModel * pControlObject;
extern IN_DLL void * FileTrace;

extern IN_DLL int _Release;

typedef
  void (*tScriptErr) ( char * Fmt, ... );
typedef
  void (*tScriptMsg) ( char * Fmt, ... );
extern  tScriptErr IN_DLL pScriptErr;
extern  tScriptMsg IN_DLL pScriptMsg;

typedef void (*tMessagingUser)( char *pMessage, COLORREF clrBkg,  COLORREF clrTxt );
extern IN_DLL tMessagingUser pMessagingUser;

#endif // CONNECT_H
