#include "Y_MainLogic.h"
#include <yokoheaders.h>
#include <macros/StrHelps.h>
#include <vector>

bool CY_MainLogic::ReadFcsRef( LPCSTR pszRootFcs )
{
  mNumberOfTags.clear();
  std::filesystem::path szRdb = std::filesystem::path(pszRootFcs) / "ETC/FcsRef";
  szRdb.replace_extension("rdb");
  std::vector<BYTE> btBuf;
  GetProjectFile( szRdb, btBuf);
  if( btBuf.empty() )
    return false;
  BYTE * pData = &btBuf[0];

  SYokogawa200* hdr = (SYokogawa200*)pData;
  SYokogawaID*  pELMD = FindYokoID( hdr, "ELMD" );
  if( !pELMD )
    return false;

  char name[20], number[24];
  ZeroMemory( name, sizeof(name) );
  ZeroMemory( number, sizeof(number) );
  for( DWORD i=0; i<pELMD->dwCount; i++ )
  {
    SELMD *p = (SELMD*)pELMD->Item( pData, i );
    if( !p->tagName[0] )
      continue;
    std::memmove( name, p->tagName, _countof(p->tagName) );
    name[_countof(p->tagName)] = '\0';
    std::memmove( number, p->szType_Number, _countof(p->szType_Number) );
    number[_countof(p->szType_Number)] = '\0';
    tNamedString_::iterator it = mNumberOfTags.find( name );
    ASS( mNumberOfTags.end()==it );
    mNumberOfTags[name] = number;
  }

  return true;
}
