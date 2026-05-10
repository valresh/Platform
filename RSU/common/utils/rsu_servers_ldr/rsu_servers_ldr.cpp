// rsu_client_test.cpp : Defines the entry point for the application.
//

#include "rsu_ex.h"
#include "rsu_servers_ldr.h"
#include <rsus/rsu_libs.h>
#include <crc32.h>
#include <thread>
#include <cstring>

namespace rsu_cp {

	#define RSU_SERVER_LIB_API_FN "rsu_create_server_object"
	#define RSU_SERVER_LIB_CLNP_FN "rsu_clear_servers"

	typedef int (*t_rsu_create_server_object)(RsuSvrObjNames* names, RsuServerAPI* api);

	class RsuServerLoader: public RsuServerExObj {
		std::string m_ApiFn = RSU_SERVER_LIB_API_FN;
		std::string m_ClnpFn = RSU_SERVER_LIB_CLNP_FN;
		RsuLibs m_Libs;
	protected:
		virtual void get_ex_cfg(RsuExchangeConfig& a_cfg)
		{			
			RsuExchangeConfig v_xcfg{ RSU_SVR_IO, RSU_SVR_IO_SZ, RSU_SVR_CS, RSU_SVR_SS, RSU_IO_TOUT, RSU_IO_TOUT };
			a_cfg = v_xcfg;
		}
	public:
		virtual void process_cmd(RsuRequestCmd* cmd)
		{
			if (cmd)
				cmd->cmd = to_underlying(RSU_ERR::ERR);
		}
		virtual bool create_svr(RsuSvrObjNames* names, RsuServerAPI* api)
		{
			if (api && m_Libs.load_lib(names->lib_name, "", m_ApiFn, m_ClnpFn))
			{
				t_rsu_create_server_object fn = (t_rsu_create_server_object)m_Libs.lib_fn(names->lib_name);
				if (fn)
					return fn(names, api) == to_underlying(RSU_ERR::SUCCESS);
			}
			return false;
		}	
		void stop_server()
		{
			stop();
			m_Libs.cleanup();			
		}
	};

	RsuServerLoader g_ldr;

	RSU_SERVER_API int rsu_start_server()
	{	
		return g_ldr.start() ? to_underlying(RSU_ERR::SUCCESS) : to_underlying(RSU_ERR::ERR);
	}

	RSU_SERVER_API void rsu_stop_server()
	{
		g_ldr.stop_server();
	}

}