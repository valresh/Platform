#include "ServerExSharedPlugin_FlexRsu.h"
#include <shdata.h>
#include <eventstring.h>
#include <Common.h>
#include "RsuF.h"
#include "TagSkipFilter.h"

extern "C" SERVEREXSHAREDPLUGIN_FLEXRSU_API int SizeShRSU(DWORD type, KRsuX *pPnt)
{
  switch (type)
  {
  case CFlexSystem::TypeID:
    return sizeof(CShFlexSystem);
  case SFlexFutureTrend::TypeID:
    return sizeof(CShFlexFutureTrend);
  case SFlex::TypeID:
    int L = 0;
    if( pPnt)
    {
      KRsuF *pF = (KRsuF*)pPnt;
      switch( pF->GetType() )
      {
      case nRSUx::e_chars:
        L = (int)pF->GetLength();
        break;
      }
    }
    return sizeof(CShFlex) + L;
  }
  return 0;
}

extern "C" SERVEREXSHAREDPLUGIN_FLEXRSU_API int ExtractumRSU( DWORD type, CShBase* pSB, CBase *pB )
{
  pSB->m_nSize = SizeShRSU(type,NULL);
  if( !pSB->m_nSize )
    return 0;
  switch (type)
  {
  default:
    pSB->RetCode = CShBase::rcNO;
    return pSB->m_nSize;
  case CFlexSystem::TypeID:
    {
      CShFlexSystem* sh = (CShFlexSystem*)pSB;
      CFlexSystem* hs = static_cast<CFlexSystem*>(sh);
      memcpy( hs, pB, pSB->m_nSize );
      pSB->Busy = 0;
    }
    break;
  case SFlexFutureTrend::TypeID:
    {
      CShFlexFutureTrend* sh = (CShFlexFutureTrend*)pSB;
      SFlexFutureTrend* hs = static_cast<SFlexFutureTrend*>(sh);
      memcpy( hs, pB, pSB->m_nSize );
      pSB->Busy = 0;
    }
    break;
  }
  pSB->RetCode = CShBase::rcOK;
  return pSB->m_nSize;
}

template<size_t _S>
void DoubleToStrF( char (&lp)[_S], double d, int nZnak )
{
  if ( nZnak < 0 ) nZnak = 3;
  if ( nZnak > 9 ) nZnak = 9;
  static char sz[6] = "%.3f";
  sz[2] = '0'+nZnak;
  if ( fabs(d) > 1e10)
  {
    static char ssz[6] = "%.3e";
    ssz[2] = '0'+nZnak;
    sprintf_s(lp, ssz, d);
    return;
  }
  sprintf_s( lp, sz,d);
  char* pStr = strchr(lp,'.');
  if ( pStr == NULL ) //Русифицированный формат
    pStr = strchr(lp,',');
  if ( pStr )
  {
    *pStr = '.';//Чтобы избавиться от запятых
    char* pTmp = lp + lstrlen(lp)-1;
    while (pTmp != pStr && *pTmp == '0' ) *(pTmp--) = '\0';
    if ( pTmp == pStr ) *pTmp = '\0';
  }
}

static LPCSTR s_pszPERMISSIVE = ".PERMISSIVE";

extern "C" SERVEREXSHAREDPLUGIN_FLEXRSU_API DWORD PutModelRSU( LPCSTR szName, SSendToModel &send, CEventString &str )
{
  DWORD dwMess = CEventLogType::etUnknown;
  if( static_cast<EDataTypes>(SFlex::TypeID)!=send.eData )
  {
    return dwMess;
  }
  KRsuX pnt;
  int r = pnt.LinkTo( send.szName, send.eData );
  if( r )
  {
    LPCSTR szFiltr = "Не найден объект '%s";
    str.Event( szFiltr, send.szName );
    return CEventLogType::etError;
  }

  /*{
    KRsuX PERMISSIVE;
    char szPERMISSIVE[_countof(send.szName)] = {};
    strcpy_s( szPERMISSIVE, send.szName );
    LPSTR p = strrchr( szPERMISSIVE, '.');
    if( p )
      strcpy_s( p, _countof(szPERMISSIVE) - ((size_t)p-(size_t)szPERMISSIVE), s_pszPERMISSIVE );
    else
      strcat_s( szPERMISSIVE, s_pszPERMISSIVE );
    int r2 = PERMISSIVE.LinkTo( szPERMISSIVE, send.eData );
    if( !r2 )
    {
      int perVal = PERMISSIVE;
      if( !perVal )
      {
        str.Event( "'%s' запрет на установку '%s'", send.szName, send.szValue );
        return CEventLogType::etError;
      }
    }
  } */
  
  dwMess = send.eData;

  switch( send.eType )
  {
  case enumValueStr:
    {
      KRsuF *f = (KRsuF*)&pnt;
      *f = send.szValue;
    }
    break;
  case enumValueInt:
    {
      const int cur = pnt;
      if( cur==send.nNew )
        dwMess = CEventLogType::etSameValue;
      else
      {
        sprintf_s( send.szValue, "%d", send.nNew );
        pnt = send.nNew;
      }
    }
    break;
  case enumValueFlt:
    {
      const float cur = pnt;
      if( cur==send.fNew )
        dwMess = CEventLogType::etSameValue;
      else
      {
        int    Z = 5;
        float F = fabs(send.fNew-cur);
        if ( F > 10.00 ) Z = 0;
        else if ( F > 1.000 ) Z = 1;
        else if ( F > 0.100 ) Z = 2;
        else if ( F > 0.010 ) Z = 3;
        else if ( F > 0.001 ) Z = 4;
        DoubleToStrF( send.szValue, send.fNew, Z );
        char szD2[64]; DoubleToStrF( szD2, send.dOld, Z );
        pnt = send.fNew;
    }
    }
    break;
  case enumValueDbl:
    {
      const double cur = pnt;
      if( cur==send.dNew )
        dwMess = CEventLogType::etSameValue;
      else
      {
        int    Z = 5;
        double F = fabs(send.dNew-cur);
        if ( F > 10.00 ) Z = 0;
        else if ( F > 1.000 ) Z = 1;
        else if ( F > 0.100 ) Z = 2;
        else if ( F > 0.010 ) Z = 3;
        else if ( F > 0.001 ) Z = 4;
        DoubleToStrF( send.szValue, send.dNew, Z );
        char szD2[64]; DoubleToStrF( szD2, send.dOld, Z );
        pnt = send.dNew;
      }
    }
    break;
  default:
    return CEventLogType::etError;
  }
  
  if( KTagSkipFilter::Instance().SkipWriteEvent( send ) )
    dwMess = CEventLogType::etSameValue;

  if( CEventLogType::etSameValue!=dwMess )
  {
    str.Event( "'%s' изменён на '%s'", send.szName, send.szValue );
  }
  return dwMess;
}
