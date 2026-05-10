#include "QbRsuX.h"
#include <rsuErr.h>

KQbRsuX::KQbRsuX()
: nBlocks( 1 )
{
}

int KQbRsuX::LinkTo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pszFile )
{
  nBlocks = 1;
  return bl1.LinkTo( pszPointName, pszIOtype, pszFile );
}

int KQbRsuX::LinkToBl2( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pszFile, LPCSTR pszDataType )
{
  this->pszDataType = pszDataType;
  nBlocks = 2;
  return bl2.LinkTo( pszPointName, pszIOtype, pszFile );
}

void KQbRsuX::ManualInit( nRSUx::SParamInfo &info )
{
  bl1.ManualInit( info );
}

bool KQbRsuX::IsConnected()
{
  if( 2==nBlocks )
    return bl1.IsConnected() && bl2.IsConnected();
  return bl1.IsConnected();
}

void KQbRsuX::Reset()
{
  nRSUx::SParamInfo info;
  bl1.ManualInit( info );
  bl2.ManualInit( info );
}

KQbRsuX::operator int()
{
  if( 2==nBlocks )
  {
    WORD v1 = (int)bl1;
    WORD v2 = (int)bl2;
    KKK();
  }
  return bl1;
}

KQbRsuX::operator double()
{
  if( 2==nBlocks )
  {
    float r = 0;
    //DWORD inV = 0;
    DWORD inV = ((int)bl1)& 0xFFFF;
    DWORD v2 = ((int)bl2)& 0xFFFF;
    if( 'R'==*pszDataType )
    {
      inV |= (v2<<16);
      memcpy( &r, &inV, sizeof(r) );
    }
    else if( 'I'==*pszDataType )
    {
      ASSD( '4'==pszDataType[3] );
      inV |= (v2<<16);
      int i = 0;
      memcpy( &i, &inV, sizeof(i) );
      r = i;
    }
    else
    {
      ASSD(0);
    }
    if( 0!=r )
      KKK();
    return r;
  }
  return bl1;
}

KQbRsuX::operator BYTE()
{
  return bl1;
}

void KQbRsuX::operator = ( int thr )
{
  bl1 = thr;
}

void KQbRsuX::operator = ( double thr )
{
  bl1 = thr;
}
