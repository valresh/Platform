TAB("Режим", 1 )
	PARM( *pMotor->pOn, "#Пуск/Стоп" )
	PARM( *pMotor->pRelOborot, "#Относительные обороты")
	PARM( _P_In, "#Давление на входе(и)")
	PARM( Р_нагнетания.Value, "#Давление на выходе(и)" )
	PARM( Head, "#Напор" )
	PARM(F_mol, "#Мольный расход")
	PARM(_Flow_Work_m3, "#Объемный расход")
	PARM( _Flow_Work_kg, "#Массовый расход")
	PARM( Eps.E, "#Паросодержание" )
	PARM( Мощность.Value, "#Мощность" )
ETAB

TAB("Параметры", 1 )
	PARM(K_F, "Коэффициент расхода")
	//PARM( Задание.Value, "Задание")
	//PARM_D( Omega, "Проходимость", 0.0000001)
ETAB

TAB("Конструктив", 1)
	PARM_D( P_max, "Ограничение давления", 3.32)
	PARM_D( F_nom, "Объемный расход", 1.75)
  PARM_D( F_mass, "Массовый расход, кг/ч", 0 ) // Чтобы было легче отслеживать потери потока
ETAB

#ifdef GET_PARAMS
if(pCoolObj)
{
	TAB(pCoolObj->Model, 1)
		pCoolObj->GetParams(NULL);
	ETAB
}
#endif	 
TAB("Блокировки",1)
	PARM( Err_BlkInObj, "#Срабатывание блокировки в ..." )
	PARM( LocalBlk, "Локальные блокировки" )
ETAB

TAB("Отказы", 1)
	MODEL("Насосы", true)
	DEFECT(Исправен, "Исправен")
	DEFECT(Загазован, "Загазованность")
		DEF_D( def_Загазован, "Загазованость в %ПДК", 0., 60., 200. )
ETAB