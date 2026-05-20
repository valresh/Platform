#ifndef COMMPROC_H
#define COMMPROC_H
#pragma once

#include <crossplatform.h>
#include "SR.h"
#include "GDI.h"// #include "Err.h"
// #include "math.h"
// #include "BaseType.h"
// #include <stdlib.h>


#undef IN_DLL
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif


#define CLEAR(Var) memset(Var,0,sizeof(Var));
#define NewArr(type,Size) (type*)NewMem( sizeof(type) * (Size) );
IN_DLL void * NewMem( size_t size );
IN_DLL int MemUsed( );

struct CMemStorageBase
{
    virtual void * New ( int size ) = 0;
    virtual void Delete( void * Addr ) = 0;
      //    ASS(0)
};

extern IN_DLL struct CMemStorageBase * pMemStorage;
extern IN_DLL struct CExternalCondition * pExt;
extern IN_DLL struct CSystem * pSys;


struct IN_DLL CModelInfo
	{
	char Name[64];				// имя модели
	char TxtPath[_MAX_PATH];	// рабочий каталог
	char BasePath[_MAX_PATH]; //
	char StatePath[_MAX_PATH];	// каталог для сохранения состояний (в models.ini не описывается)
	char dirScript[_MAX_DIR];	// каталог описаний мнемосхем
	char dirScheme[_MAX_DIR];	// каталог технологических рисунков
	char namScheme[_MAX_FNAME];// имя файла общей технол. схемы (без расширения)
    char dirProject[_MAX_DIR];	// каталог проекта Yokogawa
    char RSUFolder[_MAX_DIR];	// каталог проекта Yokogawa
    CModelInfo(){ memset ( this, 0, sizeof ( CModelInfo )); }
  };
///////////////////////////////////////////////////////////////////


template <class T, int size, int max_count > struct CMem
  {
//private:
public:
  T * Ptr[max_count];
  int kBlk;
public:
  int L;
////////////////////////////////////////
  CMem()
    {
    memset ( Ptr, 0, sizeof ( Ptr ));
    kBlk = 0;
    L = 0;
    }
  void Reset()
  {
	  int nCount = sizeof (Ptr) / sizeof (void*);
	  for (int i=0; i<nCount; i++)
	  {
		  if ( Ptr[i] )
		  {
			  pMemStorage->Delete (Ptr[i]);
			  Ptr[i] = 0;
		  }
	  }
	  L = 0;
	  kBlk = 0;
  }
  void Free()
    {
	//bool bDelBlock = (GetMemPtr() == NULL)
	bool bDelBlock = false;
    for ( int n = 0; n < kBlk; n++ )
	{
		if ( bDelBlock )
			free(Ptr[n]);
		else
			pMemStorage->Delete( Ptr[n] );
	}
    memset ( Ptr, 0, sizeof ( Ptr ));
    kBlk = 0;
    L = 0;
    }
////////////////////////////////////////
  ~CMem()
    {
		Free();
    }
////////////////////////////////////////
  T & operator [] ( int N )
    {
    ASS( N < L )
    int B = N / size;
    int P = N % size;
    while ( B >= kBlk )
      {
      ASS(kBlk < max_count)
      Ptr[kBlk++] = (T*)(pMemStorage->New ( size * sizeof ( T )));
      }
    T * pT = Ptr[B];
    return pT[P];
    }
  ////////////////////////////////////////
  T * Get( int N ) const
    {
    ASS( N < L )
    int B = N / size;
    int P = N % size;
    while ( B >= kBlk )
      {
      ASS(0)
      }
    T * pT = Ptr[B];
    return &pT[P];
    }
////////////////////////////////////////
  T & push_back( )
    {
    int N = L++;
    int B = N / size;
    int P = N % size;
    while ( B >= kBlk )
      {
      ASS(kBlk<max_count)
      Ptr[kBlk++] = (T*)NewMem( size * sizeof ( T ));//(pMemStorage->New ( size * sizeof ( T )));
      }
    T * pT = Ptr[B];
    return pT[P];
    }
  bool Bad ( int N )
    {
    if ( N >= L )
      return true;
    int B = N / size;
    int P = N % size;
    if ( B >= kBlk )
      return true;
    if ( IsBadReadPtr ( Ptr[B], sizeof ( T )))
      return true;
    return false;
    }
//
//  У датчика a)'F3012' ggggg b)'F6655'
  };



typedef
int (*tCompKey)( void * TestKey, void * NodeKey, int Type );

