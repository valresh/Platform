#include "stdafx.h"
#include "HeatAntiFlow.h"
#include "Err.h"
#include "CommProc.h"
// #include "EventLogTypes.h"
// #include "Names.h"

ON_DEFECT(CHeatAntiFlow)
	{
  IF_DEFECT(Восстановление_характеристик,"Восстановление характеристик")
  Heat_Defect = false;
  k_Heat = 1.;
  Omega_Defect = false;
  k_Omega = 1.;
  FlowQ = 0.;
  Перемешивание = false;
  Def_Trub = 0.;
  END_IF
  IF_DEFECT(Ухудшение_теплообмена,"Ухудшение теплообмена")
    Heat_Defect = true;
    k_Heat =  ( 100. - k_Defect_Heat->Dbl ) * 0.01;
    if ( k_Heat < 0.01 )
      k_Heat = 0.01;
  END_IF
  IF_DEFECT(Засорение_труб,"Засорение труб")
    Omega_Defect = true;
    k_Omega =  ( 100. - k_Defect_Omega->Dbl ) * 0.01;
  END_IF
  IF_DEFECT(Утечка,"Утечка")
    FlowQ = def_Утечки->Dbl * 0.01;
  END_IF
  IF_DEFECT(Внутренняя_разгерметизация,"Внутренняя_разгерметизация")
    Def_Trub = _def_in_Trub * def_in_Trub->Dbl * 0.02;
    Перемешивание = true;
  END_IF
/*
  IF_DEFECT(Разгерметизация_труб,"Разгерметизация труб")
    Trub_Defect = true;
    Def_Omega_Trub = def_Omega_Trub->Dbl * 0.01 * _def_Omega_Trub;
    FlowQ = 10.;
  END_IF
  IF_DEFECT(Разгерметизация_объема,"Разгерметизация объема")
    Vol_Defect = true;
    Def_Omega_Vol = def_Omega_Vol->Dbl * 0.01 * _def_Omega_Vol;
    FlowQ = 10.;
  END_IF
  IF_DEFECT(Смешивание_потоков_на_входе_труб,"Смешивание потоков на входе труб")
    Def_Trub = _def_in_Trub * def_in_Trub->Dbl;
  END_IF
  IF_DEFECT(Смешивание_потоков_на_выходе_труб,"Смешивание потоков на выходе труб")
    Def_outTrub = _def_out_Trub * def_out_Trub->Dbl;
  END_IF
  IF_DEFECT(Загазованность,"Загазованность")
    FlowQ = 10.;
  END_IF
*/
	return 0;
  }

