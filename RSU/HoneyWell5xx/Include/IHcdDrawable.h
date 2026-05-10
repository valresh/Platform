#pragma once
#include "..\HcdFactory\IHcdElement.h"

struct IHcdDrawable
{
  virtual LPCSTR GetTreeName() = 0;
  virtual size_t GetChild( IHcdDrawable **ppBuf, size_t count ) = 0;
  virtual size_t GetLogixBlocks( IHcdElement **ppBuf, size_t count ) = 0;
};
