#include "Queue.h"
#include "../Lang.h"
#include "../socketclient.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура работы с задвижкой
struct Queue_ZadvST : public Queue_PRS
{
	// vladexl CShZadv sh;
	bool mPress;
	bool mAutoMode;
	bool mView;
	bool mZagl;
	int  mSet;
	//
	int nOper;
	double dValue;
	bool m_bStop;
	bool bRichagIntPribor;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
//int PaintZadvST(DefinePaint)
//{
//	QueueP(Queue_ZadvST);
//	//
//	CShZadv* sh = NULL;
//	if (p->nNumber != -1)
//	{
//		sh = (CShZadv*)PipeSh;
//	}
//	else if (p->nSocketNumber != -1)
//	{
//		sh = (CShZadv*)q.m_pSocket->Sh(p->nSocketNumber);
//	}
//	//
//	if (sh == NULL)
//	{
//		return 1;
//	}
//
//	if (IsTypeOk)
//	{
//		if (!p->m_bPressed)
//		{
//			memcpy(&p->sh, sh, sizeof(CShZadv));
//			//memcpy( &p->sh, p_sh, sizeof( CShZadv ) );
//		}
//		//
//		p->m_bStop = p->sh.Is_Blk(CZadv::ZADV_BROKEN);
//		switch (p->nOper)
//		{
//		case 0x5: p->mView = (sh->Vent == p->dValue); break;
//		case 0x6: p->mView = (sh->Vent < p->dValue); break;
//		case 0x7: p->mView = (sh->Vent > p->dValue); break;
//		case 0x8: p->mView = (sh->Vent <= p->dValue); break;
//		case 0x9: p->mView = (sh->Vent >= p->dValue); break;
//		case 0xA: p->mView = (sh->Vent != p->dValue); break;
//		}
//	}
//	//
//	return 1;
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void fnSendData(void* base, void* pVoid, void* oldValue, bool bMessage)
{
	ASSERT(base);
	Queue_ZadvST* p = (Queue_ZadvST*)base;
	// vladexl In C#: if ( !ISTypeOK ) return;
	//
	if (p->nSocketNumber != -1)
	{
		double dValue = *(double*)pVoid;
		p->pSocket->SetSocketValue(id_Zadv, p->send.szName, p->send.szValue, dValue, p->send.dOld, bMessage, p->send.eType);

		// vladexl: it seems already solved in C#:  _remoteData.CZadvW.Vent = newValue;
		//if (p->bRichagIntPribor)
		//{
		   // //без того что ниже, при окончании перетаскивания ручки, сначала изображение ручки возврашается в исходное положение и лишь затем в нужное
		   // UINT count = p->pSocket->Count();
		   // for(UINT ii =0; ii< count; ii++)
			  //{
			  //SSterver* obj =  p->pSocket->Item(ii);
			  //if ( obj != NULL )
			  //   if(strcmp(obj->szName+1, p->send.szName) == 0)
			  //	 p->pSocket->FillBaseStructFromSocket(obj);
			  //}
		//}
	}
	/* vladexl else
	 {
		 p->send.SetNew( pVoid );
		 p->pPipe->SendData( p->send, bMessage );
	 }*/
	p->m_bPressed = !bMessage;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FillZadvig(SPriborBase* d, void* pp)
{
	d->fnData = fnSendData;
	Queue_ZadvST* p = (Queue_ZadvST*)pp;
	// Маленькие нюансы
	if (d->nType == 1 && lstrcmp(p->send.szValue, "Заглушка") == 0)
	{
		d->pVoid = &p->mZagl;
	}

	if (d->nType == 11)
	{
		p->bRichagIntPribor = true;
	}
	//
	if (p->nOper)
	{
		d->pbView = &p->mView;
	}

	if (lstrcmp(p->send.szName, "Задание") == 0)
	{
		d->pbBlock = &p->m_bStop;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddZadvST(DefineTegs)
{
	//
	AddQueueEx(Queue_ZadvST);
	//
	//vladexl CShZadv sh;
	//vladexl memcpy( &p->sh, &sh, sizeof(CShZadv) );
	//vladexl p->pSH = &p->sh;
	p->mView = true;
	char szOper[64] = "";
	char szSpecName[64] = "";
	//
	// Код, посылаемый в АРМ
	p->btSendType = QUEUE_NO;
	// Определение функции вывода
	//p->paint = &PaintZadvST;
	p->nSocketNumber = -1;
	p->nNumber = -1;
	p->bRichagIntPribor = false;
	STegObj obj[] =
	{
	{ 'S', "oper", szOper, 63 },
	{ 'I', "set" , &p->mSet   },
	{ 'S', "specname", szSpecName, 63 },
	};
	::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));
	if (*szOper)
	{
		static char* oper[] = { "==","<.",">.","<=",">=","!=" };
		char chr = szOper[2]; szOper[2] = 0;
		for (int n = 0; n < 6; n++)
		{
			if (lstrcmp(szOper, oper[n]) == 0)
			{
				p->nOper = n + 5;
				break;
			}
		}

		szOper[2] = chr;
		p->dValue = Atof(szOper + 2);
	}
	//
	int nActive = def.GetActiveObjSocket();
	if (*szSpecName)
	{
		def.AddKind(id_Zadv, szSpecName);
		def.m_queue.m_pSocket->Refresh();
	}

	auto hPribor = AddPribors(def, teg, Buffer, p, &FillZadvig, id_Zadv);
	if (*szSpecName)
	{
		def.m_queue.m_pSocket->m_nActive = nActive;
	}

	managed::RegisterElement(teg->name, p, hPribor);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
