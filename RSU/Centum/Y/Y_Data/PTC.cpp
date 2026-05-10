#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <SR_DCU.h>


static CY_Create PTC( "PTC", CY_PTC::Create );

CY_PTC::CY_PTC( CPTC *_W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;

  prevP = -DBL_MAX;
  m_timeHolder = 0;

  W->mode.M = SMode::MAN;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_PTC,CPTC,30,55)

void CY_PTC::InitParm()
{
#include "Yokogawa\PTC_P.hpp" 
#include "Yokogawa\PTC.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_PTC::GetOUT ( char * Name )
{
  if( !strcmp(Name,"OUT") )
  {
    ASSD(!"определить что на выход привязывать")
  }
  return NULL;
}

CSignal ** CY_PTC::GetIN ( char * Name )
{
  GET_IN(IN)
  return NULL;
}

void CY_PTC::Init( )
{
}

int CY_PTC::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\PTC_P.hpp" 
  return 0;
}

int CY_PTC::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  prevP = -DBL_MAX;
  m_timeHolder = 0;
  return 0;
}

void CY_PTC::Work ( double dt )
{
  m_timeHolder += dt;
  bExecutedAtStep = false;
  double fullDelta = m_timeHolder;
  if( m_timeHolder >= s_ControlPeriod )
  {
    bExecutedAtStep = true;
    while( m_timeHolder >= s_ControlPeriod )
    {
      m_timeHolder -= s_ControlPeriod;
    }
  }
  else
    return;


  double PVn = -DBL_MAX;
  if( IN )
  {
    double P = -DBL_MAX;
    *IN >> P;
    if( -DBL_MAX != prevP )
    {
      PVn = W->PulseConversionFactor * ( P - prevP ) + W->PV;
    }
    prevP = P;
  }
  if( W->RST )
  {
    W->RST = 0;
    W->PV = 0;
  }
  if( W->HSW )
  {
    W->BSTS = W->_BSTS::PAUS;
    return;
  }
  if( -DBL_MAX==PVn )
    return;

  W->PV = PVn;

  W->DV = W->PH - W->PV;

  double dALM = W->PH - W->DL;
  if( W->PV < dALM )
    W->BSTS = W->_BSTS::NR;
  else
  {
    if( W->PV >= W->PH )
      W->BSTS = W->_BSTS::CTUP;
    else
      W->BSTS = W->_BSTS::PALM;
  }
}

int CY_PTC::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"

  PARM ( W->PV, "#PV" );
  PARM ( W->PH, "#PH" );
  PARM ( W->DV, "#DV" );
  PARM ( W->DL, "#DL" );
  static SYParam::SYParamStrVal vals[] = {{"NR", W->BSTS.NR}, {"PALM", W->BSTS.PALM}, {"CTUP", W->BSTS.CTUP}, {"CTUP", W->BSTS.PAUS} };
  params.Add( W->BSTS.V, "#BSTS", _countof(vals), vals );

  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\PTC_P.hpp" 
  E_MENU("Все параметры")
  MENU("Все состояние")
#include "Yokogawa\PTC.hpp"
  E_MENU("Все состояние")
  MENU("Входы")
  PARM_IN(IN)
  E_MENU("Входы")
  MENU("Base")
  CY_Base::GetParams( params );
  E_MENU("Base")
  return 0;
}

