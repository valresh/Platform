#pragma once

//#include <stdint.h>
//#include <unistd.h>
//#include <limits.h>
//#include <fstream>
#include "string.h"
#ifndef NO_CPP
#include <cstdarg>
#endif
//#include <iostream>
//#include <cassert>
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wconversion-null"

//#define NULL nullptr

#ifndef VOID
#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef int LONG;
//typedef uint64_t UINT64;
//typedef int64_t INT64;
#if !defined(MIDL_PASS)
typedef int INT;
#endif

#ifndef _WINTYPES_DEFINED
#define _WINTYPES_DEFINED

typedef unsigned int ULONG;
typedef ULONG* PULONG;
typedef unsigned short USHORT;
typedef USHORT* PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR* PUCHAR;
typedef char* PSZ;

//typedef long long tDigital64;
typedef long long __time64_t;
typedef long long INT_PTR, * PINT_PTR;
typedef unsigned long long UINT_PTR, * PUINT_PTR;
typedef long long LONG_PTR, * PLONG_PTR;
typedef unsigned long long ULONG_PTR, * PULONG_PTR;

typedef unsigned int        DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT* PFLOAT;
typedef BOOL* PBOOL;
typedef BOOL* LPBOOL;
typedef BYTE* PBYTE;
typedef BYTE* LPBYTE;
typedef int* PINT;
typedef int* LPINT;
typedef WORD* PWORD;
typedef WORD* LPWORD;
typedef int* LPLONG;
typedef DWORD* PDWORD;
typedef DWORD* LPDWORD;
typedef ULONG_PTR DWORD_PTR, * PDWORD_PTR;
typedef void* LPVOID;
typedef void *PVOID;
typedef const void* LPCVOID;
typedef LONG_PTR            LPARAM;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int* PUINT;
//typedef int64_t __int64;

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    long long QuadPart;
} LARGE_INTEGER;
typedef DWORD   COLORREF;

typedef CHAR* PCHAR, * LPCH, * PCH;
typedef const CHAR* LPCCH, * PCCH;

typedef CHAR* NPSTR, * LPSTR, * PSTR;
typedef PSTR* PZPSTR;
typedef const PSTR* PCZPSTR;
typedef const CHAR* LPCSTR, * PCSTR;
typedef PCSTR* PZPCSTR;
typedef const PCSTR* PCZPCSTR;

typedef CHAR* PZZSTR;
typedef const CHAR* PCZZSTR;

typedef CHAR* PNZCH;
typedef const CHAR* PCNZCH;

typedef LPCH LPTCH, PTCH;
typedef LPCCH LPCTCH, PCTCH;
typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;
typedef PZZSTR PZZTSTR, PUZZTSTR;
typedef PCZZSTR PCZZTSTR, PCUZZTSTR;
typedef PZPSTR PZPTSTR;
typedef PNZCH PNZTCH, PUNZTCH;
typedef PCNZCH PCNZTCH, PCUNZTCH;
typedef char TCHAR;

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Path Manipulation
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Sizes for buffers used by the _makepath() and _splitpath() functions.
// note that the sizes include space for 0-terminator
#define _MAX_PATH   260 // max. length of full pathname
#define _MAX_DRIVE  3   // max. length of drive component
#define _MAX_DIR    256 // max. length of path component
#define _MAX_FNAME  256 // max. length of file name component
#define _MAX_EXT    256 // max. length of extension component
#define CP_UTF8                   65001       // UTF-8 translation
#define MAX_PATH          260
#ifndef FALSE
#define FALSE               0
#endif
#ifndef TRUE
#define TRUE                1
#endif
#endif //_WINTYPES_DEFINED
#define LF_FACESIZE 32
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#define _TRUNCATE ((size_t)-1)
#define MAKELPARAM(l, h)      ((LPARAM)(DWORD)MAKELONG(l, h))

#define lstrcat strcat
#define lstrlen(a) strlen(a)
#define lstrcpy(a,b) strcpy(a,b)
#define lstrcpyn(a,b,n) strncpy(a,b,n)
#define StrCpy(a,b)  strcpy(a,b)
#define strcpy(a,b) strcpy(a,b)
#define strncpy(a,b,n) strncpy(a,b,n)
#define StrCat  strcat
#define InterlockedCompareExchange __sync_val_compare_and_swap
#define _T(a) (a)
#define dllexport Q_DECL_EXPORT
#define dllimport Q_DECL_IMPORT
#define __declspec(A) A


