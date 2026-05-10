#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>
#include <float.h>
#include <math.h>


struct S_EV_P
{
#include "PrivateVars/EV.h"
};


class K_EV 
  : public KFlexBlockBase
  , protected S_EV_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_EV *W;
  SFlex *pRaW;
  int nPERMISSIVE;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _EV("EV", K_EV::Create);

KFlexBlockBase *K_EV::Create()
{
  return new K_EV;
}
// синие
static LPCSTR s_pszAlgOnAlarm  = "$Blue.Выведен из алгоритма";
static LPCSTR s_pszAlgOffAlarm  = "$Blue.Введен в алгоритм";
static LPCSTR s_pszRepairOnAlarm  = "$Blue.Вывод в ремонт";
static LPCSTR s_pszRepairOffAlarm  = "$Blue.Вывод из ремонта";
static LPCSTR s_pszDistModeAlarm  = "$Blue.Включить Дистанционный режим";
static LPCSTR s_pszAutoModeAlarm  = "$Blue.Включить Автоматический режим";
static LPCSTR s_pszOpenCmdAlarm  = "$Blue.Открыть";
static LPCSTR s_pszCloseCmdAlarm  = "$Blue.Закрыть";
// желтые
static LPCSTR s_pszOpenedAlarm  = "$Yellow.Открыт";
static LPCSTR s_pszClosedAlarm  = "$Yellow.Закрыт";

   struct S_EV_Impl : S_EV
{
  void CalcAlarmService ( KFlexBlockBase::SCallParams &params, SFlex *pRaW);
  void CalcAlarm ( KFlexBlockBase::SCallParams &params, SFlex *pRaW);
  void Convert();
}; 

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_EV, 20 )
DECLARE_SHARED_VARSINFO( S_EV, 60 )
//////////////////////////////////////////////////////////////////////////
void K_EV::InitParm()
{
#include <FlexBlocks/EV.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/EV.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_EV::OnCreated()
{
  pRaW = CreateSFlexSharedObj( m_szVisibleName, &W );
  W->PERMISSIVE = 1;
  W->OP = 1;
  nPERMISSIVE = -1;

  if( !s_kprivateVarInfo && !s_ksharedVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
    //для комиты
  W->TimeUpOpen = 60;
  W->TimeDownOpen = 20;
  W->TimeUpClose = 60;
  W->TimeDownClose = 20;
}

void K_EV::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( pIn->m_szSink=="INTERLOCK" )
      (*pIn) >> INTERLOCK;
    else if( pIn->m_szSink=="IN1" )
      (*pIn) >> DI1;
    else if( pIn->m_szSink=="IN2" )
      (*pIn) >> DI2;
    else if( pIn->m_szSink=="PERMISSIVE" )
    {
      (*pIn) >> W->PERMISSIVE;
      nPERMISSIVE = i;
    }
    else if( pIn->m_szSink=="MODE" )
      (*pIn) >> W->MODE;
    else if( pIn->m_szSink=="SV" )
      (*pIn) >> SV;
    else if( pIn->m_szSink=="Opening" )
      (*pIn) >> W->Opening;
    else if( pIn->m_szSink=="Closing" )
      (*pIn) >> W->Closing;
    else
    {
      ASSD(0);
    };
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    if( pOut->m_szSource=="ERR" )
      (*pOut) << ERR;
    else if( pOut->m_szSource=="OUT1" )
      (*pOut) << OUT1;
    else if( pOut->m_szSource=="OUT2" )
      (*pOut) << OUT2;
    else if( pOut->m_szSource=="OUT3" )
      (*pOut) << OUT3;
    else
    {
      ASSD(0);
    };
  }
}
// комита
      struct S_EV_Impl_1 : S_EV
{
  void Convert1();
}; 
//
KFlexBlockBase::SReturn K_EV::StepCalc( SCallParams &params )
{
  SReturn sr;
  ((S_EV_Impl*)W)->CalcAlarm( params, pRaW );
  if( nPERMISSIVE>=0 )
  {
    m_ppIn[nPERMISSIVE]->GetData();
  }
  if ( INTERLOCK )
    W->IL_Ind = 1;
  else
    W->IL_Ind = 0;

  if( !W->AlgOn && INTERLOCK)
  {
    W->IL_STATE = 1;
  }
  else
    W->IL_STATE = 0;

  if( !W->PERMISSIVE && !W->AlgOn )
    return sr;

  if( W->Repair )
  {
    W->MODE = 255;
    W->OP = 1;
    return sr;
  }

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
    if ( 255==W->MODE  )
  {
    W->AlgOn = 1;
    W->MODE = 3;
  }
    if (W->AlgOn == 0 && W->MODE == 3)
  {
    W->MODE = 0;
  }
    if ( W->MODE == 0 || W->MODE == 1)
    {
      if ( W->AlgFl == 2)
      {
        W->AlgFl = 0;
        W->AlgOn = 0;

      }
    }
    if (W->AlgOn == 1 && W->AlgFl == 0)
    {
        W->MODE = 3;
        W->AlgFl = 2;
        return sr;
    }

  if ( W->LocalMode)
  {
    if( W->Opening )
      W->OP = 2;
    else if (W->Closing)
      W->OP = 0;
    else
    W->OP = 1;
  }
  
  if( 0==W->MODE )
  {
    SV = W->OP;
  }
  else if( 1==W->MODE )
  {
    W->OP = SV;
  }

  if( INTERLOCK && !W->AlgOn)
  {
    W->MODE = 0;
    W->OP = LOCK_IN;
  }

  if( 1==DI1 && 0==DI2 )
    W->PV = 0;
  else if( 0==DI1 && 1==DI2 )
    W->PV = 2;
  else
    W->PV = 1;

  switch( W->OP )
  {
  case 0:
    OUT1 = 1;
    OUT2 = 0;
    OUT3 = 0;
    break;
  case 1:
    OUT1 = 0;
    OUT2 = 1;
    OUT3 = 0;
    break;
  case 2:
    OUT1 = 0;
    OUT2 = 0;
    OUT3 = 1;
    break;
  }

  if( 0==W->PV && 0!=W->OP )
    ERR = 1;
  else if( 1==W->PV && 1!=W->OP )
    ERR = 1;
  else if( 2==W->PV && 2!=W->OP )
    ERR = 1;
  else
    ERR = 0;

    // комита, таймера
  if( W->ResetCount1 )
  {
    W->AHOUR1 = 0;
    W->StartCount1 = 0;
  }
  else
    if( W->StartCount1 && W->PermCount1 )
    {
      W->AHOUR1 += (params.dtS / 3600.f );
    }
    S_EV_Impl *pImpl = (S_EV_Impl*)W;
    pImpl->Convert();
 if( W->ResetCount2 )
  {
    W->AHOUR2 = 0;
    W->StartCount2 = 0;
  }
  else
    if( W->StartCount2 && W->PermCount2 )
    {
      W->AHOUR2 += (params.dtS / 3600.f );
    }

    S_EV_Impl_1 *pImpl_1 = (S_EV_Impl_1*)W;
    pImpl_1->Convert1();

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
// комита

void S_EV_Impl::CalcAlarm ( KFlexBlockBase::SCallParams &params, SFlex *pRaW )
{
          if( AlgOn )
    {
        if( !AlgOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszAlgOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          AlgOnAlarm = 1;
        }
    }
        else if( AlgOnAlarm )
        {
          AlgOnAlarm = 0;
        }
//
        if( !AlgOn )
    {
        if( !AlgOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszAlgOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          AlgOffAlarm = 1;
        }
    }
        else if( AlgOffAlarm )
        {
          AlgOffAlarm = 0;
        }
//
        if( Repair )
    {
        if( !RepairOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszRepairOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          RepairOnAlarm = 1;
        }
    }
        else if( RepairOnAlarm )
        {
          RepairOnAlarm = 0;
        }
//
        if( !Repair )
    {
        if( !RepairOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszRepairOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          RepairOffAlarm = 1;
        }
    }
        else if( RepairOffAlarm )
        {
          RepairOffAlarm = 0;
        }
//
                  if( MODE == 0 )
    {
        if( !DistModeAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszDistModeAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          DistModeAlarm = 1;
        }
    }
        else if( DistModeAlarm )
        {
          DistModeAlarm = 0;
        }
//
                  if( MODE == 1 )
    {
        if( !AutoModeAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszAutoModeAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          AutoModeAlarm = 1;
        }
    }
        else if( AutoModeAlarm )
        {
          AutoModeAlarm = 0;
        }
//
                  if( OP == 2 )
    {
        if( !OpenCmdAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszOpenCmdAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          OpenCmdAlarm = 1;
        }
    }
        else if( OpenCmdAlarm )
        {
          OpenCmdAlarm = 0;
        }
//
        if( OP == 0 )
    {
        if( !CloseCmdAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCloseCmdAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          CloseCmdAlarm = 1;
        }
    }
        else if( CloseCmdAlarm )
        {
          CloseCmdAlarm = 0;
        }
//
                  if( PV == 2 )
    {
        if( !OpenedAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszOpenedAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          OpenedAlarm = 1;
        }
    }
        else if( OpenedAlarm )
        {
          OpenedAlarm = 0;
        }
//
        if( PV == 0 )
    {
        if( !ClosedAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszClosedAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          ClosedAlarm = 1;
        }
    }
        else if( ClosedAlarm )
        {
          ClosedAlarm = 0;
        }

        if( SetAck )
    {
      if( !Acked && pFlexAckingAlarm )
        pFlexAckingAlarm( pRaW, params.modelTime, AutoModeAlarm || AutoModeAlarm || DistModeAlarm || DistModeAlarm );
      Acked = 1;
      SetAck = 0;
    }
};


        void S_EV_Impl::Convert()
        {
            int ihour = (int)AHOUR1;
            tAnalog32 n = 0;
            tAnalog32 h = modff( AHOUR1, &n );
            int iN = (int)n;
            int s3599 = int(h * 3600.f);
              Minute1 = s3599 / 60;
              Second1 = s3599 - Minute1*60;
              Day1 = iN / 24;
              Hour1 = iN - Day1*24;
        }
        void S_EV_Impl_1::Convert1()
        {
            int ihour = (int)AHOUR2;
            tAnalog32 n = 0;
            tAnalog32 h = modff( AHOUR2, &n );
            int iN = (int)n;
            int s6000 = int(h * 3600.f);
              Minute2 = s6000 / 60;
              Second2 = s6000 - Minute2*60;
              Day2 = iN / 24;
              Hour2 = iN - Day2*24;
        }