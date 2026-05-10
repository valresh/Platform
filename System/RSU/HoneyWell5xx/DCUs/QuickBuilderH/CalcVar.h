#pragma once
#include <BaseType_.h>
#include "Trans.h"
#include <MemoryServiceQB5xx.h>

class KCalcVar : public KVar
{
protected:
  eVarType type;
  BYTE size;
  BYTE *pMem;

  union
  {
    char szValue[64*4];
    int int4;
    short int2;
    double dbl;
    bool bl;
  };

public:
  bool m_bAddrVar;
  class KQbBase* m_pObj;
public:
  KCalcVar();
  KCalcVar( KCalcVar &thr );
  void operator = ( KCalcVar &thr );
  operator LPCSTR();
  bool GetBool();
  bool NotLinked();
  void operator = ( bool v );
  void Set( short r );
  void Set( double r );
  NEWQB
public:
  bool Init();
  void InitTo( BYTE *pAddr, eVarType Type, USHORT varSize );
  void Compare( KCalcVar &res, KCalcVar &thr, eToken_value cmd );
  void AssignFrom( KCalcVar &thr );
  void Action( KCalcVar &res, KCalcVar &thr, eToken_value cmd );
private:
  void CopyFrom( KCalcVar &thr );
  void Clear();
  template<typename L, typename R>
  bool Compare( L l, R r, eToken_value cmd );
  template<typename T, typename R>
  T Action( T l, R r, eToken_value cmd );
};
