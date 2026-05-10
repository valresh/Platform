#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>



class K_SimAI : public KFlexBlockBase
{
  S_SimAI *W;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_SimAI()
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

static KBlockCreator _SimAI("SimAI", K_SimAI::Create);

KFlexBlockBase *K_SimAI::Create()
{
  return new K_SimAI;
}

#include <_InitInfo.h>
DECLARE_SHARED_VARSINFO( S_SimAI, 5 )
//////////////////////////////////////////////////////////////////////////
void K_SimAI::InitParm()
{
#include <FlexBlocks/SimAIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_SimAI::OnCreated()
{
  static int nameCount = 1;
  char uniqueVisibleName[128]; 
  sprintf_s(uniqueVisibleName, "%s/SimAI%d", (LPCSTR)m_szVisibleName, nameCount++);
  CreateSFlexSharedObj(uniqueVisibleName , &W );

  strcpy_s(W->Path, m_szVisibleName);
  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_SimAI::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << W->PV;
  }
}

KFlexBlockBase::SReturn K_SimAI::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
