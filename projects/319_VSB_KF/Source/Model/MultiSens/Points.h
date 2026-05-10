TAB("Режим", 1)
	PARM( On, "Включен" )
	PARM( Oborot, "Обороты" )
ETAB
TAB("Параметры", 1)
	PARM_D( V_Krit, "Крит. скорость", 0.45 )
	PARM_D( Max_Vibration, "Макс вибрация", 1. )
	PARM_D( Max_Shift, "Макс осевой сдвиг", 1. )
	PARM_D( Tau, "Время изменения час", 0.1 )
	PARM_D( T_min, "Мин Т", 40. )
	PARM_D( T_max, "Макс Т", 60. )
ETAB
