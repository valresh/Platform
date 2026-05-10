#include "stdafx.h"
#include "Y_Data.h"
#include "math.h"
#include <rsuErr.h>

#include "StdDef.h"
#include "Alarms.h"
#include <ConnectY.h>

#include "GlobalSettings.h"

CY_ALARM_BASE::CY_ALARM_BASE()
{
  ZeroMemory( this, sizeof(*this) );
}

void CY_ALARM_BASE::SetBlk( CAlarmBase * _pBlk )
{
  if( !_pBlk )
    return;
  pBlk = _pBlk;
  State = pBlk->ALRMmask = 0;
  CreateTextAlrm();
}

void CY_ALARM_BASE::SEND_ON( __int64 Flag, double Limit )
{
  if( pCALI && *pCALI )
    return;
  if (( State & Flag ) == 0 )
  {
    int ID = 0;
    __int64 M = 2LL;
    State |= Flag;
    for ( int n = 1; n < 64; n++ )
    {
      if ( Flag & M )
      {
        ID = n;
        break;
      }
      M = M << 1;
    }
    Set_ALRM ( );
    if ( pAlarmSignal )
    { 
      if( (pBlk->FlagUse&CAlarmBase::fuYConnect) )
        (*pAlarmSignal)( ID, true, pAlarmLevel ? *pAlarmLevel : 10, BlockName, pBlk->ID_CLASS, pBlk->ID_PNT_NAME );
    }
    if ( pAlarm	)
      (*pAlarm) ( pObj, ID, true, NULL, Limit );//pBlk->ALRM, true );
    pBlk->ALRMmask = State;
    CreateTextAlrm();
  }
}

void CY_ALARM_BASE::SEND_OFF( __int64 Flag )
{
  __int64 oState = State;
  if( State & Flag )
  {
    int ID = 0;
    __int64 M = 2LL;
    State &= ~Flag;
    for ( int n = 1; n < 64; n++ )
    {
      if ( Flag & M )
      {
        ID = n;
        break;
      }
      M = M << 1;
    }
    if ( ID > 0 )
    {
      if ( pAlarmSignal && (pBlk->FlagUse&CAlarmBase::fuYConnect) )
        (*pAlarmSignal)( ID, false, pAlarmLevel ? *pAlarmLevel : 10, BlockName, pBlk->ID_CLASS, pBlk->ID_PNT_NAME );
      if ( pAlarm	)
        (*pAlarm) ( pObj, ID, false, NULL, -1. );//pBlk->ALRM, false );
    }
    pBlk->ALRMmask = State;
    CreateTextAlrm();
  }
  if( State )
  {
    Set_ALRM ( );
  }
  else
  {
    pBlk->ALRMmask = State;
    if( oState != State )
    {
      if ( pAlarmSignal )
        (*pAlarmSignal)( 0, true, pAlarmLevel ? *pAlarmLevel : 10, BlockName, pBlk->ID_CLASS, pBlk->ID_PNT_NAME );
      CreateTextAlrm();
    }
    pBlk->ALRM = 0;
  }
}


void CY_ALARM_BASE::SendAlarm( __int64 Alarm, bool On )
{
  if ( On )
    SEND_ON( Alarm, -1. );
  else
    SEND_OFF( Alarm );
}

