#ifndef DB_H
#define DB_H
#include "CommProc.h"

#undef IN_DLL
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

struct IN_DLL DB
{
  static BYTE * Mem;
  DB();
  static int Set( const char * Class, const char * Name,  int L, void * Data );
  static int Set( BYTE * Addr, const char * Class, const char * Name,  int L, void * Data );
  static bool Get( const char * Class, const char * Name,  int L_max, int & L, void * Data );
  static int Set( const char * Class, const char * Name, const char * Value );
  static int Set( const char * Class, const char * Name, double & Value );
  static int Set( const char * Class, const char * Name, int & Value );
  static int Set( const char * Class, const char * Name, bool & Value );
//
  static const char * Summ( const char * Group, const char * Class );
  static const char * GetChar( const char * Class, const char * Name, const char * Def );
  static double GetDbl( const char * Class, const char * Nam, double Def );
  static int GetInt( const char * Class, const char * Name, int Def );
  static bool GetBool( const char * Class, const char * Name, bool Def );
  static void Test( );
};


#endif // DB_H

