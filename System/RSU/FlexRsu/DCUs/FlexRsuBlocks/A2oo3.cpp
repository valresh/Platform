#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_A2oo3
  : public KFlexBlockBase
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_A2oo3 *W;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _A2oo3("A2oo3", K_A2oo3::Create);

KFlexBlockBase *K_A2oo3::Create()
{
  return new K_A2oo3;
}

#include <_InitInfo.h>
//DECLARE_PRIVATE_VARSINFO( K_A2oo3, 5 )
DECLARE_SHARED_VARSINFO( S_A2oo3, 8 )
//////////////////////////////////////////////////////////////////////////

void K_A2oo3::InitParm()
{
    #include <FlexBlocks/A2oo3.h>
    qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
/*#include "PrivateVars/A2oo3.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );*/
}

void K_A2oo3::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( /*!s_kprivateVarInfo &&*/ !s_ksharedVarInfo )
    InitParm();

  /*m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;*/

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_A2oo3::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[2] )
    {
    case '1':
      (*pIn) >> W->IN1;
      break;
    case '2':
      (*pIn) >> W->IN2;
      break;
    case '3':
      (*pIn) >> W->IN3;
      break;
    default:
      ASSD(!"такого входа нет");
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    if( 'P'==pOut->m_szSource[0] )
      (*pOut) << W->PV;
    else
    {
      switch( pOut->m_szSource[3] )
      {
      case '1':
        (*pOut) << W->BAD1CH;
        break;
      case '2':
        (*pOut) << W->BAD2CH;
        break;
      case 'A':
        (*pOut) << W->BADALL;
        break;
      default:
        ASSD(!"такого выхода нет");
        break;
      }
    }
  }
}

KFlexBlockBase::SReturn K_A2oo3::StepCalc( SCallParams &params )
{
  SReturn sr;

  int nI = 0;
  tAnalog32 r = 0;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( !pIn->GetData() )
      continue;
    switch( pIn->m_szSink[2] )
    {
    case '1':
      r += W->IN1;
      break;
    case '2':
      r += W->IN2;
      break;
    case '3':
      r += W->IN3;
      break;
    default:
      continue;
    }
    ++nI;
  }

  if( 3==nI )
  {
    W->PV = r / nI;
    W->BADALL = W->BAD1CH = W->BAD2CH = 0;
  }
  else if( 2==nI )
  {
    W->PV = r / nI;
    W->BADALL = W->BAD2CH = 0;
    W->BAD1CH = 1;
  }
  else if( 1==nI )
  {
    W->PV = r / nI;
    W->BADALL = W->BAD1CH = 0;
    W->BAD2CH = 1;
  }
  else
  {
    W->PV = 0;
    W->BADALL = W->BAD1CH = W->BAD2CH = 1;
  }

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }

  return sr;
}
