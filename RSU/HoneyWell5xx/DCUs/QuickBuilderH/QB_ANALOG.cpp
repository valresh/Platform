#include "QB_Class.h"
#include <rsuErr.h>
#include "QBPARM_INIT.h"
#include "ParmVarInfo.h"
#include <_alarmentry.h>
#include <ConnectQB.h>
#include <macros/StrHelps.h>
#include <rsuProjectVersion.h>

nRSUx::SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix );

LIST_PARM(SQB_ANALOG,W_QBANALOG,130)

LIST_ALGO(Algo4,SQB_ANALOG,10)
LIST_ALGO(Algo7,SQB_ANALOG,10)
LIST_ALGO(Algo15,SQB_ANALOG,10)
LIST_ALGO(Algo20,SQB_ANALOG,10)

SQB_ANALOG::SQB_ANALOG()
{
}

static int *pkVarInfoAlgo = NULL;
static SVarInfo *pVarInfoAlgo = NULL;
static int maxCountAlgo = 0;

SVarInfo* InitInfoAlgo( eVarType type, USHORT shift, USHORT size, LPCSTR pszVarName)
{
  ASS( *pkVarInfoAlgo < maxCountAlgo );
  SVarInfo * pV = &(pVarInfoAlgo[(*pkVarInfoAlgo)++]);
  pV->sVar = shift;
  pV->VarName = pszVarName;
  pV->Type = type;
  pV->varSize = size;
  pV->flags |= s_defFlag;
  return pV;
}

#define _offsetof(s,m) offsetof(s,m), sizeof(((s *)0)->m)

void SQB_ANALOG::InitParm()
{
#include <QuickBuilder/Analog.h>
  s_defFlag = SVarInfo::efParam;
#include <QuickBuilder/Analog_P.h>
  qsort( VarInfo, kVarInfo, sizeof( SVarInfo ), CompVarInfo );

  pVarInfoAlgo = VarInfoAlgo4;
  pkVarInfoAlgo = &kVarInfoAlgo4;
  maxCountAlgo = _countof(VarInfoAlgo4);
  InitInfoAlgo( evtDouble, _offsetof(SQB_ANALOG, SQB_ANALOG::algo4.ALG01), "ALG(01)");
  InitInfoAlgo( evtString, _offsetof(SQB_ANALOG, SQB_ANALOG::algo4.ALG03), "ALG(03)");
  InitInfoAlgo( evtDouble, _offsetof(SQB_ANALOG, SQB_ANALOG::algo4.ALG04), "ALG(04)");
  InitInfoAlgo( evtString, _offsetof(SQB_ANALOG, SQB_ANALOG::algo4.ALG06), "ALG(06)");
  InitInfoAlgo( evtDouble, _offsetof(SQB_ANALOG, SQB_ANALOG::algo4.ALG07), "ALG(07)");
  InitInfoAlgo( evtDouble, _offsetof(SQB_ANALOG, SQB_ANALOG::algo4.ALG10), "ALG(10)");
  InitInfoAlgo( evtDouble, _offsetof(SQB_ANALOG, SQB_ANALOG::algo4.ALG13), "ALG(13)");
  InitInfoAlgo( evtDouble, _offsetof(SQB_ANALOG, SQB_ANALOG::algo4.ALG16), "ALG(16)");
  InitInfoAlgo( evtString, _offsetof(SQB_ANALOG, SQB_ANALOG::algo4.ALG19), "ALG(19)");
  qsort( pVarInfoAlgo, *pkVarInfoAlgo, sizeof( SVarInfo ), CompVarInfo );

  pVarInfoAlgo = VarInfoAlgo7;
  pkVarInfoAlgo = &kVarInfoAlgo7;
  maxCountAlgo = _countof(VarInfoAlgo7);
  InitInfoAlgo( evtString, _offsetof(SQB_ANALOG, SQB_ANALOG::algo7.ALG01), "ALG(01)");
  InitInfoAlgo( evtString, _offsetof(SQB_ANALOG, SQB_ANALOG::algo7.ALG02), "ALG(02)");
  InitInfoAlgo( evtInt2, _offsetof(SQB_ANALOG, SQB_ANALOG::algo7.ALG04), "ALG(04)");
  InitInfoAlgo( evtDouble, _offsetof(SQB_ANALOG, SQB_ANALOG::algo7.ALG12), "ALG(12)");
  InitInfoAlgo( evtString, _offsetof(SQB_ANALOG, SQB_ANALOG::algo7.ALG21), "ALG(21)");
  qsort( pVarInfoAlgo, *pkVarInfoAlgo, sizeof( SVarInfo ), CompVarInfo );

  pVarInfoAlgo = VarInfoAlgo15;
  pkVarInfoAlgo = &kVarInfoAlgo15;
  maxCountAlgo = _countof(VarInfoAlgo15);
  InitInfoAlgo( evtString, _offsetof(SQB_ANALOG, SQB_ANALOG::algo15.ALG01), "ALG(01)");
  InitInfoAlgo( evtString, _offsetof(SQB_ANALOG, SQB_ANALOG::algo15.ALG03), "ALG(03)");
  InitInfoAlgo( evtDouble, _offsetof(SQB_ANALOG, SQB_ANALOG::algo15.ALG11), "ALG(11)");
  qsort( pVarInfoAlgo, *pkVarInfoAlgo, sizeof( SVarInfo ), CompVarInfo );

  pVarInfoAlgo = VarInfoAlgo20;
  pkVarInfoAlgo = &kVarInfoAlgo20;
  maxCountAlgo = _countof(VarInfoAlgo20);
  InitInfoAlgo( evtString, _offsetof(SQB_ANALOG, SQB_ANALOG::algo20.ALG01), "ALG(01)");
  qsort( pVarInfoAlgo, *pkVarInfoAlgo, sizeof( SVarInfo ), CompVarInfo );
}

