#pragma once

#include "BaseInt.h"
#include "Connect.h"
#include "Param.h"

#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

struct IN_DLL IBaseControl
  {
  //
  IBaseModel * pObject;
  char * ControlName;
  IBaseControl * pNextControl;
  bool NewEvent;
  bool On, Use;
  //
  virtual void Test( ) = 0;
  virtual int GetMsg( char Msg[256] ) = 0; // 1 - сообщение содержит информацию о событии, 0 - сообщение о переходе в норму, -1 - сообщения нет 
  virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual void SaveState( ){;};
  virtual int RestoreState( char * StrName ){ return 1; };
  IBaseControl( char * _ControlName, struct IBaseModel * _pObject );
  };

struct IN_DLL CLevelControl_W
  {
  int State;
  };

struct IN_DLL CLevelControl : public IBaseControl, public CLevelControl_W
  {
  double * pLev;
  double Min, Max;
  int OldState;
//
  CLevelControl( char * _ControlName, struct IBaseModel * _pObject, double * _pLev ); 
  virtual void Test( );
  virtual int GetMsg( char Msg[256] );
  virtual void GetParams( IBaseModel * pMain );
  virtual void SaveState( );
  virtual int RestoreState( char * StrName );
  };

struct IN_DLL COmegaLevel_W
  {
  double MV;
  double LevelZ;
  double FlowOut;
  double Omega;
  double Lev;
//
  double En1, En;
  int TypeReg;
  };

struct IN_DLL COmegaLevel : public IBaseControl, public COmegaLevel_W
  {
  double * pLev;
  double * pControl;
  char CtrlRef[64];
  double LevZ;
  double LevMin;
  double P, Tau;
  double RegMax;
//////////////////////////////////////////////////////////////////////////
  double FlowIn; // Извне
  double dFlow;
  double OmegaMax;
  double OmegaMin;
  //
  void Control( double dt );
  void UpdateParams();
  double * FindRef();
  virtual void Test( ) { ; };
  virtual int GetMsg( char Msg[256] ) { return -1; };
  virtual void GetParams( IBaseModel * pMain );
  virtual void GetParams2( IBaseModel * pMain );
  virtual void SaveState( );
  virtual int RestoreState( char * StrName );
  COmegaLevel( char * _ControlName, struct IBaseModel * _pObject, double * _pLev );
  };


#define MAX_PNT 16
struct IN_DLL CStableControl : public IBaseControl
  {
  double * pVar;
  bool Start; 
  double Pnt[MAX_PNT];
  int kPnt;
  int kNonStable;
  int kStable;
  int State;
  double Value;
  static bool Work;
  static char Object[256];
  static double ProcNonStable;
  static double AbsNonStable;
  static int LevelStable;
//
  CStableControl( char * _ControlName, struct IBaseModel * _pObject, double * _pLev );
  virtual void Test( );
  virtual int GetMsg( char Msg[256] );
  void GetParams( IBaseModel * pMain );
  };


struct IN_DLL CDiscreteControl : public IBaseControl
  {
  bool * pTest;
  bool Norma;
  int State;
  //
  CDiscreteControl( char * _ControlName, struct IBaseModel * _pObject, bool * _pTest, bool _Norma = true ); 
  virtual void Test( );
  virtual int GetMsg( char Msg[256] );
  virtual void GetParams( IBaseModel * pMain );
  };

struct IN_DLL CAnalogControl : public IBaseControl
  {
  double * pVal;
  double * pNominal, * pDelta;
  int State;
  //
  CAnalogControl( char * _ControlName, struct IBaseModel * _pObject, double * _pVal, double * _pNom, double * _pDelta ); 
  virtual void Test( );
  virtual int GetMsg( char Msg[256] );
  virtual void GetParams( IBaseModel * pMain );
  };

struct CTraceControl
  {
  double Val;
  CTraceControl( )
    {
    Val = 0.;
    }
  void Trace ( char * File, int Line, char * Func, char * ObjName, double * pVar, char * Desc, ... ) 
    {
    // if ( FileTrace == NULL )
    //   return;
    // va_list arg;
    // va_start(arg,Desc);
    // Fprintf( FileTrace, "%10.5lf;%10.5lf;", *pVar, *pVar - Val, ObjName );
    // vFprintf( FileTrace, Desc, arg );
    // Fprintf( FileTrace, ";%s;%s;%d;%s;\n", ObjName, File, Line, Func);
    // Val = *pVar;
    };
  };

#undef TRACE
#define TRACE(Object,Var,Desc,...) \
  {\
  static CTraceControl T;\
  if ( strstr ( ObjName, Object ))\
    T.Trace( __FILE__, __LINE__, __FUNCTION__, ObjName, &Var, Desc, __VA_ARGS__ );\
  }

#undef TRACE_T
#define TRACE_T(T,Object,Var,Desc,...) \
  {\
  if ( strstr ( ObjName, Object ))\
    T.Trace( __FILE__, __LINE__, __FUNCTION__, ObjName, &Var, Desc, __VA_ARGS__ );\
  }

struct IN_DLL CShowMesValue : public IShowParam
  {
  double Value;
  int Mes, Mes_0;
  int * pMes;
  char * MesList;
  char * MesName; 
  IBaseModel * pMain;
  CShowMesValue();
  virtual void GetParams( char * ParmName, bool ShowList );
  };

struct IN_DLL CShowP : public CShowMesValue
  {
  double * pPabs;
  CShowP();
  void Init( IBaseModel * pMain, double * pPabs );
  virtual double Show_Dbl ( struct CParams * Param );
  virtual void Show ( struct CParams * Param ) {};
  //  virtual void Set( void * pData ); // &P(и)
  };


struct IN_DLL CShowF : public CShowMesValue
  {
  struct CFlow * pFlow;
  class CSEps * pState;
  char Sign;
//
  CShowF();
  void Init( IBaseModel * pMain, CFlow * pFlow );
  virtual double Show_Dbl ( struct CParams * Param );
  virtual void Show ( struct CParams * Param ) {};
  NEW
  };

struct IN_DLL CCtrlP
  {
  double * pPabs;
  static bool Msg;
//
  CCtrlP( double * pPabs );
	void Test( IBaseModel * pModel );
  };
