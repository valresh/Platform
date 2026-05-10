#include <rsuRand.h>
#include <crossplatform.h>

static UINT X0 = 0x38762CA5, Y0 = 0x7B2CD169;
static UINT X;
static UINT Y;
////////////////////////////////////////////////////////////////////////////////
UINT UL( );
static UINT R [256];
static UINT ax0[5] = { 0x007D6795, 0x009A3717, 0x007B21F3, 0x009A3257, 0x00703F2B };
static UINT bx0[5] = { 0x547F289B, 0x5FA47831, 0xB219D459, 0x369DE46B, 0xD9382B17 };
static UINT ay0[5] = { 0x009B2979, 0x007D4517, 0x008A673F, 0x0097326B, 0x007A2589 };
static UINT by0[5] = { 0xCD548A23, 0x1F379355, 0x768AB1E7, 0x651F446F, 0xA7691573 };
static UINT Ax;
static UINT Bx;
static UINT Ay;
static UINT By;
static UINT kRehash;
static double ulr;

#ifdef _WIN32
static unsigned long long getfiletime()
{
    FILETIME FT;
    GetSystemTimeAsFileTime(&FT);
    void* t = &FT;
    long long l = *(long long*)t;
    return l;
}
#else
#include <sys/time.h>
/**
 * number of seconds from 1 Jan. 1601 00:00 to 1 Jan 1970 00:00 UTC
 */
#define EPOCH_DIFF 11644473600LL

static unsigned long long getfiletime() 
{
    struct timeval tv;
    unsigned long long result = EPOCH_DIFF;
    gettimeofday(&tv, NULL);
    result += tv.tv_sec;
    result *= 10000000LL;
    result += tv.tv_usec * 10;
    return result;
}
#endif

static DWORD LowDateTime(long long DT)
{
    return (DWORD)(DT & 0xFFFFFFFF);
}

static DWORD HighDateTime(long long DT)
{
    return (DWORD)(DT >> 32);
}

UINT UL( )
{
    kRehash++;
    if ( kRehash >= 100 )
    {
        kRehash = 0;
        long long FT = getfiletime();
        X ^= LowDateTime(FT);
        Y ^= HighDateTime(FT);
    }
    UINT N, V, Xt, Yt;
    Xt  = Ax  * X  + Bx;
    Yt  = Ay  * Y  + By;
    N = Yt >> 24;
    N = N & 0x000000FF;
    V = R[N];
    R[N] = Xt;
    X = Xt;
    Y = Yt;
    return V;
}

void InitRand( int n )  
{
    long long FT = getfiletime();  
    X0 ^= LowDateTime(FT);
    Y0 ^= HighDateTime(FT);
    X = X0;
    Y = Y0;
    Ax = ax0[n];
    Bx = bx0[n];
    Ay = ay0[n];
    By = by0[n];
    int j;
    for( j = 0; j < countof(R); ++j )
    {
        for ( int k = 0; k < 100; k++ )
        {
            X  = Ax  * X  + Bx;
            Y  = Ay  * Y  + By;
        }
        R[j] = X;
    }
    kRehash = 0;
    for ( j = 0; j < 3761; j++)
        UL ( );
    unsigned long max = 0x80000000;
    double v = max;
    v *= 2.;
    v = 1. / v;
    ulr = (double)v;
}

double r01()
{
  UINT v;
  double r;
  for ( int n = 0; n < 7; n++ )
    v = UL();
  r = v * ulr;
  return r;
}
