#pragma once
#include <BaseType.h>
//#include <crossplatform.h>
#include <CommProc.h>
#include </home/resh/Platform/RSU/system/CBase.h>
//#pragma warning( disable : 444 ) //Отключаем предупреждение о невиртуальности

#include <QtCore/QtGlobal>

#ifdef UTILSH5XX_EXPORTS
#define UTILSH5XX_API Q_DECL_EXPORT
#else
#define UTILSH5XX_API Q_DECL_IMPORT
#endif

struct UTILSH5XX_API CAlarmBase : CRSUBase
{
  BYTE btEHLN;// Уровень сигнализации (enum STATUS)
  BYTE btType;// Тип сигнализации     (enum MESS  )
  BYTE notACK;// 0- квитированный 1 - не квитированный
  BYTE setACK;// Оператор изъявил желание квитировать объект
  union
  {
    BYTE changedFromArm[2];//что-то изменилось армом
    WORD cfa;
  };
  BYTE mUse;  // 0x01 - read FSC, 0x10 - write FSC
  BYTE mUnit; // Номер Unit-а в списке
  BYTE mRsrv2;// Свободно
  int Shelved;//оставшееся время до включения сигнализации ms
};

struct UTILSH5XX_API SPntTypeInfo : CRSUBase
{
  enum eNonameType
  {
    entObj = 11,
    entIO = 22,
  };
  DWORD realTypeID;
  eNonameType nObjOrIO;//0 - obj, 1 - IOs хранилище
  DWORD pntID;
  DWORD classID;
};


enum eVarType
{
  evtHZ = 0,
  evtBool,
  evtByte,
  evtUShort,
  evtInt,
  evtDouble,
  evtString,
  evtTime,//time_t
  evtFloat,
  evtUInt64,
  evtInt2,
  evtInt4,
};

enum
{
  id_OneMnemoHoney = 24000,
  id_CBHSystem = 24001,
  id_AcyMin = 24002,
  // Специфика
  #undef   HONEY_TYPE
  #define  HONEY_TYPE( a, b, c ) id_##b = a,
  #include "./HoneywellType.hpp"
  id_AcyMax = 24995,
  id_PntTypeInfo = 24995,
  id_OneAlmGrHoney = 24996,
};

inline bool IsAcy( UINT nType )
{
  return id_AcyMin <= nType && nType < id_AcyMax;
}
