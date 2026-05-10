#include <DataTypes_.h>
#include <rsuNoNames.h>


int RsuHandleInitData( CBase* pObj )
{
  switch( pObj->ID_CLASS )
  {
  case CCBHSystem::TypeID:
    return ((CCBHSystem*)pObj)->Init( );
  case COneMnemoHoney::TypeID:
    return ((COneMnemoHoney*)pObj)->Init( );
  case COneAlmGrHoney::TypeID:
    return ((COneAlmGrHoney*)pObj)->Init( );	
  case id_PntTypeInfo:
    pObj->size = sizeof(SPntTypeInfo);
    memset( ((BYTE*)pObj) + sizeof(CBase), 0, sizeof(SPntTypeInfo) - sizeof(CBase) );
    return 0;

#undef PNT
#define PNT(Class) \
  case Class::TypeID: \
  pObj->size = sizeof(Class); \
  memset( ((BYTE*)pObj) + sizeof(CBase), 0, sizeof(Class) - sizeof(CBase) ); \
  return 0;//((Class*)pObj)->Init( );	

#undef  HONEY_TYPE
#define HONEY_TYPE( a, b, c ) PNT(W_##b)
#include <HoneywellType.hpp>
  };
  return -1;
}

void RegisterHWstructs()
{
#undef PNT
#define PNT(Class) KNoName::RegisterClassInNames( Class::TypeID, Class::ClassName);
#undef  HONEY_TYPE
#define HONEY_TYPE( a, b, c ) PNT(W_##b)
#include <HoneywellType.hpp>
  KNoName::RegisterClassInNames( CCBHSystem::TypeID, CCBHSystem::ClassName);
  KNoName::RegisterClassInNames( COneMnemoHoney::TypeID, COneMnemoHoney::ClassName);
  KNoName::RegisterClassInNames( COneAlmGrHoney::TypeID, COneAlmGrHoney::ClassName);
  KNoName::RegisterClassInNames( id_PntTypeInfo, "PntTypeInfo");

  KNoName::RegisterInitCBaseCall( RsuHandleInitData );
}
