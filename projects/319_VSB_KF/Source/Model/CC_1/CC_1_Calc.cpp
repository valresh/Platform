#include "stdafx.h"
#include "CC_1.h"
#include "Err.h"
#include "CommProc.h"



void CCC_1::Calc( double dt )
{
  SET_BP BreakPoint;
  if ( pMotor )
  {
    On = *pMotor->pOn;
    Обороты = Oborot = *pMotor->pRelOborot * Reductor;
    //    DefectStop = pMotorData->DefectStop;
  }
  if ( OborotZ > 0. )
  {
    On = true;
    Oborot = OborotZ;
    Работает = 1;
    if ( pMotor && *pMotor->pRelOborot > 0. )
      Reductor = Oborot / *pMotor->pRelOborot;
    else
      Reductor = 1.;
  }
  if ( pMotor == NULL && OborotZ <= 0. )
  {
    if ( Пуск.On() || Пуск_Стоп.On() )// || Пуск_2.On() 
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
          strcpy_s( Err_BlkInObj, (char*)ObjName );
        }
        else
          ErrInBlk = false;
      }
    }
    //
    if ( Стоп.On() || Пуск_Стоп.Off()) // || Стоп_2.On()
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
          strcpy_s( Err_BlkInObj, (char*)ObjName );
        }
        else
          ErrInBlk = false;
      }
    }
    if ( DefectStop )
    {
      On = false;
    }
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
    //////////////////////////////////////////////
    double OZ = 0.;
    if ( On )
    {
      if ( Задание.Use())
        OZ = Задание * 0.01;
      else
        OZ = 1.;
      Работает = 1;
    }
    else
    {
      OZ = 0.;
      Работает = 0;
    }
    ////////////////
    if ( Oborot <= OZ )
    {
      if( Oborot < OZ )
        Oborot += dt * 3600. / Tau;
      if( Oborot > OZ )
        Oborot = OZ;
    }
    else
    {
      if( Oborot > OZ )
        Oborot -= dt * 36000. / Tau;
      if( Oborot < OZ )
        Oborot = OZ;
    }
  }
  //
  //if ( pOborot == NULL && pPump->Is_Ctrl( CPumpData::Set_On ))
  //  On = true;
  //if ( pOborot == NULL && pPump->Is_Ctrl( CPumpData::Set_Off ))
  //  On = false;
  //
  //
  //  if ( pPump->LocalRemote == 1 )
  //    Дистанционное_управление = 1;
  //  else
  //    Дистанционное_управление = 0;
  //
  //
  double PowExt = 0.;
  for ( int n = 0; n < kExternal; n++ )
  {
    ExtOborot[n].On = On;
    ExtOborot[n].Oborot = Oborot;
    PowExt += ExtOborot[n].Pow;
  }
  //
  if ( Reg_Press_out.IsOn() && pSys->bWorkReg && !pSys ->bInit_0 )
  {
    //double Pi = ST_1.pFlow_In->P;
    //double Po = ST_1.pFlow_Out->P;
    //double k_Real = Po / Pi;
    //double k_Z = ( Reg_Press_out + 1. ) / Pi;
    //double K = LimdX ( k_Z / k_Real -1, Reg_kdP ) + 1.;
    //ST_1.k_dP_0 *= K;
    //if ( ST_1.k_dP_0 < Min_kdP )
    //   ST_1.k_dP_0 = Min_kdP; 
    //if ( ST_1.k_dP_0 > Max_kdP )
    //  ST_1.k_dP_0 = Max_kdP; 
    //
    ST_1.P_Out_Z = Reg_Press_out + 1.;
    ST_1.Use_P_Out_Z = true;
  }
  else
    ST_1.Use_P_Out_Z = false;
  //
  double Pow_Compr = 0.;
  bool Is_Surge = false;
  ST_1.Oborot = Oborot;
  ST_1.Fix_k = Fix_K;
  if ( Reg_T_out.IsOn())
  {
    ST_1.T_Out_Z = Reg_T_out;
    ST_1.WorkReg_PT = true;
  }
  else
    ST_1.WorkReg_PT = false;
  //
  ST_1.Calc ( dt );
  //   if ( pOborot )
  //     pOborot->Pow = ST_1.Pow;
  static double To_MWt =  1e-3 / 3600.;
  Pow_Compr = ( ST_1.Pow + PowExt ) * To_MWt;
  if(ST_1.Thermo.P_in != 0.0)
    To_m3_in = RP * ( ST_1.Thermo.T_in + TK ) / ST_1.Thermo.P_in;
  else
    To_m3_in = 0.0;
  //  double Flow_m3 = ST_1._Flow_m3;
  double I; 
  if ( On )
    I = B_Ток * ( 1. - Oborot ) + A_Ток * Pow_Compr; 
  else
    I = 0.;
  Ток = I;
  Мощность = ST_1._Pow_MWt;
}
