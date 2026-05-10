TAB("Режим", 1)
	PARM_D( dP, "#Перепад", 0. );
	PARM_D( F_kg, "#Поток кг", 0. );
	PARM_D( T_in, "#Т на входе", 0. );
	PARM_B( Connected,"#Подключен",false)
	PARM( Oborot, "#Обороты" );

#ifdef GET
	TAB("T на выходе", 1)
	for ( int n = 0; n < kSens; n++ )
	  {
	  pSens[n]->GetParams();
	  }
	ETAB
#endif
ETAB
//
TAB("Параметры", 1)
	PARM_D( Omega, "Проходимость", 100. );
	PARM_D( T_min, "Мин. Т в норме", 50.0 );
	PARM_D( T_max, "Макс. Т в норме", 60.0 );
	PARM_D( Тm, "Макс. Т", 120.0 );
	PARM_D( Cp, "Теплоемкость", 100.0 );
  PARM_I( _kSens, "#Число датчиков", 0 );
TAB("Температуры", 1)
  PARM_B( Set_Tz, "Разыграть температуры", false );
#ifdef GET
  for ( int n = 0; n < kSens; n++ )
    {
    Name.Prt ( "Ном Т %d", n + 1 );
    PARM(Tz[n], Name );
    }
#endif
ETAB
TAB( "Использовать номинал", 1)
  PARM_B( New, "Использовать номинальные значения", false );
  PARM_D( F_nom, "Номинальный поток, кг", 1000.0 );
  PARM_D( T_in_nom, "Номинальная Т охладителя", 30.0 );
  PARM_I( Set_Oborot, "-1-Выкл,1-Вкл,0-Как есть", 0 );
ETAB

TAB( "Использовать текущий режим", 1)
  PARM( New, "Использовать номинальные значения" );
	PARM_B( Set_Norm, "Взять текущий режим как норму", false );
PARM( W, "Мощность" );
#ifdef GET
for ( int n = 0; n < kSens; n++ )
  {
  Name.Prt ( "К-т трения %d", n + 1 );
  PARM(kTz[n], Name );
  }
#endif
ETAB
ETAB