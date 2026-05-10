#pragma once
#include <stdlib.h>
#include <crosswindows.h>
#include <crossplatform.h>

enum
{
  yokoNameLength = 16,
};
//
struct SYokogawaID
  {
  union
  {
    DWORD dwID   ;// Идентификатор файла
    char szID[4];
  };
  DWORD dwShift;// Начало данных
  DWORD dwCount;// Количество записей
  DWORD dwSize ;// Размер одной структуры
  void* Item( void* Void, DWORD n = 0 )
    {
    return ((char*)Void+dwShift+dwSize*n);
    }
  };
//
struct SYokoEdfVersion
{
  DWORD mustBeNULL;
  USHORT minorVer;
  USHORT majorVer;
  DWORD mustBe_x2;
  DWORD mustBe_x10;
};
// Стандартный header
struct SYokogawa40
{
  char opgr[10];
  char type[ 6];
  char mypjt[8];
  char users[8];
  SYokoEdfVersion version;
  int32_t tmCreate;// Время создания станции
  int32_t tmChange;// Время последнего изменения
  DWORD dwUnk2[2];
};
//
struct SYokogawa200 : public SYokogawa40
{
  SYokogawaID id[28];
};

inline SYokogawaID* FindYokoID( SYokogawa200* hdr, const char* type )
{
  LPDWORD dw = LPDWORD(type);
  for ( int n = 0; n < _countof(hdr->id); n++ )
  {
    if ( dw[0] == hdr->id[n].dwID )
      return &hdr->id[n];
  }
  return NULL;
}
//
struct S_190_COMMON
{
  char szName[16];
  char szRef[64];
  DWORD dummy1[16];
  char szType[16];
  char szComment[32];
  char dummy2[48];
  char szUpperWindow[16];
  BYTE cAlarmLevel;
  char cLevel;
  USHORT ushNumber;
  char hz2[2];
  USHORT y_type;
  char hz3[2];
  BYTE cTagMark;
  char hz4[42];
  BYTE fl;
  USHORT hz5;
  char engUnit[8];
};

struct S_1A0
{
  char szName[16];
  char szRef[64];
  DWORD dummy1[16];
  char szType[16];
  char szComment[32];
};

struct SGrData
{
  char szName[16];
  char szComm[24];
  char szMame[16];
  WORD  wUnk1;//??? 100
  WORD  wUnk2;//??? 800
  DWORD dwNumber;
  DWORD dwUnk[4];
  WORD  wUnk3;
  WORD  wScale;
};
//
struct SGsData1
{
  enum eYokoType
  {
    eytGraphic = 0,
    eytOverview = 1,
    eytControl = 2,
    eytTrend = 4,
  };
  WORD wType;// 0-GR, 1-OV, 2-CG
  WORD wPar ;// Номер родителя
  WORD wNext;// Следующий
  WORD wPrev;// Предыдущий
  WORD wChild;
  WORD wXZ;// 0-для GR,OV,CG 1-для TG
  WORD wUnk1;// Всегда 0?
  WORD wUnk2;// Всегда 0?
  WORD wUnk3;// 0-для GR,OV,CG Разные номера для TG
  WORD wUnk4;// Всегда 0?
  //DWORD u6,u7,u8;// Непонятные нули
};
//
struct SGsData2
{
  char szName[16];
  WORD dwYou,// Личный номер
        dwPar,// Номер родителя
        dwNext,// Следующий номер
        dwPrev,// Предыдущий номер
        dwChild;// Первый дочерний индекс
  //u1,u2,u3;// Непонятные нули
};

// Структура файла TagRef.rdb
struct STempTagRef
{
  DWORD dwNumber;
  DWORD dwYesDat;//1 или 0
  DWORD dwUnk2;//0
  DWORD dwUnk3;//0
  DWORD dwUnk4;
  DWORD dwUnk5;
  DWORD dwUnk6;
  DWORD dwUnk7;//0
  char  szFCS [24];
  char  szFile[28];
  char  szName[20];
  char  szCategory[12];
  char  szType[20];
  DWORD dwType;
  DWORD dwUnk9;// Какое-то число, одинаковое для одного типа
  char  szComm[32];// Судя по Омску - это лажа. Может не совпадать с оригиналом.
};

