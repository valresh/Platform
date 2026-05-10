#pragma once
#include <eventstring.h>
#include <Common.h>
#include "SMessage.h"

extern int SortMessage(const void* d1, const void* d2);

class CMessageList
{
  enum
  {
    STEP_REALLOC = 100,
  };
public:
  CMessageList(void);
  ~CMessageList(void);

  void Open();
  void Parser( char* ptr );
  bool Realloc();
  int m_nCount;
  SMessage* m_list;
};
