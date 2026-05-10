#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
	enum eType { SI_1, SI_1E, SI_2, SI_2E,
		           SO_1, SO_1E, SO_2, SO_2E,
							 SIO_11, SIO_11E, SIO_12, SIO_12E,
							 SIO_21, SIO_21E, SIO_22, SIO_22E,
							 SIO_12P, SIO_12PE, SIO_22P, SIO_22PE,
               SI_1ALM };

static CY_Create oSI_1( "SI-1", CY_SIO_X::Create, SI_1 ); 
static CY_Create oSI_1E( "SI-1E", CY_SIO_X::Create, SI_1E ); 
static CY_Create oSI_2( "SI-2", CY_SIO_X::Create, SI_2 ); 
static CY_Create oSI_2E( "SI-2E", CY_SIO_X::Create, SI_2E );
static CY_Create oSO_1( "SO-1", CY_SIO_X::Create, SO_1 ); 
static CY_Create oSO_1E( "SO-1E", CY_SIO_X::Create, SO_1E ); 
static CY_Create oSO_2( "SO-2", CY_SIO_X::Create, SO_2 ); 
static CY_Create oSO_2E( "SO-2E", CY_SIO_X::Create, SO_2E );
static CY_Create oSIO_11( "SIO-11", CY_SIO_X::Create, SIO_11 ); 
static CY_Create oSIO_11E( "SIO-11E", CY_SIO_X::Create, SIO_11E ); 
static CY_Create oSIO_12( "SIO-12", CY_SIO_X::Create, SIO_12 ); 
static CY_Create oSIO_12E( "SIO-12E", CY_SIO_X::Create, SIO_12E );
static CY_Create oSIO_21( "SIO-21", CY_SIO_X::Create, SIO_21 ); 
static CY_Create oSIO_21E( "SIO-21E", CY_SIO_X::Create, SIO_21E ); 
static CY_Create oSIO_22( "SIO-22", CY_SIO_X::Create, SIO_22 ); 
static CY_Create oSIO_22E( "SIO-22E", CY_SIO_X::Create, SIO_22E );
static CY_Create oSIO_12P( "SIO-12P", CY_SIO_X::Create, SIO_12P ); 
static CY_Create oSIO_12PE( "SIO-12PE", CY_SIO_X::Create, SIO_12PE ); 
static CY_Create oSIO_22P( "SIO-22P", CY_SIO_X::Create, SIO_22P ); 
static CY_Create oSIO_22PE( "SIO-22PE", CY_SIO_X::Create, SIO_22PE );
static CY_Create oSI_1ALM( "SI-1ALM", CY_SIO_X::Create, SI_1ALM ); 

