#pragma once
#include <IFlexStateSer.h>
#include <fstream>

#pragma warning( push )
#pragma warning( disable : 4275)
class KStateSerLocal : public IFlexStateSer
{
  std::fstream m_hFile;
  DWORD m_dwFileSize;
  DWORD m_dwCurr;
  DWORD m_dwLastError;
  bool m_bRead;
  DWORD m_BytesCanRead;
  bool m_bConvertToUtf8;
public:
  KStateSerLocal( LPCSTR pszFile, bool bSave );
  KStateSerLocal( std::fstream &hFile, bool bSave );
  virtual ~KStateSerLocal();
  bool Close();
  IFlexStateSer* GetInterface();
protected:
  bool Create( LPCSTR pszFile );
  bool Open( LPCSTR pszFile );
  void OnWrote( DWORD size );
  void OnRead( DWORD size );
protected:
  //IFlexStateSer
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
  virtual void SetConvertToUtf8(bool bConvert);
};
#pragma warning( pop )

