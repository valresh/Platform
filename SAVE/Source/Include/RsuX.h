#pragma once

#include <crossplatform.h>
//#include "targetver.h"
#include "RsuX_types.h"

class RSUX_API KRsuX
{
protected:
	nRSUx::SParamInfo m_PointInfo;

	template<class T>
	T GetValue();
  template<class T, class M>
  T GetValueTyped();

	template<class T>
	void SetValue( T thr );
  template<class T, class M>
  void SetValueTyped( T thr );
public:
	KRsuX();
	~KRsuX();
	int LinkTo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pszFile );
  int LinkTo( LPCSTR pszPointName, DWORD TypeID );
  void ManualInit( nRSUx::SParamInfo &info );
  bool IsConnected();
  bool IsPriorOnCBase();

	operator int();
	operator double();
  operator bool();
  operator float();
  operator BYTE();
	KRsuX& operator = ( int thr );
	KRsuX& operator = ( double thr );

  KRsuX& operator << ( KRsuX &thr );
  KRsuX& operator *= ( double mul );
};

typedef nRSUx::SParamInfo (*tRsuConnectParamInfo)( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pszSuffixName );
typedef nRSUx::SParamInfo (*tRsuConnectParamInfoTypes)( LPCSTR pszPointName, DWORD TypeID );
typedef void (*tRegisterRsuConnection)( LPCSTR pszFileUpperName, tRsuConnectParamInfo, tRsuConnectParamInfoTypes );
extern RSUX_API tRegisterRsuConnection pRegisterRsuConnection;
