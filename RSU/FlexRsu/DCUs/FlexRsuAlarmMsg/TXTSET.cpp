#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_TxtSet
  : public KFlexBlockBase
{
  S_TXTSET *W;
public:
  K_TxtSet()
  {
    m_nullOrderExec = exnOutputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _TXTSET("TXTSET", K_TxtSet::Create);

KFlexBlockBase *K_TxtSet::Create()
{
  return new K_TxtSet;
}

namespace ns_TXTSET
{
#include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_TXTSET, 34 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_TXTSET;
void K_TxtSet::InitParm()
{
#include <FlexBlocks/TXTSET.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TxtSet::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

typedef tChars tText_[512];

KFlexBlockBase::SReturn K_TxtSet::StepCalc( SCallParams &params )
{
  SReturn sr;

  if( W->SEL_TEXT>0 && W->SEL_TEXT<33 )
  {
    tText_ * txt = &W->TEXT1;
    strcpy_s( W->TEXT_OUT, txt[W->SEL_TEXT-1] );
    
  }
  else if( 0 == W->SEL_TEXT )
  {
    W->TEXT_OUT[0] = 0;//    ZeroMemory( W->TEXT_OUT, sizeof(W->TEXT_OUT) );
  }
  else
  {
    strcpy_s( W->TEXT_OUT, "Ошибочный индекс" );
  }

  return sr;
}
