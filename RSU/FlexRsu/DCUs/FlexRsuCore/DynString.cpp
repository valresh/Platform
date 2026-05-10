#include <crossplatform.h>
#include <macros/DynString.h>
#include <FlexRsuConnect.h>

void KDynString::AssignString( LPCSTR pszSrc )
{
  size_t len = strlen( pszSrc );
  if( !len )
  {
    Test4Null();
    return;
  }
  m_pString = (LPSTR)pFlexMemoryAllocator( len+1 );
  strcpy_s( m_pString, len+1, pszSrc );
}
void KDynString::Test4Null()
{
  if( m_pString )
    return;
  m_pString = (LPSTR)pFlexMemoryAllocator( 1 );
  *m_pString= 0;
}
