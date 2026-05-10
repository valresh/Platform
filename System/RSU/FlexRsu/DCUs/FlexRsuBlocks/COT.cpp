#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>
#include <math.h>

struct S_COT_P
{
#include "PrivateVars/COT.h"
};


class K_COT
  : public KFlexBlockBase
  , protected S_COT_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_COT *W;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _COT("COT", K_COT::Create);

KFlexBlockBase *K_COT::Create()
{
  return new K_COT;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_COT, 5 )
DECLARE_SHARED_VARSINFO( S_COT, 8 )
//////////////////////////////////////////////////////////////////////////
void K_COT::InitParm()
{
#include <FlexBlocks/COT.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/COT.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_COT::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_kprivateVarInfo && !s_ksharedVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_COT::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[0] )
    {
    case 'R':
      (*pIn) >> RESET;
      break;
    case 'C':
      (*pIn) >> CONTINUE;
      break;
    default:
      ASSD(!"такого входа нет");
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[0] )
    {
    case 'I':
      (*pOut) << W->INWORK;
      break;
    case 'D':
      (*pOut) << W->DAY;
      break;
    case 'H':
      (*pOut) << W->HOUR;
      break;
    case 'M':
      (*pOut) << W->MINUTE;
      break;
    case 'S':
      if( 'T'==pOut->m_szSource[1] )
        (*pOut) << W->STARTCOUNTER;
      if( 'E'==pOut->m_szSource[1] )
        (*pOut) << W->SECOND;
      break;
    case 'A':
      (*pOut) << W->AHOUR;
      break;
    default:
      ASSD(!"такого выхода нет");
      break;
    }
  }
}

struct S_COT_Impl : S_COT
{
  void Convert();
};

KFlexBlockBase::SReturn K_COT::StepCalc( SCallParams &params )
{
  SReturn sr;

  tDigital32 curCONTINUE = CONTINUE;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if( RESET )
  {
    W->INWORK = 0;
    W->AHOUR = 0;
    W->STARTCOUNTER = 0;
  }
  else
  {
    if( CONTINUE )
    {
      if( !curCONTINUE )
        ++W->STARTCOUNTER;
      W->INWORK = 1;
      W->AHOUR += (params.dtS / 3600.f );
    }
    else
      W->INWORK = 0;
  }

  S_COT_Impl *pImpl = (S_COT_Impl*)W;
  pImpl->Convert();

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }

  return sr;
}

void S_COT_Impl::Convert()
{
  int ihour = (int)AHOUR;
  tAnalog32 n = 0;
  tAnalog32 h = modff( AHOUR, &n );
  int iN = (int)n;
  int s3599 = int(h * 3600.f);
  MINUTE = s3599 / 60;
  SECOND = s3599 - MINUTE*60;
  DAY = iN / 24;
  HOUR = iN - DAY*24;
}
