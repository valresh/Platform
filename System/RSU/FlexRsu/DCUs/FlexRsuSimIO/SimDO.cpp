#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_SimDO : public KFlexBlockBase
{
  S_SimDO *W;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  K_SimDO()
  {
    m_nullOrderExec = exnOutputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _SimDO("SimDO", K_SimDO::Create);

KFlexBlockBase *K_SimDO::Create()
{
  return new K_SimDO;
}

#include <_InitInfo.h>
DECLARE_SHARED_VARSINFO( S_SimDO, 4 )
//////////////////////////////////////////////////////////////////////////
void K_SimDO::InitParm()
{
#include <FlexBlocks/SimDIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_SimDO::OnCreated()
{
  static int nameCount = 1;
  char uniqueVisibleName[128]; 
  sprintf_s(uniqueVisibleName, "%s/SimDO%d", (LPCSTR)m_szVisibleName, nameCount++);
  CreateSFlexSharedObj(uniqueVisibleName , &W );

  strcpy_s(W->Path, m_szVisibleName);
  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_SimDO::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> W->PV;
  }
}

KFlexBlockBase::SReturn K_SimDO::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  return sr;
}
