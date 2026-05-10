#pragma once
#include <DataTypes_.h>
#include <MemoryServiceH5xx.h>
#include <HoneywellStruct.h>
#include <BmBase.h>

struct SConnectionsCont
{
  SConnectionMB *pInConns;
  DWORD inConsC;
};


struct SH_Block : SConnectionsCont, KBmBase
{
  virtual void StepT( SStepCalcParams &dt );
  virtual void InputConnectionsTransfer();
  virtual void OutputConnectionsTransfer();
  virtual BOOL IsBlock();
  virtual KBmBase* WhoHasConnection( LPCSTR pszObj, LPCSTR pszFld, bool bOutput, LPCSTR *ppFld );
  virtual void SelectedInSCM(){}//вызывается при переключении на него в последовательностях
  static bool s_DebuggerInitializing;
  static bool s_bProjectLoaded;
};

template<class R, class C, class S>
R * CreateImpl( LPCSTR dcuName, LPCSTR hmiName, KMemoryServiceH5xx &pInst, CBase*(KMemoryServiceH5xx::*fnCreate)( LPCSTR, DWORD, int) )
{
  char bufUp[48*4];
  strcpy_s(bufUp, hmiName);
  _Ktoupper(bufUp);
  S *W = (S*)((pInst.*fnCreate)( bufUp, S::TypeID, 0 ));
  C * pClass = new C();
  pClass->W = W;
  pClass->H_Type = S::TypeID;
  return pClass;
}

template<class R, class C, class S>
R * CreateImpl_SP( LPCSTR dcuName, LPCSTR hmiName, LPCSTR pszRef2RealBlock, KMemoryServiceH5xx &pInst, CBase*(KMemoryServiceH5xx::*fnCreate)( LPCSTR, DWORD, int) )
{
  char bufUp[48 * 4] = {};
  strcpy_s(bufUp, hmiName);
  _Ktoupper(bufUp);
  S *W = NULL;
  if( pszRef2RealBlock && *pszRef2RealBlock )
  {
    char buf2Up[96 * 4] = {};
    strcpy_s(buf2Up, pszRef2RealBlock);
    _Ktoupper(buf2Up);

    W = (S*)((pInst.*fnCreate)( buf2Up, S::TypeID, 0 ));
    KMemoryServiceH5xx::Instance().CreateIoRefToData( bufUp, W , S::TypeID);
  }
  else
  {
    W = (S*)((pInst.*fnCreate)( bufUp, S::TypeID, 0 ));
  }
  C * pClass = new C();
  pClass->W = W;
  pClass->H_Type = S::TypeID;
  return pClass;
}

#define SH_CREATE0(Class,Struct) \
  static SH_Block * Create( LPCSTR dcuName, LPCSTR hmiName )\
{\
  return CreateImpl<SH_Block, Class, Struct>( dcuName, hmiName, KMemoryServiceH5xx::Instance(), &KMemoryServiceH5xx::CreateObjectsStruct );\
}

#define SH_CREATE_IO0(Class,Struct) \
  static SH_Block * Create( LPCSTR dcuName, LPCSTR hmiName )\
{\
  return CreateImpl<SH_Block, Class, Struct>( dcuName, hmiName, KMemoryServiceH5xx::Instance(), &KMemoryServiceH5xx::CreateIOsStruct );\
}

#define SH_CREATE_IO_SP(Class,Struct) \
  static SH_Block * Create( LPCSTR dcuName, LPCSTR hmiName, LPCSTR pszRef2RealBlock )\
{\
  return CreateImpl_SP<SH_Block, Class, Struct>( dcuName, hmiName, pszRef2RealBlock, KMemoryServiceH5xx::Instance(), &KMemoryServiceH5xx::CreateIOsStruct );\
}

struct SH_FLAG : SH_Block
{
  W_FLAG *W;
  SH_CREATE0(SH_FLAG, W_FLAG)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  virtual bool IsArmAssigned();
  virtual void OnAssignField( LPCSTR pszFieldName );
  NEWH
};