struct STATION
{
  char szName[8];
  char szType[8];
  char szComm[32];
};
#pragma warning( push )
#pragma warning( disable : 4309 4369)
enum eEdfType : DWORD
{
  edtBlInDR = (unsigned int)(- 2),
  edtSCS = (unsigned int)(- 1),
  edtUnknown = 0,
  edtIOM_min = 0x0300,
  edtIOM_3d2 = 0x03d2,
  edtIOM_P_32DI = 0x3e6,
  edtIOM_64DI = 0x3e7,
  edtIOM_32PushButtonInput = 0x3ed,
  edtIOM_3e0 = 0x03e0,
  edtIOM_P_32DO = 0x3ef,
  edtIOM_64DO = 0x3f0,
  edtIOM_PulseOutput = 0x3fe,
  edtIOM_input = 0x3fd,
  edtIOM_655 = 0x0655,
  edtIOM_65d = 0x065d,
  edtIOM_665 = 0x0665,
  edtIOM_667 = 0x0667,
  edtIOM_max = 0x0700,
  edtStnDef = 0x2113,
  edtCommDataXWW = 0x2115,
  edtCommonSwitch = 0x2116,
  edtAnnunciator = 0x2117,
  edtOG = 0x2118,
  edtSD = 0x2119,
  edtEV = 0x211a,
  edtDR = 0x211b,
  edtGlobalSwitch = 0x2132,
  edtCommonIO = 0x2142,
  edtEquipment = 0x214b,
};
// Структура проекта в FcsRef.rdb FCS-a
struct SFILD
{
  DWORD number;
  DWORD dwYesDat;//1 или 0
  DWORD hz1[6];
  char relDir[4*(10*6+1)];
  char fileName[4*7];
  char hz2[8];
  eEdfType type;
  DWORD hz3[9];
};
#pragma warning( pop )

//элементы в FcsRef.rdb FCS-a
struct SELMD
{
  DWORD num;
  DWORD dwYesDat;//1 или 0
  char hz1[36];
  char edf[28];
  char tagName[16];
  DWORD hz2;
  char type1[12];//%BL, %Z ... 
  DWORD elementNumber;
  char szType_Number[20];
  char blkType[12];//CALCU %Z
  DWORD dwType;
  DWORD hz3;
  char comment[124];
  char szNumbs[4][16];
  DWORD hz4[3];
  DWORD hz5;
  DWORD hz6;
  DWORD hz_1;
  DWORD hz_1_[5];
  DWORD hz_2;
  DWORD hz_2_[5];
  DWORD hz_3;
  DWORD hz_3_[2];
};

struct SUDLB
{
  char Terminal[16];//%Z...
  union
  {
    char hz[16];//для совместимости с ранними исходниками
    char P_IdTagName[16];
  };
  char Label[16];//%%PT512
  char ServiceComment[48];
};

struct SUDLB_SMALL
{
  char empty[16];
  char szTagName[16];
};

struct SIOEX
{
  char P_IDTagName[256];
};

struct SHTLT
{
  char TagName[16];
  char TagComment[24];
  char UppenWindow[16];
  BYTE  btAlarm;
  BYTE  btLvl;
  BYTE  btUnk1,btUnk2;
  SHORT num;
  SHORT hz2;
  SHORT type;
  BYTE  btEquipment;
  char szUnk1[10];
  BYTE  btTagMark;
  char  szUnk2[6];
  WORD  wFlag;
  BYTE color1,color2;
  char  szUnk3[0x28];
};

struct SDTLD
{
  char szBtns[0x30];
};

struct SHTL1
{
  char TagName[16];
  char TagComment[40];
  BYTE  btAlarm;
  BYTE  btLvl;
  BYTE  btUnk1,btUnk2;
  DWORD Numb;
  WORD  wType,wUnk1;
  char  szUnk1[9];
  BYTE  btTagMark;
  char  szUnk2[6];
  WORD  wFlag;
  BYTE color1,color2;
  char  szUnk3[0x28];

};

struct STAGI
{
  char TagName[16];
  DWORD hz;
  char ModelName[8];
};

struct SASGN
{
#pragma pack(push)
#pragma pack(1)
  struct SProgramCode
  {
    BYTE be1;//всегда 1
    SHORT zero;
    SHORT nIOM;
    SHORT nSlot;
  };
#pragma pack(pop)
  BYTE hasData;
  SProgramCode ProgramCode;
  DWORD numStartWW;
  DWORD Buffer;
  char name[8];
  int addrCount;
  DWORD f;
};

