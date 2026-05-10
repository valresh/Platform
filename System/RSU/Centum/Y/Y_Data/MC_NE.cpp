#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <macros/StrHelps.h>

// D1.17
static CY_Create MC_2 ( "MC-2", CY_MC_NE::Create, CY_MC_NE::_2 );
static CY_Create MC_3 ( "MC-3", CY_MC_NE::Create, CY_MC_NE::_3 );
static CY_Create MC_2E ( "MC-2E", CY_MC_NE::Create, CY_MC_NE::_2E );
static CY_Create MC_3E ( "MC-3E", CY_MC_NE::Create, CY_MC_NE::_3E );


CY_MC_NE::CY_MC_NE( char * _Name, int _Type, int number, int domain ) : CY_Base()
, PulseW( 5. )
{
  Type = (eType)_Type;
	IN  = NULL;
	IN1  = NULL;
	IN2  = NULL;
	TT  = NULL;
	FB  = NULL;
	SWI  = NULL;
	INT  = NULL;
	IL  = NULL;
	TSI  = NULL;
  W = NULL;
  Time_1 = 0.;
  Time_2 = 0.;
  Time_3 = 0.;
  Timer_1 = -1;
  Timer_2 = -1;
  Timer_3 = -1;
  K_in = 0;
  K_out = 0;
  TypeOut = "";
  //PV_MV = false;
	MV_old = -1;
	AllOK = true;
  InvIN12 = false;
	Start = false;
	IL_mode = false;
	mtm = 0.;
  CLEAR(Name_IN_add);
  CLEAR(From_IN_add);
  CLEAR(To_IN_add);
  pPV2 = NULL;
  pPV3 = NULL;
  pPV1 = NULL;
	OUT_dummy = 0;
	OUT1 = 0;
	OUT2 = 0;
	OUT3 = 0;
  dblONTM = 0;
  ZeroMemory( pPIOouts, sizeof(pPIOouts) );
  TT_mode = false;
  switch ( Type )
    {
    case _2E:
      W = (CMC_2E*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CMC_2E::TypeID, number, domain );
    break;
    case _3E:
      W = (CMC_3E*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CMC_3E::TypeID, number, domain );
    break;
    case _2:
      W = (CMC_2*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CMC_2::TypeID, number, domain );
    break;
    case _3:
      W = (CMC_3*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CMC_3::TypeID, number, domain );
    break;
    default:
      ASS(FALSE)
    break;
    }
  CY_Base::InitFlagUse( W->FlagUse );
  Alarm.SetBlk( W );
  Alarm.pObj = this;
  Alarm.pCALI = &W->CALI;
  Alarm.BlockName = Name;
  Alarm.pAlarmLevel = &W->AlarmLevel;
  INFO(W->PV,"PV")
  INFO(W->MV,"MV")
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  kOut = 3;
  if( Type==_2E )
  {
    Out[0].Name = "OUT1";
    Out[0] << OUT1;
    Out[1].Name = "OUT2";
    Out[1] << OUT2;
    kOut = 2;
  }
  else if( Type==_3E )
  {
    Out[0].Name = "OUT1";
    Out[0] << OUT1;
    Out[1].Name = "OUT2";
    Out[1] << OUT2;
    Out[2].Name = "OUT3";
    Out[2] << OUT3;
  }
  else
  {
    Out[0].Name = "OUT";
    Out[0] << OUT1;
    if( Type==_2 )
      kOut = 2;
    else if( Type==_3 )
      kOut = 3;
  }
  InvOUT12 = false;
  W->mode.M = SMode::MAN;
}

void CY_MC_NE::ResetAlarms()
{
  Alarm.SetBlk( W );
  for( int i=0; i<_countof(_PLwork); ++i )
  {
    _PLwork[i].szText[0] = 0;
  }
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_MC_NE,CMC_NE_COMM,31,55)

