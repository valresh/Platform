#include <rsuErr.h>
#include <H_Data.h>
#include <macros/AutoRestore.h>

static SModuleCreate CONTROLMODULE( "CONTROLMODULE", S_CONTROLMODULE::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_CONTROLMODULE,W_CONTROLMODULE,82)

void S_CONTROLMODULE::InitParm()
{
#include "Modules/CONTROLMODULE.h" 
s_defFlag = SVarInfo::efParam;
#include "Modules/CONTROLMODULE_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

void S_CONTROLMODULE::StepAfterRestoreState()
{
  SH_Module::StepAfterRestoreState();
  sprintf_s( W->LOCATIONFULLITEMNAME, "/Assets/%s/%s", W->PARENTASSET, W->NAME );
  switch( W->PERIOD.V )
  {
  case W_CONTROLMODULE::_PERIOD::_50ms:
    periodSec = 0.05;
    break;
  case W_CONTROLMODULE::_PERIOD::_100ms:
    periodSec = 0.1;
    break;
  case W_CONTROLMODULE::_PERIOD::_200ms:
    periodSec = 0.2;
    break;
  case W_CONTROLMODULE::_PERIOD::_500ms:
    periodSec = 0.5;
    break;
  default:
    ASSD(!"что-то совсем плохо");
  case W_CONTROLMODULE::_PERIOD::_1sec:
  case W_CONTROLMODULE::_PERIOD::DEFAULT:
    periodSec = 1.;
    break;
  case W_CONTROLMODULE::_PERIOD::_2sec:
    periodSec = 2.;
    break;
  }
  maxPhaseIndex = periodSec / 0.05;
}

void S_CONTROLMODULE::StepT( SStepCalcParams &dt )
{
  if( !dt.usePhases )
      SH_Module::StepT( dt );
  else
  {
    if( W->activePhase==W->PHASE )
    {
      TAutoRestore<double> arPeriod(dt.periodSec);
      dt.periodSec = periodSec;
      SH_Module::StepT( dt );
    }
    ++W->activePhase;
    if( W->activePhase >= maxPhaseIndex )
      W->activePhase = 0;
  }
}