#include "stdafx.h"
#include "PC_1.h"
#include "Err.h"
#include "CommProc.h"


void CPC_1::Calc( double dt )
	{
  SET_BP BreakPoint;
  if ( pMotor )
  {
	  On = *pMotor->pOn;
	  Обороты = Oborot = *pMotor->pRelOborot;
	  //    DefectStop = pMotorData->DefectStop;
  }
  if ( Задание.Use())
    UseCtrl = true;
  else
    UseCtrl = false;
//  bool Разрешение = !Разрешение_пуска.Use() || Разрешение_пуска.On();
  if ( Пуск_стоп.Use())
    {
    bool Pusk = false;
    bool Stop = false;
    bool Err = false;
    Пуск_стоп.CmdStartStop( State, LocalBlk, Pusk, Stop, Err );
    if ( Pusk )
      On = true;
    if ( Stop )
      On = false;
    ErrInBlk = Err;
    }
  else
    {
    if ( pMotor == NULL && Пуск.On())
//    if ( pMotorData == NULL && ( Пуск.On() || Пуск_2.On()) )//&& Разрешение )
      {
      if ( CanPuskStop && pSys->IsBlk )
        {
        ErrInBlk = false;
        On = true;
        }
      else
        {
        if ( !On )
          {
          ErrInBlk = true;
          strcpy_s( Err_BlkInObj, ObjName.Str );
          }
        else
          ErrInBlk = false;
        }
      }
    if ( pMotor == NULL && ( Стоп.On() || Стоп_2.On()))
      {
      if ( CanPuskStop && pSys->IsBlk )
        {
        ErrInBlk = false;
        On = false;
        }
      else
        {
        if ( On )
          {
          ErrInBlk = true;
          strcpy_s( Err_BlkInObj, ObjName.Str );
          }
        else
          ErrInBlk = false;
        }
      }
    }
  //
  //
  //if ( pMotorData == NULL && pPump->Is_Ctrl( CPumpData::Set_On ))
  //  On = true;
  //if ( pMotorData == NULL && pPump->Is_Ctrl( CPumpData::Set_Off ))
  //  On = false;
  //
  if ( DefectStop )
    {
    On = false;
    }
  //
  if ( Electro_6000 )
    {
    if ( !pExt->IsElectro_6000	)
      On = false;
    }
  else
    {
    if ( !pExt->IsElectro_380	)
      On = false;
    }
  //
  //if ( pPump->LocalRemote == 1 )
  //  Дистанционное_управление = 1;
  //else
  //  Дистанционное_управление = 0;
  //
//  if ( LocalBlk || pSys->IsBlk )
#if 0
    {
    if ( Клапан_1.Use() && Клапан_2.Use() && Клапан_3.Use())
      {
      bool k_1 = Клапан_1.On();
      bool k_2 = Клапан_2.On();
      bool k_3 = Клапан_3.On();
      if ( k_1 && k_2 && k_3 )
        {
        State = 4;
        Oborot = 1.;
        }
      if ( !k_1 && k_2 && k_3 )
        {
        State = 3;
        Oborot = 0.75;
        }
      if ( !k_1 && !k_2 && k_3 )
        {
        State = 2;
        Oborot = 0.5;
        }
      if ( !k_1 && !k_2 && !k_3 )
        {
        State = 0;
        Oborot = 0.;
        }
      }
    else
      {
      if ( Кл_1.Use())
        {
        int K = 0;
        if ( Кл_1.On())
          K++;
        if ( Кл_2.On())
          K++;
        State = K;
        switch ( K )
          {
          case 0:
            Oborot = 0.;
          break;
          case 1:
            Oborot = 0.5;
          break;
          case 2:
            Oborot = 1.;
          break;
          }
        }
      else
        {
//        UseCtrl = false;
        if ( UseCtrl )
          {
          State = 0;
          Oborot = 0.;
          }
        if ( Задание_25.On())
          {
          State = 1;
          Oborot = 0.25;
          }
        if ( Задание_50.On())
          {
          State = 2;
          Oborot = 0.5;
          }
        if ( Задание_75.On())
          {
          State = 3;
          Oborot = 0.75;
          }
        if ( Задание_100.On() || !UseCtrl || Set_100 )
          {
          State = 4;
          Oborot = 1.;
          }
        }
      }
    }
  ////////////////////
  switch ( State )
    {
    case 0:
      Загрузка_0 = 1;
      Загрузка_25 = 0;
      Загрузка_50 = 0;
      Загрузка_75 = 0;
      Загрузка_100 = 0;
      Положение_клапана = 0.;
    break;
    case 1:
      Загрузка_0 = 0;
      Загрузка_25 = 1;
      Загрузка_50 = 0;
      Загрузка_75 = 0;
      Загрузка_100 = 0;
      Положение_клапана = 25.;
      break;
    case 2:
      Загрузка_0 = 0;
      Загрузка_25 = 0;
      Загрузка_50 = 1;
      Загрузка_75 = 0;
      Загрузка_100 = 0;
      Положение_клапана = 50.;
    break;
    case 3:
      Загрузка_0 = 0;
      Загрузка_25 = 0;
      Загрузка_50 = 0;
      Загрузка_75 = 1;
      Загрузка_100 = 0;
      Положение_клапана = 75.;
    break;
    case 4:
      Загрузка_0 = 0;
      Загрузка_25 = 0;
      Загрузка_50 = 0;
      Загрузка_75 = 0;
      Загрузка_100 = 1;
      Положение_клапана = 100.;
    break;
    }
#endif
  double OborotZ;
  if ( pMotor )
    {
    Oborot = OborotZ = *pMotor->pRelOborot;
    On = *pMotor->pOn;
    }
  else
    {
    if ( On )
      {
      if ( Задание.Use())
        {
        UseCtrl = true;
        double U = Задание;
        OborotZ = U * 0.01;
        }
      else
        OborotZ = 1.;
      Работает = 1;
      }
    else
      {
      OborotZ = 0.;
      Работает = 0;
      }
    double dV = dt * 3.6e4 / Tau_Задание;
    //Oborot = Target;
    if ( Oborot < 0. )
      Oborot = 0.;
    if ( Oborot < OborotZ )
      {
      Oborot += dV;
      if ( Oborot > OborotZ )
        Oborot = OborotZ;
      }
    if ( Oborot > OborotZ )
      {
      Oborot -= dV;
      if ( Oborot < OborotZ )
        Oborot = OborotZ;
      }
    }
  Авария = 0;
  _Oborot = Oborot;
//
  ST_1.Oborot = Oborot;
//  ST_1.k_Cool = k_Cool;
  //else
  //  {
  //  if ( On )
  //    V = 1.;
  //  else
  //    V = 0.;
  //  }
  //------------------------
  if ( DefectPow )
    ST_1.PowZ = Oborot * RelDefectPow;
  else
    ST_1.PowZ = Oborot;
  ST_1.Oborot = Oborot;
  ST_1.Calc ( dt );
  if ( Flow_nm3 > 0. )
    {
    double K = Flow_nm3 / ( ST_1.Flow_Gas * M3 );
    ST_1.Flow_Nom_m3 *= K;
    }
//  double O = OmegaOI_1_on;
//  if ( !On )//Oborot == 0. )
//    O = OmegaOI_1_off;
//  ST_1._Flow_lose = ( ST_1._P_out - ST_1._P_in ) * O * M3;
//  T_gas_in = ST_1._T_in;
//  T_gas_out = ST_1._T_out;
//
  double PowExt = 0.;
//   for ( int n = 0; n < kExternal; n++ )
//     {
//     ExtOborot[n].On = On;
//     ExtOborot[n].Oborot = Oborot;
//     PowExt += ExtOborot[n].Pow;
//     }
//  Pow = ST_1._Pow_MWt;
  //double Pow_Rel = ST_1._Pow_MWt / Ном_Нагрузка * 100.;
  //if ( Pow_Rel > 90. )
  //  Pow_Rel = 90. + LimdX ( Pow_Rel - 90, 10. );
  static double To_MWt =  1e-3 / 3600.;
  Мощность = ( ST_1.Pow + PowExt ) * To_MWt;
  //double dOdt = OborotZ - Oborot;
  //if ( On )
  //  {
  //  Tok = Tok_max * ( Pow / Pow_max + Loose * Oborot + kTok_Omega * dOdt );
  //  }
  //else
  //  {
  //  Tok = 0.;
  //  }
  double I; 
  if ( On )
    I = B_Ток * ( 1. - Oborot ) + A_Ток * ST_1._Pow_MWt; 
  else
    I = 0.;
  Ток = I;
	}
