#include <crossplatform.h>
#include "FlexTranslator.h"
#include <algorithm>

using namespace std;
using namespace ns_translator;

bool KFlexTranslator::IsVar( LPCSTR pszName )
{
  if( m_pOwner )
    return m_pOwner->IsVar( pszName );
  return false;
}

int KFlexTranslator::AddVar( LPCSTR pszName, KVar::eType Type )
{
  tVarCont_::iterator it = std::find( Var.begin(), Var.end(), pszName );
  if( Var.end()!=it )
  {
    return (int)std::distance( Var.begin(), it );
  }

  Var.push_back( KVar(pszName, Type) );

  return (int)Var.size()-1;
}
