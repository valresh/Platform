#include "stdafx.h"
#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_iTNwi_P
{
#include "PrivateVars/iTNwi.h"
};


class K_iTNwi 
  : public KFlexBlockBase
  , protected S_iTNwi_P
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

static KBlockCreator _iTNwi("iTNwi", K_iTNwi::Create);

KFlexBlockBase *K_iTNwi::Create()
{
  return new K_iTNwi;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_iTNwi, 6 )
//////////////////////////////////////////////////////////////////////////
void K_iTNwi::IniWParm()
{
#include "PrivateVars/iTNwi.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_iTNwi::OnCreated()
{
  if( !s_kprivateVarInfo )
    IniWParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_iTNwi_P>( this, &s_private );
}

void K_iTNwi::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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

KFlexBlockBase::SReturn K_iTNwi::StepCalc( SCallParams &params )
{
  SReturn sr;
  tDigital32 prevIn = in;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  if (!prevIn && in)
  {
    ACT = 1;
  }

  if( ACT == 1 )
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
