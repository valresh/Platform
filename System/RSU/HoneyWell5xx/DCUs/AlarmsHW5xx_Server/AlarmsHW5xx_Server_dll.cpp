#include "AlarmServer.h"
#include <crossplatform.h>
#include "rsu_server_lib.h"
#include "rsu_server.h"

#ifdef _WIN32

#ifdef _MANAGED
#pragma managed(push, off)
#endif

HINSTANCE g_hInst = 0;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    g_hInst = hModule;
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#endif
extern "C" _EXP IRsuModel* CreateServer(LPCSTR ObjName )
{
  IRsuModel *pModel = new KAlarmServer( ObjName );

  return pModel;
}


namespace rsu_cp {

	const unsigned g_RSU_SVR_LIB_IO_SZ = 4096;

	class RsuServerAlarmsHW5xx : public RsuServer {
		KAlarmServer* m_svr;
	protected:
		virtual void get_ex_cfg(RsuExchangeConfig& a_cfg)
		{
			std::string v_pref = prefix();
			a_cfg = RsuExchangeConfig{ v_pref + "_io", g_RSU_SVR_LIB_IO_SZ, v_pref + "_cs", v_pref + "_ss", RSU_IO_TOUT, RSU_IO_TOUT };
		}

		virtual int DoInitialize()
		{
			print_activity("Initialize");
			return m_svr->DoInitialize();
		}

		virtual int DoAfterInit()
		{
			print_activity("AfterInit");
			return m_svr->DoAfterInit();
		}

		virtual int DoAfterRestored()
		{
			print_activity("AfterRestored");
			return m_svr->DoAfterRestored();
		}

		virtual int DoCalcStep(int dtMs)
		{
			print_activity("CalcStep", dtMs);
			return m_svr->DoCalcStep(dtMs);
		}

		virtual int DoStateSave(const char* pszPath)
		{
			print_activity("StateSave");
			return m_svr->DoStateSave(pszPath);
		}

		virtual int DoStateRestore(const char* pszPath)
		{
			print_activity("StateRestore");
			return m_svr->DoStateRestore(pszPath);
		}

		virtual int DoParamsSave(const char* pszPath)
		{
			print_activity("ParamsSave");
			return m_svr->DoParamsSave(pszPath);
		}

		virtual int DoParamsRestore(const char* pszPath)
		{
			print_activity("ParamsRestore");
			return m_svr->DoParamsRestore(pszPath);
		}

		virtual int DoShowObject(const char* pszPath)
		{
			print_activity("ShowObject");
			return m_svr->DoShowObject(pszPath), 1;
		}
	public:
		virtual void process_cmd(RsuRequestCmd* cmd)
		{
			int ret = to_underlying(RSU_ERR::ERR);
			if (cmd)
			{
				switch (cmd->cmd)
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
					ret = DoAfterRestored();
					break;
				case to_underlying(RSU_CMD::STEP_T):
				{
					double dt = 0;
					if (cmd->size == sizeof(dt))
					{
						dt = *(double*)cmd->data;
						double dtMs = dt * 3600. * 1000;
						ret = DoCalcStep((int)(dtMs));
						break;
					}
					break;
				}
				case to_underlying(RSU_CMD::STATE_SAVE):
				{
					ret = DoStateSave((const char*)cmd->data);
					break;
				}
				case to_underlying(RSU_CMD::STATE_RESTORE):
				{
					ret = DoStateRestore((const char*)cmd->data);
					break;
				}
				case to_underlying(RSU_CMD::PARAMS_SAVE):
				{
					ret = DoParamsSave((const char*)cmd->data);
					break;
				}
				case to_underlying(RSU_CMD::PARAMS_RESTORE):
				{
					ret = DoParamsRestore((const char*)cmd->data);
					break;
				}
				case to_underlying(RSU_CMD::SHOW_OBJECT):
				{
					ret = DoShowObject((const char*)cmd->data);
					break;
				}
				}
				cmd->cmd = ret;
				cmd->data = NULL;
				cmd->size = 0;
			}
		}

		RsuServerAlarmsHW5xx(const RsuSvrObjNames* a_names) : RsuServer(a_names)
		{
			m_svr = (KAlarmServer*)CreateServer(a_names->obj_name);//new KServer(a_names->obj_name);
		}
		~RsuServerAlarmsHW5xx()
		{
			//delete m_svr;
		}
	};

	class RsuServerListAlarmsHW5xx : public RsuServerList {
	protected:
		virtual std::shared_ptr<RsuServer> create_server(RsuSvrObjNames* a_ObjNames)
		{
			return std::make_shared<RsuServerAlarmsHW5xx>(a_ObjNames);
		}
	public:
		RsuServerListAlarmsHW5xx(std::string a_svr_lib, std::string a_cfg_name) :
			RsuServerList{ a_svr_lib, a_cfg_name }
		{

		}
	};

	typedef std::shared_ptr<RsuServerListAlarmsHW5xx> PServers;

};

static std::string g_svr_lib = "AlarmsHW5xx_Server";
static std::vector<rsu_cp::PServers> g_servers;

rsu_cp::PServers get_servers(std::string a_cfg)
{
	for (const rsu_cp::PServers& i : g_servers)
	{
		if (i->cfg_name() == a_cfg)
			return i;
	}
	g_servers.push_back(std::make_shared<rsu_cp::RsuServerListAlarmsHW5xx>(g_svr_lib, a_cfg));
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