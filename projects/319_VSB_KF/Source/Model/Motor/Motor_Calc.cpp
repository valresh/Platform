#include "stdafx.h"
#include "Motor.h"
#include "Err.h"
#include "CommProc.h"


static double To_MWt =  1e-3 / 3600.;

void CMotor::ElectroOn()
{
	IsElectro = true;

}

void CMotor::ElectroOff()
{
	IsElectro = false;
}

void CMotor::Calc( double dt )
	{
  SET_BP BreakPoint;
//  Протечка_статора = _Протечка_статора;
//  Протечка_коробки = _Протечка_коробки;
  MotorData.AbsOborot = Oborot * NomOborot;
  if ( Угол_атаки.Use())
	  MotorData.AngleOfAttack = Угол_атаки * 0.01;
  double Pow_Drv = 0.;
  for(int i = 0; i < MAX_DRIVE; i++)
  {
	  if(DrivePow[i])
		  Pow_Drv += *DrivePow[i];
  }
  double Pow = Pow_Drv;
  Pow_Mwt = Pow * 1e-3;
  bool Curr_On = On;
  bool IsBlk = false;
  if ( Defect == АварияДвигателя	)
    {
    Авария = 1;
    Готовность = 0;
    Неисправность_ПЧ = 1;
    }
  else
    {
    Авария = 0;
    Готовность = 1;
    Неисправность_ПЧ = 0;
    }
  if(Дист_с_РСУ.Use())
    {
	//Нужно подумать как сделать в новой системе
// 	  if(Дист_с_РСУ)
// 		  pPump->LocalRemote = 1;
// 	  else
// 		  pPump->LocalRemote = 0;
    }
 //////////////////////////////////////
  if ( !IsElectro || Defect == ОтказДвигателя )
    {
    On = false;
//     pPump->Off_State ( CPumpData::Work );
//     pPump->Off_Ctrl( CPumpData::Set_On );
//     pPump->Off_Ctrl( CPumpData::Set_Off );
    Готовность = 0;
    }
  else
    {
    if ( Defect == ОтказАппаратуры )
      {
//       pPump->Off_Ctrl( CPumpData::Set_On );
//       pPump->Off_Ctrl( CPumpData::Set_Off );
      Готовность = 0;
      return;
      }
    if ( Defect != ОтказАппаратуры )
      {
      ////////////////////////////////////////////////////
      Готовность = 1;
      int Dist = 0;  // Dist < 0 - команды РСУ недоступны
//       if ( Дистанционный.Use())
//         {
//         //pPump->On_Type( CPumpData::MST_DIST );
//         int Pos = pPump->Use_Flags % 10; // Последняя 10
//         if ( Pos == 1 )
//           {
//           Dist = 1;
//           Дистанционный = 1;
//           }
//         else
//           {
//           Dist = -1;
//           Дистанционный = 0;
//           }
//         }
//       else
//         {
//         pPump->Off_Type( CPumpData::MST_DIST );
//         }
//       if ( Дистанционное_управление.Use())
//         {
//         int Pos = pPump->LocalRemote;
//         if ( Pos == 1 )
//           {
//           Dist = 1;
//           Дистанционное_управление = 1;
//           }
//         else
//           {
//           Dist = -1;
//           Дистанционное_управление = 0;
//           }
//         }
 ////////////////////////////////////////////////////
      bool Can = LocalBlk || pSys->IsBlk;
      //bool * pBlk = &pSys ->IsBlk;
      bool Can_Pusk = true;
      if ( Пуск_разрешен.Use() && Пуск_разрешен.Off())
        Can_Pusk = false;
      if ( Запрет_пуска.On())
        Can_Pusk = false;
//       if ( pPump->Is_Ctrl( CPumpData::Set_On ))
//         {
//         pPump->Off_Ctrl( CPumpData::Set_On );
//         if ( Can_Pusk )
//           {
//           On = true;
//           pPump->On_State (  CPumpData::Work  );
//           }
//         }
      bool Err = false;
      bool Pusk = false;
      bool Stop = false;
      int State = On ? 1 : 0;
      Пуск.CmdStart( State, LocalBlk, Pusk, Err );
      Пуск_2.CmdStart( State, LocalBlk, Pusk, Err );
      Пуск_Стоп.CmdStartStop( State, LocalBlk, Pusk, Stop, Err );
      if ( Dist < 0 )
        Pusk = false;
      if ( Defect == ОтказАппаратуры )
        Pusk = false;
      if ( Can_Pusk && Pusk )
        {
        On = true;
        //pPump->On_State ( CPumpData::Work );
        }
/////////////////////////////////////////
      Стоп.CmdStop( State, LocalBlk, Stop, Err );
      Стоп2.CmdStop( State, LocalBlk, Stop, Err );
      Стоп_2.CmdStop( State, LocalBlk, Stop, Err );
      Стоп_3.CmdStop( State, LocalBlk, Stop, Err );
//      Пуск_Стоп.CmdStartStop( State, LocalBlk, Pusk, Stop, Err );
      if ( Dist < 0 )
        Stop = false;
      if ( Defect == ОтказАппаратуры )
        Stop = false;
      if ( NoStop )
        Stop = false;
      if ( !Can )
        Stop = false;
      if ( Stop )
        {
        On = false;
       // pPump->Off_State ( CPumpData::Work );
        }
///
      Стоп.CmdStop( State, LocalBlk, Stop, Err );
      if ( Dist < 0 )
        Stop = false;
      if ( Defect == ОтказАппаратуры )
        Stop = false;
      if ( NoStop )
        Stop = false;
      if ( !Can )
        Stop = false;
      if ( Stop )
        {
        On = false;
        //pPump->Off_State ( CPumpData::Work );
        }
///
//       if ( pPump->Is_Ctrl( CPumpData::Set_Off ))
//         {
//         On = false;
//         pPump->Off_Ctrl( CPumpData::Set_Off );
//         pPump->Off_State (  CPumpData::Work  );
//         pPump->Off_Ctrl( CPumpData::Set_On );  //???? Пока Миша не будет сбрасывать
//         }
      ErrInBlk = Err;
      if ( ErrInBlk )
		  strcpy_s( Err_BlkInObj, pMain ? pMain->ObjName.Str : ObjName.Str );
      }
    }
  ////////////////////////////////////////////////
  if ( On )
    {
    Работает = 1;
    Фаза_пуска_5 = 1;
    Фаза_пуска_1 = 0;
    }
  else
    {
    Работает = 0;
    Фаза_пуска_5 = 0;
    Фаза_пуска_1 = 1;
    }
  Пуск_зеленый = 1;
  Пуск_возможен = 1;
  ///////////////////////////////////////////////////////////////////
  double OborotMax = Defect_Pow;
  if ( Управление_по_месту && Задание_по_месту.Use())
    OborotMax = Задание_по_месту * 0.01 * Defect_Pow;
  if ( !Управление_по_месту && Задание.Use() )
    OborotMax = Задание * 0.01 * Defect_Pow;
  if ( Oborot_AntiReg > 0. )
    OborotMax = Oborot_AntiReg;
  ЧП = 1;
  БП = 0;
  if(Reg_Speed.Use)
	  Oborot = Reg_Speed;
  else if ( On )
    {
    if ( Oborot <= OborotMax )
      {
      Oborot += dt * 3600. / Tau_On;
      if ( Oborot > OborotMax )
        Oborot = OborotMax;
      }
    else
      {
      Oborot -= dt * 3600. / Tau_On;
      if ( Oborot < OborotMax )
        Oborot = OborotMax;
      }
//////////////////////////////////////////////////////////////
    CalcТок ( dt, Pow );
    if ( NewModelТок )
      CalcПускТок ( dt, Pow );
//////////////////////////////////////////////////////////////
    }
  else
    {
    if ( Oborot > 0. )
      {
      Oborot -= dt * 3600. / Tau_Off;
      if ( Oborot < 0. )
        Oborot = 0.;
      }
    Ток = 0;
    Мощность = 0.;
    }
  //pPump->Oborot = Oborot;
  Обороты = Oborot;
////////////////////////////////////////////
 	}


