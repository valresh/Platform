#include <crossplatform.h>
#include <cmath>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>
#include <float.h>
#include <stdio.h>

struct S_PIDA_P
{
#include "PrivateVars/PIDA.h"
};


class K_PIDA 
  : public KFlexBlockBase
  , protected S_PIDA_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_PIDA *W;
  SFlex *pRaW;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _PIDA("PIDA", K_PIDA::Create);

KFlexBlockBase *K_PIDA::Create()
{
  return new K_PIDA;
}

static LPCSTR s_pszHH = "$Red.HH";
static LPCSTR s_pszH = "$Yellow.H";
static LPCSTR s_pszL = "$Yellow.L";
static LPCSTR s_pszLL = "$Red.LL";
// синие, без звука
static LPCSTR s_pszInversAlarmOn = "$Blue.Отключить Реверс вкл/выкл";
static LPCSTR s_pszInversAlarmOff = "$Blue.Включить Реверс вкл/выкл";
static LPCSTR s_pszCtrlProcAlarmOn = "$Blue.Включить контроль за процессом";
static LPCSTR s_pszCtrlProcAlarmOff = "$Blue.Отключить контроль за процессом";
static LPCSTR s_pszCtrlStatChainAlarmOn = "$Blue.Включить статическую цепь контроля";
static LPCSTR s_pszCtrlStatChainAlarmOff = "$Blue.Отключить статическую цепь контроля";
static LPCSTR s_pszPermCasModeAlarm = "$Blue.Разрешение режима Каскад";
static LPCSTR s_pszAutoModeAlarm = "$Blue.Установить режим Автомат";
static LPCSTR s_pszManModeAlarm = "$Blue.Установить режим Ручной";
static LPCSTR s_pszCasModeAlarm = "$Blue.Установить режим Каскад";
static LPCSTR s_pszAnCtrlAlarm = "$Blue.Включить аналоговое управление";
static LPCSTR s_pszRepairAlarmOn  = "$Green.Выведен в ремонт";
static LPCSTR s_pszRepairAlarmOff  = "$White.Выведен из ремонта";
static LPCSTR s_pszChangeSpAlarm = "$Blue.Уставка изменена c";
static LPCSTR s_pszChangeOpAlarm = "$Blue.Уставка изменена c";

struct S_PIDA_ALARMSPV : S_PIDA
{
  void Reset()
  {
    HH = H = L = LL = 0;
    HH_LIM_PV = H_LIM_PV = FLT_MAX;
    LL_LIM_PV = L_LIM_PV = -FLT_MAX;
  }
  bool IsValid( tAnalog32 &LIMIT )
  {
    if(  LIMIT >= MAXIMUM || LIMIT <= MINIMUM )
      return false;
    if( FLT_MAX==LIMIT )
      return false;
    if( (-FLT_MAX)==LIMIT )
      return false;
    return true;
  }
  void CalcAlarm( KFlexBlockBase::SCallParams &params, SFlex *pRaW );
  void Calc( KFlexBlockBase::SCallParams &params, SFlex *pRaW )
  {
    if( MAXIMUM <= MINIMUM )
      return;
    tAnalog32 hyst = ((MAXIMUM - MINIMUM) / 100.f) * HYST;
    char szAtValue[1024] = {};
    snprintf(szAtValue, countof(szAtValue), "%.2f", PV);
    if( IsValid(HH_LIM_PV) )
    {
      if( PV > HH_LIM_PV )
      {
        if( !HH )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszHH, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        }
        HH = 1;
      }
      else if( PV < (HH_LIM_PV-hyst) )
      {
        if( pFlexAlarm && HH )
          pFlexAlarm( pRaW, false, s_pszHH, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        HH = 0;
      }
    }
    if( IsValid(H_LIM_PV) )
    {
      if( PV > H_LIM_PV )
      {
        if( !H )
        {
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszH, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        }
        H = 1;
      }
      else if( PV < (H_LIM_PV-hyst) )
      {
        if( pFlexAlarm && H )
          pFlexAlarm( pRaW, false, s_pszH, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        H = 0;
      }
    }
    if( IsValid(L_LIM_PV) )
    {
      if( PV < L_LIM_PV )
      {
        if( !L )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszL, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        }
        L = 1;
      }
      else if( PV > (L_LIM_PV+hyst) )
      {
        if( pFlexAlarm && L )
          pFlexAlarm( pRaW, false, s_pszL, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        L = 0;
      }
    }
    if( IsValid(LL_LIM_PV) )
    {
      if( PV < LL_LIM_PV )
      {
        if( !LL )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszLL, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        }
        LL = 1;
      }
      else if( PV > (LL_LIM_PV+hyst) )
      {
        if( pFlexAlarm && LL )
          pFlexAlarm( pRaW, false, s_pszLL, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        LL = 0;
      }
    }
    if( SetAck )
    {
      if( !Acked && pFlexAckingAlarm )
        pFlexAckingAlarm( pRaW, params.modelTime, LL || L || H || HH || INVERSE || !INVERSE || CtrlProc || !CtrlProc || CtrlStatChain || !CtrlStatChain || PermCasMode || AnCtrl || !AnCtrl);
      Acked = 1;
      SetAck = 0;
    }
  }
};

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_PIDA, 20 )
DECLARE_SHARED_VARSINFO( S_PIDA, 74 )
//////////////////////////////////////////////////////////////////////////
void K_PIDA::InitParm()
{
#include <FlexBlocks/PIDA.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/PIDA.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_PIDA::OnCreated()
{
  pRaW = CreateSFlexSharedObj( m_szVisibleName, &W );
  ((S_PIDA_ALARMSPV*)W)->Reset();

  if( !s_kprivateVarInfo && !s_ksharedVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
  // для комиты
  W->SetDeltaErr = 0.1;
  W->SetDeltaTime = 1;
}

void K_PIDA::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( pIn->m_szSink=="LOCK_IN" )
      (*pIn) >> LOCK_IN;
    else if( pIn->m_szSink=="INTERLOCK" )
      (*pIn) >> INTERLOCK;
    else if( pIn->m_szSink=="CAS_IN" )
      (*pIn) >> CAS_IN;
    else if( pIn->m_szSink=="TRK_IN" )
      (*pIn) >> TRK_IN;
    else if( pIn->m_szSink=="TRK_IND" )
      (*pIn) >> W->TRK_IND;
    else if( pIn->m_szSink=="IN" )
      (*pIn) >> W->PV;
    else
    {
      ASSD(0);
    };
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[0] )
    {
    case 'H':
      if( 'H'==pOut->m_szSource[1])
        (*pOut) << W->HH;
      else
        (*pOut) << W->H;
      break;
    case 'L':
      if( 'L'==pOut->m_szSource[1])
        (*pOut) << W->LL;
      else if( 'O'==pOut->m_szSource[1])
        (*pOut) << LOCK;
      else
        (*pOut) << W->L;
      break;
    case 'O':
      (*pOut) << out;
      break;
    }
  }
}