#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

#endif

static inline int strcmps(const char* _Str1, const char* _Str2)
{
    if (_Str1 && _Str2)
        return strcmp(_Str1, _Str2);
    else
        if (_Str1 == _Str2)
            return 0;
        else
            return _Str1 > _Str2 ? 1 : -1;
}

// #ifndef _WIN32
// static inline int _strnicmp(const char* s1, const char* s2, size_t len)
// {
//     int diff = 0;
//     while (len-- && *s1 && *s2) {
//         if (*s1 != *s2)
//             if (diff = (int)std::tolower(*s1) - (int)std::tolower(*s2))
//                 break;
//         s1++;
//         s2++;
//     }
//     return diff;
// }
// #endif
#undef lstrcmp
#define lstrcmp strcmps


#ifdef max
#undef max
#endif // max


#ifndef _WIN32
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==0
#endif

// #ifndef _WIN32
// static inline int sprintf_s(char* buffer, size_t bufsz, const char* format, ...)
// {
//     va_list args;
//     va_start(args, format);
//     int res = vsnprintf(buffer, bufsz, format, args);
//     va_end(args);
//     return res;
// }

// template <size_t size>
// static inline int sprintf_s(char(&buffer)[size], const char* format, ...)
// {
//     va_list args;
//     va_start(args, format);
//     int res = vsnprintf(&buffer[0], size, format, args);
//     va_end(args);
//     return res;
// }
// #endif


//#define _access access

#ifndef _WIN32
#ifndef	_countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif
#endif

#ifndef _WIN32
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

#ifndef _WIN32
//returns zero on success, returns non-zero on error. Also, on error, writes zero to dest[0] (unless dest is a null pointer or destsz is zero or greater than RSIZE_MAX).
static inline int strcpy_s(char* strDest, size_t numberOfElements, const char* strSource, size_t count)
{
    if (count > numberOfElements)
        return 1;
    if ( strSource && numberOfElements > 0)
    {
        strcpy(strDest, strSource);
        return 0;
    }
    else
    {
        if (strDest)
            strDest[0] = '\0';
        return 1;
    }
}

static inline int strcat_s(char* strDestination, size_t numberOfElements, const char* strSource)
{
    if (!strDestination || !strSource)
        return 1;
    if (strlen(strDestination) + strlen(strSource) > numberOfElements)
        return 1;
    strcat(strDestination, strSource);
    return 0;
}
#ifndef NO_CPP
template <size_t size>
static inline int strcpy_s(const char(&strDest)[size], const char* strSource, size_t count)
{
  return strcpy_s((char*)&strDest[0], size, strSource, count);
}
template <size_t size>
static inline int strcat_s(char(&strDestination)[size], const char* strSource)
{
  return strcat_s(strDestination, size, strSource);
}
#endif

#endif            

#ifndef _WIN32
#ifndef NO_CPP
static inline int strcpy_s(char* dest, size_t destLen, const char* src) {
    if (dest && src) {
        //return snprintf(dest, destLen, "%s", src);
        //size_t destLen = strlen(dest);
        size_t srcLen  = strlen(src);
        size_t copyLen = (srcLen < destLen) ? srcLen : destLen;
        for (size_t i = 0; i < copyLen; ++i) {
            dest[i] = src[i];
        }
        dest[copyLen] = '\0';
        return 0;
    }
    else
    {
        if (dest)
            dest[0] = '\0';
        return 1;
    }
}
template <size_t size>
static inline int strcpy_s(const char(&strDest)[size], const char* strSource)
{
    return strcpy_s((char*)&strDest[0], size, strSource);
}
#endif
#endif

#define _finite finite
#define $


#ifndef ASSERT
#define ASSERT(x) assert(x)
#endif
#ifndef ASS
#define ASS(x) assert(x);
#endif

#define ALLOW_PADDINGS
#ifdef ALLOW_PADDINGS
#define PADDING(size) char __fix_alignment_0[size];
#define PADDING1(size) char __fix_alignment_1[size];
#define PADDING2(size) char __fix_alignment_2[size];
#define PADDING3(size) char __fix_alignment_3[size];
#define PADDING4(size) char __fix_alignment_4[size];
#else
#define PADDING(size)
#define PADDING1(size)
#define PADDING2(size)
#define PADDING3(size)
#define PADDING4(size)
#endif
////////////////////////////////////////////
//#define lstrcpy strcpy
//#define warning(A,B) once
