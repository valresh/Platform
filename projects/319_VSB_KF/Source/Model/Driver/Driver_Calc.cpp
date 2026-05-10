#include "stdafx.h"
#include "Driver.h"
#include "Err.h"
#include "CommProc.h"


void CDriver::Calc( double dt )
	{
  SET_BP BreakPoint;
  double Pow = 0.;
  for ( int n = 0; n < kDrive; n++ )
    {
    if ( n == nMotor )
      {
      CMotorData * pMotor = pMotorData[n];
      On = pMotor->On;
      RelOborot = pMotor->RelOborot; // 0 - 1
      AbsOborot = pMotor->AbsOborot; // об/мин
      }
    else
      Pow += pMotorData[n]->Power;
    }
  Power = Pow; 
  for ( int n = 0; n < kDrive; n++ )
    {
    if ( n == nMotor )
      pMotorData[n]->Power = Power;
    else
      {
      CMotorData * pClient = pMotorData[n];
      pClient->On = On;
      pClient->RelOborot = RelOborot;
      pClient->AbsOborot = AbsOborot;
      }
    }
	}
