//TAB("Дополнения",4)
PARM( Проблемы, "#Проблемы" );
PARM_I( N_BreakPoint, "#N тарелки останова", 0 );
//PARM_D ( Vol_Stage, "Объем тарелок", 1. )
PARM_D( k_GL, "Теплообмен газ-жидкость", 10000. )
PARM_D( Flow_src_min, "Минимальный поток источника", 0.01 )
PARM_D( Tau_sliv, "Время слива тарелок, мин", 2. )
//LONG
PARM_D( k_Cp, "Настройка Cp", 0.1 )
PARM_D( k_SlowT,  "Скорость изменения температуры", 0.01 )
PARM_D( dT_med,  "Среднее приращение температуры", 0.01 )
PARM_D( dT_max,  "Максимальное приращение температуры", 0.01 )
PARM_B( OutPureComp, "Выводить чистые составы", true )
//ELONG
PARM( T_min, "#Минимальная Т" )
PARM( T_max, "#Максимальная Т" )
PARM_D( dP_Hydro_Level_Cub, "Гидростат куба от уровня", 0. )
PARM_D( dP_Hydro_Static_Cub, "Гидростат куба статический", 0. )
PARM_D ( Tau_Gas, "Время прогрева газа, мин", 10. )
PARM_B( CalcTom3, "Пересчет плотностей", false )
//PARM ( CStage_NE::SlowCond, "Замедление конденсации" )
//PARM( Omega_Def, "Проходимость при разгерметизации")
//PARM ( kInertHeat, "Инерционность тепло-бмена" )
//ETAB
