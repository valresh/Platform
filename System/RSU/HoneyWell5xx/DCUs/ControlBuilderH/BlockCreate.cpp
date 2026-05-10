#include <rsuErr.h>
#include <H_Class.h>

SBlockCreate * SBlockCreate::pFirst = NULL;
SBlockCreate * SBlockCreate::pLast = NULL;

SBlockCreate::SBlockCreate( LPCSTR pType,	tfHCreateNP pCreate )
{
  Link( pType );
  fnCreateNP = pCreate;
  //fnCreateIP = NULL;
  fnCreateRP = NULL;
  iParam = INT_MAX;
}

/*SBlockCreate::SBlockCreate( LPCSTR pType, tfHCreateIP pCreate, int iParam )
{
  Link( pType );
  fnCreateIP = pCreate;
  this->iParam = iParam;
  fnCreateNP = NULL;
  fnCreateRP = NULL;
}*/

SBlockCreate::SBlockCreate( LPCSTR pType, tfHCreateRP pCreate )
{
  Link( pType );
  fnCreateRP = pCreate;
  this->iParam = iParam;
  //fnCreateIP = NULL;
  fnCreateNP = NULL;
}

void SBlockCreate::Link( LPCSTR pType )
{
  if ( pFirst == NULL )
    pFirst = this;
  if ( pLast )
    pLast -> pNext = this;
  pLast = this;
  pNext = NULL;
  this->pType = pType;
}

SH_Block * SBlockCreate::Create( LPCSTR dcuName, LPCSTR hmiName, LPCSTR pszRef2RealBlock, LPCSTR pType )
{
  for( SBlockCreate * pC = pFirst; pC; pC = pC->pNext )
  {
    if( _stricmp( pType, pC->pType ) )
      continue;
    SH_Block *pB = NULL;
    if( pC->fnCreateNP )
      pB = (*(pC->fnCreateNP))( dcuName, hmiName );
    /*else if( pC->fnCreateIP )
      pB = (*(pC->fnCreateIP))( dcuName, hmiName, pC->iParam );*/
    else if( pC->fnCreateRP )
      pB = (*(pC->fnCreateRP))( dcuName, hmiName, pszRef2RealBlock );
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

bool SBlockCreate::IsSupportedType( LPCSTR pType )
{
  for( SBlockCreate * pC = pFirst; pC; pC = pC->pNext )
  {
    if( strcmp( pType, pC->pType ) )
      continue;
    return true;
  }
  return false;
}
