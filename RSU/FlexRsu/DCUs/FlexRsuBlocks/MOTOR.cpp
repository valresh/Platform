#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>
#include <float.h>
#include <math.h>


struct S_MOTOR_P
{
#include "PrivateVars/MOTOR.h"
};


class K_MOTOR 
  : public KFlexBlockBase
  , protected S_MOTOR_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_MOTOR *W;
  SFlex *pRaW;
  int nPERMISSIVE;
  tDigital32 prevOP;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  virtual void OnAfterRestoreState();
  static void InitParm();
};

static KBlockCreator _MOTOR("MOTOR", K_MOTOR::Create);

KFlexBlockBase *K_MOTOR::Create()
{
  return new K_MOTOR;
}
// Синие, без звкового сопровождения
static LPCSTR s_pszAlgOnAlarm  = "$Blue.Выведен из алгоритма";
static LPCSTR s_pszAlgOffAlarm  = "$Blue.Введен в алгоритм";
static LPCSTR s_pszRepairOnAlarm  = "$Blue.Вывод в ремонт";
static LPCSTR s_pszRepairOffAlarm  = "$Blue.Вывод из ремонта";
static LPCSTR s_pszDistModeAlarm  = "$Blue.Включить Дистанционный режим";
static LPCSTR s_pszLocalModeAlarm  = "$Blue.Включить Местный режим";

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_MOTOR, 20 )
DECLARE_SHARED_VARSINFO( S_MOTOR, 56 )
//////////////////////////////////////////////////////////////////////////
void K_MOTOR::InitParm()
{
#include <FlexBlocks/MOTOR.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/MOTOR.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_MOTOR::OnCreated()
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
  W->SetTimeOn = 5;
  W->SetTimeOff = 2;
  W->SetImpTimeOn = 2;
  W->SetImpTimeOff = 0;
  W->SetTimeReversOn = 10;
}

void K_MOTOR::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( pIn->m_szSink=="INTERLOCK" )
      (*pIn) >> INTERLOCK;
    else if( pIn->m_szSink=="IN" )
      (*pIn) >> DI;
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
//Комита
   struct S_MOTOR_Impl : S_MOTOR
{
  void CalcAlarmService ( KFlexBlockBase::SCallParams &params, SFlex *pRaW);
  void CalcAlarm ( KFlexBlockBase::SCallParams &params, SFlex *pRaW);
  void Convert();
}; 
      struct S_MOTOR_Impl_1 : S_MOTOR
{
  void Convert1();
}; 
//
void K_MOTOR::OnAfterRestoreState()
{
  prevOP = W->OP;
}

KFlexBlockBase::SReturn K_MOTOR::StepCalc( SCallParams &params )
{
  SReturn sr;

  if( nPERMISSIVE>=0 )
  {
    m_ppIn[nPERMISSIVE]->GetData();
  }
  ((S_MOTOR_Impl*)W)->CalcAlarmService( params, pRaW );
    //if( !W->PERMISSIVE && !W->AlgOn)
      //return sr;

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

  if( !W->PERMISSIVE && W->PV == 0 && !W->AlgOn)
  {
    W->OP = 0;
  }
  if( !W->PERMISSIVE && W->PV==0 && W->MODE && !W->AlgOn)
  {
    SV = 0;
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
        //return sr;
    }


  if( 0==W->MODE && !W->LocalMode )
  {
    SV = W->OP;
  }
  else if( 1==W->MODE  && !W->LocalMode)
  {
    W->OP = SV;
  }
  if ( W->LocalMode && !W->IL_STATE)
  {
    W->OP = 1;
  }

   if( W->IL_STATE && !SAVE_MODE && !W->AlgOn && W->MODE != 3)
  {
    W->MODE = 0;
    W->OP = LOCK_IN;
  }

  else if (INTERLOCK && (1==SAVE_MODE) && !W->AlgOn){
	  if (1==W->MODE){
		  SV = LOCK_IN;
	  }
	  else{
		  W->OP = LOCK_IN;
	  }
  }

  if( 0==DI )
    W->PV = 0;
  else if( 1==DI )
    W->PV = 2;
  else
    W->PV = 1;
if ( !W->TypeCtrl )
{
  switch( W->OP )
  {
  case 0:
    OUT1 = 1;
    OUT2 = 0;
    break;
  case 1:
    OUT1 = 0;
    OUT2 = 0;
    break;
  case 2:
    OUT1 = 0;
    OUT2 = 1;
    break;
  }
}
  if ( W->TypeCtrl && W->OP == 2 )
  {
  if (prevOP != W->OP )
		{
      pulsing += params.dtS;
      prevOP = W->OP;
		}
  if( pulsing > 0 )
  {
    pulsing += params.dtS;
    if( pulsing >= W->SetImpTimeOn )
    {
      OUT1 = 0;
      OUT2 = 0;
      pulsing = 0;
    }
      else
      {
        OUT1 = 0;
        OUT2 = 1;
      }
    }
  }

  if ( W->TypeCtrl && W->OP == 0 )
  {
  if (prevOP != W->OP)
		{
      pulsing += params.dtS;
      prevOP = W->OP;
		}
  if( pulsing > 0 )
  {
    pulsing += params.dtS;
    if( pulsing >= W->SetImpTimeOff )
    {
      OUT1 = 0;
      OUT2 = 0;
      pulsing = 0;
    }
      else
      {
        OUT1 = 1;
        OUT2 = 0;
      }
    }
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
    S_MOTOR_Impl *pImpl = (S_MOTOR_Impl*)W;
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

    S_MOTOR_Impl_1 *pImpl_1 = (S_MOTOR_Impl_1*)W;
    pImpl_1->Convert1();

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  ((S_MOTOR_Impl*)W)->CalcAlarm( params, pRaW );
  return sr;
}
        void S_MOTOR_Impl::CalcAlarm ( KFlexBlockBase::SCallParams &params, SFlex *pRaW)
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
          if( LocalMode )
    {
        if( !LocalModeAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszLocalModeAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          LocalModeAlarm = 1;
        }
    }
        else if( LocalModeAlarm )
        {
          LocalModeAlarm = 0;
        }
        if( SetAck )
    {
      if( !Acked && pFlexAckingAlarm )
        pFlexAckingAlarm( pRaW, params.modelTime, AlgOn || !AlgOn || LocalMode || MODE==0 || Repair || !Repair);
      Acked = 1;
      SetAck = 0;
    }
   };

        void S_MOTOR_Impl::CalcAlarmService ( KFlexBlockBase::SCallParams &params, SFlex *pRaW )
        {
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
                if( SetAck )
    {
      if( !Acked && pFlexAckingAlarm )
        pFlexAckingAlarm( pRaW, params.modelTime, Repair || !Repair);
      Acked = 1;
      SetAck = 0;
    }
     }

// комита
        void S_MOTOR_Impl::Convert()
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
        void S_MOTOR_Impl_1::Convert1()
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