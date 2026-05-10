#include "stdafx.h"
#include <rsuPaths.h>
#include <macros/AutoCloser.h>
#include <io.h>

LPCSTR _GetSystemPath()
{
  static char _SYSTEM_PATH[_MAX_PATH*4] = { 0 };
  if( !_SYSTEM_PATH[0] )
  {
    strcpy_s( _SYSTEM_PATH, rsuGetExecutionPath() );
    _strupr_s( _SYSTEM_PATH );
    int L = lstrlen ( _SYSTEM_PATH );
    int K = 0;
    while ( K < 3 )
    {
      if ( _SYSTEM_PATH[L] == '\\' )
        K++;
      _SYSTEM_PATH[L--] = 0;
    }
    strcat_s( _SYSTEM_PATH, "\\SYSTEM\\" );
  }
  return _SYSTEM_PATH;
}

LPCSTR _GetAllProjectsPath()
{
  static char _ALL_PROJECTS_PATH[_MAX_PATH*4] = { 0 };
  if( !_ALL_PROJECTS_PATH[0] )
  {
    strcpy_s( _ALL_PROJECTS_PATH, _GetSystemPath() );
    _strupr_s( _ALL_PROJECTS_PATH );
    int L = lstrlen ( _ALL_PROJECTS_PATH );
    int K = 0;
    while( K < 2 )
    {
      if ( _ALL_PROJECTS_PATH[L] == '\\' )
        K++;
      _ALL_PROJECTS_PATH[L--] = 0;
    }
    _ALL_PROJECTS_PATH[L+1] = '\\';
    strcat_s( _ALL_PROJECTS_PATH, "Projects\\" );
  }
  return _ALL_PROJECTS_PATH;
}

template<size_t _S>
bool _Prop( LPCSTR pszPropCsv, char * First, char * Second, char (&Prop)[_S] )
{
  Prop[0] = 0;
  FILE * F = NULL;
  errno_t res = fopen_s( &F, pszPropCsv, "rt" );
  if( res )
    return false;
  KAutoCloser<FILE*> file( F, fclose );
  if ( F == NULL )
    return false;
  char Line[1024];
  // 
  bool OK = false;
  char * Sect = Line;
  while( fgets ( Line, _countof(Line)-1, F ))
  {
      Line[strcspn(Line, "\r\n")] = 0;
    char * P = strstr ( Line, First );
    if ( P == NULL )
      continue;
    P = strstr ( P, Second );
    if ( P == NULL )
      break;
    P += lstrlen ( Second );
    char * E = strchr ( P, ';' );
    if ( E == NULL )
      break;
    *E = 0;
    strcpy_s( Prop, P );
    OK = true;
    break;
  }
  return OK;
}

static char PROJECT_ROOT[_MAX_PATH*4] = { 0 };
void rsuSetProjectPath( LPCSTR pszPrjRootDir )
{
  if( 0!=_access( pszPrjRootDir, 0) )
    return;
  _makepath_s( PROJECT_ROOT, NULL, pszPrjRootDir, NULL, NULL );
}

LPCSTR rsuGetProjectPath()
{
  if( !PROJECT_ROOT[0] )
  {
    char Path[_MAX_PATH*4];
    sprintf_s( Path, "%sPROP\\Properties.csv", _GetSystemPath() );
    char P[512];
    if( !_Prop( Path, "Корневая папка", "ROOT#", P ) )
      return PROJECT_ROOT;
    sprintf_s( PROJECT_ROOT, "%s%s\\", _GetAllProjectsPath(), P );
  }
  return PROJECT_ROOT;
}

LPCSTR rsuGetMemoryPath()
{
  static char MEMORY_PATH[_MAX_PATH*4] = { 0 };
  if( !MEMORY_PATH[0] )
  {
    sprintf_s( MEMORY_PATH, "%sMemory\\", rsuGetProjectPath() );
    ::CreateDirectory( MEMORY_PATH, NULL );
  }
  return MEMORY_PATH;
}

LPCSTR rsuGetExecutionPath()
{
  static char EXE_PATH[_MAX_PATH*4] = { 0 };
  if( !EXE_PATH[0] )
  {
    char szDrv[_MAX_DRIVE*4],szDir[_MAX_DIR*4];
    int nLen = ::GetModuleFileName( GetModuleHandle("SystemReplace.dll"),EXE_PATH,_countof(EXE_PATH));
    _strupr_s( EXE_PATH );
    _splitpath_s(EXE_PATH,szDrv,_countof(szDrv),szDir,_countof(szDir),NULL,0,NULL,0);
    _makepath_s (EXE_PATH,_countof(EXE_PATH),szDrv,szDir,NULL,NULL);
  }
  return EXE_PATH;
}

static char RSU_PATH[_MAX_PATH] = { 0 };

LPCSTR rsuGetPath2Rsu()
{
  if( !RSU_PATH[0] )
  {
    strcpy_s( RSU_PATH, rsuGetProjectPath() );
    char Path[_MAX_PATH];
    sprintf_s( Path, "%sPROP\\Properties.csv", RSU_PATH );
    char Str[128] = {0};
    if( _Prop( Path, "SB#Папка РСУ", "DS,RSU#", Str ) && Str[0] )
      sprintf_s( RSU_PATH, "%s%s\\", rsuGetProjectPath(), Str );
  }

  return RSU_PATH;
}

void rsuSetPath2Rsu( LPCSTR pszPath )
{
  strcpy_s( RSU_PATH, pszPath );
}

void rsuTakeOffReadOnly(LPCSTR szFileName)
{
  DWORD dwFlag = FILE_ATTRIBUTE_READONLY;
  DWORD dwAttr = ::GetFileAttributes( szFileName );
  if( dwAttr & dwFlag )
  {
    dwAttr ^= dwFlag;
    ::SetFileAttributes( szFileName, dwAttr );
  }
}

// Возвращает путь к директории с файлами трендов
LPCSTR rsuGetTrendPath()
{
  return rsuGetMemoryPath();
}

LPCSTR rsuGetAcyPath()
{
  static char szPath[_MAX_PATH] = "\0";
  if ( !*szPath )
  {
    sprintf_s( szPath, sizeof(szPath), "%sData\\Acy\\", rsuGetProjectPath() );
  }
  return szPath;
}