CY_SIO_X::CY_SIO_X( char * _Name, int _Type, int number, int domain ) : CY_Base()
{
  Type = (eType)_Type;
//	INIT(PV) // Answerback input value (*1) 0 to 2 0
//	INIT(SIMM) // Simulation switch 0, 1 0
	IN = NULL;
	IN1 = NULL;
	IN2 = NULL;
	SWI = NULL;
	INT = NULL;
	TSI = NULL;
	kOut = 0;
	SET_OUT("OUT",OUT1)
	SET_OUT("OUT1",OUT1)
	SET_OUT("OUT2",OUT2)
  //
	PV_0 = 0;
	MV_0 = 0;
	PV_2 = 0;
	K_AP = 0;//Number of answerback points
	K_OUT = 0;//Number of output points
	mtmHolder = 0.;
	MV_old = -1;
	OUT1 = 0;
	OUT2 = 0;	 // Сигналы на выходах
	AllOK = true;
  PV_MV = false;
	PAZ = false;
  pPV_PAZ_IN = NULL;
  Start = false;
  bNextOut = bNextIn = false;
  W = NULL;
	switch ( Type )
		{
  case SI_1E:
    {
      CSI_1E * P = (CSI_1E*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSI_1E::TypeID, number, domain );
      W = P;
    }
    break;
		case SI_2E:
			{
      CSI_2E * P = (CSI_2E*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSI_2E::TypeID, number, domain );
      W = P;
			}
		break;
		case SO_2E:
			{
      CSO_2E * P = (CSO_2E*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSO_2E::TypeID, number, domain );
      W = P;
			}
		break;
		case SO_1:
			{
      CSO_1 * P = (CSO_1*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSO_1::TypeID, number, domain );
      W = P;
			}
		break;
		case SIO_11:
			{
      CSIO_11 * P = (CSIO_11*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_11::TypeID, number, domain );
      W = P;
			}
		break;
		case SIO_12:
			{
      CSIO_12 * P = (CSIO_12*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_12::TypeID, number, domain );
      W = P;
			}
		break;
		case SIO_22:
			{
      CSIO_22 * P = (CSIO_22*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_22::TypeID, number, domain );
      W = P;
			}
		break;
		case SO_1E:
			{
      CSO_1E * P = (CSO_1E*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSO_1E::TypeID, number, domain );
      W = P;
			}
		break;
		case SIO_11E:
			{
      CSIO_11E * P = (CSIO_11E*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_11E::TypeID, number, domain );
      W = P;
			}
		break;
		case SIO_12E:
			{
      CSIO_12E * P = (CSIO_12E*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_12E::TypeID, number, domain );
      W = P;
			}
		break;
		case SIO_12P:
			{
      CSIO_12P * P = (CSIO_12P*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_12P::TypeID, number, domain );
      W = P;
			}
		break;
    case SIO_12PE:
      {
        CSIO_12PE * P = (CSIO_12PE*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_12PE::TypeID, number, domain );
        W = P;
      }
      break;
		case SIO_21E:
			{
      CSIO_21E * P = (CSIO_21E*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_21E::TypeID, number, domain );
      W = P;
			}
		break;
		case SIO_21:
			{
      CSIO_21 * P = (CSIO_21*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_21::TypeID, number, domain );
      W = P;
			}
		break;
		case SIO_22E:
			{
      CSIO_22E * P = (CSIO_22E*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_22E::TypeID, number, domain );
      W = P;
			}
		break;
		case SIO_22P:
			{
      CSIO_22P * P = (CSIO_22P*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_22P::TypeID, number, domain );
      W = P;
			}
		break;
		case SIO_22PE:
			{
      CSIO_22PE * P = (CSIO_22PE*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSIO_22PE::TypeID, number, domain );
      W = P;
			}
		break;
		case SI_1:
			{
      CSI_1 * P = (CSI_1*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSI_1::TypeID, number, domain );
      W = P;
			}
		break;
    case SI_1ALM:
      {
        CSI_1ALM * P = (CSI_1ALM*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSI_1ALM::TypeID, number, domain );
        W = P;
      }
      break;
		case SI_2:
			{
      CSI_2 * P = (CSI_2*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSI_2::TypeID, number, domain );
      W = P;
			}
		break;
		case SO_2:
			{
      CSO_2 * P = (CSO_2*)KMemoryServiceY::Instance().CreateObjectsStruct( _Name, CSO_2::TypeID, number, domain );
      W = P;
			}
		break;
		default:
			ASSD(FALSE)
		break;
		}
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pMode = &W->mode;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pAlarmLevel = &W->AlarmLevel;
  INFO(W->PV,"PV")
  INFO(W->MV,"MV")
  CY_Base::pComm = W->Comment;
}

void CY_SIO_X::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_SIO_X,CSIO_COMM,31,51)

