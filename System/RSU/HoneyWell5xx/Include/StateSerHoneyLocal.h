#pragma once
#include <macros/NonCopyable.h>
#include "../../HoneyWell/Include/IStateSer.h"

#ifdef STATESERHONEYLOCAL_EXPORTS
#define STATESERHONEYLOCAL_API _EXP
#else
#define STATESERHONEYLOCAL_API _IMP
#endif

#pragma warning( push )
#pragma warning( disable : 4275)
class STATESERHONEYLOCAL_API KStateSerHoneyLocal 
  : public IStateSer
  , protected n_RSUs::NonCopyable
{
  FILE* m_hFile;
  DWORD m_dwFileSize;
  DWORD m_dwCurr;
  DWORD m_dwLastError;
  bool m_bRead;
  DWORD m_BytesCanRead;
  bool m_bConvert;
public:
  KStateSerHoneyLocal( LPCSTR pszFile, bool bSave );
  KStateSerHoneyLocal( FILE *hFile, bool bSave, bool convert);
  virtual ~KStateSerHoneyLocal();
  bool Close();
  IStateSer* GetInterface();
  virtual bool NeedConvertUTF();
protected:
  virtual void WriteImpl( const void *pObj, DWORD objSize );
  virtual bool ReadImpl( void *pObj, DWORD objSize );
protected:
  bool Create( LPCSTR pszFile );
  bool Open( LPCSTR pszFile );
};
#pragma warning( pop )