void CY_MC_NE::InitParm()
  {
#include "Yokogawa\MC_P.hpp" 
#include "Yokogawa\MC.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_MC_NE::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_MC_NE::GetIN ( char * Name )
	{
	GET_IN(IN)
	GET_IN(IN1)
	GET_IN(IN2)
	GET_IN(TT)
	GET_IN(FB)
	GET_IN(SWI)
	GET_IN(INT)
	GET_IN(IL)
	GET_IN(TSI)
	return NULL;
	}

void CY_MC_NE::AnswerbackCheck( double dt )
{
	double T = mtm + dt;
	bool OverTime = false;
	if ( T > W->MTM )
		{
		T = W->MTM;
		OverTime = true;
		}
	mtm = T;
// ANS+ MV==2 PV!= 2
// ANS- MV==0 PV!= 0
//	Ans_Abnorm_Alarm NO;ON;OFF;BOTH
// Запуск таймеров
	if( W->MV != MV_old )
		{
		Alarm.SendAlarm( A_INT, false );
		MV_old = W->MV;
// Изменение MV
		switch ( W->Answerback_Check )
			{
			case 0://NO
				AllOK = true;
				return;
			break;
			case 1://ON
				if ( W -> MV == 2 )
					{
					mtm = 0.;
					OverTime = false;
					}
				else
					{
					AllOK = true;
					return;
					}
			break;
			case 2://OFF
				if ( W -> MV == 0 )
					{
					mtm = 0.;
					OverTime = false;
					}
				else
					{
					AllOK = true;
					return;
					}
			break;
			case 3://BOTH
				mtm = 0.;
				OverTime = false;
			break;
			}
		}
	if ( Alarm.State & A_ANSP )
		{
		if ( W -> MV != 2 || W -> PV == 2 )
			{
	// NR
			Alarm.SendAlarm( A_ANSP, false );
			}
		}
	if ( Alarm.State & A_ANSM )
		{
		if ( W -> MV != 0 || W -> PV == 0 )
			{
	// NR
			Alarm.SendAlarm( A_ANSM, false );
			}
		}
  if( W->MV == W->PV )
    {
		AllOK = true;
	  if( !IL_mode )
      W->BSTS = W->BSTS.NR;
		Alarm.SendAlarm( A_ANSP, false );
		Alarm.SendAlarm( A_ANSM, false );
    return;
    }
	AllOK = false;
// Проверка PV
	switch ( W->Answerback_Check )
	{
	case 0://NO
    if( !IL_mode )
      W->BSTS = W->BSTS.NR;
	  break;
	case 1://ON
		if ( OverTime )
		{
	    if( !IL_mode )
        W->BSTS = W->BSTS.NR;
			if ( W->MV == 2 && W->PV != 2 )
			{
				Alarm.SendAlarm( A_ANSP, true );
        if ( W->MV != MV_On_IL && ( W->Fallback_operation & 2 ) )
        {
				  Alarm.SendAlarm( A_INT, true );
          W->MV = MV_On_IL;
        }
        return;
			}
		}
		else
		{
      W->BSTS = W->BSTS.ANCK;
		}
	  break;
	case 2://OFF
		if ( OverTime )
		{
  		if( !IL_mode )
        W->BSTS = W->BSTS.NR;
			if ( W -> MV == 0 && W -> PV != 0 )
				{
				Alarm.SendAlarm( A_ANSM, true );
        if ( W->MV != MV_On_IL && ( W->Fallback_operation & 1 ) )
          {
				  Alarm.SendAlarm( A_INT, true );
          W->MV = MV_On_IL;
          }
        return;
				}
      AllOK = true;
		}
		else
		{
      W->BSTS = W->BSTS.ANCK;
		}
	  break;
	case 3://BOTH
		if( OverTime )
		{
	    if ( !IL_mode )
        W->BSTS = W->BSTS.NR;
		  if ( W -> MV == 2 && W -> PV != 2 )
			{
			  Alarm.SendAlarm( A_ANSP, true );
        //??W->mode.M = SMode::MAN;
        if ( W->MV != MV_On_IL && ( W->Fallback_operation & 2 ) )
        {
			    Alarm.SendAlarm( A_INT, true );
          W->MV = MV_On_IL;
        }
        return;
			}
			if ( W -> MV == 0 && W -> PV != 0 )
			{
				Alarm.SendAlarm( A_ANSM, true );
        //??W->mode.M = SMode::MAN;
        if ( W->MV != MV_On_IL && ( W->Fallback_operation & 1 ) )
          {
				  Alarm.SendAlarm( A_INT, true );
          W->MV = MV_On_IL;
          }
        return;
			}
		}
		else
		{
      W->BSTS = W->BSTS.ANCK;
		}
    break;
  }
}

void CY_MC_NE::Work ( double dt )
{
  SET_HBP
  {
    KKK();
  }
  WorkImpl( dt, false );
}

void CY_MC_NE::WorkImpl( double dt, bool bSafeCSV /*= false*/ )
{
  if( TSI )
    *TSI >> W->TSW;
  if( W->TSW )
	  W->mode.TRK = true;
  else
	  W->mode.TRK = false;
	if ( IL == NULL )
		IL_mode = false;
  int MV_On_IL = 0;
  switch ( GetOutputOnIL() )
    {
    case 0:
      MV_On_IL = 0;
    break;
    case 1:
      MV_On_IL = 2;
    break;
    case 2:
      MV_On_IL = 1;
    break;
    }
	if ( TT )
		{
		int PV_TT;
		*TT >> PV_TT;
		if ( PV_TT )
			{
			Alarm.SendAlarm( A_TRIP, true );
			TT_mode = true;
			}
		else
			{
			Alarm.SendAlarm( A_TRIP, false );
      TT_mode = false;
			}
		}
	if ( IL )
		{
		int PV_IL = -1;
		*IL >> PV_IL;
		if ( W->IL_Signal_Direction )
			{
// Reverse
			if ( PV_IL == 0 )
			{
				IL_mode = true;
			}
			else 
				IL_mode = false;
			}
		else
			{
// Direct
				if ( PV_IL > 0 )
				{
				IL_mode = true;
				}
			else 
				IL_mode = false;
			}
		}
  if( W->mode.TRK )
    IL_mode = false;
  if ( W->mode.M <= SMode::PRD )
		W->mode.M = SMode::MAN;
	if ( INT )
	{
		int INT_PV;
		*INT >> INT_PV;
		if ( INT_PV )
			W->mode.M = SMode::MAN;//MAN Fallback p476
	}

  if( TT_mode && !W->mode.TRK )
  {
    W->MV = MV_On_IL;
  }

  if( W->CALI )
    W->mode.M = SMode::MAN;

	if( IL_mode && !W->mode.TRK )
	{

    bool bInInt = false;
    if( Alarm.State & A_INT )
      bInInt = true;

		if ( W->MV != MV_On_IL )
			Alarm.SendAlarm( A_INT, true );

    if( W->Hold_Output_when_IL || !bInInt )
		  prevMV = W->MV = MV_On_IL;
    W->BSTS = W->BSTS.LOCK;
    W->MV = prevMV;
	}
	else
	{
	  Alarm.SendAlarm( A_INT, false );
    if ( W->mode.M == SMode::AUT || W->mode.M == SMode::CAS )
    {
      W->MV = W->CSV;
    }
  }

  switch ( GetOutputConversion() )
  {
  case _2ST:
  case _2PL:
    if( W->MV )
      W->MV = 2;
    break;
  }
  if( !bSafeCSV )
    W->CSV = W->MV;

  switch ( K_out )
    {
    case 0:
    break;
    case 1:
      Work_1out( dt );
    break;
    case 2:
      Work_2out( dt );
    break;
    case 3:
      Work_3out( dt );
    break;
    default:
      ASS(FALSE)
    break;
    }
//
  if ( FB )
	{
		if ( FB ->Type == CSignal::NoType )
			W->FV = 0.;
		else
			*FB >> W->FV;
	}
  if ( W->FV > W->SH )
    W->FV = W->SH;
  if ( W->FV < W->SL )
    W->FV = W->SL;
  SetPV( );
  //W->PV = PV;
//-------------------------------------

  CheckONs( prevMV, W->MV, dt );
  prevMV = W->MV;

  if( !W->mode.TRK )
    WorkMV();
  else
  {
    switch( GetOutputConversion() )
    {
    case _2PL:
      if( prevMV == 2 )
      {
        Timer_1 = 1;
        Timer_2 = -1;
      }
      else
      {
        Timer_1 = -1;
        Timer_2 = 1;
      }
      break;
    case _3PL:
      if( prevMV == 2 )
      {
        Timer_1 = 1;
        Timer_2 = -1;
        Timer_3 = -1;
      }
      else if( prevMV == 0 )
      {
        Timer_1 = -1;
        Timer_2 = 1;
        Timer_3 = -1;
      }
      else
      {
        Timer_1 = -1;
        Timer_2 = -1;
        Timer_3 = 1;
      }
      break;
    }
    Time_1 = 0.;
    Time_2 = 0.;
    Time_3 = 0.;
  }
	if( W->mode.TRK )
	{
		OUT1 = OUT2 = OUT3 = 0;
  }

// Answerback Check
	if ( W->Answerback_Trk && ( W->BPSW == 4 || W ->mode.TRK ) && !IL_mode )
		W->CSV = MV_old = prevMV = W->MV = W->PV;
	if ( MV_old < 0 )
		MV_old = W->MV;
	if ( W->MV != MV_old )
		AllOK = false;
	if ( W->MV != W->PV )
		AllOK = false;
	if ( W->BPSW == 0)
		{
			W->SV = W->FV;
	}
// вариант без концевиков
	if ( W->BPSW == 1)
		{
			W ->PV = W ->MV;
	}

//  MV_On_IL = 0;
	if ( !AllOK )
		{
		if ( W->BPSW != 1 && W->BPSW	!= 3 && W->MV != 1 && !W->mode.TRK )
			{
			AnswerbackCheck( dt );
			}
		else
			{
			AllOK = true;
			if( !IL_mode )
			  W->BSTS = W->BSTS.NR;
			Alarm.SendAlarm( A_ANSP, false );
			Alarm.SendAlarm( A_ANSM, false );
			MV_old = W->MV;
			}
		}
	else
		{
		if( !IL_mode )
		  W->BSTS = W->BSTS.NR;
		Alarm.SendAlarm( A_ANSP, false );
		Alarm.SendAlarm( A_ANSM, false );
		}
  if ( pPV1 )
    *pPV1 = OUT1;
  if ( pPV2 )
    *pPV2 = OUT2;
  if ( pPV3 )
    *pPV3 = OUT3;
 }

void CY_MC_NE::SetPV( )
  {
//PV p469
//Answer_Direction
//Numb_of_IP
//	if ( Type == _2 || Type == _3	)
//    Numb_of_IP = 0;
//	if ( Type == _2E || Type == _3E )
//    Numb_of_IP = 1;

 // int k_IN = 0;
	//if ( IN )
	//	k_IN++;
	//if ( IN1 )
	//	k_IN++;
	//if ( IN2 )
	//	k_IN++;
	/*if ( K_in < 1 || K_in > 2 )
	{
		W->PV = MV;
		return;
	}*/
//	if ( k_IN == 1 )
//		W->Numb_of_IP = 0;
//	if ( k_IN == 2 )
//		W->Numb_of_IP = 1;
  switch ( K_in )
    {
    case 1: //1 point
      {
      if ( IN1 || IN )
        {
        int RV = INT_MAX;
				if ( IN1 )
					*IN1 >> RV; 
				else
					*IN >> RV; 
        if ( W->Answer_Direction == 0 ) // Direct
          {
          if ( RV )
            W->PV = 2;
          else
            W->PV = 0;
          }
        else
          {
          if ( RV )
            W->PV = 0;
          else
            W->PV = 2;
          }
        }
      else
        {
        //ASS(FB!=NULL)
        if ( W->FV >= W->ANSP )
          W->PV = 2;
        else
          W->PV = 0;
        }
      }
    break;
    case 2: //2 point
    {
      if( IN1 || IN || IN2 )
      {
//        K_in = 2;
        int RV1 = 0;//INT_MAX;
        int RV2 = 0;//INT_MAX;
        if ( IN )
          *IN >> RV1;
        if ( IN1 )
          *IN1 >> RV1;
        if( IN2 )
          *IN2 >> RV2;
        if ( W->Answer_Direction == 0 ) // Direct
          {
          if ( RV1 && !RV2 )
            W->PV = 2;
          else
            {
            if ( !RV1 && !RV2 )
              W->PV = 1;
            else
              {
              if ( !RV1 && RV2 )
                W->PV = 0;
              }
            }
          }
        if ( W->Answer_Direction == 1 ) // Reverse
          {
          if ( RV1 && !RV2 )
            W->PV = 0;
          else
            {
            if ( !RV1 && RV2 )
              W->PV = 2;
            else
              {
              if ( RV1 && RV2 )
                W->PV = 1;
              }
            }
          }
        if ( W->Answer_Direction == 2 ) // Inverted Direct
          {
          if ( RV1 && !RV2 )
            W->PV = 0;
          else
            {
            if ( !RV1 && RV2 )
              W->PV = 2;
            else
              {
              if ( !RV1 && !RV2 )
                W->PV = 1;
              }
            }
          }
        if ( W->Answer_Direction == 3 ) // Inverted Reverse
          {
          if ( RV1 && !RV2 )
            W->PV = 2;
          else
            {
            if ( RV1 && RV2 )
              W->PV = 1;
            else
              {
              if ( !RV1 && RV2 )
                W->PV = 0;
              }
            }
          }
      }
      else if( FB )
      {
        ASS(FB!=NULL)
        if ( W->FV >= W->ANSP )
          W->PV = 2;
        else
          W->PV = 0;
      }
    }
    break;
    }
  }

int CY_MC_NE::GetOutCount()
{
  int outs = 0;
  switch( Type )
  {
  case _2:
    outs = 1;
    if( 1==GetOutputConversion() )
      outs = 2;
    break;
  case _2E:
    outs = 2;
    break;
  case _3:
    outs = 2;
    if( 3==GetOutputConversion() )
      outs = 3;
    break;
  case _3E:
    outs = 3;
    break;
  }
  return outs;
}

int CY_MC_NE::GetOutputConversion()
{
  int Output_Conversion = -1;
  switch( Type )
  {
  case _2:
  case _2E:
    Output_Conversion = W->Output_Conversion2;
    break;
  default:
    ASS(0);
  case _3:
  case _3E:
    Output_Conversion = W->Output_Conversion3;
		}
  return Output_Conversion;
}

int CY_MC_NE::GetOutputOnIL()
{
  int OutputOnIL = 0;
  switch( Type )
  {
  case _2:
  case _2E:
    OutputOnIL = W->Output_On_IL2;
    break;
  default:
    ASS(0);
  case _3:
  case _3E:
    OutputOnIL = W->Output_On_IL3;
  }
  return OutputOnIL;
}

CY_MC_NE::SPulseOutMoment::SPulseOutMoment()
{
  szText[0] = 0;
}

void CY_MC_NE::SPulseOutMoment::Start( int nStep )
{
  nStepOn = nStep;
  nStepOff = 0;
  sprintf_s( szText, "%d /", nStepOn );
}

void CY_MC_NE::SPulseOutMoment::Stop( int nStep )
{
  nStepOff = nStep;
  sprintf_s( szText, "%d / %d", nStepOn, nStepOff );
}

void CY_MC_NE::WorkMV( )
{
	SET_HBP
		{
    KKK();
		}
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    return;
  }
// Out_Action_Direction
// p489
  switch ( GetOutputConversion() )
    {
    case _2ST:
			//K_out = 1;
      if ( W->Out_Action_Direction == 0 )
        {
        //TypeOut = "2ST-dir";
        if( prevMV == 2 )
					{
          OUT1 = 1;
          OUT2 = 0;
		  if ( W->BPSW !=0)
			{
			W->SV = 100.;
			}
					}
        else
					{
          OUT1 = 0;
          OUT2 = 1;
		  if ( W->BPSW !=0)
		  {
		  W->SV = 0.;
		  }
					}
        }
      else
        {
        //TypeOut = "2ST-rev";
        if( prevMV == 0 )
				{
          OUT1 = 1;
          OUT2 = 0;
		  if( W->BPSW !=0)
			{
			 W->SV = 100.;
			}
					}
        else
					{
          OUT1 = 0;
          OUT2 = 1;
		    if ( W->BPSW !=0)
			{
			 W->SV = 0.;
			}
					}
        }
    break;
    case _2PL:
      {
      //TypeOut = "2PL";
      if ( K_out < 2 )
        Timer_2 = Timer_3 = -1;
      if ( Timer_1 == 0 || Timer_2 == 0 )
        break;
      if ( prevMV == 2 )
      {
        if ( Timer_1 < 0 )
        {
          Timer_1 = 0;
          Time_1 = PulseW;
          Timer_2 = -1;
          OUT1 = 1;
          OUT2 = 0;
		      if ( W->BPSW !=0)
			    {
			      W->SV = 100.;
			    }
        _PLwork[0].Start( CY_Base::s_nCalcSteps );
        }
      }
      else
        {
        if ( Timer_2 < 0 )
          {
          Timer_2 = 0;
          Time_2 = PulseW;
          Timer_1 = -1;
          OUT1 = 0;
          OUT2 = 1;
		    if ( W->BPSW !=0)
			{
			W->SV = 0.;
			}
          _PLwork[1].Start( CY_Base::s_nCalcSteps );
          }
        }
      }
    break;
    case _3ST:
      if ( W->Out_Action_Direction == 0 )
      {
        //TypeOut = "3ST-dir";
        if ( W->MV == 2 )
          {
          OUT1 = 1;
          OUT2 = 0;
		      if ( W->BPSW !=0)
			    {
			      W->SV = 100.;
			    }
        }
        else
        {
          if( W->MV == 1 )
          {
            OUT1 = 0;
            OUT2 = 0;
			      if ( W->BPSW !=0)
				    {
				      W->SV = 50.;
				    }
          }
          else
          {
            OUT1 = 0;
            OUT2 = 1;
			      if ( W->BPSW !=0)
				    {
				      W->SV = 0.;
				    }
          }
        }
      }
      else
      {
        //TypeOut = "3ST-rev";
        if( prevMV == 2 )
        {
          OUT1 = 0;
          OUT2 = 1;
		      if ( W->BPSW !=0)
			    {
			      W->SV = 0.;
			    }
		    }
        else
        {
          if( prevMV == 1 )
          {
            OUT1 = 1;
            OUT2 = 1;
			      if ( W->BPSW !=0)
				    {
				      W->SV = 50.;
				    }
          }
          else
          {
            OUT1 = 1;
            OUT2 = 0;
			      if ( W->BPSW !=0)
				    {
				      W->SV = 100.;
				    }
          }
        }
      }
      break;
    case _3PL:
      {
      //TypeOut = "3PL";
      if ( K_out < 3 )
        Timer_3 = -1;
      if ( K_out < 2 )
        Timer_2 = Timer_3 = -1;
      if ( Timer_1 == 0 || Timer_2 == 0 || Timer_3 == 0 )
        break;
      if( prevMV == 2 )
      {
        if ( Timer_1 < 0 )
        {
          Timer_1 = 0;
          Time_1 = PulseW;
          Timer_2 = -1;
          Timer_3 = -1;
          OUT1 = 1;
          OUT2 = 0;
          OUT3 = 0;
          _PLwork[0].Start( CY_Base::s_nCalcSteps );
        }
        break;
      }
      if( prevMV == 0 )
      {
        if ( Timer_2 < 0 )
        {
          Timer_2 = 0;
          Time_2 = PulseW;
          Timer_1 = -1;
          Timer_3 = -1;
          OUT1 = 0;
          OUT2 = 1;
          OUT3 = 0;
          _PLwork[1].Start( CY_Base::s_nCalcSteps );
        }
        break;
      }
      if( prevMV == 1 )
      {
        if ( Timer_3 < 0 )
          {
          Timer_1 = -1;
          Timer_2 = -1;
          if ( K_out >= 3 )
            {
            Timer_3 = 0;
            Time_3 = PulseW;
            OUT3 = 1;
            OUT1 = 0;
            OUT2 = 0;
            _PLwork[2].Start( CY_Base::s_nCalcSteps );
            }
          }
        }
      }
    break;
  }
  for( int i=0; i<_countof(pPIOouts); ++i )
  {
    if( !pPIOouts[i] )
      continue;
    pPIOouts[i]->Work( 0 );
  }
}

