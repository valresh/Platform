TAB("Режим", 1 )
	PARM(Position, "#Положение")
	PARM_D( _Differential , "#Перепад", 0.0)
	PARM_D(_F_mol, "#Мольный расход", 0.0)
	PARM_D( _Flow_Work_kg, "#Массовый расход", 0.0)
	PARM_D(_Flow_Work_m3, "#Объемный расход", 0. )
	PARM_D(_Flow_Work_nm3, "#Объемный расход при н.у.", 0.0)
	PARM_D( _P_In, "#Давление на входе(и)", 0.0)
	PARM_D( _P_Out, "#Давление на выходе(и)", 0.0 )
	PARM_S( Type, 16, "#Тип арматуры", "")
ETAB
TAB("Конструктив", 1 )
	PARM_D(Du ,"Условный диаметр", 100.0)
ETAB
TAB("Параметры", 1 )
	PARM_D(K_Omega, "Коэффициент проходимости", 1.0)
	PARM_B(Gas, "Газовая характеристика", false )
	PARM_B(Reverse, "Обратный клапан", false)
	PARM_D(Omega_Reverse, "Обратная проходимость", 1e-7)
	PARM_D(Leak, "Утечка", 0.0);
	PARM_D(min_procent, "Минимальный % при степенной характеристики", 4.0);
	PARM_D(stepen, "Степень характеристики", 2.0);
ETAB
TAB("Подбор проходимости", 1 )
	TAB ( $"Газ",LEV0)
		PARM_D( dP_Nom_Gas, $"Ном_dP_газ", 1.0 )
		PARM_D( P_Nom_Gas, $"Ном_P", 10. )                 
		PARM_D( F_Nom_Gas, $"Ном_F_нм3_газ", 10000. )      
		PARM_D( T_Nom_Gas, $"Ном_T_C", 20. )                
		PARM_D( Mw_Nom_Gas, $"Ном_Mw_газ", 10. )           
		PARM_B( Calc_Gas, $"#Взять параметры газа", false )
	ETAB
	TAB( $"Жидкость",LEV0)
		PARM_D( dP_Nom_Liq, $"Ном_dP_жидк", 1.0 )
		PARM_D( F_Nom_Liq,  $"Ном_F_м3_жидк", 100. )         
		PARM_D( Ro_Nom_Liq, $"Ном_Ro_жидк", 1000. )          
		PARM_D( Mw_Nom_Liq, $"Ном_Mw_жидк", 18. )            
		PARM_B( Calc_Liq,   $"#Взять параметры жидк", false )
	ETAB 	
ETAB
TAB("Характеристики", 1 )
	PARM_L( Performance, "Характеристика", "Равнопроцентная;Линейная;Параболическая;")
ETAB


