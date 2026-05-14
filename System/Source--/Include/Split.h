#pragma once
#include "CommProc.h"

template <int max_col, int max_size, int max_str = 256 >
struct CSplit
  {
  FILE * F;
  char Str[max_str];
  char Cols[max_col][max_size];
  int kCol;
  CSplit()
    {
    kCol = 0;
    F = NULL;
    memset(Str,0,sizeof(Str));
    memset(Cols,0,sizeof(Cols));
    }
  ~CSplit()
    {
    Close();
    }
  bool Open( const char * Path, bool SkipFirst = true )
    {
    char Linux_Path[1024];
    strcpy ( Linux_Path, Path );
    char * P = Linux_Path;
    while ( *P )
    {
      if ( *P == '\\' )
        *P = '/';
      P++;
    }
    F = fopen ( Linux_Path, "rt" );
    if ( F )
      {
      if ( SkipFirst )
        fgets ( Str, max_str - 1, F );
      return true;
      }
    return false;
    }
  bool Test( int n, char * txt )
    {
    if ( n >= kCol )
      return false;
    return lstrcmp ( Cols[n], txt ) == 0; 
    }
  bool Next( )
    {
    if ( F )
      {
      memset(Str,0,sizeof(Str));
 //    char ansi[max_str];
      kCol = 0;
      if ( fgets ( Str, max_str - 1, F ) == NULL )
        return false;
//       CharToUTF8( Str, max_str, ansi );
      kCol = Split( NULL );
      return true;
      }
    kCol = 0;
    return false;
    }
  void Close( )
    {
    if ( F )
      fclose ( F );
    F = NULL;
    }
  int Split( const char * in_str )
    {
    if ( in_str )
      strcpy_s ( Str, max_str, in_str );
    int L = lstrlen( Str );
    if ( L > 0 && ( Str[L-1] == 0x0d || Str[L-1] == 0x0a ))
      {
      L--;
      Str[L] = 0;
      }
    int K = 0;
    memset( Cols, 0, max_col * max_size );
    char * Pnext = NULL;
    char * P = Str;
    char * Out = (char*)Cols;
    while ( K < max_col )
      {
      Pnext = strchr ( P, ';' );
      if ( Pnext == NULL )
        {
        if ( *P == 0 )
          return K + 1;
        int L = lstrlen ( P );
        if ( L >= max_size )
          P[max_size-1] = 0;
        lstrcpy ( Out, P );
        return K+1;
        }
      *Pnext = 0;
      int L = lstrlen ( P );
      if ( L >= max_size )
        P[max_size-1] = 0;
      lstrcpy ( Out, P );
      Out += max_size;
      P = Pnext + 1;
      K++;
      }
    return K;
    }
  };

