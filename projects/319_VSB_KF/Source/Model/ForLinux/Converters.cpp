#include "stdafx.h"
// UTF-8
bool Convert_To_UTF ( char * File )
  {
  HANDLE hFile = CreateFile ( File, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, NULL );
  if ( hFile == INVALID_HANDLE_VALUE )
    {
    AfxMessageBox( "1" );
    return false;
    }
  int S = GetFileSize ( hFile, NULL );
  DWORD Lr;
  ReadFile ( hFile, txt, S, &Lr, NULL );
  CloseHandle( hFile );
  //
  BYTE B0 = txt[0];
  BYTE B1 = txt[1];
  BYTE B2 = txt[2];
  //0xEF, 0xBB, 0xBF.
  if ( B0 == 0xEF && B1 == 0xBB && B2 == 0xBF )
    {
    // UTF-8
    return false;
    }            
  if ( B0 == 0xFF && B1 == 0xFE )
    {
    // Little
    return false;
    }     
  bool Big = false;
  if ( B0 == 0xFE && B1 == 0xFF )
    {
    // Big
    Big = true;
    }
  int Size = 0;
  if ( Big )
    {
    int p = 0;
    for ( int n = 2; n < S; n += 2 )
      {
      int s0 = txt[n];
      int s1 = txt[n+1];
      int W = ( s0 << 8 ) | s1;
      TXT[p++] = W;
      }
    TXT[p] = 0;
    Size = p;
    }
  else
    {
    Size = MultiByteToWideChar( 1251, 0, txt, S, TXT, MAX );
    }
  char * UTF = NULL;
  size_t Res = UnicodeToUTF8( &UTF, TXT, Size );
  hFile = CreateFile ( File, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, NULL );
  if ( hFile == INVALID_HANDLE_VALUE )
    {
    AfxMessageBox( "2" );
    return false;
    }
  DWORD Lw;
  WriteFile( hFile, UTF, Res, &Lw, NULL );
  SetEndOfFile( hFile );
  CloseHandle( hFile );
  return true;
  }

