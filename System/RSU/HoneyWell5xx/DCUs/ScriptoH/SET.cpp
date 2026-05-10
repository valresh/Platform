#include "Trans.h"
#include <rsuErr.h>

using namespace ns_translator;

void KAaTranslator::ArithOP( eYToken_value AOP )
{
  switch( AOP )
  {
  default:
  case eytFUNCTION:
    if( PosSt >= _countof(Stack) )
      KTHROW( !"Стек мал" );
    if( !kOP && !PosSt )
    {
      if( eytLP!=AOP && eytFUNCTION!=AOP && eytNOT!=AOP )
      {
        ASSD( eytPLUS==AOP );
        break;
      }
    }
    Stack[PosSt++] = AOP;
    break;
  }
}
