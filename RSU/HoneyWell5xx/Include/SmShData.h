#pragma once
#include <shemedata.h>

#undef  SM_TYPE
#define SM_TYPE( a, b, c ) \
struct CSh##b: public CShBase, public W_##b \
  { \
  CSh##b() : CShBase( sizeof(CSh##b) ){}; \
  };
#include "SafetyManagerType.hpp"
