#pragma once
#include <vector>
#include <string>

struct IControlBuilder
{
  virtual void GetPathToCnfFile( char *Path, size_t _S, LPCSTR pszFile, LPCSTR pszExt ) = 0;
  virtual void SelectAllConnections( LPCSTR pszName, LPCSTR pszField, bool bInput, std::vector<std::string> &cont ) = 0;
#ifdef _WIN32
  virtual bool ShowACSObject( HWND hMainWnd, char * Name, void * pData, bool bEnternalCall ) = 0;
#endif
  virtual bool GetVar( LPCSTR pBlock, LPCSTR pField, BYTE **ppVar, eVarType *pType, USHORT *pVarSize = NULL, LPCSTR *ppszEnum = NULL ) = 0;
  virtual void EnableMomStateDEVCTL() = 0;
};
