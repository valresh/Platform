#include <CommonY.h>
#include <yokoheaders.h>
#include <stdio.h>
#include <DataTypes_.h>
#include <Names.h>
#include <rsus/rsuPaths.h>
#include <stdlib.h>
//
/*
struct SNameComm
{
  char name[20];
  char comm[28];
};
//
inline void StdName( char* dst, const char* src, const char* fcs )
{
  sprintf_s( dst, 20, "%s%s", src, fcs+2 );
}
//
struct SAliasList
{
  int count;
  SNameComm* lst;
  bool Name( int N, char* dst, const char* src, const char* fcs )
  {
    if ( N < 0 || N >= count )
      return false;
    //
    if ( *lst[N].name )
      lstrcpy( dst, lst[N].name );
    else
      StdName( dst, src, fcs );
    return true;
  }
  bool Comm( int N, char* dst )
  {
    if ( N < 0 || N >= count )
      return false;
    lstrcpy( dst, lst[N].comm );
    return true;
  }
  int Alias( const char* src )
  {
    for ( int n = 0; n < count; n++ )
      if  ( lst[n].name[0] )
        if ( lstrcmp( lst[n].name, src ) == 0 )
          return n+1;
    return -1;
  }
};
//
class CAliasName
{
  enum
  {
    MAX_FCS = 10,
  };
public:
  CAliasName(void)
  {
    memset( fcs, 0, sizeof(fcs) );
  }
  virtual ~CAliasName(void)
  {
    for ( int n =0; n < MAX_FCS; n++ )
      ::BuffFree( fcs[n].lst );
  }
  bool Open( int nFcs, const char* name );
  //
  SAliasList fcs[MAX_FCS];
};

bool CAliasName::Open( int nFcs, const char* name )
{
  ASS( !"Узнать предназначение" );
  // Уже в мозгах
  if ( fcs[nFcs].count > 0 )
    return true;
  // Уже была неудачная попытка чтения этого файла
  if ( fcs[nFcs].count < 0 )
    return false;
  // Выход за пределы массива
  if ( !(0 <= nFcs && nFcs < MAX_FCS) )
  {
    ASS(0);
    return false;
  }
  //
  char szPath[_MAX_PATH];
  sprintf_s( szPath, sizeof(szPath), "%sFCS01%02d\\%s.edf", rsuGetPath2Rsu(), nFcs+1, name );
  //
  fcs[nFcs].count = -1;
  DWORD dwSize;
  char* pData = ::FileToBuff( szPath, dwSize );
  if ( pData == NULL ) return false;
  //
  SYokogawa200* hdr = (SYokogawa200*)pData;
  SYokogawaID* dat = FindYokoID( hdr, "HTLT" );
  if ( dat != NULL && dat->dwSize )
  {
    struct HTLT{char name[16];char comm[24];};
    fcs[nFcs].lst = (SNameComm*)::BuffAlloc( sizeof(SNameComm)*dat->dwCount, true );
    if ( fcs[nFcs].lst != NULL )
    {
      SNameComm* lst = fcs[nFcs].lst;
      fcs[nFcs].count = dat->dwCount;
      char* buffer = pData+dat->dwShift;
      for ( DWORD n = 0; n < dat->dwCount; n++ )
      {
        HTLT* htlt = (HTLT*)buffer;
        if ( htlt->name[0] != '%' )
          memcpy( lst[n].name, htlt->name, sizeof(htlt->name) );
        memcpy( lst[n].comm, htlt->comm, sizeof(htlt->comm) );
        buffer += dat->dwSize;
      }
    }
  }
  //
  ::BuffFree( pData );
  ASS(fcs[nFcs].count > 0);
  return fcs[nFcs].count > 0;
}
//
CAliasName* GS(int fcs)
{
  static CAliasName gs;
  if ( !gs.Open( fcs, "SWITCH\\GSwitchDef" ) )
    return NULL;
  return &gs;
}
//
CAliasName* SW(int fcs,int part)
{
  static CAliasName sw[5];
  switch ( part )
  {
  case 0:if ( !sw[0].Open( fcs, "SWITCH\\SwitchDef"  ) ) return NULL;break;
  case 1:if ( !sw[1].Open( fcs, "SWITCH\\SwitchDef2" ) ) return NULL;break;
  case 2:if ( !sw[2].Open( fcs, "SWITCH\\SwitchDef3" ) ) return NULL;break;
  case 3:if ( !sw[3].Open( fcs, "SWITCH\\SwitchDef4" ) ) return NULL;break;
  case 4:if ( !sw[4].Open( fcs, "SWITCH\\SwitchDef5" ) ) return NULL;break;
  default: ASS(0);
  }
  return &sw[part];
}
//
bool AliasGS( char* dst, const char* src )
{
  if ( src == NULL ) return false;
  if (*src != '%'  )
  {
    lstrcpy( dst, src );
    return true;
  }
  ASS( lstrlen(src) == 8 );
  //
  int nFCS = atoi(src+6)-1;
  CAliasName* an = GS(nFCS);
  if ( an == NULL ) return false;
  int N = (atoi( src+3 )/100)-1;
  char fcs[16] = "\0\0\0\0";
  return an->fcs[nFCS].Name( N, dst, src, fcs );
}
//
bool CommentNames( char* dst, const char* src )
{
  if ( *src != '%'  )
    return false;
  //
  char buf[32];
  lstrcpy( buf, src+3 );
  char* p = strchr( buf, 'S' );
  if ( p == NULL ) return false;
  *p++ = 0;
  int nFCS = atoi( p+2 )-1;
  //
  if ( src[1] == 'G' && src[2] == 'S' )
  {
    CAliasName* an = GS(nFCS);
    if ( an == NULL ) return false;
    int N = (atoi( buf )/100)-1;
    return an->fcs[nFCS].Comm( N, dst );
  }
  //
  if ( src[1] == 'S' && src[2] == 'W' )
  {
    int N = atoi( buf )-1;
    CAliasName* an = SW(nFCS,N/1000);
    if ( an == NULL ) return false;
    return an->fcs[nFCS].Comm( N%1000, dst );
  }
  //
  return false;
}
//
bool AliasToName( char* dst, const char* src, const char* fcs )
{
  if ( fcs == NULL ) //Это поиск комментария
    return CommentNames( dst, src );
  //
  if ( src == NULL )  return false;
  if ( *src != '%'  )
  {
    lstrcpy( dst, src );
    return true;
  }
  if ( src[1] == 'G' && src[2] == 'S' )
    return AliasGS( dst, src );
  //
  if ( src[1] == 'S' && src[2] == 'W' )
  {
    int nFCS = atoi( fcs+5 )-1;
    int N = atoi( src+3 )-1;
    CAliasName* an = SW(nFCS,N/1000);
    if ( an == NULL ) return false;
    return an->fcs[nFCS].Name( N%1000, dst, src, fcs );
  }
  //
  if ( src[1] == 'W' && src[2] == 'B' )
  {
    StdName( dst, src, fcs );
    return true;
  }
  //
  return false;
}
//
int NameToAlias( const char* mod, const char* src, const char* fcs )
{
  if ( src == NULL ) return -1;
  //
  if ( *src == '%' )
    return atoi( src+3 );
  //
  if ( mod == NULL ) return -1;
  if ( fcs == NULL ) return -1;
  int nFCS = atoi( fcs+5 )-1;
  //
  if ( mod[1] == 'G' && mod[2] == 'S' )
  {
    CAliasName* an = GS( nFCS );
    if ( an != NULL )
      return an->fcs[nFCS].Alias( src );
    return -1;
  }
  //
  if ( mod[1] == 'S' && mod[2] == 'W' )
  {
    int N = 0;
    CAliasName* an;
    do
    {
      an = SW(nFCS, N );
      if ( an )
      {
        int n = an->fcs[nFCS].Alias( src );
        if ( n != -1 ) return N*1000+n;
      }
      N++;
    }
    while ( an != NULL );
    return -1;
  }
  //
  if ( mod[1] == 'W' && mod[2] == 'B' )
    return -1;
  //
  return -1;
}
*/
//
CCentumSystem* GetCentumSystemData()
{
  CCentumSystem* pSystem = (CCentumSystem*)::FindConnPoint( "Весь Centum", CCentumSystem::TypeID, -1 );
  if ( pSystem && pSystem->size == sizeof(CCentumSystem) )
    return pSystem;
  return NULL;
}

LPCSTR GetAlarmFileName(bool bDubl)
{
  static CHAR szPath0[_MAX_PATH] = {0};
  static CHAR szPath1[_MAX_PATH] = {0};
  if ( *szPath0 == CHAR('\0'))
  {
    sprintf_s( szPath0, "%sAlarmY-0.dat", rsuGetMemoryPath().c_str() );
    sprintf_s( szPath1, "%sAlarmY-1.dat", rsuGetMemoryPath().c_str() );
  }
  return (bDubl) ? szPath1 : szPath0;
}

template<size_t _S>
void XAXA( char (&szPath)[_S], LPSTR pszName )
{
  sprintf_s( szPath, "%s_%s", rsuGetMemoryPath(), pszName );
}

LPCSTR GetMsgToOperatorFilePath()
{
  static TCHAR szPath[_MAX_PATH] = {0};
  if( *szPath == CHAR('\0') )
    XAXA( szPath, "MsgToOperatorY.dat" );
  return szPath;
}
