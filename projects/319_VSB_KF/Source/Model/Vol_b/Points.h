TAB("Режим", 1 )
PARM_D( _dt, "#dt", 0. )
#ifdef SHOW
ShowP.GetParams( "#Давление", true );
#endif
PARM_D( _P, "#Давление(и), атм", 1. )
PARM_D( _T, "#Температура", 0. )//DESC("Температура газа и жидкости, C")
PARM( VolBase.OverFlow, "#Переполнена" )
PARM( P_Napor, "#Напор" )
PARM_D( P_summ0, "#Давление в напорном режиме", 0. )
PARM( VolBase.Summ_Flow_kg, "#Суммарный поток на вход, кг" )
PARM_D( dMdt, "#Изменение массы, кг", 0. )
PARM( VolBase.Lev_liq, "#Степень заполнения" )
PARM_D( _Lev_liq, "#Уровень жидкости, %", 0. )
B_2F
PARM_D( _Lev_liq_1, "#Уровень 1 фазы, %", 0. )
PARM_D( _Lev_liq_2, "#Уровень 2 фазы, %", 0. )
E_2F
PARM_D( _M_liq, "#Количество жидкости, кмоль", 0. )
PARM_D( _M_liq_kg, "#Масса жидкости кг", 0. )
PARM_D( _M_liq_dt, "#Изменение массы, кг/ч", 0. )
PARM_D( _F_summ_kg, "#Сумма потоков, кг", 0. )
PARM_D( _Pvap, "#Давление паров(и)", 1. )
PARM( VolBase.P_Pz, "#Равновесие паров и жидкости" )
PARM_B( bp, "#", false )
ETAB
#ifdef SHOW
if ( VolBase.pReactions )
  VolBase.pReactions->GetParams ( this );
#endif
TAB("Конструктив", 1 )
PARM_D( Vol, "#Объем", 10. )
PARM_D( P_max, "Предельное давление(и)", 400. )  //??
ETAB

TAB("Параметры", 1 )
PARM_B( InitVol, "#Инициализация", false )
PARM_L_S( Тип, "Тип", "Горизонтальный цилиндр;Вертикальный цилиндр;Сфера;" )
PARM( VolBase.VolZ, "#Заданный объем" )
PARM( VolBase.Gas_Vol, "Газовый объем" )
PARM( VolBase.Liq_Vol, "Жидкий объем" )
PARM_D( k_Вес, "Поправка к весу", 1. )
PARM_D( Tara_Вес, "Вес тары", 0. )
PARM( Calc_2F, "#Учитывать расслоение воды и органики" )
PARM_D( H_Vol, "Высота (диаметр) емкости", 2. )
PARM_D( Stable_P, "Стабилизация давления", 1. )  //??
PARM( T_min, "#Минимальная T" )
PARM( T_max, "#Максимальная T" )
PARM( VolBase.kToAtm, "Связь с атмосферой" )
PARM( VolBase.SetOmegaL, "Сжатие газа жидкостью" );
PARM( VolBase.D_trub, "Отн. диаметр труб" );
PARM( VolBase.NoFunnel, "Нет воронки при сливе снизу" );
PARM_D( Slow_P, "Фильтр давлений", 0.1 );
PARM_D( k_Cond, "Скорость конденсации", 1.0 );
//PARM_D( Press_Z, "#Давление на выходах", 0. );
PARM_D( OmegaGas, "Проходимость перетока по газу", 1.0 );
PARM_D( OmegaLiq, "Проходимость перетока по жидкости", 1.0 );
PARM_D( KsiFlow, "Торможение потока", 1.0 );
PARM_D( Lev_Peretok, "Уровень перетока", 0.6 );
PARM( VolBase.Add_Comp, "#Залить заданный состав, кг" )
PARM( VolBase.Add_Comp_Name, "Заливаемый состав" )
PARM( VolBase.Add_Comp_T, "Температура заливаемого" )
PARM( VolBase.K_F1, "#Увеличить фазу 1" )
PARM( VolBase.K_F2, "#Увеличить фазу 2" )
PARM( State, "#Состояние" )
//TAB("Реакции", 1 )
//PARM_U( Reaction_DLL, "#Блок моделирования", "" )
//PARM_U( Reaction_Type, "#Реакции", "" )
//ETAB
B_2F
TAB("2 фазы", 1 )
PARM( VolBase.Type2F, "Тип 2Ф: 0-только вода, 1-Все компоненты" )
PARM( VolBase.Vol2F, "Объем отстойника" )
TAB("Параметры для Тип 2Ф = 0", 1 )
PARM( VolBase.Eps_W_in_O, "Содержание воды в органике, %%" )
PARM( VolBase.Eps_O_in_W, "Содержание органики в воде, %%" )
ETAB