void CY_MC_NE::Work_2out ( double dt )
	{
	SET_HBP
		{
    KKK();
		}
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT1 = OUT2 = 0;
    Out[0].On ( CSignal::BAD );
    Out[1].On ( CSignal::BAD );
		return ;
    }
  else
    {
    Out[0].Off ( CSignal::BAD );
    Out[1].Off ( CSignal::BAD );
    }
  if ( Timer_1 == 0 )
    {
    Time_1 -= dt;
    if ( Time_1 < 0. )
      {
      Time_1 = 0.;
      OUT1 = 0;
      Timer_1 = 1;
      _PLwork[0].Stop( CY_Base::s_nCalcSteps );
      }
    }
  if ( Timer_2 == 0 )
    {
    Time_2 -= dt;
    if ( Time_2 < 0. )
      {
      Time_2 = 0.;
      OUT2 = 0;
      Timer_2 = 1;
      _PLwork[1].Stop( CY_Base::s_nCalcSteps );
      }
    }
  if ( Timer_3 == 0 )
    {
    Time_3 -= dt;
    if ( Time_3 < 0. )
      {
      Time_3 = 0.;
      OUT3 = 0;
      Timer_3 = 1;
      _PLwork[2].Stop( CY_Base::s_nCalcSteps );
      }
    }
	}

