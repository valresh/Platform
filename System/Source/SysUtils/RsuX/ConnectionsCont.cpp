#include "RsuX.h"
#include <cstring>
//#include <err.h>


template < typename T, size_t N >
constexpr size_t countof(const T(&arr)[N])
{
    return N;
}

using namespace nRSUx;

struct SInfoHandler
{
    char szFileName[64];
    tRsuConnectParamInfo pCall;
    tRsuConnectParamInfoTypes pCallTyped;
    SInfoHandler()
    {
        memset( szFileName, 0, sizeof(szFileName) );
        pCall = NULL;
        pCallTyped = NULL;
    }
};

static SInfoHandler s_InfoImpls[10];

void RegisterRsuConnectionImpl( LPCSTR pszFileUpperName, tRsuConnectParamInfo pHandler, tRsuConnectParamInfoTypes pHandlerTyped )
{
    if( pszFileUpperName )
    {
        for( int i=0; i< countof(s_InfoImpls); i++ )
        {
            if( s_InfoImpls[i].pCall || s_InfoImpls[i].pCallTyped )
            {
                if( !strcmp( s_InfoImpls[i].szFileName, pszFileUpperName) &&
                    ( ( pHandler && s_InfoImpls[i].pCall==pHandler) || ( pHandlerTyped && s_InfoImpls[i].pCallTyped==pHandlerTyped) ) )
                    return;
                continue;
            }
            else
            {
                //ASSD( !s_InfoImpls[i].szFileName[0] );
                strcpy( s_InfoImpls[i].szFileName, pszFileUpperName);
                s_InfoImpls[i].pCall = pHandler;
                s_InfoImpls[i].pCallTyped = pHandlerTyped;
                return;
            }
        }
    }
    else
    {
        for( int i=0; i < countof(s_InfoImpls); i++ )
        {
            if( s_InfoImpls[i].pCallTyped == pHandlerTyped )
                return;
        }
        for( int i=0; i < countof(s_InfoImpls); i++ )
        {
            if( !s_InfoImpls[i].szFileName[0] && !s_InfoImpls[i].pCall && !s_InfoImpls[i].pCallTyped )
            {
                s_InfoImpls[i].pCallTyped = pHandlerTyped;
                return;
            }
        }
    }
    //ASS( !"Надо расширить хранилище" );
}

SParamInfo GetInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pszFileName )
{
    SParamInfo info;
    for( int i=0; i < countof(s_InfoImpls); i++ )
    {
        if( strstr( pszFileName, s_InfoImpls[i].szFileName)!=pszFileName )
            continue;
        if( !s_InfoImpls[i].pCall )
            continue;
        size_t lDll = strlen( s_InfoImpls[i].szFileName );
        LPCSTR pSuffix = &pszFileName[lDll];
        info = s_InfoImpls[i].pCall( pszPointName, pszIOtype, *pSuffix=='\0' ? NULL : pSuffix );
        if( info.pMem )
            break;
    }
    return info;
}

SParamInfo GetInfo( LPCSTR pszPointName, DWORD TypeID )
{
#ifdef LINUX
//    CLogFile::Log ("GetInfo_0: TypeID %d, Total %d, pszPointName %s", TypeID, countof(s_InfoImpls), pszPointName);
#endif // LINUX

    SParamInfo info;
    for( int i=0; i < countof(s_InfoImpls); i++ )
    {
        if( !s_InfoImpls[i].pCallTyped )
            continue;
        info = s_InfoImpls[i].pCallTyped( pszPointName, TypeID );
#ifdef LINUX
//        CLogFile::Log ("GetInfo_2: info.pMem %16lx", info.pMem);
#endif // LINUX

        if( info.pMem )
            break;
    }
    return info;
}