#ifdef SHOW
TAB( "Параметры для Тип 2Ф = 1", 1 )
Char<128>Name;
for ( int c = 0; c < LAST_GAS; c++ )
  {
  Name.Prt ( "Отн. растворимость %s в воде (G1/G2)", pGas[c]->Name );
  PARM( VolBase.G1_G2[c], Name )
  }
ETAB
#endif

PARM( VolBase.Add_1F, "#Добавить фазу 1, кг" )
PARM( VolBase.Add_2F, "#Добавить фазу 2, кг" )
PARM_D( X_2F, "X индикатора воды", 0.1 )
PARM_D( Y_2F, "Y индикатора воды", 0.1 )
PARM_D( W_2F, "W индикатора воды", 0.1 )
PARM_D( H_2F, "H индикатора воды", 0.8 )
ETAB
E_2F

TAB("Статический режим", 1 )
PARM_B( Static_Calc_Mix, "Расчет равновесного состава", false )
PARM_B( Fast_T, "Быстрый расчет температуры", false )
//
TAB("Давление", 1 )
PARM_B( Static_Fix_P, "Фиксировать давление", false )
PARM_D( Static_P_fiz, "Фиксированное давление(и)", 0. )
PARM_B( Static_Iter_P, "Определить давление", false )
PARM_D( Static_dP_Fix_P, "#Дисбаланс давлений выхода газов", 0. )
PARM_L( Static_Out_gas, "Место вывода газа", NodeNames )
ETAB

TAB("Уровень", 1 )
PARM_B( Static_Fix_L, "Фиксировать уровень", false )
PARM_L( Static_Out_1F, "Место вывода 1-ой(единственной) фазы", NodeNames )
PARM_L( Static_Out_2F, "Место вывода 2-ой фазы", NodeNames )
PARM_D( Static_dP_Fix_L_1, "#Дисбаланс давлений вывода 1 фазы", 0. )
PARM_D( Static_dP_Fix_L_2, "#Дисбаланс давлений вывода 2 фазы", 0. )
ETAB
ETAB

ETAB
TAB("Уровнемер", 5 )
PARM_D_S( Масштаб, "Масштаб уровнемера", 100. )
PARMS( VolBase.Lev_0, "Высота низа уровнемера" )
PARMS( VolBase.Lev_100, "Высота верха уровнемера" )
PARM_D( Shift_Lev, "Базовый уровень", 0. )
PARM_D( X_lev, "X индикатора органики", 0.8 )
PARM_D( Y_lev, "Y индикатора органики", 0.1 )
PARM_D( W_lev, "W индикатора органики", 0.1 )
PARM_D( H_lev, "H индикатора органики", 0.8 )
ETAB

#ifdef SHOW
CTRL_Params( "Задания", this );
#endif
TAB("Регулировка уровня потоком",1)
PARM_B ( LevCtrl_Flow, "Регулировать уровень", false )
#ifdef SHOW
if ( 1 )
  {
  Char<128>Name;
  for ( int n = 0; n < kIO ; n++ )
    {
    int N = pNodes[n].nIO;
    if ( N < 0 )
      continue;
    if ( pNodes[n].Type != 'X' )
      continue;
    if ( pNodes[n].pOP == NULL )
      continue;
    Name.Prt ( "Выход %s.%s (%s)", pNodes[n].pOP->ObjName.Str, pNodes[n].pOP->PntName.Str, pNodes[n].pOP->pPool->Name.Str );
    PARM( pNodes[N].UseForCtrlLev, Name );
    }
  }
#endif
PARM_D ( dFlow_Lev, "#Дисбаланс потоков", 0. )
ETAB

