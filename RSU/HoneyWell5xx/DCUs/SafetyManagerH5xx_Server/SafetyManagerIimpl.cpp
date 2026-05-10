#include "SafetyManager.h"
#include <MemoryServiceSM5xx.h>
#include <SafetyManagerStruct.h>
#include <crosslocale.h>

int CompInfo( const void * p1, const void * p2 );

int KSafetyManager::FindDbfTag( LPCSTR ptr, CFSC_Info::ePointType ept, CFSC_Info::eType *pPntType /*= NULL*/ )
{
  if( pPntType )
    *pPntType = CFSC_Info::NoneType;
  CFSC_Info Key;
  ZeroMemory( &Key, sizeof(Key) );
  strcpy_s( Key.NAME, (char*)ptr );
  Key.PointType = ept;

  CFSC_Info * pRes = (CFSC_Info*)bsearch ( &Key, m_FSC_Info, m_kFSC_Info, sizeof( CFSC_Info ), CompInfo );
  //ASSD( pRes );
  if ( !pRes )
    return -1;
  if( pPntType )
    *pPntType = pRes->Type;
  return pRes->NUMB;
}

LPCSTR KSafetyManager::DbfName( int n, int m )
{
  if( n < 0 )
    return NULL;
  ASS( n >= 0 && n <= m_kFSC_Info);
  switch ( m )
  {
  case 1:
    return (const char*)m_FSC_Info[n].NAME;
  case 2:
    return (const char*)m_FSC_Info[n].DESC;
  case 3:
    return (const char*)m_FSC_Info[n].DESC2;
  case 7:
    return "0"; //SHEET
  case 13:
    return (const char*)m_FSC_Info[n].szTypeLocation;
  case 11:
    return (const char*)m_FSC_Info[n].szFLDNumber;
  case 27:
    //return (const char*)m_FSC_Info[n].szPLCADDR;
	for ( int i=0; i<6; ++ i )
    {
      if ( !strcmp( m_FSC_Info[n].szPLCADDR[i], "" ) )
        continue;
      return (const char*)m_FSC_Info[n].szPLCADDR[i];
    }
  case 24:
    return m_FSC_Info[n].szPowerUpValue;
  default:
    return "";
    return NULL;
  }
  return NULL;
}

void* KSafetyManager::VoidInOut( int nDbf, EValueType eType, bool bIn )
{
  if ( nDbf < 0 )
    return NULL;

  CFSC_Info &info = m_FSC_Info[nDbf];

  if( CFSC_Info::catHZ==info.ComAllocationType[0] )
    return NULL;

  int PLCADDR = info.PLCADDR[0];

  if( PLCADDR<=0 )
    return NULL;

  //LPCSTR Name = DbfName( nDbf, 1 );
  int Sheet = atoi( DbfName( nDbf, 7 ));

  LPCSTR n = DbfName( nDbf, 1 );//Ищем по имени

  /*char name[64]; !!!!!
  sprintf_s( name, "%s.%d", info.szMaster1, PLCADDR );
  ::GiveCommData( name, CValue::TypeID, (void**)&info.pPlcComm );
  ASS( info.pPlcComm );*/

  void *ret = NULL;

  /*if( bIn )
  {
    ASS( CFSC_Info::catInput==m_FSC_Info[nDbf].ComAllocationType1 );
  }
  else
  {
    ASS( CFSC_Info::catOutput==m_FSC_Info[nDbf].ComAllocationType1 );
  }*/

  info.pPlcComm->eType = eType;
  switch( eType )
  {
  case enumValueBol:
    ret = &info.pPlcComm->bVal;
    break;
  case enumValueDbl:
    ret = &info.pPlcComm->dVal;
    break;
  default:
    ASS(0);
  }

  return ret;
}

