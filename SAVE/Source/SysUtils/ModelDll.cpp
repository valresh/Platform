#include "CommProc.h"
#include "Err.h"
#include "errno.h"

CCSVData * pModelDLL;

static CMem<CStr,1024,1024>c_data;

char ** CCSVData::Head ( )
{
  int N = Start;
  for ( int n = 0; n < kCol; n++ )
  {
    CStr & S = c_data[N++];
    ppRow[n] = S;
  }
  return ppRow;
}

CCSVData::~CCSVData()
{
  if ( SortOrder )
  {
    delete [] SortOrder;
    SortOrder = NULL;
  }
  if ( ppRow )
  {
    delete [] ppRow;
    ppRow = NULL;
  }
}

void CCSVData::Free()
{
  c_data.Free();
}

int CCSVData::Load ( int _kCol, char * Root, char * File, char * Pref )
{
  kCol = _kCol;
  char Path[256];
  sprintf ( Path, "%sINI/%s.csv", Root, File );
  LoadPath = Path;
  int Res = Load ( Pref );
  return Res;
}

void CCSVData::Reset()
{
  c_data.Reset();
}
//
int CCSVData::Load( char * _Pref )
{
  //Reset();
  Pref = _Pref;
  if ( ppRow == NULL )
    ppRow = new char*[kCol];
  Start = c_data.L;
  FILE * F = fopen ( LoadPath.Str, "rt" );
  if ( F == NULL )
    {
    int err = errno;
    SysMsgErr( "Ошибка %d открытия файла '%s'", err, LoadPath.Str );
    THROW
    return 1;
    }
  char Str[256];
  fgets( Str, 255, F );
  Size = 0;
  while ( fgets( Str, 255, F ))
  {
    int L = lstrlen ( Str ) - 1;
    Str[L] = 0;
//    if ( strstr ( Str, "Электрооборудование" ) )
//      KKK();
    char * P = Str;
    for ( int n = 0; n < kCol; n++ )
    {
      CStr & S = c_data.push_back();
      if ( P )
      {
        char * Pn = strchr ( P, ';' );
        if ( Pn )
          *Pn = 0;
        if ( n == 0 )
        {
          char Name[128];
          lstrcpy ( Name, Pref );
          lstrcat ( Name, P );
          S = Name;
        }
        else
          S = P;
        if ( Pn )
          P = Pn + 1;
        else
          P = NULL;
      }
      else
        S = "";
    }
    Size++;
  }
  fclose( F );
  Size++;
  return 0;
}

static int kC;
static int Shift;
static char * pKey;

int CompKey ( const void *  p1, const void *  p2 )
{
  int n1 = *(int*)p1;
  ASS(n1<0 )
  int n2 = *(int*)p2;
  char * S1 = pKey;
  int N2 = n2 * kC + Shift;
  // Здесь падает по неверному индексу (VM: 08.08.2017)
  if ( N2 >= c_data.L )
    return 1;
  CStr & S2 = c_data[N2];
  int Res = strcmp ( (char*)S1, (char*)S2 );
  return Res;
}

int CompData ( const void *  p1, const void *  p2 )
{
  int n1 = *(int*)p1;
  int n2 = *(int*)p2;
  int N1 = n1 * kC + Shift;
  int N2 = n2 * kC + Shift;
  CStr & S1 = c_data[N1];
  CStr & S2 = c_data[N2];
  int Res = lstrcmp ( (char*)S1, (char*)S2 );
  return Res;
}

bool CCSVData::Sort( )
{
  char ** ppHead = Head( );
  //
  kC = kCol;
  //
  SortOrder = new int[Size];
  for ( int s = 0; s < Size; s++ )
  {
    SortOrder[s] = s;
  }
  Shift = 0;//Start + kC;  // Пропуск заголовка
  qsort( SortOrder, Size-1, sizeof(int), CompData );
  return true;
}

char ** CCSVData::Find( char * pName_1 )
{
  char Name[128];
  lstrcpy ( Name, Pref );
  lstrcat ( Name, pName_1 );
  kC = kCol;
  int key = -1;
  pKey = Name;
  Shift = 0;//Start + kC;
  int * pN = (int*)bsearch( &key, SortOrder, Size, 4, CompKey );
  if ( pN == NULL )
    return NULL;
  int N = *pN * kC + Shift;
  if ( N >= c_data.L )
    return NULL;
  for ( int j = 0; j < kCol; j++ )
  {
    CStr & S = c_data[N+j];
    ppRow[j] = S;
  }
  return ppRow;
}

char ** CCSVData::Line( int N )
{
  int n = N * kCol + Start + kCol;
  for ( int j = 0; j < kCol; j++ )
  {
    CStr & S = c_data[n+j];
    ppRow[j] = S;
  }
  return ppRow;
}

char ** CCSVData::SortLine( int N )
{
  N = SortOrder[N];
  int n = N * kCol + Start + kCol;
  for ( int j = 0; j < kCol; j++ )
  {
    CStr & S = c_data[n+j];
    ppRow[j] = S;
  }
  return ppRow;
}

int CCSVData::Add( const char ** NewLine )
{
  FILE * F = fopen ( LoadPath, "at" );
  if ( F == NULL )
    return 1;
  for ( int j = 0; j < kCol; j++ )
    fprintf ( F, "%s;", NewLine[j] );
  fprintf ( F, "\n" );
  fclose ( F );
  ASS(0)
  int Res = Load ( "" );
  Sort( );
  return Res;
}
