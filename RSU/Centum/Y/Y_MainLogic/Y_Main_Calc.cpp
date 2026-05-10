#include "Y_MainLogic.h"
#include <rsuErr.h>
#include <macros/AutoRestore.h>
#include <math.h>


int GetInt ( void * pVar, char Type )
{
  switch ( Type )
  {
  case 'I':
    return *(int*)(pVar);
    break;
  case 'W':
    return *(BYTE*)(pVar);
    break;
  case 'D':
    {
      double src = *(double*)(pVar);
      double intptr = 0;
      double frac = modf( src, &intptr );
      if( intptr > 0 )
      {
        if( frac>0.5)
          intptr += 1.0;
      }
      else if( intptr < 0 )
      {
        if( frac>0.5)
          intptr -= 1.0;
      }
      return (int)intptr;
    }
    break;
  case 'B':
    if (*(bool*)(pVar))
      return 1;
    else
      return 0;
    break;
  default:
    ASS(FALSE)
    break;
  }
  return 0;
}

double GetDbl ( void * pVar, char Type )
{
  switch ( Type )
  {
  case 'I':
    return (double)*(int*)(pVar);
    break;
  case 'D':
    return *(double*)(pVar);
    break;
  case 'B':
    if (*(bool*)(pVar))
      return 1.;
    else
      return 0.;
    break;
  case 'W':
    return (double)*(BYTE*)(pVar);
    break;
  default:
    ASS(FALSE)
    break;
  }
  return 0.;
}

bool GetBool ( void * pVar, char Type )
{
  switch ( Type )
  {
  case 'I':
    if ( *(int*)(pVar))
      return true;
    else
      return false;
    break;
  case 'W':
    if ( *(BYTE*)(pVar))
      return true;
    else
      return false;
    break;
  case 'D':
    if ( *(double*)(pVar))
      return true;
    else
      return false;
    break;
  case 'B':
    return *(bool*)(pVar);
    break;
  default:
    ASS(FALSE)
    break;
  }
  return false;
}

BYTE GetBYTE ( void * pVar, char Type )
{
  switch ( Type )
  {
  case 'I':
    return (BYTE)*(int*)(pVar);
  case 'D':
    return (BYTE)*(double*)(pVar);
  case 'B':
    if ( *(bool*)(pVar))
      return 1;
    return 0;
  default:
    ASS(FALSE)
    break;
  }
  return 0;
}

void CY_MainLogic::TransferYConnectFlag( CObjConnection * pC )
{
  CObjConnection &con = *pC;
  if( !con.pOutObject || !con.pInObject )
    return;
  CAlarmBase::tFlagUse_ *pOutFl = con.pOutObject->GetFlagUse();
  CAlarmBase::tFlagUse_ *pInFl = con.pInObject->GetFlagUse();
  if( !pOutFl || !pInFl )
    return;
  CAlarmBase::tFlagUse_ &src = *pOutFl;
  CAlarmBase::tFlagUse_ &dst = *pInFl;
  if( !(src&CAlarmBase::fuYConnect) && !(dst&CAlarmBase::fuYConnect) )
    return;
  dst |= CAlarmBase::fuYConnect;
  src |= CAlarmBase::fuYConnect;
}

