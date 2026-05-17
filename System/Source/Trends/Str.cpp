#include "CommProc.h"
#include <string>
#include <set>

//#define CROSSLIB_EXPORTS


static CMemStorage MemStorage;
CMemStorageBase * pMemStorage = &MemStorage;

struct CStrNode
  {
  static CStrNode * pStart;
  CStrNode * pLT;
  CStrNode * pGT;
  char Str[1];
  void Init ( int L )
    {
    pLT = NULL;
    pGT = NULL;
    memset ( Str, 0, L + 1 );
    }
  void * NewMem( size_t size );

  static void Init( )
    {
    pStart = (CStrNode*)::NewMem( sizeof(CStrNode));//new CStrNode;
    pStart->pLT = NULL;
    pStart->pGT = NULL;
    pStart->Str[0] = 0;
    }

  static void DoFree(CStrNode *pNode)
    {
	  if (pNode == NULL)
		  return;
	  if ( pNode->pLT )
		  DoFree(pNode->pLT);
	  if ( pNode->pGT )
		  DoFree(pNode->pGT);
	  free(pNode);
    }
  static void Free()
    {
	  DoFree(pStart);
    }
//

  static CStrNode * New( int L )
    {
    int size = sizeof( CStrNode ) + L;
    BYTE * pMem = (BYTE*)::NewMem( size );//new BYTE[size];
//    memset ( pMem, 0, size );
    return (CStrNode*)pMem;
    }

  static void Reset( CStrNode * pNode, void * MaxAddr )
    {
    if ( pNode->pGT )
      {
      Reset( pNode->pGT, MaxAddr );
      if ( pNode->pGT > MaxAddr )
        pNode->pGT = NULL;
      }
    if ( pNode->pLT )
      {
      Reset( pNode->pLT, MaxAddr );
      if ( pNode->pLT > MaxAddr )
        pNode->pLT = NULL;
      }
    }


static const char * Insert ( const char * pStr )
  {
  if ( pStr == NULL )
    return NULL;
  int L = lstrlen( pStr );
  char * pMem = (char*)::NewMem( L + 1 );
  lstrcpy ( pMem, pStr );
  return pMem;
  };
  };

CStrNode * ::CStrNode::pStart = NULL;
//int CStrNode::N = 0;

void InitStr( )
  {
  CStrNode::Init( );
  }

void CStr::Free()
{
	CStrNode::Free();
}

void CStr::Reset( void * pMaxAddr )
  {
  //CStrNode::Reset ( CStrNode::pStart, pMaxAddr );
  // CStrNode * pStart = CStrNode::pStart;
  // pStart->pLT = NULL;
  // pStart->pGT = NULL;
  // pStart->Str[0] = 0;
//  pMemStorage->pFirst = NULL;
  }

void CStr::Set( const char * pStr )
  {
  if ( Str && strcmp ( Str, pStr ) == 0 )
    return;
  if ( pStr == NULL )
    return;
  int L = strlen( pStr );
  Str = NewArr(char,L+1);
  strcpy ( (char*)Str, pStr );
  }

void CStr::operator << ( const char * pStr )
  {
  Set((LPCSTR)pStr );
  }

void CStr::operator = ( const char * pStr )
  {
  Set( (LPCSTR)pStr );
  }

 bool CStr::operator == ( char * pStr )
  {
   if ( lstrcmp ( Str, pStr ) == 0 )
     return true;
   return false;
  }

 bool CStr::operator != ( char * pStr )
  {
    if ( lstrcmp ( Str, pStr ) == 0 )
      return false;
    return true;
  }


bool CStr::operator == ( CStr & S )
  {
  if ( Str == NULL )
    return false;
  if ( S.Str == NULL)
    return false;// Такое поведение у lstrcmp
  if ( lstrcmp ( Str, S.Str ) == 0 )
    return true;
  return false;
  }