void CY_SIO_X::InitParm()
  {
#include "Yokogawa\SIO_P.hpp" 
#include "Yokogawa\SIO.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_SIO_X::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_SIO_X::GetIN ( char * Name )
	{
	GET_IN(IN)
	GET_IN(IN1)
	GET_IN(IN2)
	GET_IN(SWI)
	GET_IN(INT)
	GET_IN(TSI)
//
	return NULL;
	}

void CY_SIO_X::Answerback_Input()
	{
	bool IsPERR = false;
// PV
	if ( K_AP == 1 )
	{
// 1 вход
		int S;
		if ( IN->Type == CSignal::NoType )
			S = 0;
		else
			*IN >> S;
		if( W->Answer_Direction <= 0 )// DIRECT
		{
			if( S )
				W->PV = 2;
			else
				W->PV = 0;
		}
		else if( 1==W->Answer_Direction )// REVERSE
		{
			if ( S )
				W->PV = 0;
			else
				W->PV = 2;
		}
	}
	if( K_AP == 2 )
	{
// {DIRECT;REVERSE;Inverted connection direct;Inverted connection direct??};
// 2 входа
		int S1 = -1, S2 = -1;
    if( IN1 )
		  *IN1 >> S1;
    if( IN2 )
		  *IN2 >> S2;
		if( W->Answer_Direction <= 0 )// DIRECT
		{
			if( S1>0 && !S2 )
				W->PV = 2;
			else
			{
				if ( !S1 && !S2 )
					W->PV = 1;
				else
				{
					if ( !S1 && S2>0 )
						W->PV = 0;
					else
					{
						IsPERR = true;
//Alarm PERR
					}
				}
			}
		}
		else if( 1==W->Answer_Direction )// REVERSE
		{
			if( S1>0 && !S2 )
				W->PV = 0;
			else
			{
				if ( !S1 && S2>0 )
					W->PV = 2;
				else
				{
					if ( S1>0 && S2>0 )
						W->PV = 1;
					else
					{
						IsPERR = true;
//Alarm PERR
					}
				}
			}
		}
    else if( 2==W->Answer_Direction )// INVERTDIR
    {
      if( !S1 && S2>0 )
        W->PV = 2;
      else
      {
        if ( !S1 && !S2 )
          W->PV = 1;
        else
        {
          if ( S1>0 && !S2 )
            W->PV = 0;
          else
          {
            IsPERR = true;
            //Alarm PERR
          }
        }
      }
    }
    else if( 2==W->Answer_Direction )// INVERTREV
    {
      if( !S1 && S2>0 )
        W->PV = 0;
      else
      {
        if ( S1>0 && !S2 )
          W->PV = 2;
        else
        {
          if ( S1>0 && S2>0 )
            W->PV = 1;
          else
          {
            IsPERR = true;
            //Alarm PERR
          }
        }
      }
    }

	}
	if ( W->Inhibit_PERR > 0 )
		Alarm.AlarmPERR( IsPERR );
	}

void CY_SIO_X::AnswerbackCheck( double dt )
	{
	double T = mtmHolder + dt;
	bool OverTime = false;
	if ( T > W->MTM )
		{
		T = W->MTM;
		OverTime = true;
		}
	mtmHolder = T;
// ANS+ MV==2 PV!= 2
// ANS- MV==0 PV!= 0
//	Ans_Abnorm_Alarm NO;ON;OFF;BOTH
// Запуск таймеров
	if ( W -> MV != MV_old )
		{
		MV_old = W -> MV;
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
					mtmHolder = 0.;
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
					mtmHolder = 0.;
					OverTime = false;
					}
				else
					{
					AllOK = true;
					return;
					}
			break;
			case 3://BOTH
				mtmHolder = 0.;
				OverTime = false;
			break;
			}
		}
	if ( Alarm.State & A_ANSP )
		{
		if ( W -> MV != 2 || W -> PV == 2 )
			{
	// NR
			W -> ANCK = false;
			Alarm.SendAlarm( A_ANSP, false );
			if ( OverTime )
				AllOK = true;
			return;
			}
		}
	if ( Alarm.State & A_ANSM )
		{
		if ( W -> MV != 0 || W -> PV == 0 )
			{
	// NR
			W -> ANCK = false;
			Alarm.SendAlarm( A_ANSM, false );
			if ( OverTime )
				AllOK = true;
			return;
			}
		}
	AllOK = false;
