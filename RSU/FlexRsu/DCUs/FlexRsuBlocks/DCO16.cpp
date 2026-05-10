#include <crossplatform.h>
#include "BlockCreator.h"
#include "ScriptableBlock.h"
#include <FlexPrivateDeclare.h>
#include <SharedFlexBlocks.h>
#include <FlexRsuConnect.h>

#undef IN

struct S_DCO16_P
{
#include "PrivateVars/DCO16.h"
};


class K_DCO16 
  : public KScriptableBlock
  , protected S_DCO16_P
{
  S_DCO16 *W;
  SFlex *pRaW;
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

static KBlockCreator _DCO16("DCO16", K_DCO16::Create);

KFlexBlockBase *K_DCO16::Create()
{
  return new K_DCO16;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_DCO16, 20 )
DECLARE_SHARED_VARSINFO( S_DCO16, 16 )
//////////////////////////////////////////////////////////////////////////
void K_DCO16::InitParm()
{
#include "PrivateVars/DCO16.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include <FlexBlocks/DCO16.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_DCO16::OnCreated()
{
  static int nameCount = 1;
  if( m_szVisibleName[0] )
    pRaW = CreateSFlexSharedObj( m_szVisibleName, &W );
  else
  {
    char uniqueVisibleName[512]; 
    sprintf(uniqueVisibleName, "DCO16_%d", nameCount++);
    pRaW = CreateSFlexSharedObj(uniqueVisibleName , &W );
  }

  if( !s_kprivateVarInfo && !s_ksharedVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_DCO16::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> IN;
  }
  
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[3] )
    {
    case '1':
      switch( pOut->m_szSource[4] )
      {
      case '0':
        (*pOut) << W->BIT10;
        break;
      case '1':
        (*pOut) << W->BIT11;
        break;
      case '2':
        (*pOut) << W->BIT12;
        break;
      case '3':
        (*pOut) << W->BIT13;
        break;
      case '4':
        (*pOut) << W->BIT14;
        break;
      case '5':
        (*pOut) << W->BIT15;
        break;
      case '6':
        (*pOut) << W->BIT16;
        break;
      case 0:
        (*pOut) << W->BIT1;
        break;
      default:
        ASSD(0);
        break;
      }
      break;
    case '2':
      (*pOut) << W->BIT2;
      break;
    case '3':
      (*pOut) << W->BIT3;
      break;
    case '4':
      (*pOut) << W->BIT4;
      break;
    case '5':
      (*pOut) << W->BIT5;
      break;
    case '6':
      (*pOut) << W->BIT6;
      break;
    case '7':
      (*pOut) << W->BIT7;
      break;
    case '8':
      (*pOut) << W->BIT8;
      break;
    case '9':
      (*pOut) << W->BIT9;
      break;
    default:
      ASSD(0);
      break;
    }
  }
}

KFlexBlockBase::SReturn K_DCO16::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  tDigital32 *pBit[] = { &W->BIT1, &W->BIT2, &W->BIT3, &W->BIT4, &W->BIT5, &W->BIT6, &W->BIT7, &W->BIT8, &W->BIT9, &W->BIT10, &W->BIT11, &W->BIT12, &W->BIT13, &W->BIT14, &W->BIT15, &W->BIT16 };

  int MASK = 1;
  for( int i=0; i<_countof(pBit); ++i )
  {
    if( IN&MASK )
      *pBit[i] = 1;
    else
      *pBit[i] = 0;
    MASK = MASK << 1;
  }


  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }

  return sr;
}