bool CStr::operator != ( CStr & S )
  {
  if ( Str == NULL )
    return false;
  if ( S.Str == NULL )
    return false;
  char * s1 = (char*)(Str);
  if ( lstrcmp ( Str, S.Str ) == 0 )
    return true;
  return false;
  }


CStr::operator char *( )
  {
  return (char*)Str;
  }

#define START_BLK 500000
#define DELTA_BLK 50000
#define START_MEM 20000000
#define DELTA_MEM 1000000
////////////////////////////////////////////////////////////
struct CSaveStrNode
  {
  static CSaveStrNode * Tree;
  static char * Mem;
  static DWORD Pos;
  static DWORD kBlk;
  static DWORD TxtSize;
  static DWORD MaxBlk;
  static DWORD kReopenBlk;
  static DWORD kReopenMem;
  static DWORD kFailed;
  static DWORD kTotalTxt;
//
  DWORD pLT;
  DWORD pGT;
  DWORD Str;
  DWORD Hash;
//
  void Init(  )
    {
    pLT = 0;
    pGT = 0;
    Str = 0;
    Hash = 0;
    }
  static void Create( )
    {
    kReopenBlk = 0;
    kReopenMem = 0;
    kFailed = 0;
    kTotalTxt = 0;
    TxtSize = START_MEM;
    MaxBlk = START_BLK;
    Tree = new CSaveStrNode[MaxBlk];
    Mem = new char[TxtSize];
    Mem[0] = 0;
    Pos = 1; // 0 = пустая строка
    kBlk = 1; //Корень в 0
    Tree[0].Init();
    //for ( int n = 0; n < 256; n++ )
    //  {
    //  HashBuf[n] = UL();
    //  }
    }
  static void Close( )
    {
    delete Tree;
    delete Mem;
    Tree = NULL;
    Mem = NULL;
    }
  static void ReOpen( )
    {
    if ( TxtSize - Pos < 512 )
      {
      TxtSize += DELTA_MEM;
      char * NewMem = new char[TxtSize];
      memmove( NewMem, Mem, Pos );
      delete Mem;
      Mem = NewMem;
      kReopenMem++;
      }
    if ( MaxBlk - kBlk < 8 )
      {
      MaxBlk += DELTA_BLK;
      CSaveStrNode * NewTree = new CSaveStrNode[MaxBlk];
      memmove( NewTree, Tree, kBlk * sizeof (CSaveStrNode ));
      delete Tree;
      Tree = NewTree;
      kReopenBlk++;
      }
    }
  //
  static const DWORD A = 0x007B21F3;
  static const DWORD B = 0xB219D459;
  static DWORD GetHash( const char * pStr )
    {
    DWORD H = 0x348A49BC;
//CRC := (CRC shl 5) + CRC + byte;
//CRC := (CRC shl 5) + CRC;
//CRC:=CRC xor (CRC shr 16);
    while ( 1 )
      {
      BYTE s = *pStr++;
      if ( s == 0 )
        {
        break;
        }
      H = ( H << 5 ) + H + s;
      H = ( H << 5 ) + H;
      H = H ^ ( H >> 16 );
//      H = H * A + B;
      }
    return H;
    }

  static DWORD New( const char * pStr, DWORD Hash  )
    {
    ASS(kBlk<MaxBlk)
    int n = kBlk++;
    Tree[n].Init();
    Tree[n].Str = Pos;
    Tree[n].Hash = Hash;
    DWORD Addr = Pos;
    int L = lstrlen ( pStr );
    ASS( Pos + L < TxtSize );
    lstrcpy ( Mem + Pos, pStr );
    Pos += L + 1;
    return Addr;
    }
  static DWORD New( int Len, const void * pData )
    {
    ASS( kBlk < MaxBlk )
    int n = kBlk++;
    Tree[n].Init();
    Tree[n].Str = Pos;
    Tree[n].Hash = 0x7FFFFFFF;
    DWORD Addr = Pos;
    ASS( Pos + Len < TxtSize );
    memcpy ( Mem + Pos, pData, Len );
    Pos += Len;
    return Addr;
    }
  static DWORD Move( const char * pStr  )
    {
    DWORD Addr = Pos;
    int L = lstrlen ( pStr );
    ASS( Pos + L < TxtSize );
    lstrcpy ( Mem + Pos, pStr );
    Pos += L + 1;
    return Addr;
    }
  static DWORD Insert ( const char * pStr )
    {
    if ( pStr[0] == 0 )
      return 0;
    ReOpen( );
    DWORD Hash = GetHash( pStr );
    DWORD Node = Tree[0].pGT; //корень в 0
    kTotalTxt++;
    while( 1 )
      {
//      int Comp = lstrcmp( pStr, Mem + Tree[Node].Str );
      int Comp = 0;
      if ( Hash < Tree[Node].Hash )
        Comp = -1;
      else
        {
        if ( Hash > Tree[Node].Hash )
          Comp = 1;
        }
      if ( Comp == 0 )
        {
        if ( lstrcmp( pStr, Mem + Tree[Node].Str ) == 0 )
          return Tree[Node].Str;
// Ложное совпадение
        kFailed++;
        DWORD Str = Move( pStr );
        return Str;
        }
//      if ( Comp == 0 )
//        return Tree[Node].Str;
      if ( Comp > 0 )
        {
        if ( Tree[Node].pGT )
          {
          Node = Tree[Node].pGT;
          continue;
          }
        // Конец
        Tree[Node].pGT = kBlk;
        DWORD Str = New( pStr, Hash );
        return Str;
        }
      if ( Comp < 0 )
        {
        if ( Tree[Node].pLT )
          {
          Node = Tree[Node].pLT;
          continue;
          }
        // Конец
        Tree[Node].pLT = kBlk;
        DWORD Str = New( pStr, Hash );
        return Str;
        }
      }
    ASS(0)
    return 0;
    }
  static DWORD Insert ( int Len, const void * pData )
    {
    ReOpen( );
    DWORD Node = Tree[0].pGT;
    kTotalTxt++;
    while( 1 )
      {
      if ( Tree[Node].pGT )
        {
        Node = Tree[Node].pGT;
        continue;
        }
      // Конец
      Tree[Node].pGT = kBlk;
      DWORD Data = New( Len, pData );
      return Data;
      }
    ASS(0)
    return 0;
    }
  };


