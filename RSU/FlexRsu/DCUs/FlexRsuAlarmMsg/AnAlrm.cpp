#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <float.h>
#include <stdio.h>

class K_AnAlrm 
  : public KFlexBlockBase
{
  S_ANALRM *W;
  SFlex *pRaW;
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  int m_InitialService;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  SReturn StepCalc( SCallParams &params );
  virtual void OnAfterRestoreState();
  virtual void OnAttributesSetted();
  static void InitParm();
};

static KBlockCreator _AnAlrm("ANALRM", K_AnAlrm::Create);

KFlexBlockBase *K_AnAlrm::Create()
{
  return new K_AnAlrm;
}

static LPCSTR s_pszHH = "$Red.Высокая аварийная граница";
static LPCSTR s_pszH = "$Yellow.Высокая предупредительная граница";
static LPCSTR s_pszL = "$Yellow.Низкая предупредительная граница";
static LPCSTR s_pszLL = "$Red.Низкая аварийная граница";
static LPCSTR s_pszSERVICE = "SERVICE";
static LPCSTR s_pszHiScale = "$Yellow.КЗ";
static LPCSTR s_pszLoScale = "$Yellow.Обрыв";
// зеленые
static LPCSTR s_pszCtrlHiHiAlarm = "$Green.Контроль верхней аварийной уставки включен";
static LPCSTR s_pszCtrlHiAlarm = "$Green.Контроль верхней предаварийной уставки включен";
static LPCSTR s_pszCtrlLoAlarm  = "$Green.Контроль нижней предаварийной уставки включен";
static LPCSTR s_pszCtrlLoLoAlarm  = "$Green.Контроль нижней аварийной уставки включен";
static LPCSTR s_pszImitOnAlarm  = "$Green.Имитация включена";
static LPCSTR s_pszSpeedOnAlarm  = "$Green.Контроль скорости включен";
static LPCSTR s_pszAlgOnAlarm  = "$Green.Выведен из алгоритма";
static LPCSTR s_pszFiltOnAlarm  = "$Green.Фильтр включен";
static LPCSTR s_pszSERVICE_AlarmOn  = "$Green.Выведен в ремонт";
// белые
static LPCSTR s_pszCtrlHiHiOffAlarm = "$White.Контроль верхней аварийной уставки отключен";
static LPCSTR s_pszCtrlHiOffAlarm = "$White.Контроль верхней предаварийной уставки отключен";
static LPCSTR s_pszCtrlLoOffAlarm  = "$White.Контроль нижней предаварийной уставки отключен";
static LPCSTR s_pszCtrlLoLoOffAlarm  = "$White.Контроль нижней аварийной уставки отключен";
static LPCSTR s_pszImitOffAlarm  = "$White.Имитация отключена";
static LPCSTR s_pszSpeedOffAlarm  = "$White.Контроль скорости отключен";
static LPCSTR s_pszAlgOffAlarm  = "$White.Ввведен в алгоритм";
static LPCSTR s_pszFiltOffAlarm  = "$White.Фильтр отключен";
static LPCSTR s_pszSERVICE_AlarmOff  = "$White.Выведен из ремонта";
// синие
static LPCSTR s_pszHiScaleAlarm = "$Blue.Включить контроль КЗ";
static LPCSTR s_pszLoScaleAlarm = "$Blue.Включить контроль Обрыв";

