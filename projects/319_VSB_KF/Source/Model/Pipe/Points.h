TAB("Режим", 1   )
#ifdef SHOW
  ShowP.GetParams( "#Давление", true );
#endif
PARM( _P, "#Давление, атм");
//PARM_L( Mes_P, "Размерность давления", "атм;Па;КПа;МПа;мм в.ст.;мм рт.ст.;Па(а);мм в.ст.(а);мм рт.ст.(а);" )
PARM_D( _T, "#Температура", 0. );
//
#ifdef SHOW
  ShowFlow.GetParams( "#Суммарный поток", true );
#endif
//PARM_D( _F_mol, "#Поток моль", 0. );				 
//PARM_D( _F_nm3, "#Поток нм3", 0. );				 
//PARM_D( _F_kg, "#Поток кг", 0. );				 
//PARM_L( Mes_F, "Размерность потока", "кмоль/ч;кг/ч;м3/ч;л/ч;нм3/ч;" )
//IF(UseSolidFlow)
//PARM_D( _F_Solid_mol, "#Поток порошка моль", 0. );				 
//PARM_D( _F_Solid_kg, "#Поток порошка кг", 0. );	
//ENDIF
//PARM_D( _F_razgerm_kg, "#Поток разгерметизации кг", 0. );
//
PARM_D( _Eps, "#Паросодержание", 0. )
PARM_D( _Ro, "#Плотность при стандартной температуре", 0. )
PARM(To_kg, "#Молекулярный вес")
PARM_B( OnlyLiq, "Расчет плотности по жидкости", false );
PARM_D( _h_Flow, "#Энтальпия", 0. )
PARM_D( _Flow_Summ, "#Сумма потоков", 0. )
PARM_I( _N_MB, "№ в МБ", -1 )
PARM_D( _М_MB, "Значение в МБ", -1. )

#ifdef SPECIAL
PARM_I( _Test, "Тест", 0 )
PARM( Alarm_Pipe, "#Нарушение границ в этой трубе" )
#endif
//PARM_D( _P_Pa, "#Давление(и) Па", 0. );
//PARM( ModelGroup, "#Группа" );
TAB("Дренажи", 5 )
PARM( kDrain, "#Число дренажей" )
//PARM_D( Vent_DrainageF, "#Суммарная проходимость", 0. )
PARM_D( Flow_DrainageF_m3, "#Поток газа через дренажи, нм3", 0. )
PARM_D( Flow_DrainageF_kg, "#Поток жидкости через дренажи, кг", 0. )
//PARM_D( Flow_Max_m3, "#Макс поток дренажей м3", 0. )
//PARM_D( Flow???? ? ?? ?????_Max_kg, "#Макс поток дренажей кг", 0. )
//PARM( TypeDrain, "#Типы дренажей" )
ETAB
ETAB

TAB("Параметры", 2 )
//??PARM( Очищать, "Очищать" )
PARM_D( M, "Масса в трубе", 0.01 )
#ifdef N2
PARM( Show_N2, "Показывать N2" )
PARM( Limit_N2, "Предел N2, %" )
PARM( ErrN2, "Где нарушен предел N2, %" )
#endif
PARM_B ( Delete_N2, "Убрать N2", false )
PARM( Vol, "Объем трубы" )
PARM_D( F_min, "Минимальный поток, моль", 0.1 )
PARM_D( TauCool, "Время охлаждения", 10. )
PARM_D( Omega, "Подтекание", 0. )
PARM_B( CalcT, "Расчет температуры", false );
PARM( T_min, "#Мин. температура" )
PARM( T_max, "#Макс. температура" )
PARM( Q_max, "Максимальная загазованность при разгерметизации" )
PARM_D( Omega_Sliv, "Проходимость слива", 1000. )
//PARM_D( Flow_Drain_Nom, "Номинальный поток дренажей", 10. )
//PARM_B( OutFlowKg, "Вывод потоков в кг", false )
//PARM_D( Min_Flow, "Мин поток разгерметизации", 0.01 )
//
TAB("Дополнения", 5 )
PARM_B( DeleteAir, "#Убрать газ", false );
PARM_B( Cool, "#Охладить", false );
PARM( SaveFlow, "#Сохранить состав в трубе" );
PARM_B( ComprComp, "#Сжать состав", false );
//PARM_D( P_Calc_State_Z, "Заданное давление расчета состояния(а)", 0. )
//PARM_D( T_heater, "T подогрева", 0. )
//PROP_B( Use_Filtr_Flow, "Использовать фильтры потоков", false )
PARM_B( UseEtalon, "Использовать эталон", false );
PARM_I( nEtalonFlow, "Номер потока для эталона", -1 );
PARM_L( EtalonToOut, "Эталон на выход", "Нет;Как давление;Как поток;" );
#ifdef SHOW
if ( pEtalonComp )
  {
  TAB("Эталонный состав", 2 )
    pEtalonComp->GetParams( this );
  ETAB
  }
#endif

ETAB

//TAB("Проверки", 5 )
//PARM_B( Test_P, "Проверять давление", false );
//PARM_D( P_test_nom, "Моминальное давление(и)", 0. )
//PARM_D( P_test_delta, "Допустимые изменения давления", 0. )
//PARM_B( Test_F, "Проверять поток, кг", false );
//PARM_D( F_test_nom, "Моминальный поток, кг", 0. )
//PARM_D( F_test_delta, "Допустимые изменения потока", 0. )
//PARM_B( Test_S, "Проверять стабильность потоков", false );
//ETAB
//
//TAB("Задания(временно)", 5 )
//PARM_D( F_Fix, "Заданный поток", 0. )
//PARM_I( nF_Fix, "#N заданного потока", -1 )
//ETAB("Задания(временно)", 5 )
////
ETAB
