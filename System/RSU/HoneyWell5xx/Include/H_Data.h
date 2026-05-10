#pragma once
#include <err.h>
#include <H_Class.h>

struct SH_Module : SConnectionsCont, KBmBase
{
  SH_Module **ppSubModules;
  SH_Block **ppBlocks;

  size_t nSubModules, nBlocks;

  char szFile[32 * 4];


  SH_Module();
  virtual KBmBase* FindObj( LPCSTR pObj );
  virtual KBmBase* FindObj( typeHASH32_ *pCrcs, int nCrcs, int nAlgFind );
  virtual void OnReadProject( KBmBase *pRoot, KBmBase *pModule );
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  virtual void StepAfterRestoreState();
  virtual void StepT( SStepCalcParams &dt );
  virtual void GetParams( KHBridge2SysParam &params );
  virtual BOOL IsBlock();
  virtual KBmBase* WhoHasConnection( LPCSTR pszObj, LPCSTR pszFld, bool bOutput, LPCSTR *ppFld );
};

#define SHM_CREATE0(Class,Struct) \
  static SH_Module * Create( LPCSTR dcuName, LPCSTR hmiName )\
{\
  return CreateImpl<SH_Module,Class,Struct>( dcuName, hmiName, KMemoryServiceH5xx::Instance(), &KMemoryServiceH5xx::CreateObjectsStruct );\
}

