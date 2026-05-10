#include "Queue.h"
#include "../Lang.h"
#include "../socketclient.h"

  enum eDataType
  {
  DataBool, 
  DataInt,
  DataDouble
  };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура работы с задвижкой
  struct Queue_DataSH : public SPROLOZ
  {
	  bool bValue;
	  int nValue;
	  double dValue;
	  eDataType dataType;
	  char szName[64];
	  char szParam[64];
	  int nType;
  };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
//int PaintDataSH(DefinePaint)
//  {
//  QueueP(Queue_DataSH);
//  //
//  if (q.m_pSocket->IsConnected())
//  {
//	  switch (p->dataType)
//	  {
//			case DataBool:q.m_pSocket->GetParam(p->szName, p->szParam,p->bValue);break;
//			case DataInt:q.m_pSocket->GetParam(p->szName, p->szParam,p->nValue);break;
//			case DataDouble:q.m_pSocket->GetParam(p->szName, p->szParam,p->dValue);break;
//	  }
//  }
//  if (p->dataType==DataDouble)
//  {
//	if ( p->nType == 0 ) DrawValue( hDC, p, p->dValue, "%" );
//	if ( p->nType == 1 ) DrawValue( hDC, p, p->dValue, "" );
//  }
//  //
//  return 1;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddDataSH(DefineTegs)
  {
  //
  AddQueueEx(Queue_DataSH);
  //
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  //p->paint = &PaintDataSH;
  p->bValue=false;
  p->nValue=0;
  p->dValue=0;
  p->dataType=DataDouble;
  char szShift[64] = "\0";
  p->nType=0;
  //
  STegObj obj[] =
    {
    { 'I', "type" , &p->nType },
	{ 'S', "shift", szShift, 63},
    // Дополнительная информация
    EXT_QUEUE
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  lstrcpy(p->szName,teg->value);
  lstrcpy(p->szParam,szShift);
  if (lstrcmp(p->szName,".")==0)
  {
	  int nActive=def.GetActiveObjSocket();
	  const char* name = def.GetModelName(nActive);
	  lstrcpy(p->szName,name);
  }

  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
