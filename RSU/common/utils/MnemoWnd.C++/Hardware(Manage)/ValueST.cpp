#include "Queue.h"
#include "../Lang.h"
#include "Names.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура работы с задвижкой
struct Queue_ValueST : public Queue_PRS
{
	CShValue sh;
	bool m_bFixator;
	int m_nHsBlock;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
int PaintValueST(DefinePaint)
{
	QueueP(Queue_ValueST);
	//  //
	//  if ( p->m_nHsBlock >= 0 )
	//    {
	//		int nNumber=p->nNumber;
	//		  CShHS& Sh = (CShHS&)q.m_pPipe->SH(p->m_nHsBlock);
	//		  if ( Sh.RetCode == CShBase::rcOK )
	//		  {
	//				p->m_bFixator =  Sh.mOn;
	//		  }
	//	}
	//
	CShValue& sh = (CShValue&)PipeSH;
	if (IsTypeOK)
	{
		if (!p->m_bPressed)
			memcpy(&p->sh, &sh, sizeof(CShValue));
	}
	//  else
	//  {
	//		if ( ::IsMnemoStyle(MNEMO_EDIT) )
	//		{
	//			RECT rc = p->mRect;
	//			::InflateRect( &rc, 2, 2 );
	//			CMyBrush brush( hDC, 0x0080ff );
	//			brush.Rect( rc );
	//		}
	//  }
	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция посылки сообщения
static void fnSendData(void* base, void* pNewValue, void* pOldValue, bool bMessage)
{
	//ASS( base )
	Queue_ValueST* p = (Queue_ValueST*)base;
	// if (!ISTypeOK) return;
	//

	bool bSend = true;
	if (bSend)
	{
		p->send.SetNew(pNewValue);
		p->pPipe->SendData(p->send, bMessage);
	}

	p->m_bPressed = !bMessage;
}

void FillValue(SPriborBase* d, void* pp)
{
	d->fnData = fnSendData;
	Queue_ValueST* p = (Queue_ValueST*)pp;
	if (_strcmpi(p->send.szValue, "Фиксатор") == 0)
	{
		d->pbBlock = &p->m_bFixator;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddValueST(DefineTegs)
{
	//
	AddQueueEx(Queue_ValueST);
	//
	CShValue sh;
	memcpy(&p->sh, &sh, sizeof(CShValue));
	p->pSH = &p->sh;
	// Код, посылаемый в АРМ
	p->btSendType = QUEUE_NO;
	// Определение функции вывода
	p->paint = &PaintValueST;
	char szHSBlock[64] = "";
	p->m_nHsBlock = -1;
	p->m_bFixator = false;
	STegObj obj[] =
	{
		{ 'S', "block", szHSBlock, 63 },
		// Дополнительная информация
		EXT_QUEUE
	};
	::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));
	//
	if (*szHSBlock)
	{
		p->m_nHsBlock = def.AddKindPipe(id_HS, szHSBlock);
	}

	auto hPribor = AddPriborsPipe(def, teg, Buffer, p, &FillValue, id_Value);
	managed::RegisterElement(teg->name, p, hPribor);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