// Проверка PV
	switch ( W->Answerback_Check )
		{
		case 0://NO
		break;
		case 1://ON
			if ( OverTime )
				{
				W -> ANCK = false;
        AllOK = true;
				if ( W->MV == 2 && W->PV != 2 )
					{
					if ( W->Ans_Abnorm_Alarm & 1 && !(Alarm.State & A_ANSP) ) 
            {
              Alarm.SendAlarm( A_ANSP, true );
            }
					}
				}
			else
				{
				W -> ANCK = true;
				}
		break;
		case 2://OFF
			if ( OverTime )
				{
        AllOK = true;
				if ( W->MV == 0 && W->PV != 0 )
					{
					if ( W->Ans_Abnorm_Alarm & 2 && !(Alarm.State & A_ANSM) ) 
            {
              Alarm.SendAlarm( A_ANSM, true );
            }
					}
				}
			else
				{
				W -> ANCK = true;
				}
		break;
		case 3://BOTH
			if ( OverTime )
				{
				W -> ANCK = false;
        AllOK = true;
				if ( W -> MV == 2 )
					{
            if ( W -> PV != 2 )
            {
              if ( W->Ans_Abnorm_Alarm & 1 && !(Alarm.State & A_ANSP) ) 
              {
                Alarm.SendAlarm( A_ANSP, true );
              }
            }
					}
				else
					{
            if ( W -> PV != 0 )
            {
              if ( W->Ans_Abnorm_Alarm & 2 && !(Alarm.State & A_ANSM) ) 
              {
                Alarm.SendAlarm( A_ANSM, true );
              }
            }
					}
				}
			else
				{
				W -> ANCK = true;
				}
		break;
		}
	}

void CY_SIO_X::OutPut( )
	{
//	Out_Action_Direct
	
	if( K_OUT == 1 )
	{
		if( 1==W->Out_Action_Direct )//DIRECT
		{
			if ( W->MV == 2 )
				OUT1 = 1;
			else
				OUT1 = 0;
		}
		else if( 0==W->Out_Action_Direct )//REVERSE
		{
			if ( W->MV == 2 )
				OUT1 = 0;
			else
				OUT1 = 1;
		}
    else if( 2==W->Out_Action_Direct )//INVERTDIR
    {
      ASSD(0);
    }
    else if( 3==W->Out_Action_Direct )//INVERTREV
    {
      ASSD(0);
    }
	}
	if( K_OUT == 2 )
	{
		if( 1==W->Out_Action_Direct )//DIRECT
		{
			if( 2==W->MV )
			{
				OUT1 = 1;
				OUT2 = 0;
			}
			else if( 1==W->MV )
			{
				OUT1 = 0;
				OUT2 = 0;
			}
			else if( 0==W->MV )
			{
				OUT1 = 0;
				OUT2 = 1;
			}
		}
		else if( 0==W->Out_Action_Direct )//REVERSE
		{
			if( 2==W->MV )
			{
				OUT1 = 0;
				OUT2 = 1;
			}
			else if ( 1==W->MV )
			{
				OUT1 = 1;
				OUT2 = 1;
			}
			else if ( 0==W->MV )
			{
				OUT1 = 1;
				OUT2 = 0;
			}
		}
    else if( 2==W->Out_Action_Direct )//INVERTDIR
    {
      if( 2==W->MV )
      {
        OUT1 = 0;
        OUT2 = 1;
      }
      else if ( 1==W->MV )
      {
        OUT1 = 0;
        OUT2 = 0;
      }
      else if ( 0==W->MV )
      {
        OUT1 = 1;
        OUT2 = 0;
      }
    }
    else if( 3==W->Out_Action_Direct )//INVERTREV
    {
      if( 2==W->MV )
      {
        OUT1 = 1;
        OUT2 = 0;
      }
      else if ( 1==W->MV )
      {
        OUT1 = 1;
        OUT2 = 1;
      }
      else if ( 0==W->MV )
      {
        OUT1 = 0;
        OUT2 = 1;
      }
    }
	}
}

FILE * fopen_err ( char * File, char * Mode );
static int PV_USE_SOLENOID = 1;

void CY_SIO_X::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}

  if( !CY_Base::s_flagBasicScanControl )
    return;

  double deltaT = CY_Base::s_ControlPeriod * m_nBasicControlPeriodCounter;
  m_nBasicControlPeriodCounter = 0;

