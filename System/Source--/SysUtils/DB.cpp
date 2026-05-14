#include "DB.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include "Err.h"

QSettings * DB::pProp = NULL;
 DB::DB()
  {
    pProp = new QSettings("Simulator.conf", QSettings::NativeFormat);
//    pProp->clear();
  }

const char * DB::_( const char * Class, const char * Name )
  {
    static char Txt[1024];
    sprintf_s ( Txt, 1024, "%s/%s", Class, Name );
    return Txt;
  }
const char * DB::_( const char * Group, const char * Class, const char * Name )
  {
    static char Txt[1024];
    sprintf_s ( Txt, 1024, "%s/%s/%s", Group, Class, Name );
    return Txt;
  }

  const char * DB::_( const char * Name )
  {
    static char Txt[1024];
    sprintf_s ( Txt, 1024, "%s", Name );
    return Txt;
  }

void DB::Set( const char * Name,  int L, void * Data )
  {
  QByteArray arr ( (char*)Data, L );
  pProp->setValue( Name, arr );
  }

void DB::Set( const char * Name, const char * Value )
  {
  return;
  pProp->setValue( Name, Value );
  }
void DB::Set( const char * Name, double & Value )
  {
  return;
  pProp->setValue( Name, Value );
  }
void DB::Set( const char * Name, int & Value )
  {
  return;
  pProp->setValue( Name, Value );
  }
void DB::Set( const char * Name, bool & Value )
  {
  return;
  pProp->setValue( Name, Value );
  }
  //
bool DB::Get( const char * Name,  int L_max, int & L, void * Data )
  {
  return false;
  QByteArray arr = pProp->value( Name ).toByteArray();
  if ( arr.isEmpty())
      return false;
  L = arr.length();
  if ( L > L_max )
      L = L_max;
  memmove ( Data, arr.data(), L );
  return true;
  }
const char * DB::GetChar( const char * Name, const char * Def )
  {
  return Def;
  static char Txt[1024];
  QVariant V = pProp->value( Name, "9" );
  QString S = pProp->value( Name, Def ).toString();
  if ( S.isEmpty())
    return Def;
  strcpy_s ( Txt, 1024, STR( S ));
  return Txt;
  }
double DB::GetDbl( const char * Name, double Def )
  {
  return Def;
  return pProp->value( Name, Def ).toReal();
  }
int DB::GetInt( const char * Name, int Def )
  {
  return Def;
  return pProp->value( Name, Def ).toInt();
  }
bool DB::GetBool( const char * Name, bool Def )
  {
  return Def;
  return pProp->value( Name, Def ).toBool();
  }
QVariant DB::GetVariant( const char * Name )
  {
  QVariant V;
    return V;//pProp->value( Name );
  }


#if 0
void ass( bool Cond )
  {
  if ( !Cond )
    KKK();
  }
//#define INIT

//BYTE * DB::Mem = NULL;
//#define SISE_DB 10 * 1024 * 1024

