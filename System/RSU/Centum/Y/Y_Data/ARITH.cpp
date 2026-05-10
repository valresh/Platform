#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
                                                
static CY_Create ARITH_ADD ( "ADD", CY_ARITH::Create, CY_ARITH::eADD );
static CY_Create ARITH_DIV ( "DIV", CY_ARITH::Create, CY_ARITH::eDIV );
static CY_Create ARITH_NOT ( "NOT", CY_ARITH::Create, CY_ARITH::eNOT );
static CY_Create ARITH_OR ( "OR", CY_ARITH::Create, CY_ARITH::eOR );
static CY_Create ARITH_AND ( "AND", CY_ARITH::Create, CY_ARITH::eAND );
static CY_Create ARITH_GT ( "GT", CY_ARITH::Create, CY_ARITH::eGT );
static CY_Create ARITH_GE ( "GE", CY_ARITH::Create, CY_ARITH::eGE );
static CY_Create ARITH_SQRT( "SQRT", CY_ARITH::Create, CY_ARITH::eSQRT );
static CY_Create ARITH_MUL( "MUL", CY_ARITH::Create, CY_ARITH::eMUL );
static CY_Create ARITH_EQ( "EQ", CY_ARITH::Create, CY_ARITH::eEQ );

CY_ARITH::CY_ARITH( char * _Name, eType _TypeOP, int number, int domain ) : CY_Base()
{
  TypeOP = _TypeOP;
  CPV_RV = false;
  switch ( TypeOP )
    {
    case eADD:
      W = (CARITH*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CADD::TypeID, number, domain );
    break;
    case eDIV:
      W = (CARITH*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CDIV::TypeID, number, domain );
    break;
    case eNOT:
      W = (CARITH*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CNOT::TypeID, number, domain );
    break;
    case eOR:
      W = (CARITH*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, COR::TypeID, number, domain );
    break;
    case eAND:
      W = (CARITH*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CAND::TypeID, number, domain );
    break;
    case eGT:
      W = (CARITH*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CGT::TypeID, number, domain );
    break;
    case eGE:
      W = (CARITH*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CGE::TypeID, number, domain );
    break;
    case eSQRT:
      W = (CARITH*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSQRT::TypeID, number, domain );
      break;
    case eMUL:
      W = (CARITH*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CMUL::TypeID, number, domain );
      break;
    case eEQ:
      W = (CARITH*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CEQ::TypeID, number, domain );
      break;
    default:
      ASS( 0 );
    }
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
//
	IN = NULL;
	Q01 = NULL;
	Q02 = NULL;
	OUT.Name = "OUT";
  OUT << W->CPV;
}

CSignal * CY_ARITH::GetOUT ( char * Name )
	{
  GETOUT
  GETSUB
  return NULL;
	}

CSignal ** CY_ARITH::GetIN ( char * Name )
	{
	GET_IN(IN)
	GET_IN(Q01)
	GET_IN(Q02)
	return NULL;
	}
//YOKO   1
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_ARITH,CARITH,30,50)

void CY_ARITH::InitParm()
  {
#include "Yokogawa\ARITH_P.hpp" 
#include "Yokogawa\ARITH.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  _W.BIAS = 0;
  _W.BS1 = 0;
  _W.GAIN = 1.;
  _W.GN1 = 1.;
  }

void CY_ARITH::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}

  double prevCV = W->CPV;
  if ( CPV_RV )
  {
    GetIN();
    W->CPV = W->RV = RV;
    dCPV = W->CPV - prevCV;
    return;
  }
  switch ( TypeOP )
  {
  case eADD:
    {
	  ADD();
    }
  break;
  case eDIV:
    {
		if ( W->GN1 == 0. )
			W->GN1 = 1.;
		if ( W->GAIN == 0. )
			W->GAIN = 1.;
	  DIV();
    }
  break;
  case eNOT:
    {
	  NOT();
    }
  break;
  case eOR:
    {
	  OR();
    }
  break;
  case eAND:
    {
	  AND();
    }
  break;
  case eGT:
    {
	  GT();
    }
  break;
  case eGE:
    {
	  GE();
    }
  break;
  case eSQRT:
    SQRT();
    break;
  case eMUL:
    MUL();
    break;
  case eEQ:
    {
      EQ();
    }
  }
  dCPV = W->CPV - prevCV;
}

