#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_OREF_P
{
#include "PrivateVars/OREF.h"
};


class K_OREF 
  : public KFlexBlockBase
  , protected S_OREF_P
{
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  K_OREF()
  {
    m_nullOrderExec = exnOutputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual void OnProjectReady( IProjectContainer *pCont );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _OREF("OREF", K_OREF::Create);

KFlexBlockBase *K_OREF::Create()
{
  return new K_OREF;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_OREF, 4 )
//////////////////////////////////////////////////////////////////////////
void K_OREF::InitParm()
{
#include "PrivateVars/OREF.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_OREF::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
}

void K_OREF::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
}

void K_OREF::OnProjectReady( IProjectContainer *pCont )
{
  eVarType type = evtHZ;
  BYTE *pMem = pCont->GetTagField( (LPSTR)(LPCSTR)REF, &type );
  if( !pMem )
    return;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( type )
    {
    case evtAnalog32:
      (*pIn) >> *(tAnalog32*)pMem;
      break;
    case evtDigital32:
      (*pIn) >> *(tDigital32*)pMem;
      break;
    }
  }
}

KFlexBlockBase::SReturn K_OREF::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  return sr;
}
