#pragma once

namespace ns_translator
{
  struct ITranslatorOwner
  {
    virtual bool IsVar( LPCSTR pszVarName ) = 0;
  };
}
