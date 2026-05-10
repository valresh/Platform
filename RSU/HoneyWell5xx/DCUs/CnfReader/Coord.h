#pragma once
#include <TinyXmlReader.h>

struct SRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
    bool EqualsTo(const SRECT& rect)
    {
        return left == rect.left &&
            top == rect.top &&
            right == rect.right &&
            bottom == rect.bottom;
    }
    void SetRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom)
    {
        left = xLeft;
        top = yTop;
        right = xRight;
        bottom = yBottom;
    }
    void OffsetRect(int dx, int dy)
    {
        left += dx;
        right += dx;
        top += dy;
        bottom += dy;
    }
#ifdef _WIN32
    operator RECT& ()
    {
        return *ToRect();
    }
/*    operator LPRECT()
    {
        return ToRect();
    }*/
    RECT* ToRect()
    {
        return (RECT*)this;
        /*/RECT* res = (RECT*)this;
        res.left = left;
        res.top = top;
        res.right = right;
        res.bottom = bottom;
        return res;*/
    }
#endif
};

class KCoord
{
public:
    SRECT coord;
public:
  KCoord(void);
  ~KCoord(void);
  void Parse( tinyxml2::XMLElementPtr &root );
  operator SRECT();
};
