#pragma once
#include <CommonRSUs.h>

typedef WORD typeHASH_;
typeHASH_ COMMONRSUS_API CreateHASH16( LPCSTR pszText );

typedef DWORD typeHASH32_;
typeHASH32_ COMMONRSUS_API CreateHASH32( LPCSTR pszText, bool bToUpper = false );
typeHASH32_ COMMONRSUS_API CreateHASH32( LPCSTR pszText, typeHASH32_ start_crc, bool bToUpper = false );
typeHASH32_ COMMONRSUS_API CreateHASH32( const BYTE *pBuf, typeHASH32_ start_crc, const size_t L );