#ifndef DB_H
#define DB_H
#include "CommProc.h"
#include <QSettings>

#undef IN_DLL
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

struct IN_DLL DB
{
  static QSettings * pProp;
//  static BYTE * Mem;
  DB();
  static void Set( const char * Name,  int L, void * Data );
  static bool Get( const char * Name,  int L_max, int & L, void * Data );
  static void Set( const char * Name, const char * Value );
  static void Set( const char * Name, double & Value );
  static void Set( const char * Name, int & Value );
  static void Set( const char * Name, bool & Value );
//
  static const char * _( const char * Name );
  static const char * _( const char * Class, const char * Name );
  static const char * _( const char * Group, const char * Class, const char * Name );
  static const char * GetChar( const char * Name, const char * Def );
  static double GetDbl( const char * Nam, double Def );
  static int GetInt( const char * Name, int Def );
  static bool GetBool( const char * Name, bool Def );
  static void Test( );
  static void TestMem();

};


#endif // DB_H