void CMotor::CalcТок ( double dt, double Pow )
  {
  double I;
  if ( k_Ток > 0. )
    {
    I = k_Ток * Pow * To_MWt; 
    if ( Pusk > 0. && Oborot > 0.02 )
      I += Pusk * ( 1. - Oborot );
    }
  else
    {
    I = B_Ток * ( 2. - Oborot ) + A_Ток * Pow; 
    if ( On )
      I = B_Ток * ( 2. - Oborot ) + A_Ток * Pow; 
    else
      I = 0.;
    }
  if ( kStep++ >= 4 )
    {
    if ( k_Pow > 0. )
      Мощность = k_Pow * Pow * To_MWt;
    else
      Мощность = I * U_work;
/*
    if ( Oborot > 0.5 && I > Avar_Ток )
      {
      kAvar++;
      if ( kAvar < 20 )
        {
        Ток = 230. + LimdX ( I - 230., 10. );
        }
      else
        {
        Ток = I;
        On = false;
        pPump->Off_State ( CPumpData::Work );
        }
      }
    else
      {
      Ток = I;
      kAvar = 0;
      }
*/
    if ( I > St_ТокМакс )
      I = St_ТокМакс;
    Ток = I;
    }
  }

void CMotor::CalcПускТок ( double dt, double Pow )
  {
  St_Pow = Pow * To_MWt;
  St_Мощность = St_k_Pow * St_Pow;
//
  if ( St_Pow < St_Pow_1 )
    St_Ток = St_Ток_1 * St_Pow / St_Pow_1;
  else
    {
    double A = ( St_Ток_2 - St_Ток_1 ) / ( St_Pow_2 - St_Pow_1 );
    double B = St_Ток_2 - A * St_Pow_2;
    St_Ток = A * St_Pow  + B;
    }
  St_Ток += St_PuskТок * ( 1. - Oborot );
  if ( St_Ток > St_ТокМакс )
    St_Ток = St_ТокМакс;
  if ( !BaseModelТок && kStep++ >= 4 )
    {
    Ток = St_Ток;
    Мощность = St_Мощность;
    }
  }
/////////////////////////////////////////////////////////////////////////////////////