#ifdef INIT
BYTE B[SISE_DB];
#endif

 DB::DB()
{
  Char<1024>Path;
  Path.Prt ( "%sDATA/DB.dat", ROOT_PATH );
  int fd = open(Path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR ); // | O_CREAT
#ifdef INIT
  memset ( B, 0, SISE_DB );
  write ( fd, B, SISE_DB );
#endif
  Mem = (BYTE*)mmap(NULL, SISE_DB, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
  close(fd);
#ifdef INIT
  memset ( Mem, 0, SISE_DB );
  *(int*)Mem = 4;
#endif
//  Test( );
}

void DB::Test( )
{
int A[100];
int B[10];
int Res;
#ifdef INIT
  for ( int n = 0; n < 100; n++ )
      A[n] = n;
    for ( int n = 0; n < 10; n++ )
      B[n] = 10-n;
  Res = Set( "1", "12345",  sizeof( A ), A );
  Res = Set( "2", "12345",  sizeof( A ), A );
  Res = Set( "2", "3333",  sizeof( A ), A );
  Res = Set( "2", "444",  sizeof( B ), B );
  Res = Set( "2", "12345",  sizeof( A ), A );
#endif
  //
  int L = 0;
  memset ( A, 0, sizeof( A ));
  Res = Get( "2", "3333",  sizeof( A ), L, A );
  ass( L == sizeof( A ) );
    for ( int n = 0; n < 100; n++ )
    {
      ass(A[n] == n );
    }
  L = 0;
  memset ( A, 0, sizeof( A ));
  Res = Get( "1", "12345",  sizeof( A ), L, A );
  ass( L == sizeof( A ) );
    for ( int n = 0; n < 100; n++ )
    {
      ass(A[n] == n );
    }
  L = 0;
  memset ( A, 0, sizeof( A ));
  Res = Get( "2", "444",  sizeof( B ), L, B );
  ass( L == sizeof( B ) );
    for ( int n = 0; n < 10; n++ )
    {
      ass(B[n] == 10-n );
    }
  L = 0;
  memset ( A, 0, sizeof( A ));
  Res = Get( "2", "12345",  sizeof( A ), L, A );
  ass( L == sizeof( A ) );
    for ( int n = 0; n < 100; n++ )
    {
      ass(A[n] == n );
    }
  KKK();
}

#define KEY 0x7F2E564A

void DB::TestMem()
{
  int Len = *(int*)Mem;
  BYTE * P_data = Mem + 4;
  BYTE * P = P_data;
  while ( P < Mem + Len )
    {
    BYTE * P0 = P; // Начало очередной записи
    ass( *(int*)P == KEY );
    int L = *(int*)(P+4);
    P += L;
    }

}
// KEY <int Полный SIZE><int SIZE Class(c 0) + 4><Class c 0><int SIZE Name(c 0) + 4><Name c 0><int L><Data>
int DB::Set( const char * Class, const char * Name,  int L, void * Data )
  {
  int Len = *(int*)Mem;
  BYTE * P_data = Mem + 4;
  BYTE * P = P_data;
  while ( P < Mem + Len )
    {
      BYTE * P0 = P; // Начало очередной записи
    ass( *(int*)P == KEY );
      BYTE * p = P + 8; // пропуск KEY Size
      if ( strcmp ( (const char * )(p + 4), Class ) == 0 )
        {
          // Класс совпал
          p += *(int*)p;
            if ( strcmp ( (const char * )(p + 4), Name ) == 0 )
            {
              // Имя совпала
              p += *(int*)p;
              int Lf = *(int*)p;
              if ( Lf >= L )
                {
                  memmove ( p + 4, Data, L );
                  *(int*)p = L;
                  TestMem();
                  return 0;
                }
              // Не влезает
              int S = P0 - Mem;
              int dL = L - Lf;
              memmove ( Mem + Len + dL, Mem + Len, Len - S );
              Len += dL;
              *(int*)Mem = Len;
              Set( P0, Class, Name, L, Data );
              TestMem();
              return 1;
          }
          P += *(int*)(P+4);
          continue;
      }
      P += *(int*)(P+4);
      continue;
    }
  // Не найден - Добавка в конец
  int S = Set( P, Class, Name, L, Data );
  *(int*)Mem += S;
  TestMem();
  return -1;
}

// KEY <int Полный SIZE><int SIZE Class(c 0)><Class c 0><int SIZE Name(c 0)><Name c 0><int L><Data>

int DB::Set( BYTE * Addr, const char * Class, const char * Name,  int L, void * Data )
{
  BYTE * P0 = Addr;
  BYTE * P = Addr;
  *(int*)P = KEY;
  P += 4; //KEY
  P += 4; //SIZE
  int lC = strlen( Class ) + 1;
  *(int*)P = lC + 4;
  P += 4;
  strcpy( (char*) P, Class );
  P += lC;
  int lN = strlen( Name ) + 1;
  *(int*)P = lN + 4;
  P += 4;
  strcpy( (char*) P, Name );
  P += lN;
  *(int*)P = L;
  P += 4;
  memcpy( P, Data, L );
  P += L;
  int S = P - P0;
  *(int*)(P0 + 4 ) = S;
  return S;
}

bool DB::Get( const char * Class, const char * Name,  int L_max, int & L, void * Data )
  {
  int Len = *(int*)Mem;
  BYTE * P_data = Mem + 4;
  BYTE * P = P_data;
  while ( P < Mem + Len )
    {
      BYTE * P0 = P; // Начало очередной записи
      ass( *(int*)P == KEY );
      BYTE * p = P + 8;
        if ( strcmp ( (const char*)(p + 4), Class ) == 0 )
        {
          // Класс совпал
          p += *(int*)p;
          if ( strcmp ( (const char*)(p + 4), Name ) == 0 )
            {
              // Имя совпала
              p += *(int*)p;
              int Lf = *(int*)p;
              if ( Lf > L_max )
                return 1;
              p += 4;
              memmove ( Data, p, Lf );
              L = Lf;
              return true;
            }
          P += *(int*)(P+4);
          continue;
        }
      P += *(int*)(P+4);
      continue;
      }
  // Не найден
  return false;
}

int DB::Set( const char * Class, const char * Name, const char * Value )
{
  return Set( Class, Name,  strlen( Value ) + 1, (void*)Value );
}
int DB::Set( const char * Class, const char * Name, double & Value )
{
  return Set( Class, Name,  sizeof( Value ), &Value );
}
int DB::Set( const char * Class, const char * Name, int & Value )
{
  return Set( Class, Name,  sizeof( Value ), &Value );
}
int DB::Set( const char * Class, const char * Name, bool & Value )
{
  return Set( Class, Name,  sizeof( Value ), &Value );
}


const char * DB::GetChar( const char * Class, const char * Name, const char * Def  )
{
  static char Txt[1024];
  int L;
  if ( Get( Class, Name, 1024, L, Txt ))
    {
      Txt[L] = 0;
    return Txt;
    }
  Set( Class, Name,  strlen( Def ) + 1, (void*)Def );
  return Def;
}
double DB::GetDbl( const char * Class, const char * Name, double Def )
{
  double V;
  int L;
  if ( Get( Class, Name, 8, L, &V ))
    {
      ass(L==8);
      return V;
    }
  Set( Class, Name,  8, &Def );
  return Def;
}
int DB::GetInt( const char * Class, const char * Name, int Def )
{
  int V;
  int L;
  if ( Get( Class, Name, 4, L, &V ))
    {
      ass(L==4);
      return V;
    }
  Set( Class, Name,  4, &Def );
  return Def;
}
bool DB::GetBool( const char * Class, const char * Name, bool Def )
{
  bool V;
  int L;
  if ( Get( Class, Name, 1, L, &V ))
    {
      ass(L==1);
      return V;
    }
  Set( Class, Name,  1, &Def );
  return Def;
}
#endif

