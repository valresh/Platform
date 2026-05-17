#include "CommProc.h"
#include "SysDataTypes.h"
#include "Err.h"
#include <QDir>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>


CTrends Trends;
bool CTrends::WasOpen = false;
Record * CTrends::pRecs = NULL;

CTrends::CTrends()
  {
    int fd = open ( "/home/resh/Platform/DATA/trends.dat", O_RDWR );
    int Size = sizeof ( TrendsHead ) + MAX_STEP * sizeof ( Record );
    BYTE * pMem = (BYTE*)mmap(0, Size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );
    pHead = (TrendsHead*)pMem;
    pRecs = (Record *)( pMem + sizeof( TrendsHead ));
    close ( fd );
    pHead->MaxRecords = MAX_STEP;
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
  return false;
  }

void CTrends::Init( )
  {
  }


int TrendsHead::AddItem( const char * Name, char Type, void * pVar )
  {
  return -1;
  }

int CTrends::Add( const char * Name, char Type, void * pVar )
  {
  for ( int n = 0; n < pHead->kItems; n++ )
    {
    if ( strcmp ( pHead->Items[n].Name, Name ) == 0 )
        return n;
    }
  return -1;
  }

bool CTrends::Write( )
  {
  return false;
  }

bool CTrends::GetLine( int Line, float * pVal )
{
  return true;
}

bool CTrends::GetGroupVar( int Line, int kVar, int * nVars, float * pVal )
{
  return true;
}

bool CTrends::GetGroupVar( int Line, int nVar, CMem<CTrendsVar,16,16> & Vars )
{
  return true;
}