void CY_ARITH::Init( )
{
  switch ( W->Aux )//CPV, dCPV
  {
  case 0:
    SUB << W->CPV;
    break;
  case 1:
    SUB << dCPV;
    break;
  default:
    ASS(0);
  }
}

int CY_ARITH::Prepare0()
	{
//YOKO 3
  switch ( TypeOP )
    {
    case eADD:
    case eDIV:
    case eGT:
    case eGE:
    case eMUL:
      {
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\ARITH_P.hpp" 
      }
    break;
    }
	return 0;
	}

int CY_ARITH::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  Prepare0();
	return 0;
}

static LPCSTR s_Name = "W";
int CY_ARITH::SaveState( )
{
	OUT.SaveState();
  SAVE_CLASS(s_Name,CY_ARITH_W)
	return 0;
}

int CY_ARITH::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS(s_Name,CY_ARITH_W)
	return 1;
}

int CY_ARITH::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  OUT.StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 7 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "RV", RV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "RV1", RV1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "RV2", RV2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "iRV1", iRV1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "iRV2", iRV2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "OutStatus", OutStatus );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dCPV", dCPV );

  return 0;
}

int CY_ARITH::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  OUT.StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 7 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "RV", RV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "RV1", RV1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "RV2", RV2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "iRV1", iRV1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "iRV2", iRV2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "OutStatus", OutStatus );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dCPV", dCPV );

  return 0;
}

int CY_ARITH::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
//	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->CPV, "#CPV" );
  PARM ( CPV_RV, "CPV = RV" );
//YOKO 4
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\ARITH_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\ARITH.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN)
	PARM_IN(Q01)
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_ARITH::SetLimits( )
  {
  if ( IN )
    {
    //IN ->Min = W->SL;
    //IN ->Max = W->SH;
    //IN ->IsLimits = true;
    }
  return true;
  }

