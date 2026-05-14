#include "RsuX.h"
#include <float.h>
//#include <err.h>
//#include <rsu_str.h>
#include <algorithm>
#include <crossplatform.h>
//#include <crosspath.h>
#ifdef LINUX
//#include "LogFile.h"
#endif // LINUX
#include <filesystem>
#include <string>
namespace fs = std::filesystem;


void RegisterRsuConnectionImpl( LPCSTR, tRsuConnectParamInfo, tRsuConnectParamInfoTypes );
nRSUx::SParamInfo GetInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pszFileName );
nRSUx::SParamInfo GetInfo( LPCSTR pszPointName, DWORD TypeID );

tRegisterRsuConnection pRegisterRsuConnection = RegisterRsuConnectionImpl;

KRsuX::KRsuX()
{
}

KRsuX::~KRsuX()
{
}

#include <QFileInfo>

int KRsuX::LinkTo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pszFile )
{
    // fs::path p(pszFile);
    // p.replace_extension("");
    // std::string fn = p.filename().generic_string();
    //??    toupper(fn);
    char Path[1024];
    strcpy ( Path, pszFile );
    char * P = Path;
    char * S = Path;
    while ( *S )
    {
        if ( *S == '\\' )
            *S = '/';
        if ( *S == '/' )
            P = S;
        S++;
    }
    P++;
    char * E = strchr ( P, '.' );
    if ( E )
        *E = 0;
    nRSUx::SParamInfo info = GetInfo( pszPointName, pszIOtype, P );
    if( info.pMem )
    {
        m_PointInfo = info;
        return 0;
    }
    return -1;
}

int KRsuX::LinkTo( LPCSTR pszPointName, DWORD TypeID )
{
#ifdef LINUX
//    CLogFile::Log ("KRsuX::LinkTo_0: TypeID %d, pszPointName %s", TypeID, pszPointName);
#endif // LINUX
    nRSUx::SParamInfo info = GetInfo( pszPointName, TypeID );
#ifdef LINUX
//    CLogFile::Log ("KRsuX::LinkTo_1: info.pMem %16lx", info.pMem);
#endif // LINUX
    if( info.pMem )
    {
        m_PointInfo = info;
        return 0;
    }

    return -1;
}

void KRsuX::ManualInit( nRSUx::SParamInfo &info )
{
    m_PointInfo = info;
}

KRsuX& KRsuX::operator << ( KRsuX &thr )
{
    switch( m_PointInfo.type )
    {
    case nRSUx::e_char:
        *this  = thr.GetValue<char>();
        break;
    case nRSUx::e_short:
        *this  = thr.GetValue<short>();
        break;
    case nRSUx::e_word:
        *this  = thr.GetValue<WORD>();
        break;
    case nRSUx::e_float:
        *this  = thr.GetValue<float>();
        break;
    case nRSUx::e_double:
        *this  = thr.GetValue<double>();
        break;
    case nRSUx::e_BOOL:
    case nRSUx::e_int:
        *this  = thr.GetValue<int>();
        break;
    case nRSUx::e_bool:
        *this  = thr.GetValue<bool>();
        break;
    case nRSUx::e_byte:
        *this  = thr.GetValue<BYTE>();
        break;
        /*default:
    ASS(0);*/
    }
    return *this;
}

KRsuX& KRsuX::operator *= ( double mul )
{
    switch( m_PointInfo.type )
    {
    case nRSUx::e_char:
        *this  = GetValue<char>() * mul;
        break;
    case nRSUx::e_short:
        *this  = GetValue<short>() * mul;
        break;
    case nRSUx::e_word:
        *this  = GetValue<WORD>() * mul;
        break;
    case nRSUx::e_float:
        *this  = GetValue<float>() * mul;
        break;
    case nRSUx::e_double:
        *this  = GetValue<double>() * mul;
        break;
    case nRSUx::e_int:
        *this  = GetValue<int>() * mul;
        break;
        /*default:
    ASS(0);*/
    }
    return *this;
}

bool KRsuX::IsConnected()
{
    return m_PointInfo.pMem ? true : false;
}

bool KRsuX::IsPriorOnCBase()
{
    return m_PointInfo.bPriorityOnCBase ? true : false;
}


template<class T, class M>
T KRsuX::GetValueTyped()
{
    M &m = *((M*)m_PointInfo.pMem);

    if( -1 == m_PointInfo.bitOffset )
        return (T)m;
    return (T)((static_cast<int>(m) & (1<<m_PointInfo.bitOffset)) ? 1 : 0);

    return (T)(-1);
}

