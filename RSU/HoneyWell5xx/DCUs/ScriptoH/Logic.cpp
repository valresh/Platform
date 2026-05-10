#include "Trans.h"
#include <rsuErr.h>

using namespace ns_translator;

int KAaTranslator::Prior( eYToken_value AOP )
{
  switch( AOP )
  {
  case eytASSIGN:
    return -10;
  case '|':
    KTHROW( !"проверить действие '|'");
  case eytBITAND:// Bit AND
  case eytBITOR:
  case eytAND:// AND
  case eytOR:
    return -2;
  case eytNOT://NOT
    return -1;
  case eytEQUAL:
  case eytNE:
  case eytGE:
  case eytLE:
  case eytBIG:
  case eytLIT:
    return 0;
  case eytPLUS:
  case eytMINUS:
    return 1;
  case eytMUL:
  case eytDIV:
  case eytPERCENT:
  case eytSHIFT_LEFT:
  case eytSHIFT_RIGHT:
  case eytMOD:
    return 2;
  case '^':
  case '#':
    return 3;
  case '~':
    KTHROW( !"проверить действие '~'");
    return 0;
  }
  ASSD(FALSE)
  return 0;
}

bool KAaTranslator::IsArith( eYToken_value AOP )
{
  if ( AOP == '(' )
    return false;
  return true;
}
