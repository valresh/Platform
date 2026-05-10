// #pragma once
#include "Dsd.h"
#include "FileXML.h"
#include "HTML.h"

class CBin : public CFileXML
  {
  HXML  mHdx;
  CDsd& mDsd;
  public:
    CBin(CDsd& rDsd);
    //
    bool Create( CHTML& html, const char* pszPath, const char* pszFileName );
    CDsd* FindDSO( CHTML& html, int nID, bool &bReadOnly, bool &bParameterFormat );
    HXML  FindBIN( int nID );
  };
