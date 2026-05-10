#include <crossplatform.h>
#include <cmath>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>
#include <float.h>

struct S_SCLR_P
{
#include "PrivateVars/SCLR.h"
};


class K_SCLR 
  : public KFlexBlockBase
  , protected S_SCLR_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_SCLR *W;
  SFlex *pRaW;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _SCLR("SCLR", K_SCLR::Create);

KFlexBlockBase *K_SCLR::Create()
{
  return new K_SCLR;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_SCLR, 8 )
DECLARE_SHARED_VARSINFO( S_SCLR, 4 )
//////////////////////////////////////////////////////////////////////////
void K_SCLR::InitParm()
{
#include <FlexBlocks/SCLR.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/SCLR.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_SCLR::OnCreated()
{
  static int nameCount = 1;
  if (m_szVisibleName[0])
    pRaW = CreateSFlexSharedObj( m_szVisibleName, &W );
  else
  {
    char uniqueVisibleName[128 * 4];
    sprintf(uniqueVisibleName, "Un_SCLR%d", nameCount++);
    CreateSFlexSharedObj(uniqueVisibleName , &W );
  }

  if( !s_kprivateVarInfo && !s_ksharedVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;

  //SetInitialVals<S_SCLR_P>( this, &s_private );
}

void K_SCLR::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> in;
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[0] )
    {
    case 'O':
      (*pOut) << out;
      break;
    case 'E':
      if( 'P'==pOut->m_szSource[3] )
        (*pOut) << EinP;
      if( 'M'==pOut->m_szSource[3] )
        (*pOut) << EinM;
      break;
    }
  }
}

KFlexBlockBase::SReturn K_SCLR::StepCalc( SCallParams &params )
{
  SReturn sr;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if( !std::isfinite( in ) )
  {
    EinM = EinP = 1;
  }
  else
  {
    float inScale = W->IN100-W->IN0;
    float outScale = W->OUT100-W->OUT0;

    if( 0!=inScale && 0!=outScale )
    {
      if( in > W->IN100 )
      {
        EinP = 1;
        in = W->IN100;
      }
      else
        EinP = 0;
      if( in < W->IN0 )
      {
        EinM = 1;
        in = W->IN0;
      }
      else
        EinM = 0;
      tAnalog32 PR = ( in - W->IN0) / inScale;
      out = outScale * PR + W->OUT0;
    }
    else
      EinM = EinP = 1;
  }

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
