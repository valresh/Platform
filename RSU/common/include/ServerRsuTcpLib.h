#pragma once
#include <crossplatform.h>
#ifdef SERVERRSUTCPLIB_EXPORTS
#define SERVERRSUTCPLIB_API _EXP
#else
#define SERVERRSUTCPLIB_API _IMP
#endif

SERVERRSUTCPLIB_API bool ServerRsuTcpStart();
SERVERRSUTCPLIB_API void ServerRsuTcpStop();
