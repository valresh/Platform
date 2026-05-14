#pragma once

#include "stdio.h"
#include <WinFiles.h>

#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif


class IN_DLL CSRFile
{
    bool m_bConvertUtf;
    bool m_bNoWriteOnConvert;
    char m_szFilePath[_MAX_PATH];
//    CSRFile* m_pConvFile;
public:
  char * m_szBuff;
  DWORD m_dwCurr;
  DWORD m_dwSize;
  public:
  QFile * hFile;
  int Bytes;
  enum eInvalidLen { Assert, Ignore, CopyFirst };
  eInvalidLen InvalidLen;
  FILE * F;
//
  CSRFile();
  ~CSRFile();

  bool Create( const char* szPath );
  bool Open( const char* szPath);
  DWORD SetFilePointer( LONG DistanceToMove, DWORD MoveMethod );
  DWORD Skip( LONG Bytes );
  bool WF( LPVOID lpVoid, DWORD dwSize, bool Txt = false );
  bool RF( LPVOID lpVoid, DWORD dwSize, bool Txt = false );
  bool RF_STR(LPVOID lpVoid, DWORD dwSize);
  bool RF_NO_WRITE(LPVOID lpVoid, DWORD dwSize);
  bool WD( LPVOID lpVoid, DWORD dwSize );// Длина + данные
	bool RD( LPVOID  lpVoid, DWORD dwSize );// Длина + данные
  bool WS( char * str );
  bool RS( char * str );
  char * RS( int Len );
  bool WConvertF(LPVOID lpVoid, DWORD dwSize);
  bool WStruct( char * Name, int Len, void * Addr );
  bool WHeadStruct( char * Name, int Len );
  bool RStruct( int Len, void * Addr );
  bool RStruct( int Len, void * Addr, int Skip );
  bool IsData() { return ( m_dwCurr < m_dwSize ); };
  DWORD Close();
  BOOL _WriteFile(
    _HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    void *
    );
//  bool IsConvertUtf() {return m_bConvertUtf;};

  };

#define SAVE_W(Name,Class) \
	 ASS(pSRFile->WStruct(Name, sizeof ( Class ), static_cast<Class*>(this)));

#define S_CLASS(Name,Class) \
	 ASS(pSRFile->WStruct(Name, sizeof ( Class ), static_cast<Class*>(this)));

#define S_STRUCT(Name,Class,pStruct) \
  if ( pStruct != NULL )\
    {\
    ASS(pSRFile->WStruct(Name, sizeof ( Class ), static_cast<Class*>(pStruct)));\
    }

#define S_DATA(Name,Size,pData) \
  if ( pData != NULL )\
    {\
    ASS(pSRFile->WStruct(Name, Size, pData ));\
    }

#define REST_W(Name,Class) \
	if ( lstrcmp ( StrName, Name ) == 0 )\
		{\
        if (std::is_empty_v<Class> == true)\
        {\
            uint8_t val[sizeof ( Class )];\
            ASS(pSRFile->RStruct( sizeof ( Class ), &val));\
            return 0;\
        }\
		ASS(pSRFile->RStruct( sizeof ( Class ), static_cast<Class*>(this)));\
		return 0;\
		}
/**/

#define R_CLASS(Name,Class) \
	if ( lstrcmp ( StrName, Name ) == 0 )\
		{\
        if (std::is_empty_v<Class> == true)\
        {\
            uint8_t val[sizeof ( Class )];\
            ASS(pSRFile->RStruct( sizeof ( Class ), &val));\
            return 0;\
        }\
		ASS(pSRFile->RStruct( sizeof ( Class ), static_cast<Class*>(this)));\
		return 0;\
		}

#define R_STRUCT(Name,Class,pStruct) \
  if ( pStruct != NULL && lstrcmp ( StrName, Name ) == 0 )\
  {\
        if (std::is_empty_v<Class> == true)\
        {\
            uint8_t val[sizeof ( Class )];\
            ASS(pSRFile->RStruct( sizeof ( Class ), &val));\
            return 0;\
        }\
  ASS(pSRFile->RStruct( sizeof ( Class ), static_cast<Class*>(pStruct)));\
  return 0;\
  }

#define R_DATA(Name,Size,pData) \
  if ( pData != NULL && lstrcmp ( StrName, Name ) == 0 )\
  {\
  ASS(pSRFile->RStruct( Size, pData ));\
  return 0;\
  }

#define WT_CLASS(Class) ASS(pSRFile->WF( static_cast<Class*>(this), sizeof ( Class )));
#define RD_CLASS(Class) ASS(pSRFile->RF( static_cast<Class*>(this), sizeof ( Class )));

#define SAVE_BEG(Name,Len) \
	{\
	ASS(pSRFile->WHeadStruct( Name, Len ))\
    DWORD PosBeg = pSRFile->SetFilePointer( 0, std::ios::cur );

#define SAVE_END(Name,Len) \
    DWORD PosEnd = pSRFile->SetFilePointer(0, std::ios::cur); \
    ASS(PosEnd - PosBeg == (Len)); \
	}


