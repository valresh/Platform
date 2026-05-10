//---------------------------------------------
TAB("Режим", 1 )
    PARM_B( IsWork, "Работает", true )
    PARM_D( _F_kg, "#Подача кг", 0. )
	PARM_D( _T_in, "#Т на входе", 0. ) 
	PARM_D( _T_out, "#Т на выходе", 0. ) 
	PARM_D( _T_air_out, "#Т воздуха на выходе", 0. ) 
	PARM_D( _dP, "#Перепад", 0. ) 
	PARM_D( _Oborot, "Обороты двигателя", 1. ) 
	PARM_D( _Угол_атаки, "#Угол атаки лопастей, % макс", 0. ) 
	PARM_D( _Жалюзи, "#Угол открытия жалюзей, % макс", 0. ) 
	PARM_B( LocalStatic, "Статика", false ) 
	PARM( Jalousie_Eff,"#Степень открытия жалюзей")
	PARM( P_cond, "#Давление конденсации(а)") 
	PARM( Eps_cond, "#Степень конденсации" ) 
	PARM( Jalousie_1,"#Жалюзи 1")
	PARM( Jalousie_2,"#Жалюзи 2")
	TAB("Дополнение", 5 )
		PARM_D( _h_in, "#h на входе", 0. ) 
		PARM_D( _h_out, "#h на выходе", 0. ) 
		PARM_D( _Lam, "#Lam", 0. ) 
		PARM_D( _Lam_Air, "#Lam воздуха", 0. ) 
		PARM_D( _Lam_Opt, "#Lam оптимальная", 0. ) 
		PARM_D( Q_cal, "#Теплопередача Гкал/ч", 0. )
	ETAB
	TAB("Перепад", 5 )
		PARM_B( dP_Eps, "Зависимость перепада от конденсации", false ) 
		PARM_B( dP_Lin, "Линейная зависимость", true ) 
		PARM_D( Alfa_Omega, "Тормоз проходимости", 0.02 ) 
		PARM_D( Flow_Nom, "Номинальный поток", 0. ) 
		PARM_D( U, "#Путь конденсации", 0. )
		PARM( kOmega, "#Изменение перепада" )
		PARM_D( dP_cond, "Перепад конденсации", 0.5 ) 
	ETAB
	TAB("Жалюзи", 5 )
		PARM_D( Жалюзи_зад, "Заданное положение жалюзей", 50. ) 
		PARM_B( Use_Жалюзи_зад, "Задать положение жалюзей", false ) 
		PARM_B( Use_T_cam, "Регулировка Т в камере", false ) 
		PARM_D( Alfa_Jal_0, "Смешение при закрытых жалюзях, %", 50. ) 
		PARM_D( Alfa_Jal_100, "Смешение при открытых жалюзях, %", 100. ) 
	ETAB
	TAB("Дренажи", 1 )
		PARM_D( VentDrainIn, "#Дренаж на входе", 0. )
		PARM_D( VentDrainOut, "#Дренаж на выходе", 0. )
		PARM_D( _F_drain_in, "#Поток дренажа входа моль", 0. ) 
		PARM_D( _F_drain_out, "#Поток дренажа выхода моль", 0. ) 
		PARM_I( TypeDrainIn, "#Вход дренаж(1:газ/-1:жидк)", 0 )
		PARM_I( TypeDrainOut, "#Выход дренаж(1:газ/-1:жидк)", 0 )
	ETAB
ETAB
//---------------------------------------------
TAB("Параметры", 1 )
  PARM_B( OldHeat, "Старая модель", true )
	PARM_D( S, "Площадь", 100. )
	PARM_D( K, "Теплоотдача", 20. )
	PARM_B( Конденсация, "Конденсация", false )
	//PARM_D( Pow_Omega, "Зависимость теплоотдачи от потока", 1. )
	PARM_B( Use_Pow_Omega, "Учет зависимости теплоотдачи от потока", false )
	TAB("Воздух", 1  )
		PARM_D( F_Air_kg, "Подача воздуха кг", 500000. ) 
		PARM_D( Pow_Air, "Зависимость воздуха от оборотов", 2. )
		PARM_D( F_Air_XX, "Относительная подача при выключении", 0.02 ) 
		PARM_D( F_Air_Jal, "Отн. подача при Jal=0", 0.8 ) 
		PARM_B( Imp_Ctrl_Jal, "Импульсное управление жалюзями", true ) 
	ETAB
	TAB("Подогреватель воздуха",3)
		PARM_D( Sh, "Площадь подогревателя", 100. )
		PARM_D( Kh, "Теплоотдача подогревателя", 100. )
		PARM_D( OmegaSteam, "Проходимость по пару", 25. )
	ETAB
	TAB("Вода",3)
		PARM_D( F_Water_Max, "Максимальный поток воды кг", 100. )
		PARM_D( k_Water_Max, "Максимальное влияние воды", 2. )
		PARM_D( OmegaWater, "Проходимость по воде", 25. )
	ETAB
	//
	TAB("Доп параметры", 5 )
		PARM_D_S( M_Corp, "Масса корпуса т", 1. )
		PARM_D_S( Ksi_out, "Время остывания", 6. )
		PARM_D( Angle_0, "Жалюзи:Эффективность угла 0", 0.5 )
		PARM_D(   Time_Jal, "Жалюзи:Время открытия, с", 120. )
		PARM_D( Omega, "Проходимость", 1e4 )
	ETAB
ETAB
#ifdef GET_PARAMS
if(kMotor > 0)
{
	TAB("Моторы",3)
	Char<64>Name;
	for ( int n = 0; n < kMotor ;n++ )
	{    
		if ( pSrc[n] )
		{
			Name.Prt("#Мотор%d", n + 1 );
			PARM( pSrc[n], Name );
		}
	}
	ETAB
}
#endif
TAB("Блокировки",1)
	PARM_B( LocalBlk, "#Локальные блокировки", false )
	PARM_B( ErrInBlk, "#Срабатывание блокировки", false )
	PARM( Err_BlkInObj, "#Срабатывание блокировки в ..." )
ETAB
