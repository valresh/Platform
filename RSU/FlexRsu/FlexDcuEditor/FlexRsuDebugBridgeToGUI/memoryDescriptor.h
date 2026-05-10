#pragma once
#include <FlexVarTypes.h>

namespace n_GUI
{
  struct SSignalMemDescr
  {
    void* memAddr;
    eVarType type;
    GUID objID;
    LPCSTR pszFieldName;
    SSignalMemDescr()
    {
      ZeroMemory( this, sizeof(*this) );
    }
  };

  struct SSignalDescriptor
  {
    SSignalDescriptor()
    {}
    SSignalMemDescr srcMem;
    SSignalMemDescr destMem;
  };

  typedef int(*lpfnOpenDebugGUI)(LPCSTR pszPath2File, LPCSTR pszSchemeName);

  typedef int(*lpfnOpenDebugGuiWithInfo)(LPCSTR pszPath2File, LPCSTR pszSchemeName, SSignalDescriptor*, DWORD count );

  typedef void(*lpfnCloseAllWindows)();
};
