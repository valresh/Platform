#include "CommProc.h"
#include "SysDataTypes.h"
#include "Err.h"
#include <QDir>

CTrends Trends;

#define MAX_VARS 64
#define MAX_STEP 300000


CTrends::CTrends()
  {
  pRecords = NewArr ( Record, MAX_STEP );
  PosRecords = -1;
  kRecords = 0;
//
  kItems = 0;
  WasOpen = false;
  }

CTrends::~CTrends()
  {
  }

bool CTrends::ReadData( const char * File )
  {
  Char<1024>Path;
  Path.Prt ("%sINI/Trends/%s", PROJECT_ROOT, File );
  FILE * F = fopen ( Path, "rt" );
  if ( F == NULL )
    return false;
  char Str[1024];
  fgets ( Str, 1024, F );
  while ( fgets ( Str, 1024, F ))
    {
    char * P = Str;
    char * E = strchr ( P, ';' );
    if ( *E )
      *E = 0;
    char Type;
    void * Addr = ConnectPoint( P, Type );
    if ( Addr )
      Add( P, Type, Addr );
    }
  fclose ( F );
  }
void CTrends::Init( )
  {
  Add( "Sys_Time", 'D', &pSys->dModelT );
  Add( "Sys_dt", 'D', &pSys->dt );
  Add( "Sys_Step", 'I', &pSys->m_nStep );
  Add( "Sys_Accel", 'D', &pSys->Accel );
  Add( "Sys_RealTime", 'D', &pSys->Time_real );
// Просмотр всех файлов
  QString Dir = PROJECT_ROOT;
  Dir += "INI/Trends/";
  QDir DirList( Dir );
  QStringList Filtr ( "*.csv");
  QFileInfoList L = DirList.entryInfoList( Filtr,
                    QDir::NoFilter, QDir::Name);
  //  ui->List->setModel();
  foreach (QFileInfo f, L)
    {
    QString Name = f.fileName();
    ReadData( STR(Name) );
    }
  }


int CTrends::Add( const char * Name, char Type, void * pVar )
  {
  if ( kItems >= MAX_VARS )
    return -1;
  for ( int n = 0; n < kItems; n++ )
    {
    if ( strcmp ( Items[n].Name, Name ) == 0 )
      return n;
    }
  Items[kItems].Name = Name;
  Items[kItems].Type = Type;
  Items[kItems].pVar = pVar;
  int N = kItems++;
  return N;
  }


bool CTrends::Write( )
  {
  int Pos = PosRecords + 1;
  if ( Pos > MAX_STEP - 10)
    return false;
  Record & R = pRecords[Pos];
  R.Time = pSys->dModelT * 3600.;
  R.Step = pSys->m_nStep;
  for ( int n = 0; n < kItems; n++ )
    {
    CTrendsItem & pI = Items[n];
    switch ( pI.Type )
      {
      case 'D':
        {
        double V = *(double*)pI.pVar;
        R.Vars[n] = (float)V;
        }
      break;
      case 'I':
        R.Vars[n] = (float)*(int*)pI.pVar;
      break;
      case 'B':
      if ( *(bool*)pI.pVar)
        R.Vars[n] = 1;
        else
        R.Vars[n] = 0;
        break;
      default:
        R.Vars[n] = 0;
        break;
      }
    }
  PosRecords = Pos;
  return true;
  }

bool CTrends::GetLine( int Line, float * pVal )
{
  // int Lrec = 4 * kData;
  // SetFilePointer( hFile, Line * Lrec, NULL, FILE_BEGIN );
  // DWORD Lr;
  // ReadFile( hFile, pVal, Lrec, &Lr, NULL );
  // LeaveCriticalSection ( &CS );
  return true;
}

bool CTrends::GetGroupVar( int Line, int kVar, int * nVars, float * pVal )
{
  // if ( Line < 0 )
  // {
  //   memset ( pVal, 0, 4 * kVar );
  //   return true;
  // }
  // int Lrec = 4 * kData;
  // DWORD Pos = SetFilePointer( hFile, Line * Lrec, NULL, FILE_BEGIN );
  // DWORD Lr;
  // ReadFile( hFile, pValues, Lrec, &Lr, NULL );
  // for ( int n = 0; n < kVar; n++ )
  // {
  //   int N = nVars[n];
  //   if ( N >= 0 && N < kVars )
  //     pVal[n] = pValues[N];
  //   else
  //     pVal[n] = -1;
  // }
  return true;
}

bool CTrends::GetGroupVar( int Line, int nVar, CMem<CTrendsVar,16,16> & Vars )
{
  // int Lrec = 4 * kData;
  // SetFilePointer( hFile, Line * Lrec, NULL, FILE_BEGIN );
  // DWORD Lr;
  // ReadFile( hFile, pValues, Lrec, &Lr, NULL );
  // for ( int n = 0; n < Vars.L; n++ )
  // {
  //   CTrendsVar & V = Vars[n];
  //   int N = V.Numb;
  //   if ( N >= 0 && N < kVars )
  //     V.Data[nVar] = pValues[N];
  //   else
  //     V.Data[nVar] = -1;
  // }
  return true;
}
