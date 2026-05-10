#pragma once
#include <DataTypes_.h>

#ifdef FLEXRSUCONNECT_EXPORTS
#define FLEXRSUCONNECT_API _EXP
#else
#define FLEXRSUCONNECT_API _IMP
#endif

typedef BYTE* (*tFlexMemory)( DWORD size );
extern FLEXRSUCONNECT_API tFlexMemory pFlexMemoryAllocator;

typedef void (*tFlexMemoryReset)();
extern FLEXRSUCONNECT_API tFlexMemoryReset pFlexMemoryResetter;


typedef SFlex* (*tCreateSFlexSharedObj)( LPCSTR pszName, DWORD FlexID, DWORD size, void ** pStruct );
extern FLEXRSUCONNECT_API tCreateSFlexSharedObj pSFlexSharedObjCreator;

typedef CBase* (*tCreateAnySharedObj)( LPCSTR pszName, DWORD ID, DWORD size, void ** pStruct );
extern FLEXRSUCONNECT_API tCreateAnySharedObj pAnySharedObjCreator;

template<typename S>
SFlex* CreateSFlexSharedObj( LPCSTR pszName, S **ppW )
{
  return pSFlexSharedObjCreator( pszName, S::TypeID, sizeof(S), (void**)ppW );
}

template<typename S>
CBase* CreateAnySharedObj( LPCSTR pszName, S **ppW )
{
  return pAnySharedObjCreator( pszName, S::TypeID, sizeof(S), (void**)ppW );
}

typedef void (*tFlexM2ULogging)( LPCSTR pszPntName, LPCSTR pszText, char bOn, __int64 moment, LPCSTR pszPrjObjName, LPCSTR pszAuditDir );
extern FLEXRSUCONNECT_API tFlexM2ULogging pFlexM2ULogging;

typedef void (*tFlexAlarm)( SFlex *prawBlockInfo, bool bOn, LPCSTR pszAlrmType, __int64 moment, LPCSTR pszDrName, LPCSTR pszComment, LPCSTR pszComment1, LPCSTR pszComment2, LPCSTR pszAtValue, tDigital32 &Acked, LPCSTR pszStandby1, LPCSTR pszStandby2, LPCSTR pszStandby3 );
extern FLEXRSUCONNECT_API tFlexAlarm pFlexAlarm;

typedef void (*tFlexAckingAlarm)( SFlex *prawBlockInfo, __int64 moment, bool bInAlarm );
extern FLEXRSUCONNECT_API tFlexAckingAlarm pFlexAckingAlarm;

typedef void (*tFlexActiveArea)(tDigital32& AreaAlmEn, LPCSTR pszArea, tDigital32& Count);
extern FLEXRSUCONNECT_API tFlexActiveArea pFlexActiveArea;