void CY_ARITH::GetIN()
  {
  bool BadRV = false;
  bool BadRV1 = false;
  if ( IN && IN->Status & CSignal::BAD )
    BadRV = true;
  if ( Q01 && Q01->Status & CSignal::BAD )
    BadRV1 = true;
  if ( !BadRV && !BadRV1 )
    {
		if ( IN )
			*IN >> RV;
    else
      RV = W->RV;
		if ( Q01 )
			*Q01 >> RV1;
    else
      RV1 = W->RV1;
    OutStatus = 0;
    return ;
    }
  OutStatus = 0;
  if ( IN && !BadRV )
    *IN >> RV;
  else
    RV = W->RV;
  if ( Q01 && !BadRV1 )
    *Q01 >> RV1;
  else
    RV1 = W->RV1;
  switch ( W->Calc_Val_Err )
    {
    case 0:
    case 1:
      {
      if ( BadRV )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 2:
      {
      if ( BadRV1 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 3:
      {
      if ( BadRV && BadRV1 )
        OutStatus = CSignal::BAD;
      else
        if ( BadRV )
          OutStatus = CSignal::QST;
      }
    break;
    case 4:
      {
      if ( BadRV && BadRV1 )
        OutStatus = CSignal::BAD;
      else
        if ( BadRV1 )
          OutStatus = CSignal::QST;
      }
    break;
    case 5:
      {
      if ( BadRV && BadRV1 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 6:
      {
      if ( BadRV || BadRV1 )
        OutStatus = CSignal::BAD;
      }
    break;
    }
  }

void CY_ARITH::iGetIN()
{
  bool BadRV1 = false;
  bool BadRV2 = false;
  OutStatus = 0;
  if( Q01 )
  {
    if( Q01->Status & CSignal::BAD )
      BadRV1 = true;
    else
      W->RV1 = int(*Q01);
  }
  if( Q02 )
  {
    if( Q02->Status & CSignal::BAD )
      BadRV2 = true;
    else
      W->RV2 = int(*Q02);
  }
  iRV1 = (int)W->RV1;
  iRV2 = (int)W->RV2;

  if( !BadRV1 && !BadRV2 )
    return;

  switch ( W->Calc_Val_Err )
    {
    case 0:
    case 1:
      {
      if ( BadRV1 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 2:
      {
      if ( BadRV2 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 3:
      {
      if ( BadRV1 && BadRV2 )
        OutStatus = CSignal::BAD;
      else
        if ( BadRV1 )
          OutStatus = CSignal::QST;
      }
    break;
    case 4:
      {
      if ( BadRV1 && BadRV2 )
        OutStatus = CSignal::BAD;
      else
        if ( BadRV2 )
          OutStatus = CSignal::QST;
      }
    break;
    case 5:
      {
      if ( BadRV1 && BadRV2 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 6:
      {
      if ( BadRV1 || BadRV2 )
        OutStatus = CSignal::BAD;
      }
    break;
    }
  }

void CY_ARITH::dGetIN()
 {
  bool BadRV1 = false;
  bool BadRV2 = false;
  if( Q01 )
  {
    if( Q01->Status & CSignal::BAD )
      BadRV1 = true;
    else
      *Q01 >> W->RV1;
  }
  if( Q02 )
  {
    if( Q02->Status & CSignal::BAD )
      BadRV2 = true;
    else
      *Q02 >> W->RV2;
  }
  RV1 = W->RV1;
  RV2 = W->RV2;
  OutStatus = 0;
  
  if( !BadRV1 && !BadRV2 )
    return;

  switch ( W->Calc_Val_Err )
    {
    case 0:
    case 1:
      {
      if ( BadRV1 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 2:
      {
      if ( BadRV2 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 3:
      {
      if ( BadRV1 && BadRV2 )
        OutStatus = CSignal::BAD;
      else
        if ( BadRV1 )
          OutStatus = CSignal::QST;
      }
    break;
    case 4:
      {
      if ( BadRV1 && BadRV2 )
        OutStatus = CSignal::BAD;
      else
        if ( BadRV2 )
          OutStatus = CSignal::QST;
      }
    break;
    case 5:
      {
      if ( BadRV1 && BadRV2 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 6:
      {
      if ( BadRV1 || BadRV2 )
        OutStatus = CSignal::BAD;
      }
    break;
    }
  }

void CY_ARITH::ADD()
  {
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  GetIN();
  OUT.Set( CSignal::BAD | CSignal::QST, OutStatus );
  W->RV = RV;
  W->RV1 = RV1;
  if ( OutStatus == CSignal::BAD )
    return ;
  if( W->CALI )
    return;
  W->CPV = W->GAIN * ( RV + ( W->GN1 * RV1 + W->BS1 )) + W->BIAS;  
  }

void CY_ARITH::DIV()
  {
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  GetIN();
  W->RV = RV;
  W->RV1 = RV1;
  OUT.Set( CSignal::BAD | CSignal::QST, OutStatus );
  if ( OutStatus == CSignal::BAD )
    return ;
  double D = W->GN1 * RV1 + W->BS1;
  if ( fabs ( RV ) > 1e10 * fabs ( D ))
    {
    W->CPV = 0.;
    OUT.On( CSignal::BAD );
    return ;
    }
  OUT.Off( CSignal::BAD );
  if( W->CALI )
    return;
  W->CPV = W->GAIN * ( RV / D ) + W->BIAS;  
  }

void CY_ARITH::NOT()
  {
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  if ( IN != NULL )
  {
  if ( IN->Status & CSignal::BAD )
    OutStatus = CSignal::BAD;
  else
    {
    iRV1 = int(*IN);
    OutStatus = 0;
    }
  }
  else
    iRV1 = (int)W->RV;
  OUT.Set( CSignal::BAD, OutStatus );
  W->RV = iRV1;
  /*if ( OutStatus == CSignal::BAD )
    return ;*/
  if( W->CALI )
    return;
  if ( iRV1 )
    W->CPV = 0;
  else
    W->CPV = 1;
  }
                 
void CY_ARITH::OR()
  {
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  iGetIN();
  OUT.Set( CSignal::BAD | CSignal::QST, OutStatus );
  W->RV1 = iRV1;
  W->RV2 = iRV2;
  if ( OutStatus == CSignal::BAD )
    return ;
  if( W->CALI )
    return;
  if ( iRV1 || iRV2 )
    W->CPV = 1;
  else
    W->CPV = 0;
  }

void CY_ARITH::GT()
  {
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  dGetIN();
  OUT.Set( CSignal::BAD | CSignal::QST, OutStatus );
  W->RV1 = RV1;
  W->RV2 = RV2;
  if ( OutStatus == CSignal::BAD )
    return ;
  if( W->CALI )
    return;
  if ( RV1 > RV2 )
    W->CPV = 1;
  else
    W->CPV = 0;
  }

void CY_ARITH::GE()
  {
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  dGetIN();
	W->RV1 = RV1;
	W->RV2 = RV2;
  OUT.Set( CSignal::BAD | CSignal::QST, OutStatus );
  if ( OutStatus == CSignal::BAD )
    return ;
  if( W->CALI )
    return;
 if ( RV1 >= RV2 )
    W->CPV = 1;
  else
    W->CPV = 0;
  }

void CY_ARITH::EQ()
{
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
    return ;
  }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  dGetIN();
  W->RV1 = RV1;
  W->RV2 = RV2;
  OUT.Set( CSignal::BAD | CSignal::QST, OutStatus );
  if ( OutStatus == CSignal::BAD )
    return;
  if( W->CALI )
    return;
  if ( RV1 == RV2 )
    W->CPV = 1;
  else
    W->CPV = 0;
}

void CY_ARITH::AND()
  {
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  iGetIN();
  OUT.Set( CSignal::BAD | CSignal::QST, OutStatus );
  W->RV1 = iRV1;
  W->RV2 = iRV2;
  if ( OutStatus == CSignal::BAD )
    return ;
  if( W->CALI )
    return;
  if ( iRV1 && iRV2 )
    W->CPV = 1;
  else
    W->CPV = 0;
  }

void CY_ARITH::SQRT()
{
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
    return ;
  }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  GetIN();
  if( 0!=W->Data_GAINin )
    W->RV = RV * W->Data_GAINin + W->Data_BIASin;
  else
    W->RV = RV;
  W->RV = RV;
  OUT.Set( CSignal::BAD|CSignal::QST, OutStatus );
  if ( OutStatus == CSignal::BAD )
    return;
  /*if ( fabs ( RV ) < 1e-10 )
  {
    OUT.On( CSignal::BAD );
    return ;
  }*/
  OUT.Off( CSignal::BAD );
  if( W->CALI )
    return;
  W->CPV = W->GAIN * sqrt( RV );
}

void CY_ARITH::MUL()
{
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
    return ;
  }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  GetIN();
  OUT.Set( CSignal::BAD | CSignal::QST, OutStatus );
  W->RV = RV;
  W->RV1 = RV1;
  if ( OutStatus == CSignal::BAD )
    return ;
  if( W->CALI )
    return;
  W->CPV = W->GAIN * ( RV * ( W->GN1 * RV1 + W->BS1 )) + W->BIAS;  
}
