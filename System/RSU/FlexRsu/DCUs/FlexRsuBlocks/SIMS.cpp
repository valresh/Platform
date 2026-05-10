#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <FlexPrivateDeclare.h>
#include <math.h>

struct S_SIMS_P
{
#include "PrivateVars/SIMS.h"
};


class K_SIMS
  : public KFlexBlockBase
  , protected S_SIMS_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_SIMS()
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

static KBlockCreator _SIMS("SIMS", K_SIMS::Create);

KFlexBlockBase *K_SIMS::Create()
{
  return new K_SIMS;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_SIMS, 5 )
//////////////////////////////////////////////////////////////////////////
void K_SIMS::InitParm()
{
#include "PrivateVars/SIMS.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_SIMS::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
  OVER = STEP = FW = 0;
}

void K_SIMS::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[0] )
    {
    case 'O':
      (*pOut) << OVER;
      break;
    case 'S':
      (*pOut) << STEP;
      break;
    case 'F':
      (*pOut) << FW;
      break;
    default:
      ASSD(!"такого выхода нет");
      break;
    }
  }
}

KFlexBlockBase::SReturn K_SIMS::StepCalc( SCallParams &params )
{
  SReturn sr;

  if( !OVER )
  {
    if( STEP < WAIT )
      FW = 1;
    else
      FW = 0;
  }

  if( INT_MAX == STEP || STEP < 0 )
  {
    OVER = 1;
    STEP = 0;
  }
  else
    ++STEP;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }

  return sr;
}
