#include <hashRSU.h>
#include <locale>
#include <string.h>
#include "crossstring.h"

static WORD crctable16[256] = { 0 };
static BOOL bInitedTable16 = false;

static void MakeCRC16Table(void)
{
    WORD r;
    for (int i = 0; i < 256; i++)
    {
        r = ((WORD)i) << 8;
        for (BYTE j = 0; j < 8; j++)
        {
            if (r & (1 << 15))
            {
                r = (r << 1) ^ 0x8005;
            }
            else
            {
                r = r << 1;
            }
        }

        crctable16[i] = r;
    }
}

static WORD GetCRC16(BYTE* buf, size_t len)
{
    WORD crc;
    crc = 0xFFFF;
    while (len--)
    {
        crc = crctable16[((crc >> 8) ^ *buf++) & 0xFF] ^ (crc << 8);
    }

    crc ^= 0xFFFF;
    return crc;
}

typeHASH_ CreateHASH16(LPCSTR pszText)
{
    char szUpperName[_MAX_PATH * 4] = { 0 };
    strcpy(szUpperName, pszText);
    const size_t L = strlen(szUpperName);
    for (size_t i = 0; i < L; i++)
    {
        szUpperName[i] = toupper(szUpperName[i]);
    }

    typeHASH_ calculate_crc = 0;
    if (!bInitedTable16)
    {
        bInitedTable16 = true, MakeCRC16Table();
    }

    calculate_crc = GetCRC16((BYTE*)szUpperName, L);
    return calculate_crc;
}
//////////////////////////////////////////////////////////////////////////
static DWORD crctable32[256] = { 0 };
static BOOL bInitedTable32 = false;

static void MakeCRC32Table(void)
{
    static const BYTE p[] = { 0,1,2,4,5,7,8,10,11,12,16,22,23,26 };

    DWORD c;
    DWORD n, k;
    DWORD poly;
    poly = 0L;
    for (n = 0; n < sizeof(p); n++)
    {
        poly |= 1L << (31 - p[n]);
    }
    
    for (n = 0; n < 256; n++)
    {
        c = n;
        for (k = 0; k < 8; k++)
        {
            c = c & 1 ? poly ^ (c >> 1) : c >> 1;
        }

        crctable32[n] = c;
    }
}

#define DO1(buf) crc = crctable32[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);

/* ========================================================================= */
DWORD crc32(DWORD crc, const BYTE* buf, size_t len)
{
    crc = crc ^ 0xffffffffL;
    while (len >= 8)
    {
        DO8(buf);
        len -= 8;
    }

    if (len)
    {
        do {
            DO1(buf);
        } while (--len);
    }

    return crc ^ 0xffffffffL;
}

typeHASH32_ CreateHASH32(const BYTE* pBuf, typeHASH32_ start_crc, const size_t L)
{
    if (!bInitedTable32)
    {
        bInitedTable32 = true, MakeCRC32Table();
    }

    typeHASH32_ calculate_crc = crc32(start_crc, pBuf, L);
    return calculate_crc;
}

typeHASH32_ CreateHASH32(LPCSTR pszText, typeHASH32_ start_crc, bool bToUpper /*= false*/)
{
    std::string result(pszText);
    if (bToUpper)
    {
        result = cross::string::toupper(pszText);
    }

    return CreateHASH32((const BYTE*)result.c_str(), start_crc, result.size());
}

typeHASH32_ CreateHASH32(LPCSTR pszText, bool bToUpper /*= false*/)
{
    return CreateHASH32(pszText, 0, bToUpper);
}
