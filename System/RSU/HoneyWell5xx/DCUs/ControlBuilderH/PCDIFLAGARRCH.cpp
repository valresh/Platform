#include <rsuErr.h>
#include <H_Class.h>

static SBlockCreate PCDIFLAGARRCH( "PCDIFLAGARRCH", SH_PCDIFLAGARRCH::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_PCDIFLAGARRCH,W_PCDIFLAGARRCH,10005)

void SH_PCDIFLAGARRCH::InitParm()
{
#include "Blocks/PCDIFLAGARRCH.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/PCDIFLAGARRCH_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

void SH_PCDIFLAGARRCH::StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule )
{
  //MessageBox( NULL, __FILE__, "SH_PCDIFLAGARRCH", MB_OK );
#pragma message( "восстановить\n" )
  /*ZeroMemory( pRegisters, sizeof(pRegisters) );
  char name[64] = { 0 };
  CValue **pr = &pRegisters[1];
  for( int n=0; n<=W->NFLAG; ++n, ++pr )
  {
    sprintf_s( name, "%s.%d", pSystemName, W->STARTINDEX + n );
    ::GiveCommData( name, CValue::TypeID, (void**)pr );
    ASS( *pr );
    if( !*pr )
      continue;
    (*pr)->eType = enumValueBol;
  }*/
}

void SH_PCDIFLAGARRCH::StepT( SStepCalcParams &dt )
{
  #pragma message( "восстановить\n" )
  InputConnectionsTransfer();
  /*for( int i=0; i<=W->NFLAG; ++i )
  {
    CValue *pr = pRegisters[i];
    if( !pr )
      continue;
    if( W->WRITEOPT==W->_WRITEOPT::ReadOnly )
      W->PV[i] = W->PVFL[i] = pr->bVal;
    else if( W->WRITEOPT==W->_WRITEOPT::WriteOnDiff )
      pr->bVal = W->PVFL[i] ? true : false;
    else
    {
      ASS(0);
    }
  }*/
  OutputConnectionsTransfer();
}
