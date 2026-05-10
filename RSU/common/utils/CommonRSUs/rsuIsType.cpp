#include <rsuIsType.h>
#include <locale.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <crossstring.h>

#ifdef _WIN32
#include <inttypes.h>
#else
//#include <cinttypes>
#endif

bool IsFlt(const char* s)
{
    if (!s)
        return false;

    char* ptr = nullptr;
    errno = 0;
    std::strtof(s, &ptr);
    return (errno == 0);
}

bool IsInt(const char* s)
{
    if (!s)
        return false;

    char* ptr = nullptr;
    errno = 0;
    std::strtol (s, &ptr, 10);
    return (errno == 0);
}

double Atos(char* ptr)
{
  //static lconv* conv = localeconv();
  struct lconv* conv = localeconv();
  static char dp = conv->decimal_point[0];
  if ( dp != '.' )
  {
    char* p = ptr;
    while ( *p )
    {
      if ( *p == '.' ) *p = dp;
      p++;
    }
  }
  char* p = ptr;
  while ( *p )
  {
    if (cross::string::starts_with( p, U'ю') || cross::string::starts_with( p, U'Ю' )) *p = dp;
    p++;
  }
  return atof(ptr);
}
//-----------------------------------------------------------------------------
double atos(const char* ptr)
{
  //static lconv* conv = localeconv();
  struct lconv* conv = localeconv();
  static char dp = conv->decimal_point[0];
  if ( dp != '.' )
  {
    const char* p = strchr( ptr, '.' );
    if ( p != NULL )
    {
      int dl = strlen(ptr);
      if ( dl >= 1024 ) return DBL_MAX;
      char str[1024];
      strcpy( str, ptr );
      return Atos( str );
    }
  }
  return atof(ptr);
}
