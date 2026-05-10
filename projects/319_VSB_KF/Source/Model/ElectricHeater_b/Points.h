TAB("Режим", 1 )
	PARM(State, "#Состояние")
	PARM_D(Flow_i, "#Поток", 0.0)
	PARM_LL((unsigned int&)eSwitch, "#Мест-Дист", "Дистанционный;Местный;")
	PARM_B(bPuskStop, "#Пуск/Стоп", false)
	PARM(T_i, "#Температура на входе")
	PARM(T_o, "#Температура на выходе")
	PARM_D(P_i , "#Давление на входе", 1.0)
	PARM_D(P_o , "#Давление на выходе", 1.0)
	PARM_D(dP, "#Перепад", 0.0)
	PARM(In.E , "Паросодержание на входе")
	PARM(Out.E , "Паросодержание на входе")
ETAB
TAB("Конструктив", 1)
	PARM_D(Power, "Мощность", 10.0)
	PARM_D(U, "Напряжение, кВ", 0.4)
	PARM_D(TauHeat, "Инерционность, с", 20.0)
ETAB
TAB("Параметры", 1 )
	PARM_D( Omega, "Проходимость", 10000.0);
ETAB
TAB("Подключение АСУ", 1)
	PARM(Задание.IsConnection, "#Задание")
	PARM(Запрет_пуска.IsConnection, "#Запрет пуска")
	PARM(Пуск.IsConnection, "#Пуск")
	PARM(Стоп.IsConnection, "#Стоп")
	PARM(Пуск_Стоп.IsConnection, "#Пуск/Стоп")
ETAB
TAB("Блокировки",1)
	PARM( Err_BlkInObj, "#Срабатывание блокировки в ..." )
	PARM_B( LocalBlk, "Локальные блокировки", false)
ETAB