namespace ns_PIDA
{
  tAnalog32 toPercent( tAnalog32 v, tAnalog32 h, tAnalog32 l )
  {
    return ( v - l ) / ( h - l );
  }
};

KFlexBlockBase::SReturn K_PIDA::StepCalc( SCallParams &params )
{
  SReturn sr;
  ((S_PIDA_ALARMSPV*)W)->CalcAlarm( params, pRaW );
  tAnalog32 En1 = ns_PIDA::toPercent(W->PV, W->MAXIMUM, W->MINIMUM) - ns_PIDA::toPercent(W->SP, W->MAXIMUM, W->MINIMUM);
  tAnalog32 EPV1 = ns_PIDA::toPercent(W->PV, W->MAXIMUM, W->MINIMUM);
  W->ActValueSSV = (W->MAXIMUM - W->MINIMUM)/100.f * W->SSV;
  W->ActValueFSV = (W->MAXIMUM - W->MINIMUM)/100.f * W->FSV;
  W->ActValueSMV = (W->MAX_OUT - W->MIN_OUT)/100.f * W->SMV;
  W->ActValueFMV = (W->MAX_OUT - W->MIN_OUT)/100.f * W->FMV;    
  if (W->Repair)
  {
    W->TRK_IND = 2;
    W->MODE = 255;
    return sr;
  }
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  if( W->MODE == 1 && SP_LIMIT)
    {
      if(W->SP > W->MAX_SP)
        W->SP = W->MAX_SP;
      if(W->SP < W->MIN_SP)
        W->SP = W->MIN_SP;
    }
  if(!SP_LIMIT)
    {
      W->MAX_SP = W->MAXIMUM;
      W->MIN_SP = W->MINIMUM;
    }

    if ( 255==W->MODE  )
  {
    W->TRK_IND = 2;
    W->AlgOn = 1;
    W->MODE = 3;
  }
     if ( W->AlgOn && W->AlgFl == 2 )
    {
      W->MODE = 3;
      W->TRK_IND = 2;
      W->AlgFl = 0;
    }
     if(W->AlgOn)
      W->NotAuto = 1;
     else
      W->NotAuto = 0;

    if ( (!W->AlgOn && W->MODE==3) || (!W->AlgOn && W->TRK_IND == 2) )
  {
    if( W->AlgFl == 0)
    {
      W->TRK_IND = 0;
      W->MODE = 0;
    }
  }
    if (W->TRK_IND==0 || W->TRK_IND==1)
    {
      if(W->AlgFl==0)
      {
        W->AlgOn = 0;
        W->AlgFl = 2;
      }
    }

  if ( W->NotAuto && !W->AlgOn)
    W->TRK_IND = 0;
  if( INTERLOCK && !W->AlgOn )
  {
    LOCK = 1;
    W->MODE = 0;
    W->OP = LOCK_IN;
  }
  else
    LOCK = 0;

  W->IL_STATE = INTERLOCK;

  if (!std::isfinite(W->OP))
    return sr;

  if( 1==W->TRK_IND )
  {
    W->MODE = 0;
    W->OP = TRK_IN;
    W->SP = W->PV;
  if ( W->OP < W->MIN_OUT )
    W->OP = W->MIN_OUT;
  if ( W->OP > W->MAX_OUT )
    W->OP = W->MAX_OUT;
  out = W->OP;
    if(W->PV >= W->SetValOpened)
    W->Opened = 1;
  else
    W->Opened = 0;
    if(W->PV <= W->SetValClosed)
    W->Closed = 1;
  else
    W->Closed = 0;
	for( DWORD i=0; i<m_nOuts; ++i )
	{
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
	}
	return sr;
   }
  
  ((S_PIDA_ALARMSPV*)W)->Calc( params, pRaW );

  if( 0==W->MODE || 255==W->MODE )
  {
    W->SP = W->PV;
  }
  else if( 1==W->MODE || 2==W->MODE )
  {
    if( 2==W->MODE )
    {
      if (!std::isfinite(CAS_IN))
        return sr;
      W->SP = CAS_IN;
    }

    tAnalog32 En = ns_PIDA::toPercent(W->PV, W->MAXIMUM, W->MINIMUM) - ns_PIDA::toPercent(W->SP, W->MAXIMUM, W->MINIMUM);
    tAnalog32 EPV = ns_PIDA::toPercent(W->PV, W->MAXIMUM, W->MINIMUM);

    tAnalog32 Ks = ( W->MAX_OUT - W->MIN_OUT ) / 100.f;
    tAnalog32 Kp = 1;
    if( 0!=W->P && !ALGORITHM == 1)
      Kp = 100.f / W->P;
    else
      Kp = W->P * 100.f;
    tAnalog32 dEPV = EPV - EPV1;
    tAnalog32 dEn = En - En1;
    tAnalog32 dMV = 0;

      switch ( ALGORITHM )
        {
        case 0:
         if( 0!=W->I )
          dMV = Kp * Ks * ( dEn + params.dtS / W->I * En );
         else
          dMV = Kp * Ks * ( dEn + params.dtS * En );
          break;
        case 2:
         if( 0!=W->I )
          dMV = Kp * Ks * ( dEPV + params.dtS / W->I * En );
         else
          dMV = Kp * Ks * ( dEPV + params.dtS * En );
          break;
        }

    if ( W->INVERSE )
      dMV = -dMV;
    tAnalog32 op = W->OP + dMV;
    if (!std::isfinite(op))
      return sr;
    if( (abs(En*100.f) < W->SetDeltaErr) && W->DeadZoneCtrl )
    {
      dMV = 0;
    }
    W->OP += dMV;
  }

  if ( W->OP < W->MIN_OUT )
    W->OP = W->MIN_OUT;
  if ( W->OP > W->MAX_OUT )
    W->OP = W->MAX_OUT;
  out = W->OP;
  if(W->PV >= W->SetValOpened)
    W->Opened = 1;
  else
    W->Opened = 0;
    if(W->PV <= W->SetValClosed)
    W->Closed = 1;
  else
    W->Closed = 0;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}

