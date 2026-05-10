#pragma once
#include <BaseType_.h>
#include "./FlexRsuCore.h"
#include <macros/FixString.h>

#pragma warning( push )
#pragma warning( disable: 4251 )

class FLEXRSUCORE_API KFlexConnection
{
  eVarType m_ConOutVarType, m_ConInVarType;
  union UPnt
  {
    tDigital32 *pD;
    tAnalog32 *pA;
  }m_Dst, m_Src;
  bool m_bAssigned;
public:
  KFlexConnection();
  void *operator new( size_t size );
  void operator >> ( tDigital32 &inVar );//вход в блок
  void operator >> ( tAnalog32 &inVar );//вход в блок
  void operator << ( tDigital32 &outVar );//выход из блок
  void operator << ( tAnalog32 &outVar );//выход из блок
  void operator = ( tDigital32 &outVar );
  void operator = ( tAnalog32 &outVar );
  void DataReady2Send();
  bool GetData();
  eVarType GetSourceType();
  eVarType GetSinkType();
  void* GetSrcRawMem();
  void* GetDstRawMem();
public:
  GUID m_SourceID;
  GUID m_SinkID;
  SFixString<16> m_szSource;
  SFixString<16> m_szSink;
};

#pragma warning( pop )