void CY_ALARM_BASE::Set_ALRM ( )
{
  if( pCALI && *pCALI )
    return;
  if ( State == 0LL )
  {
    pBlk->ALRM = 0;
    return;
  }
  int ALRM = 0;
  __int64 M = 2LL;
  for ( int n = 1; n < 64; n++ )
  {
    if ( State & M )
    {
      ALRM = n;
      break;
    }
    M = M << 1;
  }
  pBlk->ALRM = ALRM;
}
//////////////////////////////////////////////////////////////////////////
static const double s_defH_Limit = 1.25, s_defL_Limit = -0.25;
CY_ALARM::CY_ALARM()
{
  Possible = 0;
  pInput_Open_alarm = NULL;
  pSH = NULL;
  pSL = NULL;
  pH_Limit = &s_defH_Limit;
  pL_Limit = &s_defL_Limit;

  pHH = NULL;
  pLL = NULL;
  pH = NULL;
  pL = NULL;
  pHYS_HL = NULL;
  pPV = NULL;
  pHHLL = NULL;
  pHL = NULL;
  PVlast = DBL_MAX;
  V_Time = 0.;
  pSides_VL = NULL;
  pVL = NULL;
  pHYS_VL = NULL;
  pDL = NULL;
  pDeviation_DL = NULL;
  pGAIN_DL = NULL;
  pTime_DL = NULL;
  pHYS_DL = NULL;
  SVlast = DBL_MAX;
  r = 0.;
//-------------------------------------------
  pMV = NULL;
  pMH = NULL;
  pML = NULL;
  pMSH = NULL;
  pMSL = NULL;
	pHL_OUT = NULL;		// 0 - NO 1 - H 2 - L 3 - HL 
	pHYS_OUT = NULL;
	pObj = NULL;
//-------------------------------------------
  AF = NULL;
  GAINin = 1.;
  BIASin = 0.;
  }

inline bool CY_ALARM::Input_Open_Alarm( int Status )
  {
	bool Res = true;
  if( pCALI && *pCALI )
    return Res;
  bool TestH = *pInput_Open_alarm & 1;
  bool TestL = *pInput_Open_alarm >= 2;
  if ( TestH )
    {
    if ( Status & CSignal::IOPp )
      {
      SEND_ON( A_IOPP, -1. );
			Res = false;
      }
    else
      {
      SEND_OFF( A_IOPP );
      }
    }
//
  if ( TestL )
    {
    if ( Status & CSignal::IOPm )
      {
      SEND_ON( A_IOPM, -1. );
			Res = false;
      }
    else
      {
      SEND_OFF( A_IOPM );
      }
    }
	return Res;
  }

inline void CY_ALARM::HHLL_ON( )
{
  if( pCALI && *pCALI )
    return;
  //
  bool TestHH = (*pHHLL & 1) || !( AF && (*AF & 0x00080000));
  bool TestLL = (*pHHLL >= 2) || !( AF && (*AF & 0x00040000));
	if ( *pHH == DBL_MAX || *pHH >= *pSH || *pHH <= *pSL )
		TestHH = false;
	if ( *pLL == DBL_MAX || *pLL <= *pSL || *pLL >= *pSH )
		TestLL = false;
  //HH
  if ( TestHH )
  {
    if ( *pPV > *pHH )
    {
      if ( AF == NULL || ( *AF & 0x80000 ) == 0 )
        SEND_ON( A_HH, *pHH );
    }
  }
  //LL
  if ( TestLL )
  {
    if ( *pPV < *pLL )
    {
      if ( AF == NULL || ( *AF & 0x40000 ) == 0 )
        SEND_ON( A_LL, *pLL );
    }
  }
}

inline void CY_ALARM::HHLL_OFF( )
{
  if( pCALI && *pCALI )
    return;
  bool TestH = (*pHHLL & 1) || !( AF && (*AF & 0x00080000));
  bool TestL = (*pHHLL >= 2) || !( AF && (*AF & 0x00040000));
	if ( *pHH >= *pSH	)
  {
    TestH = false;
    if( State & A_HH )
      SEND_OFF( A_HH );
  }
	if ( *pLL <= *pSL	)
  {
    TestL = false;
    if( State & A_LL )
        SEND_OFF( A_LL );
  }
  if ( State & A_HH )
    {
    if ( AF && ( *AF & 0x80000 ) != 0 )
      SEND_OFF( A_HH );
    }
  if ( TestH )
    {
    if ( *pPV <= *pHH )
      {
      if ( State & A_HH )
        {
        double HYS; 
          if ( *pHYS_HL > 0. )
            HYS = *pHYS_HL * ( *pSH - *pSL );
          else
            HYS = -*pHYS_HL;
        if ( *pPV < *pHH - HYS )
          SEND_OFF( A_HH );
        }
      }
    }
// LL
  if ( State & A_LL )
  {
    if ( AF && ( *AF & 0x40000 ) != 0 )
      SEND_OFF( A_LL );
  }
  if ( TestL )
  {
    if ( *pPV >= *pLL )
    {
      if ( State & A_LL )
      {
        double HYS; 
        if ( *pHYS_HL > 0. )
          HYS = *pHYS_HL * ( *pSH - *pSL );
        else
          HYS = -*pHYS_HL;
        if ( *pPV > *pLL + HYS )
          SEND_OFF( A_LL );
      }
    }
  }
 }

