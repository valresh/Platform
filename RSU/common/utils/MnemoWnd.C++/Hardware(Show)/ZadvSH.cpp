#include "Queue.h"
#include "../Lang.h"
#include "../socketclient.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура работы с задвижкой
struct Queue_ZadvSH : public SPROLOZ
  {
  int nValue;
  int nType;
  } ;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
//int PaintZadvSH(DefinePaint)
//{
//    QueueP(Queue_ZadvSH);
//    //
//    double dValue = 0;
//    CShZadv* sh = NULL;
//    if (p->nNumber != -1)
//        sh = (CShZadv*)PipeSh;
//    else
//        if (p->nSocketNumber != -1)
//            sh = (CShZadv*)q.m_pSocket->Sh(p->nSocketNumber);
//    //
//    if (sh == NULL)
//        return 1;
//    switch (p->nValue)
//    {
//    case 0:dValue = sh->Vent; break;
//    default: dValue = sh->VentZ; break;
//    }
//    //
//    if (p->nType == 0) DrawValue(hDC, p, dValue, "%");
//    else
//        if (p->nType == 1)
//        {
//            ASSERT(0);
//        }
//        else
//            if (p->nType == 2)
//            {
//                if (::IsMnemoStyle(MNEMO_EDIT))
//                    if (!IsTypeOk)
//                    {
//                        TransparentRect(hDC, p->mRect, 0x80ff, 0xffffff);
//                    }
//            }
//            else
//                if (p->nType == 3)
//                {
//                    const char* text = NULL;
//                    if (sh->Eq_State(CZadv::DRY_STEAM))
//                    {
//                        static char* txt = "Идет сухой пар";
//                        text = txt;
//                    }
//                    else
//                        if (sh->Eq_State(CZadv::CONDENSAT))
//                        {
//                            static char* txt = "Идет конденсат";
//                            text = txt;
//                        }
//                    //
//                    if (text != NULL)
//                    {
//                        ::DrawText(hDC, text, lstrlen(text), &p->mRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
//                    }
//                    //
//                }
//    //
//    return 1;
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddZadvSH(DefineTegs)
  {
  //
  AddQueueEx(Queue_ZadvSH);
  //
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  //p->paint = &PaintZadvSH;
  p->nSocketNumber=-1;
  p->nNumber=-1;
  //
  STegObj obj[] =
    {
    { 'I', "type" , &p->nType },
    // Дополнительная информация
    EXT_QUEUE
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  AddPRSH( def, id_Zadv, teg[0].value, p, p->nValue );

  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
