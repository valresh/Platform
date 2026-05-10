#include "BlockCreator.h"
#include "ScriptableBlock.h"
#include <FlexPrivateDeclare.h>

struct S_ACalc_P
{
#include "PrivateVars/ACalc.h"
};


class K_ACalc 
  : public KScriptableBlock
  , protected S_ACalc_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void OnAttributesSetted();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _ACalc("ACalc", K_ACalc::Create);

KFlexBlockBase *K_ACalc::Create()
{
  return new K_ACalc;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_ACalc, 12 )
//////////////////////////////////////////////////////////////////////////
void K_ACalc::InitParm()
{
#include "PrivateVars/ACalc.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_ACalc::OnCreated()
{
  if( !s_kprivateVarInfo )
  {
    InitParm();
    InitSync();
  }

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_ACalc_P>( this, &s_private );
}

void K_ACalc::OnAttributesSetted()
{
  KFlexTranslator trans( this );
  trans.Translate( F1 );
  trans.Translate( F2 );
  trans.Translate( F3 );
  trans.Translate( F4 );
  AllocScriptCode( trans );
}

void K_ACalc::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[0] )
    {
    case 'A':
      (*pIn) >> A;
      break;
    case 'B':
      (*pIn) >> B;
      break;
    case 'C':
      (*pIn) >> C;
      break;
    case 'D':
      (*pIn) >> D;
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
    case '1':
      (*pOut) << O1;
      break;
    case '2':
      (*pOut) << O2;
      break;
    case '3':
      (*pOut) << O3;
      break;
    case '4':
      (*pOut) << O4;
      break;
    default:
      ASSD(!"такого выхода нет");
      break;
    }
  }
}

KFlexBlockBase::SReturn K_ACalc::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  ExecExpression( params );

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }

  return sr;
}
