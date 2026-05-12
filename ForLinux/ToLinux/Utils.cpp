#include "stdafx.h"


int CharToUTF8( char * To, int MaxTo, char * From, bool Sign )
  {
    unsigned char * F = (unsigned char *)From;
    int p = 0;
    if ( Sign )
    {
      To[p++] = 0xEF;
      To[p++] = 0xBB;
      To[p++] = 0xBF;
    }
    while ( 1 )
    {
      if ( p + 3 > MaxTo )
        break;
      int s = *F++;
      if ( s == 0 )
        break;
      if ( s < 0x80 )
      {
        To[p++] = s;
        continue;
      }
      if ( s == 0xB9 )
      {
        To[p++] = 0xE2;
        To[p++] = 0x84;
        To[p++] = 0x96;
        continue;
      }
      if ( s < 0x7FF )
      {
        if ( s >= 0xC0 )
          s += 0x410 - 0xC0;
        // требуется 2 символа
        int r2 = 0x80 | ( s & 0x3F );
        s = s >> 6;
        int r1 = 0xC0 | ( s & 0x1F );
        To[p++] = r1;
        To[p++] = r2;
        continue;
      }
      assert( 0 ); // больше 2-х не будем
    }
    To[p] = 0;
    return p;
  }

int Len, Pos;
int ObrUniStr ( unsigned char In[1024], unsigned char Out[1024] )
  {
  int o = 0;
  int End = 0;
  while ( End < 2 && Pos < Len )
    {
    unsigned char s = In[Pos++];
    if ( s != 0 )
      {
      int x = 0;
      unsigned int r = 0;
      r |= s;
      r = r << 8;
      s = In[Pos++];
      r |= s;
      r -= 1040;
      r |= 0xC0;
      Out[o++] = r & 0xFF;
      continue;
      }
    s = In[Pos++];
    if ( s == 10 )
      {
      End++;
      continue;
      }
    if ( s == 13 )
      {
      End++;
      continue;
      }
    Out[o++] = s;
    }
  Out[o] = 0;
  return o;
  }

int ObrStr ( unsigned char In[1024], unsigned char Out[1024] )
  {
  int o = 0;
  int End = 0;
  while ( End < 2 && Pos < Len )
    {
    unsigned char s = In[Pos++];
    if ( s == 10 )
      {
      End++;
      continue;
      }
    if ( s == 13 )
      {
      End++;
      continue;
      }
    Out[o++] = s;
    }
  Out[o] = 0;
  return o;
  }

char * Temp = "D:\\Platform3\\Projects\\319_VSB_KF\\ForLinux\\Debug\\Temp.txt";
bool Convert_To_UTF ( const char * Path )
  {
  HANDLE In = CreateFile ( Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
  if ( In == INVALID_HANDLE_VALUE )
    return false;
  Len = GetFileSize ( In, NULL );
  unsigned char * Str = new unsigned char [Len];
  DWORD Lr;
  ReadFile ( In, Str, Len, &Lr, NULL );
  CloseHandle ( In );
  bool UNICODE = false;
  Pos = 0;
  if ( Str[0] == 0xEF && Str[1] == 0xBB && Str[2] == 0xBF )
    {
    delete Str;
    return true;
    }
  if ( Str[0] == 254 && Str[1] == 255 && Str[2] == 0 )
    {
    Pos = 2;
    UNICODE = true;
    }
  unsigned char Code[1024];
///////////////////////////////////////////////
  FILE * Out = fopen ( Temp, "wt" );
  char Res[2048];
  bool BOM = true;
  unsigned char * W = Str;
  if ( UNICODE )
    {
    while ( Pos < Len )
      {
      ObrUniStr ( Str, Code );
      int L = CharToUTF8( Res, 1024, (char*)Code, BOM );
      BOM = false;
      Res[L] = 0;
      fprintf ( Out, "%s\r", Res );
      }
    }
  else
    {
    while ( Pos < Len )
      {
      ObrStr ( Str, Code );
      int L = CharToUTF8( Res, 1024, (char*)Code, BOM );
      BOM = false;
      Res[L] = 0;
      fprintf ( Out, "%s\r", Res );
      }
    }
  fclose ( Out );
  delete Str;
  MoveFileEx ( Temp, Path, MOVEFILE_REPLACE_EXISTING );
  return true;
  }