void SQB_ANALOG::GetAsParams( KQbBridge2SysParam &paramCont )
{
  if( !W )
    return;
  W->GetParams( paramCont );
  KQbBase::GetAsParams( paramCont );
}

LPCSTR SQB_ANALOG::GetTypeName()
{
  return W->ClassName;
}

struct ANALOG_ALRM_HANDLE : W_QBANALOG
{
  void StepT( double dt );
protected:
  template<typename TALARMTYPE, typename TPRI>
  bool FormAlarm( TALARMTYPE &ALARMTYPE, bool &ALARMENABLED, TPRI &PRI, double Trip, BYTE qbaN )
  {
    if( TALARMTYPE::None==ALARMTYPE.V )
      return false;
    double delta = (RANGEAT100 - RANGEAT0) / 100. * ALARMDEADBANDPERCENT;
    if( TALARMTYPE::PVHigh==ALARMTYPE.V || TALARMTYPE::PVHighHigh==ALARMTYPE.V )
    {
      if( PV > Trip )
      {
        if( ALARMENABLED )
          return true;
        ALARMENABLED = true;
        BYTE Alarm = TALARMTYPE::PVHighHigh==ALARMTYPE.V ? A_2HP : A_HP;
        if( pAlarmQB )
          pAlarmQB( TAGNAME, Alarm, PRI.V+1, true, PV, &Trip, qbaN );
        return true;
      }
      if( !ALARMENABLED )
        return false;
      if( PV > (Trip-delta) )
        return true;
      ALARMENABLED = false;
      BYTE Alarm = TALARMTYPE::PVHighHigh==ALARMTYPE.V ? A_2HP : A_HP;
      if( pAlarmQB )
        pAlarmQB( TAGNAME, Alarm, PRI.V+1, false, PV, &Trip, qbaN );
      return false;
    }
    else if( TALARMTYPE::PVLow==ALARMTYPE.V || TALARMTYPE::PVLowLow==ALARMTYPE.V )
    {
      if( PV < Trip )
      {
        if( ALARMENABLED )
          return true;
        ALARMENABLED = true;
        BYTE Alarm = TALARMTYPE::PVLowLow==ALARMTYPE.V ? A_2LP : A_LP;
        if( pAlarmQB )
          pAlarmQB( TAGNAME, Alarm, PRI.V+1, true, PV, &Trip, qbaN );
        return true;
      }
      if( !ALARMENABLED )
        return false;
      if( PV < (Trip+delta) )
        return true;
      ALARMENABLED = false;
      BYTE Alarm = TALARMTYPE::PVLowLow==ALARMTYPE.V ? A_2LP : A_LP;
      if( pAlarmQB )
        pAlarmQB( TAGNAME, Alarm, PRI.V+1, false, PV, &Trip, qbaN );
      return false;
    }
    return false;
  }
};

