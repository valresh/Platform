#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_TFw_P
{
#include "PrivateVars/TFw.h"
};


class K_TFw 
  : public KFlexBlockBase
  , protected S_TFw_P
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

static KBlockCreator _TFw("TFw", K_TFw::Create);

KFlexBlockBase *K_TFw::Create()
{
  return new K_TFw;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_TFw, 6 )
//////////////////////////////////////////////////////////////////////////
void K_TFw::IniWParm()
{
#include "PrivateVars/TFw.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TFw::OnCreated()
{
  if( !s_kprivateVarInfo )
    IniWParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_TFw_P>( this, &s_private );
}

void K_TFw::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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
      (*pOut) << ET;
      break;
    }
  }
}

KFlexBlockBase::SReturn K_TFw::StepCalc( SCallParams &params )
{
  SReturn sr;
  tDigital32 prevIn = in;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if( !in )
  {
    ET += params.dtS;
    if( ET >= WAIT )
    {
      out = 1;
      ET = WAIT;
    }
    else
      out = 0;
  }
  else
  {
    out = 0;
    ET = 0;
  }


  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
