// rsu_client_test.cpp : Defines the entry point for the application.
//

#include <rsus/rsu_clients_ldr.h>
#include <rsus/rsu_libs.h>
#include <rsus/rsuPaths.h>
#include "rsu_proc.h"

namespace rsu_cp {

	#define RSU_CLIENT_LIB_API_FN "rsu_create_client_object"
	#define RSU_CLIENT_LIB_CLNP_FN "rsu_clear_clients"
	//#define RSU_SVR_PROC_NAME "rsu_server_test"

	typedef int (*t_rsu_create_client_object)(const char* a_ObjName, const char* a_CfgName, RsuClientAPI* api);
#ifdef _WIN32
	typedef int (*t_rsu_show_debug_object)(const char* a_ObjName, const char* a_CfgName);
#endif

	class RsuClientsLoader {
		RsuSvrProc m_SvrProc;
		std::string m_ApiFn = RSU_CLIENT_LIB_API_FN;
		std::string m_ClnpFn = RSU_CLIENT_LIB_CLNP_FN;
		RsuLibs m_Libs;
	public:
		static RsuClientsLoader& loader()
		{
			static RsuClientsLoader ldr;
			return ldr;
		}
		void cleanup()
		{
			m_Libs.cleanup();
		}
		int create_client(const std::string a_LibName, const std::string a_LibCfg, const std::string a_ObjName, const std::string a_LibPath, RsuClientAPI* api, bool& o_ValidLib)
		{
			std::string v_svr_err = "";
			bool v_svr = m_SvrProc.create();
			if (!v_svr)
				v_svr_err = m_SvrProc.last_err_msg();
			if (api && v_svr && m_Libs.load_lib(a_LibName, a_LibPath, m_ApiFn, m_ClnpFn))
			{
				t_rsu_create_client_object fn = (t_rsu_create_client_object)m_Libs.lib_fn(a_LibName);
				o_ValidLib = fn != nullptr;
				if (fn)
					return fn(a_ObjName.c_str(), a_LibCfg.c_str(), api);
			}
			return 1;
		}
		int show_object(const std::string a_LibName, const std::string a_ShowFn, const std::string a_ObjName)
		{
#ifdef _WIN32
			std::string v_svr_err = "";
			bool v_svr = m_SvrProc.create();
			if (!v_svr)
				v_svr_err = m_SvrProc.last_err_msg();
			if (v_svr)
			{
				t_rsu_show_debug_object fn = (t_rsu_show_debug_object)m_Libs.get_symbol(a_LibName, a_ShowFn);
				if (fn)
					return fn(a_ObjName.c_str(), "");
			}
#endif
			return 1;
		}
	};

	RSU_CLIENT_API int rsu_create_client(const char* a_LibName, const char* a_LibCfg, const char* a_ObjName, const char* a_LibPath, RsuClientAPI* api, bool& o_ValidLib)
	{
		
		std::string l = a_LibName != NULL ? a_LibName : "";
		std::string c = a_LibCfg != NULL ? a_LibCfg : "";
		std::string o = a_ObjName != NULL ? a_ObjName : "";
		std::string p = a_LibPath != NULL ? a_LibPath : "";

		// to simplify debugging
		const char* cl = l.c_str();
		const char* cc = c.c_str();
		const char* co = o.c_str();
		const char* cp = p.c_str();
		auto& loader = RsuClientsLoader::loader();
		return loader.create_client(cl, cc, co, cp, api, o_ValidLib);
	}

	RSU_CLIENT_API int rsu_show_object(const char* a_LibName, const char* a_LibCfg, const char* a_ObjName)
	{
		std::string l = a_LibName != NULL ? a_LibName : "";
		std::string o = a_ObjName != NULL ? a_ObjName : "";		
		return RsuClientsLoader::loader().show_object(l.c_str(), o.c_str(), "show_object");
	}

	RSU_CLIENT_API const char* rsu_project_path()
	{		
		return  rsuGetProjectPath().c_str();
	}

	RSU_CLIENT_API void rsu_clients_cleanup()
	{
		RsuClientsLoader::loader().cleanup();
	}

}
