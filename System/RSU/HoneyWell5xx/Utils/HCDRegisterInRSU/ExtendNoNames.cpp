#include <HcdDataTypes_.h>
#include <rsuNoNames.h>


int RsuHandleInitData( CBase* pObj );
//int RsuHandleGetParams( CBase* pObj );

void RegisterHCDstructs()
{
  KNoName::RegisterClassInNames( CHcdSystem::TypeID, "HcdSys");
#undef PNT
#define PNT(Class) KNoName::RegisterClassInNames( Class::TypeID, Class::ClassName);
#undef  HCD_TYPE
#define HCD_TYPE( a, b, c ) PNT(W_##b)
#include <HCDType.hpp>

    KNoName::RegisterInitCBaseCall( RsuHandleInitData );
}

int RsuHandleInitData( CBase* pObj )
{
  switch( pObj->ID_CLASS ) 
  {
  case CHcdSystem::TypeID:
    pObj->size = sizeof(CHcdSystem);
    memset( ((BYTE*)pObj) + sizeof(CBase), 0, sizeof(CHcdSystem) - sizeof(CBase) );
    return 0;
#undef PNT
#define PNT(Class) \
  case Class::TypeID: \
  pObj->size = sizeof(Class); \
  memset( ((BYTE*)pObj) + sizeof(CBase), 0, sizeof(Class) - sizeof(CBase) ); \
  return 0;

#undef  HCD_TYPE
#define HCD_TYPE( a, b, c ) PNT(W_##b)
#include <HCDType.hpp>
  };
  return -1;
}