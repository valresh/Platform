#pragma once
#include <locale.h>

namespace ns_strformats
{
  enum
  {
    INT_STR_SIZE  = (CHAR_BIT*sizeof(int)*3/10 + 2),
    INT_SEP_STR_SIZE = (INT_STR_SIZE * 3/2 + 1),
  };
  //#define INT_SEP(x) int_sep((char[INT_SEP_STR_SIZE]) { "" }, INT_SEP_STR_SIZE, x)

  template<size_t _S>
  char *int_sep(char (&s)[_S], int x) 
  {
    struct lconv *locale_ptr = localeconv();
    //const char *grouping = locale_ptr->grouping;
    char sep = locale_ptr->thousands_sep[0];
    if( !sep )
      sep = locale_ptr->decimal_point[0];

    if( _S > 0 )
    {
      int x0 = x;
      char *ptr = s + _S;
      *--ptr = '\0';
      char count = 0;
      do 
      {
        if (count >= 3) //grouping[0]
        {
          *--ptr = sep;
          //if (grouping[1]) grouping++;
          count = 0;
        }
        count++;
        *--ptr = (char) (abs(x % 10) + '0');
      } while (x /= 10);
      if (x0 < 0) 
      {
        *--ptr = '-';
      }
      memmove(s, ptr, (&s[_S] - ptr));
    }
    return s;
  }
}
