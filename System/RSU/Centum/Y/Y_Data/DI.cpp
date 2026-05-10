#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <Names.h>

CY_DI::CY_DI( bool NoAdd ) 
: CY_Base( NoAdd )
, pointTypeZ( eptUnknown )
, modelTypeZ( eptUnknown )
, pZ( NULL )
, nBitWW4WB( -1 )
{
  Y_Type = 0;
}

CY_DI::CY_DI( char * _Name, int _ID, int NumbFCS, int domain, int numb ) 
: CY_Base()
, pointTypeZ( eptUnknown )
, modelTypeZ( eptUnknown )
, Numb( numb )
, nBitWW4WB( -1 )
{
  Y_Type = _ID;
  this->NumbFCS = NumbFCS;
  this->domain = domain;
  strcpy_s( nameWithFcs, _Name );
  CY_Base::pComm = Comment;
  IN = NULL;
	pZ = NULL;
  FlagUse = 0;
  AlarmLevel = 0;
  pZ = (C_Z *)KMemoryServiceY::Instance().CreateIOsStruct( _Name, Y_Type, NumbFCS, domain ); 

  ASS( pZ );
  CY_Base::InitFlagUse( pZ->FlagUse );
//
  Mes = -1;
  pPV = NULL;
  pDPV = NULL;
  switch ( Y_Type )
  {
  case id__Y:
    OUT << pY->PV;
    pPV = &pY->PV;
    OUTD << pY->DPV;
    pDPV = &pY->DPV;
    break;
  case id__Z:
    OUT << pZ->PV;
    pPV = &pZ->PV;
    OUTD << pZ->DPV;
    pDPV = &pZ->DPV;
    if( '%'!=_Name[0] )
    {
      char buf[20];
      sprintf_s( buf, "%%Z%06d", Numb );
      KMemoryServiceY::Instance().CreateRefToIOsStruct( buf, pZ );
    }
    break;
  case id__WW:
    OUT << pWW->PV;
    pPV = &pWW->PV;
    OUTD << pWW->DPV;
    pDPV = &pWW->DPV;
    break;
  case id__XW:
    OUT << pXW->PV;
    pPV = &pXW->PV;
    OUTD << pXW->DPV;
    pDPV = &pXW->DPV;
    break;
  case id__AN:
    OUT << pAN->PV;
    pPV = &pAN->PV;
    Alarm.pAlarmLevel = &AlarmLevel;
    Alarm.SetBlk( pAN );
    Alarm.pObj = this;
    Alarm.BlockName = nameWithFcs;
    break;
  case id__WB:
    OUT << pWB->PV;
    pPV = &pWB->PV;
    pDPV = &pWB->PVF32;
    break;
  case id__GS:
    OUT << pGS->PV;
    pPV = &pGS->PV;
    break;
  case id__SW:
    OUT << pSW->PV;
    pPV = &pSW->PV;
    if( '%'!=_Name[0] )
    {
      char buf[20];
      sprintf_s( buf, "%%SW%04d", Numb );
      KMemoryServiceY::Instance().CreateRefToIOsStruct( buf, pSW );
    }
    break;
  case id__PR:
    OUT << pPR->PV;
    pPV = &pPR->PV;
    break;
  case id__OG:
    OUT << pOG->PV;
    pPV = &pOG->PV;
    break;
  case id__RQ:
    OUT << pRQ->PV;
    pPV = &pRQ->PV;
    break;
  default:
    ASSD(FALSE);
    break;
  }
}

void CY_DI::ResetAlarms()
{
  if( id__AN==Y_Type)
    Alarm.SetBlk( pAN );
}

CSignal * CY_DI::GetOUT ( char * OutName )
	{
  if ( Y_Type == id__Z && pointTypeZ == eptAnalog )
    {
    OUTD.pFrom = Name;
    return &OUTD;
    }
  if ( Y_Type == id__Y && pointTypeZ == eptAnalog )
    {
    OUTD.pFrom = Name;
    return &OUTD;
    }
  if ( Y_Type == id__WW && pointTypeZ == eptAnalog )
    {
    OUTD.pFrom = Name;
    return &OUTD;
    }
  if ( Y_Type == id__XW && pointTypeZ == eptAnalog )
  {
    OUTD.pFrom = Name;
    return &OUTD;
  }
  if( pointTypeZ==eptAnalog )
  {
    if( pDPV )
    {
      OUTD.pFrom = Name;
      OUTD << *pDPV;
      return &OUTD;
    }
    ASSD( 0 )
  }
  if( !strcmp(OutName,"PVI32") && id__WB==Y_Type )
    OUT << pWB->PVI32, pPV = &pWB->PVI32;
  OUT.pFrom = Name;
	return &OUT;
	}

