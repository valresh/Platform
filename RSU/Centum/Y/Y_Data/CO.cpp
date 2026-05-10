#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <macros/StrHelps.h>

// D3.8
static CY_Create CO ( "CO", CY_CO::Create );

CY_CO::CY_CO( CCO * _W ) : CY_Base()
{
	W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
//
	OUT.Name = "OUT";
  OUT << W->PV;
//
	CLEAR(pPV);
  CLEAR(pDI);
  pWW = NULL;
  W->ACT = 1;
  W->mode.M = SMode::AUT;
}

//YOKO   1
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_CO,CCO,10,20)

void CY_CO::InitParm()
  {
#include "Yokogawa\CO_P.hpp" 
#include "Yokogawa\CO.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_CO::GetOUT ( char * Name )
	{
  GETOUT
	return NULL;
	}

CSignal ** CY_CO::GetIN ( char * Name )
	{
	return NULL;
	}

void CY_CO::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}
  if( W->ACT )
    W->mode.M = SMode::AUT;
  else
    W->mode.M = SMode::MAN;
  if( W->mode.M != SMode::AUT )
    return;
  int PV = W->PV;
  if ( W->Bit_Inversion)
	{
		PV = ~PV;
	}
  
  bool bHaveParentWW = false;
  if( pWW )
  {
    if( pWW->bConnectedByACS )
      bHaveParentWW = true;
    if( pWW->pWW && (pWW->pWW->FlagUse&CAlarmBase::fuYConnect) )
      bHaveParentWW = true;
  }
  if( bHaveParentWW )
  {
    switch( pWW->pointTypeZ )
    {
    case CY_DI::eptAnalog:
      ASSD( pWW->pDPV );
      if( pWW->pDPV )
        *pWW->pDPV = PV;
      break;
    case CY_DI::eptDigital:
      ASSD( pWW->pPV );
      if( pWW->pPV )
        *pWW->pPV = PV;
      break;
    }
  }

	int M = 1 << ( W->Number_Of_Bits_Output - 1 );
	for ( int n = 0; n < W->Number_Of_Bits_Output; n++ )
	{
    if ( !pPV[n] )
      continue;
		if ( PV & M )
			*pPV[n] = 1;
		else
			*pPV[n] = 0;
		M = M >> 1;
	}
}

void CY_CO::Init( )
	{
	}

int CY_CO::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\CO_P.hpp" 
  if( !OUT.pTo[0] )
    return 0;
  char FirstName[256];
	strcpy_s( FirstName, OUT.pTo[0] );
  int NumbFCS = atoi ( pDR->FCS + 3 ) % 100;
  ASS( NumbFCS==pDR->NumbFCS );
  int Numb = 0;
  int type = id__SW;

  {
    LPSTR pPnt = strchr( FirstName, '.' );
    TStringTerminator st(pPnt);
    CY_Base *pObj = (*pFindDObj)( FirstName, pDR->NumbFCS, pDR->domain );
    ASS(pObj);
    type = pObj->Y_Type;
    Numb = ((CY_DI*)pObj)->Numb;
  }
  if( id__WB==type )
  {
    int NumbWW = Numb / 100;
    pWW = pFindDI( NumbFCS, pDR->domain, NumbWW, id__WW );
  }
	ASS( Numb > 0 );
  ASS(W->Number_Of_Bits_Output<=_countof(pPV))
	for ( int n = 0; n < W->Number_Of_Bits_Output; ++n, ++Numb )
	{
    pDI[n] = (*pFindDI )( NumbFCS, pDR->domain, Numb, type );
    ASSD( pDI[n] );
    if( !pDI[n] )
      continue;
    switch( type )
    {
    case id__SW:
      pPV[n] = &pDI[n]->pSW->PV;
      break;
    case id__Z:
      pPV[n] = &pDI[n]->pZ->PV;
      break;
    case id__Y:
      pPV[n] = &pDI[n]->pY->PV;
      break;
    case id__WB:
      pPV[n] = &pDI[n]->pWB->PV;
      break;
    default:
      ASS(0);
      KKK();
      break;
    }
	}

  if( pPV[0] )
    OUT << *pPV[0];
	return 0;
}

int CY_CO::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_CO::SaveState( )
	{
	OUT.SaveState();
	SAVE_CLASS("W",CY_CO_W)
	return 0;
	}

int CY_CO::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_CO_W)
	return 1;
	}

int CY_CO::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	W_PARM(PV)
	for ( int n = 0; n < W->Number_Of_Bits_Output; n++ )
		{
		if ( pPV[n] && pDI[n] )
			{
			PARM(	*pPV[n], pDI[n]->Name );
			}
		}
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\CO_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\CO.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_CO::SetLimits( )
{
  return true;
}

bool CY_CO::EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj )
{
  for( pos; pos<_countof(pDI); ++pos )
  {
    if( !pDI[pos] )
      continue;
    *ppName = pDI[pos]->Name;
    ++pos;
    return true;
  }
  return false;
}
