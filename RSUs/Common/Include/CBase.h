#ifndef CBASE_H
#define CBASE_H

struct CBase
{
  DWORD size;// размер структуры в байтах
  DWORD ID_CLASS;// идентификатор класса
  DWORD ID_PNT_NAME;// идентификатор конкретной точки связи
};

#endif // CBASE_H
