#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_RS_P
{
#include "PrivateVars/RS.h"
};


class K_RS 
  : public KFlexBlockBase
  , protected S_RS_P
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
  static void IniRSarm();
};

static KBlockCreator _RS("RS", K_RS::Create);

KFlexBlockBase *K_RS::Create()
{
  return new K_RS;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_RS, 3 )
//////////////////////////////////////////////////////////////////////////
void K_RS::IniRSarm()
{
#include "PrivateVars/RS.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_RS::OnCreated()
{
  if( !s_kprivateVarInfo )
    IniRSarm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_RS_P>( this, &s_private );
}

void K_RS::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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

KFlexBlockBase::SReturn K_RS::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  
  if( S )
    Q = true;
  if( R )
    Q = false;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    *pOut = Q;
  }
  return sr;
}