#define REST_BEG(Name,Len) \
	if ( lstrcmp ( StrName, Name ) == 0 )\
		{\
		ASS(pSRFile->Bytes == Len);\
		DWORD PosBeg = pSRFile->PrtPos();

#define REST_END(Name,Len) \
		DWORD PosEnd = pSRFile->PrtPos();\
		ASS(PosEnd - PosBeg == Len )\
		return 0;\
		}

IN_DLL extern CSRFile * pSRFile;
#undef  IGNORE
#define IGNORE 	pSRFile->InvalidLen = CSRFile::Ignore;
#define COPY_FIRST 	pSRFile->InvalidLen = CSRFile::CopyFirst;
#define NewArr(type,Size) (type*)NewMem( sizeof(type) * (Size) );
IN_DLL void * NewMem( size_t size );

struct CSaveVarInfo
  {
  void * pAddr;
  int Size;
  char Type;
  PADDING1(3);
  };

#define MAX_DATA_BUF 10240

struct IN_DLL CSaveVarData
  {
  CSaveVarInfo * SaveVarInfo;
  int kVarInfo, MaxVarInfo;
  static BYTE Buf[MAX_DATA_BUF];
//
  CSaveVarData( int Max_Data )
    {
    kVarInfo = 0;
    MaxVarInfo = Max_Data;
    SaveVarInfo = NewArr( CSaveVarInfo, MaxVarInfo ); 
    }
  double * NewDbl( int K )
    {
    ASS(kVarInfo<MaxVarInfo)
    double * pDbl = NewArr( double, K );
    SaveVarInfo[kVarInfo].pAddr = pDbl;
    SaveVarInfo[kVarInfo].Type = 'D';
    SaveVarInfo[kVarInfo++].Size = K * sizeof ( double );
    return pDbl;
    }
  bool AddDbl( int K, double * pDbl )
    {
    ASS(kVarInfo<MaxVarInfo)
    SaveVarInfo[kVarInfo].pAddr = pDbl;
    SaveVarInfo[kVarInfo].Type = 'D';
    SaveVarInfo[kVarInfo++].Size = K * sizeof ( double );
    return true;
    }
  int * NewInt( int K )
    {
    ASS(kVarInfo<MaxVarInfo)
    int * pInt = NewArr( int, K );
    SaveVarInfo[kVarInfo].pAddr = pInt;
    SaveVarInfo[kVarInfo].Type = 'I';
    SaveVarInfo[kVarInfo++].Size = K * sizeof ( int );
    return pInt;
    }
  bool * NewBool( int K )
    {
    ASS(kVarInfo<MaxVarInfo)
    bool * pBool = NewArr( bool, K );
    SaveVarInfo[kVarInfo].pAddr = pBool;
    SaveVarInfo[kVarInfo].Type = 'B';
    SaveVarInfo[kVarInfo++].Size = K * sizeof ( bool );
    return pBool;
    }
  void Save ( char * Name )
    {
    int L = 0;
    for ( int n = 0; n < kVarInfo; n++ )
      {
      int S = SaveVarInfo[n].Size;
      if ( L + S + 5 >= MAX_DATA_BUF )
        {
        ASS(0)
        break;
        }
      memmove ( Buf + L, &SaveVarInfo[n].Type, 1 ); L++;
      memmove ( Buf + L, &SaveVarInfo[n].Size, 4 ); L += 4;
      memmove ( Buf + L, SaveVarInfo[n].pAddr, S ); L += S;
      }
    ASS(pSRFile->WStruct( Name, L, Buf ));
    }
  int Rest( )
    {
    int L = 0;
    for ( int n = 0; n < kVarInfo; n++ )
      {
      int S = SaveVarInfo[n].Size;
      if ( L + S + 5 >= MAX_DATA_BUF )
        {
        ASS(0)
        break;
        }
      L += S + 5;
      }
    bool Res = pSRFile->RStruct( L, Buf );
    if ( !Res && pSRFile->InvalidLen != pSRFile->CopyFirst )
      return -1;
    L = 0;
    for ( int n = 0; n < kVarInfo; n++ )
      {
      int S = SaveVarInfo[n].Size;
      if ( L + S + 5 >= MAX_DATA_BUF )
        {
        ASS(0)
        break;
        }
      char Type;
      int Size;
      memmove ( &Type, Buf + L, 1 ); L++;
      memmove ( &Size, Buf + L, 4 ); L += 4;
      if ( SaveVarInfo[n].Type == Type && S == Size )
        {
        memmove ( SaveVarInfo[n].pAddr, Buf + L, Size ); L += Size;
        continue;
        }
//
      if ( SaveVarInfo[n].Type != Type )
        {
        L += Size;
        continue;
        }
      if ( S < Size )
        {
        memmove ( SaveVarInfo[n].pAddr, Buf + L, S ); L += Size;
        continue;
        }
      //
      memmove ( SaveVarInfo[n].pAddr, Buf + L, Size ); L += Size;
      }
    return 0;
    }
  };
