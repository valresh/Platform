#include "ParmVarInfo.h"

int CompVarInfo( const void * pP1, const void * pP2 )
{
  SVarInfo * pI1 = (SVarInfo*)pP1;
  SVarInfo * pI2 = (SVarInfo*)pP2;
  return strcmp( pI1->VarName, pI2->VarName );
}