inline void CY_ALARM::HL_ON( )
{
  if( pCALI && *pCALI )
    return;
  bool TestHH = (*pHHLL & 1) || !( AF && (*AF & 0x00080000));
  bool TestLL = (*pHHLL >= 2) || !( AF && (*AF & 0x00040000));
  // H
  bool TestH = (*pHL & 1) || !( AF && (*AF & 0x00008000));
  bool TestL = (*pHL >= 2) || !( AF && (*AF & 0x00004000));
	if ( *pH == DBL_MAX || *pH >= *pSH || *pH <= *pSL	)
		TestH = false;
  if( TestHH && TestH && *pH >= *pHH )
    TestH = false;
	if ( *pL == DBL_MAX || *pL <= *pSL || *pL >= *pSH	)
		TestL = false;
  if( TestLL && TestL && *pL <= *pLL )
    TestL = false;
  if ( TestH )
    {
    if ( *pPV > *pH )
      {
      SEND_ON( A_HI, *pH );
      }
    }
// L
  if ( TestL )
    {
    if ( *pPV < *pL )
      {
      SEND_ON( A_LO, *pL );
      }
    }
  }

inline void CY_ALARM::HL_OFF( )
{
  if( pCALI && *pCALI )
    return;
// H
  bool TestH = (*pHL & 1) || !( AF && (*AF & 0x00008000));
  bool TestL = (*pHL >= 2) || !( AF && (*AF & 0x00004000));
	if ( *pH == DBL_MAX	)
		TestH = false;
	if ( *pL == DBL_MAX	)
		TestL = false;
  if ( TestH )
    {
    if ( *pPV <= *pH )
      {
      if ( State & A_HI )
        {
        double HYS; 
        if ( *pHYS_HL > 0. )
          HYS = *pHYS_HL * ( *pSH - *pSL );
        else
          HYS = -*pHYS_HL;
        if ( *pPV < *pH - HYS )
          SEND_OFF( A_HI );
        }
      }
    }
// L
  if ( TestL )
  {
    if ( *pPV >= *pL )
    {
      if ( State & A_LO )
      {
        double HYS; 
        if ( *pHYS_HL > 0. )
          HYS = *pHYS_HL * ( *pSH - *pSL );
        else
          HYS = -*pHYS_HL;
        if ( *pPV > *pL + HYS )
          SEND_OFF( A_LO );
      }
    }
  }
}

inline void CY_ALARM::VEL( double dt )
  {
    if( pCALI && *pCALI )
      return;
  if ( V_Time > 0. )
    {
    V_Time -= dt;
    }
  else
    {
    double PV = *pPV;
    if ( PVlast == DBL_MAX )
      PVlast = PV;
    double dPV = PV - PVlast;
    PVlast = PV;
    V_Time = 0.;//??*pdT_V_scan;
//
    double VL = *pVL;
    bool TestP, TestM;
    if ( *pSides_VL == 2 )
      {
      VL = fabs ( VL );
      TestP = TestM = true;
      }
    else
      {
      if ( VL > 0. )
        {
        TestP = true;
        TestM = false;
        }
      else
        {
        VL = -VL;
        TestP = false;
        TestM = true;
        }
      }
    if ( TestP )
      {
      if ( dPV > VL )
        {
        SEND_ON( A_VELP, VL );
        }
      else
        {
        if ( State & A_VELP )
          {
					double HYS; 
					if ( *pHYS_VL > 0. )
						HYS = *pHYS_VL * ( *pSH - *pSL );
					else
						HYS = -*pHYS_VL;
          if ( dPV < VL - HYS )
            SEND_OFF( A_VELP );
          }
        }
      }
    if ( TestM )
      {
      dPV = -dPV;
      if ( dPV > VL )
        {
        SEND_ON( A_VELM, VL );
        }
      else
        {
        if ( State & A_VELM )
          {
					double HYS; 
					if ( *pHYS_VL > 0. )
						HYS = *pHYS_VL * ( *pSH - *pSL );
					else
						HYS = -*pHYS_VL;
          if ( dPV < VL - HYS )
            SEND_OFF( A_VELM );
          }
        }
      }
    }
  }

