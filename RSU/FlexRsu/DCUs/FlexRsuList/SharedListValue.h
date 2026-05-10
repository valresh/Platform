#pragma once
#include <UniBufferT.h>
#include <FlexVarInfo.h>
#include <rsuErr.h>

#ifdef FLEXRSULIST_EXPORTS
#define FLEXRSULIST_API _EXP
#else
#define FLEXRSULIST_API _IMP
#endif

#pragma warning( push )
#pragma warning( disable : 4251 )
struct SFullVarInfo : SVarInfo
{
  DWORD structType;
};

class FLEXRSULIST_API SListValue : SUniBufferT<SFullVarInfo>
{
  /*struct SUniHAND : public SUniBufferT<HNAME>
  {
  };*/
  
  struct SLocalENUM
  { 
    SLocalENUM() : nPlace(0),nCount(0)
    {}
    size_t nPlace,nCount; 
  };
  struct SUniTENUM : public SUniBufferT<SLocalENUM>
  {
  };

  DECLARE_HANDLE(HCHAR);
  #define INVALID_HCHAR HCHAR(INVALID_UNI)
  struct SUniTChar : public SUniBufferT<char>
  {
    SUniTChar(DWORD dwAdd = 0x10000) : SUniBufferT(dwAdd)
    {
    };
    char* Name( HCHAR n )
    {
      return m_szBuffer + (LONG_PTR)n;
    }
    //
    HCHAR AddBuff( const char* text, int dl = -1 )
    {
      if ( dl < 0 ) dl = lstrlen(text)+1;// Учитываем завершающий 0
      if ( dl == 1 ) return 0;// Пустая строка
      if ( !Realloc( dl ) ) return INVALID_HCHAR;
      char* ptr = m_szBuffer+m_nCurrent;
      memcpy( ptr, text, dl );
      return (HCHAR)AddCurr(dl);
    }
    //
  };

DECLARE_HANDLE(HNAME);
#define INVALID_HNAME HNAME(INVALID_UNI)
  struct SSortLocal
  {
    typedef size_t tOrd_;
    tOrd_ m_nOrd;
    HCHAR m_nName; 
  };
  struct SUniTSort : public SUniBufferT<SSortLocal>
  {
    SUniTSort(SUniTChar& rChar,int nSort = 1,DWORD dwAdd = 0x10000)
      : SUniBufferT(dwAdd), mChar(rChar), m_nSort(nSort)
    {
    };
    //
    SUniTChar& mChar;
    int m_nSort;
    //
    const char* Name(HNAME n)
    {
      SSortLocal* obj = Obj((size_t)n);
      if ( !obj )
        return mChar.m_szBuffer;
      return mChar.Name(obj->m_nName);
    }
    //
    const char* OrdName(size_t n)
    {
      SSortLocal* obj = Obj((size_t)n);
      if ( !obj )
        return mChar.m_szBuffer;
      SSortLocal* obi = Obj(obj->m_nOrd);
      if ( !obi )
        return mChar.m_szBuffer;
      return mChar.Name(obi->m_nName);
    }
    //
    HNAME Add( const char* d )
    {
      int nb;
      HNAME n = Find( d, &nb );
      if ( n != INVALID_HNAME )
        return n;
      return Add( d, nb );
    }
    //
    HNAME Add( const char* d, int nb )
    {
      // Новое имя сразу за nb
      int K = Count();
      ASS( Realloc(1) )
      UINT n = AddCurr();
      SSortLocal* a = Obj(n);
      if ( a == NULL ) return INVALID_HNAME;
      if ( m_nMemSets )
        memset( a, m_nMemSets, sizeof(_Type) );
      a->m_nName = mChar.AddBuff(d);
      if ( nb < K - 1 )
      {
        for ( int m = K - 1; m > nb; m-- )
        {
          SSortLocal::tOrd_* m1 = &m_szBuffer[m].m_nOrd;
          SSortLocal::tOrd_* m2 = &m_szBuffer[m+1].m_nOrd;
          *m2 = *m1;
        }
      }
      //
      SSortLocal* b = Obj(nb+1);
      b->m_nOrd = K;
      ASS( a->m_nName != INVALID_HCHAR )
      return (HNAME)(uint64_t)n;
    }
    //
    void* Plus( const char* d )
    {
      HNAME n = Add( d );
      if ( n == INVALID_HNAME ) return NULL;
      return Obj((size_t)n);
    }
    //
    HNAME Find( const char* d, int* pNB = NULL )
    {
      HCHAR h;
      const int K = Count();
      int nb = -1, ne = K, no, n, Res;
      while ( ne - nb > 1 )
      {
        n  = ( nb + ne ) >> 1;
        //no = *(SSortLocal::tOrd_*)(m_szBuffer+st*n);
        //h  = *(HCHAR*)(m_szBuffer+st*no + offsetof( SSortLocal, m_nName ) );
        no = m_szBuffer[n].m_nOrd;
        h  = m_szBuffer[no].m_nName;
        //
        const char* dll = mChar.Name(h);
        switch( m_nSort )
        {
        case 0: Res = lstrcmp ( dll, d ); break;
        case 1: Res = _Kstricmp( dll, d ); break;
        //case 2: Res = Lstrcmp ( dll, d ); break;
        default:
          ASS(0);
        };
        if ( Res == 0 )
          return (HNAME)(int64_t)no;
        if ( Res < 0 )
          nb = n;
        else
          ne = n;
      }
      if( pNB )
        *pNB = nb;
      return INVALID_HNAME;
    }
    //
  };
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Список отсортированных имён
  struct SUniTName : public SUniTSort
  {
    //  struct SLocal { UINT m_nOrd; HCHAR m_nName; };
    SUniTName(SUniTChar& rChar,int nSort = 1,DWORD dwAdd = 0x10000)
      : SUniTSort(rChar,nSort,dwAdd)
    {
    };
    HNAME Numb( UINT n )
    {
      SSortLocal* obj = (SSortLocal*)Obj(n);
      if ( !obj ) return INVALID_HNAME;
      return (HNAME)obj->m_nOrd;
    }
    const char* Alfa( UINT n )
    {
      return Name( Numb( n ) );
    }
  };

  SUniTENUM mEnum;
  SUniTName mName;
  SUniTChar mChar;
public:
  SListValue();
  void Attach();
  void Detach();
  SFullVarInfo& AddRaw( LPCSTR name, eVarType eType, DWORD structType, tShiftType_ rawshift, tVarSizeType_ size );
  SVarInfo* Find( LPCSTR pszField, DWORD structType );
  DWORD EnumFieldsOf( DWORD eSharedType, LPSTR *ppFieldNames, DWORD countIn, DWORD len4Name );
private:
  void BuildList();
  void BuildName();
};
#pragma warning( pop )
FLEXRSULIST_API SVarInfo* GetFieldInfo( DWORD eSharedType, LPCSTR pszFieldName );
