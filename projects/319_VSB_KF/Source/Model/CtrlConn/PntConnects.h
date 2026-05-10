#pragma once

struct CFormula
  {
  char Func[16];
  double C;
  double A;
  double Min, Max;
  CFormula()
    {
    memset ( this, 0, sizeof (*this ));
    }
  };

struct CConnectDefect
  {
  double SH, SL;
  double HH, PH, LL, PL;
  int Defect;
  struct CDef * Исправен;
  double CurrSensorData;
  struct CDef * Показания_не_изменяются;
  struct CDef * Завышение_показаний;
  struct CDefParam * parm_Завышение_показаний;
  struct CDef * Занижение_показаний;
  struct CDefParam * parm_Занижение_показаний;
  struct CDef * Уход_показаний_на_максимум;
  struct CDefParam * time_Уход_показаний_на_максимум;
  struct CDef * Уход_показаний_на_начало_шкалы;
  struct CDefParam * time_Уход_показаний_на_начало_шкалы;
  struct CDef * Самопроизвольное_изменение_показаний;
  struct CDefParam * parm_Самопроизвольное_изменение_показаний;
  struct CDefParam * time_Самопроизвольное_изменение_показаний;
  struct CDef * Хаотическое_изменение_показаний;
  struct CDefParam * min_Хаотическое_изменение_показаний;
  struct CDefParam * max_Хаотическое_изменение_показаний;
  struct CDef * Сигнализация_HH;
  struct CDef * Сигнализация_H;
  struct CDef * Сигнализация_L;
  struct CDef * Сигнализация_LL;
  struct CDef * Сигнализация_IOPp;
  struct CDef * Сигнализация_IOPm;
// Дискреты 
  struct CDef * Восстановлен;
  struct CDef * Постоянно_0;
  struct CDef * Постоянно_1;
//
  double Alfa, Start, Finish;
  double CurrTime, Time, p1, p2, Prop;
  bool UseCoef, FixCurrValue;
  double Coef;
//
  CConnectDefect()
    {
    memset ( this, 0, sizeof ( *this ));
    Defect = -1;
    }
  int SetDefect ( struct CDef * pDefect );
  double SetDefect( double V );
  NEW
  };

union  UVal
  {
  int I;
  double D;
  bool B;
  };

union DA
  {
  int D;
  double A;
  };

struct CConnect
  {
  CStr RefFrom;
  CStr RefTo;
  CStr Desc;
  CStr Sens;
  IMM * pSrc;
  IMM * pDst;
  DA SrcOld; // Для типов X и x
  DA DstOld;
  int Line;
  char Type[2];
  bool SetDefect;
  bool UseMinMax;
  CFormula Formula;
  CConnectDefect * pDef;
  struct COP * OP;
  int kOP;
  struct CStableConnect * pStable;
  bool ThisLS;
  double * pTrueValueLS;
  double * pDefectValueLS;
//  CSensor * pSens;
  double PreDstValue;
  double Min, Max;
  double Etalon;
  void Init()
    {
    memset ( this, 0, sizeof ( *this ));
    }
  };

struct CStableConnect
  {
  CConnect * pConn;
  static CStableConnect * pFirst;
  static CStableConnect * pLast;
  CStableConnect * pNext;
  static bool UseStable; // использовать
  static bool Record;    // записывать
  static bool Evaluate;  // Оценивать
  static int kPnt;
  static struct CStableCtrl * pCtrlData;
  static int CallControl;
  union {
    struct {
// Аналог
    double Deviation;
    double Summ;
      };
    struct {
// Дискрет
    int State;
    int Change;
      };
    };
  char Type;
  IMM * pData;
//
  NEW
  CStableConnect( CConnect * pConn );
  void Work( );
  static bool Init( );
  static int Control( );
  };