inline void CY_ALARM::DEV( double dt )
  {
    if( pCALI && *pCALI )
      return;
//---
//    ( dt + T * D ) r  = K * T * D * SV
//    ( dt + T ) r - T * _r = K * T * ( SV - _SV )
//    r = T / ( dt + T ) * ( _r + K * ( SV - _SV ));
	if ( pTime_DL )
		{
		if ( SVlast == DBL_MAX )
			{
			SVlast = *pSV; 
			}
		r = *pTime_DL / ( dt + *pTime_DL ) * ( r + *pGAIN_DL * ( *pSV - SVlast ));
		SVlast = *pSV;
		}
	else
		r = 0.;
//
  double DL = *pDL;
  double DLe = fabs ( DL ) + fabs ( r );
  double DV = *pPV - *pSV;
  bool TestP, TestM;
  if ( *pDeviation_DL == 2 )
    {
    TestP = TestM = true;
    }
  else
    {
    if ( DL > 0. )
      {
      TestP = true;
      TestM = false;
      }
    else
      {
      TestP = false;
      TestM = true;
      }
    }
  if ( TestP )
    {
    if ( DV > DLe )
      {
      SEND_ON( A_DVP, DLe );
      }
    else
      {
      if ( State & A_DVP )
        {
				double HYS; 
				if ( *pHYS_DL > 0. )
					HYS = *pHYS_DL * ( *pSH - *pSL ) * 0.01;
				else
					HYS = -*pHYS_DL;
        if ( DV < DLe - HYS )
          SEND_OFF( A_DVP );
        }
      }
    }
  if ( TestM )
    {
    DV = -DV;
    if ( DV > DLe )
      {
      SEND_ON( A_DVM, DLe );
      }
    else
      {
      if ( State & A_DVM )
        {
				double HYS; 
				if ( *pHYS_DL > 0. )
					HYS = *pHYS_DL * ( *pSH - *pSL ) * 0.01;
				else
					HYS = -*pHYS_DL;
        if ( DV < DLe - HYS )
          SEND_OFF( A_DVM );
        }
      }
    }
  }


