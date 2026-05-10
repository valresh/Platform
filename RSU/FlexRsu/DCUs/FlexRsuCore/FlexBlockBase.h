#pragma once
#include "FlexConnection.h"
#include <FlexVarInfo.h>

FLEXRSUCORE_API int CompVarInfo( const void * pP1, const void * pP2 );

struct SPageConnector
{
  tAnalog32 flt32Val;
  tDigital32 dig32Val;
  eVarType typeSrcData;
  tChars flagSrcValid;
};

class FLEXRSUCORE_API KFlexBlockBase
{
public:
  struct IProjectContainer
  {
    virtual BYTE* GetTagField( LPSTR pszPntName, eVarType *pType = NULL, tVarSizeType_ *pVarSize = NULL ) = 0;
    virtual LPCSTR GetProjectDir() = 0;
    virtual KFlexBlockBase* FindBlock( LPCSTR pszName ) = 0;
    virtual SPageConnector* RegisterPageConnector( LPCSTR pszName ) = 0;
  };
public:
  enum eExecOnNullOrder
  {
    exnNever              = 0x00000000,
    exnInputsIO           = 0x00000001,
    exnOutputsIO          = 0x00000002,
    exnShowOnly           = 0x00000004,
  };
  struct SCallParams
  {
    SCallParams() : dtS(0), nStep(0), ioMask(0), modelTime(0), pszPrjObjName(NULL), pszAuditDir(NULL), pszDrName(NULL), bSkipSimIO(false)
    {}
    tAnalog32 dtS;
    DWORD nStep;
    DWORD ioMask;
    __time64_t modelTime;
    LPCSTR pszPrjObjName;
    LPCSTR pszAuditDir;
    LPCSTR pszDrName;
    bool bSkipSimIO;
  };
  struct SReturn
  {
    SReturn() : r( 0 )
    {}
    int r;
  };
protected:
  SVarInfo * m_pPrivateVarsInfo;
  DWORD m_kPrivateVarsInfo;
  BYTE *m_pPrivateObject;

  SVarInfo * m_pSharedVarsInfo;
  DWORD m_kSharedVarsInfo;
  BYTE *m_pSharedObject;
protected:
  KFlexBlockBase();
public:
  void *operator new( size_t size );
  virtual void OnCreated();
  virtual void AssignValue( LPCSTR pszName, LPCSTR pszValue );
  virtual void OnAttributesSetted();
  virtual void OnProjectReady( IProjectContainer *pCont );
  virtual BYTE* GetField( LPCSTR pszName, eVarType *pType = NULL, tVarSizeType_ *pVarSize = NULL );
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  virtual BYTE* EnumVars( DWORD &pos, SVarInfo &var );
  virtual void OnAfterRestoreState();
private:
  BYTE* FillVarInfoBy( SVarInfo* pI, BYTE *pObj, eVarType *pType = NULL, tVarSizeType_ *pVarSize = NULL );
public:
  GUID m_id;
  KDynString m_szVisibleName;
  LPCSTR m_pszType;
  int m_execOrder;
  eExecOnNullOrder m_nullOrderExec;
};


template<typename T>
void SetInitialVals( T *pDst, T *pSrc )
{
  memcpy( pDst, pSrc, sizeof(T) );
};

FLEXRSUCORE_API double _atof( LPCSTR _String );

FLEXRSUCORE_API KFlexConnection** CloneConnections( DWORD &nCon, KFlexConnection **ppSrc, DWORD nSrc );
