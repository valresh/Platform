#pragma once
#include <crossplatform.h>
#include <yISerializer.h>

#ifdef CONNECTY_EXPORTS
#define CONNECTY_API _EXP
#else
#define CONNECTY_API _IMP
#endif

typedef void (*tAlarm) ( struct CY_Base * pObj, int Alarm, bool ON, double * Value, double LimValue );
extern CONNECTY_API tAlarm pAlarm;
//////////////////////////////////////////////////////////////

typedef void (*tSet_Y_Graf_Data)( );
extern CONNECTY_API tSet_Y_Graf_Data pSet_Y_Graf_Data;

typedef void (*tAlarmSignal)( int ID, bool OnOff, int Level, char * BlockName, DWORD ID_CLASS, DWORD ID_PNT_NAME );
extern CONNECTY_API tAlarmSignal pAlarmSignal;

typedef void (*tSendOG)( char * FCS, int NumbOG );
extern CONNECTY_API tSendOG pSendOG;

typedef void (*tSendOGraw)( LPCSTR txt, int color, LPCSTR pszFcs );
extern CONNECTY_API tSendOGraw pSendOGraw;

typedef void (*tDialogue)( LPCSTR pszMsg, LPCSTR txt, int color, LPCSTR pszHeader, LPCSTR pszVariable );
extern CONNECTY_API tDialogue pDialogue;

typedef void (*tSendRQ)( int NumbRQ );
extern CONNECTY_API tSendRQ pSendRQ;

typedef void (*tRecipeLoad)( LPCSTR pszRecipeName, bool bComplete );
extern CONNECTY_API tRecipeLoad pRecipeLoad;

typedef void (*tShowLogic)( char * LCName, bool Action, char * ObjName, char * VarName, char * Value, bool State );
extern CONNECTY_API tShowLogic pShowLogic;

typedef struct CY_Base * (*tFindYObj)( char * ObjName, int domain );
extern CONNECTY_API tFindYObj pFindYObj;

typedef struct CY_Base * (*tFindYObjN)( char * ObjName, int NumbFcs, int domain );// 0 - без учета номера FCS, >0 - точное совпадение, <0 точно не с этого FCS
extern CONNECTY_API tFindYObjN pFindYObjN;

typedef struct CY_Base * (*tFindYObjTyped)( char * ObjName, int domain, int y_type );
extern CONNECTY_API tFindYObjTyped pFindYObjTyped;

typedef struct CY_DI * (*tFindDObj)( char * ObjName, int NumbFcs, int domain );
extern CONNECTY_API tFindDObj pFindDObj;

typedef struct CY_DI * (*tFindDI)( int NumbFCS, int domain, int NumbSW, int Type );
extern CONNECTY_API tFindDI pFindDI;

//typedef bool (*tFindAndShowYBlk)( HWND hWndMain, char * FCS, char * SelectedObject );
//extern CONNECTY_API tFindAndShowYBlk pFindAndShowYBlk;
//
//typedef void (*tShowCalcuCDebugWindow)(HWND hWndParent, LPSTR pszFileName );
//extern CONNECTY_API tShowCalcuCDebugWindow pShowCalcuCDebugWindow;

typedef struct IBaseModel* (*tGetYBaseWrapper)( struct CY_Base *pYobj );
extern CONNECTY_API tGetYBaseWrapper pGetYBaseWrapper;

typedef struct CY_Base* (*tGetYBaseByBaseModel)( struct IBaseModel *pBaseModel);
extern CONNECTY_API tGetYBaseByBaseModel pGetYBaseByBaseModel;

typedef struct CY_Base * (*tFindObjByNumb)( LPCSTR szObjNumb, int NumbFcs, int domain );
extern CONNECTY_API tFindObjByNumb pFindObjByNumb;

//typedef bool (*tCreateLCWnd) ( char * WndName, HWND hWndMain,	struct CY_LC64_E_Base * W );
//extern CONNECTY_API tCreateLCWnd pCreateLCWnd;
//typedef bool (*tCreateST16Wnd) ( char * WndName, HWND hWndMain,	struct CY_ST16 * W );
//extern CONNECTY_API tCreateST16Wnd pCreateST16Wnd;
//typedef bool (*tCreateST16Wnd) ( char * WndName, HWND hWndMain,	struct CY_ST16 * W );
//extern CONNECTY_API tCreateST16Wnd pCreateST16Wnd;
//typedef bool (*tCreateCalcuBaseWnd) ( char * WndName, HWND hWndMain,	struct CY_CALCU_BASE * W, class KCalcRes *pStack, int *pStackPos );
//extern CONNECTY_API tCreateCalcuBaseWnd pCreateCalcuBaseWnd;

typedef void (*tOpenDialSYSObjParams)( void * pModel, class CWnd * pParent );
extern CONNECTY_API tOpenDialSYSObjParams pOpenDialSYSObjParams;

typedef void (*tGetDR) ( int * kDR, struct CY_DR *** DR );
extern CONNECTY_API tGetDR pGetDR;

typedef void (*tExecConnectionTransfer)( int nConnect, double dt, char dirIO );
extern CONNECTY_API tExecConnectionTransfer pExecConnectionTransfer;


typedef void (*tRegisterYWstructs)();
extern CONNECTY_API tRegisterYWstructs pRegisterYWstructs;

//т.к. сохранение состояния структур идет из Y_Main, то отказываюсь от использования g_pExtendCommonBehaviours
//и буду вызывать напрямую из Y_Main
typedef int (*tYokoStructSaveState)( struct CBase* pObj );
extern CONNECTY_API tYokoStructSaveState pYokoStructSaveState;
typedef int (*tYokoStructRestoreState)( struct CBase* pObj, char * StrName );
extern CONNECTY_API tYokoStructRestoreState pYokoStructRestoreState;

extern CONNECTY_API IYokoStateSer* pYokoStateSer;

extern CONNECTY_API int g_CalcStep;//для индикации
