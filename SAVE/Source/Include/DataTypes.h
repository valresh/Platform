#pragma once
#include "crossplatform.h"
#include "CommProc.h"



#undef  STD_DEFINE
#define STD_DEFINE( TypeId ) \
enum { TypeID = TypeId };\
  static char * ClassName;\
  int Init();\
  int GetParams(  );\
  int SaveState();\
  int RestoreState( char * StrName );\
  static void BuildList();

#define BLKFLAG_UNDEF  0x00000000
#define BLKFLAG_BLK    0x00000001
#define BLKFLAG_RET    0x00000010

#define id_Forsunka 22
#define id_GasAnalyz 89
#define id_ExternalCondition 3
#define id_System 1
#define id_HS 55

struct CBase
{
    DWORD size;// размер структуры в байтах
    DWORD ID_CLASS;// идентификатор класса
//    DWORD ID_PNT_NAME;// идентификатор конкретной точки связи
//
    CStr Name;
    static CBase * pFirstBase;
    static CBase * pLastBase;
    CBase * pNextBase;
    CBase();
};

struct CShBase
{
    // коды возврата
    enum RetCodes
    {
      rcOK = 0,
      rcNO = 1,
      rcWrong,
    };
    //
    CShBase(int nSize)
    {
      memset( this, 0, nSize );
      m_nSize = nSize;
      RetCode = rcNO;
    }
    //
    int  RetCode; // код возврата
    int  m_nSize;
    int  Busy;    // счетчик подключенных клиентов
    bool m_bAOFS;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#undef  SH_BASE
#define SH_BASE(ShClass) \
ShClass() : CShBase( sizeof(ShClass) ){}\
  static void GetModelData( CBase* pBase, CShBase* pShBase );

struct CBusy : public CBase
{
    int Busy;	// счетчик клиентов
};

struct CFlagsW
{
    int Ctrl_Flags;	 // Управление
    int State_Flags; BYTE Dialog;// Состояние
    int Blk_Flags;   // Блокировки
    int Use_Flags;	 // Использование
    enum// Флаги поля Ctrl_Flags
    {
      //
      OPEN_DOWN = 0x002,// Нажата кнопка открыть(пуск)
      OPEN_UP = 0x020,// Отжата кнопка открыть
      //
      STOP_DOWN = 0x008,// Нажата кнопка стоп
      STOP_UP = 0x080,// Отжата кнопка стоп
      //
      CLOSE_DOWN = 0x004,// Нажата кнопка закрыть(стоп)
      CLOSE_UP = 0x040,// Отжата кнопка закрыть
      //
      NO_ACTIONS = 0x10000,// Не выполнять действий из АРМ
    };
};

struct CFlags : public CFlagsW
{
    int Type_Flags;	 // Особенности устройств
    CFlags(){ memset ( this, 0, sizeof ( CFlags )); };
    //
    enum { // -> Use_Flags
      Model = 0x01 };
    void SaveState( int & Bytes, BYTE * pBuffer );
    void RestoreState( int & Size, BYTE * pBuffer );
    bool Is_Ctrl  ( int Flag ) { return ( Ctrl_Flags  & Flag ) != 0; };
    bool Is_State ( int Flag ) { return ( State_Flags & Flag ) != 0; };
    bool Is_Blk   ( int Flag ) { return ( Blk_Flags   & Flag ) != 0; };
    bool Is_Type  ( int Flag ) { return ( Type_Flags  & Flag ) != 0; };
    bool Is_Use   ( int Flag ) { return ( Use_Flags   & Flag ) != 0; };
    //
    bool Eq_Ctrl  ( int Flag ) { return ( Ctrl_Flags  & Flag ) == Flag; };
    bool Eq_State ( int Flag ) { return ( State_Flags & Flag ) == Flag; };
    bool Eq_Blk   ( int Flag ) { return ( Blk_Flags   & Flag ) == Flag; };
    bool Eq_Type  ( int Flag ) { return ( Type_Flags  & Flag ) == Flag; };
    bool Eq_Use   ( int Flag ) { return ( Use_Flags   & Flag ) == Flag; };
    //
    void On_Ctrl  ( int Flag ) { Ctrl_Flags  |= Flag; };
    void On_State ( int Flag ) { State_Flags |= Flag; };
    void On_Blk   ( int Flag ) { Blk_Flags   |= Flag; };
    void On_Type  ( int Flag ) { Type_Flags  |= Flag; };
    void On_Use   ( int Flag ) { Use_Flags   |= Flag; };
    //
    void Off_Ctrl ( int Flag ) { Ctrl_Flags  &= ~Flag; };
    void Off_State( int Flag ) { State_Flags &= ~Flag; };
    void Off_Blk  ( int Flag ) { Blk_Flags   &= ~Flag; };
    void Off_Type ( int Flag ) { Type_Flags  &= ~Flag; };
    void Off_Use  ( int Flag ) { Use_Flags   &= ~Flag; };
};

struct CForsunkaW
{
    // Изменяются в модели печи
    unsigned char isFire;  // Действительное горение форсунки
    unsigned char canFire; // Физическая возможность загорания форсунки
    // Изменяются в диалоге форсунки
    bool   setFire; // Желание оператора зажечь форсунку
    double VentGas;   // Процент открытия вентиля газа (0-100)
    double ShibGas;   // Процент открытия шибера газа (0-100)
    double ShibGas2;   // Процент открытия шибера газа (0-100)
    double VentMazut; // Процент открытия вентиля мазута(0-100)
    double ShibMazut; // Процент открытия шибера мазута(0-100)
    double VentSteam; // Процент открытия парового вентиля (0-100)??
    double VentPilot; // Процент открытия пилотного вентиля (0-100)??
    double VentGas2;   // Процент открытия вентиля газа (0-100)
    double VentMazut2; // Процент открытия вентиля мазута(0-100)
    double VentSteam2; // Процент открытия парового вентиля (0-100)??
    double VentPilot2; // Процент открытия пилотного вентиля (0-100)??
};

struct CForsunka : public CBusy, public CFlags, public CForsunkaW
{
    STD_DEFINE( id_Forsunka )
    //
    double dKPD;
    //
    enum// Флаги поля Ctrl_Flags
    {
      CHANGE = 0x001, // Кто-то что-то поменял в вентиле или шибере
    };
    enum// Флаги поля State_Flags
    {
      IS_ZAPAL_GAS = 0x0100, // Есть газ на запал
      IS_ZAPAL_FIRE = 0x0200, // Запал горит
      IS_WORK_GAS = 0x0400, // Есть рабочий газ
      IS_WORK_MAZUT=0x800,
      IS_WORK_FIRE = 0x1000, // Рабочий газ горит
    };
    //
};
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
struct CShForsunka : public CShBase, public CFlags, public CForsunkaW
{
    SH_BASE(CShForsunka)
    double dKPD;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};
//
struct CGasAnalyzW
{
    double C[40];// Значения измеряемых параметров - менять не будем, чтобы работали старые тренажеры. В новой жизни не используются
};

struct CGasAnalyz : public CBusy, public CGasAnalyzW
{
    enum { TypeID = id_GasAnalyz };
    static char * ClassName;
    static void BuildList();
    bool bNewStructIsUsed;
    //
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    union { // для экономии размера при передаче в АРМ
        struct {
            // это старые параметры чтобы работали старые тренажеры.
            char Name[40][64];// Названия измеряемых параметров
            BYTE DecimalPlace[40];
        };
        struct
        {
//начинаем новую жизнь~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAX_COMP_GAS_ANANLIZ 128
            double Table[MAX_COMP_GAS_ANANLIZ]; // Все компоненты. Имена не нужны. Они находятся в ...\Projects\xxx\DATA\Equipment\Анализаторы\Компоненты.csv
// Если этот файл лежит - работает новая система, его нет - все по старой
#define MAX_POINTS_GAS_ANANLIZ 100
            double Main_Curve[2][MAX_POINTS_GAS_ANANLIZ]; // Основная кривая[X,Y]. регулярно обновляется
            double Save_Curve[2][MAX_POINTS_GAS_ANANLIZ]; // Сохраненная кривая[X,Y]. обновляется по указанию
            bool CalcData; // Рассчитывать Table, Main_Curve т.к. окно стало выводиться
            bool UseTable;  // Table рассчитана и может использоваться
            bool UseCurve;  // Main_Curve рассчитана и может использоваться
            bool SaveData; // Нажата кнопка сохранить кривую (сбрасывается в модели)
            bool UseSave;  // Save_Curve рассчитана и может использоваться
            bool ClearSave;  // Save_Curve больше рассчитывать не надо
        };
    };
    //
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    int		Init( );
    int		GetParams(  );
    int SaveState( );
    int RestoreState( char * StrName );
};
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
struct CShGasAnalyz : public CShBase
{
    SH_BASE(CShGasAnalyz)
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    char Name[40][64];// Названия измеряемых параметров
    double CC[40];// Значения измеряемых параметров
    BYTE DecimalPlace[40];
    bool bNewStructIsUsed;
#define MAX_COMP_GAS_ANANLIZ 128
    double Table[MAX_COMP_GAS_ANANLIZ]; // Все компоненты. Имена не нужны. Они находятся в ...\Projects\xxx\DATA\Equipment\Анализаторы\Компоненты.csv
// Если этот файл лежит - работает новая система, его нет - все по старой
#define MAX_POINTS_GAS_ANANLIZ 100
    double Main_Curve[2][MAX_POINTS_GAS_ANANLIZ]; // Основная кривая[X,Y]. регулярно обновляется
    double Save_Curve[2][MAX_POINTS_GAS_ANANLIZ]; // Сохраненная кривая[X,Y]. обновляется по указанию
    bool CalcData; // Рассчитывать Table, Main_Curve т.к. окно стало выводиться
    bool UseTable;  // Table рассчитана и может использоваться
    bool UseCurve;  // Main_Curve рассчитана и может использоваться
    bool SaveData; // Нажата кнопка сохранить кривую (сбрасывается в модели)
    bool UseSave;  // Save_Curve рассчитана и может использоваться
    bool ClearSave;  // Save_Curve больше рассчитывать не надо
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};

struct CHsW
{
    bool mOn;  // активирована (true) или выключена блокировка
};

struct CHs : public CBusy, public CFlags, public CHsW
{
    STD_DEFINE( id_HS )
    //
    enum// Флаги поля State_Flags
    {
      HS_PUSHED   = 0x100,
      HS_UNPUSHED = 0x200,
      NOASK    = 0x1000, // Изменение состояния HS неквитировано
      OFFNORMAL= 0x2000, // Ключ находится в ненормальном состоянии
      IS_NOACK=0x800,
    };
    //
    enum// Флаги поля Ctrl_Flags
    {
      ACK  = 0x100, // квитирование
    };
    //
};
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
struct CShHS : public CShBase, public CFlags, public CHsW
{
    SH_BASE(CShHS)
};

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
    };
    union
    {
        DWORD_PTR dwShift;
    };
    union
    {
        DWORD_PTR dwShiftSh;
    };
    EValueType   eVal;
    DWORD dwFlag;// Флаг, который надо установить или снять ( железо )
    DWORD dwLog; // Код сообщения записываемого в лог-файл
    UINT  nEnum; // Ссылки на текстовые значения нумераторов
    DWORD dwSize;//размер объекта в байтах, в основном надо для строк
};

