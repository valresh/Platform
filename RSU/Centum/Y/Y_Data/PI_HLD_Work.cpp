#include "stdafx.h"
#include "Y_Data.h"
#include <macros/AutoRestore.h>

void CY_PI_HLD::Work ( double dt )
{
  double dblTC = W->TC;
  double dblTS = W->TS;

  if( 0==W->TS )
  {
    if( !W->CSW )
    {
      tsCounter = 0;
      return;
    }
    tsCounter += dt;
    if( tsCounter>=dblTC )
    {
      tsCounter = 0;
      W->CSW = 0;
    }
  }
  else
  {
    tsCounter += dt;
    if( tsCounter>=dblTC )
    {
      if( tsCounter <= dblTS )
        return;
      tsCounter -= dblTS;
    }
  }
  double prevMV = W->MV;

  if ( OUT.Status & CSignal::OOP )
    Alarm.SEND_ON( A_OOP, -1. );
  else
    Alarm.SEND_OFF( A_OOP );

  if( INT )
  {
    if (*( INT->pInt ))
    {
      W->INTERLOCK = 1;
      W->mode.M = SMode::MAN;
    }
    else
      W->INTERLOCK = 0;
  }
  else 
  {
    W->INTERLOCK = 0;
    W->FlagUse |= CAlarmBase::fuYoko;
  }
  if( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    OUT.Status |= CSignal::BAD;
    if( W->mode.M < SMode::ROUT )
      W->mode.M = SMode::MAN;
    return ;
  }
  else
    OUT.Status &= ~CSignal::BAD;

  if( W->CALI  )
  {
    W->INTERLOCK = 0;
    W->mode.M = SMode::MAN;
    OUT.Status |= CSignal::CAL;
  }
  else
    OUT.Status &= ~CSignal::CAL;

  int Status = 0;
  dPVn1 = dPVn;
  PVn1 = W->PV;

  if( IN )
  {
    if( CSignal::Z_Connection & IN->Status )
      Alarm.Possible |= A_IOP;
    IN->Status &= ~CSignal::SIGNALS_MASK;
    {
      TAutoRestore<double> arPV(W->PV);
      Alarm.TestIn( IN, dt, Status );
    }
    ObrIn.Work ( IN, W->PV, dPVn, W->CALI );
  }
  if( TSI )
  {
    if( TSI->pInt )
    {
      if( TSI->Type == CSignal::Double )
      {
        double D;
        *TSI >> D;
        W->TSW = (int)(D+0.5);
      }
      else
      {
        *TSI >> W->TSW;
      }
    }
    else
      KKK();
  }

  if( !W->CALI && IN && IN->Status & CSignal::BAD )
    W->mode.M = SMode::MAN;
  if( !W->CALI && SET && SET->Status & CSignal::BAD )
    W->mode.M = SMode::MAN;

  if( W->PSW > 0 )
  {
    W->mode.M = SMode::MAN;
    switch ( W->PSW )
    {
    case 1:
      W->MV = W->MSL;
      break;
    case 2:
      W->MV = W->MSH;
      break;
    default:
      W->MV = W->PMV;
      break;
    }
    W->PSW = 0;//C4.4 Preset Manipulated Output
  }

  if ( W->mode.M == SMode::AUT )
    W->CSV = W->SV;
  if ( W->mode.M == SMode::RCAS )
    W->CSV = W->RSV;
  else
    W->RSV = W->CSV;
  if ( W->mode.M == SMode::ROUT )
    W->MV = W->RMV;
  else 
    W->RMV = W->MV;

  if ( SET && (W->mode.M==SMode::CAS || W->mode.M==SMode::PRD) )
  {
    if (( ObrOut.OutputStatus & CSignal::CLP ) == 0 )
    {
      double MV;
      *SET >> MV;// %
      W->CSV = MV; 
    }
    else
    {
      double CSV_new;
      *SET >> CSV_new; 
      FINITE(CSV_new)
      if ( ObrOut.OutputStatus & CSignal::CLPp )
      {
        SET->Status |= CSignal::CLPp;
        if ( CSV_new < W->CSV )
          W->CSV = CSV_new;
      }
      else
        SET->Status &= ~CSignal::CLPp;
      if ( ObrOut.OutputStatus & CSignal::CLPm ) 
      {
        SET->Status |= CSignal::CLPm;
        if ( CSV_new > W->CSV )
          W->CSV = CSV_new;
      }
      else
        SET->Status &= ~CSignal::CLPm;
    }
  }

  Setpoint_value_pushback( );
  //
  Initialization_manual( );
  /**/
  W->trkError = false;
  if( CY_Base::s_IsBlk )
  {
    if ( W->TSW )
      W->mode.TRK = true;
    else
      W->mode.TRK = false;
  }
  else
  {
    if( W->TSW )
      W->trkError = true;
    W->mode.TRK = false;
  }

  if ( W->mode.IMAN || W->mode.TRK )
  {
  }
  else
  {
    if ( SET )
      SET->Status &= ~CSignal::CND;
  }
  if ( W->mode.IMAN )
  {
    if ( W->mode.M == SMode::AUT && W->Trk_AUT_CND )
      W->SV = W->PV;
    if ( W->mode.M == SMode::MAN && W->Trk_MAN )
      W->SV = W->PV;
    if ( W->mode.M == SMode::CAS && W->Trk_CAS_CND )
    {
      W->CSV = W->SV = W->PV;
      W->MV = OUT.BackVar;
    }
  }

  dMV = 0;
  if( PW==W->OutputSignalConvertion )
  {
    if ( TIN )
      *TIN >> prevMV;
    dMV = W->MV - prevMV;
  }
  else if ( W->mode.TRK )
  {
    dMV = 0.;
    if ( TIN )
      *TIN >> W->MV;
    else
      W->MV = 0.;
  }

  if( !W->mode.IMAN && !W->mode.TRK )
  {
    if ( W->mode.M == SMode::MAN || W->mode.M == SMode::ROUT )
    {
      if ( W->Trk_MAN )
        W->SV = W->PV;
    }
    else
    {
      if( W->mode.M == SMode::PRD )
      {
        double per = (W->CSV - W->SL) / (W->SH - W->SL);
        double needMV = (W->MSH - W->MSL) * per + W->MSL;
        dMV = prevMV - needMV;
      }
      else
        PID_control( dt );

      if ( !W->PIDD )
        dMV = -dMV;
      if( bFirstCall )
      {
        dMV = 0;
        bFirstCall = false;
      }
      if ( W->Positional /*&& !NoBack*/ )
        Reset_limit_function( dt );
      Deadband_action( );
    }
  }

  if( PW==W->OutputSignalConvertion )
  {
    if(fabs(dMV)>0.05 )
    {
      bool bClear = true;
      if( !(CY_Base::s_nCalcSteps % 3) )
      {
        double Tout = W->FullStrokeValue * fabs(dMV) / 100. ;
        //if( Tout > W->MinOutputWidth )
        {
          if( dMV > 0 )
            pwmUP = 1, pwmDOWN = 0, bClear = false;
          else
            pwmUP = 0, pwmDOWN = 1, bClear = false;
        }
        if( 0==W->Positional )
          ObrOut.OutputProcessing( !W->PIDD, W->mode.TRK );
      }
      if( bClear )
        pwmUP = pwmDOWN = 0;
    }
    else
    {
      if( 0==W->Positional && SMode::MAN == W->mode.M )
        W->MV = 50.;
    }
    dMV = 0;
  }
  else
    ObrOut.OutputProcessing( !W->PIDD, W->mode.TRK );

  if ( SET )
  {
    if ( ObrOut.DataStatus & CSignal::CLPp )
    {
      if ( !W->PIDD )
        SET->Status |= CSignal::CLPp;
      else
        SET->Status |= CSignal::CLPm;
    }
    else
    {
      if ( !W->PIDD/*Reverse*/ )
        SET->Status &= ~CSignal::CLPp;
      else
        SET->Status &= ~CSignal::CLPm;
    }
    if ( ObrOut.DataStatus & CSignal::CLPm )
    {
      if ( !W->PIDD/*Reverse*/ )
        SET->Status |= CSignal::CLPm;
      else
        SET->Status |= CSignal::CLPp;
    }
    else
    {
      if ( !W->PIDD/*Reverse*/ )
        SET->Status &= ~CSignal::CLPm;
      else
        SET->Status &= ~CSignal::CLPp;
    }
  }
  //
  Setpoint_value_pushback( );
  if( SET )
  {
    SET->BackVar = W->CSV;
    if ( W->mode.M != SMode::CAS && W->mode.M != SMode::PRD )
    {
      SET->Status |= CSignal::CND;
    }
    else
    {
      if ( W->mode.IMAN )
        SET->Status |= CSignal::CND;
      else
        SET->Status &= ~CSignal::CND;
    }
    //
    SET->BackVar = W->CSV;
    SET->Min = W->SL;
    SET->Max = W->SH;
    SET->IsLimits = true;
  }
  if( !_finite( W->MV ))
    W->MV = 0.;
  W->DV = W->PV - W->SV;
  Alarm.TestOut( );
  CalcSUB();
  CalcOUT();
}