void ANALOG_ALRM_HANDLE::StepT( double dt )
{
  bool inAlarm = false;
  if( FormAlarm( ALARMTYPE1, ALARM1ENABLED, ALARM1PRIORITY, AL1, 1 ) )
    inAlarm = true, AlarmSeverity.V = ALARM1PRIORITY.V+1;
  if( FormAlarm( ALARMTYPE2, ALARM2ENABLED, ALARM2PRIORITY, AL2, 2 ) )
    inAlarm = true, AlarmSeverity.V = ALARM2PRIORITY.V+1;
  if( FormAlarm( ALARMTYPE3, ALARM3ENABLED, ALARM3PRIORITY, AL3, 3 ) )
    inAlarm = true, AlarmSeverity.V = ALARM3PRIORITY.V+1;
  if( FormAlarm( ALARMTYPE4, ALARM4ENABLED, ALARM4PRIORITY, AL4, 4 ) )
    inAlarm = true, AlarmSeverity.V = ALARM4PRIORITY.V+1;
  if( FormAlarm( ALARMTYPE5, ALARM5ENABLED, AL5PRI, AL5, 5 ) )
    inAlarm = true, AlarmSeverity.V = AL5PRI.V+1;
  if( FormAlarm( ALARMTYPE6, ALARM6ENABLED, AL6PRI, AL6, 6 ) )
    inAlarm = true, AlarmSeverity.V = AL6PRI.V+1;
  if( FormAlarm( ALARMTYPE7, ALARM7ENABLED, AL7PRI, AL7, 7 ) )
    inAlarm = true, AlarmSeverity.V = AL7PRI.V+1;
  if( FormAlarm( ALARMTYPE8, ALARM8ENABLED, AL8PRI, AL8, 8 ) )
    inAlarm = true, AlarmSeverity.V = AL8PRI.V+1;
  if( !inAlarm )
    AlarmSeverity.V = _AlarmSeverity::Journal;
}


bool SQB_ANALOG::SetValue( LPCSTR pField, LPCSTR pszVal, LPCSTR pszVal2 )
{
  bool r = KQbBase::SetValue( pField, pszVal, pszVal2 );
  if( !strcmp(pField,"PVALGONUMBER") )
  {
    switch( W->PVALGONUMBER )
    {
    case 4:
      pObjAddrAlgo = (BYTE*)this;
      ClassVarInfoAlgo = VarInfoAlgo4;
      kClassVarInfoAlgo = kVarInfoAlgo4;
      break;
    case 7:
      pObjAddrAlgo = (BYTE*)this;
      ClassVarInfoAlgo = VarInfoAlgo7;
      kClassVarInfoAlgo = kVarInfoAlgo7;
      break;
    case 15:
      pObjAddrAlgo = (BYTE*)this;
      ClassVarInfoAlgo = VarInfoAlgo15;
      kClassVarInfoAlgo = kVarInfoAlgo15;
      if( pszVal2 )
        algo15.blockNo = atoi(pszVal2);
      break;
    case 68:
      KKK();
      break;
    case 20:
      pObjAddrAlgo = (BYTE*)this;
      ClassVarInfoAlgo = VarInfoAlgo20;
      kClassVarInfoAlgo = kVarInfoAlgo20;
      break;
    default:
      ASSD(0);
      KKK();
    case -1:
      break;
    }
  }
  switch( W->PVALGONUMBER )
  {
  case 20:
    if( !strcmp(pField,"ALG(01)") )
    {
      std::string ass( PointName );
      ass.append( ".PV = ");
      ass.append( pszVal );
      algo20script.SetScript( ass.c_str(), true );
    }
    break;
  }
  ASSD(r);
  return r;
}