void CY_ALARM::TestIn( CSignal * IN, double dt, int & Status )
{
  Status &= ~CSignal::SIGNALS_MASK;
  bool shrinkHL = true;
	if ( IN )
    {
    IN ->Status &= ~CSignal::SIGNALS_MASK;
    double PV = *pPV;
    if( pCALI && *pCALI )
    {}
    else
      *IN >> PV;
    *pPV = PV * GAINin + BIASin;
    if( IN->Status & CSignal::NONE_LIMITED )
      shrinkHL = false;
    }
	else
		Status &= ~CSignal::SIGNALS_MASK;

  if( pSH && pSL && *pSH > *pSL)
  {
    if( 1==KGlobalSettings::Instance().PV_LIMIT() )
    {
      if ( *pPV > *pSH )
        *pPV = *pSH;
      if ( *pPV < *pSL )
        *pPV = *pSL;
    }
    else
    {
      double l = (*pSH - *pSL) * 0.25;
      if ( *pPV > (*pSH+l) )
      {
        double s = (*pSH - *pSL);
        if( ((*pPV-*pSH)/10.) < s )
          *pPV = *pSH+l;
      }
      if ( *pPV < (*pSL-l) )
      {
        *pPV = *pSL-l;
      }
    }
  }

	if( shrinkHL && pSH && pSL && *pSH > *pSL )
	{
    double SH = *pSH + (*pSH - *pSL ) * ((*pH_Limit)/100.);
    double SL = *pSL + (*pSH - *pSL ) * ((*pL_Limit)/100.);
		double D = ( *pSH - *pSL ) * 0.05;
    if( pHYS_HL )
      D = ( *pSH - *pSL ) * ((*pHYS_HL) / 100.);
		if ( *pPV > SH + D )
      Status |= CSignal::IOPp;
		if ( *pPV < SL - D )
			Status |= CSignal::IOPm;
	}
// Input Error Alarm
  if ( Status & CSignal::BAD && ( Status & CSignal::IOPm ) == 0 )
    Status |= CSignal::IOPp;

  if( IN )
  {
    if( Status & CSignal::IOPp )
      IN->On( CSignal::IOPp );
    else
      IN->Off( CSignal::IOPp );
    if( Status & CSignal::IOPm )
      IN->On( CSignal::IOPm );
    else
      IN->Off( CSignal::IOPm );
  }

  if( pCALI && *pCALI )
  {
    SEND_OFF( A_HH );
    SEND_OFF( A_LL );
    SEND_OFF( A_HI );
    SEND_OFF( A_LO );
    SEND_OFF( A_VELP );
    SEND_OFF( A_VELM );
    SEND_OFF( A_DVP );
    SEND_OFF( A_DVM );
    return;
  }
  if (( Possible & A_HL ) && (( pHL && *pHL > 0) || !( AF && (*AF & 0x00008000)) || !( AF && (*AF & 0x00004000)) ) )
    {
    HL_ON( );
    }
  if ( ( Possible & A_HHLL ) && (( pHHLL && *pHHLL > 0) || !( AF && (*AF & 0x00080000)) || !( AF && (*AF & 0x00040000)) ) )
    {
  // HH
    HHLL_ON( );
    HHLL_OFF( );
    }
  if (( Possible & A_HL ) && (( pHL && *pHL > 0) || !( AF && (*AF & 0x00008000)) || !( AF && (*AF & 0x00004000)) ) )
    {
    HL_OFF( );
    }
  // Input Open Alarm
  if ( ( Possible & A_IOP ) && pInput_Open_alarm && *pInput_Open_alarm > 0 )
  {
    if ( !Input_Open_Alarm( Status ))
      return ;
  }
 // Скорость
  if (( Possible & A_VEL ) && pSides_VL && *pSides_VL > 0 && pVL && *pVL != 0. )
    {
    VEL( dt );
    }
// Deviation Alarm
  if (( Possible & A_DV ) && pDeviation_DL && *pDeviation_DL > 0 && pDL && *pDL != 0. )
    {
    DEV( dt );
    }
}

