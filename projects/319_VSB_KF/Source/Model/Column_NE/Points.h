TAB("Режим",1)
PARM ( ReplaceParams, "ReplaceParams" )
PARM ( ParamFrame, "ParamFrame" )
PARM_D ( P_верха, "#Давление верха(и)", 0. )
PARM_D ( P_Top_Z, "Заданное давление верха(и)", 0. )
PARM_D ( T_верха, "#Температура верха", 0. )
PARM_D ( P_низа, "#Давление куба(и)", 0. )
PARM_D ( T_низа, "#Температура куба", 0. )
PARM_D ( L_куб, "#Уровень в кубе (истинный), %", 0. )
PARM_D ( Lev_CubZ, "Заданный уровень в кубе, %", 0. )
PARM ( L_уровнемер, "#Показания уровнемера, %" )
PARM_D ( Doliv, "#Долив, кг", 0. )
PARM_D ( Sliv, "#Слив, кг", 0. )
////////////////////////////////////////////////////////////////////////
PARM_I ( Comp_N1t, "Компонента 1 в газе вверху", -1 )
PARM_I ( Comp_N2t, "Компонента 2 в газе вверху", -1 )
PARM_I ( Comp_N1b, "Компонента 1 из куба", -1 )
PARM_I ( Comp_N2b, "Компонента 2 из куба", -1 )
#ifdef LIST_PARAMS
if ( Comp_N1t >= 0 && Comp_N1t < LAST_GAS )
  {
  Name.Prt ( "#Содержание %s в газе", pGas[Comp_N1t]->Name );
  PARM_D ( Cmol_N1t, Name, 0. )
  }
if ( Comp_N2t >= 0 && Comp_N2t < LAST_GAS )
  {
  Name.Prt ( "#Содержание %s в газе", pGas[Comp_N2t]->Name );
  PARM_D ( Cmol_N2t, Name, 0. )
  }
if ( Comp_N1b >= 0 && Comp_N1b < LAST_GAS )
  {
  Name.Prt ( "#Содержание %s в кубе", pGas[Comp_N1t]->Name );
  PARM_D ( Cmol_N1b, Name, 0. )
  }
if ( Comp_N2b >= 0 && Comp_N2b < LAST_GAS )
  {
  Name.Prt ( "#Содержание %s в кубе", pGas[Comp_N2t]->Name );
  PARM_D ( Cmol_N2b, Name, 0. )
  }

char * ValidType = "GLM";

TAB("Потоки, кг",1)
for ( int n = 0; n < kIO ; n++ )
  {
//  if ( strchr( ValidType, pNodes[n].Type ) == 0 )
//    continue;
  if ( Flow_kg )
    {
    if ( Flow_kg[n] >= 0 )
      Name.Prt ( "#Поток %s<-%s", pNodes[n].Name, pNodes[n].PipeName );
    else
      Name.Prt ( "#Поток %s->%s", pNodes[n].Name, pNodes[n].PipeName );
    PARM( Flow_kg[n], Name );
    }
  }
ETAB
TAB("Давления",1)
for ( int n = 0; n < kIO ; n++ )
  {
  if ( strchr( ValidType, pNodes[n].Type ) == 0 )
    continue;
  if ( Src == NULL || Src[n] == NULL  || Src[n]->pFlow == NULL )
    continue;
  if ( Src[n]->pFlow->Flow_mol >= 0 )
    Name.Prt ( "#Давление(а) %s<-%s", pNodes[n].Name, pNodes[n].PipeName );
  else
    Name.Prt ( "#Давление(а) %s->%s", pNodes[n].Name, pNodes[n].PipeName );
  PARM( Src[n]->pFlow->P, Name );
  }
ETAB
TAB("Температуры",1)
for ( int n = 0; n < kIO ; n++ )
  {
  if ( strchr( ValidType, pNodes[n].Type ) == 0 )
    continue;
  if ( Src == NULL || Src[n] == NULL  || Src[n]->pFlow == NULL )
    continue;
  if ( Src[n]->pFlow->Flow_mol >= 0 )
    Name.Prt ( "#Температура %s<-%s", pNodes[n].Name, pNodes[n].PipeName );
  else
    Name.Prt ( "#Температура %s->%s", pNodes[n].Name, pNodes[n].PipeName );
  PARM( Src[n]->Src.T, Name );
  }
