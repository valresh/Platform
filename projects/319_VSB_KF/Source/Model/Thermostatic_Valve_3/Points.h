TAB("Режим",1)
	PARM( Position, "#Положение");
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
ETAB

TAB("Регулятор", 1)
	PARM_D( P, "P", 100. )
	PARM_D( I, "I", 10. )
	PARM_B( Inverse, "Инверсный", true )
ETAB

TAB("Поле", 1)
	PARM( Task, "#Задание")
ETAB