//  W->mode.M = SMode::MAN;
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    Out[0].Status |= CSignal::BAD;
		return ;
    }
  else
    Out[0].Status &= ~CSignal::BAD;

  SMode::tMode Mode = W->mode.M;
	if ( Mode <= SMode::PRD )
		Mode = W->mode.M = SMode::MAN;
	if ( Mode == SMode::AUT || Mode == SMode::CAS )
		W->MV = W->CSV;
  else
		W->CSV = W->MV;

  if( W->MV )
  {
    switch( Type )
    {
    case SIO_11:
    case SIO_21:
    case SO_1:
      W->MV = 2;
      break;
    }
  }

	if ( IN == NULL && IN1 == NULL && IN2 == NULL )
		{
		OutPut( );
		return ;
		}
	bool OOP = false;
	if ( IN1 && (IN1->Status & CSignal::OOP ))
		OOP = true;
	if ( IN2 && (IN2->Status & CSignal::OOP ))
		OOP = true;
	if ( OOP )
		Alarm.SEND_ON( A_OOP, -1. );
	else
		Alarm.SEND_OFF( A_OOP );
//
	if ( SWI  )
		*SWI >> W->BPSW;
	if ( W->BPSW == 0 )
		Answerback_Input();

	if( TSI )
		*TSI >> W->TSW;

  if( W->TSW )
    W->mode.TRK = true;
  else
    W->mode.TRK = false;

  // Answerback Check
	if ( MV_old < 0 )
		MV_old = W -> MV;
	/*
  //проверять всегда после истечения MTM
  if ( W ->MV != MV_old )
		AllOK = false;
	if ( !AllOK )*/
		{
		if ( W -> TSW != 1 && W->BPSW	!= 1 && W ->MV != 1 )
			{
			AnswerbackCheck( deltaT );
			}
		}
	if ( W->BPSW == 1 )
		W ->PV = W -> MV;

// Output
//	if ( W->mode.M == SMode::AUT ||	W->mode.M == SMode::CAS	)
//		W -> MV = W -> CSV;
	OutPut( );
// Output tracking	???
// answerback tracking
	if ( W->mode.TRK && W->Answerback_Trk )
		W -> MV = W ->PV;
// MV.0
  //if ( W -> MV )
  //  {
  //  Out[3].Action = -1;
  //  }
  //else
  //  {
  //  if ( Out[3].Action < 0 )
  //    Out[3].Action = 1;
  //  }
  if ( PAZ && pPV_PAZ_IN )
    {
    if ( W->MV == 2 )
      *pPV_PAZ_IN = 1;
    else
      *pPV_PAZ_IN = 0;
    }
	if ( W->PV == 0 )
		PV_0 = 1;
	else
		PV_0 = 0;
	if ( W->PV == 2 )
		PV_2 = 1;
	else
		PV_2 = 0;
	if ( W->MV == 0 )
		MV_0 = 1;
	else
		MV_0 = 0;
	}

void CY_SIO_X::Init( )
	{
	}

int CY_SIO_X::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\SIO_P.hpp" 
	return 0;
	}

bool CY_SIO_X::Double_IN( )
  {
  if( !IN )
    return false;
  CY_Base * pObj = (*pFindYObjN)( IN->pFrom, pDR->NumbFCS, pDR->domain );
  if ( strcmp ( pObj->Model, "PIO" ) == 0 )
    {
    CY_IO * pIO = (CY_IO *)pObj;
    pObj = pIO -> pDI;
    if( !pObj )
    {
      pIO->GetObj(false);
      pObj = pIO -> pDI;
    }
    }
  ASSD(pObj);
  if( !pObj )
    return false;
  CY_DI * pDI_1 = NULL;
  CY_DI * pDI_2 = NULL;
  bool bThisStation = true;
  
  if( !pObj->Y_Type && !strcmp(pObj->Model, "AREAOUT") )
  {
    CY_AREAIN *pAr = (CY_AREAIN*)pObj;
    ASSD( pAr->m_pObjX );
    pObj = pAr->m_pObjX;
    if( pObj )
      bThisStation = false;
  }

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
      ASSD( pDI_2 );
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
      ASSD( pDI_2 );
      }
    break;
    case id__AN:
      {
      pDI_1 = (CY_DI *)pObj; 
      int N = pDI_1->Numb;
      N++;
      pDI_2 = (*pFindDI)( pDI_1->NumbFCS, pDI_1->domain, N, id__AN );
      ASSD( pDI_2 );
      }
    break;
    case id__GS:
      {
        pDI_1 = (CY_DI *)pObj; 
        int N = pDI_1->Numb;
        N++;
        pDI_2 = (*pFindDI)( pDI_1->NumbFCS, pDI_1->domain, N, id__GS );
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
  IN2->pFrom = pDI_2->Name;
  bNextIn = true;
  IN = NULL;
  return true;
  }