void CY_MainLogic::SetConnection( double dt, CObjConnection * pC )
{
  switch( pC->ConnectionType )
  {
  default:
    ASS( FALSE );
  case CObjConnection::ectUndefined:
    break;
  case CObjConnection::ectCondition2Signal:
    pC->condition.Cond();
  case CObjConnection::ectSignal2Signal:
    if( !pC->ContinueExchange() )
      break;
    if( pC->Sig.pRef && pC->Sig.pRef != &pC->Sig )
    {
      if( 'O'==pC->dirIO )
      {
        ASS( pC->Sig.Type==pC->Sig.pRef->Type );
        switch( pC->Sig.Type )
        {
        case CSignal::Double:
          pC->Sig = *pC->Sig.pRef->pDbl;
          break;
        default:
          ASSD(0);
        }
      }
    }
  case CObjConnection::ectVar2Signal:
    TransferYConnectFlag( pC );
    break;
  case CObjConnection::ectSignalMove2Action:
    KKK();
  case CObjConnection::ectSignalMove2Var:
    ASS( pC->pIn && pC->pOut );
    if( !pC->pIn || !pC->pOut )
      break;
    if( pC->pInObject && '.'==pC->In[pC->PntIn] )
    {
      LPCSTR pszField = &pC->In[pC->PntIn+1];
      if( !pC->pInObject->TransferConnection( pszField) )
        break;
    }
    if( pC->Sig.pRef )
    {
      if( pC->Sig.pRef->Status & CSignal::_SkipTrasfer )
        break;
    }
    TransferYConnectFlag( pC );
    if( !pC->ContinueExchange() )
      break;
    if( pC->Sig.pRef && (!pC->Sig.pRef->Used() || pC->Sig.pRef->IsBAD()) )
      break;
    switch ( pC->TypeOut )
    {
    case 'I':
      *(int*)(pC->pIn) = *(int*)(pC->pOut);
      break;
    case 'D':
      *(double*)(pC->pIn) = *(double*)(pC->pOut);
      break;
    case 'B':
      *(bool*)(pC->pIn) = *(bool*)(pC->pOut);
      break;
    case 'W':
      *(BYTE*)(pC->pIn) = *(BYTE*)(pC->pOut);
      break;
    case 'S':
      if ( pC->sizeIn > lstrlen ( (char*)pC->pOut ))/*Resh*/// в 083_FENOL_NKU pC->sizeIn = 0
        strcpy_s( (char*)pC->pIn, pC->sizeIn, (char*)pC->pOut );
      else
      {
        ASSD(0);
      }
      break;
    }
    if( pC->ConnectionType==CObjConnection::ectSignalMove2Action && pC->bAction )
      pC->action.Act( pC->bAction, dt );
    break;
  case CObjConnection::ectSignalConvert2Action:
    KKK();
  case CObjConnection::ectSignalConvert2Var:
    ASS( pC->pIn && pC->pOut );
    if( !pC->pIn || !pC->pOut )
      break;
    if( pC->pInObject && '.'==pC->In[pC->PntIn] )
    {
      LPCSTR pszField = &pC->In[pC->PntIn+1];
      if( !pC->pInObject->TransferConnection( pszField) )
        break;
    }
    if( pC->Sig.pRef )
    {
      if( pC->Sig.pRef->Status & CSignal::_SkipTrasfer )
        break;
    }
    TransferYConnectFlag( pC );
    if( !pC->ContinueExchange() )
      break;
    if( pC->Sig.pRef && (!pC->Sig.pRef->Used() || pC->Sig.pRef->IsBAD()) )
      break;
    switch ( pC->TypeIn )
    {
    case 'I':
      if ( pC && pC->pIn )
        *(int*)(pC->pIn) = GetInt( pC->pOut, pC->TypeOut );
      break;
    case 'D':
      if ( pC && pC->pIn )
        *(double*)(pC->pIn) = GetDbl( pC->pOut, pC->TypeOut );
      break;
    case 'B':
      if ( pC && pC->pIn )
        *(bool*)(pC->pIn) = GetBool( pC->pOut, pC->TypeOut );
      break;
    case 'W':
      if ( pC && pC->pIn )
        *(BYTE*)(pC->pIn) = GetBYTE( pC->pOut, pC->TypeOut );
      break;
    }
    if( pC->ConnectionType==CObjConnection::ectSignalConvert2Action && pC->bAction )
      pC->action.Act( pC->bAction, dt );
    break;
  }
}

enum
{
  nTrace = 50,
};

int CY_MainLogic::StepTL(double dt)
{
  g_CalcStep = CY_Base::s_nCalcSteps;
#ifndef _WIN64
  if( CY_Base::s_nCalcSteps < 3 )
    return 0;
#endif

  DWORD s = 0;
  if( CY_Base::s_nCalcSteps < nTrace )
    s = GetTickCount();

  double dt_sec = dt * 3600.;

  m_timeHolder += dt_sec;

  TAutoRestore<bool> arFlagControl( CY_Base::s_flagBasicScanControl );

  if( m_timeHolder >= CY_Base::s_ControlPeriod )
  {
    CY_Base::s_flagBasicScanControl = true;

    while( m_timeHolder >= CY_Base::s_ControlPeriod )
    {
      m_timeHolder -= CY_Base::s_ControlPeriod;
    }
  }
 
  if( m_pCentumSystem )
    m_pCentumSystem->bYinCalc = true;
 
  for ( int n = 0; n < kD_Obj; n++ )
    pD_Obj[n]->Work( dt_sec );

  for( int n = 0; n < kY_Obj; n++ )
  {
    if( CY_Base::s_flagBasicScanControl )
      pY_Obj[n]->m_nBasicControlPeriodCounter++;

    if( CY_Base::ectPAZ==pY_Obj[n]->controllerType )
    {
#ifdef _DEBUG
      if( pY_Obj[n]->sequenceNumberInDR >= 0 )
        KKK();
#endif
      pY_Obj[n]->Work( dt_sec );
    }
  }
  
  //основные элементы в порядке на  DiagramDrawing
  for ( int n = 0; n < kDR; n++ )
  {
    DR[n]->WorkYBlks( -1, dt_sec, this );
  }

  if( m_pCentumSystem )
    m_pCentumSystem->bYinCalc = false;

  if( CY_Base::s_nCalcSteps < nTrace )
  {
    DWORD d = GetTickCount() - s;
    char szDebug[64];
    sprintf_s( szDebug, "время шага (%d) Centum %d ms\n", CY_Base::s_nCalcSteps, d );
    OutputDebugString( szDebug );
  }

	return 0;
}

void CY_MainLogic::ConnectionExec( int firstC, int untilC, double dt, char dirIO )
{
  if( firstC < 0 )
    return;
  ASSD( untilC <= kConnection );
  if( untilC > kConnection )
    return;
  for ( int n = firstC; n < untilC; n++ )
  {
    CObjConnection * pC = &Connection[n];
    if( pC->dirIO!=dirIO )
      continue;
    SetConnection( dt, pC );
    if( 'O'==pC->dirIO && pC->pInObject && !strncmp(pC->pInObject->Model,"PIO",3) )
    {
      pC->pInObject->Work( dt );
      pC->pInObject->sequenceNumberInDR = -100;
    }
  }
}

extern CY_MainLogic * pYMainLogic;
void CY_MainLogic::ExecConnectionTransfer( int nConnect, double dt, char dirIO )
{
  if( !pYMainLogic )
    return;
  pYMainLogic->ConnectionExec( nConnect, nConnect+1, dt, dirIO );
}
