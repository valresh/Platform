//---------------------------------------------
TAB("Режим", 1 )
	PARM_D( _P, "#Давление(и)", 0. );
	PARM_D( _T, "#Температура", 0. );
	PARM_D( _F_in, "#Сумма входящих потоков, кг", 0. );
	PARM_D( _dF_in, "#Дисбаланс потоков, кг", 0. );
//  PARM_D( _dH_io, "#Дисбаланс потоков тепла, кДж/ч", 0. );
ETAB
#ifdef GET_PARAMS
TAB("Потоки, кг",2)
for ( int n = 0; n < kIO; n++ )
  {
  Name.Prt ( "#%s", (char*)pNodes[n].pOP->pPool->Name );
  PARM( Flow_kg[n], Name );
  }
ETAB
#endif
//PARM_I( Timer, "#Timer", 0 );
//PARM_I( Timer2, "#Count", 0 );
TAB("Конструктив", 1 )
  PARM_L( Summ_In, "Вход коллектора", PipeNames );
  PARM_L( Summ_Out, "Выход коллектора", PipeNames );
  PARM_B( UseOut, "Использовать вход/выход", false );
  PARM(NameIn,"Труба на вход")
  PARM(NameOut,"Труба на выход")
ETAB

TAB("Параметры", 1 )
//	PARM_D( Tau_C, "Инерционность изменения состава", 10. );
//	PARM_D( Tau_T, "Инерционность изменения температуры", 30. );
//	PARM_B( Use_H, "Расчет через энтальпию", true );
  PARM_D( Cp, "Теплоемкость коллектора", 10. );
	PARM_D( kPress, "Инерционность коллектора", 1. );
  PARM_D( k_In, "Инерционность входов", 1. );
  PARM_D( k_SummIn, "Инерционность Суммарного входа", 1.0 );
  PARM_D( dP_io, "#Дисбаланс давлений", 0. );
#ifdef GET_PARAMS
  TAB("Проходимости",2)
    for ( int n = 0; n < kIO; n++ )
      {
      Name.Prt ( "Вход %s", (char*)pNodes[n].pOP->pPool->Name );
      PARM( Omega[n], Name );
      }
  ETAB
#endif
  TAB("Пределы", 1 )
  PARM( T_min, "#Минимальная температура" );
  PARM( T_max, "#Максимальная температура" );
	PARM_D( P_min, "Мин. давление(а)", -100. );
	PARM_D( P_max, "Макс. давление(а)", 1000. );
ETAB
ETAB
	//

TAB("Задания", 1 )
PARM_D( Tz, "Заданная температура", 20. );
PARM_B( Use_Tz, "Использовать заданную температуру", false );
PARM_D( Pz, "Заданное давление(и)", 0. );
PARM_B( Use_Pz, "Использовать заданное давление", false );
PARM_S( Состав, 32, "Заданный состав", "" );
PARM_B( Use_Comp, "Использовать заданный состав", false );
ETAB

#ifdef GET_PARAMS
TAB("Старые названия", 1 )
	PARM( Pio, "#Давление на входе(а)" );
  PARM_D( Tz, "Заданная T", 20. );
  PARM_B( Use_Tz, "Использовать заданную T", false );
  PARM_D( Pz, "Заданное P(и)", 0. );
  PARM_B( Use_Pz, "Использовать заданное P", false );
  PARM_S( Состав, 32, "Состав", "" );
  PARM_B( Use_Comp, "Использовать заданный состав", false );
  PARM( NumbIn, "#Номер входа" );
  PARM( NumbOut, "#Номер выхода" );
ETAB
#endif
