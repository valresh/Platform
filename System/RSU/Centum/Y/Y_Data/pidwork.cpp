#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include <macros/AutoRestore.h>

void CY_PID::Work ( double dt )
{
  dblInDt = dt;
  if( PW!=W->OutputSignalConvertion )
  {
    if( 3!=W->Algorithm )
    {//алгоритмы, завизящие от dPV
      nCtrlPeriodWork = __max( 0, nCtrlPeriodWork );
      if( nCtrlPeriodWork > 0 && dt < nCtrlPeriodWork )
      {
        if( !CY_Base::s_flagBasicScanControl )
          return;
        if( m_nBasicControlPeriodCounter < nCtrlPeriodWork )
          return;
        dt = CY_Base::s_ControlPeriod * m_nBasicControlPeriodCounter;
        m_nBasicControlPeriodCounter = 0;
      }
    }
  }
  nStepWork = CY_Base::s_nCalcSteps;

  bool bSetMvFrom_mv4Out = false;
  double prevMV = mv4Out;//W->MV;
  if( /*user defined*/W->Display_style_MV && W->MSH > W->MSL && W->MVH>W->MVL )
  {
    if( W->MV > W->MVH )
      W->MV = W->MVH;
    if( W->MV < W->MVL )
      W->MV = W->MVL;
    double percentMV = W->MV / (0.01 * ( W->MVH - W->MVL ));
    mv4Out = ((W->MSH - W->MSL)*0.01)*percentMV;
    prevMV = mv4Out;
  }
  else
    prevMV = mv4Out = W->MV;
  /*if ( MVOpt > 0. )
    {
    MVOpt = 0.;
    }*/
  //??	MSH = MH;
//??	MSL = ML;
  //Reverse = _Reverse;
	SET_HBP
		{
 		KKK();
		}
	if ( OUT.Status & CSignal::OOP )
		Alarm.SEND_ON( A_OOP, -1. );
	else
		Alarm.SEND_OFF( A_OOP );
	//W->AF = 0;
	if ( INT )
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
    if( PW==W->OutputSignalConvertion )
    {
      if ( TIN )
        *TIN >> W->MV;
    }
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
	if ( TSI )
		{
		if ( TSI->pInt )
			{
			if ( TSI->Type == CSignal::Double )
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
//	if ( SET == NULL && W->mode.M == SMode::CAS )
//		{
//		W->mode.M = SMode::AUT;
//		}
  if( !W->CALI && IN && IN->Status & CSignal::BAD )
    W->mode.M = SMode::MAN;
  if( !W->CALI && SET && SET->Status & CSignal::BAD )
    W->mode.M = SMode::MAN;

  if( W->PSW > 0 /*&& !prevPSW*/ )
    {
    W->mode.M = SMode::MAN;
    switch ( W->PSW )
      {
      case 1:
        /*W->MV*/mv4Out = W->MSL;
      break;
      case 2:
        /*W->MV*/mv4Out = W->MSH;
      break;
      default:
        /*W->MV*/mv4Out = W->PMV;
      break;
      }
    W->PSW = 0;//C4.4 Preset Manipulated Output
    bSetMvFrom_mv4Out = true;
    }
  //prevPSW = W->PSW;
//
  if ( W->mode.M == SMode::AUT )
    W->CSV = W->SV;
  if ( W->mode.M == SMode::RCAS )
    W->CSV = W->RSV;
  else
    W->RSV = W->CSV;
  if ( W->mode.M == SMode::ROUT )
    /*W->MV*/mv4Out = W->RMV;
  else 
    W->RMV = mv4Out;//W->MV;
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
//			if ( ( ObrOut.OutputStatus & CSignal::CLPp ) && CSV_new < W->CSV )
			if ( ObrOut.OutputStatus & CSignal::CLPp )
				{
	 		  SET->Status |= CSignal::CLPp;
				if ( CSV_new < W->CSV )
					W->CSV = CSV_new;
				}
			else
	 		  SET->Status &= ~CSignal::CLPp;
//			if ( ( ObrOut.OutputStatus & CSignal::CLPm ) && CSV_new > W->CSV )
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
/*
  if ( SV && Get_SV )
		{
		if (( ObrOut.OutputStatus & CSignal::CLP ) == 0 )
			*SV >> W->CSV;
		else
			{
			double CSV_new;
			*SV >> CSV_new; 
			if ( ( ObrOut.OutputStatus & CSignal::CLPp ) && CSV_new < W->CSV )
				W->CSV = CSV_new;
			if ( ( ObrOut.OutputStatus & CSignal::CLPm ) && CSV_new > W->CSV )
				W->CSV = CSV_new;
			}
		}
*/
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
/**/
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
      /*W->MV*/mv4Out = OUT.BackVar;
    }
  }
//
  //if ( IMAN )
  //  {
  //  dMV = 0.;
  //  MV = OUT.BackVar;
  //  }
  //else
  dMV = 0;
  En1 = En;
  En = W->PV - W->SV;
  dEn1 = dEn;
  dEn = En - En1;

  if( PW==W->OutputSignalConvertion )
    {
    if ( TIN )
      *TIN >> prevMV;
    dMV = /*W->MV*/mv4Out - prevMV;
    }
  else 
    if ( W->mode.TRK )
      {
		  dMV = 0.;
		  if ( TIN )
        *TIN >> mv4Out;//W->MV;
		  else
        /*W->MV*/mv4Out = 0.;
		  }

  if ( !W->mode.IMAN && !W->mode.TRK )
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
      if ( W->Positional && !NoBack )
        Reset_limit_function( dt );
      Deadband_action( );
    }
  }

  if( PW==W->OutputSignalConvertion )
  {
    if( abs(dMV)>0.05 )
    {
      bool bClear = true;
      if( !(CY_Base::s_nCalcSteps % 3) )
      {
        double Tout = W->FullStrokeValue * abs(dMV) / 100. ;
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
        /*W->MV*/mv4Out = 50.;
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
			if ( !W->PIDD )
			  SET->Status &= ~CSignal::CLPp;
			else
			  SET->Status &= ~CSignal::CLPm;
			}
    if ( ObrOut.DataStatus & CSignal::CLPm )
			{
			if ( !W->PIDD )
	 		  SET->Status |= CSignal::CLPm;
			else
	 		  SET->Status |= CSignal::CLPp;
			}
	  else
			{
			if ( !W->PIDD )
			  SET->Status &= ~CSignal::CLPm;
			else
			  SET->Status &= ~CSignal::CLPp;
			}
    }
//
  Setpoint_value_pushback( );
	if ( SET )
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
//  FINITE(W->MV)
  if ( !_finite( mv4Out ))
    /*W->MV*/mv4Out = 0.;
	W->DV = W->PV - W->SV;
  Alarm.TestOut( );
  if( SMode::MAN!=W->mode.M || bSetMvFrom_mv4Out )
  {
    if( /*user defined*/W->Display_style_MV && W->MSH > W->MSL && W->MVH>W->MVL )
    {
      double percentMV = mv4Out / (0.01 * ( W->MSH - W->MSL ));
      W->MV = ((W->MVH - W->MVL)*0.01)*percentMV;
    }
    else
      W->MV = mv4Out;
  }

  CalcSUB();
  CalcOUT();
  if ( W->mode.M != SMode::ROUT )
    W->RMV = W->MV;
}

