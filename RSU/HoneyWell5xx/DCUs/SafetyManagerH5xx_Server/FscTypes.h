#pragma once
#include <BaseType.h>
#include <UniBufferT.h>
#include "../CnfReader/Coord.h"

#pragma pack(push, 1)
typedef struct tagCSIZE
{
    LONG        cx;
    LONG        cy;
} CSIZE, * PCSIZE, * LPCSIZE;
//
struct SGoto
{
  UINT  nData;
  short int nSrc;
  short int nDst;
  BYTE  nIndex;
};
//
struct SGotoN
{
  UINT  nData;
  short int nSrc;
  short int nDst[5];
  BYTE  nIndex;
};
//
struct STrigger
{
  UINT Q;// Timer output
  UINT u;// Remaining time value
  UINT X;// Старое состояние входа S
  UINT S;// Start timer
  UINT R;// Reset timer
  UINT a;// изменение t
  double t;// Time value + base (time constant)
  char   m;// Для отображения единиц
  BYTE base;
};
//
struct LFscFile
{
  UINT nNumber;//Номер файла
  int nFromFldCall;
  // Иерархия файлов в списке SFscFile
  UINT nParent;// Родитель
  UINT nChild; // Первая процедура
  UINT nNextC; // Следующая процедура
  //
  // Список объектов в SFscList
  UINT nCount;
  UINT nPlace;
  //
  CSIZE szDoc; //Размер документа
  //
  char szTagName[32*4];
  char szTitle[128*4];
};
//
struct LFscBase
{
  enum typeB : unsigned char
  {
    unknown = 0,
    COM = 1,
    FLD = 2,
    POLE = 3,
    SYS = 4,
    COUNT,
    HZ,
  };
  DWORD ID;
  SRECT rect;
  SRECT realSMrect;
  UINT nBuff;
  unsigned char mT;//
  typeB mB;
  UINT nNumberFld;
  int nFromFldCall;
  UINT uniNum;
  /*union ???
  {
    double dblLocal;
    bool   flLocal;
  };
  void *pFldCopyTo;*/
  LFscBase()
  {
    ZeroMemory( this, sizeof(*this) );
  }
  bool ValidateB()
  {
    if( mB>=LFscBase::unknown && mB < LFscBase::COUNT)
      return true;
    ASS(0);
    return false;
  }
};
//
#pragma pack(pop)

struct SPointIn //Для 0x0B и 0x0C (входы из модели)
{
  int  nDbf;// Номер в DBF-файле
  UINT nOut;//Ссылка  SFscDots
  void*  In;//Указатель на объект модели (GiveCommData)
};
//
struct SPointOut //Для 0x1A и 0x19 (выходы в модель)
{
  int  nDbf;// Номер в DBF-файле
  UINT  nIn;//Ссылка  SFscDots
  void* Out;//Указатель на объект модели (GiveCommData)
};
// Сохраняемые переменные
struct SDotValue
{
  SDotValue(EValueType type = enumValueUnk)
  {
    memset( this, 0, sizeof(SDotValue) );
    eType = type;
  }
  DWORD    dwFlags;
  EValueType eType;
  union { double dVal; int nVal; char cVal; bool bVal; float fVal; char sVal[8]; __int64 lVal; };
  //
  bool Bol()
  {
    switch ( eType )
    {
    case enumValueBol: return bVal;
    case enumValueDbl: return (dVal != 0) ? true : false;
    }
    // Здесь не должны быть
    return bVal;
  }
  //
  double Dbl()
  {
    switch ( eType )
    {
    case enumValueBol: return bVal ? 1 : 0;
    case enumValueI64:
    case enumValueDbl: return dVal;
    case enumValueInt: return nVal;
    }
    // Здесь не должны быть
    return dVal;
  }
  //
  void Set( bool val )
  {
    switch ( eType )
    {
    case enumValueDbl: dVal = val ? 1 : 0;break;
    case enumValueInt: nVal = val ? 1 : 0;break;
    case enumValueBol: bVal = val;break;
    default:
      KKK();
    }
  }
  //
  void Set( double val )
  {
    switch ( eType )
    {
    case enumValueInt: nVal = (int)val;break;
    case enumValueDbl: dVal = val;break;
    case enumValueBol: bVal = (val != 0) ? true : false;break;
    default:
      KKK();
    }
  }
};
