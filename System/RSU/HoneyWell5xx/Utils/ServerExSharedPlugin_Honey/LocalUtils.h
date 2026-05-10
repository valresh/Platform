#pragma once
#include <ServerExPluginTypes.h>
#include <DataTypes_.h>

void InitiaizeLocalUtils();
CBase* _FindConnPoint( LPCSTR ObjName, DWORD TypeID, int number );
bool _GetNameEx( CBase* pB, LPCSTR* pszClass, LPCSTR* pszEntry, int *pFcsNumber );
DWORD _FindHoney( DWORD qRsuNumber, LPSTR pszNameIn, DWORD type, BYTE **buffer, int &sizeReq, tMemReallocer pMemR );