struct S_xC300 : SH_Module
{
  W_xC300 *W;
  SHM_CREATE0( S_xC300, W_xC300)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct S_PCDI_MASTER : SH_Module
{
  W_PCDI_MASTER *W;
  SHM_CREATE0( S_PCDI_MASTER, W_PCDI_MASTER )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_CEEC300 : SH_Module
{
  W_CEEC300 *W;
  SHM_CREATE0( S_CEEC300, W_CEEC300 )
  static void InitParm();
  virtual void SetParmList();
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct S_CONTROLMODULE : SH_Module
{
  double periodSec;
  int maxPhaseIndex;
  W_CONTROLMODULE *W;
  SHM_CREATE0( S_CONTROLMODULE, W_CONTROLMODULE)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepAfterRestoreState();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct S_IOLINK : SH_Module
{
  W_IOLINK *W;
  SHM_CREATE0( S_IOLINK, W_IOLINK)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_xHART_HL : SH_Module
{
  W_xHART_HL *W;
  SHM_CREATE0( S_xHART_HL, W_xHART_HL)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_Dx_24y : SH_Module
{
  W_Dx_24y *W;
  SHM_CREATE0( S_Dx_24y, W_Dx_24y)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_ROOT : SH_Module
{
  W_ROOT *W;
  SHM_CREATE0( S_ROOT, W_ROOT)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_FIM4 : SH_Module
{
  W_FIM4 *W;
  SHM_CREATE0( S_FIM4, W_FIM4)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_FFLINK : SH_Module
{
  W_FFLINK *W;
  SHM_CREATE0( S_FFLINK, W_FFLINK)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_9413_DIGIT_0102 : SH_Module
{
  W_9413_DIGIT_0102 *W;
  SHM_CREATE0( S_9413_DIGIT_0102, W_9413_DIGIT_0102)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_AI_LLMUX : SH_Module
{
  W_AI_LLMUX *W;
  SHM_CREATE0( S_AI_LLMUX, W_AI_LLMUX)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_Dx_SOE : SH_Module
{
  W_Dx_SOE *W;
  SHM_CREATE0( S_Dx_SOE, W_Dx_SOE)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_TC_IAH161 : SH_Module
{
  W_TC_IAH161 *W;
  SHM_CREATE0( S_TC_IAH161, W_TC_IAH161 )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_CPM200 : SH_Module
{
  W_CPM200 *W;
  SHM_CREATE0( S_CPM200, W_CPM200 )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_CEEFB : SH_Module
{
  W_CEEFB *W;
  SHM_CREATE0( S_CEEFB, W_CEEFB )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_TC_IDD321 : SH_Module
{
  W_TC_IDD321 *W;
  SHM_CREATE0( S_TC_IDD321, W_TC_IDD321 )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_TC_ODD321 : SH_Module
{
  W_TC_ODD321 *W;
  SHM_CREATE0( S_TC_ODD321, W_TC_ODD321 )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_FTEB : SH_Module
{
  W_FTEB *W;
  SHM_CREATE0( S_FTEB, W_FTEB )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_RMFBCLASS : SH_Module
{
  W_RMFBCLASS *W;
  SHM_CREATE0( S_RMFBCLASS, W_RMFBCLASS )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_TC_IXL061 : SH_Module
{
  W_TC_IXL061 *W;
  SHM_CREATE0( S_TC_IXL061, W_TC_IXL061 )
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct S_TC_MDP081 : SH_Module
{
  W_TC_MDP081 *W;
  SHM_CREATE0( S_TC_MDP081, W_TC_MDP081 )
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct S_TC_IXL062 : SH_Module
{
  W_TC_IXL062 *W;
  SHM_CREATE0( S_TC_IXL062, W_TC_IXL062 )
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct S_TC_OAV081 : SH_Module
{
  W_TC_OAV081 *W;
  SHM_CREATE0( S_TC_OAV081, W_TC_OAV081 )
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct S_TC_IXR061 : SH_Module
{
  W_TC_IXR061 *W;
  SHM_CREATE0( S_TC_IXR061, W_TC_IXR061 )
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct S_HANDLER : SH_Module
{
  enum
  {
    nStep2TransConnects = 10,
  };
  struct SSeqRec
  {
    enum eType
    {
      eHZ = 0,
      eConnection = 'C',
      eStep = 'S',
      eTrans = 'T',
    };
    eType type;
    SH_Block *pBlock;
    int number;
    int nextRecs[nStep2TransConnects];
    char szName[sizeofBlockName];
    SSeqRec()
    {
      memset( nextRecs, -1, sizeof(nextRecs) );
    }
    NEWH
  }*pSeqs;
  int nSeqCount;

  char szActiveNames[nStep2TransConnects][sizeofBlockName];

  W_HANDLER *W;
  SHM_CREATE0(S_HANDLER, W_HANDLER)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual void StepAfterRestoreState();
  NEWH
  void SelectWorkingSeqs( int seqNum );
  void ActualizeNames();
  void ShiftNextFrom0();
};

struct S_SCM : SH_Module
{
  double periodSec;
  int maxPhaseIndex;
  S_HANDLER *pHandlers[_countof(((W_SCM*)0)->ACTIVEHANDLR) ];
  LPSTR pszRawOrder;
  W_SCM *W;
  SHM_CREATE0( S_SCM, W_SCM)
  static void InitParm();
  virtual void OnReadProject( KBmBase *pRoot, KBmBase *pModule );
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  virtual bool SetValue( LPCSTR pField, LPCSTR pszVal );
  virtual bool RestoreBlock( std::fstream& hFile );
  virtual void SaveBlock( std::fstream& hFile );
  virtual void StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule );
  virtual void StepAfterRestoreState();
  NEWH
};

struct S_AO16 : SH_Module
{
  W_AO16 *W;
  SHM_CREATE0( S_AO16, W_AO16 )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_DO32 : SH_Module
{
  W_DO32 *W;
  SHM_CREATE0( S_DO32, W_DO32 )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_DI : SH_Module
{
  W_DI *W;
  SHM_CREATE0( S_DI, W_DI )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_HLAIHART: SH_Module
{
  W_HLAIHART *W;
  SHM_CREATE0(S_HLAIHART, W_HLAIHART)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_HLAI: SH_Module
{
  W_HLAI *W;
  SHM_CREATE0(S_HLAI, W_HLAI)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_DISOE : SH_Module
{
  W_DISOE *W;
  SHM_CREATE0(S_DISOE, W_DISOE)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_DI24V : SH_Module
{
  W_DI24V *W;
  SHM_CREATE0( S_DI24V, W_DI24V )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_LLMUX : SH_Module
{
  W_LLMUX *W;
  SHM_CREATE0( S_LLMUX, W_LLMUX )
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_UIO_2 : SH_Module
{
  W_UIO_2 *W;
  SHM_CREATE0( S_UIO_2, W_UIO_2)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};

struct S_SP : SH_Module
{
  W_SP *W;
  SHM_CREATE0( S_SP, W_SP)
  static void InitParm();
  virtual void SetParmList();
  virtual void StepT( SStepCalcParams &dt );
  NEWH
};

struct S_UIO : SH_Module
{
  W_UIO *W;
  SHM_CREATE0( S_UIO, W_UIO)
  static void InitParm();
  virtual void SetParmList();
  NEWH
};
//////////////////////////////////////////////////////////////////////////
class SModuleCreate
{
  typedef SH_Module * (*tfHCreateNP)( LPCSTR dcuName, LPCSTR hmiName );
  static SModuleCreate * pFirst;
  static SModuleCreate * pLast;

  SModuleCreate * pNext;
  LPCSTR pType;
  tfHCreateNP fnCreateNP;
public:
  SModuleCreate( LPCSTR pType, tfHCreateNP pCreate );

  static SH_Module * Create( LPCSTR dcuName, LPCSTR hmiName, LPCSTR pType );
  static bool IsSupportedType( LPCSTR pType );
};
