#include <rsuErr.h>
#include <H_Data.h>

SModuleCreate * SModuleCreate::pFirst = NULL;
SModuleCreate * SModuleCreate::pLast = NULL;

SModuleCreate::SModuleCreate( LPCSTR pType,	tfHCreateNP pCreate )
{
  if ( pFirst == NULL )
    pFirst = this;
  if ( pLast )
    pLast -> pNext = this;
  pLast = this;
  pNext = NULL;
  this->pType = pType;
  fnCreateNP = pCreate;
}

SH_Module * SModuleCreate::Create( LPCSTR dcuName, LPCSTR hmiName, LPCSTR pType )
{
  for( SModuleCreate * pC = pFirst; pC; pC = pC->pNext )
  {
    if( _stricmp( pType, pC->pType ) )
      continue;
    SH_Module *pB = NULL;
    if( pC->fnCreateNP )
      pB = (*(pC->fnCreateNP))( dcuName, hmiName );
    ASS( pB );
    if( pB )
    {
      strcpy_s( pB->BlockName, dcuName );
      strcpy_s( pB->EntityName, hmiName );
      strcpy_s( pB->TypeName, pType );
      pB->CreateCrcs();
    }
    return pB;
  }
  return NULL;
}

bool SModuleCreate::IsSupportedType( LPCSTR pType )
{
  for( SModuleCreate * pC = pFirst; pC; pC = pC->pNext )
  {
    if( strcmp( pType, pC->pType ) )
      continue;
    return true;
  }
  return false;
}
