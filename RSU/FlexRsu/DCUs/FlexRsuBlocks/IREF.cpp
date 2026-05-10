#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_IREF_P
{
#include "PrivateVars/IREF.h"
};


class K_IREF 
  : public KFlexBlockBase
  , protected S_IREF_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_IREF()
  {
    m_nullOrderExec = exnInputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual void OnProjectReady( IProjectContainer *pCont );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _IREF("IREF", K_IREF::Create);

KFlexBlockBase *K_IREF::Create()
{
  return new K_IREF;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_IREF, 4 )
//////////////////////////////////////////////////////////////////////////
void K_IREF::InitParm()
{
#include "PrivateVars/IREF.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_IREF::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
}

void K_IREF::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
}

void K_IREF::OnProjectReady( IProjectContainer *pCont )
{
  eVarType type = evtHZ;
  BYTE *pMem = pCont->GetTagField( (LPSTR)(LPCSTR)REF, &type );
  if( !pMem )
    return;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( type )
    {
    case evtAnalog32:
      (*pOut) << *(tAnalog32*)pMem;
      break;
    case evtDigital32:
      (*pOut) << *(tDigital32*)pMem;
      break;
    }
  }
}

KFlexBlockBase::SReturn K_IREF::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