struct IN_DLL CStr0
  {
  char * Str;
  CStr0()
    {
    Str = NULL;
    }
  ~CStr0()
  {
	  if ( Str )
	  {
		  delete [] Str;
		  Str = NULL;
	  }
  }
  void Set( const char * pStr )
    {
    int L = lstrlen( pStr );
    Str = new char[L+1];
    lstrcpy ( Str, pStr );
    }
  void operator = ( const char * pStr )
    {
    Set( pStr );
    }
  bool operator == ( const char * pStr )
    {
    int Res = lstrcmp ( Str, pStr );
    if ( Res == 0 )
      return true;
    return false;
    }
  bool operator != ( const char * pStr )
    {
    int Res = lstrcmp ( Str, pStr );
    if ( Res )
      return true;
    return false;
    }
  operator const char *( )
    {
    return Str;
    }
  };


struct IN_DLL CStr
  {
  const char * Str;
  CStr()
    {
    Str = NULL;
    }
  static void Free();
  static void Reset( void * pMaxAddr );
  void Set( const char * pStr );
  void operator = ( const char * pStr );
  void operator << ( const char * pStr );
  bool operator == ( CStr & Str );
  bool operator != ( CStr & Str );
  bool operator == ( char * pStr );
  bool operator != ( char * pStr );
  operator char *( );
  NEW
  };


template <int size> struct Char
  {
  char Str[size];
  int L;
  int S;
  // Char( _bstr_t & wStr )
  //   {
  //   memset(Str,0,size);
  //   S = size;
  //   L = lstrlenW ( wStr );
  //   ASS( L < size )
  //   if ( L >= size )
  //     L = size - 1;
  //   WideCharToMultiByte( CP_ACP, 0, wStr, -1, Str, L, NULL, FALSE );
  //   }
  Char()
    {
    memset(Str,0,size);
    S = size;
    L = 0;
    }
  Char( const char * pStr )
    {
    S = size;
    L = lstrlen ( pStr );
    ASS(L<size)
    if ( L >= size )
      L = size - 1;
    memset(Str,0,size);
    memcpy (Str,pStr,L);
    }
  Char( CStr & cStr )
    {
    S = size;
    const char * pStr = cStr;
    L = lstrlen ( pStr );
    ASS(L<size)
    if ( L >= size )
      L = size - 1;
    memset(Str,0,size);
    memcpy (Str,pStr,L);
    }
  bool IsEmpty()
    {
    if ( Str[0] == 0 )
      {
      L = 0;
      return true;
      }
    return false;
    }
  int Find( char s )
    {
    char * P = strchr ( Str, s );
    if ( P == NULL )
      return -1;
    return P - Str;
    }
  int Find( const char * s )
    {
    char * P = strstr ( Str, s );
    if ( P == NULL )
      return -1;
    return P - Str;
    }
  char * Get( char s )
    {
    return strchr ( Str, s );
    }
  char * Get( const char * s )
    {
    return strstr ( Str, s );
    }
  void Cut( int Pos )
    {
    ASS( Pos>=0 && Pos < L )
    Str[Pos] = 0;
    }
  void operator = ( const char * pStr )
    {
    S = size;
    L = lstrlen ( pStr );
    ASS(L<size)
    if ( L >= size )
      L = size - 1;
    memset(Str,0,size);
    memcpy (Str,pStr,L);
    }
  void operator += ( const char * pStr )
    {
    S = size;
    int Add = lstrlen ( pStr );
    ASS( L + Add < size )
    if ( L + Add >= size )
      Add = size - 1 - L;
    if ( Add > 0 )
      {
      memcpy ( Str + L, pStr, Add );
      L += Add;
      Str[L] = 0;
      }
    }
  bool operator == ( const char * pStr )
    {
    if ( strcmp ( Str, pStr ) == 0 )
      return true;
    return false;
    }
  bool operator != ( const char * pStr )
    {
    if ( strcmp ( Str, pStr ) != 0 )
      return true;
    return false;
    }
  // void operator = ( const wchar_t * pStr )
  //   {
  //   S = size;
  //   L = lstrlenW ( pStr );
  //   ASS( L < size )
  //   if ( L >= size )
  //     L = size - 1;
  //   memset(Str,0,size);
  //   WideCharToMultiByte( CP_ACP, 0, pStr, -1, Str, L, NULL, FALSE );
  //   }
  // void ConvFromW ( const wchar_t * pStr )
  //   {
  //   S = size;
  //   L = lstrlenW ( pStr );
  //   if ( L >= size )
  //     L = size - 1;
  //   memset(Str,0,size);
  //   WideCharToMultiByte( CP_ACP, 0, pStr, -1, Str, L, NULL, FALSE );
  //   }
  // void ConvToW ( wchar_t * pStr )
  //   {
  //   S = size;
  //   L = lstrlenW ( pStr );
  //   if ( L >= size )
  //     L = size - 1;
  //   MultiByteToWideChar( CP_ACP, 0, Str, -1, pStr, L, NULL, FALSE );
  //   }
  void operator = ( CStr & cStr )
    {
    S = size;
    const char * pStr = cStr;
    L = lstrlen ( pStr );
    ASS( L < size)
    if ( L >= size )
      L = size - 1;
    memset(Str, 0, size);
    memcpy (Str, pStr, L);
    }
  int Prt ( const char * Fmt, ... )
    {
    S = size;
    va_list arg;
    va_start( arg, Fmt );
    L = vsprintf ( Str, Fmt, arg );
    Str[L] = 0;
    return L;
    }
  operator char *( )
    {
    return Str;
    }
  };

