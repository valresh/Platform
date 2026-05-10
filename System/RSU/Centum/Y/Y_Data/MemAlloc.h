#pragma once
#include <MemoryServiceY.h>
#include <memory>

template<class T> 
T* MemAlloc( int C )
{
  T* p = new (KMemoryServiceY::Instance().NewMemCentum( sizeof(T)*C )) T[C];
  return p;
}