ETAB
Column.GetParams( "Режим по тарелкам" );
#endif 
ETAB

TAB("Конструктив", 1 )
PARM_L ( ModelCond, "Модель конденсации", "В объеме;В слое;" )
PARM( Column.nCub, "#Число тарелок + куб" );
PARM_D ( Vol_Col, "Объем колонны", 40. )
//PARM( Column.P_max, "Макс. давление" )
//PARM( Column.P_min, "Мин. давление" )
//
//PROP( ColCommonGamma, "Модель коэффициентов активности" )
//PROP( ColAbsGamma, "Модель абсорбции" )
//PROP( ColReactions, "Модель химических реакций" )
PARM_B ( Refresh_Config, "#Обновить конфигурацию", false )
PARM_D ( Acc, "Ускорение", 1. )
#ifdef LIST_PARAMS
PARM_L ( Column.MesPress, "Размерность выводимых давлений", "атм;Па;КПа;МПа;мм в.ст.;мм рт.ст.;атм(а);Па(а);мм в.ст.(а);мм рт.ст.(а);");
#endif
ETAB

TAB("Параметры",1)
//PARM_D ( Omega_Gas, "Проходимость по газу", 10000. )
PARM_D ( Omega_Heat, "Проходимость теплообменника", 1000. )
PARM_B ( OK_IO, "Обратные клапана на входах", false )
PARM_B ( InitState, "#Инициализация", false )
PARM_D ( dLev_dt_max, "Макс скорость изменения уровня, %/ч", 0. );
PARM_D ( dVout_max, "Макс изменение потоков", 10000. );
PARM_D ( Lev_Opt, "Опт. уровень, %", 0. );
PARM_D ( Max_Flow_Add, "Макс. долив, кг", 0. );
//PARM_B ( WaterCol, "Водяная", false )
PARM_B ( Use_Lev_hydro, "Учитывать естественный гидростат", true )
PARM_D ( Slow_Press, "Торможение давления тарелок", 0.01 )
PROP_E( ControlLev, "Управление" )
PARM_I ( Repeats, "Число итераций", 1 )
PARM ( Column.Tau_Gas, "Время прогрева газа, мин" )
PARM ( Column.CalcTom3, "Пересчет плотности" )
PARM ( Column.Cond_Level, "Зависимость конденсации от уровня" )

TAB("Воздух", 1 )
PARM_S ( Air_in, 32, "Вход воздуха", "" );
PARM_D ( Flow_Air, "#Поток воздуха", 0 );
PARM_D ( Flow_Air_Nom, "Номинальный поток воздуха", 0 );
PARM_D ( k_Air_st, "Макс. снижение уровня пены", 5. );
PARM_D ( k_Air_L, "Макс. повышение скорости слива", 5 );
ETAB

TAB("Статический режим", 1 )
  PARM_B( Static_Calc_Mix, "Расчет равновесного состава", false )
  TAB("Давление", 1 )
  PARM_B( Static_Fix_P, "Фиксировать давление верха", false )
  PARM_D( Static_P_fix, "Фиксированное давление(и)", 0. )
  PARM_D( Static_dF_Fix_P, "#Дисбаланс давлений выхода газов", 0. )
  ETAB

  TAB("Уровень", 1 )
  PARM( Column.Static_Fix_L, "Фиксировать уровень на тарелках" )
  PARM_B( Static_Fix_L, "#Фиксированный уровень на тарелках", false )
  PARM_D( Static_dF_Fix_L, "#Дисбаланс потоков из куба", 0. )
  ETAB

  TAB("Температура", 1 )
  PARM ( Column.Static_Iter_T, "Итерационный расчет Т" )
  PARM ( Column.Fast_T, "Ускоренный расчет Т" )
  PARM ( Column.k_SlowT, "Скорость изменения температуры" );
  PARM ( Column.dT_med,  "Среднее приращение температуры" )
  PARM ( Column.dT_max,  "Максимальное приращение температуры" )
 // PARM ( Column.Slow_Fast_T, "Стабилизация расчета Т" )
  ETAB

