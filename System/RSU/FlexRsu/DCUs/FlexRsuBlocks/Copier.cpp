#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_Copier_P
{
#include "PrivateVars/Copier.h"
};

class K_Copier 
  : public KFlexBlockBase
  , protected S_Copier_P
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

static KBlockCreator _Copier("Copier", K_Copier::Create);

KFlexBlockBase *K_Copier::Create()
{
  return new K_Copier;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_Copier, 10 )
//////////////////////////////////////////////////////////////////////////
void K_Copier::InitParm()
{
#include "PrivateVars/Copier.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_Copier::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

}

void K_Copier::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( !strcmp(pIn->m_szSink, "IN") )
      (*pIn) >> inD;
    else if( !strcmp(pIn->m_szSink, "CND") )
      (*pIn) >> cnd;
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << outD;
  }
}

KFlexBlockBase::SReturn K_Copier::StepCalc( SCallParams &params )
{
  tDigital32 prevCnd = cnd;

  if( evtHZ==m_TransferType )
  {
    for( DWORD i=0; i<m_nOuts; ++i )
    {
      KFlexConnection *pOut = m_ppOut[i];
      eVarType eDst = pOut->GetSinkType();
      if( evtHZ==m_TransferType )
      {
        m_TransferType = eDst;
        switch( m_TransferType )
        {
        case evtAnalog32:
          (*pOut) << outA;
          break;
        case evtDigital32:
          (*pOut) << outD;
          break;
        }
      }
      else
      {
        ASS( m_TransferType == eDst );
      }

    }

    for( DWORD i=0; i<m_nIns; ++i )
    {
      KFlexConnection *pIn = m_ppIn[i];
      if( strcmp(pIn->m_szSink, "IN") )
        continue;
      switch( m_TransferType )
      {
      case evtAnalog32:
        (*pIn) >> inA;
        break;
      case evtDigital32:
        (*pIn) >> inD;
        break;
      case evtHZ:
        m_TransferType = pIn->GetSourceType();
        switch( pIn->GetSourceType() )
        {
        case evtAnalog32:
          (*pIn) >> inA;
          break;
        case evtDigital32:
          (*pIn) >> inD;
          break;
        }
        break;
      }
    }
  }

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  bool bTransfer = false;
  switch( mode )
  {
  case 0:
    if( 1==cnd )
      bTransfer = true;
    break;
  case 1:
    if( 0==prevCnd && 1==cnd )
      bTransfer = true;
    break;
  case 2:
    if( 1==prevCnd && 0==cnd )
      bTransfer = true;
    break;
  case 3:
    if( prevCnd != cnd )
      bTransfer = true;
    break;
  case 4:
    if( 0==cnd )
      bTransfer = true;
    break;
  }
  SReturn sr;
  if( bTransfer )
  {
    switch( m_TransferType )
    {
    case evtAnalog32:
      outA = inA;
      break;
    case evtDigital32:
      outD = inD;
      break;
    }

    for( DWORD i=0; i<m_nOuts; ++i )
    {
      KFlexConnection *pOut = m_ppOut[i];
      switch( m_TransferType )
      {
      case evtAnalog32:
        *pOut = outA = inA;
        break;
      case evtDigital32:
        *pOut = outD = inD;
        break;
      }
    }
  }

  return sr;
}
