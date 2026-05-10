#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>


class K_SimDI : public KFlexBlockBase
{
  S_SimDI *W;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_SimDI()
  {
    m_nullOrderExec = exnInputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _SimDI("SimDI", K_SimDI::Create);

KFlexBlockBase *K_SimDI::Create()
{
  return new K_SimDI;
}

#include <_InitInfo.h>
DECLARE_SHARED_VARSINFO( S_SimDI, 4 )
//////////////////////////////////////////////////////////////////////////
void K_SimDI::InitParm()
{
#include <FlexBlocks/SimDIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_SimDI::OnCreated()
{
  static int nameCount = 1;
  char uniqueVisibleName[128]; 
  sprintf_s(uniqueVisibleName, "%s/SimDI%d", (LPCSTR)m_szVisibleName, nameCount++);
  CreateSFlexSharedObj(uniqueVisibleName , &W );

  strcpy_s(W->Path, m_szVisibleName);
  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_SimDI::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppOut[i];
    (*pIn) << W->PV;
  }
}

KFlexBlockBase::SReturn K_SimDI::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
