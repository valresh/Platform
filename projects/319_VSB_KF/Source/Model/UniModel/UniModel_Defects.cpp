#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"


int CUniModel::OnDefect( CDef * pDefect )
	{
//  SET_BP BreakPoint;
//  IF_DEFECT(Опустошение,"Опустошение")
//	Defect = 2;
//	Уровень = 0.0;
//  END_IF
//  IF_DEFECT(Переполнение,"Переполнение")
//	Defect = 2;
//	Уровень = 100.0;
//  END_IF
//  IF_DEFECT(Восстановление,"Восстановление")
//    Defect = 0;
//    FlowQ = 0.;
//  END_IF
////
//  IF_DEFECT(Засорение,"Засорение фильтра")
////    DEF_D (Засор, "Степень засорения, %", 50, 0, 100 )
//  Defect = 1;
//  double kDef = 1. - Засор->Dbl * 0.01;
//  if ( kDef < 1e-3 )
//    kDef = 1e-5;
//  Omega_def = pow ( kDef, Pow_Defect );
//  if ( Omega_def > 1. )
//    Omega_def = 1.;
//  if ( Omega_def < 1e-10 )
//    Omega_def = 1e-10;
//  END_IF
//  IF_DEFECT(Загазованность,"Загазованность")
//    FlowQ = Уровень_загазованности->Dbl * 0.01;
//  END_IF
	return 0;
  }

