#pragma once
#include <DataTypes_.h>
#include <rsuErr.h>
#include <Y_Data.h>
#include "ObjConnection.h"
#include <map>
#include <string>
#include <vector>


#ifdef Y_MAINLOGIC_EXPORTS
#define Y_MAINLOGIC_API _EXP
#else
#define Y_MAINLOGIC_API _IMP
#endif

struct Y_MAINLOGIC_API _W_main
{
  double O;
  _W_main();
};

#pragma warning( push )
#pragma warning( disable : 4251 )

class Y_MAINLOGIC_API CY_MainLogic : public _W_main, IConnectionsImpl
{
#ifdef _WIN64
  enum
  {
    MAX_YOBJ =  45000,
    MAX_DOBJ = 300000,
    MAX_YCONN = 70000,
    MAX_DR =    10000,
  };
#else
  enum
  {
    MAX_YOBJ =  25000,
    MAX_DOBJ = 300000,
    MAX_YCONN = 70000,
    MAX_DR =    10000,
  };
#endif

  enum EProjectKeys
  {
    eKeyNULL =            0,
    kKeyVersion =         0x9abc6def,
    eKeyDrCount =         0x11111111,
    eKeyDrNext =          0x22222222,
    eKeyDrEnd =           0x33333333,
    eKeyObjCount =        0x55555555,
    eKeyObjNext =         0x66666666,
    eKeyObjEnd =          0x77777777,
    eKeyDiscretCount =    0x88888888,
    eKeyDiscretNext =     0x99999999,
    eKeyDiscretEnd =      0xAAAAAAAA,
    eKeyConnectionCount = 0xBBBBBBBB,
    eKeyConnectionEnd =   0xFFFFFFFF,
  };

  static const DWORD s_CurVersion;
	//
protected:
  void TransferYConnectFlag( CObjConnection * pC );
public:
	CY_MainLogic( LPCSTR ObjName );
	virtual ~CY_MainLogic();
	virtual int InitL();
  virtual int Step0L();
  virtual int Step1L();
  virtual int StepTL(double dt);
//IConnectionsImpl
  virtual void ConnectionExec( int firstC, int untilC, double dt, char dirIO );
///////////////////////////////////////////////////
	NEWY
/////////////////////////////////////////////////
private:
  bool CreateAndCheckSystemVer();
protected:
  static CY_Base * pY_Obj[MAX_YOBJ];
  static int kY_Obj;
  static struct CY_DI * pD_Obj[MAX_DOBJ];
  static struct CY_DI ** m_ppDigNumbTypeSorted;//% отсортированные по номерам, FCS и типу
  static int kD_Obj;
  static CObjConnection Connection[MAX_YCONN];
  static int kConnection;
  static CY_DR * DR[MAX_DR];
  static int kDR;
  static bool InitParams;
public:
//
  static CY_DI * FindDI( int NumbFCS, int domain, int NumbSW, int Type );
  static bool FindAndShowBlk ( HWND hWndMain, char * FCS, char * SelectedObject );
  static CY_DI *  GetDObj ( char * Name, int NumbFCS, int domain, bool bSameNumbFCS = true/*должен совпадать номер*/ );
  static CY_Base * GetObj ( char * Name, int NumbFCS, int domain, bool bSameNumbFCS = true/*должен совпадать номер*/ );
	static CY_Base * FindYObj ( char * ObjName, int kObjFind = 0 );
	static CY_DI * FindDObj ( char * ObjName, int kObjFind = 0 );
  static CY_DR * FindDR ( char * FCS );
  static int AddYObj( CY_Base * pObj );
  static bool AddDObj( CY_Base * pObj );
  static void InitParm();
  static CY_Base * FindYObjN( char * ObjName, int NumbFcs, int domain );//существует вызов из одноименной глобальной функции
  static CY_Base * FindYObj( char * ObjName, bool bSameFCS, int NumbFcs, int domain );
  static CY_Base * FindYObjTyped ( char * ObjName, int domain, int y_type );//существует вызов из одноименной глобальной функции
  static CY_Base * FindYObjTyped( char * ObjName, bool bSameFCS, int NumbFcs, int domain, int y_type );
  static CY_Base * FindObjByNumb( LPCSTR szObjNumb, int NumbFcs, int domain );
  static CY_DI * FindDObjOld( char * ObjName, int NumbFcs, int domain );//существует вызов из одноименной глобальной функции
  static CY_Base * FindDObj( char * ObjName, bool bSameFCS, int NumbFcs, int domain );
  static void GetDR( int * kDR, struct CY_DR *** DR );//существует вызов из одноименной глобальной функции
  static void ShowCalcuCDebugWindow( HWND hWndMain, LPSTR pszName );
  static void ExecConnectionTransfer( int nConnect, double dt, char dirIO );
protected:
  void SetCsv( LPSTR pszPath );
  LPCSTR GetCsvPrefix();
  void InitPrevConnectionsSources();
private:
  int GetProjectFile( const std::filesystem::path & pszFile, std::vector<BYTE> &readBuf );
protected:
  char m_szObjName[32];
private:
  double m_timeHolder;

