#include <StateSer.h>
#include <CommProc.h>
#include <sr.h>

KStateSer::KStateSer(void)
{
}

KStateSer::~KStateSer(void)
{
}

IStateSer* KStateSer::GetInterface()
{
  return this;
}

void KStateSer::WriteImpl( const void *pObj, DWORD objSize )
{
  pSRFile->WD( (LPVOID)pObj, objSize );
}

bool KStateSer::ReadImpl( void *pObj, DWORD objSize )
{
  bool r = pSRFile->RD( pObj, objSize );
  ASS( r );
  return r;
}