struct CMemStorage : public CMemStorageBase
  {
  struct CBlk 
    {
    struct CBlk * pNext;
    int size;
    int free;
    void * Addr;
    };
  CBlk * pFirst;
  CMemStorage()
    {
    pFirst = NULL;
    }
  void * New ( int size )
    {
    CBlk * pB = pFirst;
    CBlk * pLast = NULL;
    int s = sizeof(CBlk);
    int S = size + s;
    while ( pB )
      {
      pLast = pB;
      if ( pB->size == size && pB->free )
        {
        pB->free = 0;
        memset( pB->Addr, 0, S );
        return pB->Addr;
        }
      pB = pB->pNext;
      }
    BYTE * pMem = NewArr(BYTE,S);
    memset( pMem, 0, S );
    pB = (CBlk*)pMem;
    if ( pFirst == NULL )
      pFirst = pB;
    else
      pLast->pNext = pB;
    pB->size = size;
    pB->free = 0;
    void * Addr = pMem + s;
    pB->Addr = Addr;
    return Addr;
    }
  void Delete( void * Addr )
    {
    CBlk * pB = pFirst;
    while ( pB )
      {
      if ( pB->Addr == Addr )
        {
        ASS(pB->free==0)
        pB->free = 1;
        return;
        }
      pB = pB->pNext;
      }
//    ASS(0)
    }
  };

class IN_DLL CSemLock
	{
//	_HANDLE hSem;
	public:
	CSemLock ( char * SemName, DWORD Timeout );
	~CSemLock ( );
	};

struct  IN_DLL CCSVData
  {
  int kCol;
  int Start;
  int Size;
  //
  char ** ppRow; 
  int * SortOrder;
  CStr LoadPath;
  char * Pref;
  CCSVData()
    {
    kCol = 0;
    Start = 0;
    Size = 0;
    ppRow = NULL;
    SortOrder = NULL;
    Pref = "";
    }
  ~CCSVData();
  static void Free();
  int Load ( int _kCol, char * Root, char * File, char * Pref );
  int Load ( char * Pref );
  char ** Head ( );
  bool Sort( );
  char ** Find( char * pName_1 );
  char ** Line( int N );
  char ** SortLine( int N );
  int Add( const char ** NewLine );
  void Reset();
  };

  struct IN_DLL CSortTree
  {
      CSortTree * pNext_Group_Pnt;
      int Balance;
      //
      void * Key;
      CSortTree * pL;
      CSortTree * pR;
      CSortTree( )
      {
        Key = NULL;
        pL = NULL;
        pR = NULL;
        Balance = 0;
        pNext_Group_Pnt = NULL;
      }
      void Init( )
      {
        Key = NULL;
        pL = NULL;
        pR = NULL;
        Balance = 0;
        pNext_Group_Pnt = NULL;
      }
  };

  typedef
    int (*tCompKey)( void * TestKey, void * NodeKey, int Type );

  typedef
    CSortTree * (*tNew)( void * Key, int Type );

  struct IN_DLL CSortTreeInfo
  {
      CSortTree * Root;
      CSortTree * FirstInGroup;
      CSortTree * LastInGroup;
      tCompKey pCompKey;
      tNew pNew;
      int Type;
      CSortTreeInfo( )
      {
        Type = 0;
        Root = NULL;
        pCompKey = NULL;
        pNew = NULL;
        FirstInGroup = NULL;
        LastInGroup = NULL;
      };
      CSortTreeInfo( CSortTree * _Root, tCompKey _pCompKey, tNew _pNew, int _Type )
      {
        Type = _Type;
        Root = _Root;
        pCompKey = _pCompKey;
        pNew = _pNew;
        FirstInGroup = NULL;
        LastInGroup = NULL;
      }
      CSortTree * GetFirst( );
      CSortTree * GetNext( CSortTree * pPrev );
      void Add( CSortTree * pItem );
  };

  IN_DLL CSortTree * Find( CSortTreeInfo * Info, void * Key, bool AddNoFound = true, bool *pbAdded = NULL );
  IN_DLL bool Add( CSortTreeInfo * Info, void * Key, CSortTree * pNewItem );
  IN_DLL CSortTree * GetFirst( CSortTreeInfo * Info );
  IN_DLL CSortTree * GetNext( );

