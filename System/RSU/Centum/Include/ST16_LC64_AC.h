#pragma once

struct CCond_ST16_LC64;
typedef void (CCond_ST16_LC64::*tC_ST16_LC64)();
typedef void (CCond_ST16_LC64::*tC_ST16_LC64_IParm)(int thr);
typedef void (CCond_ST16_LC64::*tC_ST16_LC64_BParm)(BYTE thr);
typedef void (CCond_ST16_LC64::*tC_ST16_LC64_FParm)(bool thr);
typedef void (CCond_ST16_LC64::*tC_ST16_LC64_I64Parm)(__int64 thr);

struct Y_DATA_API SCond_ST16_LC64_Consts
{
  SCond_ST16_LC64_Consts()
  {
    Const_Yes = false;
    Const_No = false;
  }
  bool Const_Yes;
  bool Const_No;
};

struct Y_DATA_API CCond_ST16_LC64 : SCond_ST16_LC64_Consts
{
  char * Name;
  void * pVar;
  CAlarmBase::tFlagUse_ * pFlagUse;
  char VarType;
  BYTE Yes;
  tC_ST16_LC64 pC;
  tC_ST16_LC64_IParm pCIP;
  tC_ST16_LC64_BParm pCBP;
  tC_ST16_LC64_FParm pCbool;
  tC_ST16_LC64_I64Parm pCI64P;
  struct CY_Base *pObj;
  int Iparam;
  union
  {
    __int64 I64param;
    USHORT saCount;
  };
  CCond_ST16_LC64()
  {
    Name = NULL;
    pVar = NULL;
    pFlagUse = NULL;
    VarType = ' ';
    Yes = false;
    pC = NULL;
    pCIP = NULL;
    pCBP = NULL;
    pCbool = NULL;
    pObj = NULL;
    Iparam = 0;
    I64param = 0LL;
    saCount = 0;
  }
  bool IsMethod()
  {
    return pC || pCIP || pCBP || pCbool || pCI64P;
  }
  //=====================================================
  void Cond()
  {
    if( Const_Yes )
    {
      Yes = true;
      return;
    }
    if( Const_No )
    {
      Yes = false;
      return;
    }
    if( pObj )
    {
      pObj->CallSA( Name );
    }
    if( pC )
      (this->*pC)();
    else if( pCI64P )
      (this->*pCI64P)( I64param );
    else if( pCIP )
      (this->*pCIP)( Iparam );
    else if( pCBP )
      (this->*pCBP)( (BYTE)Iparam );
    else if( pCbool )
      (this->*pCbool)( Iparam ? true : false );
  }
  //=====================================================
  void LE();
  void LT();
  void GE();
  void GT();
  void EQ();
  void AND();

  void CompareInt( int thr );
  void CompareIntAnd( int thr );
  void CompareIntCalcuActOn( int thr );
  void CompareByte( BYTE thr );
  void CompareArrayByte4ST16( BYTE thr );
  void CompareBool( bool thr );
  void CompareInt64And( __int64 thr );
  NEWY
};
