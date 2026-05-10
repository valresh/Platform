#pragma once
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <mio.hpp>
#include <rsuStr.h>
#include <rsuErr.h>
#include <memory>

#ifndef  INVALID_UNI
#define INVALID_UNI -1
#endif

#pragma warning(push)
#pragma warning( disable: 4311 4267 4312)

static size_t s_MemoryCounter = 0;

template<class T>
struct SUniBufferT
{
protected:
  size_t m_nCurrent;
  struct SMemInfo 
  {
    //mio::mmap_sink hFileMapping;
    void *pMem = nullptr;
    SMemInfo()
    {
      Reset();
    }
    void Reset()
    {
        if (pMem)
            std::free(pMem);
        //hFileMapping.unmap();
        pMem = NULL;
    }
  }m_Mapping[2];
  void* _NewMem( size_t nBytes )
  {
      size_t dwAlloc = ((nBytes / mio::page_size())+1)* mio::page_size();

    for( int i=0; i<countof(m_Mapping); ++i )
    {
      SMemInfo &info = m_Mapping[i];
      if(info.pMem)
        continue;
      std::error_code error;
      info.pMem = std::malloc(dwAlloc);
      //info.hFileMapping.map((mio::file_handle_type)NULL, 0, dwAlloc, error);
      //info.pMem = info.hFileMapping.data();
      if (!info.pMem)
      {
          info.Reset();
      }
      return info.pMem;
    }
    return NULL;
  }
  void _FreeMem( void *p )
  {
    for( int i=0; i<countof(m_Mapping); ++i )
    {
      SMemInfo &info = m_Mapping[i];
      if( info.pMem!=p )
        continue;
      //info.hFileMapping.unmap();
      info.Reset();
      break;
    }
  }
public:
  typedef T _Type;
  SUniBufferT( DWORD dwAdd = sizeof(T)*0x10000 )
    : m_nCurrent(0), m_nMaxBuff(0), m_szBuffer(NULL), m_dwAddBuf(dwAdd), m_nMemSets(0) 
  {
  }
  virtual ~SUniBufferT()
  {
    if ( m_nMaxBuff != 0 )
      _FreeMem( m_szBuffer );
  }
  //
  size_t m_dwAddBuf;
  size_t m_nMaxBuff;
  T*    m_szBuffer;
  int   m_nMemSets;
  //
  bool Realloc(size_t nSize)
  {
    nSize *= sizeof(T);
    if ( (m_nCurrent+nSize) > m_nMaxBuff )
    {
      if( m_dwAddBuf == 0 )
        m_dwAddBuf = sizeof(T) * 100;

      size_t memSize = m_nMaxBuff;

      s_MemoryCounter -= memSize;

      while ( (m_nCurrent+nSize) > memSize )
        memSize += m_dwAddBuf;
      s_MemoryCounter += memSize;
      
      T* t = (T*)_NewMem(memSize);
      if( t == NULL )
      {
        if ( m_szBuffer != NULL )
          s_MemoryCounter += m_nMaxBuff;
        return false;
      }
      m_nMaxBuff = memSize;
      s_MemoryCounter += m_nMaxBuff;
      memset( t, 0, m_nMaxBuff );
      if ( m_szBuffer != NULL )
      {
        memcpy(t,m_szBuffer,m_nCurrent);
        _FreeMem( m_szBuffer );
      }
      m_szBuffer = t;
    }
    return true;
  }
  //
  size_t Count() 
  {
    return size_t(m_nCurrent/sizeof(SUniBufferT::_Type)); 
  }
  //
  T* Obj( size_t n )
  {
    if ( n >= Count() )
      return NULL;
    //return (void*)(m_szBuffer+n*sizeof(SUniBufferT::_Type));
    if( !m_szBuffer )
      return NULL;
    return m_szBuffer + n;
  }
  //
  size_t AddCurr(int nSize = 1)
  {
    size_t nCurr = size_t(m_nCurrent/sizeof(_Type));
    m_nCurrent += (nSize * sizeof(_Type));
    return nCurr;
  }
  // Добавление массива структур
  size_t AddObj( _Type* Void, int N = 1 )
  {
    if ( !Realloc(N) )
      return INVALID_UNI;
    _Type* ptr = (_Type*)(((BYTE*)m_szBuffer)+m_nCurrent);
    memcpy( ptr, Void, N*sizeof(_Type) );
    return AddCurr(N);
  }
  //
  void Clear()
  {
    _FreeMem(m_szBuffer);
    m_nCurrent = 0;
    m_nMaxBuff = 0;
    m_szBuffer = 0;
  }
  void Write(std::ofstream& stream)
  {
      stream.write((char*)&m_nCurrent, sizeof(m_nCurrent));
      if (m_nCurrent != 0)
          stream.write((char*)m_szBuffer, m_nCurrent);
  }
/*  void Write(HANDLE hFile)
  {
    DWORD Lw;
    hFile.write( &m_nCurrent, sizeof(int) );
    if ( m_nCurrent != 0 )
      hFile.write( m_szBuffer, m_nCurrent );
  }*/
  //
  void Read( char*& ptr )
  {
    size_t* N = (size_t*)ptr;
    ptr += sizeof(N[0]);
    char* buff = ptr;
    ptr += N[0];
    m_nMaxBuff = N[0];
    //
    m_nCurrent = m_nMaxBuff;
    if ( m_nMaxBuff )
    {
      s_MemoryCounter += m_nMaxBuff;
      m_szBuffer = (T*)_NewMem(m_nMaxBuff);
      //ASS( m_szBuffer );
      if ( m_szBuffer != NULL )
        memcpy(m_szBuffer,buff,m_nCurrent);
    }
  }
  void Read(std::ifstream& stream)
  {
    size_t N = 0;
    stream.read((char*)&N, sizeof(N));
    //hFile.read( &N, sizeof(N) );
    m_nMaxBuff = N;
    m_nCurrent = m_nMaxBuff;
    if ( m_nMaxBuff )
    {
      s_MemoryCounter += m_nMaxBuff;
      m_szBuffer = (T*)_NewMem(m_nMaxBuff);
      //ASS( m_szBuffer );
      if (m_szBuffer != NULL)
          stream.read((char*)m_szBuffer, m_nCurrent);
        //hFile.read( m_szBuffer, m_nCurrent );
    }
  }
  void Memory( char*& ptr )
  {
    int* N = (int*)ptr;
    ptr += sizeof(int);
    m_szBuffer = (T*)ptr;
    ptr += N[0];
    m_nCurrent = N[0];
    m_nMaxBuff = 0;
  }
  //
  char* Temp( const char* ptr )
  {
    if( ptr == NULL )
      return NULL;
    int dl = strlen(ptr);
    if ( dl == 0 )
      return NULL;
    if ( !Realloc( dl+1 ) )
      return NULL;
    char* cur = ((char*)m_szBuffer)+m_nCurrent;
    strncpy( cur, ptr, dl+1 );
    return cur;
  }
};

