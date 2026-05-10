#include "Queue.h"
#include "../Lang.h"
//
enum SendType
{
	None = 0,
	mPress,
	mZacepka,
	m_bIsManual,
	m_bZadaniePlus,
	m_bZadanieMinus,
	mAutoMode,
	m_bOpen,
	nResetOpenClose,
	m_bIsOpen,
	m_bIsClose,
	m_b0Lamp,
	m_b100Lamp,
	m_b1_99Lamp,
	m_bIsMufta,
	m_bOpen5Sec,
	Vent_Dubler,
	Vent_Dubler_Auto,
	Vent,
	m_bZacepka
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура работы с задвижкой
struct  Queue_KlapST : public Queue_PRS
{
	CShKlapan sh;
	bool mPress;
	bool mAutoMode;
	int  mZacepka;
	bool m_bOpen;
	bool m_bStopor;
	bool m_bStopBaipas;
	bool m_bStopIn;
	bool m_bStopOut;
	bool m_bIsOpen;
	bool m_bIsClose;
	bool m_bIsMufta;
	int nHS;
	bool bState;
	int nSend;
	bool bSend;
	bool m_bStoporKoleso;
	bool m_bIsManual;
	bool m_bIsSend;
	bool m_bStopControlSP;
	int nResetOpenClose;
	int nKlapanStop;
	bool m_b0Lamp;
	bool m_b100Lamp;
	bool m_b1_99Lamp;
	bool  m_bOpen5Sec;
	bool  m_bZadaniePlus;
	bool  m_bZadanieMinus;
	double m_dStep;
	bool bPipe;
	bool bViewInvert;
	int  m_PrevZacepka; //предыдущее состояние зацепления
	double dDelta;      //между значением положения дублера и реальным положнием - вычисляется в момент зацепления 
	SPriborBase::MINMAX m_MinMax; //значения пределов для PribDouble c учетом dDelta для коррекции пределов вращния
	SendType sendType;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static char cszZacepl_Dubler[] = "Зацепленный дублёр";

int PaintKlapST(DefinePaint)
{
	QueueP(Queue_KlapST);
//    if (p->nHS >= 0)
//    {
//        CShHS& Sh = (CShHS&)p->pPipe->SH(p->nHS);
//        if (Sh.RetCode == CShBase::rcOK)
//        {
//            p->bState = p->bViewInvert ? !Sh.mOn : Sh.mOn;
//        }
//    }
//
//    if (p->nKlapanStop >= 0)
//    {
//        CShKlapan& Sh = (CShKlapan&)p->pPipe->SH(p->nKlapanStop);
//        if (Sh.VentZ <= 0)
//		{
//			p->m_bStopControlSP = true;
//		}
//        else
//		{
//			p->m_bStopControlSP = false;
//		}
//    }
//
//    if (p->nSend >= 0)
//    {
//        CShHS& Sh = (CShHS&)p->pPipe->SH(p->nSend);
//        if (Sh.RetCode == CShBase::rcOK)
//		{
//			p->bSend = Sh.mOn;
//		}
//    }
//
    CShKlapan* sh = (CShKlapan*)p->pPipe->Sh(p->nNumber);
	if (!p->m_bPressed)
	{
		if (p->nSocketNumber != -1)
		{
			sh = (CShKlapan*)p->pSocket->Sh(p->nSocketNumber);
		}

		if (IsTypeOk)
		{
			memcpy(&p->sh, sh, sizeof(CShKlapan));
		}
	}
//            p->mAutoMode = false;
//            p->m_bStopor = ((!p->sh.Is_Ctrl(CKlapan::KLAPAN_MANUAL)) || (p->sh.Is_Ctrl(CKlapan::KLAPAN_HYDRAVLIC)));
//            p->m_bStopBaipas = p->sh.Is_State(CKlapan::BAIPAS_BROKEN);
//            p->m_bStopIn = p->sh.Is_State(CKlapan::IN_BROKEN);
//            p->m_bStoporKoleso = !p->sh.Is_Ctrl(CKlapan::KLAPAN_MANUALKOLESO);
//            p->m_bStopOut = p->sh.Is_State(CKlapan::OUT_BROKEN);
//            p->m_bIsOpen = p->sh.Is_State(CKlapan::IS_OPEN);
//            p->m_bIsClose = p->sh.Is_State(CKlapan::IS_CLOSE);
//            p->m_bIsMufta = p->sh.Is_State(CKlapan::IS_MUFTA);
//            p->m_b0Lamp = (p->sh.Vent == 0);
//            p->m_b100Lamp = (p->sh.Vent == 100);
//            p->m_b1_99Lamp = (!p->m_b0Lamp) && (!p->m_b100Lamp);
//            p->mZacepka = p->sh.m_bZacepka;
//            p->m_bOpen5Sec = p->sh.Is_Ctrl(CKlapan::OPEN_5SEC_TIMER);
//            p->m_bIsManual = p->sh.Is_Ctrl(CKlapan::KLAPAN_MANUALKOLESO);
//        }
//    }
//
//    if (strcmp(p->send.szValue, cszZacepl_Dubler) == 0)
//    {
//        p->mZacepka = p->sh.m_bZacepka;
//        if (p->mZacepka)
//        {  
//			//здесь при зацеплении вычисляется Delta между положением дублера и реальным положением 
//            if (p->mZacepka != p->m_PrevZacepka)
//            {
//                p->dDelta = p->sh.Vent_Dubler - p->sh.Vent;
//            }
//
//            p->m_MinMax.min = 0. + p->dDelta;
//            p->m_MinMax.max = 100. + p->dDelta;
//        }
//        else
//        {
//            p->m_MinMax.min = -1000000.;
//            p->m_MinMax.max = 1000000.;
//        }
//
//        p->m_PrevZacepka = p->mZacepka;
//    }
//    //
    return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void fnSendData(void* base, void* pVoid, void* pOldValue, bool bMessage)
{
	ASSERT(base);
	Queue_KlapST* p = (Queue_KlapST*)base;
	if (!ISTypeOK)
	{
		return;
	}

	bool bSend = true;
	// Работа ручной помпы
	if (p->sendType == SendType::mPress && strcmp(p->send.szValue, "Гидравлическая помпа") == 0)
	{
		// Moved to C# !!!
		bSend = false;
		/*if (p->sh.ePumpCode == CKlapan::enumNeitral || p->sh.Vent_OilB >= 100.)
		{
			bSend = false;
		}
		else
		{
			double Old = p->sh.Vent;
			double New = Old;
			double step = 1. - 0.01 * p->sh.Vent_OilB;
			if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
				step *= 10;
			}

			if (p->sh.ePumpCode == CKlapan::enumOpen)
			{
				New = min(100., Old + step);
			}
			else
			{
				New = std::max(0.00, Old - step);
			}

			p->send.Set(enumValueDbl, &Old, &New);
		}*/
	}
	// Если нет зацепления крутилка не работает
	// ???
	else if (p->sendType == SendType::Vent_Dubler && strcmp(p->send.szValue, cszZacepl_Dubler) == 0)
	{
		bSend = p->sh.m_bZacepka;
		if (bSend)
		{
			p->m_MinMax.min = 0. + p->dDelta;
			p->m_MinMax.max = 100. + p->dDelta;
			p->send.SetNew(pVoid);
			if (((p->sh.Vent >= 100.0) && (p->send.dNew > p->send.dOld)) ||
				((p->sh.Vent <= 0.0) && (p->send.dNew < p->send.dOld)))
			{
				bSend = false;
			}
		}
		else
		{
			p->m_MinMax.min = -1000000.;
			p->m_MinMax.max = 1000000.;
			if (p->sh.Vent != p->send.dOld)
			{
				// The first assignment moved to C#
				//*((double*)pVoid) = p->send.dNew = p->sh.Vent;
				p->send.dNew = p->sh.Vent;
				bSend = true;
				bMessage = false;
			}
		}

		if ((bSend) && (p->bSend))
		{
			if (p->nSocketNumber != -1)
			{
				if (p->send.eType == enumValueDbl)
				{
					//здесь в каждой из переменных szValue и szValueOld посылаем по ДВА значения разделенных ;
					//первое для изменения положения дублера в симуляторе, второе для записи в журнал (с коррецией 
					//на dDelta для для приведения к требуемому положению клапана)
					double dValue = *(double*)pVoid;
					char szValue[32], szValueOld[32];
					double dtrueNew = dValue - p->dDelta;
					double dOld = p->send.dOld - p->dDelta;
					if (dtrueNew < 1.)
					{
						dtrueNew = 0.;
					}

					if (dtrueNew > 99.)
					{
						dtrueNew = 100.;
					}

					sprintf_s(szValue, "%0.5f;%0.5f", dValue, dtrueNew);
					sprintf_s(szValueOld, "%0.5f;%0.5f", p->send.dOld, dOld);
					const char* szField = p->pSocket->GetModelPseudonym(id_Klapan, p->send.szValue);
					p->pSocket->SetParam(p->send.szName, szField, p->send.szValue, szValue, szValueOld, bMessage, id_Klapan, p->send.eType);
					bSend = false; //здесь послали - не посылать ниже
				}
			}
		}
	}
	else if (p->sendType == SendType::mZacepka && strcmp(p->send.szValue, "Зацепление") == 0)
	{
		bool Old = p->sh.m_bZacepka;
		bool New = p->mZacepka != 0;
		p->send.Set(enumValueBol, &Old, &New);
		bSend = bMessage;
		bMessage = true; //для сброса p->m_bPressed в конце - т.к. в PaintKlapST() по !p->m_bPressed заполняется реальное положение
	}
	else if (p->send.eType == enumValueInt && strcmp(p->send.szValue, "Дистанц.") == 0)
	{
		bSend = bMessage;
		bMessage = true; //для сброса p->m_bPressed в конце - т.к. в PaintKlapST() по !p->m_bPressed заполняется реальное положение
	}
	else if (p->sendType == SendType::m_bIsManual)
	{
		bool Old = p->sh.Is_Ctrl(CKlapan::KLAPAN_MANUALKOLESO);
		bool New = p->m_bIsManual;
		p->send.Set(enumValueBol, &Old, &New);
	}
	else if (p->sendType == SendType::m_bZadaniePlus)
	{
		if (p->m_bZadaniePlus)
		{
			double Old = p->sh.VentZ;
			double New = p->sh.VentZ + p->m_dStep;
			if (New > 100)
			{
				New = 100;
			}

			p->send.Set(enumValueDbl, &Old, &New);
		}
	}
	else if (p->sendType == SendType::m_bZadanieMinus)
	{
		if (p->m_bZadanieMinus)
		{
			double Old = p->sh.VentZ;
			double New = p->sh.VentZ - p->m_dStep;
			if (New < 0)
			{
				New = 0;
			}

			p->send.Set(enumValueDbl, &Old, &New);
		}
	}
	else if (p->sendType == SendType::mAutoMode && strcmp(p->send.szValue, "Мода") == 0)
	{
		char Old = 0;
		BYTE New = (p->mAutoMode) ? SModePole::AUT : SModePole::MAN;
		p->send.Set(enumValueChr, &Old, &New);
	}
	else if (p->sendType == SendType::m_bOpen && strcmp(p->send.szValue, "Откр/Закр") == 0)
	{
		if (p->nSocketNumber != -1)
		{
			double dValue = 1;
			char szValue[128];
			if (p->m_bOpen)
			{
				strcpy(szValue, "Открыть");
			}
			else
			{
				strcpy(szValue, "Закрыть");
			}

			p->pSocket->SetSocketValue(id_Klapan, p->send.szName, szValue, dValue, p->send.dOld, bMessage, p->send.eType);
			bSend = false;
		}
		else
		{
			bool Old = false;
			bool New = true;
			p->send.Set(&Old, &New);
			if (p->m_bOpen)
			{
				strcpy(p->send.szValue, "Открыть");
			}
			else
			{
				strcpy(p->send.szValue, "Закрыть");
			}

			p->pPipe->SendData(p->send, bMessage);
			strcpy(p->send.szValue, "Откр/Закр");
			bSend = false;
		}
	}
	else if (p->sendType == SendType::nResetOpenClose && strcmp(p->send.szValue, "СбросОткр/Закр") == 0)
	{
		bool Old = false;
		bool New = true;
		p->send.Set(&Old, &New);
		if (p->nResetOpenClose == 1)
		{
			strcpy(p->send.szValue, "Открыть");
			if (p->nSocketNumber != -1)
			{
				if (p->send.eType == enumValueInt)
				{
					int nValue = 1;
					p->send.nOld = 0;
					p->pSocket->SetSocketValue(id_Klapan, p->send.szName, p->send.szValue, nValue, p->send.nOld, bMessage, p->send.eType);
				}
			}
			else
			{
				p->pPipe->SendData(p->send, bMessage);
			}
		}
		else if (p->nResetOpenClose == 2)
		{
			strcpy(p->send.szValue, "Закрыть");
			if (p->nSocketNumber != -1)
			{
				if (p->send.eType == enumValueInt)
				{
					int nValue = 1;
					p->send.nOld = 0;
					p->pSocket->SetSocketValue(id_Klapan, p->send.szName, p->send.szValue, nValue, p->send.nOld, bMessage, p->send.eType);
				}
			}
			else
			{
				p->pPipe->SendData(p->send, bMessage);
			}
		}

		strcpy(p->send.szValue, "СбросОткр/Закр");
		bSend = false;
	}
	else if (strcmp(p->send.szValue, "Открыть") == 0 ||
		strcmp(p->send.szValue, "Закрыть") == 0 ||
		strcmp(p->send.szValue, "Остановить") == 0)
	{
		bool Old = false;
		bool New = true;
		p->send.Set(&Old, &New);
		p->send.SetNew(pVoid);
	}
	// Если нет зацепления крутилка не работает
	// ???
	else if (p->sendType == SendType::Vent && strcmp(p->send.szValue, "Зацепленный шибер") == 0)
	{
		bSend = p->sh.m_bZacepka && p->sh.Is_Ctrl(CKlapan::KLAPAN_MANUALKOLESO);
		if (bSend)
		{
			p->send.SetNew(pVoid);
		}
	}
	// ???
	else if (p->sendType == SendType::m_bZacepka && strcmp(p->send.szValue, "Зацепление шибера") == 0)
	{
		bSend = p->sh.Is_Ctrl(CKlapan::KLAPAN_MANUALKOLESO);
		if (bSend)
		{
			p->send.SetNew(pVoid);
		}
	}
	else if (strcmp(p->send.szValue, "Открыть5сек") == 0)
	{
		int New = *(int*)pVoid;
		int Old = (New == 0);
		p->send.Set(enumValueInt, &Old, &New);
	}
	else
	{
		p->send.SetNew(pVoid);
	}
	// ???
	if (p->sendType == SendType::Vent_Dubler_Auto && strcmp(p->send.szValue, "Дублёр Автозацеп") == 0)
	{
		double dublerNewValue = *(double*)pVoid;
		SSendToModel sendZacep;
		bool New = true;
		if ((bMessage) && (dublerNewValue <= 0.9))
		{
			New = false;
		}

		bool Old = !New;
		sendZacep.Set(enumValueBol, &Old, &New);
		strcpy(sendZacep.szValue, "Зацепление");
		strcpy(sendZacep.szName, p->send.szName);
		sendZacep.eData = id_Klapan;
		p->pPipe->SendData(sendZacep, false);
	}

	if ((bSend) && (p->bSend))
	{
		if (p->nSocketNumber != -1)
		{
			if (p->send.eType == enumValueDbl)
			{
				double dValue = *(double*)pVoid;
				p->pSocket->SetSocketValue(id_Klapan, p->send.szName, p->send.szValue, dValue, p->send.dOld, bMessage, p->send.eType);
			}
			else if (p->send.eType == enumValueInt)
			{
				int nValue = *(int*)pVoid;
				p->pSocket->SetSocketValue(id_Klapan, p->send.szName, p->send.szValue, nValue, p->send.nOld, bMessage, p->send.eType);
			}
			else if (p->send.eType == enumValueBol)
			{
				bool bValue = *(bool*)pVoid;
				p->pSocket->SetSocketValue(id_Klapan, p->send.szName, p->send.szValue, bValue, p->send.bOld, bMessage, p->send.eType);
			}
		}
		else
		{
			p->pPipe->SendData(p->send, bMessage);
		}
	}

	if (strcmp(p->send.szValue, "КонтрольСостояния") == 0)
	{
		int Old = CKlapan::Ctrl_State;
		int New = CKlapan::Ctrl_State;
		p->send.Set(enumValueInt, &Old, &New);
		p->pPipe->SendData(p->send, false);
	}
	
	p->m_bPressed = !bMessage;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FillKlapan(SPriborBase* d, void* pp)
{
	d->fnData = fnSendData;
	Queue_KlapST* p = (Queue_KlapST*)pp;
	if (p->nHS >= 0 && d->nType == 1)
	{
		SPriborBool* a = (SPriborBool*)d;
		a->pbOther = &p->bState;
		if ((strcmp(p->send.szValue, "Задание+") == 0) || (strcmp(p->send.szValue, "Задание-") == 0))
		{
			a->pbBlock = &p->bState;
		}
	}
	// Маленькие нюансы
	if (d->pVoid == &p->sh.Vent && strcmp(p->send.szValue, "Гидравлическая помпа") == 0)
	{
		d->pVoid = &p->mPress;
		p->sendType = SendType::mPress;
	}
	else if (strcmp(p->send.szValue, "Мода") == 0)
	{
		d->pVoid = &p->mAutoMode;
		p->sendType = SendType::mAutoMode;
	}
	else if (d->pVoid == &p->sh.m_bZacepka && strcmp(p->send.szValue, "Зацепление") == 0)
	{
		d->pVoid = &p->mZacepka;
		p->sendType = SendType::mZacepka;
	}
	else if (d->pVoid == &p->sh.Ctrl_Flags && strcmp(p->send.szValue, "Откр/Закр") == 0)
	{
		d->pVoid = &p->m_bOpen;
		p->sendType = SendType::m_bOpen;
	}
	else if (d->pVoid == &p->sh.Ctrl_Flags && strcmp(p->send.szValue, "СбросОткр/Закр") == 0)
	{
		d->pVoid = &p->nResetOpenClose;
		p->sendType = SendType::nResetOpenClose;
	}
	else if (d->pVoid == &p->sh.State_Flags && strcmp(p->send.szValue, "Открыт") == 0)
	{
		d->pVoid = &p->m_bIsOpen;
		p->sendType = SendType::m_bIsOpen;
	}
	else if (d->pVoid == &p->sh.State_Flags && strcmp(p->send.szValue, "Закрыт") == 0)
	{
		d->pVoid = &p->m_bIsClose;
		p->sendType = SendType::m_bIsClose;
	}
	else if (d->pVoid == &p->sh.Vent && strcmp(p->send.szValue, "0Лампа") == 0)
	{
		d->pVoid = &p->m_b0Lamp;
		p->sendType = SendType::m_b0Lamp;
	}
	else if (d->pVoid == &p->sh.Vent && strcmp(p->send.szValue, "100Лампа") == 0)
	{
		d->pVoid = &p->m_b100Lamp;
		p->sendType = SendType::m_b100Lamp;
	}
	else if (d->pVoid == &p->sh.Vent && strcmp(p->send.szValue, "1_99Лампа") == 0)
	{
		d->pVoid = &p->m_b1_99Lamp;
		p->sendType = SendType::m_b1_99Lamp;
	}
	else if (d->pVoid == &p->sh.State_Flags && strcmp(p->send.szValue, "Муфта") == 0)
	{
		d->pVoid = &p->m_bIsMufta;
		p->sendType = SendType::m_bIsMufta;
	}
	else if (d->pVoid == &p->sh.Ctrl_Flags && strcmp(p->send.szValue, "ManualKoleso") == 0)
	{
		d->pVoid = &p->m_bIsManual;
		p->sendType = SendType::m_bIsManual;
	}
	else if (d->pVoid == &p->sh.Ctrl_Flags && strcmp(p->send.szValue, "Открыть5сек") == 0)
	{
		d->pVoid = &p->m_bOpen5Sec;
		p->sendType = SendType::m_bOpen5Sec;
	}
	else if (d->pVoid == &p->sh.VentZ && strcmp(p->send.szValue, "Задание+") == 0)
	{
		d->pVoid = &p->m_bZadaniePlus;
		p->sendType = SendType::m_bZadaniePlus;
	}
	else if (d->pVoid == &p->sh.VentZ && strcmp(p->send.szValue, "Задание-") == 0)
	{
		d->pVoid = &p->m_bZadanieMinus;
		p->sendType = SendType::m_bZadanieMinus;
	}
	//Показания HC
	if (d->nType == 4 && strcmp(p->send.szValue, "Показания") == 0)
	{
		SPriborArrow* a = (SPriborArrow*)d;
		a->pdVal = &p->sh.VentZ;
	}

	if (strcmp(p->send.szName, "ПоворотБлок") == 0)
	{
		d->pbBlock = &p->m_bStopor;
	}

	if (strcmp(p->send.szValue, "ЗаданиеБлок") == 0)
	{
		d->pbBlock = &p->m_bStoporKoleso;
	}

	if (strcmp(p->send.szName, "Байпас") == 0)
	{
		d->pbBlock = &p->m_bStopBaipas;
	}

	if (strcmp(p->send.szName, "Вход") == 0)
	{
		d->pbBlock = &p->m_bStopIn;
	}

	if (strcmp(p->send.szName, "Выход") == 0)
	{
		d->pbBlock = &p->m_bStopOut;
	}

	if (strcmp(p->send.szValue, "Поворот_С_Блокировкой") == 0)
	{
		d->pbBlock = &p->m_bStopControlSP;
	}

	if (strcmp(p->send.szValue, cszZacepl_Dubler) == 0)
	{
		d->pMinMax = &p->m_MinMax;
	}

	// New code fo C#
	if (d->pVoid == &p->sh.Vent_Dubler && strcmp(p->send.szValue, cszZacepl_Dubler) == 0)
	{
		p->sendType = SendType::Vent_Dubler;
	}

	if (d->pVoid == &p->sh.Vent && strcmp(p->send.szValue, "Зацепленный шибер") == 0)
	{
		p->sendType = SendType::Vent;
	}

	if (d->pVoid == &p->sh.m_bZacepka && strcmp(p->send.szValue, "Зацепление шибера") == 0)
	{
		p->sendType = SendType::m_bZacepka;
	}

	if (d->pVoid == &p->sh.Vent_Dubler && strcmp(p->send.szValue, "Дублёр Автозацеп") == 0)
	{
		p->sendType = SendType::Vent_Dubler_Auto;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddKlapST(DefineTegs)
{
	AddQueueEx(Queue_KlapST);
	//
	CShKlapan sh;
	memcpy(&p->sh, &sh, sizeof(CShKlapan));
	p->pSH = &p->sh;
	//
	// Код, посылаемый в АРМ
	p->btSendType = QUEUE_NO;
	// Определение функции вывода
	p->paint = &PaintKlapST;
	p->nHS = -1;
	char szHS[64] = "";
	char szSF[64] = "";
	p->nSend = -1;
	p->bSend = true;
	p->m_bIsSend = false;
	char szSend[64] = "";
	p->m_bStopControlSP = false;
	char szKlapanStop[64] = "";
	p->nKlapanStop = -1;
	p->m_b0Lamp = false;
	p->m_b100Lamp = false;
	p->m_b1_99Lamp = false;
	p->m_bOpen5Sec = false;
	p->m_bZadaniePlus = false;
	p->m_bZadanieMinus = false;
	p->m_dStep = 0;
	p->bViewInvert = false;
	p->nSocketNumber = -1;
	p->m_MinMax.min = 0.;
	p->m_MinMax.max = 100.;
	p->dDelta = 0.;
	p->bPipe = false;
	p->sendType = SendType::None;
	//
	STegObj obj[] =
	{
	  { 'S', "view", szHS, 63 },
	  { 'S', "send", szSend, 63 },
	  { 'S', "stop", szKlapanStop, 63 },
	  { 'S', "suff", szSF, 63 },
	  { 'D', "step" ,  &p->m_dStep },
	  { 'B', "viewinvert", &p->bViewInvert},
	  { 'B', "pipe", &p->bPipe},
	  // Дополнительная информация
	  EXT_QUEUE
	};
	::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));
	if (*szSF)
	{
		int nActive = def.GetActiveObjSocket();
		const char* name = def.GetModelName(nActive);
		char szName[128];
		sprintf_s(szName, sizeof(szName), "%s%s", name, szSF);
		def.AddKind(id_Klapan, szName);
		def.m_queue.m_pSocket->Refresh();
	}

	if (*szHS)
	{
		p->nHS = def.AddKind(id_HS, szHS);
	}

	if (*szSend)
	{
		p->nSend = def.AddKind(id_HS, szSend);
	}

	if (*szKlapanStop)
	{
		p->nKlapanStop = def.AddKind(id_Klapan, szKlapanStop);
	}

	HPRIBOR hPribor;
	if (p->bPipe)
	{
		hPribor = AddPriborsPipe(def, teg, Buffer, p, &FillKlapan, id_Klapan);
	}
	else
	{
		hPribor = AddPribors(def, teg, Buffer, p, &FillKlapan, id_Klapan);
	}

	if (hPribor == NULL)
	{
		Buffer.nPtr -= sizeof(Queue_KlapST);
		memset(Buffer.szQueue + Buffer.nPtr, 0, sizeof(Queue_KlapST));
	}

	managed::RegisterElement(teg->name, p, hPribor);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