void CY_ALARM::TestInScaled( CSignal * IN, double dt, int & Status, double A )
{
  if( A==0 )
    return TestIn( IN, dt, Status );
  Status &= ~CSignal::SIGNALS_MASK;
  bool shrinkHL = true;
  if ( IN )
  {
    IN ->Status &= ~CSignal::SIGNALS_MASK;
    double PV = *pPV;
    if( pCALI && *pCALI ){}
    else
      *IN >> PV;
    PV *= A;
    *pPV = PV * GAINin + BIASin;
    if( IN->Status & CSignal::NONE_LIMITED )
      shrinkHL = false;
  }
  else
    Status &= ~CSignal::SIGNALS_MASK;
  if( shrinkHL && *pSH > *pSL )
  {
    double SH = *pSH + (*pSH - *pSL ) * ((*pH_Limit)/100.);
    double SL = *pSL + (*pSH - *pSL ) * ((*pL_Limit)/100.);
    double D = ( *pSH - *pSL ) * 0.05;
    if( pHYS_HL )
      D = ( *pSH - *pSL ) * ((*pHYS_HL) / 100.);
    if ( *pPV > SH + D )
      Status |= CSignal::IOPp;
    if ( *pPV < SL - D )
      Status |= CSignal::IOPm;
    /*if ( *pPV > SH )
      *pPV = SH;
    if ( *pPV < SL )
      *pPV = SL;*/
  }
  // Input Error Alarm
  if ( Status & CSignal::BAD && ( Status & CSignal::IOPm ) == 0 )
    Status |= CSignal::IOPp;

  // Input Open Alarm
  if ( ( Possible & A_IOP ) && pInput_Open_alarm && *pInput_Open_alarm > 0 )
  {
    if ( !Input_Open_Alarm( Status ))
      return ;
  }
  if (( Possible & A_HL ) && pHL && *pHL > 0 )
  {
    HL_ON( );
  }
  if ( ( Possible & A_HHLL ) && pHHLL && *pHHLL > 0 )
  {
    // HH
    HHLL_ON( );
    HHLL_OFF( );
  }
  if (( Possible & A_HL ) && pHL && *pHL > 0 )
  {
    HL_OFF( );
  }
  // Скорость
  if (( Possible & A_VEL ) && pSides_VL && *pSides_VL > 0 && pVL && *pVL != 0. )
  {
    VEL( dt );
  }
  // Deviation Alarm
  if (( Possible & A_DV ) && pDeviation_DL && *pDeviation_DL > 0 && pDL && *pDL != 0. )
  {
    DEV( dt );
  }
}

void CY_ALARM::TestOut( )
{
//Output Open Alarm
// Пока пропускаем
//Output Fail Alarm
// Пока пропускаем
//Output High and Low Limit Alarm
  if ( !pMV || !pMH || !pML || !pMSL || !pMSH || !pHL_OUT )
    return;
  if( *pMSH < *pMSL || *pMH < *pML )
    return;
  if( Possible&A_MHI && *pMH < *pMSH && ( *pHL_OUT & 1 )) 
    {
    if ( *pMV > *pMH )
      {
      SEND_ON( A_MHI, *pMH );
      }
    else
      {
      if ( State & A_MHI )
        {
        if ( *pMV < *pMH - *pHYS_OUT )
          SEND_OFF( A_MHI );
        }
      }
    }
  if ( Possible&A_MLO && *pML > *pMSL && ( *pHL_OUT >= 2 )) 
    {
    if ( *pMV < *pML )
      {
      SEND_ON( A_MLO, *pML );
      }
    else
      {
      if ( State & A_MLO )
        {
        if ( *pMV > *pML - *pHYS_OUT )
          SEND_OFF( A_MLO );
        }
      }
    }
}

void CY_ALARM::AlarmPERR( bool Is )
	{
	if ( Is )
		SEND_ON( A_PERR, -1. );
	else
		SEND_OFF( A_PERR );
	}

typedef struct tagIdNames
{
  int id;
  LPCSTR lpszName;
  __int64 mask;
  int yokoCode;
}SIdNames;

static const SIdNames slistAlarms[] =
{
#undef  EnumMESS
#define EnumMESS( Name, Number, Text, CodeYoko ) { Number, Text, __int64(1)<<Number, CodeYoko },
#include <AlarmMess_.hpp>
};

void CY_ALARM_BASE::CreateTextAlrm()
{
  if( !pBlk )
    return;
  if( 0==pBlk->ALRMmask )
  {
    strcpy_s( pBlk->szALRM, "NR" );
    return;
  }
  pBlk->szALRM[0] = 0;
  __int64 M = 1LL << pBlk->ALRM;
  for( int n = 0; n < _countof(slistAlarms); ++n )
  {
    const SIdNames &id = slistAlarms[n];
    if( id.mask != (M & id.mask) )
      continue;
    strcat_s( pBlk->szALRM, id.lpszName );
    break;
  }
}