ETAB

#ifdef LIST_PARAMS
Column.GetParams( "Main" );

TAB("Проходимости",1)
for ( int n = 0; n < kIO ; n++ )
  {
//  if ( pNodes[n].Type != 'G' && pNodes[n].Type != 'L' )
//    continue;
  if ( Src == NULL || Src[n] == NULL )
    continue;
  if ( pNodes[n].pOP->pPool == NULL )
    continue;
  Name.Prt ( "Проходимость %s (%s)", pNodes[n].Name, pNodes[n].PipeName );
  PARM( Src[n]->Omega, Name );
  }
ETAB

bool IsGasOut = false;
for ( int n = 0; n < kIO ; n++ )
  {
  if ( Src == NULL || Src[n] == NULL )
    continue;
  if ( Src[n]->Gas )
    {
    IsGasOut = true;
    break;
    }
  }
if ( IsGasOut )
  {
  TAB("Газовые выходы",1)

  for ( int n = 0; n < kIO ; n++ )
    {
  //  if ( pNodes[n].Type != 'G' && pNodes[n].Type != 'L' )
  //    continue;
    if ( Src == NULL || Src[n] == NULL )
      continue;
    if ( pNodes[n].pOP->pPool == NULL )
      continue;
    Name.Prt ( "Проходимость %s (%s)", pNodes[n].Name, pNodes[n].PipeName );
    PARM( Src[n]->Omega, Name );
    }
  ETAB
  }

TAB("Гидростат",1)
for ( int n = 0; n < kIO ; n++ )
  {
  if ( pNodes[n].Type == 'G' )
    continue;
  if ( pNodes[n].pOP->pPool == NULL )
    continue;
  if ( Src == NULL || Src[n] == NULL )
    continue;
  Name.Prt ( "Статический гидростат %s (%s)", pNodes[n].Name, pNodes[n].PipeName );
  PARM( Src[n]->dP_Hydro_Static, Name );
  Name.Prt ( "Гидростат от уровня %s (%s)", pNodes[n].Name, pNodes[n].PipeName );
  PARM( Src[n]->dP_Hydro_Level, Name );
  }
ETAB
if ( kHeatStage > 0 )
  {
  TAB("Нагреватели",1)
  for ( int n = 0; n < kHeatStage; n++ )
    {
    Name.Prt ( "Проходимость нагревателя %d", n + 1 );
    PARM( OmegaHeatStage[n], Name );
    }
  for ( int n = 0; n < kHeatStage; n++ )
    {
    Name.Prt ( "Теплоотдача нагревателя %d", n + 1 );
    PARM( KSHeatStage[n], Name );
    }
  ETAB
  }
TAB("Henry",5)
for ( int n = 0; n < LAST_GAS; n++ )
  {
  if ( kStep == 0 || Column.IsComp[n] )
    {
    Sprintf ( Name, "Henry %s", pGas[n]->Name );
    PARM_D( Column.Henry[n], Name, 1. );
    }
  }
ETAB
TAB("Alfa",5)
for ( int n = 0; n < LAST_GAS; n++ )
  {
  if ( kStep == 0 || Column.IsComp[n] )
    {
    Sprintf ( Name, "Alfa %s", pGas[n]->Name );
    PARM_D( Column.Alfa_Comp[n], Name, 1. );
    }
  }
ETAB
Column.GetParams( "Параметры по тарелкам" );
//
#endif
ETAB

TAB("Тарелки",1)
TAB("Общие параметры",1)
PARM_B( Сбросить, "#Сбросить", false )
#ifdef LIST_PARAMS
if ( Column.pStage )
  {
  for ( int n = 1; n <= kStage; n++ )
    {
    Name.Prt("#%s", Column.pStage[n]->Name );
    PARM( Column.pCommonStage[n], Name );
    }
  }
#endif
ETAB
PARM( Column.NumbShow, "Номер тарелки" )
#ifdef LIST_PARAMS
//PARM_L ( Column.ShowVariantParams, "Показывать", "Состав жидкости;Состав газа;" )
if ( !CParams::SaveAllParams && Column.pStage && Column.NumbShow >= 1 && Column.NumbShow <= kStage )
  Column.pStage[Column.NumbShow]->GetParams( NULL );