CSignal ** CY_DI::GetIN ( char * Name )
{
  if( !Name || !*Name )
	  return &IN;
  if( !strcmp(Name, "IN") )
    return &IN;
  return NULL;
}

void CY_DI::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}
  Prepare1();

  if( (FlagUse&CAlarmBase::fuYConnect) || (pZ && (pZ->FlagUse&CAlarmBase::fuYConnect)) )
  {
    FlagUse |= CAlarmBase::fuYConnect;
    if( pZ )
      pZ->FlagUse |= CAlarmBase::fuYConnect;
  }

  if ( IN )
  {
    if ( Y_Type == id__Z && pointTypeZ == eptAnalog )
    {
      double V;
      *IN >> V;
      if( pDPV )
        *pDPV = V;
      IN->BackVar = V;
      return;
    }
    if ( Y_Type == id__Y && pointTypeZ == eptAnalog )
    {
      double V;
      *IN >> V;
      if( pDPV )
        *pDPV = V;
      IN->BackVar = V;
      return;
    }
    int V;
    *IN >> V;
    if( pPV )
      *pPV = V;
    double D = 0;
    *IN >> D;
    if( pDPV )
      *pDPV = D;
  }

  switch( Y_Type )
  {
  case id__AN:
    if( Alarm.pObj && AlarmLevel && pPV )
    {
      if( *pPV )
        Alarm.SendAlarm( A_ALM, true );
      else
        Alarm.SendAlarm( A_ALM, false );
    }
    break;
  case id__OG:
    if( pOG )
    {
      if( pOG->PV )
      {
        char OG[ sizeof(szAkaObjName) ];
        strcpy_s( OG, szAkaObjName );
        char * P = strchr ( OG, '/' );
        if ( P )
          *P = 0;
        if( pSendOG )
          pSendOG( OG, Numb );
        pOG->PV = 0;
      }
    }
    break;
  case id__RQ:
    if( pRQ )
    {
      if( pRQ->PV )
      {
        if( pSendRQ )
          pSendRQ( Numb );
        pRQ->PV = 0;
      }
    }
    break;
  }
  if( pParenWW4WB )
  {
    switch( pParenWW4WB->pointTypeZ )
    {
    case CY_DI::eptDigital:
      if( *pPV )
        *pParenWW4WB->pPV |= 1<<nBitWW4WB;
      else
        *pParenWW4WB->pPV &= ~(1<<nBitWW4WB);
      break;
    }
  }
}

