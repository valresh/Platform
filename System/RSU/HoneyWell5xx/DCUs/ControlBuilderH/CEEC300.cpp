#include <rsuErr.h>
#include <H_Data.h>
#include <macros/AutoRestore.h>

static SModuleCreate CEEC300( "CEEC300", S_CEEC300::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_CEEC300,W_CEEC300,51)

void S_CEEC300::InitParm()
{
#include "Modules/CEEC300.h" 
s_defFlag = SVarInfo::efParam;
#include "Modules/CEEC300_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

void S_CEEC300::StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule )
{
  ASS( !pSystemName );
  SH_Module::StepBeforeRestoreState( BlockName, pRoot, pModule );
}

static const double s_Kvant = 0.05;

void S_CEEC300::StepT( SStepCalcParams &dt )
{  
  if( !dt.usePhases )
    SH_Module::StepT( dt );
  else
  {
    TAutoRestore<double> arPeriod(dt.periodSec), arkvant(dt.kvantCeeSec);
    W->timeHolder4Phase += dt.periodSec;
    dt.periodSec = dt.kvantCeeSec = s_Kvant;
    while( W->timeHolder4Phase >= s_Kvant )
    {
      SH_Module::StepT( dt );
      W->timeHolder4Phase -= s_Kvant;
    }
  }
}