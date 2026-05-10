TAB("Режим",1)
PARM_D( _P, "#Давление(и)", 0. );
PARM_D( _Flow_in, "#Поток на вход, нм3", 0. );
PARM_D( _Flow_gas_out, "#Поток газа на выход, нм3", 0. );
PARM_D( _Flow_liq_out, "#Поток жидкости на выход, кг", 0. );
PARM_D( _T_gas, "#Температура газа", 0. );
PARM_D( _Eps_inv_gas, "#Сухость газа", 0. );    
PARM_D( _T_liq, "#Температура жидкости", 0. );
PARM_D( _M_liq, "#Масса жидкости, кг", 0. );
PARM  ( M_Liq,  "#Количество жидкости, кмоль" );
PARM  ( Ro_Liq, "#Плотность жидкости" );
PARM  ( Mw_liq, "#Молекулярный вес жидкости" );
PARM_D( _Level, "#Уровень жидкости, %", 0. );
PARM_D( _F_liq, "#Поступление жидкости, кг/ч", 0. );
PARM_D( _dM_liq, "#Изменение массы, кг/ч", 0. );
PARM_D( P_Pz, "#Неравновесность", 0. );
//PARM_D( Lev_Opt, "Оптимальный уровень", 0. )
//PARM_S( KlapReg, 32, "Регулировочный клапан", "" )
//PARM_D( _F_out_liq, "#Выводится жидкости, кг", 0. );
TAB("Дополнение",4)
PARM_D( _dP_gas, "#Перепад по газу", 0. );
PARM_D( _dP_hydro, "#Фактический гидростат", 0. );
ETAB
ETAB
//
TAB("Конструктив",2)
PARM_D( Volume, "Объем", 10. )
PARM_D( P_max, "Предельное давление", 400. )
ETAB

TAB("Параметры",2)
PARM_D( Eps_Sep, "Качество сепарации, %", 100. )
PARM_D( Cp_сorp, "Теплоемкость корпуса", 100. )
PARM_D( K_GL, "Теплообмен газ-жидкость", 10000. )
PARM( T_min, "#Минимальная T" )
PARM( T_max, "#Максимальная T" )
PARM_D( Add_Comp, "#Залить заданный состав, кг", 0. )
PARM_S( Add_Comp_Name, 32, "Заливаемый состав", "Вода" )
PARM_D( Add_Comp_T, "Температура заливаемого", 15. )
PARM_D( Lev_Funnel, "Уровень воронки, %", 10. )
PARM_D( kVC, "Скорость конденсации", 0.1 )
ETAB

TAB("Уровнемер",2)
PARM_D( Масштаб, "Масштаб уровнемера", 100. )
PARM_D( Lev_1_0,"Датчик уровень 0", 0.1 )
PARM_D( Lev_1_100,"Датчик уровень 1", 0.9 )
ETAB

TAB("Подключения",2)
PARM_D( Omega_Gas, "Проходимость по газу", 100000. )
PARM_D( Omega_Liq, "Проходимость по жидкости", 1000. )
PARM_D( Hydro_C, "Постоянный гидростат", 0. )
PARM_D( Hydro_L, "Гидростат от уровня", 0.5 )
PARM_B( Close_Out_liq, "Закрывать слив при малом уровне", false )
ETAB
//PARM_D( Cp_Liq, "Теплоемкость жидкости", 10. )
//PARM_D( Flow_Level, "Поток для уровня = 50, кмоль", -1. );
//
TAB("Змеевики",2)
PARM( Heat.T_in,  "#Т на входе" )
PARM( Heat.T_out, "#Т на выходе" )
PARM( Heat.Q_MWt, "#Тепловой поток, МВт" )
PARM_D( Omega_Heat, "Проходимость нагревателя", 1000. )
PARM_D( K_heat, "Коэффициент теплопередачи", 100. )
PARM_D( S_heat, "Площадь змеевика", 20. )
PARM_D( H_heat, "Высота верха змеевика", 0.1 )
PARM( Heat.k_Cp, "Коэффициент сходимости" )
ETAB

#ifdef SHOW
Char<128>Name;
TAB("Составы", 5)
TAB("Жидкость", 5)
TAB("Мольные", 5)
for ( int n = 0; n < LAST_GAS; n++ )
  {
  if ( IsComp[n] )
    {
    Name.Prt( "#%s, моль %%", (char*)pGas[n]->Name );
    PARMU( X_mol[n], Name, &Use_mol );
    }
  }
ETAB
TAB("Массовые", 5)
for ( int n = 0; n < LAST_GAS; n++ )
  {
  if ( IsComp[n] )
    {
    Name.Prt( "#%s, масс %%", (char*)pGas[n]->Name );
    PARMU( X_mass[n], Name, &Use_mass );
    }
  }
ETAB
ETAB
TAB("Газ", 5)
TAB("Мольные", 5)
for ( int n = 0; n < LAST_GAS; n++ )
  {
  if ( IsComp[n] )
    {
    Name.Prt( "#%s, моль %%", pGas[n]->Name );
    PARMU( Y_mol[n], Name, &Use_mol );
    }
  }
ETAB
TAB("Массовые", 5)
for ( int n = 0; n < LAST_GAS; n++ )
  {
  if ( IsComp[n] )
    {
    Name.Prt( "#%s, масс %%", pGas[n]->Name );
    PARMU( Y_mass[n], Name, &Use_mass );
    }
  }
ETAB
ETAB
TAB("Растворимость", 5)
PARM( NoInLiq_N2, "Азот не растворяется" );
for ( int n = 0; n < LAST_GAS; n++ )
  {
  Name.Prt( "%s растворимость", pGas[n]->Name );
  PARM( Henry[n], Name );
  }
ETAB
ETAB
#endif

#ifdef SHOW
if ( ShowAllParams || CParams::SaveAllParams )
  {
#endif
TAB("Дополнение",4)
PARM_D( Ro_Liq, "Плотность жидкости", 1000. )
PARM( Mw_liq, "Mw жидкости" )
PARM_D( Lev_max, "Макс уровень", 1.1 );
PARM_D( Add_M_Water, "Добавить воды кг", 0. );
PARM_B( Get_Comp, "Взять состав со входа", false )
ETAB
#ifdef SHOW
  }
#endif
//
