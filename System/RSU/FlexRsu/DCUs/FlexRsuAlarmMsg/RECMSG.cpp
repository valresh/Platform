#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <FlexPrivateDeclare.h>
#include <math.h>

struct S_RECMSG_P
{
#include "PrivateVars/RECMSG.h"
};

class K_RECMSG 
  : public KFlexBlockBase
  , protected S_RECMSG_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  eVarType m_typeP1, m_typeP2;
public:
  K_RECMSG()
  {
    m_nullOrderExec = exnOutputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _RECMSG("RECMSG", K_RECMSG::Create);

KFlexBlockBase *K_RECMSG::Create()
{
  return new K_RECMSG;
}

namespace ns_RECMSG
{
  #include <_InitInfo.h>
  DECLARE_PRIVATE_VARSINFO( K_RECMSG, 10 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_RECMSG;
void K_RECMSG::InitParm()
{
#include "PrivateVars/RECMSG.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_RECMSG::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

}

void K_RECMSG::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( !strcmp(pIn->m_szSink, "IN") )
      (*pIn) >> CLK;
    else if( !strcmp(pIn->m_szSink, "P1") )
      (*pIn) >> iP1;
    else if( !strcmp(pIn->m_szSink, "P2") )
      (*pIn) >> iP2;
  }
}


template<size_t _S>
void FltToStr( char (&szBuf)[_S], tAnalog32 d )
{
  double ab = fabs(d);
  if ( ab < 0.001 || ab > 100000 )
  {
    if ( d == 0 )
      sprintf_s(szBuf,"0");
    else
      sprintf_s(szBuf,"%.1e",d);
  }
  else
  {
    //
    char szFrm[8] = "%.5f";
    /*if ( ab > 100 ) 
      szFrm[2] = '0';
    else if ( ab >  10 )
      szFrm[2] = '1';
    else if ( ab >   1 )
      szFrm[2] = '2';*/
    //
    sprintf_s( szBuf, szFrm, d );
    char* pStr = strchr(szBuf,'.');
    if( pStr == NULL ) //Русифицированный формат
      pStr = strchr(szBuf,',');
    if( pStr )
    {
      *pStr = '.';//Чтобы избавиться от запятых
      char* pTmp = szBuf + strlen(szBuf)-1;
      while( pTmp != pStr && *pTmp == '0' )
        *(pTmp--) = '\0';
      if( pTmp == pStr )
        *pTmp = '\0';
    }
  }
}

template<size_t _S>
void GetText( char (&szBuf)[_S], eVarType type, tAnalog32 ap, tDigital32 ip )
{
  switch( type )
  {
  case evtDigital32:
      std::snprintf( szBuf, _S, "%d", ip );
    break;
  case evtAnalog32:
    //FltToStr( szBuf, ap );
      std::snprintf( szBuf, _S, "%.2f", ap );
    break;
  default:
    szBuf[0] = 0;
  }
}

template<size_t _S>
void StrPX( char (&str)[_S], LPCSTR P, LPCSTR val )
{
  LPSTR btr = strstr( str, P );
  if( btr == NULL ) 
    return;
  *btr = 0;
  char* etr = btr + strlen(P);

  char out[_S] = {};
  sprintf( out, "%s%s%s", str, val, etr );
  strcpy_s( str, out);
}

KFlexBlockBase::SReturn K_RECMSG::StepCalc( SCallParams &params )
{
  tDigital32 prevCLK = CLK;

  tAnalog32 *aps[] = { &aP1, &aP2 };
  tDigital32 *ips[] = { &iP1, &iP2 };
  eVarType *types[] = { &m_typeP1, &m_typeP2 };

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[1] )
    {
    case '1':
    case '2':
      break;
    default:
      continue;
    }
    int index = pIn->m_szSink[1] - '1';
    *types[index] = pIn->GetSourceType();
    switch( *types[index] )
    {
    case evtAnalog32:
      (*pIn) >> *aps[index];
      break;
    case evtDigital32:
      (*pIn) >> *ips[index];
      break;
    }
  }

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if( !prevCLK && CLK )
  {
    char param1[64] = {}, param2[64] = {};
    GetText( param1, *types[0], *aps[0], *ips[0] );
    GetText( param2, *types[1], *aps[1], *ips[1] );

    char str[_countof(SHORTTEXT)*2] = {};
    strcpy_s( str, SHORTTEXT );
    StrPX( str, "%P1", param1 );
    StrPX( str, "%P2", param2 );
    if( pFlexM2ULogging )
      pFlexM2ULogging( m_szVisibleName, str, -1, params.modelTime, params.pszPrjObjName, params.pszAuditDir );
    if( pFlexM2ULogging )
      pFlexM2ULogging( m_szVisibleName, str, -2, params.modelTime, params.pszPrjObjName, params.pszAuditDir );
  }
  
  SReturn sr;
  return sr;
}
