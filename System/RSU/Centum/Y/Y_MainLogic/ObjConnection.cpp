#include <DataTypes_.h>
#include <rsuErr.h>
#include <Y_Data.h>
#include "ObjConnection.h"
#include <stddef.h>

CObjConnection::CObjConnection()
{
  ZeroMemory( this, sizeof(*this)-offsetof(CObjConnection, CObjConnection::condition) );
}

bool CObjConnection::ContinueExchange()
{
  if( !bTest4SrcChanged || !pOut )
    return true;
  bool bSame = false;
  switch( TypeOut )
  {
  case 'D':
    bSame = prevSrcValue.D == *(double*)pOut;
    prevSrcValue.D = *(double*)pOut;
    break;
  case 'I':
    bSame = prevSrcValue.I == *(int*)pOut;
    prevSrcValue.I = *(int*)pOut;
    break;
  case 'B':
    bSame = prevSrcValue.B == *(bool*)pOut;
    prevSrcValue.B = *(bool*)pOut;
    break;
  case 'W':
    bSame = prevSrcValue.W == *(BYTE*)pOut;
    prevSrcValue.W = *(BYTE*)pOut;
    break;
  default:
    return true;
  }
  if( bSame )
    return false;
  return true;
}