struct S_ANALRM_IMPL : S_ANALRM
{
  void Reset()
  {
    HH = H = L = LL = 0;
    HH_LIMIT = H_LIMIT = FLT_MAX;
    LL_LIMIT = L_LIMIT = -FLT_MAX;
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
  void CalcService( KFlexBlockBase::SCallParams &params, SFlex *pRaW);
  void Calc( KFlexBlockBase::SCallParams &params, SFlex *pRaW)
  {
	AlarmEnPrev = AlarmEn;
    if( MAXIMUM <= MINIMUM )
      return;
    tAnalog32 hyst = ((MAXIMUM - MINIMUM) / 100.f) * HYST;
    char szAtValue[32] = {};
    std::snprintf( szAtValue, _countof(szAtValue)-1, "%.2f", PV );
    if ( FLT_MAX == HH_LIMIT)
      CtrlHiHi = 0;
    if ( FLT_MAX == H_LIMIT)
      CtrlHi = 0;
    if ( -FLT_MAX == LL_LIMIT)
      CtrlLoLo = 0;
    if ( -FLT_MAX == L_LIMIT)
      CtrlLo = 0;

     if( SERVICE )
     {
       if( HH || H || L || LL )
       {
         if( pFlexAlarm )
           pFlexAlarm( pRaW, false, s_pszSERVICE, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
       }
       HH = H = L = LL = 0;
     }
		if( IsValid(HH_LIMIT) && (CtrlHiHi) && (AlarmEn) )
			{
				En_HH = 1;
			}
		if( IsValid(H_LIMIT) && (CtrlHi) && (AlarmEn) )
			{
				En_H = 1;
			}
		if( IsValid(L_LIMIT)&& (CtrlLo) && (AlarmEn) )
			{
				En_L = 1;
			}
		if( IsValid(LL_LIMIT)&& (CtrlLoLo) && (AlarmEn) )
			{
				En_LL = 1;
			}
    // имитация сигнала
    if(ImitOn)
      PV = ImitVal;

      float inScale = MAXIMUM-MINIMUM;
      tAnalog32 PR = ( PV - MINIMUM) / inScale;
      PVmA = 16 * PR + 4;

    if( IsValid(HH_LIMIT) && !SERVICE && (CtrlHiHi) && (AlarmEn))
    {
      if( PV >= HH_LIMIT )
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
      else if( PV < (HH_LIMIT-hyst) )
      {
        if( pFlexAlarm && HH || !CtrlHiHi)
          pFlexAlarm( pRaW, false, s_pszHH, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        HH = 0;
      }
    }
    if(!CtrlHiHi)
      HH = 0;
    if( IsValid(H_LIMIT) && !SERVICE && (CtrlHi) && (AlarmEn) )
    {
      if( PV >= H_LIMIT )
      {
        if( !H )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszH, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        }
        H = 1;
      }
      else if( PV < (H_LIMIT-hyst) )
      {
        if( pFlexAlarm && H || !CtrlHi )
          pFlexAlarm( pRaW, false, s_pszH, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        H = 0;
      }
    }
    if(!CtrlHi)
      H = 0;
    if( IsValid(L_LIMIT) && !SERVICE && (CtrlLo) && (AlarmEn) )
    {
      if( PV <= L_LIMIT )
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
      else if( PV > (L_LIMIT+hyst) )
      {
        if( (pFlexAlarm && L) || !CtrlLo )
          pFlexAlarm( pRaW, false, s_pszL, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        L = 0;
      }
    }
    if(!CtrlLo)
      L = 0;
    if( IsValid(LL_LIMIT) && !SERVICE && (CtrlLoLo) && (AlarmEn) )
    {
      if( PV <= LL_LIMIT )
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
      else if( PV > (LL_LIMIT+hyst) )
      {
        if( (pFlexAlarm && LL) || !CtrlLoLo )
          pFlexAlarm( pRaW, false, s_pszLL, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        LL = 0;
      }
    }
    if(!CtrlLoLo)
      LL = 0;

//
    if( !SERVICE && (CtrlHiScale) )
    {
      if( CtrlHiScale && (PV > MAXIMUM) )
      {
        if( !HiScale )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszHiScale, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );;
        }
        HiScale = 1;
      }
      else if( HiScale )
      {
        if( pFlexAlarm )
          if (CtrlHi)
          pFlexAlarm( pRaW, false, s_pszHiScale, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        HiScale = 0;
      }
    }
    if(!CtrlHiScale)
      HiScale = 0;

    //

    if( !SERVICE && (CtrlLoScale) )
    {
      if( CtrlLoScale && (PV < MINIMUM) )
      {
        if( !LoScale )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszLoScale, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        }
        LoScale = 1;
      }
      else if( LoScale )
      {
        if( pFlexAlarm )
          pFlexAlarm( pRaW, false, s_pszLoScale, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
        LoScale = 0;
      }
    }
    if(!CtrlLoScale)
      LoScale = 0;

    ////
    if( CtrlHiHi )
    {
        if( !CtrlHiHiAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlHiHiAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          CtrlHiHiAlarm = 1;
        }
    }
        else if( CtrlHiHiAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszCtrlHiHiAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          CtrlHiHiAlarm = 0;
        }
//
        if( !CtrlHiHi )
    {
        if( !CtrlHiHiOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlHiHiOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          CtrlHiHiOffAlarm = 1;
        }
    }
        else if( CtrlHiHiOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszCtrlHiHiOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          CtrlHiHiOffAlarm = 0;
        }

//
            if( CtrlHi )
    {
        if( !CtrlHiAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlHiAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          CtrlHiAlarm = 1;
        }
    }
        else if( CtrlHiAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszCtrlHiAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          CtrlHiAlarm = 0;
        }
//
        if( !CtrlHi )
    {
        if( !CtrlHiOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlHiOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          CtrlHiOffAlarm = 1;
        }
    }
        else if( CtrlHiOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszCtrlHiOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          CtrlHiOffAlarm = 0;
        }
//
            if( CtrlLo )
    {
        if( !CtrlLoAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlLoAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          CtrlLoAlarm = 1;
        }
    }
        else if( CtrlLoAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszCtrlLoAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          CtrlLoAlarm = 0;
        }
//
        if( !CtrlLo )
    {
        if( !CtrlLoOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlLoOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          CtrlLoOffAlarm = 1;
        }
    }
        else if( CtrlLoOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszCtrlLoOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          CtrlLoOffAlarm = 0;
        }
//
        if( CtrlLoLo )
    {
        if( !CtrlLoLoAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlLoLoAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          CtrlLoLoAlarm = 1;
        }
    }
        else if( CtrlLoLoAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszCtrlLoLoAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          CtrlLoLoAlarm = 0;
        }
//
        if( !CtrlLoLo )
    {
        if( !CtrlLoLoOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlLoLoOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          CtrlLoLoOffAlarm = 1;
        }
    }
        else if( CtrlLoLoOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszCtrlLoLoOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          CtrlLoLoOffAlarm = 0;
        }
//
        if( FiltOn )
    {
        if( !FiltOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszFiltOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          FiltOnAlarm = 1;
        }
    }
        else if( FiltOnAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszFiltOnAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          FiltOnAlarm = 0;
        }
//
        if( !FiltOn )
    {
        if( !FiltOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszFiltOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          FiltOffAlarm = 1;
        }
    }
        else if( FiltOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszFiltOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          FiltOffAlarm = 0;
        }
//
        if( SpeedOn )
    {
        if( !SpeedOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszSpeedOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          SpeedOnAlarm = 1;
        }
    }
        else if( SpeedOnAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszSpeedOnAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          SpeedOnAlarm = 0;
        }
//
        if( !SpeedOn )
    {
        if( !SpeedOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszSpeedOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          SpeedOffAlarm = 1;
        }
    }
        else if( SpeedOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszSpeedOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          SpeedOffAlarm = 0;
        }
        if( SpeedOn )
    {
        if( !SpeedOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszSpeedOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          SpeedOnAlarm = 1;
        }
    }
        else if( SpeedOnAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszSpeedOnAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          SpeedOnAlarm = 0;
        }
//
        if( !SpeedOn )
    {
        if( !SpeedOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszSpeedOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          SpeedOffAlarm = 1;
        }
    }
        else if( SpeedOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszSpeedOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          SpeedOffAlarm = 0;
        }
        if( AlgOn )
    {
        if( !AlgOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszAlgOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          AlgOnAlarm = 1;
        }
    }
        else if( AlgOnAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszAlgOnAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
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
            pFlexAlarm( pRaW, true, s_pszAlgOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          AlgOffAlarm = 1;
        }
    }
        else if( AlgOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszAlgOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          AlgOffAlarm = 0;
        }
        if( ImitOn )
    {
        if( !ImitOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszImitOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          ImitOnAlarm = 1;
        }
    }
        else if( ImitOnAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszImitOnAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          ImitOnAlarm = 0;
        }
//
        if( CtrlHiScale )
    {
        if( !HiScaleAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszHiScaleAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          HiScaleAlarm = 1;
        }
    }
        else if( HiScaleAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszImitOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          HiScaleAlarm = 0;
        }
//
        if( CtrlLoScale )
    {
        if( !LoScaleAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszLoScaleAlarm, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          LoScaleAlarm = 1;
        }
    }
        else if( LoScaleAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszImitOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          LoScaleAlarm = 0;
        }

    //

    if( SetAck )
    {
      if( !Acked && pFlexAckingAlarm )
        pFlexAckingAlarm( pRaW, params.modelTime, LL || L || H || HH || HiScale || LoScale || CtrlHiHiAlarm );
      Acked = 1;
      SetAck = 0;
    }
  }
};
void S_ANALRM_IMPL::CalcService( KFlexBlockBase::SCallParams &params, SFlex *pRaW)
  {
        char szAtValue[32] = {};
        std::snprintf( szAtValue, _countof(szAtValue)-1, "%.2f", PV );
        if( SERVICE )
    {
        if( !SERVICE_AlarmOn )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszSERVICE_AlarmOn, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          SERVICE_AlarmOn = 1;
        }
    }
        else if( SERVICE_AlarmOn )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszSERVICE_AlarmOn, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          SERVICE_AlarmOn = 0;
        }
//
        if( !SERVICE )
    {
        if( !SERVICE_AlarmOff )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszSERVICE_AlarmOff, params.modelTime, params.pszDrName, DESCR, "", "", szAtValue, Acked, "", "", "" );
          SERVICE_AlarmOff = 1;
        }
    }
        else if( SERVICE_AlarmOff )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszSERVICE_AlarmOff, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          SERVICE_AlarmOff = 0;
        }
      if( SetAck )
    {
      if( !Acked && pFlexAckingAlarm )
        pFlexAckingAlarm( pRaW, params.modelTime, LL || L || H || HH || HiScale || LoScale || CtrlHiHiAlarm );
      Acked = 1;
      SetAck = 0;
    }
	if (AlarmEnPrev && !AlarmEn)
	{
		CtrlHiHiPrev = CtrlHiHi;
		CtrlHiPrev = CtrlHi;
		CtrlLoPrev = CtrlLo;
		CtrlLoLoPrev = CtrlLoLo;
	}
	if(!AlarmEn)
	{
		CtrlHiHi = 0;
		CtrlHi = 0;
		CtrlLoLo = 0;
		CtrlLo = 0;
	}
	if(!AlarmEnPrev && AlarmEn)
	{
		CtrlHiHi = CtrlHiHiPrev;
		CtrlHi = CtrlHiPrev;
		CtrlLo = CtrlLoPrev;
		CtrlLoLo = CtrlLoLoPrev;
	}
  };

