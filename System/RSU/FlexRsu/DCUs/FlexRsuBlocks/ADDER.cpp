#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>

struct S_ADDER_P
{
#include "PrivateVars/ADDER.h"
};

class K_ADDER
  : public KFlexBlockBase
  , protected S_ADDER_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_ADDER *W;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _ADDER("ADDER", K_ADDER::Create);

KFlexBlockBase *K_ADDER::Create()
{
  return new K_ADDER;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_ADDER, 5 )
DECLARE_SHARED_VARSINFO( S_ADDER, 8 )
//////////////////////////////////////////////////////////////////////////
void K_ADDER::InitParm()
{
#include <FlexBlocks/ADDER.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/ADDER.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_ADDER::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_ADDER::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[0] )
    {
    case 'I':
      (*pIn) >> in;
      break;
    case 'R':
      (*pIn) >> RESET;
      break;
    default:
      ASSD(!"такого входа нет");
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    if( 'O'==pOut->m_szSource[0] )
      (*pOut) << W->PV;
    else
    {
        ASSD(!"такого выхода нет");
    }
  }
}

KFlexBlockBase::SReturn K_ADDER::StepCalc( SCallParams &params )
{
  SReturn sr;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  int nAdd = 0;
  waitDT += params.dtS;
  while( waitDT >= DT )
  {
    ++nAdd;
    waitDT -= DT;
    if( DT <= 0.001f )
      DT = params.dtS;
  }

  if( RESET )
    W->PV = 0;
  else if( nAdd )
  {
    for( int i=0; i<nAdd; ++i )
      W->PV += in;
  }

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }

  return sr;
}