template<class T>
T KRsuX::GetValue()
{
    switch( m_PointInfo.type )
    {
    case nRSUx::e_bool:
        return GetValueTyped<T,bool>();
    case nRSUx::e_byte:
        return GetValueTyped<T,BYTE>();
    case nRSUx::e_char:
        return GetValueTyped<T,char>();
    case nRSUx::e_short:
        return GetValueTyped<T,short>();
    case nRSUx::e_word:
        return GetValueTyped<T,WORD>();
    case nRSUx::e_dword:
        return GetValueTyped<T,DWORD>();
    case nRSUx::e_float:
        return (T)(*((float*)m_PointInfo.pMem));
    case nRSUx::e_double:
        return (T)(*((double*)m_PointInfo.pMem));
    case nRSUx::e_BOOL:
    case nRSUx::e_int:
        return GetValueTyped<T,int>();
        /*default:
	ASS(0);*/
    }
    return (T)(-1);
}

template<typename T>
T SwapBytes( T s )
{
    BYTE *pB = (BYTE*)&s;
    for (size_t i = 0; i < sizeof(T) / 2; ++i)
        std::swap(pB[i], pB[sizeof(T) - i - 1]);
    return s;
}

template<class T, class M>
void KRsuX::SetValueTyped( T thr )
{
    M &m = *((M*)m_PointInfo.pMem);
    if( -1 == m_PointInfo.bitOffset )
    {
        m = (M)thr;
        if( m_PointInfo.bSwapDoubleData )
            m = SwapBytes(m);
    }
    else
    {
        if( 0 != thr )
            m = static_cast<int>(m) | (1<<m_PointInfo.bitOffset);
        else
            m = static_cast<int>(m) & (~(1<<m_PointInfo.bitOffset));
    }
}

template<class T>
void KRsuX::SetValue( T thr )
{
    switch( m_PointInfo.type )
    {
    case nRSUx::e_bool:
        SetValueTyped<T,bool>(thr);
        break;
    case nRSUx::e_char:
        SetValueTyped<T,char>(thr);
        break;
    case nRSUx::e_short:
        SetValueTyped<T,short>(thr);
        break;
    case nRSUx::e_word:
        SetValueTyped<T,WORD>( (thr<0) ? 0 : thr);
        break;
    case nRSUx::e_dword:
        SetValueTyped<T,DWORD>( (thr<0) ? 0 : thr);
        break;
    case nRSUx::e_double:
        if( m_PointInfo.bSwapDoubleData )
            *((double*)m_PointInfo.pMem) = SwapBytes( (double)thr );
        else
            *((double*)m_PointInfo.pMem) = (double)thr;
        break;
    case nRSUx::e_float:
    {
        float flt = FLT_MAX;
        if( thr < FLT_MAX )
            flt = thr;
        if( m_PointInfo.bSwapDoubleData )
            *((float*)m_PointInfo.pMem) = SwapBytes( flt );
        else
            *((float*)m_PointInfo.pMem) = flt;
    }
    break;
    case nRSUx::e_BOOL:
    case nRSUx::e_int:
        SetValueTyped<T,int>(thr);
        break;
    case nRSUx::e_byte:
        SetValueTyped<T,BYTE>(thr);
        break;
        /*default:
		ASS(0);*/
    }
}

KRsuX::operator int()
{
    if( !m_PointInfo.pMem )
        return (-1);
    int i = GetValue<int>();
    if( m_PointInfo.bSwapDoubleData )
        i = SwapBytes( i );
    return i;
}

KRsuX::operator double()
{
    if( !m_PointInfo.pMem )
        return DBL_MAX;
    double d = GetValue<double>();
    if( m_PointInfo.bSwapDoubleData )
        d = SwapBytes( d );
    return d;
}

KRsuX::operator bool()
{
    if( !m_PointInfo.pMem )
        return 0;
    bool b = GetValue<bool>();
    return b;
}

KRsuX::operator BYTE()
{
    if( !m_PointInfo.pMem )
        return 0;
    bool b = GetValue<BYTE>();
    return b;
}

KRsuX::operator float()
{
    if( !m_PointInfo.pMem )
        return 0;
    float b = GetValue<float>();
    if( m_PointInfo.bSwapDoubleData )
        b = SwapBytes( b );
    return b;
}

KRsuX& KRsuX::operator = ( int thr )
{
    if( m_PointInfo.pMem )
        SetValue( thr );
    return *this;
}

KRsuX& KRsuX::operator = ( double thr )
{
    if( m_PointInfo.pMem )
        SetValue( thr );

    return *this;
}