/*
struct IN_DLL CTrendsItem : public CSortTree
  {
  CStr Name;
  char Type;
  void * pVar;
  };

struct CTrendsVar
  {
  int Numb;
  COLORREF Col;
  double Min, Max;
  char Source;
  char Type;
  bool ShowNom;
  bool Show;
  void * pVar;
  float * Data;
  double Nom;
  };

struct IN_DLL CTrends
  {
  CSortTree RootVars;
  CSortTreeInfo Vars;
  CTrendsItem ** pItems;
  float * pValues;
  int kVars;
  int kData;
  _HANDLE hFile;
  DWORD CurrLine;
  bool WasOpen;
  CRITICAL_SECTION  CS;
//
  CTrends();
  ~CTrends();
  void Init( );
  bool Add( const char * Name, char Type, void * pVar );
  int Find( const char * Name );
  bool OpenTrends( );
  void CloseTrends( );
  bool Write( );
  bool GetLine( int Line, float * pValues );
  bool GetGroupVar( int Line, int kVar, int * nVars, float * pVal );
  bool GetGroupVar( int Line, int nVar, CMem<CTrendsVar,16,16> & Vars ); 
  };
*/
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// снимаем атрибут "read only" для файла
// IN_DLL void TakeOffReadOnly(LPCTSTR szFileName);
// bool IN_DLL ClearReadOnly ( char * File );
// IN_DLL bool ClearRO( LPCTSTR Path );
// IN_DLL void StrCPY( char* dst, char* src, int size );
// IN_DLL void Qsort( void * list, size_t nCount, size_t nSize, bool& bInit );
// IN_DLL void * SearchName( const char* name, const void * list, size_t nCount, size_t nSize );
// IN_DLL bool HashData( void * pData, int Len, BYTE Hash[20] );
// IN_DLL bool HashFile( const char * Path, BYTE Hash[20] );
// IN_DLL bool HashData( void * pData, int Len, char Str[41] );
// IN_DLL bool HashFile( const char * Path, char Str[41] );
// IN_DLL void ConvertScript( char * Script, char * Out );
// IN_DLL int CompareScript( char * Text, char * VBText );// Text = Русский, VBText = Псевдо
// //IN_DLL int OpenVisioProp( void ** pData );
// IN_DLL int GetVisioProp(  void * pData, const char * Path, char Decsription[128], char Subject[128] );
// //IN_DLL int CloseVisioProp( void * pData );
// IN_DLL int SetVisioProp( const char * Path, const char Decsription[128], const char Date[128] );
// IN_DLL int GetVisioProp( const char * Path, char Decsription[128], char Date[128] );
//IN_DLL bool GetCmdLineParam( const char * ParmName, char Parm[256] );


// Для чтения файлов по сети
typedef void (*LPFuncNetWork)(const char* pszFileName);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//IN_DLL _HANDLE Load_Dll( const char * DllName, const char *DllPath = NULL );

IN_DLL bool CanRunApp();

// Возвращает имя папки EXE
IN_DLL char* GetExePath();
IN_DLL void SetProjectPath( char* Root );
IN_DLL void SetPaths();
// Возвращает имена папок 
IN_DLL char* GetBasePath(); // ...DCU/
IN_DLL char* GetDirScheme(); // ...Scheme/
IN_DLL char* GetDirScript(); // ...Script/
//
// Возвращает имя файла, в зависимости от префикса
IN_DLL void GetPathScn( char* szPath, const char* pszName, bool bExt = true );
//
// Возвращает имя папки с текстами
IN_DLL char* GetTxtPath();
// Возвращает имя папки с сохраненными состояниями
IN_DLL char* GetSavePath();
// Возвращает имя папки дампов
IN_DLL char* GetDumpPath();
// Возвращает путь с пережёванными файлами
IN_DLL LPCTSTR GetBinPath(LPCTSTR pSubDir = NULL);
IN_DLL void    GetBinFile( char* pszBin, LPCTSTR pszFile, const char* pszExt = NULL );
// Возвращает полный путь файла переменных
IN_DLL char* GetMemoryPath( );
// Возвращает полный путь файла переменных
IN_DLL char* GetVariablesPath( );
// Возвращает имя папки с составами
IN_DLL char* GetCompPath();
IN_DLL char* GetApprPath();
// Возвращает указатель на общую память
IN_DLL BYTE * SetVariables( DWORD Base, DWORD Size, const char * FileName, bool Clear );
// Возвращает полный путь файла параметров
IN_DLL char* GetParamsPath( );
// Возвращает полный путь папки параметров
IN_DLL char* GetParamsDir( );
// Возвращает полный путь файла параметров KlapZadv
IN_DLL char* GetAcyPath();
// Возвращает полный путь к лог-файлу
IN_DLL LPCTSTR GetEventLogFilePath();
// Возвращает путь к директории с файлами трендов
IN_DLL LPCTSTR GetTrendPath();
// Возвращает полный путь к файлу с описанием упражнений
IN_DLL LPCTSTR GetSamplesFilePath();
// возвращает пути к файлам отчетов
IN_DLL LPCTSTR GetReportPath(int ModelID);
IN_DLL LPCTSTR GetTemplatePath(int ModelID);
//
IN_DLL LPCTSTR GetTemplatePath(int ModelID);
// Рекурсивное создание директорий относительно базовой
IN_DLL bool RecursiveFolder( char* szDir );

