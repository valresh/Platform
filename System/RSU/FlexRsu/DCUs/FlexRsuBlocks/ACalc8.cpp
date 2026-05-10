#include "BlockCreator.h"
#include "ScriptableBlock.h"
#include <FlexPrivateDeclare.h>

struct S_ACalc8_P
{
#include "PrivateVars/ACalc8.h"
};


class K_ACalc8 
  : public KScriptableBlock
  , protected S_ACalc8_P
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

static KBlockCreator _ACalc8("ACalc8", K_ACalc8::Create);

KFlexBlockBase *K_ACalc8::Create()
{
  return new K_ACalc8;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_ACalc8, 24 )
//////////////////////////////////////////////////////////////////////////
void K_ACalc8::InitParm()
{
#include "PrivateVars/ACalc8.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_ACalc8::OnCreated()
{
  if( !s_kprivateVarInfo )
  {
    InitParm();
    InitSync();
  }

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_ACalc8_P>( this, &s_private );
}

void K_ACalc8::OnAttributesSetted()
{
  KFlexTranslator trans( this );
  trans.Translate( F1 );
  trans.Translate( F2 );
  trans.Translate( F3 );
  trans.Translate( F4 );
  trans.Translate( F5 );
  trans.Translate( F6 );
  trans.Translate( F7 );
  trans.Translate( F8 );
  AllocScriptCode( trans );
}

void K_ACalc8::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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
    case 'E':
      (*pIn) >> E;
      break;
    case 'F':
      (*pIn) >> F;
      break;
    case 'G':
      (*pIn) >> G;
      break;
    case 'H':
      (*pIn) >> H;
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
    case '5':
      (*pOut) << O5;
      break;
    case '6':
      (*pOut) << O6;
      break;
    case '7':
      (*pOut) << O7;
      break;
    case '8':
      (*pOut) << O8;
      break;
    default:
      ASSD(!"такого выхода нет");
      break;
    }
  }
}

KFlexBlockBase::SReturn K_ACalc8::StepCalc( SCallParams &params )
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