#ifdef SHOW
TAB("Подключения", 5 )
TAB("Гидростат фактический", 5 )
for ( int n = 0; n < kNodes; n++ )
  {
  int N = pNodes[n].nIO;
  if ( N < 0 )
    continue;
  sprintf_s ( Name, sizeof(Name), "#%s - Гидростат фактический", (char*)pNodes[n].pOP->PntName );
  PARM( dP_Hydro[N], Name );
//  sprintf_s ( Name, sizeof(Name), "%s - гидростат(L)", (char*)pNodes[n].pOP->PntName );
//  PARM( dPHydro[N], Name );
//  sprintf_s ( Name, sizeof(Name), "%s - стат. гидростат", (char*)pNodes[n].pOP->PntName );
//  PARM( dPGydro_Static[N], Name );
  }
ETAB
TAB("Устойчивость", 5 )
for ( int n = 0; n < kNodes; n++ )
  {
  int N = pNodes[n].nIO;
  if ( N < 0 )
    continue;
  sprintf_s ( Name, sizeof(Name), "%s - устойчивость", (char*)pNodes[n].pOP->PntName );
  PARM( pNodes[n].Slow, Name );
  }
ETAB
TAB("Потоки", 5 )
PARM_L( Type_Flow, "Параметр потока", "Поток, кг;Поток, кмоль;Температура;Доля газа;Перепад;" )
CParams::GetLast().pShowParam = &Show_Flow;
for ( int n = 0; n < kNodes; n++ )
  {
  int N = pNodes[n].nIO;
  if ( N < 0 )
    continue;
  sprintf_s ( Name, sizeof(Name), "#%s ", (char*)pNodes[n].pOP->PntName );
  PARM( pFlowProps[N], Name );
  }
ETAB
//////////////////////////////////////////////////////
TAB("Проходимости", 5 )
for ( int n = 0; n < kNodes; n++ )
  {
  int N = pNodes[n].nIO;
  if ( N < 0 )
    continue;
  sprintf_s ( Name, sizeof(Name), "%s - проходимость", (char*)pNodes[n].pOP->PntName );
  PARM( Omega[N], Name );
  }
ETAB
TAB("Высоты", 5)
for ( int n = 0; n < kNodes; n++ )
  {
  int N = pNodes[n].nIO;
  if ( N < 0 )
    continue;
  sprintf_s ( Name, sizeof(Name), "%s - высота входа", pNodes[n].pOP->PntName.Str );
  PARM( VolBase.H_node[N], Name );
  }
ETAB
TAB("Подключения", 5)
for ( int n = 0; n < kNodes; n++ )
  {
  int N = pNodes[n].nIO;
  if ( N < 0 )
    continue;
  sprintf_s ( Name, sizeof(Name), "%s - подключение к", pNodes[n].pOP->PntName.Str );
  PARM_L( VolBase.Vol_Conn[N], Name,"емкости;отстойнику;перетоку газа;перетоку жидкости;" );
  }
ETAB
TAB("Гидростат задание", 5)
for ( int n = 0; n < kNodes; n++ )
  {
  int N = pNodes[n].nIO;
  if ( N < 0 )
    continue;
  sprintf_s ( Name, sizeof(Name), "%s - Постоянный гидростат", (char*)pNodes[n].pOP->PntName );
  PARM( dPGydro_Static[N], Name );
  }
for ( int n = 0; n < kNodes; n++ )
  {
  int N = pNodes[n].nIO;
  if ( N < 0 )
    continue;
  sprintf_s ( Name, sizeof(Name), "%s - Гидростат(L)", (char*)pNodes[n].pOP->PntName );
  PARM( dPHydro[N], Name );
  }
