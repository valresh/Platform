#include <BlockCreator.h>
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>

class K_TCOREMAI : public KFlexBlockBase
{
  S_TCOREMAI *W;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_TCOREMAI()
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

static KBlockCreator _AI("TCOREMAI", K_TCOREMAI::Create);

KFlexBlockBase *K_TCOREMAI::Create()
{
  return new K_TCOREMAI;
}

#include <_InitInfo.h>
DECLARE_SHARED_VARSINFO( S_TCOREMAI, 10 )
//////////////////////////////////////////////////////////////////////////
void K_TCOREMAI::InitParm()
{
#include <FlexBlocks/TCoreMAIO.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TCOREMAI::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
  W->GAIN = 1.;
}

void K_TCOREMAI::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    if( pOut->m_szSource=="OUT" )
      (*pOut) << W->PV32;
    else if( pOut->m_szSource=="QUALITY" )
      (*pOut) << W->QUALITY;
    else if( pOut->m_szSource=="OVPLUS" )
      (*pOut) << W->OVPLUS;
    else if( pOut->m_szSource=="OVMINUS" )
      (*pOut) << W->OVMINUS;
  }
}

KFlexBlockBase::SReturn K_TCOREMAI::StepCalc( SCallParams &params )
{
  SReturn sr;

  tAnalog64 v = W->RAW64 * W->GAIN;
  W->PV32 = (tAnalog32)v;

  if( v > 16776999.f )
    W->OVPLUS = 1;
  else
    W->OVPLUS = 0;
  if( v < -16776999.f )
    W->OVMINUS = 1;
  else
    W->OVMINUS = 0;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
