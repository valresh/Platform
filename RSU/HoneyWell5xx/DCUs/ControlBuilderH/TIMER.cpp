#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate TIMER( "TIMER", SH_TIMER::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_TIMER,W_TIMER,50)

void SH_TIMER::InitParm()
{
#include "Blocks/TIMER.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/TIMER_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

void SH_TIMER::StepAfterRestoreState()
{
  curSP = W->SP;
}

class TIMER_IMPL : public W_TIMER
{
  void CalcVars( double &dRV, double &dPV );
public:
  void StepT( SStepCalcParams &dt, int &prevSP );
};

void SH_TIMER::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  TIMER_IMPL *impl = reinterpret_cast<TIMER_IMPL*>(W);
  impl->StepT( dt, curSP );
  curSP = W->SP;
  OutputConnectionsTransfer();
}
void SH_TIMER::OnAssignField( LPCSTR pszFieldName )
{
  //if( strcmp(pszFieldName,"COMMAND") ) return;
  TIMER_IMPL *impl = reinterpret_cast<TIMER_IMPL*>(W);
  SStepCalcParams dt;
  impl->StepT( dt, curSP );
  curSP = W->SP;
}
//////////////////////////////////////////////////////////////////////////
void TIMER_IMPL::StepT( SStepCalcParams &dt, int &prevSP )
{
  if( STARTFL || _COMMAND::START==COMMAND.V )
  {
    if( _lastCOMMAND::RESET==lastCOMMAND.V || _lastCOMMAND::NONE==lastCOMMAND.V )
    {
      STATE = _STATE::RUNNING;
      lastCOMMAND.V = _lastCOMMAND::START;
    }
    STARTFL = 0;
    COMMAND = _COMMAND::NONE;
  }
  if( RESTARTFL || _COMMAND::RESTART==COMMAND.V )
  {
    if( _lastCOMMAND::STOP==lastCOMMAND.V )
    {
      lastCOMMAND.V = _lastCOMMAND::RESTART;
      STATE = _STATE::RUNNING;
    }
    RESTARTFL = 0;
    COMMAND = _COMMAND::NONE;
  }
  if( STOPFL || _COMMAND::STOP==COMMAND.V )
  {
    STOPFL = 0;
    COMMAND = _COMMAND::NONE;
    if( _STATE::RUNNING == STATE.V )
    {
      STATE = _STATE::STOPPED;
      lastCOMMAND.V = _lastCOMMAND::STOP;
    }
  }
  if( RESETFL || _COMMAND::RESET==COMMAND.V )
  {
    lastCOMMAND.V = _lastCOMMAND::RESET;
    if( !SO && _STATE::RUNNING == STATE.V )
      STATE = _STATE::STOPPED;
    SO = 0;
    RESETFL = 0;
    COMMAND = _COMMAND::NONE;
    CalcVars( dRV, dPV );
  }

  if( _STATE::STOPPED == STATE.V )
  {
    CalcVars( dRV, dPV );
  }

  if( _STATE::RUNNING != STATE.V )
    return;
  
  if( !SO )
  {
    double dtTB = dt;
    if( _TIMEBASE::MINUTES==TIMEBASE.V )
      dtTB /= 60.;

    dPV += dtTB;
    dRV -= dtTB;
    if( dRV <= 0 )
    {
      SO = 1;
      STATE = _STATE::STOPPED;
      dRV = 0;
      switch( TIMEBASE.V )
      {
      case _TIMEBASE::SECONDS:
        dPV = SP;
        break;
      case _TIMEBASE::MINUTES:
        dPV = SP;// * 60;
        break;
      default:
        ASSD(0);
      }
    }
  }
  PV = (int)dPV;
  RV = (int)dRV;
}

void TIMER_IMPL::CalcVars( double &dRV, double &dPV )
{
  PV = 0;
  RV = 0;
  switch( TIMEBASE.V )
  {
  case _TIMEBASE::SECONDS:
    RV = SP;
    break;
  case _TIMEBASE::MINUTES:
    RV = SP;// * 60;
    break;
  default:
    ASSD(0);
  }
  dPV = 0;
  dRV = RV;
}
