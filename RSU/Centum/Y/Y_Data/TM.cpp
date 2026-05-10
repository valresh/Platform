#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create TM ( "TM", CY_TM::Create );

CY_TM::CY_TM( CTM * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
	OUT.Name = "OUT";
  OUT << W->PV;
  prevOP = 0;
  prevPausBSTS = 255;
	}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_TM,CTM,10,25)

void CY_TM::InitParm()
  {
#include "Yokogawa\TM_P.hpp" 
#include "Yokogawa\TM.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_TM::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_TM::GetIN ( char * Name )
	{
	return NULL;
	}

template<typename S, typename D>
class KCopyPV
{
  S &m_src;
  D &m_dst;
public:
  KCopyPV( S &src, D &dst ) : m_src(src), m_dst(dst)
  {}
  ~KCopyPV()
  {
    double intptr = 0;
    double frac = modf( m_src, &intptr );
    m_dst = (int)intptr;
  }
};

void CY_TM::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}

  if( !CY_Base::s_flagBasicScanControl )
    return;

  KCopyPV<double, int> copier(W->dblPV, W->PV);

  double deltaT = CY_Base::s_ControlPeriod * m_nBasicControlPeriodCounter;
  m_nBasicControlPeriodCounter = 0;

  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    return;
  }

  switch( W->OP.V )
  {
  case W->OP.STOP:
  case W->OP.START:
    checkOPstart_stop();
    break;
  case W->OP.RSTR:
    checkOPstart_stop();
    break;
  case W->OP.WAIT:
    if( 255==prevPausBSTS && W->BSTS.PAUS!=W->BSTS.V )
      prevPausBSTS = W->BSTS.V;
    W->BSTS = W->BSTS.PAUS;
    break;
  }

  if ( W->BSTS == W->BSTS.STOP || W->BSTS == W->BSTS.PAUS )
    return;
  if ( W->BSTS == W->BSTS.CTUP )
    return;
  
  if ( W->SecMin )
    deltaT /= 60.;

  W->dblPV += deltaT;

  if ( W->dblPV >= W->PH )
  {
    if ( W->Periodical )
    {
      W->dblPV = 0.;
      W->BSTS = W->BSTS.NR;
      W->DV = W->PH;
    }
    else
    {
      W->dblPV = W->PH;
      W->BSTS = W->BSTS.CTUP;
      W->DV = 0.;
    }
  }
  else
  {
    W->DV = W->PH - W->dblPV;
    if ( W->DV <= W->DL )
      W->BSTS = W->BSTS.PALM;
    else
      W->BSTS = W->BSTS.NR;
  }
}

void CY_TM::OnNoAssignedByFalseState( void *pAddr, LPCVOID pCanByValue )
{
  if( pAddr == &W->OP )
  {
    if( !pCanByValue )
      return;
    BYTE canValue = *(BYTE*)pCanByValue;
    switch( canValue )
    {
    /*case W->OP.STOP:
      prevOP = W->OP;
      W->BSTS = W->BSTS.STOP;
      break;*/
    case W->OP.START:
      //хотели стартануть, скорее всего по PV.ON
      W->OP = W->OP.STOP;
      /*if( W->BSTS.V != W->BSTS.STOP )
      {
        prevOP = W->OP;
        W->BSTS = W->BSTS.STOP;
        W->PV = 0;
        W->dblPV = 0;
      }*/
      break;
    /*case W->OP.RSTR:
      if( 255 != prevPausBSTS )
        W->BSTS = prevPausBSTS;
      else
        W->BSTS = W->BSTS.RUN;
      prevPausBSTS = 255;
      prevOP = W->OP;
      break;*/
    }
  }
}

void CY_TM::OnAssigned( void *pAddr )
{
  if( pAddr == &W->OP )
  {
    switch( W->OP.V )
    {
    case W->OP.STOP:
      prevOP = W->OP;
      W->BSTS = W->BSTS.STOP;
      break;
    case W->OP.START:
      if( W->OP.V != prevOP )
      {
        W->PV = 0;
        W->dblPV = 0;
      }
      W->BSTS = W->BSTS.RUN;
      prevOP = W->OP;
      break;
    case W->OP.RSTR:
      if( 255 != prevPausBSTS )
        W->BSTS = prevPausBSTS;
      else
        W->BSTS = W->BSTS.RUN;
      prevPausBSTS = 255;
      prevOP = W->OP;
      break;
    }
  }
}

CY_Base::ELActLC64 CY_TM::IsLActionInLC64( LPCSTR pszAct )
{
  if( !_strcmpi(pszAct,"OP.START") )
    return elyesL;
  return elHZ;
}

void CY_TM::checkOPstart_stop()
{
  switch( W->OP.V )
  {
  case W->OP.STOP:
    prevOP = W->OP;
    W->BSTS = W->BSTS.STOP;
    break;
  case W->OP.START:
    if( W->OP.V != prevOP )
    {
      W->BSTS = W->BSTS.RUN;
      W->PV = 0;
      W->dblPV = 0;
    }
    prevOP = W->OP;
    break;
  case W->OP.RSTR:
    if( W->OP.V != prevOP )
    {
      if( 255 != prevPausBSTS )
        W->BSTS = prevPausBSTS;
      else
        W->BSTS = W->BSTS.RUN;
      prevPausBSTS = 255;
    }
    prevOP = W->OP;
    break;
  }
}

void CY_TM::Init( )
	{
	}

int CY_TM::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\TM_P.hpp" 
	return 0;
	}


int CY_TM::Prepare1()
{
  //W->PL = 0.;
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  prevOP = W->OP;
  if( W->OP.V == W->OP.RSTR )
    W->BSTS = W->BSTS.RUN;

  if( !W->SecMin )
  {//оставляю целую часть
    W->PV = (int)W->PV;
  }

	return 0;
}

int CY_TM::SaveState( )
	{
	OUT.SaveState();
	return 0;
	}

int CY_TM::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
	}

#include "IO_Parms.h"

int CY_TM::GetParams( KYBridge2SysParam &params )
	{
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->Comment, "#Comment" );
  PARM ( W->PV, "#PV" );
  PARM ( W->OP.V, "#OP" );
  {
    static SYParam::SYParamStrVal vals[] = {{"nothing", W->OP.nothing}, {"STOP", W->OP.STOP}, {"RSTR", W->OP.RSTR}, {"START", W->OP.START}, {"WAIT", W->OP.WAIT}};
    params.Add( W->OP.V, "#@OP", _countof(vals), vals );
  }
  PARM ( W->BSTS.V, "#BSTS" );
  {
    static SYParam::SYParamStrVal vals[] = {{"STOP", W->BSTS.STOP}, {"NR", W->BSTS.NR}, {"PALM", W->BSTS.PALM}, {"CTUP", W->BSTS.CTUP}, {"RUN", W->BSTS.RUN}, {"PAUS", W->BSTS.PAUS}};
    params.Add( W->BSTS.V, "#@BSTS", _countof(vals), vals );
  }
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\TM_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\TM.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
    PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_TM::SetLimits( )
  {
  return true;
  }

