#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_TW_P
{
#include "PrivateVars/TW.h"
};


class K_TW 
  : public KFlexBlockBase
  , protected S_TW_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void IniWParm();
};

static KBlockCreator _TW("TW", K_TW::Create);

KFlexBlockBase *K_TW::Create()
{
  return new K_TW;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_TW, 6 )
//////////////////////////////////////////////////////////////////////////
void K_TW::IniWParm()
{
#include "PrivateVars/TW.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TW::OnCreated()
{
  if( !s_kprivateVarInfo )
    IniWParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_TW_P>( this, &s_private );
}

void K_TW::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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
      (*pOut) << waiting;
      break;
    }
  }
}

KFlexBlockBase::SReturn K_TW::StepCalc( SCallParams &params )
{
  SReturn sr;
  tDigital32 prevIn = in;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if( in )
  {
    waiting += params.dtS;
    if( waiting >= WAIT )
    {
      out = 1;
      waiting = WAIT;
    }
  }
  else
  {
    out = 0;
    waiting = 0;
  }


  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
