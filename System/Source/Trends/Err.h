#pragma once
#undef left
#undef right
#include <QDebug>
#include <float.h>
//#include <eh.h>
#include <stdio.h>
#include <QObject>
#include <crossplatform.h>


#undef OBJ
#define OBJ(_ObjName) (!lstrcmp(ObjName, _ObjName))

#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

extern class MainWindow * pMainWnd;

//#undef BREAK
#define BREAK Q_ASSERT(false);


#define AbortApp() if ( CErrInfo::pAbort )(*CErrInfo::pAbort)(); else exit(1);
// ...

void IN_DLL SysAssert( char * File, int Line, int* pNumbAssert );
void IN_DLL MsgErr ( void *rezerv, LPCSTR Fmt, ... );
void IN_DLL SysMsgErr ( LPCSTR Fmt, ... );
void IN_DLL LogMsg( char * Fmt, ... );
void IN_DLL ModelMsg ( struct IBaseModel * pModel, LPCSTR Fmt, ... );
void IN_DLL InitErr( MainWindow * pMainWnd );
void IN_DLL KKK();
#define Sprintf(Txt,...) sprintf_s ( Txt, sizeof ( Txt ), __VA_ARGS__ );


typedef void (*tSysMsg)( const char * Fmt, ... );
IN_DLL extern tSysMsg pSysMsg;
#define SysMSG if ( pSysMsg ) (*pSysMsg)

#undef FINITE
#define FINITE(V) ASS(std::isfinite(V));
#undef ASS
#define ASS(V) { static int NumbAssert = 0;if(!(V)) SysAssert((char*)__FILE__,__LINE__,&NumbAssert); }

#undef ASSD
#ifdef _DEBUG
#define ASSD(V) { static int NumbAssert = 0;if(!(V)) SysAssert(__FILE_NAME__,__LINE__,&NumbAssert); }
#else
#define ASSD(V) { if(!(V)) {} }
#endif

struct IN_DLL PosErrInFile
  {
    const char * File;
    int Line;
    PosErrInFile( )
      {
      File = "";
      Line = 0;
      }
  };


struct IN_DLL C_BP
    {
    static C_BP * pFirst;
    static C_BP * pLast;
    C_BP * pNext;
    DWORD ID;
    const char * Name;
    char * File;
    int Line;
    bool Break;
 //
    C_BP();
    void SetID( DWORD & ClassID );
    };


#undef SET_BP

#define SET_BP \
	{\
	static C_BP T;\
	if ( ClassID == 0 || T.ID == 0 )\
		T.SetID( ClassID );\
  if ( T.Name == NULL )\
	  {\
		T.Name = __FUNCTION__;\
		T.File = __FILE__;\
		T.Line = __LINE__;\
		}\
  if ( bp && T.Break ) BREAK;\
	}
#define  BreakPoint bp

struct SendMsg // : public QObject
    {
//    Q_OBJECT;
    public:
      SendMsg();
      void Init();
      void SysOutTxt( const char * Txt );
      void SysOutMsg( const char * Title, const char * Txt, DWORD flags,
                       DWORD def,int * Res );
      signals:
    };

extern IN_DLL SendMsg Sender;
extern IN_DLL PosErrInFile PosErr;
#define THROW { PosErr.File = __FILE_NAME__; PosErr.Line = __LINE__; throw "Критическая ошибка"; }

//using MsgPtr = void (mainwindow::*)(const char * Txt);
