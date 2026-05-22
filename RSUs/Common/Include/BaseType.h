#ifndef BASETYPE_H
#define BASETYPE_H
#pragma once
#include "CBase.h"
#include <string.h>
#pragma warning( disable : 444 ) //Отключаем предупреждение о невиртуальности
//
enum EValueType : unsigned char
{
  enumValueUnk = 0,
  enumValueDbl = 1,
  enumValueInt = 2,
  enumValueChr = 3,
  enumValueBol = 4,
  enumValueFlt = 5,
  enumValueStr = 6,
  enumValueI64 = 7,
  // Для редактора
  //enumValueColor =  8,//Цвет
  //enumValueStyle =  9,//Стиль линии
  //enumValueBrush = 10,//Стиль закраски
  //enumValuePoint = 11,//Точка
  //enumValueSize  = 12,//Размер
  //enumValueRect  = 13,//Область
  //enumValueAligH = 14,//Выравнивание по горизонтали
  //enumValueAligV = 15,//Выравнивание по вертикали
  //enumValueFont  = 16,//Фамилия шрифта
  //enumPipeError  = 17,//при выполнении скриптов не получено значение с сервера
};

struct SValueDef
{
  void Val( const char* n, DWORD_PTR hs, DWORD_PTR sh, EValueType e )
  {
    memset( this, 0, sizeof(*this) );
    name = n;
    dwShift = hs;
    dwShiftSh = sh;
    eVal = e;
  }
  //
  double D(void* sh)
  {
    char* Void = (char*)((char*)(sh) + dwShiftSh);
      switch ( eVal )
      {
        case enumValueDbl: return *(double*)Void;
        case enumValueInt: return *(int   *)Void;
        case enumValueChr: return *(BYTE  *)Void;
      };
    return 1.7976931348623158e+308;
  };
  //
  void* V(void* sh)
  {
    return (void*)((char*)(sh) + dwShiftSh);
  };
  //
  union
  {
    const char*  name;
#ifndef X64
    unsigned __int64 nShiftX64_0;
#endif
  };
  union
  {
    DWORD_PTR dwShift;
#ifndef X64
    unsigned __int64 nShiftX64_1;
#endif
  };
  union
  {
    DWORD_PTR dwShiftSh;
#ifndef X64
    unsigned __int64 nShiftX64_2;
#endif
  };
  EValueType   eVal;
  DWORD dwFlag;// Флаг, который надо установить или снять ( железо )
  DWORD dwLog; // Код сообщения записываемого в лог-файл
  UINT  nEnum; // Ссылки на текстовые значения нумераторов
  DWORD dwSize;//размер объекта в байтах, в основном надо для строк
};

typedef enum  EDataTypes
{
// Специфика модели
#undef  DATATYPE
#define DATATYPE( Id, Nomer, Text, Icon ) id_##Id = Nomer,
#include "BaseType.hpp"
  id_ObjInfo   = 19,// 19 - расширенная информация об объекте
  //
  //200 - 202 DeltaV
  //210 - 219 RsLogix
  //230 -     Intouch
  //240 -     ArchestrA Wonderware
  //249 - 408 Honeywell 3xx
  //0x199(409)- OneMnemo, Centum
  //0x200 - 0x53DD Centum
  //22000 - 22599 - ABB, из них до 22299 - базовые, остальные распределяются между отдельными приложениями
  //23000 - 23995 Honeywell 4xx
  //23996 - 23999 QuickBuilder 4xx
  //24000 - 24999 Honeywell 5xx
  //30000 - 31999 TSoft Ручной АРМ
  //32000 - 32009 SiemensS2
  //32010 - 32019 SiemensS7e
  //33000 - 33009 FlexRsu TSoft Ручной АРМ v2
  //33010 - 33019 Prosafe4
  //33020 - 33039 Eld / SUER / Prosoft - Systems/ Epsilon LD
  //40000 - (40002)40099 QuickBuilder 311
  //40100 - 40199 RsLogix500
  //40200 - 40299 MetsoDNA - только передаваемые типы
  //40300 - 40399 Schneider FoxboroEvo
} EDataTypes ;

//
#define MAX_SUMM_NAME 128
#define MAX_DLL_NAME 32
#define MAX_OBJ_NAME 64
#define MAX_PARM_NAME 64
//Sergej 12.04.10 #define NaN 1e10
const unsigned long _nan[2]= { 0xffffffff, 0x7fffffff };
const double NaN = *( double* )_nan;
//
inline bool IsNaN( double d )
{
  return memcmp( &d, _nan, sizeof(d) ) == 0;
}
//
#endif // BASETYPE_H