#endif
ETAB
TAB("Карманы",1)
#ifdef LIST_PARAMS
Char<64>TabName;
for ( int n = 0; n < kKarman; n++ )
  {
  CKarman * pKarman = ppKapmans[n];
  int nNode = pKarman->nNode;
  TAB(pKarman->Name,1)
  Name.Prt ( "#%%Уровень в кармане %s, %%", pNodes[nNode].Name );
  PARM(pKarman->Level, Name );
  Name.Prt ( "#Заданный уровень в кармане %s, %%", pNodes[nNode].Name );
  PARM(pKarman->LevZ, Name );
  Name.Prt ( "#Поток из кармана %s, кг", pNodes[nNode].Name );
  PARM(pKarman->F_Karman_Out_kg, Name );
  Name.Prt ( "#Дисбаланс потоков в кармане %s, кг", pNodes[nNode].Name );
  PARM(pKarman->dF_kg, Name );
  Name.Prt ( "#Гидростат в кармане %s", pNodes[nNode].Name );
  PARM(pKarman->HydroPress, Name );
//    
  Name.Prt ( "Объем кармана %s", pNodes[nNode].Name );
  PARM(pKarman->Vol, Name );
  Name.Prt ( "Макс. доля потока в карман %s", pNodes[nNode].Name );
  PARM(pKarman->Alfa_Karman, Name );
  Name.Prt ( "Скорость изменения гидростата кармана %s", pNodes[nNode].Name );
  PARM(pKarman->Eps_Press, Name );
  Name.Prt ( "Статический гидростат кармана %s", pNodes[nNode].Name );
  PARM(pKarman->pSrc->dP_Hydro_Static, Name )
  Name.Prt ( "Гидростат от уровня кармана %s", pNodes[nNode].Name );
  PARM(pKarman->pSrc->dP_Hydro_Level, Name )
  ETAB
  }
#endif
ETAB
TAB("Управление и контроль",5)
//PROP ( Use_LevCtrl, "Использовать регулятор уровня" )
//??TAB("Регулировка уровня в кубе",1)
//??PARM_B ( LevCtrl_Cub, "Регулировать уровень в кубе", false )
#if 0 //LIST_PARAMS
if ( LevCtrl_Cub )
  {
  for ( int n = 0; n < kIO ; n++ )
    {
    if ( pNodes[n].Type != 'L' )
      continue;
    if ( Src == NULL || Src[n] == NULL )
      continue;
    if ( Src[n]->nStage != kStage )
      continue;
    Name.Prt ( "Выход %s(%s)", pNodes[n].Name, pNodes[n].PipeName );
    PARM( Src[n]->UseForCtrlLev, Name );
    }
//  if ( pLevCtrl )
//    pLevCtrl->GetParams2( this );
  }
#endif
//??PARM_D ( dFlow_Lev_Cub, "#Дисбаланс потоков", 0. )
//??ETAB
#ifdef LIST_PARAMS
Column.GetParams(  "Контроль" );
#endif
ETAB

TAB("Расчет давлений",5)
PARM_D( Calc_T, "Расчетная температура", 15.)
PARM_D( Calc_P, "Расчетное давление(и)", 0. )
#ifdef LIST_PARAMS
for ( int n = 0; n < LAST_GAS; n++ )
  {
  if ( Column.IsComp[n] )
    {
    Sprintf ( Name, "Расчет %s", pGas[n]->Name );
    PARM( Calc_C[n], Name );
    }
  }
#endif
PARM_B( Norm_Calc_C, "#Нормировать состав", false )
PARM_B( Gas_Calc_C, "Это газ", false )
ETAB

TAB("Отказы",5)
MODEL("Колонны",true)
DEFECT(Восстановление,"Восстановлена")
DEFECT(Резгерметизация_верха,"Разгерметизация верха")
DEFECT(Резгерметизация_низа,"Разгерметизация низа")
PARM_D ( Omega_Top, "Проходимость при разгерметизации верха", 10000. )
PARM_D ( Omega_Bott, "Проходимость при разгерметизации низа", 10000. )
//DEFECT(Переполнение,"Переполнение+другой отказ")
ETAB
