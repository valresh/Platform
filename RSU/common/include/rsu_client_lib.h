#pragma once

#if defined(_WIN32)
#ifdef RSU_CLIENT_LIB_EXPORT
#define RSU_CLIENT_API extern "C" __declspec(dllexport)
#else
#define RSU_CLIENT_API extern "C" __declspec(dllimport)
#endif
#else
#ifdef RSU_CLIENT_LIB_EXPORT
#   define RSU_CLIENT_API extern "C" __attribute__((visibility("default")))
#else
#   define RSU_CLIENT_API extern "C"
#endif
#endif

#include <rsus/rsu_api.h>

RSU_CLIENT_API int rsu_create_client_object(const char* a_ObjName, const char* a_Cfg, rsu_cp::RsuClientAPI* api);
RSU_CLIENT_API void rsu_clear_clients();