void* KSafetyManager::CreateSharedIO( int nDbf, EValueType eType, unsigned char mT, LFscBase::typeB mB )
{
  if ( nDbf < 0 )
    return NULL;

  CFSC_Info &info = m_FSC_Info[nDbf];
  void *ret = NULL;
  char szArmFullName[128] = {};
  if( info.szFldName[0] )
  {
    if( info.szBlockName[0] )
      sprintf_s( szArmFullName, "%s.%s", info.szFldName, info.szBlockName );
    else
    {
      KKK();
      //sprintf_s( szArmFullName, "%s.%s", info.szFldName, info.NAME );
    }
  }

  if( mT > 0 && LFscBase::unknown!=mB )
  {
    switch( mT )
    {
    default:
      ASSD(0);
      break;
    case 0x0B://"Digital Input"
      {
        if( /* LFscBase::COM==mB ||*/ LFscBase::FLD==mB )
        {
          ASSD( enumValueBol==eType );
          W_SMDI *pObj = (W_SMDI*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( szArmFullName, info.NAME, W_SMDI::TypeID, 0 );
          if( !pObj )
            return NULL;
          ret = &pObj->PVFL;
          strcpy_s( pObj->DESC, info.DESC );
          strcpy_s( pObj->STATE0TEXT, info.szStatesText[0] );
          strcpy_s( pObj->STATE1TEXT, info.szStatesText[1] );
          strcpy_s( pObj->NORNALSTATE, info.szNormalState );
          if( info.szPowerUpValue[0] )
            pObj->PVFL = atoi( info.szPowerUpValue );
        }
        else if( LFscBase::COM==mB )
        {
          ASSD( enumValueBol==eType );
          W_SMDICOM *pObj = (W_SMDICOM*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( szArmFullName, info.NAME, W_SMDICOM::TypeID, 0 );
          if( !pObj )
            return NULL;
          ret = &pObj->PVFL;
          strcpy_s( pObj->DESC, info.DESC );
          strcpy_s( pObj->STATE0TEXT, info.szStatesText[0] );
          strcpy_s( pObj->STATE1TEXT, info.szStatesText[1] );
          strcpy_s( pObj->NORNALSTATE, info.szNormalState );
        }
        else
        {
          ASSD( 0 );
        }
      }
      break;
    case 0x0C://"Analog Input (AI)"
      {
        if( /*LFscBase::COM==mB ||*/ LFscBase::FLD==mB )
        {
          ASSD( enumValueDbl==eType );
          W_SMAI *pObj = (W_SMAI*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( szArmFullName, info.NAME, W_SMAI::TypeID, 0 );
          if( !pObj )
            return NULL;
          ret = &pObj->PV;
          strcpy_s( pObj->DESC, info.DESC );
          strcpy_s( pObj->EUDESC, info.szEngineeringUnits );
          pObj->PVEULO = cross::locale::safe_atof( info.szBottomScale );
          pObj->PVEUHI = cross::locale::safe_atof( info.szTopScale );
          if( info.szPowerUpValue[0] )
            pObj->PV = cross::locale::safe_atof( info.szPowerUpValue );

          if( info.szAlarmLimitHighHigh[0] )
            pObj->PVHHALMTP = cross::locale::safe_atof( info.szAlarmLimitHighHigh );
          else
            pObj->PVHHALMTP = NaN;
          if( info.szAlarmLimitHigh[0] )
            pObj->PVHIALMTP = cross::locale::safe_atof( info.szAlarmLimitHigh );
          else
            pObj->PVHIALMTP = NaN;
          if( info.szAlarmLimitLow[0] )
            pObj->PVLOALMTP = cross::locale::safe_atof( info.szAlarmLimitLow );
          else
            pObj->PVLOALMTP = NaN;
          if( info.szAlarmLimitLowLow[0] )
            pObj->PVLLALMTP = cross::locale::safe_atof( info.szAlarmLimitLowLow );
          else
            pObj->PVLLALMTP = NaN;

        }
        else if( LFscBase::SYS==mB )
        {
          KKK();
        }
        else
        {
          ASSD( 0 );
        }
      }
      break;
    case 0x19://"Analog Output"
      {
        if( /*LFscBase::COM==mB ||*/ LFscBase::FLD==mB )
        {
          ASSD( enumValueDbl==eType );
          W_SMAO *pObj = (W_SMAO*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( szArmFullName, info.NAME, W_SMAO::TypeID, 0 );
          if( !pObj )
            return NULL;
          ret = &pObj->OP;
          strcpy_s( pObj->DESC, info.DESC );
          strcpy_s( pObj->EUDESC, info.szEngineeringUnits );
          pObj->PVEULO = cross::locale::safe_atof( info.szBottomScale );
          pObj->PVEUHI = cross::locale::safe_atof( info.szTopScale );
        }
        else
        {
          ASSD( 0 );
        }
      }
      break;
    case 0x1A://"Digital Output"
      {
        if( /*LFscBase::COM==mB || */LFscBase::FLD==mB )
        {
          ASSD( enumValueBol==eType );
          W_SMDO *pObj = (W_SMDO*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( szArmFullName, info.NAME, W_SMDO::TypeID, 0 );
          if( !pObj )
            return NULL;
          ret = &pObj->OPFL;
          strcpy_s( pObj->DESC, info.DESC );
          strcpy_s( pObj->STATE0TEXT, info.szStatesText[0] );
          strcpy_s( pObj->STATE1TEXT, info.szStatesText[1] );
          strcpy_s( pObj->NORNALSTATE, info.szNormalState );
          strcpy_s( pObj->BLOCKNAME, info.szBlockName );
        }
        else if( LFscBase::COM==mB )
        {
          ASSD( enumValueBol==eType );
          W_SMDOCOM *pObj = (W_SMDOCOM*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( szArmFullName, info.NAME, W_SMDOCOM::TypeID, 0 );
          if( !pObj )
            return NULL;
          ret = &pObj->PVFL;
          strcpy_s( pObj->DESC, info.DESC );
          strcpy_s( pObj->STATE0TEXT, info.szStatesText[0] );
          strcpy_s( pObj->STATE1TEXT, info.szStatesText[1] );
          strcpy_s( pObj->NORNALSTATE, info.szNormalState );
        }
        else
        {
          ASSD( 0 );
        }
      }
      break;
    case 0x46://"Multiple Input (BI)"
      //if( LFscBase::COM==mB )
      {
        ASSD( enumValueDbl==eType );
        W_SMBICOM *pObj = (W_SMBICOM*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( szArmFullName, info.NAME, W_SMBICOM::TypeID, 0 );
        if( !pObj )
          return NULL;
        ret = &pObj->PV;
        strcpy_s( pObj->DESC, info.DESC );
        strcpy_s( pObj->EUDESC, info.szEngineeringUnits );
        if( info.szPowerUpValue[0] )
          pObj->PV = cross::locale::safe_atof( info.szPowerUpValue );
      }
      /*else
      {
        ASSD( 0 );
      }*/
      break;
    case 0x47://"Multiple Output (BO)"
      if( LFscBase::COM==mB )
      {
        if( enumValueDbl!=eType )
          break;
        W_SMBOCOM *pObj = (W_SMBOCOM*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( szArmFullName, info.NAME, W_SMBOCOM::TypeID, 0 );
        if( !pObj )
          return NULL;
        ret = &pObj->PV;
        strcpy_s( pObj->DESC, info.DESC );
        strcpy_s( pObj->EUDESC, info.szEngineeringUnits );
      }
      else
      {
        ASSD( 0 );
      }
      break;
    }
    return ret;
  }

  switch( eType )
  {
  case enumValueBol:
    //ret = &info.pPlcComm->bVal;
    ASSD( 'D'==info.szPointType[0] );
    {
      W_SMDISCRET *pObj = (W_SMDISCRET*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( szArmFullName, info.NAME, W_SMDISCRET::TypeID, 0 );
      if( !pObj )
        return NULL;
      ret = &pObj->PVFL;
      strcpy_s( pObj->DESC, info.DESC );
      strcpy_s( pObj->STATE0TEXT, info.szStatesText[0] );
      strcpy_s( pObj->STATE1TEXT, info.szStatesText[1] );
      strcpy_s( pObj->NORNALSTATE, info.szNormalState );
    }
    break;
  case enumValueDbl:
    //ret = &info.pPlcComm->dVal;
    ASSD( 'A'==info.szPointType[0] || 'B'==info.szPointType[0] );
    {
      W_SMANALOG *pObj = (W_SMANALOG*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( szArmFullName, info.NAME, W_SMANALOG::TypeID, 0 );
      if( !pObj )
        return NULL;
      ret = &pObj->PV;
      strcpy_s( pObj->DESC, info.DESC );
      strcpy_s( pObj->EUDESC, info.szEngineeringUnits );
    }
    break;
    break;
  default:
    ASSD(0);
  }
  return ret;
}
