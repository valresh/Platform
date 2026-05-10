#include "stdafx.h"
#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_iTFwi_P
{
#include "PrivateVars/iTFwi.h"
};


class K_iTFwi 
  : public KFlexBlockBase
  , protected S_iTFwi_P
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

static KBlockCreator _iTFwi("iTFwi", K_iTFwi::Create);

KFlexBlockBase *K_iTFwi::Create()
{
  return new K_iTFwi;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_iTFwi, 6 )
//////////////////////////////////////////////////////////////////////////
void K_iTFwi::IniWParm()
{
#include "PrivateVars/iTFwi.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_iTFwi::OnCreated()
{
  if( !s_kprivateVarInfo )
    IniWParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_iTFwi_P>( this, &s_private );
}

void K_iTFwi::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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

KFlexBlockBase::SReturn K_iTFwi::StepCalc( SCallParams &params )
{
  SReturn sr;
  tDigital32 prevIn = in;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  if ( prevIn && !in )
  {
    ACT = 1;
  }

  if( ACT )
  {
    ET += params.dtS;
    if( ET >= WAIT )
    {
      out = 1;
      ET = WAIT;
      ACT = 0;
    }
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
