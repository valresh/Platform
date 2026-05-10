#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include <macros/StrHelps.h>
#include <string>

void TraceErrors( LPCSTR Name, LPCSTR type, int NumbFCS )
{
  char szBuf[256];
  sprintf_s( szBuf, "Через GetObj не найдена точка \"%s\" типа %s, FCS %d\n", Name, type, NumbFCS );
  OutputDebugString( szBuf );
}

CY_Base* GetObj( char * Name, CY_Base *pOwnerObj, bool bRefToOtherFcs /*= false*/ )
{
  CY_Base * pObj = NULL;
  if ( strncmp ( Name, "%Z", 2 ) == 0 )
  {
    int Numb;
    int NumbFCS = 0, domain = 0;
    Numb = atoi ( Name + 2 );
    
    if( pOwnerObj && pOwnerObj->pDR )
    {
      NumbFCS = bRefToOtherFcs ? (-pOwnerObj->pDR->NumbFCS) : pOwnerObj->pDR->NumbFCS;
      domain = pOwnerObj->pDR->domain;
    }

    if( Name[8]=='S' )
    {
      NumbFCS = atoi ( Name+9 );
      if( strlen(Name+9)==6 )
        NumbFCS /= 100;
      domain = NumbFCS / CY_DR::s_dwFcsNumModulus;
      NumbFCS %= CY_DR::s_dwFcsNumModulus;
    }

    pObj = (*pFindDI)( NumbFCS, domain, Numb, id__Z );
    if( !pObj)
      TraceErrors( Name, "%Z", NumbFCS );
    return pObj;
  }
  if ( strncmp ( Name, "%Y", 2 ) == 0 )
  {
    int Numb;
    int NumbFCS = 0, domain = 0;
    Numb = atoi ( Name + 2 );

    if( pOwnerObj && pOwnerObj->pDR )
    {
      NumbFCS = bRefToOtherFcs ? (-pOwnerObj->pDR->NumbFCS) : pOwnerObj->pDR->NumbFCS;
      domain = pOwnerObj->pDR->domain;
    }

    if( Name[9]=='S' )
    {
      ASSD(!"проверить")
      NumbFCS = atoi( Name + 10 );
      if( strlen(Name+10)==6 )
        NumbFCS /= 100;
      domain = NumbFCS / CY_DR::s_dwFcsNumModulus;
      NumbFCS %= CY_DR::s_dwFcsNumModulus;
    }

    pObj = (*pFindDI)( NumbFCS, domain, Numb, id__Y );
    if( !pObj)
      TraceErrors( Name, "%Y", NumbFCS );
    return pObj;
  }
  if ( strncmp ( Name, "%GS", 3 ) == 0 )
  {
    int Numb;
    ASSD( pOwnerObj );
    int NumbFCS = 0, domain = 0;

    if( pOwnerObj && pOwnerObj->pDR )
    {
      NumbFCS = bRefToOtherFcs ? (-pOwnerObj->pDR->NumbFCS) : pOwnerObj->pDR->NumbFCS;
      domain = pOwnerObj->pDR->domain;
    }

    if( Name[6] )
    {
      LPSTR pNumb = Name + 3;
      Numb = atoi ( pNumb+3 );
      NumbFCS = Numb % CY_DR::s_dwFcsNumModulus;
      TStringTerminator st( pNumb + 3);
      Numb = atoi( pNumb );
    }
    else
    {
      Numb = atoi ( Name + 3 );
      NumbFCS = pOwnerObj->pDR->NumbFCS;
    }

    pObj = (*pFindDI)( NumbFCS, domain, Numb, id__GS );

    if( !pObj )
      TraceErrors( Name, "%GS", NumbFCS );
    return pObj;
  }
  if ( strncmp ( Name, "%WB", 3 ) == 0 )
  {
    ASS( Name[6] );
    int Numb = atoi ( Name + 3 );
    int NumbFCS = pOwnerObj->pDR->NumbFCS;
    int domain = pOwnerObj->pDR->domain;
    pObj = (*pFindDI)( NumbFCS, domain, Numb, id__WB );
    if( !pObj)
    {
      using namespace std;
      string szName( Name );

      string::size_type pos = szName.find_first_not_of( "0123456789", 3 );
      if( string::npos!=pos )
      {
        int NumbFcsIn;
        ASS( 'S'==Name[pos] );
        char szBL[20] = { 0 };
        strncpy_s( szBL, Name, pos );
        string::size_type lenStation = szName.length()-pos-1;
        NumbFcsIn = atoi( Name+pos+1 );
        switch( lenStation )
        {
        case 4:
          break;
        case 6:
          NumbFcsIn = NumbFcsIn / 100;
          break;
        default:
          ASSD(0);
        }
        domain = NumbFcsIn / CY_DR::s_dwFcsNumModulus;
        NumbFcsIn = NumbFcsIn % CY_DR::s_dwFcsNumModulus;
        pObj = pFindObjByNumb( szBL, NumbFcsIn, domain );
        if( pObj )
          return pObj;
      }

      TraceErrors( Name, "%WB", NumbFCS );
    }
    return pObj;
  }
  if ( strncmp ( Name, "%SW", 3 ) == 0 )
  {
    int Numb = atoi ( Name + 3 );
    int NumbFCS = pOwnerObj->pDR->NumbFCS;
    int domain = pOwnerObj->pDR->domain;
    if ( Name[7]  && isdigit(Name[7]) )
    {
      NumbFCS = Numb % 100;
      Numb = Numb / 100;
    }
    else if( Name[7]=='S' )
    {
      NumbFCS = atoi( Name + 8 );
      domain = NumbFCS / CY_DR::s_dwFcsNumModulus;

      NumbFCS %= CY_DR::s_dwFcsNumModulus;
    }
    else
    {
      if( Name[7] && !isdigit(Name[7]) )
      {
        ASSD( !strcmp( &Name[7], ".PV") );
      }
    }
    pObj = (*pFindDI)( NumbFCS, domain, Numb, id__SW );
    if( !pObj)
      TraceErrors( Name, "%SW", NumbFCS );
    return pObj;
  }
  if ( strncmp ( Name, "%PR", 3 ) == 0 )
  {
    int Numb;
    int NumbFCS;
    Numb = atoi ( Name + 3 );
    NumbFCS = pOwnerObj->pDR->NumbFCS;
    pObj = (*pFindDI)( NumbFCS, pOwnerObj->pDR->domain, Numb, id__PR );
    if( !pObj)
      TraceErrors( Name, "%PR", NumbFCS );
    return pObj;
  }
  if ( strncmp ( Name, "%OG", 3 ) == 0 )
  {
    int Numb;
    int NumbFCS;
    Numb = atoi ( Name + 3 );
    NumbFCS = pOwnerObj->pDR->NumbFCS;
    pObj = (*pFindDI)( NumbFCS, pOwnerObj->pDR->domain, Numb, id__OG );
    if( !pObj )
      pObj = (*pFindDI)( -NumbFCS, pOwnerObj->pDR->domain, Numb, id__OG );
    if( !pObj)
      TraceErrors( Name, "%OG", NumbFCS );
    return pObj;
  }
  if ( strncmp ( Name, "%RQ", 3 ) == 0 )
  {
    int Numb = atoi( Name + 3 );
    pObj = (*pFindDI)( 0, pOwnerObj->pDR->domain, Numb, id__RQ );
    if( !pObj)
      TraceErrors( Name, "%RQ", 0 );
    return pObj;
  }

  LPSTR pszFcsSep = strchr( Name, '@');
  TStringTerminator st( pszFcsSep && pszFcsSep > Name && '.'!=pszFcsSep[-1] ? pszFcsSep : NULL );
  if( pszFcsSep && pszFcsSep > Name && '.'!=pszFcsSep[-1] )
    bRefToOtherFcs = true;

  int NumbFcs = 0;
  int domain = 0;
  if( pOwnerObj && pOwnerObj->pDR )
  {
    NumbFcs = bRefToOtherFcs ? (-pOwnerObj->pDR->NumbFCS) : pOwnerObj->pDR->NumbFCS;
    domain = pOwnerObj->pDR->domain;
  }

  pObj = (*pFindYObjN)( Name, NumbFcs, domain );
  if( pObj == NULL )
    pObj = (*pFindDObj)( Name, NumbFcs, domain );
  if( NULL==pObj )
    pObj = (*pFindYObjN)( Name, -NumbFcs, domain );
  if( pObj == NULL )
    pObj = (*pFindDObj)( Name, -NumbFcs, domain );

  if( !pObj)
  {
    if( strncmp(Name,"%CP", 3) )
      TraceErrors( Name, "%??", NumbFcs );
  }
  return pObj;
}