void CY_PI_HLD::CalcSUB()
{
  if( eaodt_DataItem==m_tSUB )
  {
    switch( W->Aux )
    {
    case 0:
      m_SUB_Value = W->PV;
      break;
    case 1:
      m_SUB_Value = dPVn;
      break;
    case 2:
      m_SUB_Value = W->MV;
      break;
    case 3:
      m_SUB_Value = dMV;
      break;
    }
  }
  else if( eaodt_ProcessIO==m_tSUB )
  {
    switch( W->Aux )
    {
    case 0:
      m_SUB_Value = (W->PV - W->SL) / (W->SH - W->SL) * 100.;
      break;
    case 1:
      m_SUB_Value = dPVn / (W->SH - W->SL) * 100.;
      break;
    case 2:
      m_SUB_Value = (W->MV - W->MSL) / (W->MSH - W->MSL) * 100.;
      break;
    case 3:
      m_SUB_Value = dMV / (W->MSH - W->MSL) * 100.;
      break;
    }
  }
}

void CY_PI_HLD::CalcOUT()
{
  if( SUBSYS!=W->OutputSignalConvertion )
    return;
  if( W->MV > W->MH )
    W->MV = W->MH;
  if( W->MV < W->ML )
    W->MV = W->ML;
  if( s_globalApplyOutGainBias )
    W->m_mvForOutBySUBSYS = (W->MV - W->Conv_BIAS) / W->Conv_GAIN;
  else
    W->m_mvForOutBySUBSYS = W->MV;
  /*if( W->Use_MINMAX )
  {
    if( W->m_mvForOutBySUBSYS > W->Out_MAX )
      W->m_mvForOutBySUBSYS = W->Out_MAX;
    if( W->m_mvForOutBySUBSYS < W->Out_MIN )
      W->m_mvForOutBySUBSYS = W->Out_MIN;
  }*/
}

