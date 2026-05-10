#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <FlexPrivateDeclare.h>

#include "RS.h"
#include "TON.h"

struct S_agt3_P
{
#include "PrivateVars/agt3.h"
  ns_HIMA::KRS rs;
  ns_HIMA::KTON ton;
};

class K_agt3 
  : public KFlexBlockBase
  , S_agt3_P
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

static KBlockCreator _agt3("HIMA:agt3", K_agt3::Create);

KFlexBlockBase *K_agt3::Create()
{
  return new K_agt3;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_agt3, 10+10 )
//////////////////////////////////////////////////////////////////////////
void K_agt3::InitParm()
{
  PrivateFieldStructDigital32Savebale( rs, S );
  PrivateFieldStructAnalog32Savebale( rs, Q1 );
  PrivateFieldStructDigital32Savebale( rs, R1 );
  PrivateFieldStructDigital32Savebale( ton, in );
  PrivateFieldStructAnalog32Savebale( ton, PT );
  PrivateFieldStructDigital32Savebale( ton, Q );
  PrivateFieldStructAnalog32Savebale( ton, ET );
  PrivateFieldStructAnalog32Savebale( ton, PDATE );
  PrivateFieldStructDigital32Savebale( ton, FEDGE );
#include "PrivateVars/agt3.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_agt3::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  ton.PT = 2;
}


void K_agt3::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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
    case 'h':
      switch( pIn->m_szSink[2] )
      {
      case 'o':
        (*pIn) >> h_ok_l;
        break;
      case 't':
        (*pIn) >> h_t_l;
        break;
      default:
        ASSD(!"такого входа нет");
        break;
      }
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
    case 'l':
      (*pOut) << flt;
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

KFlexBlockBase::SReturn K_agt3::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  tAnalog32 minus_ok = ok_l - h_ok_l;
  tAnalog32 plus_t = t_l + h_t_l;

  tDigital32 smaller1 = in <= plus_t;

  rs.S = in >= minus_ok;

  ton.in = (in<minus_ok) && (in>plus_t);
  ton.Calc( params );
  rs.R1 = smaller1 || ton.Q;
  rs.Calc( params );
  flt = ton.Q;
  o = rs.Q1;
  out = n_e && rs.Q1;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