void SQB_ANALOG::StepT( double dt )
{
  KQbBase::StepT( dt );
  if( W->cfa )
  {
    switch( W->changedFromArm[0] )
    {
    case 's'://SP
    case 'S':
      if( m_SPDestinc.IsConnected() )
        m_SPDestinc = W->SP;
      break;
    case 'o'://OP
    case 'O':
      if( m_OPDestinc.IsConnected() )
        m_OPDestinc = W->OP;
      break;
    case 'm'://MD
    case 'M':
      if( m_MDDestinc.IsConnected() )
        m_MDDestinc = (int)W->MODE;
      break;
    case 'A':
    case 'a':
      {
        int index = W->changedFromArm[1] - '1';
        if( index<0 || index>=A_COUNT )
          break;
        if( !m_ADestinct[index].IsConnected() )
          break;
        double *pA = &W->A1;
        m_ADestinct[index] = pA[index];
      }
      break;
    }
    W->cfa = 0;
  }

  if( W->PVPERIOD > 0 )
  {
    m_period += dt;
    if( m_period < W->PVPERIOD )
      return;
    m_period -= W->PVPERIOD;
  }

  switch( W->PVALGONUMBER )
  {
  case 4:
    if( alg4_obj3.IsConnected() )
      W->PV = ( (double)alg4_obj3*algo4.ALG01 + algo4.ALG04 + algo4.ALG07 + algo4.ALG10 ) / (algo4.ALG13 + algo4.ALG16);
    break;
  case 7:
    if( alg7_obj2.IsConnected() )
    {
      if( algo7.ALG04==(int)alg7_obj2 )
      {
        if( m_PVSource.IsConnected() )
          W->PV = m_PVSource;
      }
    }
    break;
  case 15:
    if( alg15_obj3.IsConnected() )
    {
      if( IsNaN(W->PV) )
        W->PV = 0;
      W->PV += ((double)alg15_obj3) * dt * algo15.ALG11;
    }
    break;
  case 20:
    algo20script.Algo20StepT( dt );
    break;
  case 68:
    break;
  default:
    ASSD(0);
  case -1:
    if( m_PVSource.IsConnected() )
      W->PV = m_PVSource;
    if( m_OPSource.IsConnected() )
      W->OP = m_OPSource;
    if( m_MDSource.IsConnected() )
      W->MODE = (BYTE)m_MDSource;
    if( m_SPSource.IsConnected() )
      W->SP = m_SPSource;
    double *pAs = &W->A1;
    for( int i=0; i<_countof(m_ASources); ++i )
    {
      if( !m_ASources[i].IsConnected() )
        continue;
      pAs[i] = m_ASources[i];
    }
    break;
  }
  if( W->RANGEAT100 > W->RANGEAT0 )
  {
    if( W->PV > W->RANGEAT100 )
      W->PV = W->RANGEAT100;
    if( W->PV < W->RANGEAT0 )
      W->PV = W->RANGEAT0;
  }

  ANALOG_ALRM_HANDLE *alarmImpl = (ANALOG_ALRM_HANDLE*)W;
  alarmImpl->StepT( dt );
}

void SQB_ANALOG::OnScriptAssgned( LPCSTR pszFld )
{
  switch( pszFld[0] )
  {
  case 's'://SP
  case 'S':
    if( m_SPDestinc.IsConnected() )
      m_SPDestinc = W->SP;
    break;
  case 'o'://OP
  case 'O':
    if( m_OPDestinc.IsConnected() )
      m_OPDestinc = W->OP;
    break;
  case 'm'://MD
  case 'M':
    if( m_MDDestinc.IsConnected() )
      m_MDDestinc = (int)W->MODE;
    break;
  case 'A':
  case 'a':
    {
      int index = pszFld[1] - '1';
      if( index<0 || index>=A_COUNT )
        break;
      if( !m_ADestinct[index].IsConnected() )
        break;
      double *pA = &W->A1;
      m_ADestinct[index] = pA[index];
    }
    break;
  }
}