void CY_PI_HLD::Deadband_action( )
{
  double dHYS = W->HYS_DB * ( W->SH	- W->SL );
  double Emax, Emin;
  double DB = W->DB;
  if ( Deadband_state > 0 )
  {
    Emax = DB - dHYS;
    Emin = -DB;
  }
  if ( Deadband_state == 0 )
  {
    Emax = DB;
    Emin = -DB;
  }
  if ( Deadband_state < 0 )
  {
    Emax = DB;
    Emin = -DB + dHYS;
  }
  if ( En <= Emax && En >= Emin )
  {
    Deadband_state = 0;
  }
  else
  {
    if ( En > Emax )
      Deadband_state = 1;
    else
      Deadband_state = -1;
  }
  //	if ( Deadband_state == 0 )
  //		dMV = 0.;
}

void CY_PI_HLD::Reset_limit_function( double dt )
{
  return;
  if ( OUT.BackVar == 0. )
    return ;
  if ( RL1 )
    *RL1 >> W->RLV1;
  else
    W->RLV1 = OUT.BackVar;
  if ( RL2 )
    *RL2 >> W->RLV2;
  else
    W->RLV2 = 0.;
  dMV = dMV + dt / W->I * ( W->RLV1 - W->RLV2 - W->MV );
}

void CY_PI_HLD::PID_control( double dt )
{
  if ( W->P == 0. )
  {
    // Control Action Bypass
    double Ks = ( W->MSH - W->MSL ) / ( W->SH - W->SL );
    dMV = Ks * ( dt / W->I * En );
    return;
  }
  En1 = En;
  En = W->PV - W->SV;
  double Ks = ( W->MSH - W->MSL ) / ( W->SH - W->SL );
  double Kp = 100. / W->P;
  dEn1 = dEn;
  dEn = En - En1;
  double ddEn = dEn - dEn1;
  dMV = Kp * Ks * ( dEn + dt / W->I * En );
}

void CY_PI_HLD::Initialization_manual( )
{
  if ( ( OUT.Status & CSignal::CND ))
  {
    W->mode.IMAN = true;
    // Track output
  }
  else
  {
    W->mode.IMAN = false;
  }
}

void CY_PI_HLD::Setpoint_value_pushback( )
{
  switch( W->mode.M )
  {
  case SMode::AUT:
  case SMode::MAN:
    W->CSV = W->SV;
    break;
  case SMode::RCAS:
    W->SV = W->RSV;
    break;
  default:
    W->SV = W->CSV;
  }
  if( W->mode.M == SMode::CAS )
  {
    if( W->SV < W->SVL )
		  W->SV = W->SVL;
    if( W->SV > W->SVH )
      W->SV = W->SVH;
  }
}

bool CY_PI_HLD::TransferConnection( LPCSTR pszFieldInName )
{
  if( !memcmp(pszFieldInName,"MV",2) )
  {
    switch( W->mode.M )
    {
    case SMode::MAN:
      return true;
    default:
      return false;
    }
  }
  return true;
}