bool CY_SIO_X::Double_OUT( )
  {
  ASSD(Out[0].pTo[0])
  CY_Base * pObj = (*pFindDObj)( Out[0].pTo[0], pDR->NumbFCS, pDR->domain );
  ASSD(pObj);
  if( !pObj )
    return false;
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
      pDI_2 = (*pFindDI)( pDR->NumbFCS, pDR->domain, N, id__WB );
      ASSD( pDI_2 );
      }
    break;
    case id__Z:
      {
      pDI_1 = (CY_DI *)pObj; 
      int N = pDI_1->Numb;
      N++;
      pDI_2 = (*pFindDI)( pDR->NumbFCS, pDR->domain, N, id__Z );
      ASSD( pDI_2 );
      }
    break;
    case id__Y:
      {
        pDI_1 = (CY_DI *)pObj; 
        int N = pDI_1->Numb;
        N++;
        pDI_2 = (*pFindDI)( pDR->NumbFCS, pDR->domain, N, id__Y );
        ASSD( pDI_2 );
      }
      break;
    case id__SW:
      {
      pDI_1 = (CY_DI *)pObj; 
      int N = pDI_1->Numb;
      N += 1;
      pDI_2 = (*pFindDI)( pDR->NumbFCS, pDR->domain, N, id__SW );
      ASSD( pDI_2 );
      }
    break;
    default:
//??      ASSD(FALSE)
    return false;
    }
  if ( pDI_1 == NULL || pDI_2 == NULL )
    return false;
  CY_Base * pObj_1 = (*pFindYObjN)( pDI_1->Name, pDR->NumbFCS, pDR->domain );
  CY_Base * pObj_2 = (*pFindYObjN)( pDI_2->Name, pDR->NumbFCS, pDR->domain );
  ASSD( pObj_1 && pObj_2 );
  CSignal ** IN1 = pObj_1->GetIN ( "IN" );
  CSignal ** IN2 = pObj_2->GetIN ( "IN" );
  *IN1 = &Out[1];
  *IN2 = &Out[2];
  Out[0].pTo[0] = NULL;
  Out[1].pTo[0] = pDI_1->Name;
  Out[2].pTo[0] = pDI_2->Name;
  bNextOut = true;
  
  if( !strncmp(pObj_2->Model,"PIO",3) )
  {
    pObj_2->sequenceNumberInDR = -200;
  }

  return true;
  }