void S_PIDA_ALARMSPV::CalcAlarm( KFlexBlockBase::SCallParams &params, SFlex *pRaW )
{
  // комита
           if( INVERSE )
    {
        if( !InversAlarmOn )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszInversAlarmOn, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          InversAlarmOn = 1;
        }
    }
        else if( InversAlarmOn )
        {
          if( pFlexAlarm)
          InversAlarmOn = 0;
        }
        if( !INVERSE )
    {
        if( !InversAlarmOff )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszInversAlarmOff, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          InversAlarmOff = 1;
        }
    }
        else if( InversAlarmOff )
        {
          if( pFlexAlarm )
          InversAlarmOff = 0;
        }
  //
        if( CtrlProc )
    {
        if( !CtrlProcAlarmOn )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlProcAlarmOn, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          CtrlProcAlarmOn = 1;
        }
    }
        else if( CtrlProcAlarmOn )
        {
          if( pFlexAlarm )
          CtrlProcAlarmOn = 0;
        }
//
        if( !CtrlProc )
    {
        if( !CtrlProcAlarmOff )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlProcAlarmOff, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          CtrlProcAlarmOff = 1;
        }
    }
        else if( CtrlProcAlarmOff )
        {
          if( pFlexAlarm )
          CtrlProcAlarmOff = 0;
        }
