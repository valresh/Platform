#ifndef DB_H
#define DB_H

#undef IN_DLL
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif


struct IN_DLL DB
{
  DB();
  static bool Changet;
  static void Read ( );
  static void Write ( );
	static struct ObjData * Find ( const char * Class, const char * Name );
  static void Set( const char * Class, const char * Name,  int L, void * Data );
  static bool Get( const char * Class, const char * Name,  int L_data, void * Data );
  static void Set( const char * Class, const char * Name, const char * Value );
  static void Set( const char * Class, const char * Name, double & Value );
  static void Set( const char * Class, const char * Name, int & Value );
  static void Set( const char * Class, const char * Name, bool & Value );
//
  static const char * _( const char * Name );
  static const char * _( const char * Class, const char * Name );
  static const char * _( const char * Group, const char * Class, const char * Name );
  static const char * GetChar( const char * Class, const char * Name, const char * Def );
  static double GetDbl( const char * Class, const char * Name, double Def );
  static int GetInt( const char * Class, const char * Name, int Def );
  static bool GetBool( const char * Class, const char * Name, bool Def );
  static void Test( );

};


#endif // DB_H

