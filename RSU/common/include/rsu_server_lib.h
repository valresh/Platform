#pragma once

#if defined(_WIN32)
#ifdef RSU_SERVER_LIB_EXPORT
#define RSU_SERVER_API extern "C" __declspec(dllexport)
#else
#define RSU_SERVER_API extern "C" __declspec(dllimport)
#endif
#else
#ifdef RSU_SERVER_LIB_EXPORT
#   define RSU_SERVER_API extern "C" __attribute__((visibility("default")))
#else
#   define RSU_SERVER_API extern "C"
#endif
#endif

#include <rsus/rsu_api.h>
#include "rsu_ex.h"

RSU_SERVER_API int rsu_create_server_object(rsu_cp::RsuSvrObjNames* a_names, rsu_cp::RsuServerAPI* api);
RSU_SERVER_API void rsu_clear_servers();