ETAB
ETAB
//
//////////////////////////////////////////////////////////////////////
if ( UseHeat[0] || UseHeat[1] || UseHeat[2] )
  {
  TAB("Змеевики", 5 )
  if ( UseHeat[0]  )
    {
    TAB("Змеевик 1", 5 )
    PARM( Heat.T_in, "#Т на входе" )
    PARM( Heat.T_out, "#Т на выходе" )
    PARM( Q_Heat_1, "#Тепловой поток, МВт" )
    PARM( Omega_Heat, "Проходимость змеевика" )
    PARM( k_Heat, "Коэффициент теплопередачи" )
    PARM( S_Heat, "Площадь змеевика, м2" )
    PARM( Heat.Lev_Zm, "Высота верха змеевика" )
    PARM( Cond_Heat, "Конденсация на змеевике" )
    PARM( Heat.k_Cp, "Коэффициент сходимости" )
	PARM( Heat.Inverse_Lev, "Чем ниже уровень, тем больше теплоотдача");
    ETAB
    }
  if ( UseHeat[1]  )
    {
    TAB("Змеевик 2", 5 )
    PARM( Heat_2.T_in, "#Т на входе" )
    PARM( Heat_2.T_out, "#Т на выходе" )
    PARM( Q_Heat_2, "#Тепловой поток, МВт" )
    PARM_D( Omega_Heat_2, "Проходимость нагревателя 2", 1000. )
    PARM( k_Heat_2, "Коэффициент теплопередачи" )
    PARM( S_Heat_2, "Площадь змеевика, м2" )
    PARM( Heat_2.Lev_Zm, "Высота верха змеевика" )
    PARM( Cond_Heat_2, "Конденсация на змеевике" )
    PARM( Heat_2.k_Cp, "Коэффициент сходимости" )
	PARM( Heat_2.Inverse_Lev, "Чем ниже уровень, тем больше теплоотдача");
    ETAB
    }
  if ( UseHeat[2]  )
    {
    TAB("Змеевик 3", 5 )
    PARM( Heat_3.T_in, "#Т на входе" )
    PARM( Heat_3.T_out, "#Т на выходе" )
    PARM( Q_Heat_2, "#Тепловой поток, МВт" )
    PARM_D( Omega_Heat_3, "Проходимость нагревателя 3", 1000. )
    PARM( k_Heat_3, "Коэффициент теплопередачи" )
    PARM( S_Heat_3, "Площадь змеевика, м2" )
    PARM( Heat_3.Lev_Zm, "Высота верха змеевика" )
    PARM( Cond_Heat_3, "Конденсация на змеевике" )
    PARM( Heat_3.k_Cp, "Коэффициент сходимости" )
	PARM( Heat_2.Inverse_Lev, "Чем ниже уровень, тем больше теплоотдача");
    ETAB
    }
  ETAB
  if ( UseHeater )
    {
    TAB("Электрообогрев", 5 )
    PARM( WorkHeat, "#Обогрев включен" )
    PARM( Q_Heat, "Мощность нагревателя, кВт" )
    ETAB
    }
  }
B_2F
  TAB("2 фазы", 5)
    PARM ( VolBase.M_Liq_1, "#Количество фазы 1, кмоль" )
    PARM ( VolBase.M_Liq_2, "#Количество фазы 2, кмоль" )
    PARM ( VolBase.M_Liq_1_kg, "#Масса фазы 1, кг" )
    PARM ( VolBase.M_Liq_2_kg, "#Масса фазы 2, кг" )
    PARM ( VolBase.V_Liq_1, "#Объем фазы 1, м3" )
    PARM ( VolBase.V_Liq_2, "#Объем фазы 2, м3" )
  ETAB
E_2F
TAB("Составы", 5)
TAB("Жидкость/Газ", 5)
PARM_L( CompLiqGas, "Среда", "Жидкость;Газ;Для расчета;" )
CParams::GetLast().pShowParam = &Show_Comp;
PARM_L( CompMolMass, "Выводить", "Моль %;Масс  %;" )
for ( int n = 0; n < LAST_GAS; n++ )
  {
  if ( VolBase.IsComp[n] )
    {
    sprintf_s ( Name, sizeof(Name), "#%s", (char*)pGas[n]->Name );
    PARM ( ShowComp[n], Name );
    }
  }
ETAB
B_2F
  TAB("2 фазы", 5)
  PARM_L( Comp12, "Фаза", "Органика;Вода;" )
  CParams::GetLast().pShowParam = &Show_2F;
  PARM_L( Comp2FMolMass, "Выводить как", "Моль %;Масс  %;" )
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( VolBase.IsComp[n] )
      {
      sprintf_s ( Name, sizeof(Name), "#%s", (char*)pGas[n]->Name );
      PARM( ShowComp2F[n], Name );
      }
    }
//
  ETAB
E_2F
TAB("Растворимость", 5)
for ( int n = 0; n < LAST_GAS; n++ )
  {
  sprintf_s ( Name, sizeof(Name), "Henry %s", pGas[n]->Name );
  PARM( VolBase.StateVol.Henry[n], Name );
  }
ETAB
ETAB

