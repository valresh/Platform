#pragma once


#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#undef IN_DLL
#ifdef DLL_CTRL
#undef IN_DLL
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif

struct IN_DLL COP
  {
  enum eType { None, Load_X, Load_I, Load_D, Load_0, Load_1, Unload, Operation, Ariph, Logic, Func, Symb };
  eType Type;
  union 
    {
    int I;
    double D;
    char OP;
    };
  COP()
    {
    memset ( this, 0, sizeof ( COP ));
    }
  void Set( COP * pOP )
    {
    memcpy ( this, pOP, sizeof ( COP ));
    }
  };


#define MAX_OP 128
#define MAX_ST 32


struct IN_DLL CTrans
  {
//  char * Txt;
  char * Str;
  //
  int Pos;
  int Len;
//  int nLine;
//  char * File;
  struct CErr * Err;
  static void * CPP;
//  static void * Msg;
  int Trace;
  //===========================================
  char Stack[MAX_ST];
  int PosSt; 
  //===========================================
  char TestStack[MAX_ST];
  int PosTestSt; 
  //===========================================
  COP OP[MAX_OP];
  int kOP;
  ////////////////////////////////////////////
  int nFunction[10];
  int kFunction;
  bool CanMin;
  bool CanNumb;
  ////////////////////////////////////////////
  //
  CTrans();
  bool Numb( char & Type, int & iVar, double & dVar );
  bool Numb( );
  bool Identificator( char Name[64] );
  bool Symb( char & S );
  bool Int( int & I );
  bool Hex( DWORD & I );
  bool Drob( double & D );
  void DelBlank( );                           
  void Init( );
  void TestOP( char OP );
  ////
  bool SetCondition();
  bool SetAction();
  void ObrAction( );
  void ArithOP( char OP );
  void LogicOP( char OP );
  void Logical( );
  int IsFunc( char * Name );
  bool Pow10 ( double & M );
  bool Pow100 ( double & M );
  void SetOP( char OP );
  };


#define EQ 2
#define NE 3
#define GE 4
#define LE 5
#define OR 6
#define AND 7

struct CRes
  {
  union {
    int iVal;
    double dVal;
    };
  char Type;
  };

bool IN_DLL WorkOP ( int kOP, COP OP[], CRes & X, CRes & Y );
