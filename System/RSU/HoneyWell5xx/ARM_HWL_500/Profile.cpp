// Для работы с системным реестром
#include "stdafx.h"
#include "ARM.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UINT ReadInt( LPCTSTR key, LPCTSTR entry, int nDefault)
  {
  return theApp.GetProfileInt( key, entry, nDefault );
  }
int ReadInt( LPCTSTR key, LPCTSTR entry, int nDef, int nMin, int nMax )
  {
  int nVal = ReadInt( key, entry, nDef );
  nVal = min( nVal, nMax );
  nVal = max( nVal, nMin );
  return nVal;
  }
UINT SaveInt( LPCTSTR key, LPCTSTR entry, int nValue)
  {
  return theApp.WriteProfileInt( key, entry, nValue );
  }
LPCTSTR ReadString( LPCTSTR key, LPCTSTR entry, LPCTSTR pDefault )
  {
  cross::CString str = theApp.GetProfileString( key, entry, pDefault );
  static char text[512];
  lstrcpy( text, str );
  return text;
  }
BOOL SaveString( LPCTSTR key, LPCTSTR entry, LPCTSTR pValue)
  {
  return theApp.WriteProfileString( key, entry, pValue );
  }
BOOL ReadBinary( LPCTSTR key, LPCTSTR entry, LPBYTE* ppData, UINT* pBytes)
  {
  return theApp.GetProfileBinary( key, entry, ppData, pBytes );
  }
BOOL SaveBinary( LPCTSTR key, LPCTSTR entry, LPBYTE   pData, UINT nBytes)
  {
  return theApp.WriteProfileBinary( key, entry, pData, nBytes );
  }
double ReadDouble( LPCTSTR key, LPCTSTR entry, double nDefault )
  {
  char text[512];
  sprintf_s( text, sizeof(512), "%f", nDefault );
  cross::CString str = theApp.GetProfileString( key, entry, text );
  return atof(str);
  }
double ReadDouble( LPCTSTR key, LPCTSTR entry, double nDef, double nMin, double nMax )
  {
  double nVal = ReadDouble( key, entry, nDef );
  nVal = min( nVal, nMax );
  nVal = max( nVal, nMin );
  return nVal;
  }
BOOL SaveDouble( LPCTSTR key, LPCTSTR entry, double nValue)
  {
  char text[512];
  sprintf_s( text, sizeof(512), "%f", nValue );
  return theApp.WriteProfileString( key, entry, text );
  }
bool ReadBool( LPCTSTR key, LPCTSTR entry, bool nDef )
  {
  return ReadInt( key, entry, nDef ? 1 : 0 ) != 0;
  }
BOOL SaveBool( LPCTSTR key, LPCTSTR entry, bool nValue)
  {
  return SaveInt( key, entry, nValue ? 1 : 0 );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define CURRENT_STATE  _T("Текущее состояние")
UINT ReadInt( LPCTSTR entry, int nDefault )
  {
  return ReadInt( CURRENT_STATE, entry, nDefault );
  }
int ReadInt( LPCTSTR entry, int nDef, int nMin, int nMax )
  {
  return ReadInt( CURRENT_STATE, entry, nDef, nMin, nMax );
  }
BOOL SaveInt( LPCTSTR entry, int nValue )
  {
  return SaveInt( CURRENT_STATE, entry, nValue );
  }
LPCTSTR ReadString( LPCTSTR entry, LPCTSTR pDefault )
  {
  return ReadString( CURRENT_STATE, entry, pDefault );
  }
BOOL SaveString( LPCTSTR entry, LPCTSTR pValue)
  {
  return SaveString( CURRENT_STATE, entry, pValue );
  }
BOOL ReadBinary( LPCTSTR entry, LPBYTE* ppData, UINT* pBytes )
  {
  return ReadBinary( CURRENT_STATE, entry, ppData, pBytes );
  }
BOOL SaveBinary( LPCTSTR entry, LPBYTE  pData , UINT nBytes )
  {
  return SaveBinary( CURRENT_STATE, entry, pData, nBytes );
  }
double ReadDouble( LPCTSTR entry, double nDefault )
  {
  return ReadDouble( CURRENT_STATE, entry, nDefault );
  }
double ReadDouble  ( LPCTSTR entry, double nDef, double nMin, double nMax )
  {
  return ReadDouble( CURRENT_STATE, entry, nDef, nMin, nMax );
  }
BOOL SaveDouble( LPCTSTR entry, double nValue )
  {
  return SaveDouble( CURRENT_STATE, entry, nValue );
  }
bool ReadBool( LPCTSTR entry, bool nDef )
  {
  return ReadBool( CURRENT_STATE, entry, nDef );
  }
BOOL SaveBool( LPCTSTR entry, bool nValue)
  {
  return SaveBool( CURRENT_STATE, entry, nValue );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
