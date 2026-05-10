TAB("Режим", 1)
	PARM(Положение,"#Положение")
	PARM_B(Заглушка,"#Заглушка", false)
	PARM_D( P_In, "#Давление на входе", 0. );
	PARM_D( P_Out, "#Давление на выходе", 0. );
	PARM_D( dP, "#Перепад", 0. );
	PARM_D( F, "#Поток моль", 0. );
	PARM_B( IsOpen, "#Открыта", false );
ETAB
//
TAB("Параметры", 1)
	PARM_B( UsedInTuning, "Использовать при настройке", false )
	PARM ( NoUsed, "#Исключена" )
	PARM_B( ИсключитьИзСписка, "#Исключить из списка исключенных", false )
	PARM_D( К_Reg, "Кзагл", 1. );
	PARM_D( Dy, "Ду", 100. );
	PARM_B( GasGag, "Газовая заглушка", true );
	PARM_D( VentMin, "Утечка", 1e-7 )
	PARM_I( Type, "Тип", 0 );
	PARM_D( Mu, "Mu", 18. );
	PARM_D( Ro_liq, "Ro", 1000. );
	PARM_D( Ro_TC, "T для Ro", 20. );
	PARM_D( F_min_gas, "Минимальный газовый поток", 0.1 );
	PARM_B( bOpen, "Открыть (Игнорируется связь с пультом)", false );
ETAB

