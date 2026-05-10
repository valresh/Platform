TAB("Режим", 1 )
//	PARM( pPos->VentZ, "#Открытие");
	PARM_D( Vent, "#Положение", 0. );
	PARM_D( _Flow_1, "#Поток o1(i1), кг", 0. );
	PARM_D( _Flow_2, "#Поток o2(i2), кг", 0. );
ETAB
TAB("Параметры", 1 )
	PARM_D( Omega, "Проходимость клапана", 10000. );
	PARM_D( Xo1_1, "Xo1_1", 0. );
	PARM_D( Yo1_1, "Yo1_1", 0. );
	PARM_D( Xo1_2, "Xo1_2", 100. );
	PARM_D( Yo1_2, "Yo1_2", 100. );
	PARM_D( Xo2_1, "Xo2_1", 0. );
	PARM_D( Yo2_1, "Yo2_1", 100. );
	PARM_D( Xo2_2, "Xo2_2", 100. );
	PARM_D( Yo2_2, "Yo2_2", 0. );
	PARM_D( Tau, "Время открытия, мин", 2. )
	PARM_B( Prty_1, "Приоритет соленоид = 1", true );
ETAB
TAB("Блокировки",1)
	PARM_B( LocalBlk, "#Локальные блокировки", false )
	PARM_B( ErrInBlk, "#Срабатывание блокировки", false )
	PARM( Err_BlkInObj, "#Срабатывание блокировки в ..." )
ETAB
