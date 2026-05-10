#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <FlexPrivateDeclare.h>

#include "F_TRIG.h"
#include "RS.h"
#include "TP.h"

struct S_fts10s_P
{
#include "PrivateVars/fts10s.h"
  ns_HIMA::KF_TRIG f_trig[10];
  ns_HIMA::KRS rs[10];
  ns_HIMA::KTP tp;
};

class K_fts10s 
  : public KFlexBlockBase
  , S_fts10s_P
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

static KBlockCreator _fts10s("HIMA:fts10s", K_fts10s::Create);

KFlexBlockBase *K_fts10s::Create()
{
  return new K_fts10s;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_fts10s, 35+30+30+6 )
//////////////////////////////////////////////////////////////////////////
void K_fts10s::InitParm()
{
  static char s_trigNames[_countof(s_private.f_trig)][3][64 * 4] = {};
  for( int i=0; i<_countof(s_private.f_trig); ++i )
  {
    sprintf_s( s_trigNames[i][0], "f_trig[%d].CLK", i );
    PrivateFieldStructArrDigital32Savebale( f_trig, i, CLK, s_trigNames[i][0] );
    sprintf_s( s_trigNames[i][1], "f_trig[%d].Q", i );
    PrivateFieldStructArrDigital32Savebale( f_trig, i, Q, s_trigNames[i][1] );
    sprintf_s( s_trigNames[i][2], "f_trig[%d].FEDGE", i );
    PrivateFieldStructArrDigital32Savebale( f_trig, i, FEDGE, s_trigNames[i][2] );
  }
  static char s_rsNames[_countof(s_private.rs)][3][64 * 4] = {};
  for( int i=0; i<_countof(s_private.rs); ++i )
  {
    sprintf_s( s_rsNames[i][0], "rs[%d].S", i );
    PrivateFieldStructArrDigital32Savebale( rs, i, S, s_rsNames[i][0] );
    sprintf_s( s_rsNames[i][1], "rs[%d].Q1", i );
    PrivateFieldStructArrDigital32Savebale( rs, i, Q1, s_rsNames[i][1] );
    sprintf_s( s_rsNames[i][2], "rs[%d].R1", i );
    PrivateFieldStructArrDigital32Savebale( rs, i, R1, s_rsNames[i][2] );
  }
  PrivateFieldStructDigital32Savebale( tp, in );
  PrivateFieldStructAnalog32Savebale( tp, PT );
  PrivateFieldStructDigital32Savebale( tp, Q );
  PrivateFieldStructAnalog32Savebale( tp, ET );
  PrivateFieldStructAnalog32Savebale( tp, PDATE );
  PrivateFieldStructDigital32Savebale( tp, REDGE );
#include "PrivateVars/fts10s.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_fts10s::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  T_PULS = 2.f;
}


void K_fts10s::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[0] )
    {
    case 'i':
      {
        int index = atoi( pIn->m_szSink+1 );
        switch( index )
        {
        case 1: (*pIn) >> i1; break;
        case 2: (*pIn) >> i2; break;
        case 3: (*pIn) >> i3; break;
        case 4: (*pIn) >> i4; break;
        case 5: (*pIn) >> i5; break;
        case 6: (*pIn) >> i6; break;
        case 7: (*pIn) >> i7; break;
        case 8: (*pIn) >> i8; break;
        case 9: (*pIn) >> i9; break;
        case 10: (*pIn) >> i10; break;
        default:
          ASSD(!"такого входа нет");
          break;
        }
      }
      break;
    case 'e':
      (*pIn) >> enable;
      break;
    case 'r':
      (*pIn) >> res;
      break;
    case 'T':
      (*pIn) >> T_PULS;
      break;
    default:
      ASSD(!"такого входа нет");
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[0] )
    {
    case 'o':
      {
        int index = atoi( pOut->m_szSource+1 );
        switch( index )
        {
        case 1: (*pOut) << o1tr; break;
        case 2: (*pOut) << o2tr; break;
        case 3: (*pOut) << o3tr; break;
        case 4: (*pOut) << o4tr; break;
        case 5: (*pOut) << o5tr; break;
        case 6: (*pOut) << o6tr; break;
        case 7: (*pOut) << o7tr; break;
        case 8: (*pOut) << o8tr; break;
        case 9: (*pOut) << o9tr; break;
        case 10: (*pOut) << o10tr; break;
        default:
          ASSD(!"такого выхода нет");
          break;
        }
      }
      break;
    case 'f':
      (*pOut) << ft_out;
      break;
    default:
      ASSD(!"такого выхода нет");
      break;
    }
  }
}

KFlexBlockBase::SReturn K_fts10s::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  tDigital32 andEnable_Ft = enable && (!ft_out);

  tp.in = res;
  tp.PT = T_PULS;
  tp.Calc( params );

  tDigital32 *pIs = &i1;
  tDigital32 *pOfts = &o1ft;
  tDigital32 *pOtrs = &o1tr;
  tDigital32 ft_outT = 0;
  for( int i=0; i<_countof(f_trig); ++i )
  {
    f_trig[i].CLK = pIs[i];
    f_trig[i].Calc( params );
    pOfts[i] = f_trig[i].Q && andEnable_Ft;
    rs[i].S = pOfts[i];
    rs[i].R1 = tp.Q;
    rs[i].Calc(params);
    pOtrs[i] = rs[i].Q1;
    if( pOtrs[i] )
      ft_outT = 1;
  }
  ft_out = ft_outT;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }

  return sr;
}