void SQB_ANALOG::Link()
{
  W->ALARM1ENABLED = false;
  W->ALARM2ENABLED = false;
  W->ALARM3ENABLED = false;
  W->ALARM4ENABLED = false;
  W->ALARM5ENABLED = false;
  W->ALARM6ENABLED = false;
  W->ALARM7ENABLED = false;
  W->ALARM8ENABLED = false;
  W->AlarmSeverity = W_QBANALOG_P::_AlarmSeverity::Def;

  m_period = W->PVPERIOD;
  if( -1==W->PVALGONUMBER )
      KQbBase::Link();
  ScriptsAfterStateRestored();

  if( 0 && ns_rsu::T1163_ONPZ==rsuProjectVersion() )
  {
    int n = 0;
    if( m_scripts[0].szFieldOfObj=="A1" && m_scripts[4].szFieldOfObj=="AL1" )
      if( W->A1 != W->AL1 )
        W->A1 = W->AL1, n++;
    if( m_scripts[1].szFieldOfObj=="A2" && m_scripts[5].szFieldOfObj=="AL2" )
      if( W->A2 != W->AL2 )
        W->A2 = W->AL2, n++;
    if( m_scripts[2].szFieldOfObj=="A3" && m_scripts[6].szFieldOfObj=="AL3" )
      if( W->A3 != W->AL3 )
        W->A3 = W->AL3, n++;
    if( m_scripts[3].szFieldOfObj=="A4" && m_scripts[7].szFieldOfObj=="AL4" )
      if( W->A4 != W->AL4 )
        W->A4 = W->AL4, n++;
    if( n )
      OutputDebugString( PointName ), OutputDebugString( "\n" );
  }

  for( int i=0; i<_countof(m_ASources); ++i )
  {
    LinkPnt( m_ASources[i], m_ASourceConnectInfo[i], false );
  }
  for( int i=0; i<_countof(m_ADestinct); ++i )
  {
    LinkPnt( m_ADestinct[i], m_ADestinctConnectInfo[i], true );
  }

  switch( W->PVALGONUMBER )
  {
  case 4:
    {
      if( !algo4.ALG03[0] )
        break;
      char buf[4][32*4] = {};
      SplitString( algo4.ALG03, buf, ' ', false );
      strcat_s( buf[0], "." );
      if( buf[1][0] )
        strcat_s( buf[0], buf[1] );
      else if( buf[2][0] )
        strcat_s( buf[0], buf[2] );
      else
      {
        ASSD(0);
      }
      nRSUx::SParamInfo link = RsuConnectParamInfo( buf[0], NULL, NULL );
      alg4_obj3.ManualInit( link );
    }
    break;
  case 7:
    {
      char buf[4][32*4] = {};
      SplitString( algo7.ALG02, buf, ' ', false );
      strcat_s( buf[0], "." );
      if( buf[1][0] )
        strcat_s( buf[0], buf[1] );
      else if( buf[2][0] )
        strcat_s( buf[0], buf[2] );
      else
      {
        ASSD(0);
      }
      nRSUx::SParamInfo link = RsuConnectParamInfo( buf[0], NULL, NULL );
      alg7_obj2.ManualInit( link );
    }
    break;
  case 15:
    {
      char buf[4][32*4] = {};
      SplitString( algo15.ALG03, buf, ' ', false );
      strcat_s( buf[0], "." );
      if( buf[1][0] )
        strcat_s( buf[0], buf[1] );
      else if( buf[2][0] )
        strcat_s( buf[0], buf[2] );
      else
      {
        ASSD(0);
      }
      nRSUx::SParamInfo link = RsuConnectParamInfo( buf[0], NULL, NULL );
      alg15_obj3.ManualInit( link );
    }
    break;
  case 20:
    algo20script.Algo20Init();
    break;
  case 68:
    break;
  default:
    ASSD(0);
  case -1:
    break;
  }
}

void SQB_ANALOG::SetSourceAddressSM( LPCSTR pszController, LPCSTR pszWord, LPCSTR pszParmName )
{
  if( !*pszParmName )
    return;
  if( 'A'==*pszParmName )
  {
    int index = atoi(pszParmName+1);
    if( index>0 && index<=A_COUNT )
    {
      --index;
      FillConnectInfoSM( m_ASourceConnectInfo[index], pszController, pszWord );
    }
  }
  else
      KQbBase::SetSourceAddressSM( pszController, pszWord, pszParmName );
}

void SQB_ANALOG::SetDestinctAddressSM( LPCSTR pszController, LPCSTR pszWord, LPCSTR pszParmName )
{
  ASSD( pszParmName && *pszParmName );
  if( 'A'==*pszParmName )
  {
    int index = atoi(pszParmName+1);
    if( index>0 && index<=A_COUNT )
    {
      --index;
      FillConnectInfoSM( m_ADestinctConnectInfo[index], pszController, pszWord );
    }
  }
  else
      KQbBase::SetDestinctAddressSM( pszController, pszWord, pszParmName );
}

void SQB_ANALOG::SetSourceAddressMOD( LPCSTR pszController, LPCSTR pszWord1, LPCSTR pszWord2, int ID, LPCSTR pszParmName )
{
  ASSD( pszParmName && *pszParmName );
  if( 'A'==*pszParmName )
  {
    int index = atoi(pszParmName+1);
    if( index>0 && index<=A_COUNT )
    {
      --index;
      FillConnectInfoMOD( m_ASourceConnectInfo[index], pszController, pszWord1, pszWord2, ID );
    }
  }
  else
      KQbBase::SetSourceAddressMOD( pszController, pszWord1, pszWord2, ID, pszParmName );
}

