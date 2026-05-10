#pragma once

#include <memory>
#include "rsu_servers_ldr.h"
#include <rsus/rsu_libs.h>

namespace rsu_cp {

#define ERR_INVALID_LDR 1
#define RSU_SERVER_LDR_LIB "rsu_servers_ldr"
#define RSU_SERVER_START_FN "rsu_start_server"
#define RSU_SERVER_STOP_FN "rsu_stop_server"

	typedef int (*t_rsu_server_start_fn)();
	typedef void (*t_rsu_server_stop_fn)();

	class RsuServerLdrHelper {
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
		int start ()
		{
			if (!m_lib.state())
				return ERR_INVALID_LDR;
			t_rsu_server_start_fn fn = (t_rsu_server_start_fn) m_lib.api_fn();
			return fn();
		}
		void stop()
		{
			if (m_lib.state())
			{
				t_rsu_server_stop_fn fn = (t_rsu_server_stop_fn)m_lib.get_symbol(RSU_SERVER_STOP_FN);
				if (fn)
					fn();
			}			
		}
		RsuServerLdrHelper() : m_lib(RSU_SERVER_LDR_LIB, "", RSU_SERVER_START_FN, "") { }
	};
}
