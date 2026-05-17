#include <QDir>
#include "Err.h"
#include "CommProc.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_MEM 50000000

void KKK()
{

}

char PROJECT[32];
char PROJECT_ROOT[1024];
char EXE_PATH[1024];
char SYSTEM_PATH[1024];

void SetPaths()
  {
  QDir DirEXE = QDir::current();
  QString EXE = DirEXE.path();
  strcpy ( EXE_PATH, EXE.toStdString().c_str() );
  strcpy ( PROJECT_ROOT, EXE_PATH );
  int L = strlen ( PROJECT_ROOT );
    while ( PROJECT_ROOT[L] != '/' )
      L--;
  PROJECT_ROOT[L] = 0;
  strcpy ( SYSTEM_PATH, PROJECT_ROOT );
  strcat ( SYSTEM_PATH, "/" );
  strcat ( PROJECT_ROOT, "/projects/" );
  strcat ( PROJECT_ROOT, PROJECT );
  strcat ( PROJECT_ROOT, "/" );
  strcat ( EXE_PATH, "/" );
  KKK();
  }

int sprintf_s ( char * To, int MaxLen, const char * Fmt, ... )
  {
    va_list arg;
    va_start(arg,Fmt);
    return vsnprintf ( To, MaxLen, Fmt, arg );
  }

  static BYTE * pMem = NULL;
  static int64_t PosMem = 0;
#include <QMutex>
  QMutex M;
  void * NewMem( size_t size )
  {
    // int k = size;
    // BYTE * pAddr = new BYTE[size];
    // PosMem += size;
    // memset(pAddr,0,size);
    //return pAddr;
    M.lock();
    if ( pMem == NULL )
      {
        int fd = open ( "/home/resh/Platform/DATA/mem.dat", O_RDWR );
        pMem = (BYTE*)mmap(0, MAX_MEM, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0 );
        close ( fd );
        PosMem = 0;
    }
    //  assert(PosMem + size < MAX_MEM );
    PosMem = (( PosMem >> 4 ) + 1 ) << 4;
      if ( PosMem + size >= MAX_MEM )
        KKK();
  BYTE * Addr = pMem + PosMem;
  memset(Addr,0,size);
  PosMem += size;
  M.unlock();
  return Addr;
  }
