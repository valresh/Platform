TAB("Режим",1)
PARM_D ( P_верха, "Давление верха(и)", 0. )
PARM_D ( T_верха, "Температура верха", 0. )
PARM_D ( P_низа, "Давление куба(и)", 0. )
PARM_D ( T_низа, "Температура куба", 0. )
PARM_D ( L_куб, "Уровень в кубе (истинный), %", 0. )
PARM_D ( Lev_CubZ, "Заданный уровень в кубе, %", 0. )
//PARM_B ( DialCalcPress, "Расчет давлений", false )

PARM_B ( InitState, "#Инициализация", false )
#ifdef SHOW
  TAB("Потоки, кг",1)
    for ( int n = 0; n < kIO ; n++ )
      {
      if ( pNodes[n].Type != 'G' && pNodes[n].Type != 'L' )
        continue;
      Name.Prt ( "#Поток %s(%s)", pNodes[n].Name, pNodes[n].PipeName );
      PARM( Flow_kg[n], Name );
      }
  ETAB
  TAB("Давления",1)
    for ( int n = 0; n < kIO ; n++ )
      {
      if ( pNodes[n].Type != 'G' && pNodes[n].Type != 'L' )
        continue;
      if ( Src == NULL || Src[n] == NULL )
        continue;
      Name.Prt ( "#P %s(%s)", pNodes[n].Name, pNodes[n].PipeName );
      PARM( Src[n]->pFlow->P, Name );
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
      Name.Prt ( "Меплоотдача нагревателя %d", n + 1 );
      PARM( KSHeatStage[n], Name );
      }
    ETAB
    }
  TAB("Температуры",1)
    for ( int n = 0; n < kIO ; n++ )
      {
      if ( pNodes[n].Type != 'I' && pNodes[n].Type != 'O' && pNodes[n].Type != 'G' )
        continue;
	    if ( Src == NULL || Src[n] == NULL )
		    continue;
      Name.Prt ( "#T %s(%s)", pNodes[n].Name, pNodes[n].PipeName );
      PARM( Src[n]->pFlow->T, Name );
      }
  ETAB
#endif
ETAB
TAB("Конструктив",1)
PARM( Column.nCub, "#Число тарелок + куб" );
PARM_B ( WaterCol, "Вода - основная фаза", false )
ETAB

TAB("Параметры",1)
PARM_D ( dP_hydro_stat, "Гидростат куба статический", 0. )
PARM_D ( dP_hydro_lev, "Гидростат куба от уровня", 0.1 )
PARM( Column.Lev_Voronka, "Уровень воронки" )
PARM( Column.Ksi_R, "Конденсация газа")
//PARM( Column.k_GL, "Теплоотдача парр-жидкость")
//PARM_D ( dP_hydro_lev_in, "Гидростат входа", 0.2 )
PARM_B ( OK_IO, "Обратные клапана на входах", false )
PARM( Column.Tau_cool, "Время охлаждения тарелок, мин" )
PARM( Column.Tau_sliv, "Время слива тарелок, мин" )
PARM( Column.Tau_gas, "Время нагрева газа, мин" )
PARM( Column.P_Top_Z, "Заданное давление верха(а)")
LONG
PARM_D ( Omega_Gas, "Проходимость по газу", 10000. )
PARM_D ( Omega_Heat, "Проходимость теплообменника", 1000. )
PARM_B ( Use_Lev_hydro, "Учитывать естественный гидростат", true )
PARM_D ( Omega_Top, "Проходимость при разгерметизации верха", 10000. )
PARM_D ( Omega_Bott, "Проходимость при разгерметизации низа", 10000. )
ELONG
#ifdef SHOW
  TAB("Проходимости",1)
    for ( int n = 0; n < kIO ; n++ )
      {
      if ( pNodes[n].Type != 'G' && pNodes[n].Type != 'L' )
        continue;
      Name.Prt ( "Проходимость %s(%s)", pNodes[n].Name, pNodes[n].PipeName );
      PARM( Omega[n], Name );
      }
  ETAB
  TAB("Гидростат",1)
    for ( int n = 0; n < kIO ; n++ )
      {
      if ( pNodes[n].Type != 'G' && pNodes[n].Type != 'L' )
        continue;
      Name.Prt ( "Гидростат %s(%s)", pNodes[n].Name, pNodes[n].PipeName );
      PARM( Hydro_dPmax[n], Name );
      }
  ETAB
PROF ///////////////////////////////////
  TAB("Дополнения",1)
    for ( int n = 0; n < kIO ; n++ )
      {
      if ( pNodes[n].Type != 'G' && pNodes[n].Type != 'L' )
        continue;
      if ( Src == NULL || Src[n] == NULL )
        continue;
      Name.Prt ( "kdH %s(%s)", pNodes[n].Name, pNodes[n].PipeName );
      PARM( Src[n]->kdH, Name );
      }
  ETAB
EPROF ///////////////////////////////////
#endif
ETAB

