#include <BlockCreator.h>
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>


class K_TCOREAI : public KFlexBlockBase
{
  S_TCOREAI *W;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_TCOREAI()
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

static KBlockCreator _AI("TCoreAI", K_TCOREAI::Create);

KFlexBlockBase *K_TCOREAI::Create()
{
  return new K_TCOREAI;
}

#include <_InitInfo.h>
DECLARE_SHARED_VARSINFO( S_TCOREAI, 7 )
//////////////////////////////////////////////////////////////////////////
void K_TCOREAI::InitParm()
{
#include <FlexBlocks/TCoreAIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TCOREAI::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_TCOREAI::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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

KFlexBlockBase::SReturn K_TCOREAI::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
