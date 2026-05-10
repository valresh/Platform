#pragma once

#include "CommProc.h"

#undef PARM_D
#define PARM_D(Val,Name,Def) \
  {\
  Char<256>ValName;\
  CTrends * pT = (CTrends*)pData;\
  ValName.Prt( "%s.%s", (char*)ObjName, Name );\
  pT->Add( ValName, 'D', &Val );\
  }

#undef PARM_I
#define PARM_I(Val,Name,Def) \
  {\
  Char<256>ValName;\
  CTrends * pT = (CTrends*)pData;\
  ValName.Prt( "%s.%s", (char*)ObjName, Name );\
  pT->Add( ValName, 'I', &Val );\
  }
