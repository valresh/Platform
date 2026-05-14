#pragma once

#include <memory>
#include "rsu_clients_ldr.h"
#include "rsu_libs.h"

namespace rsu_cp {

#define ERR_INVALID_LDR 1
#define RSU_CLIENTS_LDR_LIB "rsu_clients_ldr"
#define RSU_CLIENTS_LDR_FN "rsu_create_client"
#define RSU_CLIENTS_CLNP_FN "rsu_clients_cleanup"

	class RsuClientsLdrHelper {
		RsuLib m_lib;
	public:
		std::string ldr_error()
		{
			return m_lib.last_error();
		}
		bool ldr_state()
		{
			return m_lib.state();
		}
		int create_client(const char* a_LibName, const char* a_LibCfg, const char* a_ObjName, const char* a_LibPath, RsuClientAPI* api, bool& o_ValidLib)
		{
			if (!m_lib.state())
				return ERR_INVALID_LDR;
			t_rsu_create_client fn = (t_rsu_create_client)m_lib.api_fn();
			return fn (a_LibName, a_LibCfg, a_ObjName, a_LibPath, api, o_ValidLib);
		}
		int show_object(const char* a_LibName, const char* a_ObjName)
		{
			if (!m_lib.state())
				return ERR_INVALID_LDR;
			t_rsu_project_path fn = (t_rsu_project_path)m_lib.get_symbol("rsu_show_object");
		}
		const char* project_path()
		{
			if (m_lib.state())
			{
				t_rsu_project_path fn = (t_rsu_project_path)m_lib.get_symbol("rsu_project_path");
				if (fn)
					return fn();
			}
			return nullptr;			
		}
		void cleanup()
		{
			if (m_lib.state())
			{
				t_rsu_clients_cleanup fn = (t_rsu_clients_cleanup)m_lib.get_symbol("rsu_clients_cleanup");
				if (fn)
					fn();
			}
		}
		RsuClientsLdrHelper(): m_lib (RSU_CLIENTS_LDR_LIB, "", RSU_CLIENTS_LDR_FN, RSU_CLIENTS_CLNP_FN)  { }
	};
}