void CY_MC_NE::Work_3out ( double dt )
	{
	SET_HBP
		{
    KKK();
		}
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT1 = OUT2 = OUT3 = 0;
    Out[0].On ( CSignal::BAD );
    Out[1].On ( CSignal::BAD );
    Out[2].On ( CSignal::BAD );
		return ;
    }
  else
    {
    Out[0].Off ( CSignal::BAD );
    Out[1].Off ( CSignal::BAD );
    Out[2].Off ( CSignal::BAD );
    }
  if ( Timer_1 == 0 )
    {
    Time_1 -= dt;
    if ( Time_1 < 0. )
      {
      Time_1 = 0.;
      OUT1 = 0;
      Timer_1 = 1;
      _PLwork[0].Stop( CY_Base::s_nCalcSteps );
      }
    }
  if ( Timer_2 == 0 )
    {
    Time_2 -= dt;
    if ( Time_2 < 0. )
      {
      Time_2 = 0.;
      OUT2 = 0;
      Timer_2 = 1;
      _PLwork[1].Stop( CY_Base::s_nCalcSteps );
      }
    }
  if ( Timer_3 == 0 )
    {
    Time_3 -= dt;
    if ( Time_3 < 0. )
      {
      Time_2 = 0.;
      OUT3 = 0;
      Timer_3 = 1;
      _PLwork[0].Stop( CY_Base::s_nCalcSteps );
      }
    }
 	}

