#pragma once

#include "BaseInt.h"
#include "math.h"
#include "ACS_Def.h"
#include "RsuX.h"

struct CScriptVar
  {
  enum eType { Dbl, Int, Rsu }; 
  eType Type;
  static CScriptVar * pFirstVar;
  static CScriptVar * pLastVar;
  CScriptVar * pNextVar;
  char * Name;
  char * Ref;
  void * pValue;
  virtual void Connect( );
  int ConnectPoint( );
  int ConnectRSU( );
  CScriptVar();
  void operator = ( char * ParmName ) { Name = ParmName; }
  };

struct CScriptDouble : public CScriptVar
  {
  double Value;
  CScriptDouble( char * Ref );
  CScriptDouble( );
  operator double() { if ( pValue ) return Value = *(double*)pValue; else return 0.; }
  double operator []( int N ) { if ( pValue ) return ((double*)pValue)[N]; else return 0.; }
  CScriptDouble & operator = ( double V ) { if ( pValue ) Value = *(double*)pValue = V; return *this; }
  CScriptDouble & operator = ( CScriptDouble& V ) { if ( pValue ) Value = *(double*)pValue = V; return *this; }
  };

struct CScriptInt : public CScriptVar
  {
  int Value;
  CScriptInt( char * Ref );
  //CScriptInt( int nValue );
  CScriptInt( );
  operator int() { if ( pValue ) return Value = *(int*)pValue; else return 0; }
  CScriptInt & operator = ( int V ) { if ( pValue ) Value = *(int*)pValue = V; return *this; }
  CScriptInt & operator = ( CScriptInt& V ) { if ( pValue ) Value = *(int*)pValue = V; return *this; }
  };
struct CScriptRsu : public CScriptVar
{
	KRsuX Value;
  double Show;
	CScriptRsu( char * Ref );
	//CScriptInt( int nValue );
	CScriptRsu( );
	//operator int() { if ( pValue ) return Value = *(int*)pValue; else return 0; }
	CScriptRsu & operator = ( int V ) { Value = V; Show = V; return *this; }
	CScriptRsu & operator = ( double V ) { Value = V; Show = V; return *this; }
	void Connect( );
};

struct CIO_Double : public CScriptVar
  {
  double Value;
  struct CPnt * pPnt;
  struct CStarter * pStarter;
  CIO_Double( struct CStarter * pStarter, char * Ref );
  operator double();
  CIO_Double & operator = ( double V );
  void Connect( );
  };

struct CIO_Int : public CScriptVar
  {
  int Value;
  struct CPnt * pPnt;
  struct CStarter * pStarter;
  CIO_Int( struct CStarter * pStarter, char * Ref );
  operator int();
  CIO_Int & operator = ( int V );
  void Connect( );
  };

#define MAX_ANALOG 10
struct CScript : public IBaseModel
  {
  //
  public:
    static CScript * pFirstScript;
    static CScript * pLastScript;
    CScript * pNextScript;
    static CScriptVar * pFirstScriptVar;
    static CScriptVar * pLastScriptVar;
    CScriptVar * pFirstVar;
    CScriptVar * pLastVar;
    CScript( char * ObjName );
// Интерфейс IBaseModel
    int Init( int Regim );
    int Step0( );
    int Step1( );
    virtual int StepT( double dt ) = 0;
    virtual int GetParams( char * );
    int ShowParams( DWORD DataType, struct CShowData * pSD );
    int UpdateParam( struct CParams & Param );
    int SaveState ( );
    int RestoreState ( char * StrName );
    int SetData( int TypeData, void * pData );
    NEW
	CAnalog* pAnalog[MAX_ANALOG];
	int nAnalogCount;
/////////////////////////////////////////////////
  };


//#define DBL static CScriptDouble 
//#define INT static CScriptInt 
//#define Analog CScriptDouble 
//#define Diskrete static CScriptInt 

typedef CScriptInt Int;
typedef CScriptDouble Dbl;
typedef CScriptRsu Rsu;
typedef CIO_Int IO_Int;
typedef CIO_Double IO_Dbl;

#define SAVE static

#define И &&
#define ИЛИ ||

#define ОПИСАНИЕ  

#define СКРИПТ(Name) \
struct CLocalScript_##Name : public CScript\
  {\
  CLocalScript_##Name( char * ObjName ) : CScript( ObjName )\
    {\
    };\
  virtual int StepT( double dt );\
  typedef int Int;\
  typedef double Dbl;\
  };\
static CLocalScript_##Name Name( #Name ); \
int CLocalScript_##Name::StepT( double dt )\
  {\
  try {
	
#define КОНЕЦ \
	for(int i = 0; i < nAnalogCount; i++)\
	*pAnalog[i] = *pAnalog[i]->pSrcAddr;\
} catch ( ... ) { return 1; }; return 0; }

#define ПАРАМЕТРЫ(Name) \
void CLocalScript_##Name::GetParams(  IBaseModel * pMain );\
  {\

#define KOHEЦ_ПАРАМЕТРОВ }

#define НАЧАЛО_СКРИПТА(МОДЕЛЬ) \
namespace _##МОДЕЛЬ {\
struct C##МОДЕЛЬ : public CBLK_SCRIPT\
  {\
  C##МОДЕЛЬ::C##МОДЕЛЬ( char * Name ) : CBLK_SCRIPT ( Name )\
  {\
  };\
  virtual int StepT( double dt );\
  typedef int Int;\
  typedef double Dbl;\
  };\
struct CStart_##МОДЕЛЬ : public CStarter\
  {\
  CStart_##МОДЕЛЬ( const char * Name ) : CStarter( Name )\
  {\
  }\
  IBaseModel * CreateObject( LPCTSTR ObjName )\
    {\
    pModel = new C##МОДЕЛЬ((char*)ObjName);\
    return pModel;\
    }\
  };\
static CStart_##МОДЕЛЬ Start_##МОДЕЛЬ( #МОДЕЛЬ );\
static CStarter * _ = &Start_##МОДЕЛЬ;

#define КОНЕЦ_СКРИПТА \
  } catch ( ... ) { return 1; }; return 0; }}

#define РАСЧЕТ(МОДЕЛЬ) \
int C##МОДЕЛЬ::StepT( double dt )\
  {\
  try {