struct SH_DEVCTL : SH_Block
{
  W_DEVCTL *W;
  SH_CREATE0(SH_DEVCTL, W_DEVCTL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  virtual void OnAssignField( LPCSTR pszFieldName );
  NEWH
};

struct SH_DICHANNEL : SH_Block
{
  BYTE *pSlave;
  W_DICHANNEL *W;
  SH_DICHANNEL();
  SH_CREATE_IO0(SH_DICHANNEL, W_DICHANNEL)
  static void InitParm();
  virtual void SetParmList();
  virtual LPCSTR SetDestIO( KBmBase *dest, LPCSTR pszField );
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_DOCHANNEL : SH_Block
{
  double prevONOFFPULSE;
  double timerTo0;
  BYTE *pMaster;
  W_DOCHANNEL *W;
  SH_DOCHANNEL();
  SH_CREATE_IO_SP(SH_DOCHANNEL, W_DOCHANNEL)
  static void InitParm();
  virtual void SetParmList();
  virtual LPCSTR SetDestIO( KBmBase *dest, LPCSTR pszField );
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_AICHANNEL : SH_Block
{
  double *pSlave;
  W_AICHANNEL *W;
  SH_AICHANNEL();
  SH_CREATE_IO0(SH_AICHANNEL, W_AICHANNEL)
  static void InitParm();
  virtual void SetParmList();
  virtual LPCSTR SetDestIO( KBmBase *dest, LPCSTR pszField );
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_AOCHANNEL : SH_Block
{
  double *pSlave;
  W_AOCHANNEL *W;
  SH_AOCHANNEL();
  SH_CREATE_IO0(SH_AOCHANNEL, W_AOCHANNEL)
  static void InitParm();
  virtual void SetParmList();
  virtual LPCSTR SetDestIO( KBmBase *dest, LPCSTR pszField );
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SPARE : SH_Block
{
  W_SPARE *W;
  SH_SPARE();
  SH_CREATE0(SH_SPARE, W_SPARE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};
struct SH_DATAACQ : SH_Block
{
  W_DATAACQ *W;
  SH_CREATE0(SH_DATAACQ, W_DATAACQ)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void GetParams( KHBridge2SysParam &params );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_NUMERIC : SH_Block
{
  W_NUMERIC *W;
  SH_CREATE0(SH_NUMERIC, W_NUMERIC)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_TOTALIZER : SH_Block
{
  W_TOTALIZER *W;
  SH_CREATE0(SH_TOTALIZER, W_TOTALIZER)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_PID : SH_Block
{
  W_PID *W;
  BYTE *pINITMAN_Master;
  BYTE *pINITREQ1_Master;
  BYTE *pINITREQ2_Master;
  BYTE *pINITREQ3_Master;
  BYTE *pINITREQ4_Master;
  /*BYTE *pINITREQ5_Master;
  BYTE *pINITREQ6_Master;
  BYTE *pINITREQ7_Master;
  BYTE *pINITREQ8_Master;*/
  double *pOP_Master;
  SH_CREATE0(SH_PID, W_PID)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void GetParams( KHBridge2SysParam &params );
  virtual void StepAfterRestoreState();
  NEWH
};

#include <transtypes.h>
#include <CalcRes.h>
struct SAuxCalcCommon : SH_Block
{
  SAuxCalcCommon();
protected:
  struct SExprCode
  {
    ns_translator::SOPElement *pOPs;
    KCalcVar *pVars;
    SLocalVar *pLocalVars;
    int cOPs, cVars, cLocalVars;
  };

  bool bErrorInitialization;

  bool Calc( SExprCode &expr, double &r );
  void Operation( ns_translator::eYToken_value cmd );
  void Function( ns_translator::eYFunctions nF );
  void CreateCodes( KBmBase *pRoot, KBmBase *pModule, SExprCode *pCodes, LPSTR pExprs[], int N );
  void SaveBlock( std::fstream& hFile, SExprCode *pCodes, LPSTR pExprs[], int N );
  bool RestoreBlock( std::fstream& hFile, SExprCode *pCodes, LPSTR pExprs[], int N );
};

struct SH_AUXCALC : SAuxCalcCommon
{
  W_AUXCALC *W;

  enum
  {
    enExpr = _countof( ((W_AUXCALC*)0)->EXPR),
  };

  SExprCode m_Code[enExpr];

  SH_CREATE0(SH_AUXCALC, W_AUXCALC)
  static void InitParm();
  virtual void SetParmList();
  virtual void OnReadProject( KBmBase *pRoot, KBmBase *pModule );
  virtual void SaveBlock(std::fstream& hFile );
  virtual bool RestoreBlock(std::fstream& hFile );
  virtual void StepT( SStepCalcParams &dt );
  virtual void GetParams( KHBridge2SysParam &params );
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_REGCALC : SAuxCalcCommon
{
  BYTE *pINITMAN_Master;
  double *pOP_Master;

  W_REGCALC *W;
  BYTE *pINITREQ1_Master;
  BYTE *pINITREQ2_Master;
  BYTE *pINITREQ3_Master;
  BYTE *pINITREQ4_Master;  

  enum
  {
    enExpr = _countof( ((W_REGCALC*)0)->EXPR),
  };

  SExprCode m_Code[enExpr];

  SH_CREATE0(SH_REGCALC, W_REGCALC)
  static void InitParm();
  virtual void SetParmList();
  virtual void OnReadProject( KBmBase *pRoot, KBmBase *pModule );
  virtual void SaveBlock(std::fstream& hFile );
  virtual bool RestoreBlock(std::fstream& hFile );
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_ENHAUXCALC : SAuxCalcCommon
{
  W_ENHAUXCALC *W;
  enum
  {
    enExpr = _countof( ((W_ENHAUXCALC*)0)->EXPR),
  };

  SExprCode m_Code[enExpr];

  SH_CREATE0(SH_ENHAUXCALC, W_ENHAUXCALC)
  static void InitParm();
  virtual void SetParmList();
  virtual void OnReadProject( KBmBase *pRoot, KBmBase *pModule );
  virtual void SaveBlock( std::fstream& hFile );
  virtual bool RestoreBlock( std::fstream& hFile );
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  NEWH
};

struct SH_ENHREGCALC : SAuxCalcCommon
{
  W_ENHREGCALC *W;
  enum
  {
    enExpr = _countof( ((W_ENHREGCALC*)0)->EXPR),
  };

  SExprCode m_Code[enExpr];

  SH_CREATE0(SH_ENHREGCALC, W_ENHREGCALC)
  static void InitParm();
  virtual void SetParmList();
  virtual void OnReadProject( KBmBase *pRoot, KBmBase *pModule );
  virtual void SaveBlock( std::fstream& hFile );
  virtual bool RestoreBlock( std::fstream& hFile );
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_TYPECONVERT : SH_Block
{
  enum eConIn
  {
    ecioNo, ecioBool, ecioInt32, ecioDouble64, ecioEnum, ecioSdEnum, ecioDouble32, ecioUInt32
  } m_ConnectedIn, m_ConnectedOut[2];
  W_TYPECONVERT *W;
  SH_CREATE0(SH_TYPECONVERT, W_TYPECONVERT)
  SH_TYPECONVERT();
  static void InitParm();
  virtual void SetParmList();
  virtual bool GetVar( LPCSTR pField, BYTE **ppVar, eVarType *pType, USHORT *pVarSize = NULL, LPCSTR *ppszEnum = NULL, BYTE *pFlags = NULL );
  virtual void StepT( SStepCalcParams &dt );
  NEWH
protected:
  void SetOutType( LPCSTR pField, eConIn &ConnectedOut );
  void TransOut( eConIn ConnectedOut );
};

struct SH_PCDINUMARRCH : SH_Block
{
  W_PCDINUMARRCH *W;
  SH_CREATE_IO0(SH_PCDINUMARRCH, W_PCDINUMARRCH)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_PCDIFLAGARRCH : SH_Block
{
  W_PCDIFLAGARRCH *W;
  //CValue *pRegisters[64+1];
  SH_CREATE0(SH_PCDIFLAGARRCH, W_PCDIFLAGARRCH)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_ADD : SH_Block
{
  W_ADD *W;
  SH_CREATE0(SH_ADD, W_ADD)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_MUL : SH_Block
{
  W_MUL *W;
  SH_CREATE0(SH_MUL, W_MUL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_DIV : SH_Block
{
  W_DIV *W;
  SH_CREATE0(SH_DIV, W_DIV)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_OR : SH_Block
{
  W_OR *W;
  SH_CREATE0(SH_OR, W_OR)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_NOT : SH_Block
{
  W_NOT *W;
  SH_CREATE0(SH_NOT, W_NOT)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_AND : SH_Block
{
  W_AND *W;
  SH_CREATE0(SH_AND, W_AND)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_NOR : SH_Block
{
  W_NOR *W;
  SH_CREATE0(SH_NOR, W_NOR)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_PULSE : SH_Block
{
  W_PULSE *W;
  SH_CREATE0(SH_PULSE, W_PULSE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_EQ : SH_Block
{
  W_EQ *W;
  SH_CREATE0(SH_EQ, W_EQ)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SELREAL : SH_Block
{
  W_SELREAL *W;
  SH_CREATE0(SH_SELREAL, W_SELREAL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_TEXTCOMMENT : SH_Block
{
  W_TEXTCOMMENT *W;
  SH_CREATE0(SH_TEXTCOMMENT, W_TEXTCOMMENT)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_FANOUT : SH_Block
{
  W_FANOUT *W;
  BYTE *pINITMAN_Master;
  double *pOP_Master;
  SH_CREATE0(SH_FANOUT, W_FANOUT)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  virtual void GetParams( KHBridge2SysParam &params );
  NEWH
};

struct SH_AUTOMAN : SH_Block
{
  W_AUTOMAN *W;
  BYTE *pINITMAN_Master;
  BYTE *pINITREQ1_Master;
  BYTE *pINITREQ2_Master;
  BYTE *pINITREQ3_Master;
  BYTE *pINITREQ4_Master;
  double *pOP_Master;
  SH_CREATE0(SH_AUTOMAN, W_AUTOMAN)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  virtual void StepAfterRestoreState();
  NEWH
};


struct SH_RBPERMISSION : SH_Block
{
  W_RBPERMISSION *W;
  SH_CREATE0(SH_RBPERMISSION, W_RBPERMISSION)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_DIGACQ : SH_Block
{
  W_DIGACQ *W;
  SH_CREATE0(SH_DIGACQ, W_DIGACQ)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepAfterRestoreState();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_848_0601 : SH_Block
{
  W_848_0601 *W;
  SH_CREATE0(SH_848_0601, W_848_0601)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_FLOWCOMP: SH_Block
{
  W_FLOWCOMP *W;
  SH_CREATE0(SH_FLOWCOMP, W_FLOWCOMP)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_OFFDELAY: SH_Block
{
  W_OFFDELAY *W;
  SH_CREATE0(SH_OFFDELAY, W_OFFDELAY)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_ROLLAVG : SH_Block
{
  struct SInternal
  {
    double *pBuf;
    int counts;
    double waitS, cwS;
  }m_Data;
  W_ROLLAVG *W;
  SH_CREATE0(SH_ROLLAVG, W_ROLLAVG)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_ONDELAY : SH_Block
{
  W_ONDELAY *W;
  SH_CREATE0(SH_ONDELAY, W_ONDELAY)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_DELAY : SH_Block
{
  W_DELAY *W;
  SH_CREATE0(SH_DELAY, W_DELAY)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_MAI: SH_Block
{
  W_MAI *W;
  SH_CREATE0(SH_MAI, W_MAI)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_MAXPULSE: SH_Block
{
  W_MAXPULSE *W;
  SH_CREATE0(SH_MAXPULSE, W_MAXPULSE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_LT: SH_Block
{
  W_LT *W;
  SH_CREATE0(SH_LT, W_LT)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SIGNALSEL: SH_Block
{
  W_SIGNALSEL *W;
  SH_CREATE0(SH_SIGNALSEL, W_SIGNALSEL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_9413_DIGIT_0102_RESOURCE: SH_Block
{
  W_9413_DIGIT_0102_RESOURCE *W;
  SH_CREATE0(SH_9413_DIGIT_0102_RESOURCE, W_9413_DIGIT_0102_RESOURCE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_9413_DIGIT_0102_DO1TB: SH_Block
{
  W_9413_DIGIT_0102_DO1TB *W;
  SH_CREATE0(SH_9413_DIGIT_0102_DO1TB, W_9413_DIGIT_0102_DO1TB)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_9413_DIGIT_0102_MDO: SH_Block
{
  W_9413_DIGIT_0102_MDO *W;
  SH_CREATE0(SH_9413_DIGIT_0102_MDO, W_9413_DIGIT_0102_MDO)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_9413_DIGIT_0102_AI: SH_Block
{
  W_9413_DIGIT_0102_AI *W;
  SH_CREATE0(SH_9413_DIGIT_0102_AI, W_9413_DIGIT_0102_AI)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_RS : SH_Block
{
  W_RS *W;
  SH_CREATE0(SH_RS, W_RS)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_PUSH : SH_Block
{
  enum eConIn
  {
    ecioNo, ecioBool, ecioInt, ecioDouble, ecioEnum
  } m_ConnectedIn, m_ConnectedOut;
  W_PUSH *W;
  SH_CREATE0(SH_PUSH, W_PUSH)
  SH_PUSH();
  static void InitParm();
  virtual void SetParmList();
  virtual bool GetVar( LPCSTR pField, BYTE **ppVar, eVarType *pType, USHORT *pVarSize = NULL, LPCSTR *ppszEnum = NULL, BYTE *pFlags = NULL );
  virtual void StepT( SStepCalcParams &dt );
  virtual void AddAsOutputConnection( SConnectionMB *pCon );
  NEWH
protected:
  SConnectionMB *m_pOutCon[5];
  DWORD m_nOutCon;
};

struct SH_TIMER : SH_Block
{
  int curSP;
  W_TIMER *W;
  SH_CREATE0(SH_TIMER, W_TIMER)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  virtual void OnAssignField( LPCSTR pszFieldName );
  NEWH
};

struct SH_CHECKBAD : SH_Block
{
  W_CHECKBAD *W;
  SH_CREATE0(SH_CHECKBAD, W_CHECKBAD)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SR : SH_Block
{
  W_SR *W;
  SH_CREATE0(SH_SR, W_SR)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_GE : SH_Block
{
  W_GE *W;
  SH_CREATE0(SH_GE, W_GE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_GT : SH_Block
{
  W_GT *W;
  SH_CREATE0(SH_GT, W_GT)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SWITCH : SH_Block
{
  struct SRegOpInit
  {
    BYTE *pINITMAN_Master;
    double *pOP_Master;
  };
  BYTE *pINITREQ1_Master;
  BYTE *pINITREQ2_Master;
  BYTE *pINITREQ3_Master;
  BYTE *pINITREQ4_Master;
  SRegOpInit Pids[8];
  W_SWITCH *W;
  SH_CREATE0(SH_SWITCH, W_SWITCH)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  NEWH
private:
  void ActualizeConnectionActives();
};

struct SH_OVRDSEL : SH_Block
{
  struct SRegOpInit
  {
    BYTE *pINITMAN_Master;
    double *pOP_Master;
  };
  BYTE *pINITREQ1_Master;
  BYTE *pINITREQ2_Master;
  BYTE *pINITREQ3_Master;
  BYTE *pINITREQ4_Master;
  SRegOpInit Pids[4];	
  W_OVRDSEL *W;
  SH_CREATE0(SH_OVRDSEL, W_OVRDSEL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  NEWH
private:
  void ActualizeConnectionActives();  
};

struct SH_RATIOCTL : SH_Block
{
  W_RATIOCTL *W;
  SH_CREATE0(SH_RATIOCTL, W_RATIOCTL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_LE: SH_Block
{
  W_LE *W;
  SH_CREATE0(SH_LE, W_LE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_NE: SH_Block
{
  W_NE *W;
  SH_CREATE0(SH_NE, W_NE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_RTRIG : SH_Block
{
  W_RTRIG *W;
  SH_CREATE0(SH_RTRIG, W_RTRIG)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_FTRIG: SH_Block
{
  W_FTRIG *W;
  SH_CREATE0(SH_FTRIG, W_FTRIG)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_MESSAGE : SH_Block
{
  W_MESSAGE *W;
  SH_CREATE0(SH_MESSAGE, W_MESSAGE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_TEXTARRAY : SH_Block
{
  W_TEXTARRAY *W;
  SH_CREATE0(SH_TEXTARRAY, W_TEXTARRAY)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_PITOTALIZER : SH_Block
{
  W_PITOTALIZER *W;
  SH_CREATE0(SH_PITOTALIZER, W_PITOTALIZER)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_PICHANNEL : SH_Block
{
  W_PICHANNEL *W;
  SH_CREATE0(SH_PICHANNEL, W_PICHANNEL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_PIDFF : SH_Block
{
  W_PIDFF *W;
  BYTE *pINITMAN_Master;
  double *pOP_Master;
  SH_CREATE0(SH_PIDFF, W_PIDFF)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void GetParams( KHBridge2SysParam &params );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_NUMERICARRAY : SH_Block
{
  W_NUMERICARRAY *W;
  SH_CREATE0(SH_NUMERICARRAY, W_NUMERICARRAY)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SUB : SH_Block
{
  W_SUB *W;
  SH_CREATE0(SH_SUB, W_SUB)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_REGSUMMER : SH_Block
{
  W_REGSUMMER *W;
  SH_CREATE0(SH_REGSUMMER, W_REGSUMMER)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_9413_DIGIT_0102_MDI: SH_Block
{
  W_9413_DIGIT_0102_MDI *W;
  SH_CREATE0(SH_9413_DIGIT_0102_MDI, W_9413_DIGIT_0102_MDI)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_DEADTIME : SH_Block
{
  W_DEADTIME *W;
  SH_CREATE0(SH_DEADTIME, W_DEADTIME)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_AIREF : SH_Block
{
  double *pIO;
  double *pHDY;
  W_AIREF *W;
  SH_AIREF();
  SH_CREATE0(SH_AIREF, W_AIREF)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual LPCSTR SetDestIO( KBmBase *dest, LPCSTR pszField );
  NEWH
};

struct SH_AOREF : SH_Block
{
  double *pIO;
  W_AOREF *W;
  SH_AOREF();
  SH_CREATE0(SH_AOREF, W_AOREF)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual LPCSTR SetDestIO( KBmBase *dest, LPCSTR pszField );
  NEWH
};

struct SH_DIREF : SH_Block
{
  BYTE *pIO;
  W_DIREF *W;
  SH_DIREF();
  SH_CREATE0(SH_DIREF, W_DIREF)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual LPCSTR SetDestIO( KBmBase *dest, LPCSTR pszField );
  NEWH
};

struct SH_DOREF : SH_Block
{
  double timerForPulse;
  BYTE *pIO;
  W_DOREF *W;
  SH_DOREF();
  SH_CREATE0(SH_DOREF, W_DOREF)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual LPCSTR SetDestIO( KBmBase *dest, LPCSTR pszField );
  NEWH
};

struct SH_NUM2BOOL : SH_Block
{
  W_NUM2BOOL *W;
  SH_CREATE0(SH_NUM2BOOL, W_NUM2BOOL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_BOOL2NUM : SH_Block
{
  W_BOOL2NUM *W;
  SH_CREATE0(SH_BOOL2NUM, W_BOOL2NUM)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_INTERLOCK : SH_Block
{
  W_INTERLOCK *W;
  SH_CREATE0(SH_INTERLOCK, W_INTERLOCK)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_2OO3 : SH_Block
{
  W_2OO3 *W;
  SH_CREATE0(SH_2OO3, W_2OO3)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_QOR : SH_Block
{
  W_QOR *W;
  SH_CREATE0(SH_QOR, W_QOR)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_FIRSTOUT : SH_Block
{
  W_FIRSTOUT *W;
  SH_CREATE0(SH_FIRSTOUT, W_FIRSTOUT)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_ABS : SH_Block
{
  W_ABS *W;
  SH_CREATE0(SH_ABS, W_ABS)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_MUXREAL : SH_Block
{
  W_MUXREAL *W;
  SH_CREATE0(SH_MUXREAL, W_MUXREAL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_MAX : SH_Block
{
  W_MAX *W;
  SH_CREATE0(SH_MAX, W_MAX)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_CTUD : SH_Block
{
  W_CTUD *W;
  SH_CREATE0(SH_CTUD, W_CTUD)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_GENLIN : SH_Block
{
  W_GENLIN *W;
  SH_CREATE0(SH_GENLIN, W_GENLIN)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_TRANSITION : SAuxCalcCommon
{
  W_TRANSITION *W;
  bool bErrorInitialization;

  enum
  {
    enExpr = _countof( ((W_TRANSITION*)0)->C),
  };

  SExprCode m_Code[enExpr];

  SH_TRANSITION();
  SH_CREATE0(SH_TRANSITION, W_TRANSITION)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void OnReadProject( KBmBase *pRoot, KBmBase *pModule );
  virtual void SaveBlock( std::fstream& hFile );
  virtual bool RestoreBlock( std::fstream& hFile );
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  virtual void StepAfterRestoreState();
  virtual void SelectedInSCM();
  NEWH
};

struct SH_STEP : SAuxCalcCommon
{
  W_STEP *W;
  bool bErrorInitialization;

  enum
  {
    enExpr = _countof( ((W_STEP*)0)->OP),
  };

  SExprCode m_CodeOP[enExpr];
  SExprCode m_stopCon;

  SH_CREATE0(SH_STEP, W_STEP)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void OnReadProject( KBmBase *pRoot, KBmBase *pModule );
  virtual void SaveBlock( std::fstream& hFile );
  virtual bool RestoreBlock( std::fstream& hFile );
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  virtual void StepAfterRestoreState();
  virtual void SelectedInSCM();
  NEWH
};

struct SH_AUXSUMMER : SH_Block
{
  bool bPconnected[11];
  W_AUXSUMMER *W;
  SH_CREATE0(SH_AUXSUMMER, W_AUXSUMMER)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_LIMIT : SH_Block
{
  W_LIMIT *W;
  SH_CREATE0(SH_LIMIT, W_LIMIT)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_MINPULSE : SH_Block
{
  W_MINPULSE *W;
  SH_CREATE0(SH_MINPULSE, W_MINPULSE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_HAICHANNEL : SH_Block
{
  double *pSlave;
  W_HAICHANNEL *W;
  SH_HAICHANNEL();
  SH_CREATE_IO0(SH_HAICHANNEL, W_HAICHANNEL)
  static void InitParm();
  virtual void SetParmList();
  virtual LPCSTR SetDestIO( KBmBase *dest, LPCSTR pszField );
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_XOR : SH_Block
{
  W_XOR *W;
  SH_CREATE0(SH_XOR, W_XOR)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_WATCHDOG : SH_Block
{
  W_WATCHDOG *W;
  SH_CREATE0(SH_WATCHDOG, W_WATCHDOG)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SEL: SH_Block
{
  W_SEL *W;
  SH_CREATE0(SH_SEL, W_SEL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_ROR : SH_Block
{
  W_ROR *W;
  SH_CREATE0(SH_ROR, W_ROR)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_STARTSIGNAL : SH_Block
{
  W_STARTSIGNAL *W;
  SH_CREATE0(SH_STARTSIGNAL, W_STARTSIGNAL)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SP_SPDVOTE : SH_Block
{
  W_SP_SPDVOTE *W;
  SH_CREATE_IO_SP(SH_SP_SPDVOTE, W_SP_SPDVOTE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SP_AI : SH_Block
{
  W_SP_AI *W;
  SH_CREATE_IO_SP(SH_SP_AI, W_SP_AI)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SP_AO : SH_Block
{
  W_SP_AO *W;
  SH_CREATE_IO_SP(SH_SP_AO, W_SP_AO)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SP_DI : SH_Block
{
  W_SP_DI *W;
  SH_CREATE_IO_SP(SH_SP_DI, W_SP_DI)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_SP_DO : SH_Block
{
  W_SP_DO *W;
  SH_CREATE_IO_SP(SH_SP_DO, W_SP_DO)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};


struct SH_SP_SPEED : SH_Block
{
  W_SP_SPEED *W;
  SH_CREATE_IO_SP(SH_SP_SPEED, W_SP_SPEED)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_NAND : SH_Block
{
  W_NAND *W;
  SH_CREATE0(SH_NAND, W_NAND)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};


struct SH_FLAGARRAY : SH_Block
{
  W_FLAGARRAY *W;
  SH_CREATE0(SH_FLAGARRAY, W_FLAGARRAY)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_TRIG : SH_Block
{
  W_TRIG *W;
  SH_CREATE0(SH_TRIG, W_TRIG)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_MIN : SH_Block
{
  W_MIN *W;
  SH_CREATE0(SH_MIN, W_MIN)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct SH_RATIOBIAS : SH_Block
{
  W_RATIOBIAS *W;
  SH_CREATE0(SH_RATIOBIAS, W_RATIOBIAS)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  NEWH
};

struct SH_NEG : SH_Block
{
  W_NEG *W;
  SH_CREATE0(SH_NEG, W_NEG)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

//////////////////////////////////////////////////////////////////////////
class SBlockCreate
{
  typedef SH_Block * (*tfHCreateNP)( LPCSTR dcuName, LPCSTR hmiName );
  //typedef SH_Block * (*tfHCreateIP)( LPCSTR dcuName, LPCSTR hmiName, int iParam );
  typedef SH_Block * (*tfHCreateRP)( LPCSTR dcuName, LPCSTR hmiName, LPCSTR pszRef2RealBlock );
  static SBlockCreate * pFirst;
  static SBlockCreate * pLast;

  SBlockCreate * pNext;
  LPCSTR pType;
  int iParam;
  tfHCreateNP fnCreateNP;
  //tfHCreateIP fnCreateIP;
  tfHCreateRP fnCreateRP;

  void Link( LPCSTR pType );
public:
  SBlockCreate( LPCSTR pType, tfHCreateNP pCreate );
  //SBlockCreate( LPCSTR pType, tfHCreateIP pCreate, int iParam );
  SBlockCreate( LPCSTR pType, tfHCreateRP pCreate );

  static SH_Block * Create( LPCSTR dcuName, LPCSTR hmiName, LPCSTR pszRef2RealBlock, LPCSTR pType );
  static bool IsSupportedType( LPCSTR pType );
};