void CY_MC_NE::Work_1out ( double dt )
	{
	SET_HBP
		{
    KKK();
		}
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    Out[0].On ( CSignal::BAD );
		return ;
    }
  else
    {
    Out[0].Off ( CSignal::BAD );
    }
  if ( Timer_1 == 0 )
    {
    Time_1 -= dt;
    if ( Time_1 < 0. )
      {
      Time_1 = 0.;
      OUT1 = 0;
      Timer_1 = 1;
      _PLwork[0].Stop( CY_Base::s_nCalcSteps );
      }
    }
	}


void CY_MC_NE::Init( )
	{
	}

int CY_MC_NE::Prepare0()
{
  if( DBL_MAX==W->SH && DBL_MAX!=W->PH )
    W->SH = W->PH;
  if( DBL_MAX==W->SL && DBL_MAX!=W->PL )
    W->SL = W->PL;
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\MC_P.hpp" 
	return 0;
}

//
#undef VAR_DBL 
#define VAR_DBL(Var) W->Var = Var;
int CY_MC_NE::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::MAN;
  if ( InvIN12 )
    {
    CSignal * pW = IN1;
    IN1 = IN2;
    IN2 = pW;
    }
  if ( InvOUT12 )
    {
    CSignal W = Out[1];
    Out[1] = Out[0];
    Out[0] = W;
    }
