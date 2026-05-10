#include "Queue.h"
#include "../Lang.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура работы с задвижкой
struct Queue_PumpST : public Queue_PRS
{
	CShPump sh;
	int   mPoly;
	DWORD nTime;
	bool mPress;
	bool mOn;
	bool mReady;
	bool bStopor;
	bool bMstAuto;
	bool bNoElectro;
	bool m_bStopIn;
	bool m_bStopOut;
	bool m_bNewAuto;//0-мануал 1-авто
	bool m_bIsOpen;
	bool m_bIsClose;
	int nHS;
	bool bState;
	bool m_bClose;
	bool m_bNoSendHs;
	int nNoSendHs;
	//
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
int PaintPumpST(DefinePaint)
{
	QueueP(Queue_PumpST);
	//
	/*if (p->nHS >= 0)
	{
		CShHS& Sh = (CShHS&)q.m_pPipe->SH(p->nHS);
		if (Sh.RetCode == CShBase::rcOK)
		{
			p->bState = Sh.mOn;
		}
	}
	if (p->nNoSendHs >= 0)
	{
		CShHS& Sh = (CShHS&)q.m_pPipe->SH(p->nNoSendHs);
		if (Sh.RetCode == CShBase::rcOK)
		{
			p->m_bNoSendHs = Sh.mOn;
		}
	}*/
	if (!p->m_bPressed)
	{
		CShPump* sh = (CShPump*)PipeSh;
		if (p->nSocketNumber != -1)
		{
			sh = (CShPump*)q.m_pSocket->Sh(p->nSocketNumber);
		}

		if (IsTypeOk)
		{
			memcpy(&p->sh, sh, sizeof(CShPump));
			p->mOn = sh->Eq_State(CPumpData::Work);
			p->mReady = sh->Eq_State(CPumpData::ReadyToStart);
			p->bStopor = sh->Eq_State(CPumpData::Stopor);
			p->bMstAuto = sh->Eq_State(CPumpData::AutoPusk);
			p->bNoElectro = false;// Для кислоты obj->Eq_State( CPumpData::Paketnik );
			p->m_bStopIn = sh->Is_State(CPumpData::IN_BROKEN);
			p->m_bStopOut = sh->Is_State(CPumpData::OUT_BROKEN);
			p->m_bNewAuto = ((sh->Use_Flags % 10) != 0);
			p->m_bIsOpen = p->sh.Is_State(CPumpData::IS_OPEN);
			p->m_bIsClose = p->sh.Is_State(CPumpData::IS_CLOSE);
			p->m_bClose = p->sh.Is_Ctrl(CPumpData::Set_Off);
			if (p->bMstAuto)
			{
				/*DWORD nTime = ::GetTickCount();*/
				DWORD nTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
				if (nTime > (p->nTime + 2000))
				{
					p->nTime = 0;
					p->mPoly = 1;
				}
				else
				{
					p->mPoly = 2;
				}
			}
		}
	}
	//
	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void fnSendData(void* base, void* pNewValue, void* pOldValue, bool bMessage)
{
	Queue_PumpST* p = (Queue_PumpST*)base;
	// if (!ISTypeOK) return;
	//
	bool bSend = true;
	if (p->m_bNoSendHs)
	{
		bSend = false;
	}
	//
	if (pNewValue == &p->mPress)
	{
		bMessage = true;
		int Old = p->mPress ? 1 : 0;
		int New = p->mPress ? CPumpData::Set_On : CPumpData::Set_Off;
		p->send.Set(&Old, &New);
	}
	else if (pNewValue == &p->bStopor)
	{
		bool Old = p->sh.Eq_State(CPumpData::Stopor);
		bool New = p->bStopor;
		bMessage = true;
		p->send.Set(enumValueBol, &Old, &New);
	}
	else if (pNewValue == &p->bMstAuto)
	{
		bool Old = p->sh.Eq_State(CPumpData::AutoPusk);
		bool New = p->bMstAuto;
		p->send.Set(enumValueBol, &Old, &New);
	}
	else if (pNewValue == &p->m_bNewAuto)
	{
		bMessage = true;
		int Old = p->sh.Use_Flags;
		int New = (p->sh.Use_Flags / 10) * 10;
		if (p->m_bNewAuto)
		{
			New = New + 1;
		}

		p->send.Set(enumValueInt, &Old, &New);
	}
	else if (pNewValue == &p->bNoElectro)
	{
		bool Old = false;//( p->sh.State_Flags & CPumpData::AutoPusk ) ? 1 : 0;
		bool New = p->bNoElectro;
		p->send.Set(enumValueBol, &Old, &New);
	}
	else if (strcmpi(p->send.szValue, "Тумблер") == 0)
	{
		/*
		if ( p->mPoly == 0 )
		  {
		  p->nTime = 0;
		  p->sh.ChangeCode = CShPump::shManAuto;
		  p->sh.m_bMstAuto = true;
		  p->pPipe->PtData( p->nNumber, p->pSH );
		  //
		  p->sh.ChangeCode = CShPump::shOff;
		  }
		else
		if ( p->mPoly == 1 )
		  {
		  p->nTime = 0;
		  p->sh.ChangeCode = CShPump::shManAuto;
		  p->sh.m_bMstAuto = false;
		  }
		else
		if ( p->mPoly == 2 )
		  {
		  p->nTime = ::GetTickCount();
		  p->sh.ChangeCode = CShPump::shManAuto;
		  p->sh.m_bMstAuto = false;
		  p->pPipe->PtData( p->nNumber, p->pSH );
		  //
		  p->sh.ChangeCode = CShPump::shOn;
		  }
		*/
	}
	else
	{
		p->send.SetNew(pNewValue);
	}
	//
	if (bSend)
	{
		if (p->nSocketNumber != -1)
		{
			if (p->send.eType == enumValueInt)
			{
				double dValue = p->send.nNew;
				if (strcmpi(p->send.szValue, "Вкл/Выкл") == 0)
				{
					if (dValue == CPumpData::Set_On)
					{
						dValue = 1;
					}
					else if (dValue == CPumpData::Set_Off)
					{
						dValue = 0;
					}
				}

				p->pSocket->SetSocketValue(id_Pump, p->send.szName, p->send.szValue, dValue, p->send.dOld, bMessage, p->send.eType);
			}
			else if (p->send.eType == enumValueDbl)
			{
				double dValue = p->send.dNew;
				p->pSocket->SetSocketValue(id_Pump, p->send.szName, p->send.szValue, dValue, p->send.dOld, bMessage, p->send.eType);
			}
		}
		else
		{
			p->pPipe->SendData(p->send, bMessage);
		}
	}

	if (strcmpi(p->send.szValue, "КонтрольСостояния") == 0)
	{
		int Old = CPumpData::Ctrl_State;
		int New = CPumpData::Ctrl_State;
		p->send.Set(enumValueInt, &Old, &New);
		p->pPipe->SendData(p->send, false);
	}

	p->m_bPressed = !bMessage;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FillPump(SPriborBase* d, void* pp)
{
	d->fnData = fnSendData;
	//
	Queue_PumpST* p = (Queue_PumpST*)pp;
	if (p->nHS >= 0 && d->nType == 1)
	{
		SPriborBool* a = (SPriborBool*)d;
		a->pbOther = &p->bState;
	}

	if (strcmpi(p->send.szName, "ON/OFF") == 0)
	{
		d->eType = enumValueBol;
		d->pVoid = &p->mPress;
		d->pbBlock = &p->bStopor;
	}
	else if (strcmpi(p->send.szName, "Дренаж") == 0)
	{
		d->pbBlock = &p->mOn;
	}
	else if (strcmpi(p->send.szName, "Стопор") == 0)
	{
		d->eType = enumValueBol;
		d->pVoid = &p->bStopor;
		d->pbBlock = &p->mOn;
	}
	else if (strcmpi(p->send.szName, "Auto") == 0)
	{
		d->pVoid = &p->bMstAuto;
	}
	else if (strcmpi(p->send.szValue, "Авто") == 0)
	{
		d->pVoid = &p->m_bNewAuto;
	}
	else if (strcmpi(p->send.szValue, "Показания") == 0 && d->nType == 4)
	{
		d->eType = enumValueBol;
		d->pVoid = &p->mOn;
	}
	else if (strcmpi(p->send.szValue, "Тумблер") == 0)
	{
		d->pVoid = &p->mPoly;
	}
	else if (strcmpi(p->send.szValue, "Электро") == 0)
	{
		d->pVoid = &p->bNoElectro;
	}
	else if (d->pVoid == &p->sh.State_Flags && strcmpi(p->send.szValue, "Открыт") == 0)
	{
		d->pVoid = &p->m_bIsOpen;
	}
	else if (d->pVoid == &p->sh.State_Flags && strcmpi(p->send.szValue, "Включен") == 0)
	{
		d->pVoid = &p->mOn;
	}
	else if (d->pVoid == &p->sh.State_Flags && strcmpi(p->send.szValue, "Готов") == 0)
	{
		d->pVoid = &p->mReady;
	}
	else if (d->pVoid == &p->sh.Ctrl_Flags && strcmpi(p->send.szValue, "Остановить") == 0)
	{
		d->pVoid = &p->m_bClose;
	}
	else if (d->pVoid == &p->sh.State_Flags && strcmpi(p->send.szValue, "Закрыт") == 0)
	{
		d->pVoid = &p->m_bIsClose;
	}

	if (strcmpi(p->send.szName, "Вход") == 0)
	{
		d->pbBlock = &p->m_bStopIn;
	}

	if (strcmpi(p->send.szName, "Выход") == 0)
	{
		d->pbBlock = &p->m_bStopOut;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddPumpST(DefineTegs)
{
	//
	AddQueueEx(Queue_PumpST);
	//
	CShPump sh;
	memcpy(&p->sh, &sh, sizeof(CShPump));
	p->pSH = &p->sh;
	p->nSocketNumber = -1;
	//
	// Код, посылаемый в АРМ
	p->btSendType = QUEUE_NO;
	// Определение функции вывода
	p->paint = &PaintPumpST;
	//
	p->nHS = -1;
	p->m_bNoSendHs = false;
	p->nNoSendHs = -1;
	char szHS[64] = "";
	char szSuffNoSend[64] = "";
	//
	STegObj obj[] =
	{
		{ 'S', "view", szHS, 63 },
		{ 'S', "suffNoSend", szSuffNoSend, 63 },
		// Дополнительная информация
		EXT_QUEUE
	};
	::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));
	//
	if (*szHS)
	{
		p->nHS = def.AddKind(id_HS, szHS);
	}
	//
	if (*szSuffNoSend)
	{
		int nActive;
		nActive = def.GetActiveObjSocket();
		const char* name = def.GetModelName(nActive);
		char szName[128];
		sprintf_s(szName, sizeof(szName), "%s%s", name, szSuffNoSend);
		p->nNoSendHs = def.AddKindPipe(id_HS, szName);
		def.m_queue.m_pPipe->End();
	}
	//
	auto hPribor = AddPribors(def, teg, Buffer, p, &FillPump, id_Pump);
	//
	managed::RegisterElement(teg->name, p, hPribor);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
