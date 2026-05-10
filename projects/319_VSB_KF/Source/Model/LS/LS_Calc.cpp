#include "stdafx.h"
#define DLL_LS
#include "LS.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "SetData.h"


void CLS::Calc( double dt )
	{
  SET_BP BreakPoint;
  double True = 0.;
  if ( pValue )
    True = *pValue;
  else
    if ( pBaseSens )
      pBaseSens->SetData( sd_GetSensInfo, &True );
  TrueValue = True * Gain + Bias;
  if ( Value > VH )
    Value = VH;
  if ( Value < VL )
    Value = VL;
  ShowValue = Value;
  Показания = ShowValue;
  if ( Value > Sign_H )
    {
    IsSign_H = true;
    _IsSign_H = 1;
    }
  else
    {
    IsSign_H = false;
    _IsSign_H = 0;
    }
  if ( Value < Sign_L )
    {
    IsSign_L = true;
    _IsSign_L = 1;
    }
  else
    {
    IsSign_L = false;
    _IsSign_L = 0;
    }
//
  if ( Value > Blk_H )
    {
    IsBlk_H = true;
    _IsBlk_H = 1;
    }
  else
    {
    IsBlk_H = false;
    _IsBlk_H = 0;
    }
  if ( Value < Blk_L )
    {
    IsBlk_L = true;
    _IsBlk_L = 1;
    }
  else
    {
    IsBlk_L = false;
    _IsBlk_L = 0;
    }
	}
