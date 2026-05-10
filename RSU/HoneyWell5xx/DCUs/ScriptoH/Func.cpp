#include "Trans.h"

using namespace ns_translator;

struct SFuncNumb 
{
  LPCSTR pName;
  int ID;
};

static SFuncNumb sFuncs[] = 
{
  {"ABS", eyfABS},
  {"DABS", eyfABS},
  {"LABS", eyfABS},
  {"SQRT", eyfSQRT},
  {"INT", eyfINT},
  {"POWER", eyfPOWER},
  {"LOG", eyfLOG},
  {"LN", eyfLN},
  {"DLIMIT", eyfDLIMIT},
  {"TRUNC", eyfTRUNC},
  {"STRINGREPLACE", eyfSTRINGREPLACE},
  {"STRINGINSTRING", eyfSTRINGINSTRING},
  {"STRINGMID", eyfSTRINGMID},
  {"STRINGTOINTG", eyfSTRINGTOINTG},
  {"STRINGLEN", eyfSTRINGLEN},
  {"STRINGLEFT", eyfSTRINGLEFT},
  {"STRINGRIGHT", eyfSTRINGRIGHT},
  {"LOGDATACHANGEEVENT", eyfLOGDATACHANGEEVENT },
  {"STRINGFROMREAL", eyfSTRINGFROMREAL },
  {"TEXT", eyfTEXT },
  {"NOW", eyfNOW },
  {"MAX", eyfMAX },
  {"ISNAN", eyfISNAN },
  {"EXP", eyfEXP },
  {"ATN", eyfATN },
  {"SIN", eyfSIN },
  {"MIN", eyfMIN },
  {"ISFIN", eyfISFIN },
  {"AVG", eyfAVG },
};

int KAaTranslator::IsFunc( LPCSTR pszName )
{
  for( int i=0; i<_countof(sFuncs); i++ )
  {
    if( !_strcmpi(pszName,sFuncs[i].pName) )
      return sFuncs[i].ID;
  }
  return -1;
}

LPCSTR KAaTranslator::IsDefine( std::string &szVal )
{
  tStrByStrCont_::iterator it = m_Defines.find(szVal);
  if( m_Defines.end()!=it )
    return it->second.c_str();
  return NULL;
}
