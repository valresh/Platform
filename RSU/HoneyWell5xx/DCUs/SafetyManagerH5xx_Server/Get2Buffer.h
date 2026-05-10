#pragma once
#include <crossplatform.h>

void* Get2Buffer( LPCSTR pszFile, DWORD &size );
void ClearBuffer( void *pMem );

BOOL GetAttr( const char* name, FILETIME& attr  );
//bool EqAttr( _WIN32_FILE_ATTRIBUTE_DATA& a, FILETIME& b );
