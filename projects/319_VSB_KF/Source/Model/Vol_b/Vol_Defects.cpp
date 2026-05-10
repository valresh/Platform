#include "stdafx.h"
#include "Vol.h"
#include "Err.h"

DWORD CVol::ClassRootNode = 0;
int CVol::OnDefect( CDef * pDefect )
	{
  SET_BP BreakPoint;
  LPCSTR szVol = "Емкость";
  char szMessage[150];
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
    HeatDefect = false;
    VolBase.Defect = 0;
    VolBase.EmptyVol = false;
    VolBase.FullVol = false;
    FlowQ = 0.;
    DefectUplot = false;
    Reg_T.Use = false;
	sprintf_s ( szMessage, sizeof(szMessage),  "%s,%s,%s", szVol, pDefect->ObjName, pDefect->Name );
//	if ( pMessagingUser )
//		(*pMessagingUser)( szMessage, RGB( 255, 0, 0 ),  RGB( 255, 255, 255 ) );
  END_IF
  IF_DEFECT(Опустошение,"Опустошение")
    VolBase.EmptyVol = true;
  END_IF
  IF_DEFECT(Переполнение,"Переполнение")
    VolBase.FullVol = true;
  END_IF
//   IF_DEFECT( Разгерметизация_сверху,"Разгерметизация верха")
//     VolBase.Defect = 1;
//     VolBase.Def_Omega_top = Def_Omega_top;
//     FlowQ = def_Загазован_1->Dbl * 0.01;
// 	sprintf_s ( szMessage, sizeof(szMessage),  "%s,%s,%s", szVol, pDefect->ObjName, pDefect->Name );
// 	if ( pMessagingUser )
// 		(*pMessagingUser)( szMessage, RGB( 255, 0, 0 ),  RGB( 255, 255, 255 ) );
//   END_IF
//   IF_DEFECT( Разгерметизация_снизу,"Разгерметизация низа")
//     VolBase.Defect = 2;
//     VolBase.Def_Omega_bott = Def_Omega_bott;
//     VolBase.Def_Hydro_bott = Def_Hydro_bott;
//     FlowQ = def_Загазован_2->Dbl * 0.01;
// 	sprintf_s ( szMessage, sizeof(szMessage),  "%s,%s,%s", szVol, pDefect->ObjName, pDefect->Name );
// 	if ( pMessagingUser )
// 		(*pMessagingUser)( szMessage, RGB( 255, 0, 0 ),  RGB( 255, 255, 255 ) );
//   END_IF
  IF_DEFECT(Уход_уровня,"Уход уровня")
    Reg_L_1.Value = Def_Low_level->Dbl;// "Остаточный уровень", 1. )
    Reg_L_1.Use = true;
  END_IF
  IF_DEFECT(Изменение_температуры,"Изменение_температуры")
    Reg_T.Set ( Def_TZ->Dbl );
  END_IF
  IF_DEFECT(Загазованность,"Загазованность")
    FlowQ = def_Загазован_4->Dbl * 0.01;
  END_IF
	return 0;
  }

