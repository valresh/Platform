#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_Hys_P
{
#include "PrivateVars/Hys.h"
};


class K_Hys 
  : public KFlexBlockBase
  , protected S_Hys_P
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
  SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _Hys("HYS", K_Hys::Create);

KFlexBlockBase *K_Hys::Create()
{
  return new K_Hys;
}

namespace ns_Hys
{
#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_Hys, 10 )
}
using namespace ns_Hys;
//////////////////////////////////////////////////////////////////////////
void K_Hys::InitParm()
{
#include "PrivateVars/Hys.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_Hys::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;
}

void K_Hys::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> in;
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << out;
  }
}

KFlexBlockBase::SReturn K_Hys::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  switch( MODE[0] )
  {
  default:
    return sr;
  case '>':
    switch( MODE[1] )
    {
    case '=':
      if( in >= REFVAL)
        out = 1;
      else if( in < (REFVAL-DELTA) )
        out = 0;
      break;
    case 0:
      if( in > REFVAL)
        out = 1;
      else if( in <= (REFVAL-DELTA) )
        out = 0;
      break;
    }
    break;
  case '<':
    switch( MODE[1] )
    {
    case '=':
      if( in <= REFVAL)
        out = 1;
      else if( in > (REFVAL+DELTA) )
        out = 0;
      break;
    case 0:
      if( in < REFVAL)
        out = 1;
      else if( in >= (REFVAL+DELTA) )
        out = 0;
      break;
    }
    break;
  case '=':
    if( (in > (REFVAL-DELTA)) && (in < (REFVAL+DELTA)) )
      out = 1;
    else if( (in < (REFVAL-DELTA)) || (in > (REFVAL+DELTA)) )
      out = 0;
    break;
  }


  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    *pOut = out;
  }
  return sr;
}
