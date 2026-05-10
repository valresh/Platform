#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"


static CY_Create INDST2 ( "INDST2", CY_INDST2::Create );

CY_INDST2::CY_INDST2(CINDST2 * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  //
  JMOD = NULL;
  INT = NULL;
  JBST = NULL;
  JALM = NULL;
  JPV = NULL;
  JSV = NULL;
  J01 = NULL;
  J02 = NULL;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_INDST2,CINDST2,20,30)

void CY_INDST2::InitParm()
{
#include "Yokogawa\INDST2_P.hpp" 
#include "Yokogawa\INDST2.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_INDST2::GetOUT ( char * Name )
{
  return NULL;
}

CSignal ** CY_INDST2::GetIN ( char * Name )
{
  GET_IN(JMOD)
  GET_IN(INT)
  GET_IN(JBST)
  GET_IN(JALM)
  GET_IN(JPV)
  GET_IN(JSV)
  GET_IN(J01)
  GET_IN(J02)
  return NULL;
}

void CY_INDST2::Work ( double dt )
{
  SET_HBP
  {
    KKK();
  }
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    return ;
  }
  if ( JMOD )
  {
    int M;
    *JMOD >> M;
    W->mode.M = M;
  }
  if ( JBST )
  {
    int I;
    *JBST >> I;
    W->BSTS = I;
  }
  if ( JALM )
  {
    int A;
    *JALM >> A;
    W->ALRM = A;
  }
  if ( JPV )
    *JPV >> W->PV;
  if ( JSV )
    *JSV >> W->SV;
  if ( J01 )
    *J01 >> W->SVH;
  if ( J02 )
    *J02 >> W->SVL;
}

void CY_INDST2::Init( )
{
}

int CY_INDST2::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\INDST2_P.hpp" 
  return 0;
}

int CY_INDST2::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  return 0;
}

int CY_INDST2::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM ( W->PV, "#PV" );
  PARM ( W->SV, "#SV" );
  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\INDST2_P.hpp" 
  E_MENU("Все параметры")
  MENU("Все состояние")
#include "Yokogawa\INDST2.hpp"
  E_MENU("Все состояние")
  MENU("Входы")
  PARM_IN(JMOD)
  PARM_IN(INT)
  PARM_IN(JBST)
  PARM_IN(JALM)
  PARM_IN(JPV)
  PARM_IN(JSV)
  PARM_IN(J01)
  PARM_IN(J02)
  E_MENU("Входы")
  MENU("Base")
  CY_Base::GetParams( params );
  E_MENU("Base")
  return 0;
}

bool CY_INDST2::SetLimits( )
{
  return true;
}
