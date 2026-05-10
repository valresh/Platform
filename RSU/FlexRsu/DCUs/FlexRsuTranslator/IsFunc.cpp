#include "FlexTranslator.h"
#include <rsuStr.h>

using namespace ns_translator;

struct SFuncNumb 
{
  LPCSTR pName;
  int ID;
};

static SFuncNumb sFuncs[] = 
{
  {"SR", eyfSR},
  {"RS", eyfRS},
  {"SQRT", eyfSQRT},
  {"ABS", eyfABS},
  {"RAND", eyfRAND},
};

int KFlexTranslator::IsFunc( LPCSTR pszName )
{
  for( size_t i=0; i<countof(sFuncs); i++ )
  {
    if( !_Kstricmp(pszName,sFuncs[i].pName) )
      return sFuncs[i].ID;
  }
  return -1;
}
