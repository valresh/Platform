// rsu_client_test.cpp : Defines the entry point for the application.
//

#include "rsu_client_lib.h"
#include "rsu_client.h"
#include "rsu_ex.h"

namespace rsu_cp {	

	const unsigned g_obj_cnt = 4;
	const unsigned g_obj_sz = 2;
	const char* g_obj_cfg[g_obj_cnt][g_obj_sz] = { 
		{"cfg1", "a"},
		{"cfg1", "b"},
		{"cfg2", "c"},
		{"cfg2", "d"}
	};

	class RsuClientTest : public RsuClient {
	protected:
		virtual int DoInit(int ret)
		{			
			size_t rd = 0;
			return send_cmd(to_underlying(RSU_CMD::INIT), NULL, 0, NULL, rd);
		}

		virtual int DoStep0()
		{
			size_t rd = 0;
			return send_cmd(to_underlying(RSU_CMD::STEP_0), NULL, 0, NULL, rd);
		}
		
		virtual int DoStep1()
		{
			size_t rd = 0;
			return send_cmd(to_underlying(RSU_CMD::STEP_1), NULL, 0, NULL, rd);
		}
				
		virtual int DoStepT(double dt)
		{
			size_t rd = 0;			
			return send_cmd(to_underlying(RSU_CMD::STEP_T), &dt, sizeof(dt), NULL, rd);
		}
		
		virtual int DoCleanup()
		{
			size_t rd = 0;
			return send_cmd(to_underlying(RSU_CMD::CLEANUP), NULL, 0, NULL, rd);
		}
	public:
		RsuClientTest(const RsuSvrObjNames& a_names, RsuCfgLdr& cfg) : RsuClient(a_names) {}
	};

	class RsuObjCfgTest : public RsuObjCfg {
	public:
		RsuObjCfgTest(std::string a_ObjName) : RsuObjCfg{ a_ObjName } {}
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

	class RsuClientListTest : public RsuClientList {
		RsuCfgLdrTest m_Cfg;
	protected:
		virtual RsuCfgLdr& cfg()
		{
			return m_Cfg;
		}
		virtual std::unique_ptr<RsuClient> create_client(const RsuSvrObjNames& a_ObjNames)
		{
			return std::make_unique<RsuClientTest>(a_ObjNames, cfg());
		}
	public:
		RsuClientListTest (std::string a_svr_lib, std::string a_cfg_name, std::string a_svr_io, std::string a_ws, std::string a_rs, size_t a_svr_io_sz) :
			RsuClientList { a_svr_lib, a_cfg_name, a_svr_io, a_ws, a_rs, a_svr_io_sz }
		{
			m_Cfg.load_cfg(cfg_name());
		}
	};

	typedef std::shared_ptr<RsuClientListTest> PClients;
};

static std::vector<rsu_cp::PClients> g_clients;

rsu_cp::PClients get_clients (const char* a_Cfg)
{
	static const char* g_svr_lib = "rsu_server_lib";	
	std::string c = (a_Cfg ? a_Cfg:"");
	for (const rsu_cp::PClients& i : g_clients)
	{
		if (i->cfg_name() == c)
			return i;
	}
	g_clients.push_back(std::make_shared<rsu_cp::RsuClientListTest>(g_svr_lib, c,
		RSU_SVR_IO, RSU_SVR_CS, RSU_SVR_SS, RSU_SVR_IO_SZ));
	return g_clients.back();
}

RSU_CLIENT_API int rsu_create_client_object(const char* a_ObjName, const char* a_Cfg, rsu_cp::RsuClientAPI* api)
{
	int ret = to_underlying(rsu_cp::RSU_ERR::ERR);
	if (api)
	{		
		auto t = get_clients(a_Cfg);
		ret = t->rsu_create_client(a_ObjName, api);		
	}
	return ret;
}

RSU_CLIENT_API void rsu_clear_clients()
{
	g_clients.clear();
}
