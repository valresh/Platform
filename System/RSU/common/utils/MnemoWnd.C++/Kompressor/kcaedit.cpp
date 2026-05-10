//#include "..\MnemoWnd.h"
#include "Queue.h"
#include "../Lang.h"
#include <string>

struct Queue_KcaEdit : public SPROLOZ
{
	//HWND  hWnd;
	float dScale;
	bool  bDestroy;
	char szName[64];
	char szNewName[64];
	int nFloat;
	int  nHS;
	int nEqual;
	int nNoEqual;
	bool bGreen;
	bool bIsGreen;
	int  nKlapan;
	bool showAlways;
	char szHSControl[64];
	bool bShowEditbox;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void ClearKcaEdit(SQueue& q,void* s)
//  {
//  Queue_KcaEdit* p = (Queue_KcaEdit*)s;
//  if ( p->bDestroy )
//    if ( ::IsWindow( p->hWnd ) )
//      ::DestroyWindow( p->hWnd );
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
int PaintKcaEdit(DefinePaint)
{
	QueueP(Queue_KcaEdit);
	return 0;
}
int PressKcaEdit(DefinePress)
{
	Queue_KcaEdit* p = (Queue_KcaEdit*)s;
	if (nCode == 2)
	{
		//p->hWnd = (HWND)::SendMsg(q.hWnd, QUEUE_EDIT_W, (LPARAM)&p->mRect);
		p->dScale = q.setScale;
		p->bIsGreen = p->bGreen;
		if (p->nHS >= 0)
		{
			CShHS& Sh = (CShHS&)q.m_pPipe->SH(p->nHS);
			int nState = -1;
			if (Sh.RetCode == CShBase::rcOK)
			{
				if (Sh.mOn)
				{
					nState = 1;
				}
				else
				{
					nState = 0;
				}

				if (p->nEqual != -1)
				{
					p->bIsGreen = (nState == p->nEqual);
					p->bShowEditbox = (nState == p->nEqual);
				}
				
				if (p->nNoEqual != -1)
				{
					p->bIsGreen = (nState != p->nNoEqual);
					p->bShowEditbox = (nState != p->nNoEqual);
				}
			}
		}

		bool bSendOtherName = false;
		if (p->nKlapan >= 0)
		{
			CShKlapan& Sh = (CShKlapan&)q.m_pPipe->SH(p->nKlapan);
			if (Sh.RetCode == CShBase::rcOK)
			{
				bool bManual = (Sh.LocalRemoteDouble == 0) && !p->showAlways;
				if ((bManual) && (strstr(p->szName, ".Задание") != NULL))
				{
					bSendOtherName = true;
				}
			}
		}
		//::PostMessage(p->hWnd, nPressQueue, QUEUE_SETEDITGREEN, (LPARAM)(&p->bIsGreen));
		//::PostMessage(p->hWnd, nPressQueue, QUEUE_SCALE, (LPARAM)(&p->dScale));
		//::PostMessage(p->hWnd, nPressQueue, QUEUE_FLOAT, (LPARAM)(&p->nFloat));
		if (!bSendOtherName)
		{
			if (p->bShowEditbox)
			{
				//if (*p->szHSControl)
				//	::PostMessage(p->hWnd, nPressQueue, QUEUE_HSCONTROLNAME, (LPARAM)(&p->szHSControl));
				//::PostMessage(p->hWnd, nPressQueue, QUEUE_POINTNAME, (LPARAM)(&p->szName));
			}
		}
		else
		{
			std::string newName(p->szName);
			std::string strReplace = ".Задание";
			size_t nFind = newName.find(strReplace);
			size_t nReplaceLength = strReplace.length();
			newName.replace(nFind, nReplaceLength, ".Поворот");
			strcpy(p->szNewName, newName.c_str());
			//::PostMessage(p->hWnd, nPressQueue, QUEUE_POINTNAME, (LPARAM)(&p->szNewName));
		}
	}

	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddKcaEdit(DefineTegs)
{
	AddQueueEx(Queue_KcaEdit);
	// Код, посылаемый в АРМ
	p->btSendType = QUEUE_NO;
	// Определение функции вывода
	//p->paint = &PaintKcaEdit;
	//p->press= &PressKcaEdit;
	//p->clear = &ClearKcaEdit;
	p->nFloat = 0;
	p->nEqual = -1;
	p->nNoEqual = -1;
	p->bGreen = false;
	p->nHS = -1;
	p->nKlapan = -1;
	p->bIsGreen = false;
	p->showAlways = 0;
	p->bShowEditbox = true;
	char szHS[64] = "";
	char szKlapan[64] = "";
	char szSFcom[64] = "\0";
	Multiline(p->szName, teg[0].value, sizeof(p->szName));
	STegObj obj[] =
	{
	{ 'R', "rect"   , &p->mRect },
	{ 'E', "rc"     , &p->mRect },
	{ 'L', "destroy", &p->bDestroy },
	{ 'I', "float", &p->nFloat },
	{ 'I', "equal", &p->nEqual },
	{ 'I', "noequal", &p->nNoEqual },
	{ 'B', "green", &p->bGreen },
	{ 'S', "view", szHS, 63 },
	{ 'S', "viewKlapan", szKlapan, 63 },
	{ 'S', "suffcommon", szSFcom, 63 },
	{ 'L', "showAlways", &p->showAlways},
	{ 'S', "view", szHS, 63 },
	{ 'S', "hsControl", p->szHSControl, 63 },
	};
	::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));

	if (*szSFcom)
	{//добавляется суффикс к переменной
		strncat(p->szName, szSFcom, sizeof(szSFcom));
	}
	if (*szHS)
		p->nHS = def.AddKindPipe(id_HS, szHS);
	if (*szKlapan)
		p->nKlapan = def.AddKind(id_Klapan, szKlapan);
	//ASSERT(FALSE);
	p->dScale = 1.0;

	managed::RegisterElement(teg->name, p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~