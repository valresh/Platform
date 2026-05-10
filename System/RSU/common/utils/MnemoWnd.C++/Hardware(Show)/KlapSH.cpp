#include "Queue.h"
#include "../Lang.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура работы с задвижкой
struct Queue_KlapSH : public SPROLOZ
  {
  int nValue;
  int nType;
  COLORREF color;
  COLORREF colorTrans;
  int nText;
  double dScaleValue;
  int nMaxValue;
  } ;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern int nLastKlapan;
void AddKlapSH(DefineTegs)
  {
  AddQueueEx(Queue_KlapSH);
  //
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  p->nValue = nLastKlapan;
  p->color=RGB(255,0,0);
  p->colorTrans=0xcdcccc;
  p->nText=-1;
  p->dScaleValue=0;
  p->nMaxValue=0;
  p->nSocketNumber=-1;
  p->nNumber=-1;
  //
  STegObj obj[] =
    {
    { 'I', "type" , &p->nType },
	{ 'A', "color", &p->color  },
	{ 'A', "colorTrans", &p->colorTrans  },
	{ 'I', "text", &p->nText},
	{ 'D', "scaleValue", &p->dScaleValue},
	{ 'I', "nMaxValue", &p->nMaxValue},
    // Дополнительная информация
    EXT_QUEUE
    };
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  AddPRSH( def, id_Klapan, teg[0].value, p, p->nValue );
  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
