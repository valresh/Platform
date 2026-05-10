#pragma once
#include <macros/NonCopyable.h>
#include <CommonY.h>

class KStateSer 
  : public IYokoStateSer
  , protected n_RSUs::NonCopyable
{
  HANDLE m_hFile;
  DWORD m_dwInitialFileSize;
  DWORD m_dwFileSize;
  DWORD m_dwCurr;
  DWORD m_dwLastError;
  bool m_bRead;
  DWORD m_BytesCanRead;
public:
  KStateSer( LPCSTR pszFile, bool bSave );
  KStateSer( HANDLE hFile, bool bSave );
  virtual ~KStateSer();
  bool Close();
  IYokoStateSer* GetInterface();
protected:
  bool Create( LPCSTR pszFile );
  bool Open( LPCSTR pszFile );
  void OnWrote( DWORD size );
  void OnRead( DWORD size );
protected:
  //IYokoStateSer
  virtual DWORD Shift( LONG lDistanceToMove );
  virtual bool IsData();
  virtual DWORD BytesCanRead();
  virtual void SetReadyBytes( DWORD count );
  virtual DWORD SimpleRead( LPVOID Addr, DWORD dwSize );
  virtual DWORD ReadLenStr( LPSTR str, DWORD dwSize );
  virtual DWORD SimpleWrite( LPCVOID Addr, DWORD dwSize );
  virtual DWORD WriteStr( LPCSTR str );
  virtual DWORD GetPosition();
  virtual DWORD WriteAtPosition( LONG shiftFromBegin, LPCVOID Addr, DWORD objSize );
};
