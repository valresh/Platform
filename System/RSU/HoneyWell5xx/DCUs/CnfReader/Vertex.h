#pragma once
#include <TINYXML/tinyxml2.h>
#ifdef  _WIN32
#include <atltypes.h>
#endif //  _WIN32


struct SPOINT
{
    int  x;
    int  y;
#ifdef _WIN32
    operator POINT()
    {
        POINT* res = (POINT*)this;
        return *res;
    }
#endif
};

class KVertex
{
  SPOINT vertex;
public:
  KVertex(void)
  {
    vertex.x = 0;
    vertex.y = 0;
  }
  void Parse( tinyxml2::XMLElementPtr &root );
  operator SPOINT()
  {
    return vertex;
  }
};
