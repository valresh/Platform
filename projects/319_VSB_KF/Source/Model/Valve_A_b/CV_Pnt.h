TAB("Режим", 1 )
	PARM( Type, "#Тип клапана")
	PARM(Position, "#Положение")
	PARM( Task, "#Задание" )
	PARM( _Differential, "#Перепад")
	PARM( _P_In, "#Давление на входе(и)")
	PARM( _P_Out, "#Давление на выходе(и)")
	PARM(_Flow_Work_nm3, "#Объемный расход при н.у.")
	PARM(_Flow_Work_m3, "#Объемный расход")
	PARM( _Flow_Work_kg, "#Массовый расход")	
	PARM(_F_mol, "#Мольный расход")
        PARM(bp, "#")
ETAB
TAB("Конструктив", 1 )
	PARM(Du ,"Условный диаметр")
	TAB("КИП", 2)
		PARM_LL( (unsigned int&)KIP, "#Без воздуха", "Открывается;Закрывается;Не меняется;Не влияет;" )		
		PARM( PminKIP, "Минимальное давление" )
		PARM( nKIP, "Номер секции КиП" )
	ETAB
	TAB("Электроэнергия", 2)
		PARM_LL ((unsigned int&)Electro, "#Без электроэнергии", "Открывается;Закрывается;Не меняется;Не влияет;")
	ETAB
ETAB
TAB("Параметры", 1 )
	PARM(K_Omega, "Коэффициент проходимости")
	PARM(Gas, "Газовая характеристика")
	PARM(Reverse, "Обратный клапан")
	PARM(Omega_Reverse, "Обратная проходимость")
	PARM(Leak, "Утечка");
	PARM_LL( Performance, "Тип характеристики", "Равнопроцентная;Линейная;Параболическая;")
	PARM(Tau_Open, "Время полного хода, с")
	PARM(min_procent, "Минимальный % при степенной характеристики");
	PARM(stepen, "Степень характеристики");
ETAB
TAB("Подбор проходимости", 1 )
	TAB ( $"Газ",LEV0)
		PARM( dP_Nom_Gas, $"Номинальный перепад" )
		PARM( P_Nom_Gas, $"Номинальное давление" )                 
		PARM( F_Nom_Gas, $"Номинальный расход" )      
		PARM( T_Nom_Gas, $"Номинальная температура" )                
		PARM( Mw_Nom_Gas, $"Номинальный молекулярный вес" )           
		PARM( Calc_Gas, $"#Взять параметры газа" )
	ETAB
	TAB( $"Жидкость",LEV0)
		PARM( dP_Nom_Liq, $"Номинальный перепад" )           
		PARM( F_Nom_Liq,  $"Номинальный расход, м3/ч" )         
		PARM( Ro_Nom_Liq, $"Номинальная плотность" )          
		PARM( Mw_Nom_Liq, $"Номинальный молекулярный вес" )            
		PARM( Calc_Liq,   $"#Взять параметры жидк" )
	ETAB 	
ETAB
TAB("Характеристики", 1 )
TAB("Отн. проходимости", 1)
	PARM_D( K_IO, "Отн. вх-вых", 10. )
	PARM_D( K_BP, "Отн. байпас", 2. )
ETAB
ETAB
TAB("Блокировки",1)
	PARM( Err_BlkInObj, "#Срабатывание блокировки в ..." )
	PARM( LocalBlk, "Локальные блокировки")
ETAB
TAB("Поле", 1)
	TAB("Задвижки", 1)
		PARM(Zadv_In, "#Вход")
		PARM(Zadv_Out, "#Выход")
		PARM(Zadv_Bypass, "#Байпас")
	ETAB
	PARM(bZacep, "#Зацепление")
	PARM(Dubler, "#Положение колеса дублера")
ETAB

