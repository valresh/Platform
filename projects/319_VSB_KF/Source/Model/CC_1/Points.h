TAB("Режим",1)
	PARM( On, "#Работает" )
	PARM( Oborot, "#Относительные обороты" )
  PARM_B ( Is_Surge, "#Помпаж", false )
  PARM_D ( Surge_stock, "#Помпажный запас", 0. )
  PARM ( ST_1._P_in, "#Давление на входе(и)" )
  PARM ( ST_1._P_out, "#Давление на выходе(и)" )
  PARM ( ST_1._T_in, "#Температура на входе" )
  PARM ( ST_1._T_out, "#Температура на выходе" )
  PARM_D ( Flow_M3, "#Поток м3", 0. )
  PARM ( ST_1._Flow_m3, "#Поток нм3" )
  PARM ( ST_1._Flow_kg, "#Поток кг" )
  PARM( ST_1.k_dP, "#Рабочий напор" )
  PARM_D( k_dP_Real, "#Спад напора", 0. )
  PARM_D( k_dP_Summ, "#Фактический напор", 0. )
  PARM ( ST_1._Pow_MWt, "#Мощность МВт" )
ETAB
TAB("Конструктив",1)
	PARM( ST_1.k_dP_0, "Номинальный напор" )
  PARM_D( Flow_Surge, "Граница помпажа, м3", 5000. )
  PARM_D( Flow_Nom, "Номинальная подача, м3", 10000. )
  PARM_D( Спад, "Отн. напор при ном. подаче", 0.6 )
  PARM_I( k_Points, "Число точек рабочей характеристики", 20 )
  PARM( ST_1.Tin_0, "Номинальная Т на входе" )
  PARM( ST_1.Mu_0, "Номинальный мол. вес газа" )
  PARM( ST_1.GetCurrData, "#Взять данные из потока" )
//
  PARM ( ST_1.Thermo.Cp, "Теплоемкость корпуса" );
ETAB
#ifdef GET_PARAMS
TAB("Ступень",1)
	ST_1.GetParams( "" );
ETAB
#endif
TAB("Параметры",1)
  PARM( ST_1.Gamma, "Показатель политропы" );
  PARM( ST_1.KPD, "КПД термодинамический %" );
  PARM( ST_1.KPD_M, "КПД механический %" );
  PARM_D( Tau, "Время разгона, с", 10. )
  PARM_B( Electro_6000,"Питание 6000V", false )
  PARM_D( A_Ток, "Рабочий ток, A/МВт", 1e6 / 6000. )
  PARM_D( B_Ток, "Пусковой ток, A", 100. )
  PARM_S( Мотор, 32, "#Двигатель", "" )
  PARM_D( Omega_Inv, "Обратная проходимость", 1. )
  PARM_D( _def_Omega_Razgerm, "def_Omega_Razgerm", 100. )
  PARM_B(bDefect, "Показать отказы", false)
	TAB("Дополнение",5)
//		PARM_D( Pow_Ro, "Степень плотности", 0.5 )
//    PARM( ST_1.Pow_Omega, "Зависимость напора от оборотов" );
//    PARM( ST_1.Pow_Omega_V, "Зависимость прив. расхода от оборотов" );
    PARM( ST_1.Thermo.T_out_max, "max Т выхода" );
//    PARM( ST_1.Thermo.T_out_min, "min Т выхода");
	ETAB
	TAB("Настройка",5)
    PARM ( ST_1.Fix_k_dP, "Фиксированный напор" );
    PARM_B( Fix_K, "Фиксировать напор", false )
    PARM_D( OborotZ, "Заданные обороты", 0. )
    PARM_D( Reductor, "Редуктор", 1. )
//    PARM_D( P_Out_Z, "Заданное давление на выходе(а)", 1. );
//    PARM_B( Use_P_Out_Z, "Использовать заданное давление", false );
#ifdef GET_PARAMS
    CTRL_Params( NULL, this );
#endif
    PARM( ST_1.Min_kdP, "Мин напора" )
    PARM( ST_1.Max_kdP, "Макс напора" )
    PARM( ST_1.Reg_kdP, "Регулировка напора" )
//    PARM_D( dP_Z, "Заданный перепад", 0. );
 //   PARM_D( F_In_Z, "Заданный F на входе нм3", 0. );
 //   PARM_D( kReg_PZ, "Регулировка заданного P out", 0. );
 	ETAB
ETAB
TAB("Блокировки",1)
	PARM_B( LocalBlk, "#Разрешить блокировку", false );
	PARM_B( ErrInBlk, "#Срабатывание блокировки", false );
	PARM( Err_BlkInObj, "#Срабатывание блокировки в ..." )
ETAB
//------------------------------------------------------
TAB("Расположение",1)
	PARM_S( Room, 32, "Помещение", "Установка" )
	PARM_S( SensRoom, 256, "Датчики", "" )
ETAB
