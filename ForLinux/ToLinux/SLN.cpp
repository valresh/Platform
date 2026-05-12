#include "stdafx.h"
//#include "ForLinux.h"

struct CDll
  {
  char Name[64];
  char GUID[128];
  int kDep;
  char DEP[20][128];
  };

CDll Dll[100];
int kDll = 0;
int CurrDll = -1;

int FindGUID ( char * GUID ) // [64]
  {
  int N = -1;
  for ( int n = 0; n < kDll; n++ )
    {
    if ( strcmp ( Dll[n].GUID, GUID ) == 0 )
      {
      return n;
      }
    }
  return -1;
  }

bool FindDll ( char * Name, int & kDep, char * Dep ) // [64]
  {
  int N = -1;
  for ( int n = 0; n < kDll; n++ )
    {
    if ( strcmp ( Dll[n].Name, Name ) == 0 )
      {
      N = n;
      break;
      }
    }
  if ( N < 0 )
    return false;
  CDll & D = Dll[N];
  kDep = D.kDep;
  if ( kDep == 0 )
    return true;
  for ( int n = 0; n < kDep; n++ )
    {
    int nDep = FindGUID ( D.DEP[n] ); // [64]
    if ( nDep < 0 )
      continue;
    strcpy ( Dep, Dll[nDep].Name );
    Dep += 64;
    }
  }

bool ReadSln( char * Dir )
  {
  char Path[1024];
  sprintf_s ( Path, 1024, "%s\\*.sln", Dir );
  CFileFind finder;
  BOOL bWorking = finder.FindFile( Path );
  if ( !bWorking )
    return false;
  bWorking = finder.FindNextFile();
  strcpy ( Path, finder.GetFilePath() );
  finder.Close();
  FILE * F = fopen ( Path, "rt" );
  char Str[1024];
  bool Project = false;
  bool Section = false;
  while ( fgets ( Str, 1024, F ))
    {
    if ( strstr ( Str, "Global"))
      {
      break;
      }
    if ( Section )
      {
      if ( strstr ( Str, "EndProjectSection"))
        {
        Section = false;
        continue;
        }
      char * P = strchr ( Str, '{' );
      P++;
      char * E = strchr ( Str, '}' );
      *E = 0;
      CDll & D = Dll[CurrDll];
      strcpy ( D.DEP[D.kDep++], P );
      continue;
      }
    if ( Project )
      {
      if ( strstr ( Str, "EndProject"))
        {
        Project = false;
        continue;
        }
      if ( strstr ( Str, "ProjectSection"))
        {
        Section = true;
        continue;
        }
      }
    if ( strstr ( Str, "Project"))
      {
      CurrDll = kDll++;
      CDll & D = Dll[CurrDll];
      char * P = strchr ( Str, '=' );
      P = strchr ( P, '\"' );
      P++;
      char * E = strchr ( P, '\"' );
      *E = 0;
      strcpy ( D.Name, P );
      P = strchr ( E+1, '{' );
      P++;
      E = strchr ( P, '}' );
      *E = 0;
      strcpy ( D.GUID, P );
      D.kDep = 0;
      Project = true;
      continue;
      }
    }
  fclose ( F );
  //char Dep[20][64];
  //int kDep = 0;
  //bool Res = FindDll ( "BaseCol", kDep, (char *) Dep ); // [64]
  int i = 0;
  }