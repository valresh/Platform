#include "rsu_ex.h"
#include "rsu_client_lib.h"
#include "rsu_client.h"
#include "TrendsRemoteClient.h"

namespace rsu_cp {

	class RsuTrendsRemoteClient : public RsuClient {
		KTrendsRemoteClient* m_flex_cl;
	protected:
		virtual int DoInit(int ret)
		{
			return m_flex_cl ? m_flex_cl->Init(ret) : -1;
		}

		virtual int DoStep0()
		{
			return m_flex_cl ? m_flex_cl->Step0() : -1;
		}

		virtual int DoStep1()
		{
			return m_flex_cl ? m_flex_cl->Step1() : -1;
		}

		virtual int DoStepT(double dt)
		{
			return m_flex_cl ? m_flex_cl->StepT(dt) : -1;
		}
		virtual int DoCleanup()
		{
			size_t rd = 0;
			return send_cmd(RSU_CMD::CLEANUP, NULL, 0, NULL, rd);
		}
		virtual int DoSaveState(const char* data) { return -1; }
		virtual int DoSaveParams(const char* data) { return -1; }
		virtual int DoRestoreState(const char* data) { return -1; }
		virtual int DoRestoreParams(const char* data) { return -1; }
	public:
		RsuTrendsRemoteClient(const RsuSvrObjNames& a_names) : RsuClient(a_names)
		{
			std::string n = a_names.obj_name;
			m_flex_cl = new KTrendsRemoteClient((char*)(n.c_str()), this);
		}
		~RsuTrendsRemoteClient()
		{
			delete m_flex_cl;
		}
	};

	class RsuTrendsRemoteClientList : public RsuClientList {
	protected:
		virtual std::unique_ptr<RsuClient> create_client(const RsuSvrObjNames& a_ObjNames)
		{
			return std::make_unique<RsuTrendsRemoteClient>(a_ObjNames);
		}
	public:
		RsuTrendsRemoteClientList(std::string a_svr_lib, std::string a_cfg_name, std::string a_svr_io, std::string a_ws, std::string a_rs, size_t a_svr_io_sz) :
			RsuClientList{ a_svr_lib, a_cfg_name, a_svr_io, a_ws, a_rs, a_svr_io_sz }
		{

		}
	};

	typedef std::shared_ptr<RsuTrendsRemoteClientList> PClients;
};

static std::vector<rsu_cp::PClients>* s_clients = nullptr;

rsu_cp::PClients get_clients(const char* a_Cfg)
{
	static const char* g_svr_lib = "TrendsRSU_Server";
	std::string c = (a_Cfg ? a_Cfg : "");
	assert(s_clients != nullptr);
	for (const rsu_cp::PClients& i : *s_clients)
	{
		if (i->cfg_name() == c)
			return i;
	}

	s_clients->push_back(std::make_shared<rsu_cp::RsuTrendsRemoteClientList>(g_svr_lib, c, RSU_SVR_IO, RSU_SVR_CS, RSU_SVR_SS, RSU_SVR_IO_SZ));
	return s_clients->back();
}

RSU_CLIENT_API int rsu_create_client_object(const char* a_ObjName, const char* a_Cfg, rsu_cp::RsuClientAPI* api)
{
	int ret = to_underlying(rsu_cp::RSU_ERR::ERR);
	if (api)
	{
		if (!s_clients)
		{
			s_clients = new std::vector<rsu_cp::PClients>();
		}

		auto t = get_clients(a_Cfg);
		ret = t->rsu_create_client(a_ObjName, api);
	}
	return ret;
}

RSU_CLIENT_API void rsu_clear_clients()
{
	if (s_clients)
	{
		s_clients->clear();
		delete s_clients;
		s_clients = nullptr;
	}
}
