#include "Queue.h"
#include "../Lang.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура
struct Queue_Draw : public SPROLOZ
  {
  int dl;
  char* text;
  int nAlign;
  } ;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ClearDraw(SQueue& q,void* s)
  {
  Queue_Draw* p = (Queue_Draw*)s;
  delete [] p->text;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
int PaintDraw(DefinePaint)
  {
  //QueueP(Queue_Draw);
  //int nOldAlign = ::SetTextAlign( hDC, TA_LEFT|TA_TOP );
  //::DrawText( hDC, p->text, p->dl, &p->mRect, p->nAlign|DT_SINGLELINE );
  //::SetTextAlign( hDC, nOldAlign );
  return 1;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
static int PaintTime(DefinePaint)
  {
  //QueueP(Queue_Draw);
  ////
  //__time64_t T = GiveModelTime();
  //struct tm TM;
  //_localtime64_s( &TM, &T );
  //char szTime[256];
  //strftime( szTime, 255, p->text, &TM );
  ////
  //int nOldAlign = ::SetTextAlign( hDC, TA_LEFT|TA_TOP );
  //::DrawText( hDC, szTime, lstrlen(szTime), &p->mRect, p->nAlign|DT_SINGLELINE );
  //::SetTextAlign( hDC, nOldAlign );
  return 1;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddDraw(DefineTegs)
  {
  int nMultik = Multiline( NULL, teg[0].value, lstrlen(teg[0].value)+1 );
  if ( nMultik )
    {
    AddWord( def, teg, Buffer, rLog );
    return;
    }
  AddQueueEx(Queue_Draw);
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  p->paint = &PaintDraw;
  p->clear = &ClearDraw;
  //
  STegObj obj[] =
    {
    { 'R', "rect" , &p->mRect  },
    { 'E', "rc"   , &p->mRect  },
    { 'W', "a"    , &p->nAlign },
    { 'W', "align", &p->nAlign },
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  p->dl = lstrlen(teg[0].value);
  p->text = new char[p->dl+1];
  lstrcpy( p->text, teg[0].value );
  
  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddDrawDbl   (DefineTegs)
  {
  //AddQueueEx(Queue_Draw);
  //// Код, посылаемый в АРМ
  //p->btSendType = QUEUE_NO;
  //// Определение функции вывода
  //p->paint = &PaintDraw;
  //p->clear = &ClearDraw;
  //int nFloat = 0;
  ////
  //STegObj obj[] =
  //  {
  //  { 'R', "rect" , &p->mRect  },
  //  { 'E', "rc"   , &p->mRect  },
  //  { 'I', "float", &nFloat    },
  //  { 'W', "a"    , &p->nAlign },
  //  { 'W', "align", &p->nAlign },
  //  };
  ////
  //::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  ////
  //p->text = new char[512];
  //double dValue = Atof(teg[0].value);
  //if ( nFloat >= 0 )
  //  {
  //  char filtr[8] = "%.0f";
  //  filtr[2] += nFloat;
  //  sprintf_s( p->text, 512, filtr, dValue );
  //  }
  //else
  //::DblToStr( p->text, dValue, 512 );
  //p->dl = lstrlen(p->text);
  ////
  ////
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddDrawInt   (DefineTegs)
  {
  AddQueueEx(Queue_Draw);
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  p->paint = &PaintDraw;
  p->clear = &ClearDraw;
  //
  STegObj obj[] =
    {
    { 'R', "rect" , &p->mRect  },
    { 'E', "rc"   , &p->mRect  },
    { 'W', "a"    , &p->nAlign },
    { 'W', "align", &p->nAlign },
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  p->text = new char[32];
  int nValue = Atoi(teg[0].value);
  sprintf_s( p->text, 32, "%d", nValue );
  p->dl = lstrlen(p->text);
  //
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddDrawTime(DefineTegs)
  {
  AddQueueEx(Queue_Draw);
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  p->paint = &PaintTime;
  p->clear = &ClearDraw;
  //
  STegObj obj[] =
    {
    { 'R', "rect" , &p->mRect  },
    { 'E', "rc"   , &p->mRect  },
    { 'W', "a"    , &p->nAlign },
    { 'W', "align", &p->nAlign },
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  p->dl = lstrlen(teg[0].value);
  p->text = new char[p->dl+1];
  lstrcpy( p->text, teg[0].value );
  //
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddDrawValue(DefineTegs)
  {
  AddQueueEx(Queue_Draw);
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  p->paint = &PaintDraw;
  p->clear = &ClearDraw;
  int nFloat = 0;
  CharMP szMin = "0";
  CharMP szMax = "100";
  //
  STegObj obj[] =
    {
    { 'R', "rect" , &p->mRect  },
    { 'E', "rc"   , &p->mRect  },
    { 'W', "a"    , &p->nAlign },
    { 'W', "align", &p->nAlign },
    { 'I', "float", &nFloat    },
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  // Тупиковый вариант
  //
  p->dl = lstrlen(teg[0].value);
  p->text = new char[p->dl+1];
  lstrcpy( p->text, teg[0].value );
  //
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
