#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <FlexPrivateDeclare.h>

#include "SR.h"

struct S_qza_P
{
#include "PrivateVars/qza.h"
  ns_HIMA::KSR sr[2];
};

class K_qza 
  : public KFlexBlockBase
  , S_qza_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _qza("HIMA:qza", K_qza::Create);

KFlexBlockBase *K_qza::Create()
{
  return new K_qza;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_qza, 5+10 )
//////////////////////////////////////////////////////////////////////////
void K_qza::InitParm()
{
  static char s_srNames[_countof(s_private.sr)][3][64 * 4] = {};
  for( int i=0; i<_countof(s_private.sr); ++i )
  {
    sprintf_s( s_srNames[i][0], "sr[%d].S", i );
    PrivateFieldStructArrDigital32Savebale( sr, i, S1, s_srNames[i][0] );
    sprintf_s( s_srNames[i][1], "sr[%d].Q1", i );
    PrivateFieldStructArrDigital32Savebale( sr, i, Q1, s_srNames[i][1] );
    sprintf_s( s_srNames[i][2], "sr[%d].R1", i );
    PrivateFieldStructArrDigital32Savebale( sr, i, R, s_srNames[i][2] );
  }
#include "PrivateVars/qza.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_qza::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
}


void K_qza::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[4] )
    {
    case 'A':
      (*pIn) >> Pre_Al;
      break;
    case 'F':
    case 'R':
      (*pIn) >> Pre_Flt;
      break;
    case 't':
      (*pIn) >> Reset;
      break;
    default:
      ASSD(!"такого входа нет");
      break;
    }
  }
  
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[4] )
    {
    case 'A':
      (*pOut) << Pre_Al_o;
      break;
    case 'F':
    case 'R':
      (*pOut) << Pre_Flt_o;
      break;
    default:
      ASSD(!"такого выхода нет");
      break;
    }
  }
}

KFlexBlockBase::SReturn K_qza::StepCalc( SCallParams &params )
{
  SReturn ret;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  sr[0].S1 = Pre_Al;
  sr[0].R = Reset;
  sr[1].S1 = Pre_Flt;
  sr[1].R = Reset;
  sr[0].Calc( params );
  sr[1].Calc( params );
  Pre_Al_o = sr[0].Q1;
  Pre_Flt_o = sr[1].Q1;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }

  return ret;
}
