TAB("Режим", 1 )
	STATE_PARAM(*pMotor->pOn, "#Пуск/Стоп" )
	PARM(*pMotor->pRelOborot, "#Относительные обороты")
	PARM(_P_In, "#Давление на входе(и), ат")
	PARM(_P_Out, "#Давление на выходе(и), ат")
	PARM( Head, "#Напор, ат" )
	PARM(F_mol, "#Мольный расход, кмоль/ч")
	PARM(_Flow_Work_m3, "#Объемный расход, м3/ч")
	PARM(_Flow_Work_kg, "#Массовый расход, кг/ч")
	PARM(E, "#Паросодержание" )
	STATE(bRest, "#Сброшен")
	PARM(power, "#Мощность, кВт" )
	PARM(ro, "#Плотность, кг/м3")
ETAB

TAB("Параметры", 1 )
	PARM_B( Use_Motor, "Связь с мотором", true )
	PARM_D(K_H, "Коэффициент напора", 1.0)
	PARM(K_F, "Коэффициент расхода")	
	PARM_B( _bRest, "Запрет сброса", false)
#ifdef GET_PARAMS
	if(_pMotor)
	{
		PARAM(_pMotor->Tau_On, "#Время набора оборотов, с")
		PARAM(_pMotor->Tau_Off, "#Время останова, с")
	}
#endif
	PARM_D( W_friction, "Мощность на потери, кВт", 0.0)
	PARM_D( W_inert, "Инерционность мощности, c", 3.0)
ETAB

TAB("Конструктив", 1)
	PARM_D( p_nom, "Номинальный напор", 10 )
	PARM_D( Q_nom, "Номинальный расход", 100 )
	PARM_D( p_max, "Максимальный напор", 1.14 * p_nom)
	PARM_D( Q_0, "#Расход при нулевом перепаде", 300.0)
	PARM_D( ro_nom, "Номинальная плотность", 1000.0)
ETAB

#ifdef GET_PARAMS
// if(_pMotor)
// {
// 	TAB("Двигатель", 1)
// 		_pMotor->GetParams(NULL);
// 	ETAB
// }
if(pCoolObj)
{
	TAB("Охдаждение", 1)
		pCoolObj->GetParams(NULL);
	ETAB
}
TAB("Блокировки",1)
	PARM( Err_BlkInObj, "#Срабатывание блокировки в ..." )
  if ( _pMotor )
	  PARM( _pMotor->LocalBlk, "Локальные блокировки" )
ETAB
#endif	