//---------------------------------------------
	Start = true;//---------------------------------------------
//
  if ( W->Numb_of_IP == 0 )
    {
    if ( IN || IN1 )
      K_in = 1;
    else
      K_in = 0;
//??    K_in = 1;
//??    ASS(IN)
    }
  else
  {
    ASSD( 1==W->Numb_of_IP );
    K_in = 2;
    if ( IN1 == NULL || IN2 == NULL )
    {
      Double_IN();
      /*if ( !Double_IN( ))
      {
        K_in = W->Numb_of_IP;//??
        if( 1==K_in && IN2 )
          K_in = 2;
      }*/
    }
  }
  bool SetOut = true; 
  K_out = GetOutCount();
  if( 1==K_out )
  {
    if ( Out[0].pTo[0] == NULL )
    {
      if ( Out[1].pTo[0] )
      {
        Out[0].pTo[0] = Out[1].pTo[0];
        Out[0].pFrom = Out[1].pFrom;
        Out[0] << OUT2;
        Out[0].Name = "OUT2";
        SetOut = false;
      }
      else
        K_out = 0; // Управление через ST-16 по MV
    }
  }
  else if( 2==K_out )
  {
    if ( Out[0].pTo[0] == NULL && Out[1].pTo[0] == NULL )
      K_out = 0;
  }
  bool All = true; 
  if( K_out >= 2 && Out[1].pTo[0] == NULL )
    All = false; 
  if( K_out >= 3 && Out[2].pTo[0] == NULL )   
    All = false; 
  if( !All )
  {
    Double_OUT( K_out );
  }

  if ( SetOut && K_out == 0 )
    {
    Out[0].Name = "OUT";
	  Out[0] << OUT1;
    kOut = 1;
    }
  if ( SetOut && K_out == 1 )
    {
    Out[0].Name = "OUT";
	  Out[0] << OUT1;
    kOut = 1;
    }
  if ( SetOut && K_out == 2 )
    {
    Out[0].Name = "OUT1";
    Out[0] << OUT1;
		if ( Out[0].pFrom && strstr ( Out[0].pFrom, "OUT2" ))
			{
			Out[0].Name = "OUT2";
			Out[0] << OUT2;
			}	
    Out[1].Name = "OUT2";
    Out[1] << OUT2;
    kOut = 2;
    }
  if ( SetOut && K_out == 3 )
    {
    Out[0].Name = "OUT1";
    Out[0] << OUT1;
	  Out[1].Name = "OUT2";
	  Out[1] << OUT2;
	  Out[2].Name = "OUT3";
	  Out[2] << OUT3;
    kOut = 3;
    }
  switch ( GetOutputConversion() )
  {
  case _2ST:
    if ( W->Out_Action_Direction == 0 )
      TypeOut = "2ST-dir";
    else
      TypeOut = "2ST-rev";
    break;
  case _2PL:
    TypeOut = "2PL";
    break;
  case _3ST:
    if ( W->Out_Action_Direction == 0 )
      TypeOut = "3ST-dir";
    else
      TypeOut = "3ST-rev";
    break;
  case _3PL:
    TypeOut = "3PL";
    break;
  }

  return 0;
}

