#include "stdafx.h"

bool ReadSln( char * Dir );
bool Convert_To_UTF ( const char * Path );

void Find_pro ( char * File )
  {
  if ( !ReadSln( File ))
    return;
  char Path[1024];
  sprintf_s ( Path, 1024, "%s\\*.*", File );
  CFileFind finder;
  BOOL bWorking = finder.FindFile( Path );
  int State = 0;
  while ( bWorking )
    {
    bWorking = finder.FindNextFile();
    if ( finder.IsDots())
      continue;
    if ( strstr ( File, "ForLinux"))
      continue;
    if ( finder.IsDirectory())
      {
      char Path[1024];
      strcpy ( Path, finder.GetFilePath() );
      char Name[1024];
      strcpy ( Name, finder.GetFileName() );
      //if ( Create_Pro ( State, Path, Name ))
      //  {
      //  char P[1024];
      //  sprintf ( P, "%s\\%s.pro", Path, Name );
      //  Convert_To_UTF ( P, false );
      //  }
//      Find ( Path );
      continue;
      }
    continue;
    char File[512];
    strcpy ( File, finder.GetFileName() );
    bool OK = false;
    char * P = strchr ( File, '.' );
    //if ( strcmp ( P, ".cpp") == 0 )
    //  OK = true;
    //if ( strcmp ( P, ".h") == 0 )
    //  OK = true;
    //if ( strcmp ( P, ".hpp") == 0 )
    //  OK = true;
    if ( strstr ( File, "stdafx.h" ))
      OK = true;
    if ( !OK )
      continue;
    //char Path[1024];
    //strcpy ( Path, finder.GetFilePath() );
    //if ( Convert_stdafx ( Path ))
    //  {
    //  TRACE(_T("%s\n"), (LPCTSTR)finder.GetFileName());
    //  }
    } 
  State = 2;
//  Create_Pro ( State, "", "" );
  finder.Close();
  }

void Find_stdafx ( char * File )
  {
  char Path[1024];
  sprintf_s ( Path, 1024, "%s\\*.*", File );
  CFileFind finder;
  BOOL bWorking = finder.FindFile( Path );
  int State = 0;
  while ( bWorking )
    {
    bWorking = finder.FindNextFile();
    if ( finder.IsDots())
      continue;
    if ( strstr ( File, "ForLinux"))
      continue;
    if ( finder.IsDirectory())
      {
      //char Path[1024];
      //strcpy ( Path, finder.GetFilePath() );
      //char Name[1024];
      //strcpy ( Name, finder.GetFileName() );
      //if ( Create_Pro ( State, Path, Name ))
      //  {
      //  TRACE(_T("%s\n"), (LPCTSTR)finder.GetFileName());
      //  }
      //      Find ( Path );
      continue;
      }
    continue;
    char File[512];
    strcpy ( File, finder.GetFileName() );
    bool OK = false;
    char * P = strchr ( File, '.' );
    //if ( strcmp ( P, ".cpp") == 0 )
    //  OK = true;
    //if ( strcmp ( P, ".h") == 0 )
    //  OK = true;
    //if ( strcmp ( P, ".hpp") == 0 )
    //  OK = true;
    if ( strstr ( File, "stdafx.h" ))
      OK = true;
    if ( !OK )
      continue;
    char Path[1024];
    strcpy ( Path, finder.GetFilePath() );
    //if ( Convert_stdafx ( Path ))
    //  {
    //  TRACE(_T("%s\n"), (LPCTSTR)finder.GetFileName());
    //  }
    } 
  State = 2;
//  Create_Pro ( State, "", "" );
  finder.Close();
  }

void Find_UTF ( char * File )
  {
  char Path[1024];
  sprintf_s ( Path, 1024, "%s\\*.*", File );
  CFileFind finder;
  BOOL bWorking = finder.FindFile( Path );
  while ( bWorking )
    {
    bWorking = finder.FindNextFile();
    if ( finder.IsDots())
      continue;
    if ( strstr ( File, "ForLinux"))
      continue;
    if ( finder.IsDirectory())
      {
      char Path[1024];
      strcpy ( Path, finder.GetFilePath() );
      Find_UTF ( Path );
      continue;
      }
    if ( strstr ( File, "stdafx" ))
      continue;
    char File[512];
    strcpy ( File, finder.GetFileName() );
    bool OK = false;
    char * P = strchr ( File, '.' );
    if ( P == NULL )
      continue;
    if ( strcmp ( P, ".cpp") == 0 )
      OK = true;
    if ( strcmp ( P, ".h") == 0 )
      OK = true;
    if ( strcmp ( P, ".hpp") == 0 )
      OK = true;
    if ( !OK )
      continue;
    char Path[1024];
    strcpy ( Path, finder.GetFilePath() );
    if ( !Convert_To_UTF ( Path ))
      {
      TRACE(_T("%s\n"), (LPCTSTR)finder.GetFileName());
      }
    } 
  finder.Close();
  }

void Find_vcproj ( char * Dir, char * Ph )
  {
  char Path[1024];
  sprintf_s ( Path, 1024, "%s\\*.*", Ph );
  CFileFind finder;
  BOOL bWorking = finder.FindFile( Path );
  int State = 0;
  while ( bWorking )
    {
    bWorking = finder.FindNextFile();
    if ( finder.IsDots())
      continue;
    if ( strstr ( Ph, "ForLinux"))
      continue;
    if ( finder.IsDirectory())
      {
      char Path[1024];
      strcpy ( Path, finder.GetFilePath() );
      char Name[1024];
      strcpy ( Name, finder.GetFileName() );
      Find_vcproj ( Name, Path );
      continue;
      }
    char File[512];
    strcpy ( File, finder.GetFileName() );
    bool OK = false;
    char * P = strchr ( File, '.' );
    if ( P && strcmp ( P, ".pro") == 0 )
      {
      char From[1024];
      strcpy ( From, finder.GetFilePath() );
      char To[1024];
      sprintf_s ( To, 1024, "D:\\Platform_3.0\\Projects\\Platform_LW\\projects\\319_VSB_KF\\Source\\Model\\%s\\%s.pro", Dir, Dir );
      BOOL Res = CopyFile( From, To, TRUE );
      int i = 0;
      }
    } 
  finder.Close();
  }