#define MAX_SIMPLE_STRUCT 128
typedef
  void (*tBuildList)();

#define FINITE_0(Flag,Val) \
if ( !_finite(Val)) { if ( Flag ) { ASS ( FALSE ); }; Val = 0.; }

#define MAX_SIMPLE_STRUCT 128
extern tBuildList ClassBuildList[MAX_SIMPLE_STRUCT];

#define INIT(Class) \
  size = sizeof ( Class );\
  ID_CLASS = Class::TypeID;\
  if ( ID_CLASS < MAX_SIMPLE_STRUCT )\
  ClassBuildList[ID_CLASS] = BuildList;

#undef  SHIFT_OBJ
#define SHIFT_OBJ( Params ) \
(DWORD_PTR)(&hs.Params)-(DWORD_PTR)(&hs),\
                                              (DWORD_PTR)(&sh.Params)-(DWORD_PTR)(&sh)

#undef  SHIFT_OBJ2
#define SHIFT_OBJ2( Params,Params2 ) \
                                              (DWORD_PTR)(&hs.Params)-(DWORD_PTR)(&hs),\
                                                                    (DWORD_PTR)(&sh.Params2)-(DWORD_PTR)(&sh)

CBase * CreateStruct( int Type, const char * Name );
int SaveStruct( CBase * );
int RestStruct( CBase *, char * Param );
CBase * FindStruct( int Type, const char * Name );
void GiveCommData ( const char * ObjName, int TypeID, void ** ppObj );