bool CY_MC_NE::Double_IN( )
  {
  if ( IN == NULL )
    return false;
  LPSTR pszPnt = strchr( IN->pFrom, '.');
  TStringTerminator st( pszPnt );
  CY_Base * pObj = pFindYObjN( IN->pFrom, pDR->NumbFCS, pDR->domain );
  ASSD(pObj);
  if( !pObj )
    return false;
  if ( strcmp ( pObj->Model, "PIO" ) == 0 )
  {
    if( pszPnt )
    {
      ++pszPnt;
      ASSD( !strcmp(pszPnt,"PV") );
    }
    CY_IO * pIO = (CY_IO *)pObj;
    pObj = pIO -> pDI;
    if( !pObj )
    {
      pIO->GetObj(true);
      pObj = pIO -> pDI;
    }
  }
  CY_DI * pDI_1 = NULL;
  CY_DI * pDI_2 = NULL;
  switch( pObj->Y_Type )
    {
    case id__WB:
      {
      pDI_1 = (CY_DI *)pObj;
      int N = pDI_1->Numb;
      int N0 = N % 100;
      int N1 = N / 100;
      if ( N0 < 16 )
        N0++;
      else
        {
        N0 = 1;
        N1++;
        }
      N = N1 * 100 + N0;
      pDI_2 = (*pFindDI)( pDI_1->NumbFCS, pDI_1->domain, N, id__WB );
      if ( pDI_2 == NULL )
        return false;
      }
    break;
    case id__Z:
      {
      pDI_1 = (CY_DI *)pObj; 
      int N = pDI_1->Numb;
      N++;
      pDI_2 = (*pFindDI)( pDI_1->NumbFCS, pDI_1->domain, N, id__Z );
      ASSD( pDI_2 );
      }
    break;
    case id__Y:
      {
        pDI_1 = (CY_DI *)pObj; 
        int N = pDI_1->Numb;
        N++;
        pDI_2 = (*pFindDI)( pDI_1->NumbFCS, pDI_1->domain, N, id__Y );
        ASSD( pDI_2 );
      }
      break;
    case id__SW:
      {
      pDI_1 = (CY_DI *)pObj; 
      int N = pDI_1->Numb;
      N++;
      pDI_2 = (*pFindDI)( pDI_1->NumbFCS, pDI_1->domain, N, id__SW );
      if ( pDI_2 == NULL )
        {
        N -= 2; //предыдущий
        pDI_2 = (*pFindDI)( pDI_1->NumbFCS, pDI_1->domain, N, id__SW );
        }
      ASSD( pDI_2 );
      }
    break;
    case id__GS:
      {
      pDI_1 = (CY_DI *)pObj;
      int N = pDI_1->Numb;
      int N0 = N % 100;
      int N1 = N / 100;
      N1++;
      N = N1 * 100 + N0;
      pDI_2 = (*pFindDI)( N0, pDI_1->domain, N, id__GS );
      ASSD( pDI_2 );
      }
    break;
    case id__AN:
      {
      pDI_1 = (CY_DI *)pObj; 
      int N = pDI_1->Numb;
      N += 1;
      pDI_2 = (*pFindDI)( pDI_1->NumbFCS, pDI_1->domain, N, id__AN );
      ASSD( pDI_2 );
      }
    break;
    default:
      ASSD(FALSE)
    return false;
    }
  if ( pDI_1 == NULL || pDI_2 == NULL )
    return false;
  CY_Base * pObj_1 = (*pFindYObjN)( pDI_1->Name, pDR->NumbFCS, pDR->domain );
  CY_Base * pObj_2 = (*pFindYObjN)( pDI_2->Name, pDR->NumbFCS, pDR->domain );
  IN1 = IN;
  IN2 = pObj_2->GetOUT ( "OUT" );
  IN2->pTo[0] = Name;
  if( !IN2->pFrom )
    IN2->pFrom = pDI_2->Name;
  IN = NULL;
  return true;
  }

bool CY_MC_NE::Double_OUT( int kOut )
  {
  if( Type==_3E || Type==_2E )
    return true;
  if( !Out[0].pTo[0] )
    return false;
  CY_Base * pObj = (*pFindDObj)( Out[0].pTo[0], pDR->NumbFCS, pDR->domain );
  if( !pObj )
  {
    LPSTR pszP = strchr( Out[0].pTo[0], '.' );
    if( pszP )
    {
      TStringTerminator st( pszP );
      pObj = (*pFindDObj)( Out[0].pTo[0], pDR->NumbFCS, pDR->domain );
    }
  }
  ASSD(pObj);
  if( !pObj )
    return false;
  CY_DI * pDI[3] = { NULL };
  switch( pObj->Y_Type )
    {
    case id__WB:
      {
      pDI[0] = (CY_DI *)pObj; 
      for ( int n = 1; n < kOut; n++ )
        {
        int N = pDI[n-1]->Numb;
        int N0 = N % 100;
        int N1 = N / 100;
        if ( N0 < 16 )
          N0++;
        else
          {
          N0 = 1;
          N1++;
          }
        int Nf = N1 * 100 + N0;
        pDI[n] = pFindDI( pDI[0]->NumbFCS, pDI[0]->domain, Nf, id__WB );
        }
      }
    break;
    case id__GS:
      KKK();
    case id__AN:
    case id__Z:
    case id__Y:
    case id__SW:
      {
      pDI[0] = (CY_DI *)pObj; 
      for ( int n = 1; n < kOut; n++ )
        {
        int N = pDI[n-1]->Numb;
        N++;
        pDI[n] = (*pFindDI)( pDI[0]->NumbFCS, pDI[0]->domain, N, pObj->Y_Type );
        ASS(pDI[n])
        }
      }
    break;
    default:
      ASS(FALSE)
    return false;
    }
  for ( int n = 0; n < kOut; n++ )
  {
    if( !pDI[n] )
      continue;
    pPIOouts[n] = (*pFindYObjN)( pDI[n]->Name, pDI[n]->NumbFCS, pDI[n]->domain );
    ASS( pPIOouts[n] );
    if( pPIOouts[n] )
    {
      if( n )
        pPIOouts[n]->sequenceNumberInDR = -200;
      CSignal ** IN = pPIOouts[n]->GetIN ( "IN" );
      *IN = &Out[n];
      Out[n].pTo[0] = pDI[n]->Name;
      Out[n].pFrom = Name;
    }
  }
  return true;
  }