IN_DLL bool IsCMD( char * CMD ); // -COMMAND
IN_DLL bool GetCMD( char * CMD, char Param[], int Max_Param ); // -COMMAND

//
IN_DLL double r01();
//
//IN_DLL void SetSens ( struct CSensor * pSens, char * Type, double Value );
// IN_DLL int Lstrcmp( LPCTSTR lpString1, LPCTSTR lpString2 );
// IN_DLL DWORD ObrFlag( DWORD & Flag, DWORD Set, DWORD Remove );
// IN_DLL bool IsFlag( DWORD Flag, DWORD Mask );
// IN_DLL bool NoFlag( DWORD Flag, DWORD Mask );

//
bool IN_DLL LinSolv ( int N, double * A, double * B, double * X );
double IN_DLL Exp1 ( double Lam );
// double IN_DLL Scale ( int Type, double To_kg, double To_m3 );
// void IN_DLL UnitC ( int Type, COLORREF& color );
// bool IN_DLL IsNormalMes( int Type );
// void IN_DLL DblToStr(char* lp,double d, size_t dstSize, HWND m_hWnd = NULL );
// void IN_DLL DoubleToStr(char* lp,double d,int nZnak);
// void IN_DLL Integr ( double New, double & Value, double dt, double Tau );
// void IN_DLL OutVar ( double Var, char Txt[256] );
// int IN_DLL ConvTime ( char * Time );
// int IN_DLL Select ( char * Name, char * List[] );
// double IN_DLL InvExp1 ( double Exp1, double Lam0 );
//
//
// IN_DLL BYTE* GetMemPtr();
// IN_DLL void SetMemPtr(BYTE* pNewMem);
// IN_DLL void   DelMem( void*  obj  );
// IN_DLL void SetDebugMode(bool bDebugMode, long nAllocNum);
// IN_DLL bool IsDebugMode(long& nAllocNum);

// IN_DLL void * AddrMem( DWORD Pos );

// IN_DLL void PrtMem( );

// IN_DLL DWORD GetMemPos( );
// IN_DLL void SetMemPos( DWORD P );
// IN_DLL void SetMemLen(DWORD nLen);



inline double LimdX ( double dX, double MaxdX )
	{
	if ( dX >= 0. )
		return MaxdX * dX / ( dX + MaxdX );
	double dx = -dX;
	return -MaxdX * dx / ( dx + MaxdX );
	}


#define IF_OBJ(Name) if ( strstr ( ObjName, Name ))

inline double Max ( double v1, double v2 ) 
  { 
  return ( v1 > v2 )? v1 : v2 ;
  }

inline double Min ( double v1, double v2 ) 
  { 
  return ( v1 > v2 ) ? v1 : v2 ;
  }

void IN_DLL DblOut ( double V, char Txt[256] );

void IN_DLL TxtToClp( char * Txt );
void IN_DLL TxtToClpU( char * Txt );
void IN_DLL TxtFromClp( int MaxLen, char * Txt );

inline double Lev_0_100( double Level, double Lev0, double Lev100 )
  {
  double L = ( Level - Lev0 ) / ( Lev100 - Lev0 );
  if ( L > 1. )
    L = 1.;
  if ( L < 0. )
    L = 0.;
  return L;
  }

inline void Inerc ( double dt, double Tau_sec, double NewValue, double & Value )
  {
  if ( Tau_sec <= 0. )
    {
    Value = NewValue;
    return;
    }
  double Ksi = dt * 3600 / Tau_sec;
  double Old = 1. / ( 1. + Ksi );
  double New = Ksi * Old;
  Value = Old * Value + New * NewValue;
  }

