#include "QbTranslator.h"

struct SFuncNumb 
{
  LPCSTR pName;
  int ID;
};

static SFuncNumb sFuncs[] = 
{
  {"PARAMVALUE", efPARAMVALUE},
};

int KQbTranslator::IsFunc( LPCSTR pszName )
{
  for( int i=0; i<_countof(sFuncs); i++ )
  {
    if( !_strcmpi(pszName,sFuncs[i].pName) )
      return sFuncs[i].ID;
  }
  return -1;
}
