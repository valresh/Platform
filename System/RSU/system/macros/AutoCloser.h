#pragma once
#include <memory>

namespace detail
{
  class IDel
  {
  public:
    virtual ~IDel(){}
  };

  template< class T, class D>
  class TDel : public IDel
  {
    D m_D;
    T m_T;
  public:
    TDel( T t, D d)
      : m_T( t )
      , m_D( d )
    {
      ;
    }
    virtual ~TDel()
    {
      if( m_T )
        m_D( m_T );
    }
  };
};

template<class T>
class KAutoCloser
{
  T m_H;
  std::unique_ptr<detail::IDel> m_Destr;
public:
  typedef T value_type;
public:
  template<class D>
  KAutoCloser( T h, D d )
  {
    m_H = h;
    m_Destr.reset( new detail::TDel<T,D>(h, d) );
  }
  KAutoCloser()
    : m_H(NULL)
  {
  }
  ~KAutoCloser()
  {
  }
  template<class D>
  void Assign( T h, D d )
  {
    m_H = h;
    m_Destr.reset( new detail::TDel<T,D>(h, d) );
  }
  operator T()
  {
    return m_H;
  }
  void Reset()
  {
    m_Destr.reset( NULL );
    m_H = NULL;
  }
};
