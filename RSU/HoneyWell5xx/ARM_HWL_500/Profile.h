// Для работы с системным реестром
#ifndef PROFILE_H
#define PROFILE_H
//


#include <crosswindows.h>

UINT    ReadInt   ( LPCTSTR entry, int nDefault = 0);
int     ReadInt   ( LPCTSTR entry, int nDef, int nMin, int nMax );
BOOL    SaveInt   ( LPCTSTR entry, int nValue);
LPCTSTR ReadString( LPCTSTR entry, LPCTSTR pDefault = NULL);
BOOL    SaveString( LPCTSTR entry, LPCTSTR pValue);
BOOL    ReadBinary( LPCTSTR entry, LPBYTE* ppData, UINT* pBytes);
BOOL    SaveBinary( LPCTSTR entry, LPBYTE   pData, UINT nBytes);
double  ReadDouble( LPCTSTR entry, double nDefault = 0);
double  ReadDouble( LPCTSTR entry, double nDef, double nMin, double nMax );
BOOL    SaveDouble( LPCTSTR entry, double nValue);
bool    ReadBool  ( LPCTSTR entry, bool nDef );
BOOL    SaveBool  ( LPCTSTR entry, bool nValue);
//
UINT    ReadInt   ( LPCTSTR key, LPCTSTR entry, int nDefault = 0);
int     ReadInt   ( LPCTSTR key, LPCTSTR entry, int nDef, int nMin, int nMax );
UINT    SaveInt   ( LPCTSTR key, LPCTSTR entry, int nValue);
LPCTSTR ReadString( LPCTSTR key, LPCTSTR entry, LPCTSTR pDefault );
BOOL    SaveString( LPCTSTR key, LPCTSTR entry, LPCTSTR pValue);
BOOL    ReadBinary( LPCTSTR key, LPCTSTR entry, LPBYTE* ppData, UINT* pBytes);
BOOL    SaveBinary( LPCTSTR key, LPCTSTR entry, LPBYTE   pData, UINT nBytes);
double  ReadDouble( LPCTSTR key, LPCTSTR entry, double nDefault = 0);
double  ReadDouble( LPCTSTR key, LPCTSTR entry, double nDef, double nMin, double nMax );
BOOL    SaveDouble( LPCTSTR key, LPCTSTR entry, double nValue);
bool    ReadBool  ( LPCTSTR key, LPCTSTR entry, bool nDef );
BOOL    SaveBool  ( LPCTSTR key, LPCTSTR entry, bool nValue);
//
inline bool AS( bool b )
  {
//if ( !::IsMnemoStyle(MNEMO_COMMENT) )
//Sergej  ASSERT ( b );
  return (!b );
  }
//
inline bool As( UINT n )
  {
  //MIHAIL_HWL куча warning была, когда комментируешь ASSERT в stdafx.h
  //if ( !::IsMnemoStyle(MNEMO_COMMENT) )
  ASSERT ( n != -1 );
  return (n == -1);
  }
//
inline bool As( HXML n )
  {
  //MIHAIL_HWL куча warning была, когда комментируешь ASSERT в stdafx.h
  //if ( !::IsMnemoStyle(MNEMO_COMMENT) )
  ASSERT ( n != INVALID_HXML );
  return (n == INVALID_HXML);
  }
//
#undef  _static
#define _static static
//
#define ASSUNO { _static bool a = true; if ( a ) { a = false; ASSERT(0);} }

#endif