/////////////////////////////////////////////
// IN_DLL void * Fopen ( char * Path, char * Mode );
// IN_DLL void * FopenErr ( char * File = NULL );
// IN_DLL void * FopenErrA ( char * File = NULL );
// IN_DLL bool Fclose ( void * File );
// IN_DLL int Fprintf ( void * File, LPCSTR Fmt, ... );
// IN_DLL char * Fgets (  char *str, int len, void * File  );
// IN_DLL size_t Fwrite( const void *buffer, size_t size,	size_t count, void * File );
// IN_DLL int vFprintf( void * File, char * Fmt, va_list argptr );
// IN_DLL int Fsend ( char * File, char * Fmt, ... );
// IN_DLL int FsendErr ( char * File, char * Fmt, ... );
// IN_DLL int Fsend ( int nStep, char * File, char * Fmt, ... );
// IN_DLL int FsendErr ( int nStep, char * File, char * Fmt, ... );
// IN_DLL bool Fmove ( char * OldFile, char * NewFile );
// IN_DLL char * FormPath( char * Fmt, ... );
// IN_DLL int DevideStr ( char * Str, int MaxParts, char * Parts[] );
// IN_DLL int TestVarLimits( double Var, struct CY_Base * pBase );
// IN_DLL int TestVarLimits( double Var, struct CY_Base * pBase, double & Limit );
// IN_DLL int TestVarLimits( struct CY_Base * pBase, double & ShowVar, int & Mes );
// IN_DLL void DrawSens ( double Sens, int SensMes, HDC hDC, int x0, int y0, int HL );
// IN_DLL char * NodeNoNumb ( char * Node );
// IN_DLL char * ConvertNode ( char * Node );
// IN_DLL bool CompNodeM ( char * Node, char * Etalon );
// IN_DLL int InitMes( );
// IN_DLL int k_Mes( );
// IN_DLL char * MesNames(); // Через ;
// IN_DLL double Scale( int Mes );
// IN_DLL int GetEngUnits( char * Mes );
// IN_DLL char * MesName( int Mes );
// IN_DLL void InitALL( );
// IN_DLL int lstrcmpi_m ( char * Variants, char * Test );

// IN_DLL CModelInfo& ModelInfo();
// IN_DLL const char * ProjectDir();
// IN_DLL void GetBuildList( int ID );

// IN_DLL void Set_ErrBlkInObj ( const char * ObjName );
// IN_DLL void CloseTrends( );
// IN_DLL bool CreatePath( const char * File );

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

#define NEW  void *operator new( size_t size ) { return NewMem ( size ); };\
             void *operator new[]( size_t size ) { return NewMem ( size ); };\
             void operator delete[ ]( void * ){ ASS(0); };


#define LIM(Val,Max) \
	{\
	if ( Val > (Max) ) Val = (Max);\
	if ( Val < -(Max) ) Val = -(Max);\
	}

#define TV(Vent) \
	{\
	if ( Vent < 0. ) Vent = 0.;\
	if ( Vent > 100. ) Vent = 100.;\
	}

#define T0(Val) \
	{\
	if ( Val < 0. ) Val = 0.;\
	}

#define NO_RESTORE return 1;

//++
#define NUMBER_MODEL 0
// Имя трубки для связи с сервером
#define NAME_PIPE_SERVER    "MainPipe"
// Имя трубки для связи с сервером
#define NAME_PIPE_TRENDS    "TrendPipe"
// Имя класса окна сервера
#define NAME_CLASS_SERVER       "Server Network"
#define NAME_CLASS_SERVER_TCP   "Server Rsu TCP"
// Имя класса окна АРМ-а
#define NAME_CLASS_ARM      "АРМ Оператора"
// Имя класса окна АРМ-а
#define NAME_CLASS_ARM2      "АРМ Оператора2"
// Имя окна АРМ-а
#define NAME_WINDOW_ARM     "АРМ Оператора"
//#define _T(x)  x

#define _PI 3.1415926535897932384626433832795
#define _PI_4 0.78539816339744830961566084581988

#define To_Air(T) T = aT_air * T + bT_air * T_air; 


#define MinMax(Value,Min,Max) \
	{\
	if ( Value < Min ) Value = Min;\
	if ( Value > Max ) Value = Max;\
	}

#define TEST_FINITE(Variable,Default) if ( !_finite(Variable)) Variable = (Default);

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


//extern IN_DLL eVersion Version;
extern IN_DLL bool GlobalStatic;
extern IN_DLL int kValueDef;
extern IN_DLL struct SValueDef ** ListValueDef;

extern IN_DLL char PROJECT[32];
extern IN_DLL char EXE_PATH[1024];
extern IN_DLL char SYSTEM_PATH[1024];
extern IN_DLL char ROOT_PATH[1024];
extern IN_DLL char PROJECT_ROOT[1024];
extern IN_DLL char CURR_PROJECT[64];
extern IN_DLL char START_CONFIG[64];
extern IN_DLL char MODELS_PATH[1024];
extern IN_DLL char HELP_PATH[1024];
extern IN_DLL CModelInfo Info;
void IN_DLL SetPaths();

extern IN_DLL int UserLevel;
extern IN_DLL double Press_KIP[4];
extern IN_DLL bool Electro[10];

extern IN_DLL char ReadState[_MAX_PATH];
extern IN_DLL char ReadStateDescr[256];
extern IN_DLL char Read_Params[64];
extern IN_DLL char ReadParamsDescr[256];
extern IN_DLL char WriteState[_MAX_PATH];
extern IN_DLL char WriteStateDescr[256];
extern IN_DLL char WriteParams[64];
extern IN_DLL char WriteParamsDescr[256];
extern IN_DLL char Err_BlkInObj[128];
extern IN_DLL bool CompressParams;

