#pragma once

#if defined(_WIN32)
#ifdef RSU_SERVER_EXPORT
#define RSU_SERVER_API extern "C" __declspec(dllexport)
#else
#define RSU_SERVER_API extern "C" __declspec(dllimport)
#endif
#else
#ifdef RSU_SERVER_EXPORT
#   define RSU_SERVER_API extern "C" __attribute__((visibility("default")))
#else
#   define RSU_SERVER_API extern "C"
#endif
#endif

#include <rsus/rsu_api.h>

extern "C" {
	RSU_SERVER_API int rsu_start_server();
	RSU_SERVER_API void rsu_stop_server();
} // extern "C"