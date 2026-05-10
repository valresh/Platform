#include "DataTypes.h"
#include "SysDataTypes.h"

tBuildList ClassBuildList[MAX_SIMPLE_STRUCT];

void CFlags::SaveState( int & Bytes, BYTE * pBuffer )
	{
	memcpy ( pBuffer + Bytes, this, sizeof ( CFlags ));
	Bytes += sizeof ( CFlags );
	}

void CFlags::RestoreState( int & Size, BYTE * pBuffer )
	{
	int TypeFlags = Type_Flags;// Задаются во внешних файлах
	memcpy ( this, pBuffer + Size, sizeof ( CFlags ));
	Size += sizeof ( CFlags );
    Type_Flags = TypeFlags;
	}

CBase * CBase:: pFirstBase = NULL;
CBase * CBase:: pLastBase = NULL;
CBase::CBase()
  {
  if ( pFirstBase == NULL )
      pFirstBase = this;
  void * ptr = pLastBase;
  if ( pLastBase )
    {
//    ptr = &pLastBase->pNextBase;
    pLastBase->pNextBase = this;
    }
    pLastBase = this;
  pNextBase = NULL;
  }

 CBase * FindStruct( int Type, const char * Name )
  {
  CBase * pBase = CBase::pFirstBase;
  while ( pBase )
    {
    if ( pBase->ID_CLASS == Type && pBase->Name.Str && strcmp ( pBase->Name, Name ) == 0 )
      return pBase;
    pBase = pBase->pNextBase;
    }
  return NULL;
  }

 CBase * CreateStruct( int Type, const char * Name )
  {
  switch ( Type )
    {
    case id_System:
      {
        CBase * pBase = new CSystem;
        pBase->ID_CLASS = Type;
        pBase->size = sizeof(CSystem);
        pBase->Name = Name;
        return pBase;
      }
    case id_ExternalCondition:
      {
        CBase * pBase = new CExternalCondition;
        pBase->ID_CLASS = Type;
        pBase->size = sizeof(CExternalCondition);
        pBase->Name = Name;
        return pBase;
      }
    case id_Forsunka:
      {
        CBase * pBase = new CForsunka;
        pBase->ID_CLASS = Type;
        pBase->size = sizeof(CForsunka);
        pBase->Name = Name;
        return pBase;
      }
    case id_GasAnalyz:
      {
        CBase * pBase = new CGasAnalyz;
        pBase->ID_CLASS = Type;
        pBase->size = sizeof(CGasAnalyz);
        pBase->Name = Name;
        return pBase;
      }
    case id_HS:
      {
        CBase * pBase = new CHs;
        pBase->ID_CLASS = Type;
        pBase->size = sizeof(CHs);
        pBase->Name = Name;
        return pBase;
      }
    default:
//      ASS(0)
    return NULL;
    }
  return NULL;
  }

void GiveCommData ( const char * ObjName, int TypeID, void ** ppObj )
  {
  CBase * pObj = CreateStruct( TypeID, ObjName );
  *ppObj = pObj;
  }

int SaveStruct( CBase * pBase )
  {
    switch ( pBase->ID_CLASS  )
    {
      case id_System:
        {
          CSystem * pObj = (CSystem*) pBase;
          return pObj->SaveState( );
        }
      case id_ExternalCondition:
        {
          CExternalCondition * pObj = (CExternalCondition*) pBase;
          return pObj->SaveState( );
        }
      case id_Forsunka:
        {
          CForsunka * pObj = (CForsunka*) pBase;
          return pObj->SaveState( );
        }
      case id_GasAnalyz:
        {
          CGasAnalyz * pObj = (CGasAnalyz*) pBase;
          return pObj->SaveState( );
        }
      case id_HS:
        {
          CHs * pObj = (CHs*) pBase;
          return pObj->SaveState( );
        }
      default:
        ASS(0)
      return __LINE__;
    }
    return __LINE__;
  }

 int RestStruct( CBase * pBase, char * Param)
  {
    switch ( pBase->ID_CLASS  )
    {
      case id_System:
        {
          CSystem * pObj = (CSystem*) pBase;
          return pObj->RestoreState( Param );
        }
      case id_ExternalCondition:
        {
          CExternalCondition * pObj = (CExternalCondition*) pBase;
          return pObj->RestoreState( Param );
        }
      case id_Forsunka:
        {
          CForsunka * pObj = (CForsunka*) pBase;
          return pObj->RestoreState( Param );
        }
      case id_GasAnalyz:
        {
          CGasAnalyz * pObj = (CGasAnalyz*) pBase;
          return pObj->RestoreState( Param );
        }
      case id_HS:
        {
          CHs * pObj = (CHs*) pBase;
          return pObj->RestoreState( Param );
        }
      default:
        ASS(0)
        return __LINE__;
    }
    return __LINE__;
  }