TAB("Баланс",5)
lstrcpy ( CompNames, "Не рассчитывать;");
kNumbCompNames = 1;
NumbCompNames[0] = -1;
int P = lstrlen ( CompNames );
for ( int c = 0; c < LAST_GAS; c++ )
  {
  if ( VolBase.IsComp[c] )
    {
    NumbCompNames[kNumbCompNames++] = c;
    int S = lstrlen ( pGas[c]->Name );
    if ( P + S + 2 > 1024 )
      break;
    memcpy ( CompNames + P, pGas[c]->Name, S );
    P += S;
    CompNames[P++] = ';';
    CompNames[P] = 0;
    }
  }
PARM_L( nCompBalance, "#Баланс по компоненте", CompNames )
PARM( VolBase.Balance, "#Баланс выбранной компоненты" )
PARM( VolBase.Balance_S, "#Суммарный баланс массы" )
PARM( VolBase.Balance_H_S, "#Суммарный баланс массы энергии" )
ETAB

#endif

#ifdef SHOW
if ( ShowAllParams || CParams::SaveAllParams )
  {
#endif

TAB("Дополнения", 5 )
//..................................................................
PARM( VolBase.Залить, "#Залить" )
PARM_S( KlapReg, 32, "Регулировочный клапан", "" )
PARM_L( numbKlapReg, "Выход регулировки уровня", NodeNames )

PARM_D( Q_add, "Подогрев, МДж/ч", 0. )

PARM_D( kT_wall, "Охлаждение стенки", 0.8 )
PARM( Heat.Alfa, "Положение точки в середине змеевика" );


TAB("Регулировки", 5 )
PARM_D( _Sliv, "#Слив кг", 0. )
PARM_I( nFlow_lev_out, "N выхода с потоком по уровню", -1 )
PARM_D( Flow_lev_out, "Поток при уровне 50% моль", 1000. )
ETAB
//////////////////////////////////////////////////////////
#ifdef SHOW
TAB("Закупорка", 5)
for ( int n = 0; n < kNodes; n++ )
  {
  int N = pNodes[n].nIO;
  if ( N < 0 )
    continue;
  sprintf_s ( Name, sizeof(Name), "%s - закупорка", pNodes[n].pOP->PntName.Str );
  PARM( UseTopIn[N], Name );
  }
ETAB
TAB("Направление", 5)
for ( int n = 0; n < kNodes; n++ )
  {
  int N = pNodes[n].nIO;
  if ( N < 0 )
    continue;
  sprintf_s ( Name, sizeof(Name), "%s - направление", pNodes[n].pOP->PntName.Str );
  PARM( Направление[N], Name );
  }
ETAB
ETAB
  }
#endif
TAB("Напор",2 )
PARM_B( CommIO, "#Объединенные входы", false )
//PARM( OverFlow, "#Переполнен" )
PARM( P_Napor, "#Реальный напор" )
PARM_D( kP_Napor, "Скорость изменения напора", 1. )
PARM_B( Use_Flow_Balance, "Напор из баланса потоков", false )
PARM_D( k_Flow, "Скорость изменения потоков", 1. )
PARM_D( k_napor, "Фильтр колебаний напора", 1. )
PARM_L( nFlow_summ, "Выход при переполнении", NodeNames )
//PARM_D( P_Summ, "#Сумма без гидростата", 0. )
//PARM_D( Lev_max_napor, "Уровень напора", 0.9 )
//PARM_D( P_max_napor, "Давление напора", 10. )
PARM_D( Lev_max_hydro, "Уровень закупорки", 0.9 )
PARM_B( Use_Lev_max_hydro, "Закупорка", false )
//
PARM_B( Use_Lev_hydro, "Учитывать естественный гидростат", true )
ETAB
TAB("Расчет давлений",5)
PARM_D( Calc_T, "Расчетная температура", 15.)
PARM_D( Calc_P, "Расчетное давление(и)", 0. )
#ifdef SHOW
for ( int n = 0; n < LAST_GAS; n++ )
  {
  if ( VolBase.IsComp[n] )
    {
    Sprintf ( Name, "Расчет %s", pGas[n]->Name );
    PARM( Calc_C[n], Name );
    }
  }
#endif
PARM_B( Gas_Calc_C, "Это газ", false )
PARM_B( Norm_Calc_C, "#Нормировать состав", false )
ETAB
