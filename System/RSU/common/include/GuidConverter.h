#pragma once


typedef struct _GUID {  DWORD Data1;  WORD Data2;  WORD Data3;  BYTE Data4[8];
} GUID;


template< size_t _S >
void GuidToString(GUID guid, char (&szGuid)[_S])
{
  _snprintf_s( szGuid, _S-1,
            "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}

inline GUID StringToGuid( LPCSTR pszGuid )
{
  GUID guid;
  unsigned long p0;
  int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
  
  int err = sscanf(pszGuid, "{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                              &p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);

  guid.Data1 = p0;
  guid.Data2 = p1;
  guid.Data3 = p2;
  guid.Data4[0] = p3;
  guid.Data4[1] = p4;
  guid.Data4[2] = p5;
  guid.Data4[3] = p6;
  guid.Data4[4] = p7;
  guid.Data4[5] = p8;
  guid.Data4[6] = p9;
  guid.Data4[7] = p10;

  return guid;
}

inline GUID StringToGuidWithoutBrackets( LPCSTR pszGuid )
{
  GUID guid;
  unsigned long p0;
  int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
  
  int err = sscanf(pszGuid, "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                              &p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);

  guid.Data1 = p0;
  guid.Data2 = p1;
  guid.Data3 = p2;
  guid.Data4[0] = p3;
  guid.Data4[1] = p4;
  guid.Data4[2] = p5;
  guid.Data4[3] = p6;
  guid.Data4[4] = p7;
  guid.Data4[5] = p8;
  guid.Data4[6] = p9;
  guid.Data4[7] = p10;

  return guid;
}
