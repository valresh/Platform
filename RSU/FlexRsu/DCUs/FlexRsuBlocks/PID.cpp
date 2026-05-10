#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>

struct S_PID_P
{
#include "PrivateVars/PID.h"
};


class K_PID 
  : public KFlexBlockBase
  , protected S_PID_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_PID *W;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _PID("PID", K_PID::Create);

KFlexBlockBase *K_PID::Create()
{
  return new K_PID;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_PID, 5 )
DECLARE_SHARED_VARSINFO( S_PID, 15 )
//////////////////////////////////////////////////////////////////////////
void K_PID::InitParm()
{
#include <FlexBlocks/PID.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/PID.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_PID::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_kprivateVarInfo && !s_ksharedVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_PID::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[0] )
    {
    case 'P':
      (*pIn) >> W->PV;
      break;
    /*case 'M':
      (*pIn) >> W->MV;
      break;
    case 'S':
      (*pIn) >> W->SV;
      break;*/
    default:
      ASSD(!"такого входа нет");
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << W->CV;
  }
}

namespace ns_PID
{
  tAnalog32 toParcent( tAnalog32 v, tAnalog32 h, tAnalog32 l )
  {
    return ( v - l ) / ( h - l );
  }
};

KFlexBlockBase::SReturn K_PID::StepCalc( SCallParams &params )
{
  SReturn sr;
  tAnalog32 En1 = ns_PID::toParcent(W->PV, W->MH, W->ML) - ns_PID::toParcent(W->SP, W->SH, W->SL);

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  tAnalog32 En = ns_PID::toParcent(W->PV, W->MH, W->ML) - ns_PID::toParcent(W->SP, W->SH, W->SL);
  if( 1==W->MA )
  {
    tAnalog32 Ks = ( W->MH - W->ML ) / ( W->SH - W->SL );
    tAnalog32 Kp = 100.f / W->P;
    tAnalog32 dEn = En - En1;
    tAnalog32 dMV = Kp * Ks * ( dEn + params.dtS / W->I * En );
    //
    if ( Inverse )
      dMV = -dMV;
    W->CV += dMV;
    if ( W->CV < W->ML )
      W->CV = W->ML;
    if ( W->CV > W->MH )
      W->CV = W->MH;
    W->MV = W->CV;
  }
  else
    W->CV = W->MV;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
