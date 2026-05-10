#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_TOFF_P
{
#include "PrivateVars/TOFF.h"
};


class K_TOFF 
  : public KFlexBlockBase
  , protected S_TOFF_P
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

static KBlockCreator _TOFF("TOFF", K_TOFF::Create);

KFlexBlockBase *K_TOFF::Create()
{
  return new K_TOFF;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_TOFF, 6 )
//////////////////////////////////////////////////////////////////////////
void K_TOFF::IniWParm()
{
#include "PrivateVars/TOFF.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TOFF::OnCreated()
{
  if( !s_kprivateVarInfo )
    IniWParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_TOFF_P>( this, &s_private );
}

void K_TOFF::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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
    }
  }
}

KFlexBlockBase::SReturn K_TOFF::StepCalc( SCallParams &params )
{
  SReturn sr;
  tDigital32 prevIn = in;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if( prevIn && !in )
    out = 1;
  else
    out = 0;


  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
