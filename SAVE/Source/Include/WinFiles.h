#ifndef WINFILES_H
#define WINFILES_H

#include "crossplatform.h"
//#include <QtCore/qglobal.h>
#include <QFile>
#include <QThread>

#undef _HANDLE
#define _HANDLE QFile *

#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

#define INVALID_HANDLE_VALUE NULL
#define GENERIC_READ QIODeviceBase::ReadOnly
#define GENERIC_WRITE QIODeviceBase::WriteOnly
#define OPEN_EXISTING QIODeviceBase::ExistingOnly
#define CREATE_ALWAYS QIODeviceBase::Truncate
#define FILE_ATTRIBUTE_ARCHIVE 0
#define FILE_SHARE_READ 0
#define HMODULE void *
#define NEW
#define APIENTRY
IN_DLL class QFile * CreateFile(
  const char * Path,
  QIODeviceBase::OpenMode RW,
  int,
  int,
  QIODeviceBase::OpenMode mode,
  int,
  int );


#undef ReadFile
IN_DLL BOOL ReadFile( QFile * hFile, void * To, size_t Size, DWORD * pLr, int );
IN_DLL BOOL WriteFile( QFile * hFile, void * From, size_t Size, DWORD *, int );
IN_DLL void DeleteFile ( const char * Path );
IN_DLL _HANDLE Load_Dll ( const char * DllName, const char * DllPath );
IN_DLL void * GetProcAddress( HMODULE hLib, const char * FuncName );
IN_DLL int GetFileSize( QFile * hFile, void * );

#define FILE_BEGIN 0
#define FILE_CURRENT 1
DWORD SetFilePointer(
  QFile * hFile,
  LONG lDistanceToMove,
  LONG *,
  DWORD dwMoveMethod
  );

IN_DLL void CloseHandle( QFile * hFile );
#define WIN32_FIND_DATA QFindFile
#define FILE_ATTRIBUTE_DIRECTORY 1
#define INVALID_HANDLE_VALUE NULL
#define HANDLE void *
#include <QDir>

struct IN_DLL QFindFile
{
    DWORD dwFileAttributes;
    char cFileName[128];
    QDir * pDir;
    QStringList * files;
    int NumbFile;
    QFindFile();
    ~QFindFile();
    bool Open (const char * File );
    BOOL Next();
};

IN_DLL void * FindFirstFile( const char * Path, QFindFile * pFF );
IN_DLL BOOL FindNextFile( void * , QFindFile * pFF );
IN_DLL void FindClose( void * );

/////////////////////////////////////////////////////////
/// Zip
#if 0
#define unzFile void *
typedef unsigned int   uInt;  /* 16 bits or more */
typedef unsigned long  uLong; /* 32 bits or more */
#define ZEXPORT Q_DECL_IMPORT
#define OF(A) A
#define voidp void *
//  unsigned int

/* tm_unz contain date/time info */
typedef struct tm_unz_s
{
    uInt tm_sec;            /* seconds after the minute - [0,59] */
    uInt tm_min;            /* minutes after the hour - [0,59] */
    uInt tm_hour;           /* hours since midnight - [0,23] */
    uInt tm_mday;           /* day of the month - [1,31] */
    uInt tm_mon;            /* months since January - [0,11] */
    uInt tm_year;           /* years - [1980..2044] */
} tm_unz;

typedef struct unz_file_info_s
{
    uLong version;              /* version made by                 2 bytes */
    uLong version_needed;       /* version needed to extract       2 bytes */
    uLong flag;                 /* general purpose bit flag        2 bytes */
    uLong compression_method;   /* compression method              2 bytes */
    uLong dosDate;              /* last mod file date in Dos fmt   4 bytes */
    uLong crc;                  /* crc-32                          4 bytes */
    uLong compressed_size;      /* compressed size                 4 bytes */
    uLong uncompressed_size;    /* uncompressed size               4 bytes */
    uLong size_filename;        /* filename length                 2 bytes */
    uLong size_file_extra;      /* extra field length              2 bytes */
    uLong size_file_comment;    /* file comment length             2 bytes */

    uLong disk_num_start;       /* disk number start               2 bytes */
    uLong internal_fa;          /* internal file attributes        2 bytes */
    uLong external_fa;          /* external file attributes        4 bytes */

    tm_unz tmu_date;
} unz_file_info;

typedef voidp zipFile;

extern "C" {
unzFile Q_DECL_IMPORT unzOpen ( const char * path);
int Q_DECL_IMPORT unzGetCurrentFileInfo (unzFile file,
                                        unz_file_info *pfile_info,
                                        char *szFileName,
                                        uLong fileNameBufferSize,
                                        void *extraField,
                                        uLong extraFieldBufferSize,
                                        char *szComment,
                                        uLong commentBufferSize);
int ZEXPORT unzOpenCurrentFilePassword OF((unzFile file,
                                           const char* password));
int ZEXPORT unzReadCurrentFile OF((unzFile file,
                                   voidp buf,
                                   unsigned len));
int ZEXPORT unzClose OF((unzFile file));
zipFile ZEXPORT zipOpen OF((const char *pathname, int append));
int ZEXPORT zipWriteInFileInZip OF((zipFile file,
                                           const void* buf,
                                           unsigned len));

int getFileCrc(const char* filenameinzip,void*buf,unsigned long size_buf,unsigned long* result_crc);
int ZEXPORT zipCloseFileInZip OF((zipFile file));

typedef struct tm_zip_s
{
    uInt tm_sec;            /* seconds after the minute - [0,59] */
    uInt tm_min;            /* minutes after the hour - [0,59] */
    uInt tm_hour;           /* hours since midnight - [0,23] */
    uInt tm_mday;           /* day of the month - [1,31] */
    uInt tm_mon;            /* months since January - [0,11] */
    uInt tm_year;           /* years - [1980..2044] */
} tm_zip;

typedef struct
{
    tm_zip      tmz_date;       /* date in understandable format           */
    uLong       dosDate;       /* if dos_date == 0, tmu_date is used      */
    /*    uLong       flag;        */   /* general purpose bit flag        2 bytes */

    uLong       internal_fa;    /* internal file attributes        2 bytes */
    uLong       external_fa;    /* external file attributes        4 bytes */
} zip_fileinfo;

};

#define UNZ_OK 0
#define ZIP_OK 0
#endif

// #ifdef LINUX
// #include <QtCore/qglobal.h>
// #ifdef UTILS_EXPORTS
// #define IN_DLL Q_DECL_EXPORT
// #else
// #define IN_DLL Q_DECL_IMPORT
// #endif
// #else

#endif // WINFILES_H
