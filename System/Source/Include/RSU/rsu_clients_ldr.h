#pragma once
#include <crossplatform.h>


#include <QtCore/qglobal.h>
#define _EXP Q_DECL_EXPORT
#define _IMP Q_DECL_IMPORT

#ifdef RSU_CLIENT_EXPORT
#define RSU_CLIENT_API extern "C" _EXP
#else
#define RSU_CLIENT_API extern "C" _IMP
#endif

#include "rsu_api.h"

RSU_CLIENT_API int rsu_create_client(const char* a_LibName, const char* a_LibCfg, const char* a_ObjName, const char* a_LibPath, rsu_cp::RsuClientAPI* api, bool& o_ValidLib);
RSU_CLIENT_API int rsu_show_object(const char* a_LibName, const char* a_LibCfg, const char* a_ObjName);
RSU_CLIENT_API const char* rsu_project_path();
RSU_CLIENT_API void rsu_clients_cleanup();