bool CY_DI::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool b = __super::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( b )
    return b;
  bool OPMK = VarName ? strcmp( VarName, "OPMK" ) == 0 : false; 
  bool AFLS = VarName ? strcmp( VarName, "AFLS" ) == 0 : false; 
  bool PSW = VarName ? strcmp( VarName, "PSW" ) == 0 : false;
  bool AOFS = VarName ? strcmp( VarName, "AOFS" ) == 0 : false; 
  if( PSW )
    OPMK = true;// пока
  //ASS(VarName[0] == 0 || ( VarName[0] == 'P' && VarName[1] == 'V' && VarName[2] == 0) || OPMK || AFLS );
  /*bool bPV = VarName[0] == 0 || ( VarName[0] == 'P' && VarName[1] == 'V' && VarName[2] == 0);
  bool pPVF32 = VarName[0] == 0 || ( VarName[0] == 'P' && VarName[1] == 'V' && VarName[2] == 'F' && VarName[3] == '3' && VarName[4] == '2');
*/
  if( !_strcmpi(VarName,"PV=BAD") )
  {
    *Type = 'I';
    *pTuneVar = NULL;
    *Work = CY_Base::evtP;
    *pVar = &PV_BAD;
    return true;
  }
  if( Type )
    *Type = 'I';
  if( pTuneVar )
    *pTuneVar = NULL;
  if( Work )
    *Work = CY_Base::evtWork;
  if( ppFlagUse )
  {
    *ppFlagUse = &FlagUse;
  }
  Prepare1();
  if ( OPMK )
    {
    switch ( Y_Type )
      {
      case id__Z:
        *pVar = &pZ->OPMK;
      break;
      case id__Y:
        *pVar = &pY->OPMK;
        break;
      case id__AN:
        *pVar = &pAN->OPMK;
      break;
      case id__WB:
        *pVar = &pWB->OPMK;
      break;
      case id__GS:
        *pVar = &pGS->OPMK;
      break;
      case id__SW:
        *pVar = &pSW->OPMK;
      break;
      case id__PR:
        *pVar = &pPR->OPMK;
      break;
      case id__OG:
        *pVar = &pOG->OPMK;
      break;
      case id__RQ:
        *pVar = &pRQ->OPMK;
        break;
      default:
        ASSD(FALSE);
      break;
      }
    return true;
    }
  if ( AFLS )
    {
      if( Type )
        *Type = 'W';
    switch ( Y_Type )
      {
      case id__Z:
        if( pVarSize )
          *pVarSize = sizeof(pZ->AFLS);
        *pVar = &pZ->AFLS;
        break;
      case id__Y:
        if( pVarSize )
          *pVarSize = sizeof(pY->AFLS);
        *pVar = &pY->AFLS;
        break;
      case id__AN:
        if( pVarSize )
          *pVarSize = sizeof(pAN->notACK);
        *pVar = &pAN->notACK;
        break;
      case id__WB:
        if( pVarSize )
          *pVarSize = sizeof(pWB->AFLS);
        *pVar = &pWB->AFLS;
        break;
      case id__GS:
        if( pVarSize )
          *pVarSize = sizeof(pGS->AFLS);
        *pVar = &pGS->AFLS;
        break;
      case id__SW:
        if( pVarSize )
          *pVarSize = sizeof(pSW->AFLS);
        *pVar = &pSW->AFLS;
        break;
      case id__PR:
        if( pVarSize )
          *pVarSize = sizeof(pPR->AFLS);
        *pVar = &pPR->AFLS;
        break;
      case id__OG:
        if( pVarSize )
          *pVarSize = sizeof(pOG->AFLS);
        *pVar = &pOG->AFLS;
        break;
      case id__RQ:
        if( pVarSize )
          *pVarSize = sizeof(pRQ->AFLS);
        *pVar = &pRQ->AFLS;
        break;
      default:
        ASSD(FALSE);
        break;
      }
    return true;
    }
  if( AOFS )
  {
    if( Type )
      *Type = 'W';
    switch ( Y_Type )
    {
    case id__AN:
      if( pVarSize )
        *pVarSize = sizeof(pAN->AOFS);
      *pVar = &pAN->AOFS;
      break;
    default:
      ASSD(FALSE);
      break;
    }
    return true;
  }
  switch ( Y_Type )
    {
    case id__Z:
      if( pointTypeZ==eptAnalog )
        {
          if( pVarSize )
            *pVarSize = sizeof(pZ->DPV);
          *pVar = &pZ->DPV;
          *Type = 'D';
        }
      else
        {
        *pVar = &pZ->PV;
        if( pVarSize )
          *pVarSize = sizeof(pZ->PV);
        *Type = 'I';
        }
    break;
    case id__Y:
      if( pointTypeZ==eptAnalog )
      {
        if( pVarSize )
          *pVarSize = sizeof(pY->DPV);
        *pVar = &pY->DPV;
        *Type = 'D';
      }
      else
      {
        *pVar = &pY->PV;
        if( pVarSize )
          *pVarSize = sizeof(pY->PV);
        *Type = 'I';
      }
      break;
    case id__AN:
      if( pVarSize )
        *pVarSize = sizeof(pAN->PV);
      *pVar = &pAN->PV;
    break;
    case id__WB:
      if( !strcmp(VarName, "PVI32") )
      {
        if( pVarSize )
          *pVarSize = sizeof(pWB->PVI32);
        *pVar = &pWB->PVI32;
      }
      else if( !strcmp(VarName, "PVF32") )
      {
        if( pVarSize )
          *pVarSize = sizeof(pWB->PVF32);
        if( Type )
          *Type = 'D';
        *pVar = &pWB->PVF32;
      }
      else if( !strcmp(VarName, "PVF64") )
      {
        if( pVarSize )
          *pVarSize = sizeof(pWB->PVF64);
        if( Type )
          *Type = 'D';
        *pVar = &pWB->PVF64;
      }
      else
      {
        ASSD( 'P'==VarName[0] && 'V'==VarName[1] )
        if( pVarSize )
          *pVarSize = sizeof(pWB->PV);
        *pVar = &pWB->PV;
      }
      break;
    case id__GS:
      if( pVarSize )
        *pVarSize = sizeof(pGS->PV);
      *pVar = &pGS->PV;
      break;
    case id__SW:
      if( pVarSize )
        *pVarSize = sizeof(pSW->PV);
      *pVar = &pSW->PV;
      break;
    case id__PR:
      if( pVarSize )
        *pVarSize = sizeof(pPR->PV);
      *pVar = &pPR->PV;
      break;
    case id__OG:
      if( pVarSize )
        *pVarSize = sizeof(pOG->PV);
      *pVar = &pOG->PV;
      break;
    case id__RQ:
      if( pVarSize )
        *pVarSize = sizeof(pRQ->PV);
      *pVar = &pRQ->PV;
      break;
    case id__WW:
      if( pointTypeZ==eptAnalog )
      {
        if( pVarSize )
          *pVarSize = sizeof(pWW->DPV);
        *pVar = &pWW->DPV;
        if( Type )
          *Type = 'D';
      }
      else
      {
        *pVar = &pWW->PV;
        if( pVarSize )
          *pVarSize = sizeof(pWW->PV);
        if( Type )
          *Type = 'I';
      }
      break;
    case id__XW:
      if( pointTypeZ==eptAnalog )
      {
        if( pVarSize )
          *pVarSize = sizeof(pXW->DPV);
        *pVar = &pXW->DPV;
        if( Type )
          *Type = 'D';
      }
      else
      {
        *pVar = &pXW->PV;
        if( pVarSize )
          *pVarSize = sizeof(pWW->PV);
        if( Type )
          *Type = 'I';
      }
      break;
    default:
      ASSD(FALSE);
      break;
    }
  return true;
}

