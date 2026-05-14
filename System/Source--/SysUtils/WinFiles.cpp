#include "WinFiles.h"
#include "CommProc.h"


QFile * CreateFile(
  const char * Path,
  QIODeviceBase::OpenMode RW,
  int,
  int,
  QIODeviceBase::OpenMode mode,
  int,
  int )
{
  char Linux_Path[1024];
  strcpy ( Linux_Path, Path );
  char * P = Linux_Path;
  while ( *P )
  {
    if ( *P == '\\' )
      *P = '/';
    P++;
  }
  if ( mode & QIODeviceBase::Truncate )
  {
      bool Res = QFile::remove ( Linux_Path );
      Res = false;
  }
  QFile * pFile = new QFile ( Linux_Path );
  if ( !pFile->open( RW | mode ))//, QFileDevice::ReadOther ))
  {
    QString Err = pFile->errorString();
    delete pFile;
    return NULL;
  }
  return pFile;
}

BOOL ReadFile( QFile * hFile, void * To, size_t Size, DWORD * pLr, int )
{
  qint64 Res = hFile->read((char*)To, Size);
  if ( Res < 0 )
    return FALSE;
  if ( pLr )
    *pLr = Res;
  return TRUE;
}

BOOL WriteFile( QFile * hFile, void * From, size_t Size, DWORD *, int )
{
  qint64 Res = hFile->write((char*)From, Size);
  if ( Res < 0 )
    return FALSE;
  return TRUE;

}
void DeleteFile ( const char * Path )
{
//  QFile file;
  bool Res = QFile::remove( Path );
}

int GetFileSize( QFile * hFile, void * )
{
  hFile->size( ) ;
}

DWORD SetFilePointer(
  QFile * hFile,
  LONG DistanceToMove,
  LONG *,
  DWORD MoveMethod
  )
{
  if ( MoveMethod == 0 )
  {
    bool Res = hFile->seek(DistanceToMove);
    ASS(Res)
    return hFile->pos();
  }
  if ( MoveMethod == 1 )
  {
    bool Res = hFile->seek( hFile->pos()+DistanceToMove);
    return hFile->pos();
  }
  ASS(0)
  return 0;
}

void CloseHandle( QFile * hFile )
{
  hFile ->close();
  delete hFile;
}
extern char LOG_PATH[MAX_PATH];

int Fsend ( const char * File, const char * Fmt, ... )
{
  Char<1024> Path;
  Path.Prt ( "%s/%s", LOG_PATH, File );
  FILE * F = fopen ( Path, "at" );
  if ( F == NULL )
    return -1;
  va_list arg;
  va_start(arg,Fmt);
  int res = vfprintf ( F, Fmt, arg );
  fclose ( F );
  return res;
}

QFindFile::QFindFile()
{
  dwFileAttributes = 0;
  memset ( cFileName, 0, sizeof (cFileName));
  pDir = NULL;
  files = NULL;
  NumbFile = 0;
}
QFindFile::~QFindFile()
{
  if ( pDir )
    delete pDir;
  if ( files )
    delete files;
  pDir = NULL;
  files = NULL;
}
bool QFindFile::Open (const char * File )
{
  char Path[1024];
  strcpy ( Path, File );
  int p = 0;
  while ( Path[p] && p < 1024 )
  {
    if ( Path[p] == '\\' )
      Path[p] = '/';
    if ( Path[p] == '*' )
        break;
    p++;
  }
  if ( Path[p] != '*' )
      return false;
  QStringList Filtr(Path+p);
  Path[p] = 0;
  pDir = new QDir ( Path );
  // Фильтры: только файлы (Files), без системных "." и ".." (NoDotAndDotDot)
  files = new QStringList (pDir->entryList(Filtr,QDir::Files | QDir::NoDotAndDotDot));
  NumbFile = 0;
  if ( NumbFile >= files->size())
      return false;
  strcpy(cFileName, files->at(NumbFile++).toStdString().c_str());
  return true;
  }

BOOL QFindFile::Next()
{
  if ( NumbFile >= files->size())
    return FALSE;
  strcpy(cFileName, files->at(NumbFile++).toStdString().c_str());
  return TRUE;
}

void * FindFirstFile( const char * Path, QFindFile * pFF )
{
  if ( pFF->Open( Path ))
    return pFF;
  return NULL;
}
BOOL FindNextFile( void * , QFindFile * pFF )
{
  return pFF->Next();
}
void FindClose( void * )
{

}
