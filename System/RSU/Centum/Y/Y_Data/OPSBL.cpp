#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <_CALCU.h>
#include <yokoheaders.h>

static CY_Create OPSBL( "OPSBL", CYOPSBL::Create );

CYOPSBL::CYOPSBL( COPSBL * _W )
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  W->BSTS.V = COPSBL::_BSTS::STOP;
}

bool CYOPSBL::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( !strncmp(ParmName, "HISR", 4) )
    return C_SFC_BASE_Impl::ReadSfc( ParmName, ParmValue );

  return __super::ReadParm(ParmName, ParmValue, pData);
}

#include "Yokogawa\YPARM_INIT.h" 


LIST_PARM(CYOPSBL,COPSBL,15,30)

void CYOPSBL::InitParm()
{
#include "Yokogawa\OPSBL.hpp"
#include "Yokogawa\OPSBL_P.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

void CYOPSBL::Work ( double dt )
{
  SET_HBP
  {
    KKK();
  }
  if( SMode::AUT!=W->mode.M )
    return;

  if( !pAlg )
    return;

  if( !CY_Base::s_flagBasicScanControl )
    return;
  double deltaT = CY_Base::s_ControlPeriod * m_nBasicControlPeriodCounter;
  m_nBasicControlPeriodCounter = 0;

  W->FlagUse |= CAlarmBase::fuYConnect;
  pAlg->m_pOwner = this;

  if( W->PREBSTS.V != W->BSTS.V )
    pAlg->OnStatusChange( W, deltaT );

  if( COPSBL::_BSTS::RUN==W->BSTS.V )
    pAlg->Work( W, deltaT );

  if( W->PREBSTS.V != W->BSTS.V )
    pAlg->OnStatusChange( W, deltaT );

  W->PREBSTS.V = W->BSTS.V;
}

void CYOPSBL::Init( )
{
}

int CYOPSBL::Prepare0()
{
  pAlg = CAlg::Find( Name );
  if( pAlg )
    pAlg->Prepare0();
  return 0;
}

int CYOPSBL::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  if( pAlg )
  {
    pAlg->Prepare1();
  }
  return 0;
}

int CYOPSBL::SaveState( )
{
  if( pAlg )
  {
    pAlg->SaveState();
  }
  return 0;
}

int CYOPSBL::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  if( pAlg )
  {
    if( pAlg->RestoreState( StrName ) )
      return 2;
    return 0;
  }
  return 1;
}

int CYOPSBL::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM( W->STEPNO, "#STEPNO" );

  static SYParam::SYParamStrVal vals[] = {{"STOP", W->BSTS.STOP}, {"PAUS", W->BSTS.PAUS}, {"RUN", W->BSTS.RUN} };
  params.Add( W->BSTS.V, "#BSTS", _countof(vals), vals );

  PARMS( W->RCP_FILE, "#файл рецепта" );

  MENU("Base")
    CY_Base::GetParams( params );
  E_MENU("Base")

    MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\OPSBL_P.hpp" 
    E_MENU("Все параметры")
    MENU("Все состояние")
#include "Yokogawa\OPSBL.hpp"
    E_MENU("Все состояние")
    return 0;
}

bool CYOPSBL::SetLimits( )
{
  return true;
}

#include <sbsts.h>
void CYOPSBL::OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue )
{
  if( !strcmp(pszParm, "BSTS") )
  {
    LPSTR pEnd = NULL;
    DWORD v = strtoul(pszValue, &pEnd, 10 );
    switch( v )
    {
    case BS_STOP:
      W->BSTS.V = COPSBL::_BSTS::STOP;
      W->PREBSTS.V = COPSBL::_PREBSTS::STOP;
      break;
    case BS_RUN:
      W->BSTS.V = COPSBL::_BSTS::RUN;
      W->PREBSTS.V = COPSBL::_PREBSTS::RUN;
      break;
    case BS_WAIT:
      W->BSTS.V = COPSBL::_BSTS::PAUS;
      W->PREBSTS.V = COPSBL::_PREBSTS::PAUS;
      break;
    case BS_END:
#pragma message("!!!в будующем реализовать UTSW BSTS\n")//111_BITUM_MSK
      W->BSTS.V = COPSBL::_BSTS::STOP;
      W->PREBSTS.V = COPSBL::_PREBSTS::STOP;
      break;
    default:
      ASSD(!"подобрать");
    }
  }
}
