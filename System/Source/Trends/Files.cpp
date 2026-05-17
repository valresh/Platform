#include "CommProc.h"
#include "stdio.h"

#undef fopen

FILE * l_fopen ( const char * filename,
                 const char * modes )
  {
  char Path[1024];
  strcpy ( Path, filename );
  int p = 0;
  while ( Path[p] && p < 1024 )
    {
    if ( Path[p] == '\\' )
      Path[p] = '/';
    p++;
    }
  return fopen ( Path, modes );
  }

void * Fopen ( const char * filename,
                const char * modes )
  {
    char Path[1024];
    strcpy ( Path, filename );
    int p = 0;
    while ( Path[p] && p < 1024 )
    {
      if ( Path[p] == '\\' )
        Path[p] = '/';
      p++;
    }
    return fopen ( Path, modes );
  }

void Fclose ( void * stream )
  {
  fclose ( (FILE*)stream);
  }


#undef fgets
char * l_fgets ( char * s, int n, FILE * stream, bool Convert )
  {
  char str[1024];
  char * r = fgets ( str, 1024, stream );
  if ( r == NULL )
    return r;
  int L = strlen ( str );
  if ( L > 0 )
    str[L-1] = 0; // в Win конец \n в L \r\n
  char * Loc = setlocale( LC_ALL, NULL );
  if ( Convert )
    CharToUTF8( s, n, str );
  else
    strcpy_s ( s, n, str );
  return r;
  }

#undef fgets
char * Fgets ( char * s, int n, void * stream )
  {
    char str[1024];
    char * r = fgets ( str, 1024, (FILE*)stream );
    if ( r == NULL )
      return r;
    int L = strlen ( str );
    if ( L > 0 )
      str[L-1] = 0; // в Win конец \n в L \r\n
    CharToUTF8( s, n, str );
    return r;
  }

#undef ReadFile
BOOL l_ReadFile( QFile & hFile, void * To, int size, void *, void * )
  {
  int Res = hFile.read( (char*) To, size );
  if ( Res >= 0 )
    return 1;
  return 0;
  }

#undef CloseHandle
void l_CloseHandle( QFile & hFile )
  {
  hFile.close();
  }

void ToLinux( char * Path )
  {
  int p = 0;
  while ( *Path )
    {
    if ( *Path == '\\' )
      *Path = '/';
    Path++;
    }
  }

void l_TakeOffReadOnly( const char * Path )
  {
  QFile file ( Path );
  file.setPermissions(QFile::ReadOther | QFile::WriteOther);
  }

int Fprintf ( void * File, const char * format, ...)
  {
  if ( File == NULL)
          return 0;
  va_list arg;
  va_start(arg,format);
  return vfprintf ( (FILE*)File, format, arg );
  }
