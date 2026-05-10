#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <macros/StrHelps.h>

// D3.8
static CY_Create CI ( "CI", CY_CI::Create );

CY_CI::CY_CI( CCI * _W ) 
: CY_Base()
{
	W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
//
	IN = NULL;
	CLEAR(pPV);
  CLEAR(pDI);
  pWW = NULL;
  W->ACT = 1;
  W->mode.M = SMode::AUT;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_CI,CCI,30,50)

void CY_CI::InitParm()
  {
#include "Yokogawa\CI_P.hpp" 
#include "Yokogawa\CI.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_CI::GetOUT ( char * Name )
	{
	return NULL;
	}

CSignal ** CY_CI::GetIN ( char * Name )
	{
	GET_IN(IN)
	return NULL;
	}

void CY_CI::Work ( double dt )
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

#pragma message("!!!!! 186_DKS_NTKR_UBGPZ\n")
  //ASSD( W->Code_Conversion==0 );

  int PV = 0;
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
        PV = (int)*pWW->pDPV;
      break;
    case CY_DI::eptDigital:
      ASSD( pWW->pPV );
      if( pWW->pPV )
        PV = *pWW->pPV;
      break;
    }
  }
  else
  {
    int M = 1 << (W->Number_Of_Bits_Input-1);
	  for ( int n = 0; n < W->Number_Of_Bits_Input; n++ )
    {
		  if( pPV[n] && *pPV[n] )
			  PV |= M;
		  M = M >> 1;
    }
  }
  if( W->Bit_Inversion )
    PV = ~PV;
	W->PV = PV;
}

void CY_CI::Init( )
	{
	}

int CY_CI::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\CI_P.hpp" 
  if( !IN )
    return 0;
	char FirstName[256];
	strcpy_s( FirstName, IN->pFrom );
  int NumbFCS = atoi ( pDR->FCS + 3 ) % 100;
  ASS( NumbFCS==pDR->NumbFCS );
	int Numb = 0;
  int type = id__SW;

  {
    LPSTR pPnt = strchr( FirstName, '.' );
    TStringTerminator st( pPnt );
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
	ASSD( Numb > 0 );
  ASSD(W->Number_Of_Bits_Input<=_countof(pPV) );
	for( int n = 0; n < W->Number_Of_Bits_Input; ++n, ++Numb )
	{
    pDI[n] = (*pFindDI )( NumbFCS, pDR->domain, Numb, type );
    //!!!!!
    //ASSD( pDI[n] );
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
    case id__WW:
      pPV[n] = &pDI[n]->pWW->PV;
      break;
    case id__XW:
      pPV[n] = &pDI[n]->pXW->PV;
      break;
    case id__GS:
      pPV[n] = &pDI[n]->pGS->PV;
      break;
    default:
      ASS(0);
      break;
    }
	}
	return 0;
}

int CY_CI::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
 if( W->mode.M = SMode::AUT )
    W->ACT = 1;
  else
    W->ACT = 0;
	return 0;
}

int CY_CI::SaveState( )
	{
	SAVE_CLASS("W",CY_CI_W)
	return 0;
	}

int CY_CI::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_CI_W)
	return 1;
	}

int CY_CI::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	W_PARM(PV)
	for ( int n = 0; n < W->Number_Of_Bits_Input; n++ )
		{
		if ( pPV[n] )
			{
			PARM(	*pPV[n], pDI[n]->Name );
			}
		}
	if ( pDR && pDR->FCS )
		PARM_CALLBACK( pDR->FCS, "#FCS" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\CI_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\CI.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN)
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_CI::SetLimits( )
  {
  return true;
  }

bool CY_CI::EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj )
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
