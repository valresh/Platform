#pragma once
//#include "../ARM.h"
#include "BaseHTML.h"
#include "FileXML.h"

//#include "afxtempl.h" находится в Platform 3.0\SYSTEM\Source\MFCLinux\include\
=======

//
class CDsd : public std::vector<CDsd*>, public SBaseDSD
  {
  public:
    CDsd();
   ~CDsd();
    void Clear();
    bool Parser( CHTML& html, char*& ptr );
    bool Create( CHTML& html, const char* pszPath, const char* pszFileName );
    int  Save(CHTML& html);
    //
    CDsd* FindHDX( int nBind );
  };
//
inline bool IsChilds( STegScn* teg, int& n )
  {
  bool b = lstrcmpi( teg[n-1].name, "/" ) == 0;
  if ( b ) n--,teg[n].name = NULL;
  return !b;
  }
//