void CY_DI::Init( )
{
}

int CY_DI::Prepare0()
{
  switch ( Y_Type )
  {
  case id__AN:
    if( !strncmp( Name, "%AN", 3) && 0==nameWithFcs[7] )
      strncat_s( nameWithFcs, szAkaObjName+2, 5 );
    break;
  case id__WB:
    if( 'O'==IOZ && -1==nBitWW4WB )
    {
      int NumbWW = Numb / 100;
      nBitWW4WB = Numb % 100 - 1;
      nBitWW4WB = 15 - nBitWW4WB;
      ASSD( nBitWW4WB >= 0 && nBitWW4WB < 16 );
      pParenWW4WB = pFindDI( NumbFCS, domain, NumbWW, id__WW );
      ASSD( pParenWW4WB );
      if( pParenWW4WB )
      {
        ASSD( pParenWW4WB->pPV );
        if( !pParenWW4WB->pPV )
          pParenWW4WB = 0;
      }
    }
    break;
  }
	return 0;
}

int CY_DI::Prepare1()
{
  switch ( Y_Type )
  {
  case id__Y:
    pY->FlagUse |= FlagUse;
    break;
  case id__Z:
    pZ->FlagUse |= FlagUse;
    break;
  case id__AN:
    pAN->FlagUse |= FlagUse;
    break;
  case id__WB:
    pWB->FlagUse |= FlagUse;
    break;
  case id__GS:
    pGS->FlagUse |= FlagUse;
    break;
  case id__SW:
    FlagUse |= CAlarmBase::fuYConnect;
    pSW->FlagUse |= FlagUse;
    break;
  case id__PR:
    pPR->FlagUse |= FlagUse;
    break;
  case id__OG:
    pOG->FlagUse |= FlagUse;
    break;
  case id__RQ:
    pRQ->FlagUse |= FlagUse;
    break;
  case id__WW:
    pWW->FlagUse |= FlagUse;
    break;
  case id__XW:
    pWW->FlagUse |= FlagUse;
    break;
  default:
    ASS(FALSE);
    break;
  }
  return 0;
}

int CY_DI::SaveState( )
	{
	return 0;
	}

int CY_DI::RestoreState ( char * StrName )
	{
	return 1;
	}

int CY_DI::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
  if( pointTypeZ==eptAnalog )
  {
    if( pDPV )
      PARM(*pDPV,"#DPV")
    if( pPV )
      PARM(*pPV,"#PV")
  }
  else if( pointTypeZ==eptDigital )
  {
    if( pPV )
      PARM(*pPV,"#PV")
    if( pDPV )
      PARM(*pDPV,"#DPV")
  }
  else
  {
    if( pPV )
      PARM(*pPV,"#~PV")
    if( pDPV )
      PARM(*pDPV,"#~DPV")
  }
  PARM(Numb,"#Numb")
  PARM(Comment,"#Comment")
  if ( pDR )
    PARM_CALLBACK( pDR->FCS, "#FCS" );
	PARM_IN(IN)
	PARM_OUT
	return 0;
	}

bool CY_DI::SetLimits( )
  {
  return true;
  }

void CY_DI::SetObjName( int NumbFCS /*= 0*/, char chStationType /*= 'F'*/ )
{
  if ( NumbFCS )
  {
    if( NumbFCS < CY_DR::s_dwFcsNumModulus )
      sprintf_s ( szAkaObjName, "%cCS00%02d/", chStationType, NumbFCS );
    else
      sprintf_s ( szAkaObjName, "%cCS%04d/", chStationType, NumbFCS );
  }
  else
  {
    strcpy_s( szAkaObjName, pDR->FCS );
    strcat_s( szAkaObjName, "/" );
  }
  strcat_s( szAkaObjName, Name );
}

void CY_DI::SetObjName( const char *pTextedName )
{
  sprintf_s( szAkaObjName, "%s/%s", pTextedName, Name );
}

CBase* CY_DI::GetStructure()
{
  return pZ;
}
