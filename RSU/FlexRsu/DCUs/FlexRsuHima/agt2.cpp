#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <FlexPrivateDeclare.h>

#include "RS.h"

struct S_agt2_P
{
#include "PrivateVars/agt2.h"
  ns_HIMA::KRS rs;
};

class K_agt2 
  : public KFlexBlockBase
  , S_agt2_P
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

static KBlockCreator _agt2("HIMA:agt2", K_agt2::Create);

KFlexBlockBase *K_agt2::Create()
{
  return new K_agt2;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_agt2, 10+5 )
//////////////////////////////////////////////////////////////////////////
void K_agt2::InitParm()
{
  PrivateFieldStructDigital32Savebale( rs, S );
  PrivateFieldStructAnalog32Savebale( rs, Q1 );
  PrivateFieldStructDigital32Savebale( rs, R1 );
#include "PrivateVars/agt2.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_agt2::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
}


void K_agt2::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[0] )
    {
    case 'i':
      (*pIn) >> in;
      break;
    case 'o':
      (*pIn) >> ok_l;
      break;
    case 't':
      (*pIn) >> t_l;
      break;
    case 'n':
      (*pIn) >> n_e;
      break;
    default:
      ASSD(!"такого входа нет");
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[1] )
    {
    case 0:
      (*pOut) << o;
      break;
    case 'u':
      (*pOut) << out;
      break;
    default:
      ASSD(!"такого выхода нет");
      break;
    }
  }
}

KFlexBlockBase::SReturn K_agt2::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  rs.S = in > ok_l;
  rs.R1 = ok_l <= t_l;
  rs.Calc( params );
  o = rs.Q1;
  out = rs.Q1 && n_e;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