//extern IN_DLL CTrends Trends;

extern double IN_DLL aT_air;
extern double IN_DLL bT_air;
extern double IN_DLL T_air;
extern double IN_DLL T_min;
extern double IN_DLL T_max;
extern double IN_DLL T_std;

extern bool IN_DLL StopModel;
extern int IN_DLL nRunApp;
extern bool IN_DLL ShowAllParams;

// extern IN_DLL struct CExternalCondition * pExt;
// extern IN_DLL struct CSystem * pSys;
// //extern int IN_DLL CountModels;
// extern int IN_DLL CurrentStep;
// //extern IN_DLL struct IBaseModel ** ModelList;
// extern IN_DLL char StdAction[256];
// extern IN_DLL char StdActionParm[256];


// IN_DLL void SyncMsg( char * Msg, ... );
// IN_DLL BOOL SyncCmd( int ID, int L, void * pData );
// IN_DLL void SyncErr( int Code );
// IN_DLL void SyncAction( int Code );
// IN_DLL const char *Transl(char *szWord);
// IN_DLL void LoadDict (char *szDctName, bool bForceLoad = false);
// IN_DLL void ClearDict ();
IN_DLL int Split( const char * Model, const char * Section, const char ** ParamsName, char Params[][256] );
IN_DLL extern struct CExtern_Pnt * pFirst_Extern_Pnt;
IN_DLL extern struct CExtern_Pnt * pLast_Extern_Pnt;
extern IN_DLL struct IBaseControl * pFirstControl;
extern IN_DLL struct IBaseControl * pLastControl;
extern IN_DLL struct IBaseModel * pControlObject;
extern IN_DLL void * FileTrace;

// IN_DLL void ToLinuxPath (char *szPath);
// IN_DLL BOOL IsLocalConnection (bool bUsePipe, const char *szServer);
// IN_DLL BOOL IsNewInstallation ();
// IN_DLL BOOL IsDeveloperMode ();
// IN_DLL int GetCustomerID ();
// IN_DLL BOOL SetCustomerID (int nID);
// IN_DLL BOOL GetCustomerName (int nCustomerID, char *szName);
// IN_DLL int GetCustomerNames (customerMap& map);

//#include "SR.h"
//Memory
//IN_DLL bool InitGlobalMem(const char* szMemPath, int nMemSize);

#define CZ (char*)

extern IN_DLL BOOL g_bInstructor;

struct IN_DLL CTrendsItem
  {
  char Name[128];
  char Type;
  void * pVar;
  NEW
  };

struct CTrendsVar
  {
  int Numb;
  COLORREF Col;
  double Min, Max;
  char Source;
  char Type;
  bool ShowNom;
  bool Show;
  void * pVar;
  float * Data;
  double Nom;
  NEW
  };

#define MAX_VARS 64
#define MAX_STEP 1000000

struct Record
  {
    float Time;
    int Step;
    float Vars[MAX_VARS];
  };

struct IN_DLL TrendsHead  {
  CTrendsItem Items[MAX_VARS];
  int kItems;
  int PosRecords;
  int MaxRecords;
  int AddItem( const char * Name, char Type, void * pVar );
  };

struct IN_DLL CTrends
  {
  TrendsHead * pHead;
//   float * pValues;
//   int kVars;
//   int kData;
// //  _HANDLE hFile;
//   DWORD CurrLine;
  static bool WasOpen;
  static Record * pRecs;
//  CRITICAL_SECTION  CS;
//
  CTrends();
  ~CTrends();
  void Init( );
  bool OpenTrends() { return true; }
  bool Write( );
  bool GetLine( int Line, float * pValues );
  bool GetGroupVar( int Line, int kVar, int * nVars, float * pVal );
  bool GetGroupVar( int Line, int nVar, CMem<CTrendsVar,16,16> & Vars );
  bool ReadData( const char * Path );
  int Add( const char * Name, char Type, void * pVar );
  Record & R( int n );
  NEW
  };

extern IN_DLL CTrends Trends;

IN_DLL int CharToUTF8( char * To, int MaxTo, char * From, bool Sign = false );
IN_DLL int CharToUTF_s( char * To, int MaxTo, char * From, int L_From );
IN_DLL char * UTF( char * Str );
IN_DLL int UtfToChar( unsigned char * To, int MaxTo, char * From, bool Sign = false );
IN_DLL int UtfToChar_s( unsigned char * To, int MaxTo, char * From, int L_From  );
IN_DLL void ToLinux( char * Path );
IN_DLL int ToUTF8( char * Str, int Maxlen );
IN_DLL int Fsend ( const char * File, const char * Fmt, ... );
#define STR(A) A.toStdString().c_str()
#define fopen l_fopen
#define fgets l_fgets
#define TakeOffReadOnly l_TakeOffReadOnly
#define Lstrcmp strcmp
#define lstrcmpi strcmpi