//
        if( CtrlStatChain )
    {
        if( !CtrlStatChainAlarmOn )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlStatChainAlarmOn, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          CtrlStatChainAlarmOn = 1;
        }
    }
        else if( CtrlStatChainAlarmOn )
        {
          if( pFlexAlarm )
          CtrlStatChainAlarmOn = 0;
        }
//
        if( !CtrlStatChain )
    {
        if( !CtrlStatChainAlarmOff )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlStatChainAlarmOff, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          CtrlStatChainAlarmOff = 1;
        }
    }
        else if( CtrlStatChainAlarmOff )
        {
          if( pFlexAlarm )
          CtrlStatChainAlarmOff = 0;
        }
  //
        if( PermCasMode )
    {
        if( !PermCasModeAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszPermCasModeAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          PermCasModeAlarm = 1;
        }
    }
        else if( PermCasModeAlarm )
        {
          if( pFlexAlarm )
          PermCasModeAlarm = 0;
        }
//
        if( MODE == 0 )
    {
        if( !ManModeAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszManModeAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          ManModeAlarm = 1;
        }
    }
        else if( ManModeAlarm )
        {
          if (pFlexAlarm)
          ManModeAlarm = 0;
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
         if (pFlexAlarm)
          AutoModeAlarm = 0;
        }
//
        if( MODE == 2 )
    {
        if( !CasModeAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCasModeAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          CasModeAlarm = 1;
        }
    }
        else if( CasModeAlarm )
        {
          if (pFlexAlarm)
          CasModeAlarm = 0;
        }
        if( AnCtrl )
    {
        if( !AnCtrlAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszAnCtrlAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          AnCtrlAlarm = 1;
        }
    }
        else if( AnCtrlAlarm )
        {
          if (pFlexAlarm)
          AnCtrlAlarm = 0;
        }
        if( Repair )
    {
        if( !RepairAlarmOn )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszRepairAlarmOn, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          RepairAlarmOn = 1;
        }
    }
        else if( RepairAlarmOn )
        {
          if (pFlexAlarm)
          RepairAlarmOn = 0;
        }
        if( !Repair )
    {
        if( !RepairAlarmOff )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszRepairAlarmOff, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          RepairAlarmOff = 1;
        }
    }
        else if( RepairAlarmOff )
        {
          if (pFlexAlarm)
          RepairAlarmOff = 0;
        }
//
        if( PREVSP != SP && MODE == 1)
    {
         char szAtValue[32*4] = {};
        snprintf( szAtValue, _countof(szAtValue), "%1.2f на %1.2f", PREVSP, SP );
        PREVSP = SP;
        if (PREVMODE == MODE)
        {
            if( !ChangeSpAlarm )
            {
              SetAck = 0;
              Acked = 0;
              if( pFlexAlarm )
                pFlexAlarm( pRaW, true, s_pszChangeSpAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
              ChangeSpAlarm = 1;
            }
        }
        PREVMODE = MODE;
    }
        else if( ChangeSpAlarm )
        {
          if (pFlexAlarm)
          ChangeSpAlarm = 0;
        }
//
        if( PREVOP != OP && MODE == 0 && TRK_IND == 0)
    {
         char szAtValue[32 * 4] = {};
        snprintf( szAtValue, _countof(szAtValue), "%1.2f на %1.2f", PREVOP, OP );
        PREVOP = OP;
        if (PREVMODE == MODE)
        {
          if( !ChangeOpAlarm )
          {
            SetAck = 0;
            Acked = 0;
            if( pFlexAlarm )
              pFlexAlarm( pRaW, true, s_pszChangeOpAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
            ChangeOpAlarm = 1;
          }
        }
        PREVMODE = MODE;
    }
        else if( ChangeOpAlarm )
        {
          if (pFlexAlarm)
          ChangeOpAlarm = 0;
        }


            if( SetAck )
    {
      if( !Acked && pFlexAckingAlarm )
        pFlexAckingAlarm( pRaW, params.modelTime, LL || L || H || HH || INVERSE || !INVERSE || CtrlProc || !CtrlProc || CtrlStatChain || !CtrlStatChain || PermCasMode || AnCtrl || !AnCtrl);
      Acked = 1;
      SetAck = 0;
    }
}