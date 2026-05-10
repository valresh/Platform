#pragma once
#include <MemoryServiceY.h>
#include <Y_export.h>

namespace ns_CALCU
{
  enum eType
  { 
    None, 
    Const, 
    Var,
    ReferenceToField = Const + 2,
    Condition,
  };

  enum eTypeValue
  {
    HZ,
    INTEGER = 'I',
    REAL = 'D',
    FLAG = 'W',
    STRING = 'S',
    IntFromReal = 'R'
  };
}

struct SVarRetainedPart
{
  ns_CALCU::eType m_Type;
  ns_CALCU::eTypeValue m_TypeVar;
  char NameValue[28];
  union
  {
    BYTE wValue;
    int iValue;
    double dValue;
  };
};

struct SLocalVar
{
  char NameVar[28];
  int nInVars;
  ns_CALCU::eTypeValue m_TypeVar;
  union
  {
    BYTE wValue;
    int iValue;
    double dValue;
  };
  NEWY
};

class Y_DATA_API KCalcVar : public SVarRetainedPart
{
public:
  static bool IsInt(const char* ptr);
  static bool IsFlt(const char* ptr);
public:
  KCalcVar();
  void Init( struct CY_Base *owner );
  template<size_t _S>
  void OutValue( char (&txt)[_S] )
  {
    OutValue( txt, _S );
  }
  template<size_t _S>
  void GetBeautyName( char (&txt)[_S] )
  {
    GetBeautyName( txt, _S );
  }

  struct CY_Base *m_Obj;
  struct CSignal *m_pSignal;//терминальный выход для переменой OUT <- CPV

  typedef void (KCalcVar::*tCompare)();
  tCompare m_pCompare;
  void LE();
  void LT();
  void GE();
  void GT();
  void EQ();
  void CompareFlagArrayByte4ST16();
  NEWY
protected:
  bool Numb();
  void OutValue( char *txt, size_t _S );
  void GetBeautyName( char *txt, size_t _S );

protected:
  union 
  {
    int    *piValue;
    double *pdValue;
    BYTE   *pwValue;
    LPSTR  pszValue;
  };

  union
  {
    BYTE cond_wValue;
    int cond_iValue;
    double cond_dValue;
  };
#pragma warning( push )
#pragma warning( disable : 4341 )
  enum eCondVals : char
  {
    condHZ = 0,
    condBAD = SCHAR_MIN,
    condGOOD = SCHAR_MAX,
  };
#pragma warning( pop )
  //для обработки качества сигнала =BAD, 0 не задан, 1-хорошо, -1-BAD
  eCondVals ecvCondConstRes;

  USHORT saCount;
  int assignConditionAsIs;

private:
  void AssignCondConstI( int &cValue, LPCSTR pCondName, LPCSTR pFieldName );
  void AssignCondConstW( BYTE &cValue, LPCSTR pCondName, LPCSTR pFieldName );
  void AssignCondConstD( double &cValue, LPCSTR pCondName, LPCSTR pFieldName );
};
//////////////////////////////////////////////////////////////////////////
class KCalcRes : public KCalcVar
{
public:
  BOOL m_bAddrVar;
  ns_CALCU::eTypeValue *m_pTypeVar;
public:
  KCalcRes();
  
  void Reset();
  bool IsReal();
  bool IsInteger();
  bool IsFlag();
  bool IsUnknown();
  bool IsString();

  KCalcRes& operator = ( const KCalcVar &thr );
  KCalcRes& operator = ( const double &thr );
  KCalcRes& operator = ( const int &thr );
  KCalcRes& operator = ( LPCSTR thr );
  operator double();
  operator int();
  operator BYTE();
  operator LPCSTR();
protected:
  //KCalcVar m_var;
};