struct SUniCharT : public SUniBufferT<char>
{
  SUniCharT(DWORD dwAdd = 0x10000) 
    : SUniBufferT(dwAdd)
  {};
  SUniBufferT::_Type* Name( size_t n ) 
  {
    return m_szBuffer+int(n);
  }
  //
  size_t AddBuff( const char* text, int dl = -1 )
  {
    if ( dl < 0 ) 
      dl = strlen(text)+1;// Учитываем завершающий 0
    if ( dl == 1 )
      return 0;// Пустая строка
    if ( !Realloc( dl ) )
      return INVALID_UNI;
    char* ptr = m_szBuffer+m_nCurrent;
    memcpy( ptr, text, dl );
    return AddCurr(dl);
  }
  //
};

namespace ns_US
{
  struct SLocal 
  { 
    size_t m_nOrd;
    size_t m_nName;
  };
}
// Универсальный список отсортированных имён
template<class T>
struct SUniSortT : public SUniBufferT<T>
{
  SUniSortT( SUniCharT& rChar,int nSort = 1, DWORD dwAdd = sizeof(typename SUniBufferT<T>::_Type)*100)
    : SUniBufferT<T>(dwAdd), mChar(rChar), m_nSort(nSort)
  {};
  //
  SUniCharT& mChar;
  int m_nSort;
  //
  const char* Name( size_t n)
  {
    typename SUniBufferT<T>::_Type* obj = this->Obj(n);
    if ( !obj )
      return mChar.m_szBuffer;
    return mChar.Name(obj->m_nName);
  }
  //
  const char* OrdName( size_t n)
  {
    ns_US::SLocal* obj = this->Obj(n);
    if ( !obj )
      return mChar.m_szBuffer;
    typename SUniBufferT<T>::_Type* obi = this->Obj(obj->m_nOrd);
    if ( !obi )
      return mChar.m_szBuffer;
    return mChar.Name(obi->m_nName);
  }
  //
  size_t Add( const char* d )
  {
    int nb;
    size_t n = Find( d, &nb );
    if ( n != INVALID_UNI )
      return n;
    return Add( d, nb );
  }
  //
  size_t Add( const char* d, int nb )
  {
    // Новое имя сразу за nb
    int K = this->Count();
    ASS(this->Realloc(1));
    size_t n = this->AddCurr();
    typename SUniBufferT<T>::_Type* a = this->Obj(n);
    if ( a == NULL )
      return INVALID_UNI;
    if ( this->m_nMemSets )
      memset( a, this->m_nMemSets, sizeof(typename SUniBufferT<T>::_Type) );
    a->m_nName = mChar.AddBuff(d);
    if ( nb < K - 1 )
    {
      for ( int m = K - 1; m > nb; m-- )
      {
        typename SUniBufferT<T>::_Type* inf1 = this->m_szBuffer+m;
        typename SUniBufferT<T>::_Type* inf2 = this->m_szBuffer+m+1;
        LPCSTR na1 = mChar.Name(inf1->m_nName);
        LPCSTR na2 = mChar.Name(inf2->m_nName);
        inf2->m_nOrd = inf1->m_nOrd;
        //this->KKK();
      }
    }
      //
    typename SUniBufferT<T>::_Type* b = this->Obj(nb+1);
    LPCSTR nab = mChar.Name(b->m_nName);
    b->m_nOrd = K;
    //ASS( a->m_nName != INVALID_UNI )
    return (size_t)n;
  }
  void Trace()
  {
    int K = this->Count();
    for( int i=0; i<K; ++i )
    {
      typename SUniBufferT<T>::_Type* inf = this->m_szBuffer+i;
      LPCSTR na = mChar.Name(inf->m_nName);
      char buf[512];
      sprintf_s( buf, "i=%d ord=%d %s\n", i, inf->m_nOrd, na );
      this->OutputDebugString( buf );
    }
  }
  //
  void* Plus( const char* d )
  {
    size_t n = Add( d );
    if ( n == INVALID_UNI ) 
      return NULL;
    return this->Obj( size_t(n) );
  }
  //
  size_t Find( const char* d, int* pNB = NULL )
  {
    size_t h;
    int K = this->Count();
    int nb = -1, ne = K, no, n, Res;
    while ( ne - nb > 1 )
    {
      n  = ( nb + ne ) >> 1;
      typename SUniBufferT<T>::_Type *inf = this->m_szBuffer+n;
      no = inf->m_nOrd;
      h  = inf->m_nName;

      typename SUniBufferT<T>::_Type *infByOrd = this->m_szBuffer+inf->m_nOrd;
      LPCSTR dll = mChar.Name(infByOrd->m_nName);
      //LPCSTR dll = OrdName( n );
      //
      switch ( m_nSort )
      {
      case 0: Res = strcmp ( dll, d ); break;
      case 1: Res = _Kstricmp( dll, d ); break;
      case 2: Res = strcmp ( dll, d ); break;
      };
      if ( Res == 0 )
      {
        if( pNB ) 
          *pNB = nb;
        return no;
      }
      if ( Res < 0 ) 
        nb = n;
      else
        ne = n;
    }
    if( pNB ) 
      *pNB = nb;
    return INVALID_UNI;
  }
  //
};
// Список отсортированных имён
struct SUniNameT : public SUniSortT<ns_US::SLocal>
{
  SUniNameT(SUniCharT& rChar,int nSort = 1,DWORD dwAdd = sizeof(typename SUniBufferT::_Type)*100)
    : SUniSortT(rChar,nSort,dwAdd){};
  size_t Numb( size_t n )
  {
    SUniBufferT::_Type* obj = (SUniBufferT::_Type*)Obj(n);
    if ( !obj )
      return INVALID_UNI;
    return obj->m_nOrd;
  }
  const char* Alfa( size_t n )
  {
    return Name( Numb( n ) );
  }
};

#pragma warning( pop )
