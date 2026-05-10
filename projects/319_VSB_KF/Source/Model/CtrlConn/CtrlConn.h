#pragma once


#include "BaseInt.h"
#include "Data.h"
#include "ACS_Def.h"
#include "CommProc.h"
#include "PntConnects.h"
#include "Defects.h"
#include "Connect.h"
#include "MM.h"

struct _W
  {
  double O;
  _W();
  };

struct CSensInfo
  {
  char SensName[48];
  CAnalogD * pA;
  };

struct CErr
  {
  char * File;
  int Line;
  void * F;
  void Head()
    {
    Fprintf ( F, "%s;%d;", File, Line );
    }
  };

#define MAX_EVENTS 512
#define MAX_DEFECTS 1024
#define MAX_RES 4

struct CPatialDefect
  {
  CDef * pDef;
  int BaseN;
  CPatialDefect()
    {
    pDef = NULL;
    BaseN = -1;
    };
  };
#ifdef LINUX
struct ConnInfo
  {
   char Type; //'I' 'O' -jn RSU
   const char * RSU;
   const char * Model;
   const char * Descr;
  };
#endif
class CCtrlConn : public IBaseModel, public _W
	{
//
protected:
	void ReadRsuCsv(  int NumbFile, char * File );
  void ReadConnections_MM( char * File );
  void CreateStruct_MM( char * File );
  void CreateStruct_Rsu( );
  void CreateStruct_Rsu( char * File );
public:
	CCtrlConn( char * ObjName );
	virtual ~CCtrlConn();
// Интерфейс IBaseModel
	int Init( int Regim );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams( char * );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( CDef * pDefect );
  int PreStepT();
///////////////////////////////////////////////////
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
  CErr Err;
  void * F_err;
  void * pScript;
  int nFormula;
  int kErr;
  bool HaveFormula;
  bool ReRead;
  double * KIP[3];
  int kStep;
  bool DiscreteDefects;
  bool NoElectroShort;
  double TimeNoElectroShort;
#ifdef LINUX
  CMem<ConnInfo,256,256>Connections_Info;
#endif
/////////////////////////////////////////////////
  CMem<CConnect,256,256>Connections;
//  CScript VBS;
  /////////////////////////////////////////////////
  CDef * Events[MAX_EVENTS];
  int kEvents;
  CPatialDefect Defects[MAX_DEFECTS];
  int kDefects;
  CDef * Resource[MAX_RES];
  CDefParam * ResParm_T[MAX_RES];
  CDefParam * ResParm_P[MAX_RES];
  /////////////////////////////////////////////////
  void SpecFunc ( CConnect & Conn, CFormula & Formula, struct IMM * X, struct IMM * Res, double & SensValue );
  IMM * GetObjPnt( CErr * pErr, IBaseModel * pModel, char * Point, char * TypeDCS, bool ** pConnection ); 
  IMM * GetObjPnt( CErr * pErr, char * ObjName, char * Point, char * TypeDCS, bool ** pConnection ); 
  IMM * GetConnPnt( CErr * pErr, char * ObjName, char * Point ); 
  IMM * GetHSPnt( CErr * pErr, char * HSName ); 
  IMM * GetPolyHSPnt( CErr * pErr, char * PolyHSName, int Value_1, bool Inverse ); 
  IMM * GetRSUPnt( CErr * pErr, char * RSUPnt, char * Type, char * File ); 
  IMM * GetValuePnt( CErr * pErr, char * ValueName, char * ValueType ); 
  IMM * CreateSensor( CErr * pErr, char * ObjName, char * Point ); 
  IMM * CreateKlapan( CErr * pErr, char * ObjName, char * Point ); 
  IMM * CreateHS( CErr * pErr, char * ObjName, char * Point ); 
  IMM * GetStructPnt( CErr * pErr, char * Object, char * Type, char * Point ); 
  void AddNewPoint( char * Объект, char * ТипТочки, char * ИмяТочки, char * Объект_Модель );
  int AddNewPoint( IBaseModel * pModel, char * ТипТочки, char * ИмяТочки );
  void ConnectObjects( char * ОбъектДитя, char * Создать_Найти, char * ОбъектОтец, char * ТипПривязки );
  bool SimpleFormula( CConnect & Conn, char * Формула );
  void SetSensDefects();
  void SetDefects( struct CAnalogD * pPnt );
  void SetConnDefects( );
  void SetDefectsA( CConnect & Conn );
  void SetDefectsD( CConnect & Conn );
  int SetDefect( struct CDef * pDefect );
  int SetSensDefect( struct CDef * pDefect );
  int SetDefectD(  CConnect & Conn, struct CDef * pDefect );
  int SetDefect(  struct CAnalogD * pPnt, struct CDef * pDefect );
  int SetDefect(  CConnectDefect * pConnDef, struct CDef * pDefect );
  bool FireDefect( CDef * pDef );
  void SetEvents( );
  void ReadParams( );
  //void ReadPID( );
  void ReadSens( );
  void ReadSolid( );
  void Duplex_A( CConnect & Conn );
  void Duplex_D( CConnect & Conn );
  bool Translate ( CConnect & Conn, char * Формула );
//
  void Calc ( double dt );
  static LPCSTR m_szScale;
  bool bIn;
	};

template<size_t STR_LEN, size_t K_COL, size_t K_LEN>
void Split( char (&Str)[STR_LEN], char (&Res)[K_COL][K_LEN] )
  {
  char * P = Str;
  int n = 0;
  for ( n = 0; n < K_COL; n++ )
    {
    memset ( Res[n], 0, _countof(Res[n]) );
    if ( P == NULL )
      continue;
    char * Pn = strchr ( P, ';' );
    if ( Pn )
      {
      *Pn = 0;
      strcpy_s ( Res[n], K_LEN, P );
      P = Pn + 1;
      continue;
      }
    if ( P )
      {
      strcpy_s ( Res[n], K_LEN, P );
      P = NULL;
      }
    }
  }

struct Cstring
  {
  char * pStr;
  Cstring( )
    {
    pStr = NULL;
    }
  Cstring( char * S )
    {
    pStr = S;
    }
  void operator = ( char * S )
    {
    pStr = S;
    }
  operator char * ( )
    {
    return pStr;
    }
  bool operator == ( char * S )
    {
    if ( lstrcmp ( pStr, S ) == 0 )
      return true;
    return false;
    }
  char operator [] ( int N )
    {
    return pStr[N];
    }
  };

enum eDefect { eИсправен = 0, eПоказания_не_изменяются, eЗавышение_показаний, eЗанижение_показаний,
eУход_показаний_на_максимум, eУход_показаний_на_начало_шкалы, eСамопроизвольное_изменение_показаний,
eХаотическое_изменение_показаний,
eСигнализация_HH, eСигнализация_H, eСигнализация_L, eСигнализация_LL,
eСигнализация_IOPp, eСигнализация_IOPm,
eВосстановлен, eПостоянно_0, eПостоянно_1,
  };
