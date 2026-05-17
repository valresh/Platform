#include "CommProc.h"
#include "SysDataTypes.h"
#include "Err.h"
#include <QDir>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>

#define INIT_TREND

CTrends Trends;
bool CTrends::WasOpen = false;
Record * CTrends::pRecs = NULL;

CTrends::CTrends()
  {
    int fd = open ( "/home/resh/Platform/DATA/trends.dat", O_RDWR );
#ifdef INIT_TREND
    Record Buf[MAX_STEP/1000];
    BYTE Head[sizeof(TrendsHead)];
    CLEAR(Buf)
    CLEAR(Head)
    write ( fd, Head, sizeof ( Head ) );
    for ( int n = 0; n < 1000; n++ )
      write ( fd, Buf, sizeof ( Buf ) );
#endif
    int Size = sizeof ( TrendsHead ) + MAX_STEP * sizeof ( Record );
    BYTE * pMem = (BYTE*)mmap(0, Size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );
    pHead = (TrendsHead*)pMem;
    pRecs = (Record *)( pMem + sizeof( TrendsHead ));
    close ( fd );
    pHead->PosRecords = -1;
    pHead->MaxRecords = MAX_STEP;
    //
    pHead->kItems = 0;
    WasOpen = true;
  }

Record & CTrends::R( int n )
  {
  while ( n < 0 )
    n += MAX_STEP;
  if ( n > MAX_STEP )
    n = n % MAX_STEP;
  return pRecs[n];
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


int TrendsHead::AddItem( const char * Name, char Type, void * pVar )
  {
  if ( kItems >= MAX_VARS )
    return -1;
  for ( int n = 0; n < kItems; n++ )
    {
    if ( strcmp ( Items[n].Name, Name ) == 0 )
      return n;
    }
  strcpy_s(Items[kItems].Name, 128, Name );
  Items[kItems].Type = Type;
  Items[kItems].pVar = pVar;
  int N = kItems++;
  return N;
  }

int CTrends::Add( const char * Name, char Type, void * pVar )
  {
  return pHead->AddItem( Name, Type, pVar );
  }

bool CTrends::Write( )
  {
  int Pos = ( pHead->PosRecords + 1 ) % MAX_STEP;
  Record & R = pRecs[Pos];
  R.Time = pSys->dModelT * 3600.;
  R.Step = pSys->m_nStep;
  for ( int n = 0; n < pHead->kItems; n++ )
    {
    CTrendsItem & pI = pHead->Items[n];
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
  pHead->PosRecords++;
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