  enum e_RestResult
  {
    err_WrongKey,
    err_CantOpen,
    err_WrongVer,
    err_CommonErr,
    err_DatVerGreaterSrcVer,
    err_Ok
  };
  void SaveAll();
  e_RestResult RestAll();
  bool Test_Data_Y();
  void Delete_Data_Y();

  std::filesystem::path m_szCsvFile;
  bool m_bCreateYokoData;

  char m_szDirPrj[_MAX_PATH];

  static bool s_bFcsMultiDomain;
  //static bool s_bScsMultiDomain;
  char m_szFCS[16];
  int m_NumbFCS;
  int m_domain;
  char m_szDR[20];
  typedef std::map<std::string, std::string> tNamedString_;
  tNamedString_ mNumberOfTags;
  void SaveAllTo(std::fstream& hFile );
  std::fstream& RestAllFrom( LPCSTR pszFile );
protected:
  CCentumSystem* m_pCentumSystem;
protected:
  virtual CY_Base* CreateYobj( char * Name, char * Model, int number, int domain );
  virtual CY_DI* CreateDobj( char * Name, int Y_Type, int NumbFCS, int domain, int numb );
  virtual void OnYobjFullNameReady( CY_Base* pObj );
private:
  CY_Base* SetModel( char * Y, CY_DI::ePointType eptModelType, char IO, char (&Model)[256], char (&Point)[256], char (&cMes)[64], int NumbFCS, int domain );
  void ObjInit();
  bool ReadProject();
  bool ReadFcsRef( LPCSTR pszRootFcs );
  void ReadObjParams ( char * EDF, CY_Base * pB, bool SaveConnections );
  void ObrConnections();
  void SetConnection( double dt, CObjConnection * pC );
  int GetConnections( CY_Base * pB, char * pConn, bool SaveConnections );

  bool ConnectVar2Signal( void *pVar, char TypeVar, CSignal &Sig, USHORT varSize );
  bool ConnectSignal2Var( void **pVar, char &TypeVar, CSignal &Sig );

  void ChackConnection( CObjConnection * pC );

  void GetEdf( char * Root );
  void ObrEdf( char * File );
  void ObrDiskrete( char * File );
  void ObrTunning( char * File );
  bool FindAllFiles( int Type, char * Dir );
  bool ReadDiskrete( int Type, char * File );

  void CreateRQs( int domain );
  void CreateLeftWBs();
  void TestForExistWB( LPSTR pWBName, int NumbFCS, int domain, bool bRefOtherFcs, LPCSTR pszExistComment = NULL );

  void ReadBCVO( LPCSTR pszDir, LPCSTR pszName );

  bool ReadSCS_O( const char * File, const char *pszScsName );
  void HandleTAGD( SYokogawaID *dat, BYTE *pData, CY_DR *pDR, BYTE *pTagEdfIntegration );
  void HandleSLWR( SYokogawaID *dat, BYTE *pData, CY_DR *pDR );
  void ManualCreateSystemSWs( LPCSTR pszSCS );
  void AddSystemSW( int NumbSCS, int domain, int Numb, LPCSTR pszComment = NULL );

  int FindIn( char * ObjRef, int onNumbFCS, int nFrom = 0 );
  int FindInSkipThisFCS( char * ObjRef, int NumbFCSskip, int nFrom = 0 );
  int FindOut( char * ObjRef, int onNumbFCS, int nFrom = 0 );
  int FindOutSkipThisFCS( char * ObjRef, int NumbFCSskip, int nFrom = 0 );

  CY_Base * FormCsvRecord( FILE *F, CY_DI *pY, int &bNeedStationNumb, int n, bool bOutIfEmpty, LPCSTR pszObj, LPCSTR pExComment, int &line );
  void GetYokoDatPath( LPSTR pszPath, DWORD _S, LPCSTR pszExt );
  template<DWORD _S> void GetYokoDatPath( char (&szPath)[_S], LPCSTR pszExt = "dat" )
  {
    return GetYokoDatPath( szPath, _S, pszExt );
  }
};

#pragma warning( pop )

template<size_t _S>
void CorrectComment( char (&text)[_S] )
{
  for( char *p = text; *p; ++p )
  {
    switch( *p )
    {
    case ';':
      *p = '|';
      continue;
    case '\r':
    case '\n':
      *p = ' ';
    default:
      continue;
    case '\\':      
      break;
    }
    strcpy_s( p, _S - (p-text), p+1 );
  }
}