static LPCSTR s_Name = "W";
int CY_MC_NE::SaveState( )
{
	SAVE_OUT
	SAVE_CLASS(s_Name,CY_MC_NE_W)
	return 0;
}

int CY_MC_NE::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	REST_OUT
	REST_CLASS(s_Name,CY_MC_NE_W)
	return 1;
}


int CY_MC_NE::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  /*for ( int n = 0; n < _countof(Out); n++ )
  {
    Out[n].StateSave( pStateSer );
  }*/
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 11 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "PV", dummyPV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "MV", prevMV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Time_1", Time_1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Time_2", Time_2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Time_3", Time_3 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Timer_1", Timer_1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Timer_2", Timer_2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Timer_3", Timer_3 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "mtm", mtm );
  WriteFullStruct( pStateSer, eKeyAnyVar, "MV_old", MV_old );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dblONTM", dblONTM );

  return 0;
}

int CY_MC_NE::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  /*for ( int n = 0; n < _countof(Out); n++ )
  {
    Out[n].StateRestore( pStateSer );
  }*/
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 11 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "PV", dummyPV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "MV", prevMV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Time_1", Time_1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Time_2", Time_2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Time_3", Time_3 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Timer_1", Timer_1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Timer_2", Timer_2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Timer_3", Timer_3 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "mtm", mtm );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "MV_old", MV_old );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dblONTM", dblONTM );
  return 0;
}

int CY_MC_NE::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM ( W->Comment, "#Что это" );
  PARM_MODEFULL( W->mode, "#MODE" )
  PARM ( W->PV, "#PV" )
  PARM ( W->MV, "#MV" )
  PARM ( W->notACK, "#noACK" )
  //PARM ( PV_MV, "#PV->MV" )
	if ( pDR && pDR->FCS )
		PARM_CALLBACK( pDR->FCS, "#FCS" );
  PARM ( K_in, "#Число входов" )
  PARM ( K_out, "#Число выходов" )
	PARM ( W->Answer_Direction, "#IN(Dir,Rev,InvDir,InvRev)" )
	PARM ( W->Out_Action_Direction, "#OUT(Dir,Rev,InvDir,InvRev)" )
  PARM ( TypeOut, "#Тип выходов")
  PARM ( InvIN12, "Переставить входы" )
  PARM ( InvOUT12, "Переставить выходы" )
	PARM ( W->MTM, "#MTM" )
  PARM ( OUT1, "#OUT1" )
  PARM ( OUT2, "#OUT2" )
  PARM ( OUT3, "#OUT3" )
  if( TT )
    PARM( TT_mode, "#TT" )
  if( IL )
    PARM( IL_mode, "#IL" )
  static SYParam::SYParamStrVal vals[] = {{"NR", W->BSTS.NR}, {"ANCK", W->BSTS.ANCK},{"LOCK", W->BSTS.LOCK} };
  params.Add( W->BSTS.V, "#BSTS", _countof(vals), vals );
  switch( GetOutputConversion() )
  {
  case _2PL:
  case _3PL:
    if( K_out>0 )
    {
      PARM( _PLwork[0].szText, "#on/off OUT1");
      PARM( Timer_1, "#Timer_1");
      PARM( Time_1, "#Time_1");
    }
    if( K_out>1 )
    {
      PARM( _PLwork[1].szText, "#on/off OUT2");
      PARM( Timer_2, "#Timer_2");
      PARM( Time_2, "#Time_2");
    }
    if( K_out>2 )
    {
      PARM( _PLwork[2].szText, "#on/off OUT3");
      PARM( Timer_3, "#Timer_3");
      PARM( Time_3, "#Time_3");
    }
    break;
  }
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\MC_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\MC.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN)
	PARM_IN(IN1)
	PARM_IN(IN2)
	PARM_IN(TT)
	PARM_IN(FB)
	PARM_IN(SWI)
	PARM_IN(INT)
	PARM_IN(IL)
	PARM_IN(TSI)
	PARM_ALL_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_MC_NE::SetLimits( )
  {
  if ( FB )
    {
    FB ->Min = W->SL;
    FB ->Max = W->SH;
    FB ->IsLimits = true;
    }
  return true;
  }

void CY_MC_NE::CheckONs( int mvOld, int mvNew, double dt )
{
  if( 0!=mvNew && !mvOld )
    W->ONCT++;
  if( 99999999>=W->ONCT || W->ONCT<0 )
    W->ONCT = 0;
  if( mvNew>0 )
  {
    dblONTM += dt;
    W->ONTM = (int)dblONTM;
    if( 99999999<=W->ONTM || W->ONTM<0 )
      W->ONTM = 0;
    W->ONTS = W->ONTM % 3600;
    W->ONTH = W->ONTM / 3600;
  }
}

bool CY_MC_NE::EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj )
{
  *ppObj = NULL;
  for( pos; pos<_countof(pPIOouts); ++pos )
  {
    if( !pPIOouts[pos] )
      continue;
    *ppName = pPIOouts[pos]->Name;
    *ppObj = pPIOouts[pos];
    ++pos;
    return true;
  }
  if( pos < 10 && IN2 && IN2->pFrom )
  {
    pos = 10;
    *ppName = IN2->pFrom;
    return true;
  }
  return false;
}

void CY_MC_NE::OnAssigned( void *pAddr )
{
  if( pAddr == &W->mode )
  {
    WorkImpl( 0 );
  }
  if( pAddr == &W->CSV )
  {
    WorkImpl( 0, true );
  }
}
