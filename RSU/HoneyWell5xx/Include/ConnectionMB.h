#pragma once
#include <RSUBaseType_.h>
#include <BmBase.h>

class SConnectionMB 
{
protected:
  union uRefs
  {
    BYTE   *pByte;
    int    *pInt;
    double *pDbl;
    char *pSz;
  }varInput, varOutput;

  eVarType typeInput, typeOutput;
  USHORT varSizeI, varSizeO;

public:
  enum eDir : BYTE
  {
    eForward = 0,
    eBackward = 1,
  };
public:
  char szOutObj[36*4];
  char szOutFld[20*4];
  char szInObj[36*4];
  char szInFld[20*4];
  bool enabledTrasfer;
  LPCSTR szTypeObjOut;
  LPCSTR szTypeObjIn;
  eDir direction;
  class KBmBase *objI;
  class KBmBase *objO;
public:
  int g_throughNumb;
public:
  void AssignInputRef( BYTE *pVar, eVarType type, USHORT varSize, KBmBase *outI );
  void AssignOutputRef( BYTE *pVar, eVarType type, USHORT varSize, KBmBase *outO );
  void Transfer();
  void FeedBack2Output( double fdv );
protected:
  void AssignRefImpl( BYTE *pVar, eVarType type, uRefs &var );
};