namespace ns_ANALRM
{
#include <_InitInfo.h>
  DECLARE_SHARED_VARSINFO( S_ANALRM, 70 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_ANALRM;
void K_AnAlrm::InitParm()
{
#include <FlexBlocks/AnAlrm.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
}
void K_AnAlrm::OnCreated()
{
  pRaW = CreateSFlexSharedObj( m_szVisibleName, &W );
  ((S_ANALRM_IMPL*)W)->Reset();

  if( !s_ksharedVarInfo )
    InitParm();

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_AnAlrm::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( !strcmp(pIn->m_szSink, "IN") )
      (*pIn) >> W->PV;
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
      else
        (*pOut) << W->L;
      break;
    }
  }
}

void K_AnAlrm::OnAfterRestoreState()
{
  W->AlarmEn = 1;
  W->CtrlHiHiPrev = W->CtrlHiHi;
  W->CtrlHiPrev = W->CtrlHi;
  W->CtrlLoLoPrev = W->CtrlLoLo;
  W->CtrlLoPrev = W->CtrlLo;
  W->SetAck = 0;
  W->Acked = 1;
  W->LL = W->L = W->HH = W->H = 0;
  W->SERVICE = m_InitialService;
}

void K_AnAlrm::OnAttributesSetted()
{
  m_InitialService = W->SERVICE;
}

KFlexBlockBase::SReturn K_AnAlrm::StepCalc( SCallParams &params )
{
  SReturn sr;
    ((S_ANALRM_IMPL*)W)->CalcService( params, pRaW );
  if( W->SERVICE )
  {
    W->MODE = 255;
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
    
    if ( !W->AlgOn && W->MODE==3 )
  {
    W->MODE = 0;
  }
  ((S_ANALRM_IMPL*)W)->Calc( params, pRaW );
  
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
