#include "BlockCreator.h"
#include "ScriptableBlock.h"
#include <FlexPrivateDeclare.h>

#undef IN

struct S_FD2W_P
{
#include "PrivateVars/FD2W.h"
};


class K_FD2W 
  : public KScriptableBlock
  , protected S_FD2W_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  eVarType m_TransferType;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _FD2W("FD2W", K_FD2W::Create);

KFlexBlockBase *K_FD2W::Create()
{
  return new K_FD2W;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_FD2W, 4 )
//////////////////////////////////////////////////////////////////////////
void K_FD2W::InitParm()
{
#include "PrivateVars/FD2W.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_FD2W::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_FD2W_P>( this, &s_private );
}

void K_FD2W::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    m_TransferType = pIn->GetSourceType();
    switch( pIn->GetSourceType() )
    {
    case evtAnalog32:
      (*pIn) >> inA;
      break;
    case evtDigital32:
      (*pIn) >> inD;
      break;
    default:
      ASSD(0);
      break;
    }
  }

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[1] )
    {
    case '1':
      (*pOut) << W1;
      break;
    case '2':
      (*pOut) << W2;
      break;
    default:
      ASSD(0);
      break;
    }
  }
}

KFlexBlockBase::SReturn K_FD2W::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  DWORD rawIN = 0;
  switch( m_TransferType )
  {
  case evtAnalog32:
    memcpy( &rawIN, &inA, sizeof(rawIN) );
    break;
  case evtDigital32:
    memcpy( &rawIN, &inD, sizeof(rawIN) );
    break;
  default:
    SReturn sr;
  }

  W1 = HIWORD(rawIN);
  W2 = LOWORD(rawIN);


  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }

  return sr;
}
