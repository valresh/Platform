#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>
#include <float.h>
#include <math.h>


struct S_VLV_P
{
#include "PrivateVars/VLV.h"
};


class K_VLV 
  : public KFlexBlockBase
  , protected S_VLV_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_VLV *W;
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

static KBlockCreator _VLV("VLV", K_VLV::Create);

KFlexBlockBase *K_VLV::Create()
{
  return new K_VLV;
}
// Синие, без звкового сопровождения
static LPCSTR s_pszAlgOnAlarm  = "$Blue.Выведен из алгоритма";
static LPCSTR s_pszAlgOffAlarm  = "$Blue.Введен в алгоритм";
static LPCSTR s_pszRepairOnAlarm  = "$Blue.Вывод в ремонт";
static LPCSTR s_pszRepairOffAlarm  = "$Blue.Вывод из ремонта";
static LPCSTR s_pszDistModeOnAlarm  = "$Blue.Включить Дистанционный режим";
static LPCSTR s_pszDistModeOffAlarm  = "$Blue.Отключить Дистанционный режим";
static LPCSTR s_pszAutoModeOnAlarm  = "$Blue.Включить Автоматический режим";
static LPCSTR s_pszAutoModeOffAlarm  = "$Blue.Отключить Автоматический режим";
static LPCSTR s_pszOpenCmdAlarm  = "$Blue.Открыть";
static LPCSTR s_pszCloseCmdAlarm  = "$Blue.Закрыть";
// желтые
static LPCSTR s_pszOpenedAlarm  = "$Yellow.Открыт";
static LPCSTR s_pszClosedAlarm  = "$Yellow.Закрыт";

  struct S_VLV_Impl : S_VLV
{
  void CalcAlarmService ( KFlexBlockBase::SCallParams &params, SFlex *pRaW);
  void CalcAlarm ( KFlexBlockBase::SCallParams &params, SFlex *pRaW);
  void Convert();
}; 

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_VLV, 20 )
DECLARE_SHARED_VARSINFO( S_VLV, 61 )
//////////////////////////////////////////////////////////////////////////
void K_VLV::InitParm()
{
#include <FlexBlocks/VLV.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/VLV.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_VLV::OnCreated()
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

void K_VLV::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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
    else if( pIn->m_szSink=="STARTCOUNT1" )
      (*pIn) >> W->StartCount1;
    else if( pIn->m_szSink=="STARTCOUNT2" )
      (*pIn) >> W->StartCount2;
    else
    {
      ASSD(0);
    };
  }

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    if( pOut->m_szSource=="ERR" )
      (*pOut) << W->ERR;
    else if( pOut->m_szSource=="OUT1" )
      (*pOut) << OUT1;
    else if( pOut->m_szSource=="OUT2" )
      (*pOut) << OUT2;
    else
    {
      ASSD(0);
    };
  }
}

      struct S_VLV_Impl_1 : S_VLV
{
  void Convert1();
};

  

KFlexBlockBase::SReturn K_VLV::StepCalc( SCallParams &params )
{
  SReturn sr;
  ((S_VLV_Impl*)W)->CalcAlarm( params, pRaW );

  if( nPERMISSIVE>=0 )
  {
    m_ppIn[nPERMISSIVE]->GetData();
  }

  if ( !W->PERMISSIVE && !W->AlgOn)
     return sr;

  if( W->Repair )
  {
    W->MODE = 255;
    W->OP = 1;
    return sr;
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


   /* if (W->AlgOn==0 && W->MODE==3)
      W->MODE = 0;
    
    if ( W->MODE==1 || W->MODE==0 )
  {
    if ( W->AlgOn == 1 && W->AlgFl ==0 && W->MODE != 3)
    {
    W->AlgFl = 1;
    }
  }
    if (W->AlgOn==0 && W->MODE==3)
      W->MODE = 0;
    if (W->AlgFl == 1)
    {
      W->MODE = 3;
      W->AlgFl = 0;
    }*/
  if ( W->LocalMode)
    W->OP = 1;

  if( 0==W->MODE )
  {
    SV = W->OP;
  }
  else if( 1==W->MODE )
  {
    W->OP = SV;
  }
  if ( INTERLOCK && HOLD_IL && !W->AlgOn)
  {
    W->IL_STATE = 1;
  }
  if ( INTERLOCK && !HOLD_IL && !W->AlgOn)
  {
    W->IL_STATE = 1;
  }
  else if(!INTERLOCK && !HOLD_IL || W->AlgOn)
  {
    W->IL_STATE = 0;
  }
  if (W->RESET && W->IL_STATE)
  {
    W->IL_STATE = 0;
    W->RESET = 0;
  }
  if( W->IL_STATE && !SAVE_MODE && !W->AlgOn)
  {
    W->MODE = 0;
    W->OP = LOCK_IN;
  }
  else if (1==W->IL_STATE && !W->AlgOn){
	  if (1==W->MODE){
		  SV = LOCK_IN;
	  }
	  else{
		  W->OP = LOCK_IN;
	  }
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
    W->DO = 0;
    break;
  case 1:
    OUT1 = 0;
    OUT2 = 0;
    if ( W->PV == 2)
      W->DO = 1;
    else if( W->PV == 0)
      W->DO = 0;
    break;
  case 2:
    OUT1 = 0;
    OUT2 = 1;
    W->DO = 1;
    break;
  }

  if( 0==W->PV && 2==W->OP )
    W->ERR = 1;
  else if( 2==W->PV && 0==W->OP )
    W->ERR = 1;
  else
    W->ERR = 0;

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
    S_VLV_Impl *pImpl = (S_VLV_Impl*)W;
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

    S_VLV_Impl_1 *pImpl_1 = (S_VLV_Impl_1*)W;
    pImpl_1->Convert1();

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
// комита
void S_VLV_Impl::CalcAlarm ( KFlexBlockBase::SCallParams &params, SFlex *pRaW)
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
        if( !DistModeOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszDistModeOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          DistModeOnAlarm = 1;
        }
    }
        else if( DistModeOnAlarm )
        {
          DistModeOnAlarm = 0;
        }
//
        if( MODE != 0 )
    {
        if( !DistModeOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszDistModeOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          DistModeOffAlarm = 1;
        }
    }
        else if(DistModeOffAlarm)
        {
          DistModeOffAlarm = 0;
        }
//
                  if( MODE == 1 )
    {
        if( !AutoModeOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszAutoModeOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          AutoModeOnAlarm = 1;
        }
    }
        else if( AutoModeOnAlarm )
        {
          AutoModeOnAlarm = 0;
        }
//
        if( MODE != 1 )
    {
        if( !AutoModeOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszAutoModeOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          AutoModeOffAlarm = 1;
        }
    }
        else if( AutoModeOffAlarm )
        {
          AutoModeOffAlarm = 0;
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
        pFlexAckingAlarm( pRaW, params.modelTime, AutoModeOnAlarm || AutoModeOffAlarm || DistModeOnAlarm || DistModeOffAlarm );
      Acked = 1;
      SetAck = 0;
    }
};

        void S_VLV_Impl::Convert()
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
        void S_VLV_Impl_1::Convert1()
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
