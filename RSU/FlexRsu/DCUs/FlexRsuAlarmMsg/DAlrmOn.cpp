#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>

struct S_DAlrmOn_P
{
#include "PrivateVars/DigAlrm.h"
};


class K_DAlrmOn
  : public KFlexBlockBase
  , protected S_DAlrmOn_P
{
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_DIGALRM *W;
  SFlex *pRaW;
public:
  K_DAlrmOn()
  {
    m_nullOrderExec = exnOutputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  SReturn StepCalc( SCallParams &params );
  virtual void OnAfterRestoreState();
  static void InitParm();
};

static KBlockCreator _DigAlrm("DALRMON", K_DAlrmOn::Create);

KFlexBlockBase *K_DAlrmOn::Create()
{
  return new K_DAlrmOn;
}

static LPCSTR s_pszON = "$Red.ON";
// желтые
static LPCSTR s_pszAlgOnAlarm  = "$Yellow.Выведен из алгоритма";
// зеленые
static LPCSTR s_pszImitOnAlarm  = "$Green.Имитация включена";
static LPCSTR s_pszFiltOnAlarm  = "$Green.Фильтр включен";
static LPCSTR s_pszRepairOnAlarm  = "$Green.Выведен в ремонт";
static LPCSTR s_pszCtrlCircuitOnAlarm = "$Green.Контроль цепи включен";
// белые
static LPCSTR s_pszImitOffAlarm  = "$White.Имитация выключена";
static LPCSTR s_pszAlgOffAlarm  = "$White.Ввведен в алгоритм";
static LPCSTR s_pszFiltOffAlarm  = "$White.Фильтр выключен";
static LPCSTR s_pszRepairOffAlarm  = "$White.Выведен из ремонта";
static LPCSTR s_pszCtrlCircuitOffAlarm = "$White.Контроль цепи выключен";

namespace ns_DAlrmOn
{
  #include <_InitInfo.h>
  DECLARE_PRIVATE_VARSINFO( K_DAlrmOn, 10 )
  DECLARE_SHARED_VARSINFO( S_DIGALRM, 26 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_DAlrmOn;
void K_DAlrmOn::InitParm()
{
#include <FlexBlocks/DigAlrm.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/DigAlrm.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_DAlrmOn::OnCreated()
{
  pRaW = CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_DAlrmOn::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( !strcmp(pIn->m_szSink, "IN") )
      (*pIn) >> PV;
  }
}

void K_DAlrmOn::OnAfterRestoreState()
{
  W->STATE = 0;
  W->SetAck = 0;
  W->Acked = 1;
  PV = PREVPV = 0;
}
struct S_DIGALRM_Impl : S_DIGALRM
{
  void CalcService ( KFlexBlockBase::SCallParams &params, SFlex *pRaW);
  void Calc( KFlexBlockBase::SCallParams &params, SFlex *pRaW )
    {
 //
        if( FiltOn )
    {
        if( !FiltOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszFiltOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
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
            pFlexAlarm( pRaW, true, s_pszFiltOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
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
            pFlexAlarm( pRaW, true, s_pszAlgOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          AlgOffAlarm = 1;
        }
    }
        else if( AlgOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszAlgOffAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          AlgOffAlarm = 0;
        }
//
        if( ImitOn )
    {
        if( !ImitOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszImitOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
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
        if( !ImitOn )
    {
        if( !ImitOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszImitOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          ImitOffAlarm = 1;
        }
    }
        else if( ImitOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszImitOnAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          ImitOffAlarm = 0;
        }
//
        if( CtrlCircuit )
    {
        if( !CtrlCircuitOnAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlCircuitOnAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          ImitOnAlarm = 1;
        }
    }
        else if( CtrlCircuitOnAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszImitOnAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          CtrlCircuitOnAlarm = 0;
        }
//
        if( !CtrlCircuit )
    {
        if( !CtrlCircuitOffAlarm )
        {
          SetAck = 0;
          Acked = 0;
          if( pFlexAlarm )
            pFlexAlarm( pRaW, true, s_pszCtrlCircuitOffAlarm, params.modelTime, params.pszDrName, DESCR, "", "", "", Acked, "", "", "" );
          ImitOffAlarm = 1;
        }
    }
        else if( CtrlCircuitOffAlarm )
        {
          //if (pFlexAlarm)
            //pFlexAlarm( pRaW, false, s_pszImitOnAlarm, params.modelTime, params.pszDrName, DESCR, szAtValue, Acked );
          CtrlCircuitOffAlarm = 0;
        }
    if( SetAck )
    {
      if( !Acked && pFlexAckingAlarm )
        pFlexAckingAlarm( pRaW, params.modelTime, AlgOnAlarm || AlgOffAlarm || ImitOnAlarm || ImitOffAlarm || CtrlCircuitOnAlarm || CtrlCircuitOffAlarm || FiltOnAlarm || FiltOffAlarm );
      Acked = 1;
      SetAck = 0;
    }
  }
};



void S_DIGALRM_Impl::CalcService ( KFlexBlockBase::SCallParams &params, SFlex *pRaW)
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
        pFlexAckingAlarm( pRaW, params.modelTime, RepairOnAlarm || RepairOffAlarm );
      Acked = 1;
      SetAck = 0;
    }
  }
KFlexBlockBase::SReturn K_DAlrmOn::StepCalc( SCallParams &params )
{
  SReturn sr;
  ((S_DIGALRM_Impl*)W)->CalcService( params, pRaW );
  if( W->Repair )
  {
    W->MODE = 255;
    PV = 0;
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
      if ( W->ImitOn )
      PV = W->ImitVal;
      if( PV != PREVPV )
      {
       W->STATE = PV;
        if( pFlexAlarm )
        {
          char szAtValue[32] = {};
          snprintf( szAtValue, _countof(szAtValue), "%d", PV );
          if( W->STATE )
          {
            W->SetAck = 0;
            W->Acked = 0;
            pFlexAlarm( pRaW, true, s_pszON, params.modelTime, params.pszDrName, W->DESCR, "", "", "", W->Acked, "", "", "" );
          }
          else
            pFlexAlarm( pRaW, false, s_pszON, params.modelTime, params.pszDrName, W->DESCR, "", "", "", W->Acked, "", "", "" );
        }
        if( W->SetAck )
      {
        if( !W->Acked && pFlexAckingAlarm )
          pFlexAckingAlarm( pRaW, params.modelTime, W->STATE ? true : false );
        W->Acked = 1;
        W->SetAck = 0;
      }
      PREVPV = PV;
      ((S_DIGALRM_Impl*)W)->Calc( params, pRaW );
    }
  return sr;
}