CSaveStrNode * CSaveStrNode::Tree;
char * CSaveStrNode::Mem;
DWORD CSaveStrNode::Pos;
DWORD CSaveStrNode::kBlk;
DWORD CSaveStrNode::TxtSize;
DWORD CSaveStrNode::MaxBlk;
DWORD CSaveStrNode::kReopenBlk;
DWORD CSaveStrNode::kReopenMem;
DWORD CSaveStrNode::kFailed; 
DWORD CSaveStrNode::kTotalTxt; 


void CreateCharBuf( )
  {
  CSaveStrNode::Create( );
  }

void CloseCharBuf( )
  {
  CSaveStrNode::Close( );
  }

char * Mem = NULL;
int MaxMem = 1000000;
DWORD Pos = 0;

DWORD SaveToCharBuf(  const char * pStr )
  {
  return CSaveStrNode::Insert ( pStr );
  }

DWORD SaveToCharBuf( int Len, const void * pData )
  {
  return CSaveStrNode::Insert ( Len, pData );
  }

void GetCharBuf( DWORD & Len, char ** ppMem )
  {
  Len = CSaveStrNode::Pos;
  *ppMem = CSaveStrNode::Mem;
  }

void GetReopen( DWORD & kReopenBlk, DWORD & kReopenMem, DWORD & kFailedHash, DWORD & kTotal )
  {
  kReopenBlk = CSaveStrNode::kReopenBlk;
  kReopenMem = CSaveStrNode::kReopenMem;
  kFailedHash = CSaveStrNode::kFailed;
  kTotal = CSaveStrNode::kTotalTxt;
  }
