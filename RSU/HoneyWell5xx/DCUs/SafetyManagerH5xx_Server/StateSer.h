#pragma once
#include "IStateSer.h"
#include <macros/NonCopyable.h>

class KStateSer 
  : public IStateSer
  , protected n_RSUs::NonCopyable
{
  FILE* m_hFile;
  DWORD m_dwFileSize;
  bool m_bRead;
  DWORD m_dwCurr;

  bool m_bStartCalled, m_bCompleteCalled;
  DWORD m_dwStartPos;

  DWORD m_saverVerFormat;
  bool m_bConvert;
public:
  KStateSer( LPCSTR pszFile, bool bSave );
  KStateSer( FILE* hFile, bool bSave, bool convert );
  virtual ~KStateSer();
  void StartSave();
  void SaveCompleted();
  int StartRestore();
  virtual bool NeedConvertUTF();
protected:
  bool Close();
  bool Create( LPCSTR pszFile );
  bool Open( LPCSTR pszFile );
  void OnWrote( DWORD size );
  void OnRead( DWORD size );
protected:
  virtual void SetSavedVersionFormat( DWORD v );
  virtual DWORD GetSavedVersionFormat();
  virtual void Skip( DWORD shift );
  virtual DWORD RawRead( void *pOut, DWORD objSize );
  virtual DWORD GetPosition();
  virtual DWORD WriteAtPositionImpl( DWORD shiftFromBegin, const void *pObj, DWORD objSize );
  virtual DWORD WriteImpl( const void *pObj, DWORD objSize );
  virtual DWORD ReadImpl( void *pObj, DWORD objSize );
  virtual void WriteDotV( SDotValue *pV, LPCSTR pszName );
  virtual int ReadDotV( SDotValue *pV, LPCSTR pszName );
};
