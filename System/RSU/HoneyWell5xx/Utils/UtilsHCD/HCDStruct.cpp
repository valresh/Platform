#include <HcdDataTypes_.h>
#include <HcdStruct.h>

#undef  HCD_TYPE
#define HCD_TYPE( a, b, c ) LPCSTR W_##b::ClassName = c;
#include <HCDType.hpp>