void SQB_ANALOG::SetDestinctAddressMOD( LPCSTR pszController, LPCSTR pszWord1, LPCSTR pszWord2, int ID, LPCSTR pszParmName )
{
  ASSD( pszParmName && *pszParmName );
  if( 'A'==*pszParmName )
  {
    int index = atoi(pszParmName+1);
    if( index>0 && index<=A_COUNT )
    {
      --index;
      FillConnectInfoMOD( m_ADestinctConnectInfo[index], pszController, pszWord1, pszWord2, ID );
    }
  }
  else
      KQbBase::SetDestinctAddressMOD( pszController, pszWord1, pszWord2, ID, pszParmName );
}

void SQB_ANALOG::SetSourceAddressHC900_UMB( LPCSTR pszIPxAddr, LPCSTR pszParmName )
{
  ASSD( pszParmName && *pszParmName );
  if( 'A'==*pszParmName )
  {
    int index = atoi(pszParmName+1);
    if( index>0 && index<=A_COUNT )
    {
      --index;
      strcpy_s( m_ASourceConnectInfo[index].m_szController, pszIPxAddr );
      m_ASourceConnectInfo[index].m_pntType = etpUMC_HC900;
    }
  }
  else
      KQbBase::SetSourceAddressHC900_UMB( pszIPxAddr, pszParmName );
}

void SQB_ANALOG::SetDestinctAddressHC900_UMB( LPCSTR pszIPxAddr, LPCSTR pszParmName )
{
  ASSD( pszParmName && *pszParmName );
  if( 'A'==*pszParmName )
  {
    int index = atoi(pszParmName+1);
    if( index>0 && index<=A_COUNT )
    {
      --index;
      strcpy_s( m_ADestinctConnectInfo[index].m_szController, pszIPxAddr );
      m_ADestinctConnectInfo[index].m_pntType = etpUMC_HC900;
    }
  }
  else
      KQbBase::SetDestinctAddressHC900_UMB( pszIPxAddr, pszParmName );
}

void SQB_ANALOG::SetSourceAddressRsLogixMod( LPCSTR pszNameB1, LPCSTR pszNameB2, LPCSTR pszDataType, LPCSTR pszParmName )
{
  ASSD( pszParmName && *pszParmName );
  if( 'A'==*pszParmName )
  {
    int index = atoi(pszParmName+1);
    if( index>0 && index<=A_COUNT )
    {
      --index;
      strcpy_s( m_ASourceConnectInfo[index].m_szController, pszNameB1 );
      strcpy_s( m_ASourceConnectInfo[index].m_szAddress2, pszNameB2 );
      strcpy_s( m_ASourceConnectInfo[index].szDataType, pszDataType );
      m_ASourceConnectInfo[index].m_nID = etpRsLogixMOD;
    }
  }
  else
      KQbBase::SetSourceAddressRsLogixMod( pszNameB1, pszNameB2, pszDataType, pszParmName );
}

void SQB_ANALOG::SetDestinctAddressRsLogixMod( LPCSTR pszNameB1, LPCSTR pszNameB2, LPCSTR pszDataType, LPCSTR pszParmName )
{
  ASSD( pszParmName && *pszParmName );
  if( 'A'==*pszParmName )
  {
    int index = atoi(pszParmName+1);
    if( index>0 && index<=A_COUNT )
    {
      --index;
      strcpy_s( m_ADestinctConnectInfo[index].m_szController, pszNameB1 );
      strcpy_s( m_ADestinctConnectInfo[index].m_szAddress2, pszNameB2 );
      strcpy_s( m_ADestinctConnectInfo[index].szDataType, pszDataType );
      m_ADestinctConnectInfo[index].m_nID = etpRsLogixMOD;
    }
  }
  else
      KQbBase::SetDestinctAddressRsLogixMod( pszNameB1, pszNameB2, pszDataType, pszParmName );
}

SUserDef* SQB_ANALOG::GetUserDefs( DWORD &nCount )
{
  nCount = _countof(W->userDefined);
  return W->userDefined;
}

void SQB_ANALOG::AfterRestoreState()
{
  W->ALARM1ENABLED = false;
  W->ALARM2ENABLED = false;
  W->ALARM3ENABLED = false;
  W->ALARM4ENABLED = false;
  W->ALARM5ENABLED = false;
  W->ALARM6ENABLED = false;
  W->ALARM7ENABLED = false;
  W->ALARM8ENABLED = false;
  W->AlarmSeverity = W_QBANALOG_P::_AlarmSeverity::Def;
  KKK();
}