int CY_SIO_X::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::MAN;
  Start = true;
	switch ( Type )
		{
		case SI_1:
			K_AP = 1;
			K_OUT = 0;
		break;
    case SI_1ALM:
      K_AP = 1;
      K_OUT = 0;
      break;
		case SI_1E: 
			K_AP = 1;
			K_OUT = 0;
		break;
		case SI_2: 
			K_AP = 2;
			K_OUT = 0;
		break;
		case SI_2E:
			K_AP = 2;
			K_OUT = 0;
		break;
		case SO_1:
			K_AP = 0;
			K_OUT = 1;
		break;
		case SO_1E: 
			K_AP = 0;
			K_OUT = 1;
		break;
		case SO_2: 
			K_AP = 0;
			K_OUT = 2;
		break;
		case SO_2E:
			K_AP = 0;
			K_OUT = 2;
		break;
		case SIO_11: 
			K_AP = 1;
			K_OUT = 1;
		break;
		case SIO_11E: 
			K_AP = 1;
			K_OUT = 1;
		break;
		case SIO_12: 
			K_AP = 1;
			K_OUT = 2;
		break;
		case SIO_12E:
			K_AP = 1;
			K_OUT = 2;
		break;
		case SIO_21: 
			K_AP = 2;
			K_OUT = 1;
		break;
		case SIO_21E: 
			K_OUT = 1;
			K_AP = 2;
		break;
		case SIO_22: 
			K_AP = 2;
			K_OUT = 2;
		break;
		case SIO_22E:
			K_AP = 2;
			K_OUT = 2;
		break;
		case SIO_12P: 
			K_AP = 1;
			K_OUT = 2;
		break;
		case SIO_12PE: 
			K_AP = 1;
			K_OUT = 2;
		break;
		case SIO_22P: 
			K_AP = 2;
			K_OUT = 2;
		break;
		case SIO_22PE:
			K_AP = 2;
			K_OUT = 2;
		break;
		};
  if ( K_AP == 2 )
    {
    if ( IN1 == NULL || IN2 == NULL )
      {
      //ASS ( IN );
      if( !IN && IN1 )
        IN = IN1;
      bool b = Double_IN( );
      if( !b )
      {
        OutputDebugString( "Не сформировался Double_IN для точки \"");
        OutputDebugString( Name );
        OutputDebugString( "\" типа ");
        OutputDebugString( Model );
        OutputDebugString( "\n" );
      }
      KKK();
      }
    }
  if ( K_OUT == 2 )
    {
    if ( Out[0].pTo[0] != NULL && Out[1].pTo[0] == NULL )
      {
      if ( !Double_OUT())
        K_OUT = 1;
      }
    }
#include "SIO_X_def.h"

  if ( PAZ )
    {
		bool IsOut = false;
		if ( Out[0].pTo[0] )
			IsOut = true;
		if ( Out[1].pTo[0] && Out[2].pTo[0] )
			IsOut = true;
    if ( IN == NULL || !IsOut )
      {
      PAZ = false;
      return 0;
      }
    if ( K_AP != 1 ||	K_OUT != 2 )
      {
      PAZ = false;
      return 0;
      }
    //SET_PAZ_IN
    //TEST_PAZ_OUT(1)
    //TEST_PAZ_OUT(2)
    }
	return 0;
}

int CY_SIO_X::SaveState( )
{
	SAVE_OUT
	SAVE_CLASS("W",CY_SIO_X_W)
	return 0;
}

int CY_SIO_X::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	REST_OUT
	REST_CLASS("W",CY_SIO_X_W)
	return 1;
}

int CY_SIO_X::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 4 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "OUT1", OUT1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "OUT2", OUT2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "mtmHolder", mtmHolder );
  WriteFullStruct( pStateSer, eKeyAnyVar, "MV_old", MV_old );

  return 0;
}

int CY_SIO_X::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 4 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "OUT1", OUT1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "OUT2", OUT2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "mtmHolder", mtmHolder );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "MV_old", MV_old );

  return 0;
}

int CY_SIO_X::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODEFULL( W->mode, "#Mode" );
	PARM ( W->Comment, "#Comment" );
	PARM ( W->PV, "#PV" );
	PARM ( W->MV, "#MV" );
  PARM ( PV_MV, "#PV->MV" )
  PARM ( OUT1, "#OUT1" )
  PARM ( OUT2, "#OUT2" )
	if ( pDR && pDR->FCS )
		PARM_CALLBACK( pDR->FCS, "#FCS" );
  PARM ( K_AP, "#Число входов" )
  PARM ( K_OUT, "#Число выходов" )
	PARM ( W->Out_Action_Direct, "#Out Action Direct(0-REV,1-DIR,InvDir,InvRev)" )
	PARM ( W->Answer_Direction, "#Answer Direction(Dir,Rev,InvDir,InvRev)" )
  if( W->MTM > 1. )
  {
    PARM ( W->MTM, "#MTM" );
    PARM ( mtmHolder, "#mtmHolder" );
  }
//	PARM ( PAZ, "ПАЗ" )
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\SIO_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\SIO.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN)
	PARM_IN(IN1)
	PARM_IN(IN2)
	PARM_IN(SWI)
	PARM_IN(INT)
	PARM_IN(TSI)
	PARM_ALL_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_SIO_X::SetLimits( )
  {
  return true;
  }

