#pragma once
#include <ST16_LC64_AC.h>

struct CAct_ST16;
typedef void (CAct_ST16::*tA_ST16)( bool Y );
typedef void (CAct_ST16::*tA_ST16_IParm)( bool Y, int nv);
typedef void (CAct_ST16::*tA_ST16_BParm)( bool Y, BYTE nv);
typedef void (CAct_ST16::*tA_ST16_DParm)( bool Y, double nv);
typedef void (CAct_ST16::*tA_ST16_SParm)( bool Y, LPCSTR nv);


struct Y_DATA_API CAct_ST16
{
  LPCSTR Name;
  void * pVar;
  CAlarmBase::tFlagUse_ *pFlagUse;
  char VarType;
  int State;
  bool Active;
  bool L_Type;
  bool dbgH_Type;//только для отладки
  USHORT varSize;
  union
  {
    int Iparam;
    double Dparam;
    LPCSTR Sparam;
    struct tagSExtI
    {
      int dummy;
      int Iparam2;
    }Ex;
  };
  CY_Base *pPreActionApply;
  tA_ST16 pA;
  tA_ST16_IParm pAI;
  tA_ST16_BParm pAB;
  tA_ST16_DParm pAD;
  tA_ST16_SParm pAS;
  tA_ST16_SParm pASA;//.SA.A1
  struct CY_Base *pObj;
//==========================================
  CAct_ST16()
  {
    Name = NULL;
    pVar = NULL;
    pFlagUse = NULL;
    VarType = ' ';
    State = -1;
    L_Type = false;
    dbgH_Type = false;
    varSize = 0;
    Active = false;
    pPreActionApply = NULL;
    pA = NULL;
    pAI = NULL;
    pAB = NULL;
    pAD = NULL;
    pAS = NULL;
    pASA = NULL;
    Dparam = 0;
    pObj = NULL;
  }
  bool IsMethod()
  {
    return pA || pAI || pAB || pAD || pAS;
  }
//=====================================================
  void Act( bool Y, double dt );
//==========================================
  void SET_0( bool Y );
  void SET_H( bool Y );
  void SetInt( bool Y, int thr );
  void SetIntYN( bool Y, int thr );
  void SetIntP_CSV( bool Y, int thr );//D3-73
  void SetByte( bool Y, BYTE thr );
  void SetByte4ACT( bool Y, BYTE thr );
  void SetByteYN( bool Y, BYTE thr );
  void SetDouble( bool Y, double thr );
  void SetMaskInt( bool Y, int thr );
  void SetMaskIntInvert( bool Y, int thr );//D3-45
  void SetString( bool Y, LPCSTR thr );
  void CallSA( bool Y, LPCSTR thr );
};
