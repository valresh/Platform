#pragma once
#include <shemedata.h>
#include <H_Class.h>

#undef  HONEY_TYPE
#define HONEY_TYPE( a, b, c ) \
struct CSh##b: public CShBase, public W_##b \
  { \
  CSh##b() : CShBase( sizeof(CSh##b) ){}; \
	};
#include "HoneywellType.hpp"

struct CShOneMnemoHoney : public CShBase, public COneMnemoHoney
{
  SH_BASE(CShOneMnemoHoney)
};

struct CShHoneywellSystem : public CShBase, public CCBHSystem
{
  SH_BASE(CShHoneywellSystem)
};

struct CShOneAlmGrHoney : public CShBase, public COneAlmGrHoney
{
  SH_BASE(CShOneAlmGrHoney)
};