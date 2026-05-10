#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_SR_P
{
#include "PrivateVars/SR.h"
};


class K_SR 
  : public KFlexBlockBase
  , protected S_SR_P
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
  static void IniSRarm();
};

static KBlockCreator _SR("SR", K_SR::Create);

KFlexBlockBase *K_SR::Create()
{
  return new K_SR;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_SR, 3 )
//////////////////////////////////////////////////////////////////////////
void K_SR::IniSRarm()
{
#include "PrivateVars/SR.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_SR::OnCreated()
{
  if( !s_kprivateVarInfo )
    IniSRarm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_SR_P>( this, &s_private );
}

void K_SR::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[0] )
    {
    case 'R':
      (*pIn) >> R;
      break;
    case 'S':
      (*pIn) >> S;
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << Q;
  }
}

KFlexBlockBase::SReturn K_SR::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if( R )
    Q = false;
  if( S )
    Q = true;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    *pOut = Q;
  }
  return sr;
}
