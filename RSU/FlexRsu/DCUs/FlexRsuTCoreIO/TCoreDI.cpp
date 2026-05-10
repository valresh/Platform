#include <BlockCreator.h>
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_TCOREDI : public KFlexBlockBase
{
  S_TCOREDI *W;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_TCOREDI()
  {
    m_nullOrderExec = exnInputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _DI("TCoreDI", K_TCOREDI::Create);

KFlexBlockBase *K_TCOREDI::Create()
{
  return new K_TCOREDI;
}

#include <_InitInfo.h>
DECLARE_SHARED_VARSINFO( S_TCOREDI, 7 )
//////////////////////////////////////////////////////////////////////////
void K_TCOREDI::InitParm()
{
#include <FlexBlocks/TCoreDIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TCOREDI::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_TCOREDI::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    if( pOut->m_szSource=="OUT" )
      (*pOut) << W->PV;
    else if( pOut->m_szSource=="QUALITY" )
      (*pOut) << W->QUALITY;
  }
}

KFlexBlockBase::SReturn K_TCOREDI::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
