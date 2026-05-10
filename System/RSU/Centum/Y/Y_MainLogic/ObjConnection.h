#pragma once
#include <Y_Signal.h>
#include <Y_Class.h>
#include <ST16_AC.h>

struct CObjConnection
{
  enum en_ConnectionType
  {
    ectUndefined = 0,
    ectSignal2Signal,
    ectVar2Signal,
    ectSignalMove2Var,
    ectSignalConvert2Var,
    ectCondition2Signal,
    ectSignal2Action,
    ectSignalMove2Action,
    ectSignalConvert2Action,
  };

  char Out[33];
  BYTE PntOut;
  en_ConnectionType ConnectionType;
  char TypeOut;
  char In[34];
  BYTE PntIn;
  char TypeIn;
  void * pOut;
  void * pIn;
  USHORT sizeIn;
  CSignal Sig;
  int NumbFCS;
  int domain;
  bool bRefOtherFcsOut;
  bool bRefOtherFcsIn;
  CY_Base * pOutObject;
  CY_Base * pInObject;
  char dirIO;
  bool bAction;

  CCond_ST16_LC64 condition;
  CAct_ST16 action;

  union
  {
    double D;
    int I;
    BYTE W;
    bool B;
  } prevSrcValue;
  bool bTest4SrcChanged;

  CObjConnection();
  bool ContinueExchange();
};
