#include <SmDataTypes_.h>
#include <SafetyManagerStruct.h>

#undef  SM_TYPE
#define SM_TYPE( a, b, c ) LPCSTR W_##b::ClassName = c;
#include <SafetyManagerType.hpp>
