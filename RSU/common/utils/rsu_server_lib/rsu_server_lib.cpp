// rsu_client_test.cpp : Defines the entry point for the application.
//

#include "rsu_server_lib.h"
#include "rsu_server.h"
#include "rsu_ex.h"
#include "rsu_cfg_ldr.h"

namespace rsu_cp {

	const unsigned g_obj_cnt = 4;
	const unsigned g_obj_sz = 2;
	const char* g_obj_cfg[g_obj_cnt][g_obj_sz] = {
		{"cfg1", "a"},
		{"cfg1", "b"},
		{"cfg2", "c"},
		{"cfg2", "d"}
	};

	const unsigned g_RSU_SVR_LIB_IO_SZ = 4096;

	class RsuServerTest : public RsuServer {		
	protected:
		virtual void get_ex_cfg(RsuExchangeConfig& a_cfg)
		{		
			std::string v_pref = prefix();
			a_cfg = RsuExchangeConfig{ v_pref + "_io", g_RSU_SVR_LIB_IO_SZ, v_pref + "_cs", v_pref + "_ss", RSU_IO_TOUT, RSU_IO_TOUT};
		}

		virtual int DoInitialize()
		{
			print_activity("Initialize");
			return to_underlying(RSU_ERR::SUCCESS);
		}

		virtual int DoAfterInit()
		{
			print_activity("AfterInit");
			return to_underlying(RSU_ERR::SUCCESS);
		}

		virtual int DoAfterRestored()
		{
			print_activity("AfterRestored");
			return to_underlying(RSU_ERR::SUCCESS);
		}

		virtual int DoCalcStep(int dtMs)
		{
			print_activity("CalcStep", dtMs);
			return to_underlying(RSU_ERR::SUCCESS);
		}

		virtual int DoStateSave(std::string pszPath)
		{
			print_activity("StateSave");
			return to_underlying(RSU_ERR::SUCCESS);
		}

		virtual int DoStateRestore(std::string pszPath)
		{
			print_activity("StateRestore");
			return to_underlying(RSU_ERR::SUCCESS);
		}

		virtual int DoParamsSave(std::string pszPath)
		{
			print_activity("ParamsSave");
			return to_underlying(RSU_ERR::SUCCESS);
		}

		virtual int DoParamsRestore(std::string pszPath)
		{
			print_activity("ParamsRestore");
			return to_underlying(RSU_ERR::SUCCESS);
		}
	public:
		virtual void process_cmd(RsuRequestCmd* cmd)
		{
			int ret = to_underlying(RSU_ERR::ERR);
			if (cmd)
			{
				switch ( cmd->cmd )
				{
				case to_underlying(RSU_CMD::INIT):	
					{
						ret = DoInitialize();
						break;
					}
					case to_underlying(RSU_CMD::STEP_0):
					{
						ret = DoAfterInit();
						break;
					}
					case to_underlying(RSU_CMD::STEP_1):
					case to_underlying(RSU_CMD::STEP_T):
					{
						double dt = 0;
						if (cmd->size == sizeof(dt))
						{
							dt = *(double*)cmd->data;
							ret = DoCalcStep((int)(dt * 1000));
							break;
						}
					} 
				}
				cmd->cmd = ret;
				cmd->data = NULL;
				cmd->size = 0;
			}
		}

		RsuServerTest(const RsuSvrObjNames* a_names) : RsuServer(a_names) {}
	};

	class RsuObjCfgTest : public RsuObjCfg {
	public:
		RsuObjCfgTest(const char* a_ObjName) : RsuObjCfg{ a_ObjName } {}
	};

	class RsuCfgLdrTest : public RsuCfgLdr {
	protected:
		virtual void load() {
			for (unsigned i = 0; i < g_obj_cnt; ++i)
			{
				std::string v_cfg = g_obj_cfg[i][0];
				if (v_cfg == cfg_name())
					configs().push_back(std::make_shared<RsuObjCfgTest>(g_obj_cfg[i][1]));
			}
		}
	};
	
	class RsuServerListTest : public RsuServerList {
		RsuCfgLdrTest m_Cfg;
	protected:
		virtual RsuCfgLdr& cfg()
		{
			return m_Cfg;
		}
		virtual std::shared_ptr<RsuServer> create_server(RsuSvrObjNames* a_ObjNames)
		{
			return std::make_shared<RsuServerTest>(a_ObjNames);
		}		
	public:
		RsuServerListTest(std::string a_svr_lib, std::string a_cfg_name) :
			RsuServerList{ a_svr_lib, a_cfg_name }
		{
			m_Cfg.load_cfg(cfg_name());
		}
	};

	typedef std::shared_ptr<RsuServerListTest> PServers;
	
};

static std::string g_svr_lib = "rsu_server_lib";
static std::vector<rsu_cp::PServers> g_servers;

rsu_cp::PServers get_servers(std::string a_cfg)
{	
	for (const rsu_cp::PServers& i : g_servers)
	{
		if (i->cfg_name() == a_cfg)
			return i;
	}
	g_servers.push_back(std::make_shared<rsu_cp::RsuServerListTest>(g_svr_lib, a_cfg));
	return g_servers.back();
}

RSU_SERVER_API int rsu_create_server_object(rsu_cp::RsuSvrObjNames* a_names, rsu_cp::RsuServerAPI* api)
{
	int ret = to_underlying(rsu_cp::RSU_ERR::ERR);
	if (api && a_names)
	{
		std::string l = char2str(a_names->lib_name);
		std::string c = char2str(a_names->lib_cfg);
		if (l == g_svr_lib)
		{
			auto t = get_servers(c);
			ret = t->rsu_create_server(a_names, api);
		}
	}
	return ret;
}

RSU_SERVER_API void rsu_clear_servers()
{
	g_servers.clear();
}