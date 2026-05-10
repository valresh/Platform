#include "Common.h"
#include "Queue.h"
#include "../Lang.h"
//
// Обработка кнопок
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура
struct Queue_Tele : public SPROLOZ, public SBtnDataEx
{
	char szGoto[64];
	bool m_bPress;
	bool m_bOperMessage;
	bool m_bCommand;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//int PressTele(DefinePress)
//  {
//  Queue_Tele* p = (Queue_Tele*)s;
//  if ( nCode == 0 && !p->m_bPress )
//    p->m_bPressed = true;
//  else
//  if ( nCode == 1 )
//    { }
//  else
//  if ( nCode == 2 )
//    p->m_bPressed = false;
//  //
//  bool bPtInRect = ::PtInRect( &p->mRect, point ) == TRUE;
//  bool bPressed = p->m_bPressed;
//  if ( bPressed )
//    bPressed = bPtInRect;
//  //
//  HDC hDC = ::GetDC( q.hWnd );
//  if ( hDC != NULL )
//    {
//    {
//    MYWT;
//    ::PaintButton( hDC, p->mRect, p, bPressed, false );
//    }
//    ::ReleaseDC( q.hWnd, hDC );
//    }
//  //
//  if ( nCode == 2 && bPtInRect && !p->m_bPress )
//    {
//    char szText[256];
//	if (!p->m_bOperMessage)
//		sprintf_s( szText, sizeof( szText ), "Звонок '%s'", p->szName );
//	else
//		sprintf_s( szText, sizeof( szText ), "%s", p->szGoto );
//    //
//    SSendToModel send;
//    send.eData = id_Msg;
//	send.eType = (EValueType)CEventLogType::etRsuAction;
//    lstrcpyn( send.szName, szText, sizeof(send.szName)+sizeof(send.szValue) );
//    q.m_pPipe->SendData( send, true );
//    //
//    p->m_bPress = true;
//    p->txtNorma = 0x808080;
//	if ((Version==UPO_KF)||(Version==LAB_KF)||(Version==L35_11600_KF))
//	{
//		p->txtNorma =0xffffff;
//	}
//	else
//	{
//		if (!p->m_bOperMessage)
//			lstrcpy( p->szName, "Звонок принят" );
//		else
//		{
//			if (p->m_bCommand) 
//				lstrcpy( p->szName, "Команда принята" );
//			else
//				lstrcpy( p->szName, "Вызов принят" );
//		}
//	}
//    ::InvalidateRect( q.hWnd, NULL, FALSE );
//    }
//  //
//  return 0;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
//int PaintTele(DefinePaint)
//  {
//  QueueP(Queue_Tele);
//  //
//  bool bPressed = ::PressedButton( p, p->mRect, q );
//  ::PaintButton( hDC, p->mRect, p, bPressed, false );
//  //
//  return 1;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddTele(DefineTegs)
{
	//
	AddQueueEx(Queue_Tele);
	// Код, посылаемый в АРМ
	p->btSendType = QUEUE_BUTTON;
	// Определение функции вывода
	// p->paint = &PaintTele;
	// p->press = &PressTele;
	p->m_bOperMessage = false;
	p->m_bCommand = false;
	//
	DefaultButton(p, teg[0].value);
	//
	STegObj obj[] =
	{
	{ 'S', "goto" , p->szGoto, 63 },
	{ 'B', "opermessage",&p->m_bOperMessage},
	{ 'B', "command",&p->m_bCommand},
	TEG_BUTTON // Кнопочная информация
	EXT_QUEUE  // Дополнительная информация
	//
	};
	::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));
	p->nNumber = def.GetActiveObj(); // ???
	// PostfixButton(p); add funcional to C#
	//
	// Выводим элементы, которые остаются постоянными
	//sprintf_s(def.m_szStr, 0x800, "<DefPushBtn=%d,%d par0=%d par1=%d>",
	//	p->mRect.left, p->mRect.top, p->mRect.right, p->mRect.bottom);
	// ::InflateRect(&p->mRect, -1, -1); not supported on Linux. Move to C#
	//
	managed::RegisterElement(teg->name, p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
