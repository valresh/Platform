#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_SimAO : public KFlexBlockBase
{
  S_SimAO *W;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  K_SimAO()
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

static KBlockCreator _SimAO("SimAO", K_SimAO::Create);

KFlexBlockBase *K_SimAO::Create()
{
  return new K_SimAO;
}

#include <_InitInfo.h>
DECLARE_SHARED_VARSINFO( S_SimAO, 5 )
//////////////////////////////////////////////////////////////////////////
void K_SimAO::InitParm()
{
#include <FlexBlocks/SimAIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_SimAO::OnCreated()
{
  static int nameCount = 1;
  char uniqueVisibleName[128]; 
  sprintf_s(uniqueVisibleName, "%s/SimAO%d", (LPCSTR)m_szVisibleName, nameCount++);
  CreateSFlexSharedObj(uniqueVisibleName , &W );

  strcpy_s(W->Path, m_szVisibleName);
  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_SimAO::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> W->PV;
  }
}

KFlexBlockBase::SReturn K_SimAO::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  return sr;
}
