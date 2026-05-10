#include "FlexTranslator.h"

using namespace std;
using namespace ns_translator;

struct CSysID
{
  const char *Name;
  eYToken_value ID;
};

static CSysID SysID[] = 
{
  "IF", eytIF, 
  "THEN", eytTHEN, 
  "ELSE", eytELSE, 
  "ENDIF", eytEND_IF, 
  "INTEGER", eytINTEGERkeyword,
  "LONG", eytINTEGERkeyword,
  "DOUBLE", eytDOUBLEkeyword,
  "FLOAT", eytDOUBLEkeyword,
  "END", eytEND,
  "OR", eytOR,
  "XOR", eytXOR,
  "AND", eytAND,
  "|", eytBITOR,
  "&", eytBITAND,
};


bool KFlexTranslator::IsSys( eYToken_value &tok, const std::string &szVal )
{
  for( size_t i=0; i< countof(SysID); i++ )
  {
    if( !strcmp(szVal.c_str(), SysID[i].Name) )
    {
      tok = SysID[i].ID;
      return true;
    }
  }
  return false;
}