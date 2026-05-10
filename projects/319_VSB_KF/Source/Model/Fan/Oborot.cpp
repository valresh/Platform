#include "stdafx.h"
#include "Fan.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "SysDataTypes.h"

bool CFan::ExtOboror( double dt )
  {
  if (pMotor == NULL)
    return false;
  On = *pMotor->pOn;
  Oborot = *pMotor->pRelOborot;
  //if ( On )
  //  pPump->On_State ( CPumpData::Work );
  //else
  //  pPump->Off_State ( CPumpData::Work );
  return true;
  }

void CFan::IntOboror( double dt )
  {
  if ( kStep > 5 )
    {
    int State = On ? 1 : 0;
    bool Start = false;
    bool Stop = false;
    bool Err = false;
    Пуск.CmdStart( State, LocalBlk, Start, Err );
//	  Пуск_2.CmdStart( State, LocalBlk, Start, Err );
    Пуск_Стоп.CmdStartStop( State, LocalBlk, Start, Stop, Err );
    //if ( Command > 0. )
    //  {
    //  if ( pSys->IsBlk || LocalBlk )
    //    {
    //    Start = true;
    //    }
    //  else
    //    {
    //    if ( !On )
    //      {
    //      Err = true;
    //      Set_ErrBlkInObj ( ObjName );
    //      }
    //    }
    //  }
    ErrInBlk = Err;
    if ( Start && Defect != 4 )
      On = true;
    //  pPump-> On_State ( CPumpData::Work );//Включение
    }
  //if ( pPump->Is_Ctrl ( CPumpData::Set_On ))
  //  {
  //  pPump->Off_Ctrl ( CPumpData::Set_On );
  //  if ( Defect != 4 )
  //    pPump-> On_State ( CPumpData::Work );//Включение
  //  }
  if ( kStep > 5 )
    {
    int State = On ? 1 : 0;
    bool Start = false;
    bool Stop = false;
    bool Err = false;
    Стоп.CmdStop( State, LocalBlk, Stop, Err );
	  Стоп_2.CmdStop( State, LocalBlk, Stop, Err );
    Пуск_Стоп.CmdStartStop( State, LocalBlk, Start, Stop, Err );
    //if ( Command < 0. )
    //  {
    //  if ( pSys->IsBlk || LocalBlk )
    //    {
    //    Stop = true;
    //    }
    //  else
    //    {
    //    if ( On )
    //      {
    //      Err = true;
    //      Set_ErrBlkInObj ( ObjName );
    //      }
    //    }
    //  }
	if(!ErrInBlk)
		ErrInBlk = Err;
    if ( Stop && Defect != 4 )
      On = false;
    //  pPump-> Off_State ( CPumpData::Work );//Выключение
    }
  //
  //if ( pPump->Is_Ctrl ( CPumpData::Set_Off ))
  //  {
  //  pPump->Off_Ctrl ( CPumpData::Set_Off );
  //  if ( Defect != 4 )
  //    pPump-> Off_State ( CPumpData::Work );//Включение
  //  }
  if ( !IsElectro	)
	  On = false;
 
 /*****/
  if ( Defect == 1 )
    Авария = 1;
  else
    Авария = 0;
//  Дистанционный = 1;
  if ( Defect == 2 )
	  On = false;
  //
//  On = pPump->Is_State ( CPumpData::Work );
  double OZ = 0.;
  if ( On )
    {
    if ( Oborot_Z > 0. )
      OZ = Oborot_Z;
    else
      {
      if ( pMotor == NULL )
        {
        if ( Задание.Use())
          {
          OZ = Задание * 0.01;
          if ( OZ > 1. )
            OZ = 1.;
          }
        else
          OZ = 1.;
        //if ( Заданнные_обороты.Use())
        //  OZ = Заданнные_обороты / Nom_Oborot;
        }
      }
    if( Oborot >= OZ - 0.01 )
      Oborot = OZ;
    else
      Oborot += dt * 3600. / Tau;
    }
  else
    {
    if ( Oborot > 0.01 )
      Oborot = 0.9 * Oborot;
    else 
      Oborot = 0.;
    }
  }


