#pragma once

template<class T>
class KMethodOnDestroy
{
  typedef void(T::*tAny)();
  T &m_t;
  tAny m_method;
public:
  KMethodOnDestroy( T &t, tAny a ) : m_t(t), m_method(a){}
  ~KMethodOnDestroy()
  {
    T *p = &m_t;
    (p->*m_method)();
  }

  operator T()
  {
    return m_t;
  }
};