struct SINFO_WW
{
  enum EReverse
  {
    erNo = 0,
    erBits = 1,
    erNo2 = 2,
    erWords = 4,
  };
  USHORT Station;
  SHORT Size;
  char Device_and_Address[8];
  USHORT endPrev;
  BYTE dataType;
  BYTE ioType;
  BYTE Reverse;
  BYTE PortFrom0;
  USHORT startBlockIndexFrom0;
};
// точки линий рисунка
struct SGCOD
{
  SHORT x, y;
};

struct SGBLK
{
  union
  {
    SHORT type;// 1-block, 2-PIO
    BYTE GeomType;
  };
  SHORT Numb;
  union
  {
    int Tp;
    BYTE GeomSymbol;
  };
  SHORT x, y;
};

struct SGBLK_LC64_HDR : SGBLK
{
};

struct SGCNC_LC64
{
  SHORT Dummies[4];
  SHORT ObjFrom;
  SHORT NodeFrom;
  SHORT ObjTo;
  SHORT NodeTo;
};

struct SGCOD_LC64
{
  SHORT x, y;
};

struct SUDIT_SFC
{
  enum EDataType1
  {
    edt1Unknown = 0,
    edt1I16     = 0x0C,
    edt1I32     = 0x14,
    edt1F32     = 0x1C,
    edt1F64     = 0x20,
    edt1CHR     = 0x08, 
  };
  struct STypeSize
  {
    BYTE type;
    BYTE sizeinBytes;
  };

  char DataItem[8];
  char Comment[32];
  char EngUnitSymbol[88];
  union
  {
    USHORT DataType;
    STypeSize sizedType;
  };
  USHORT dummy1;
  USHORT Array1, Array2;
  BYTE dummy2[8];
  char DisplayFormat[8];

  static EDataType1 CheckDataType( USHORT dt )
  {
    STypeSize ts;
    std::memcpy( &ts, &dt, sizeof(ts) );
    static const EDataType1 s_edts[] = { edt1I16, edt1I32, edt1F32, edt1CHR, edt1F64 };
    int iTest = 0;
    for( iTest; iTest<_countof(s_edts); iTest++ )
    {
      if( ts.type==s_edts[iTest] )
        return s_edts[iTest];
    }
    return edt1Unknown;
  }
  static USHORT GetElementSize( USHORT dt )
  {
    STypeSize ts;
    std::memcpy( &ts, &dt, sizeof(ts) );
    switch( ts.type )
    {
    case edt1I32:
    case edt1I16:
      return sizeof(int);
    case edt1F32:
    case edt1F64:
      return sizeof(double);
    case edt1CHR:
      return sizeof(char)*ts.sizeinBytes;
    }
    return sizeof(double);
  }
  static char GetType( USHORT dt )
  {
    STypeSize ts;
    std::memcpy( &ts, &dt, sizeof(ts) );
    switch( ts.type )
    {
    case edt1I32:
    case edt1I16:
      return 'I';
    case edt1F32:
    case edt1F64:
      return 'D';
    case edt1CHR:
      return 'S';
    }
    return '\0';
  }
};

struct SScsArea
{
  DWORD numStartWW;
  DWORD Buffer;
  DWORD programCode;
  DWORD d0;
  char name[8];
  DWORD addrCount;
  DWORD f;
};

struct SScsInfo_WW
{
  WORD endPrev;
  WORD Size;
  WORD Port;
  WORD n00;
  DWORD n01;
  WORD Station;
  BYTE dataType;
  BYTE n02;
  char Device_and_Address[8];
  BYTE Reverse;
  BYTE InputprocessingAtFault;
  WORD n03[3];
  WORD o30;
  WORD n04[15];
};

struct SScsWIRE
{
  DWORD numbTerminal;
  DWORD n01[3];
  char VariableName[16];
  BYTE n02[112];
};

struct SMSRQ //HISxxxx\CONFIGURATION\MsgReqDef.edf есть еще SMSR2
{
  DWORD hz;
  char szStationName[8];
  char Function[52];
};

// Структура файла HisRef.rdb
struct STempHisRef// 0x60
{
  DWORD dwNumber;// Номер
  DWORD dwYesDat;// Наличие данных
  DWORD null1,null2;// Нули
  DWORD u1,u2,u3,null3;
  char  szMnem[20],szType[20],szName[24];
};

// Структура файла DRxxxx.edf
//оснвыне блоки
struct SRGTL
{
  char Name[16];
  DWORD hz;
  char Model[12];
  char cXZ[16];
};
// ссылки (PIO)
struct SFREF
{
  char Model[16];
  char Name[16];
  char cXZ[32];
};
