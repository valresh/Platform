#pragma once
#include <crossplatform.h>

struct IFlexStateSer 
{
  virtual DWORD Shift( LONG lDistanceToMove ) = 0;
  virtual bool IsData() = 0;

  virtual DWORD BytesCanRead() = 0;
  virtual void SetReadyBytes( DWORD count ) = 0;

  virtual DWORD SimpleRead( LPVOID Addr, DWORD dwSize ) = 0;
  virtual DWORD ReadLenStr( LPSTR str, DWORD dwSize ) = 0;

  virtual DWORD WriteSized( LPCVOID Addr, DWORD dwSize )
  {
    if( !SimpleWrite( &dwSize, sizeof(dwSize)) )
      return 0;
    return SimpleWrite( Addr, dwSize );
  }
  virtual DWORD SimpleWrite( LPCVOID Addr, DWORD dwSize ) = 0;
  virtual DWORD WriteStr( LPCSTR str ) = 0;

  virtual DWORD GetPosition() = 0;
  virtual DWORD WriteAtPosition( LONG shiftFromBegin, LPCVOID Addr, DWORD objSize ) = 0;

  virtual void SetConvertToUtf8(bool bConvert) = 0;

  template<typename T>
  DWORD SimpleRead( T &obj )
  {
    return SimpleRead( &obj, sizeof(obj) );
  }

  template<typename T>
  DWORD SimpleWrite( T obj )
  {
    return SimpleWrite( &obj, sizeof(obj) );
  }

  template<typename T>
  DWORD WriteAtPosition( DWORD shiftFromBegin, T obj )
  {
    return WriteAtPosition( shiftFromBegin, &obj, sizeof(obj) );
  }
};
