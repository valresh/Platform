#include "FlexTranslator.h"
#include <rsuErr.h>

using namespace std;
using namespace ns_translator;

int KFlexTranslator::Prior( eYToken_value AOP )
{
  switch( AOP )
  {
  case eytASSIGN:
    return -3;
  case eytAND:// AND
  case eytOR:
  case eytXOR:
    return -2;
  case eytPLUS:
  case eytMINUS:
    return 1;
  case eytMUL:
  case eytDIV:
  //case '%':
    return 2;
  case eytPOW:
  //case '#':
    return 3;
  /*case '|':
    KTHROW( !"проверить действие '|'");*/
  case eytBITAND:// Bit AND
  case eytBITOR:
    return 4;
  /*case eytNOT://NOT
  case eytValMINUS:
    return 5;*/
  case eytNegation:
  case eytInverseByVar:
    return 5;
  case '~':
    KTHROW( !"проверить действие '~'");
  case eytEQUAL:
  case eytNE:
  case eytGE:
  case eytLE:
  case eytGT:
  case eytLT:
    return 0;
  }
  ASS(true)
    return 0;
}

bool KFlexTranslator::IsArith( eYToken_value AOP )
{
  if ( AOP == '(' )
    return false;
  return true;
}
