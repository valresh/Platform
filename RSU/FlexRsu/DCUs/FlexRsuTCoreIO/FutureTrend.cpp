#include <BlockCreator.h>
#include <FlexRsuConnect.h>

class K_FUTURETREND : public KFlexBlockBase
{
  SFlexFutureTrend *W;
public:
  K_FUTURETREND()
  {
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _FUTURETREND("FUTURETREND", K_FUTURETREND::Create);

KFlexBlockBase *K_FUTURETREND::Create()
{
  return new K_FUTURETREND;
}

#include <_InitInfo.h>
DECLARE_SHARED_VARSINFO( SFlexFutureTrend, 3610 )
//////////////////////////////////////////////////////////////////////////
void K_FUTURETREND::InitParm()
{
#include <FlexBlocks/FutureTrend.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_FUTURETREND::OnCreated()
{
  CreateAnySharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

KFlexBlockBase::SReturn K_FUTURETREND::StepCalc( SCallParams &params )
{
  SReturn sr;
  return sr;
}