void CY_PID::CalcSUB()
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
      if( W->Display_style_MV )
        m_SUB_Value = mv4Out;
      else
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
      if( W->Display_style_MV )
        m_SUB_Value = (mv4Out - W->MSL) / (W->MSH - W->MSL) * 100.;
      else
        m_SUB_Value = (W->MV - W->MSL) / (W->MSH - W->MSL) * 100.;
      break;
    case 3:
      m_SUB_Value = dMV / (W->MSH - W->MSL) * 100.;
      break;
    }
  }
}

void CY_PID::CalcOUT()
{
  if( SUBSYS!=W->OutputSignalConvertion )
    return;
  if( !W->Display_style_MV )
  {
    if( W->MV > W->MH )
      W->MV = W->MH;
    if( W->MV < W->ML )
      W->MV = W->ML;
    if( s_globalApplyOutGainBias )
      W->m_mvForOutBySUBSYS = (W->MV - W->Conv_BIAS) / W->Conv_GAIN;
    else
      W->m_mvForOutBySUBSYS = W->MV;
  }
  else
  {
    if( s_globalApplyOutGainBias )
      W->m_mvForOutBySUBSYS = (mv4Out - W->Conv_BIAS) / W->Conv_GAIN;
    else
      W->m_mvForOutBySUBSYS = mv4Out;
  }
  /*if( W->Use_MINMAX )
  {
    if( W->m_mvForOutBySUBSYS > W->Out_MAX )
      W->m_mvForOutBySUBSYS = W->Out_MAX;
    if( W->m_mvForOutBySUBSYS < W->Out_MIN )
      W->m_mvForOutBySUBSYS = W->Out_MIN;
  }*/
}

bool CY_PID::TransferConnection( LPCSTR pszFieldInName )
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
