#include <CommProc.h>
#ifndef CRSUBASE
#define CRSUBASE

struct CRSUBase
  {
  DWORD size;// размер структуры в байтах
  DWORD ID_CLASS;// идентификатор класса
  DWORD ID_PNT_NAME;// идентификатор конкретной точки связи
  };

#endif
