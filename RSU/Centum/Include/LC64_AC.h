#pragma once
#include <ST16_LC64_AC.h>

struct CAct_LC64;
typedef void (CAct_LC64::*tA_LC64)();
typedef void (CAct_LC64::*tA_LC64_IParm)(int nv);
typedef void (CAct_LC64::*tA_LC64_BParm)(BYTE nv);
typedef void (CAct_LC64::*tA_LC64_DParm)(double nv);
typedef void (CAct_LC64::*tA_LC64_IParmState)(int nv, bool State);
typedef void (CAct_LC64::*tA_LC64_WParmState)(BYTE nv, bool State);

struct Y_DATA_API CAct_LC64
{
  char * Name;
  void * pVar;
  char VarType;
  int State;
  bool Active;
  bool L_Type;
  bool P_Type;
  struct CY_Base *pObj, *pObj4ExternAlarm;
  union
  {
    int Iparam;
    double Dparam;
  };
  tA_LC64 pA;
  tA_LC64_IParm pAI;
  tA_LC64_BParm pAB;
  tA_LC64_DParm pAD;
  tA_LC64_IParmState pAIstate;
//==========================================
  CAct_LC64()
  {
    Name = NULL;
    pVar = NULL;
    VarType = ' ';
    State = -1;
    L_Type = false;
    P_Type = false;
    Active = false;
    pA = NULL;
    pAI = NULL;
    pAB = NULL;
    pAD = NULL;
    pAIstate = NULL;
    Dparam = 0;
    pObj = NULL;
    pObj4ExternAlarm = NULL;
  }
  bool IsMethod()
  {
    return pA || pAI || pAB || pAD || pAIstate;
  }
//==========================================
  void Act( BYTE State )
  {
    Active = State ? true : false;
    if( State )
    {
      if( pObj )
      {
        pObj->CallSA( Name );
      }
      if( pA )
        (this->*pA)();
      else if( pAI )
        (this->*pAI)( Iparam );
      else if( pAB )
        (this->*pAB)( Iparam );
      else if( pAD )
        (this->*pAD)( Dparam );
      if( pObj )
        pObj->OnAssigned( pVar );
    }
    else
    {
      LPCVOID pCanByValue = NULL;
      if( pAI )
        pCanByValue = &Iparam;
      else if( pAB )
        pCanByValue = &Iparam;
      else if( pAD )
        pCanByValue = &Dparam;
      if( pObj )
        pObj->OnNoAssignedByFalseState( pVar, pCanByValue );
    }
    if( pAIstate )
      (this->*pAIstate)( Iparam, State ? true : false );
  }
//==========================================
  void SET_0();
  void Set_P( bool bChange, bool bState );
  void SetInt( int thr );
  void SetByte( BYTE thr );
  void SetDouble( double thr );
  void SetIntP_CSV( int thr, bool Y );
  void SetIntALRM( int thr, bool Y );
  void SetMaskInt( int thr, bool Y );
  NEWY
};