IN_DLL int strcmpi( const char * str1, const char * str2 );

IN_DLL char * l_fgets ( char * s, int n, FILE * stream, bool Convert = true );
IN_DLL FILE * l_fopen ( const char * filename,
                     const char * modes );
IN_DLL char * Fgets ( char * s, int n, void * stream );
IN_DLL void * Fopen ( const char * filename,
                     const char * modes );
IN_DLL void Fclose ( void * stream );
IN_DLL int Fprintf ( void * File, const char * format, ...);

//IN_DLL BOOL ReadFile( QFile * hFile, void * To, int size, void *, void * );

IN_DLL void l_CloseHandle( QFile & hFile );
IN_DLL void l_TakeOffReadOnly( const char * Path );
IN_DLL extern bool UseAlt_CompNames;
IN_DLL int sprintf_s ( char * To, int MaxLen, const char * Fmt, ... );
IN_DLL int _access( const char *path, int mode );
IN_DLL IBaseModel * Load_Obj ( const char * ObjName, const char * Model );
IN_DLL IBaseModel * Load_Object( const char * DllName, const char * ObjName, const char *DllPath = NULL );
IN_DLL void SafeCopy( char * To, int MaxTo, const char* Name );
IN_DLL void Cut( char * To, int MaxTo );
IN_DLL int lstrcmpi_m ( char * Variants, char * Test );
IN_DLL bool CompNodeM ( char * Node, char * Etalon );
IN_DLL int SaveParamsToFile( LPCTSTR FilePath );
IN_DLL int SaveStateToFile( LPCTSTR FilePath );
IN_DLL void * ConnectPoint( const char * Ref, char & Type );
IN_DLL bool OutDI( void * Addr, char Type, char Txt[256]);


IN_DLL int InitMes( );
IN_DLL double Scale( int Mes );
IN_DLL int GetEngUnits( char * Mes );
IN_DLL char * MesNames();
IN_DLL double Use_Mem();

typedef
  BOOL (*tFindSensInfo)(const char* szName, double* pValue); // не модельное имя датчика
extern IN_DLL  tFindSensInfo pFindSensInfo;
IN_DLL extern void * pConnectionsList;
typedef int (*tGetAllConnections)( char * ObjName, char * PntName, char & IO, char * RSU[16],  char * Desc[16] );
extern IN_DLL tGetAllConnections pGetAllConnections;
extern IN_DLL void set_thread_affinity(int core_id);
struct IN_DLL ProcessData
  {
    int64_t User[32], Idle[32], System[32];
    double Mem;
    int64_t S, U;
    double Proc[32], ThisProc;
    double SysProc[3];
    ProcessData()
    {
      CLEAR(User)
      CLEAR(Idle)
      CLEAR(System);
      Mem = 0;
      S = 0;
      U = 0;
      CLEAR(Proc);
      CLEAR(SysProc);
    }
 //   void Set();
 //   void SetSys();
    void SetCPU( int n0, int n1, int n2 );
  };

 enum
  {
      eRM_COUNT = 55,
  };

extern IN_DLL IBaseModel * g_RuntimeModels[eRM_COUNT];
extern IN_DLL int kRuntimeModel;


typedef
   struct IBaseModel* (*tCreateObject)( LPCTSTR ObjName,  LPCTSTR SubType );

IN_DLL extern class MainWindow * pMainWnd;
IN_DLL extern struct CCSVData * pModelDLL;//
IN_DLL extern bool ShortParamsList;
IN_DLL extern bool Stop;
IN_DLL extern bool Pause;
IN_DLL extern int SysSteps;
IN_DLL extern int SysErrors;
IN_DLL void TestMem();
IN_DLL void ACS_Params( char * TabName, IBaseModel * pModel );
IN_DLL void Init_RSU();
IN_DLL void GetRSUFilesList  ( int *kF, const char * pList[16] );
IN_DLL void GetRSUModelsList  ( int *kF, const char * pList[256] );
IN_DLL char * Path( const char * Root, const char * Dir );
IN_DLL char * Summ( const char * s1, const char * s2 );
IN_DLL char * Summ( const char * s1, const char * s2, const char * s3 );

//IN_DLL void * malloc(size_t size);
//IN_DLL void free ( void * ptr );
#include <QClipboard>
#include <QGuiApplication>

IN_DLL void TxtToClp( char * Txt );
IN_DLL void TxtFromClp( int MaxLen, char * Txt );



#include "WinDraw.h"
#endif

#ifdef LINUX
#define CParams QParams
#endif
