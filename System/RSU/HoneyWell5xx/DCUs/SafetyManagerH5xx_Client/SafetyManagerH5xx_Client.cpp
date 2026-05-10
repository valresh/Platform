#include <crossplatform.h>
#include "SM_RemoteClient.h"
#include <ConnectSM.h>
#include "rsu_client_lib.h"
#include "rsu_client.h"

#pragma comment( lib, "ConnectSM5xx.lib")

#ifdef _MANAGED
#pragma managed(push, off)
#endif
#ifdef _WIN32
#include <windows.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  return TRUE;
}
#endif

#ifdef _MANAGED
#pragma managed(pop)
#endif

static bool s_bConstructedMemory = false;
/*
extern "C" _EXP IBaseModel* CreateObject(LPCTSTR ObjName )
{
  if( !s_bConstructedMemory )
  {
    s_bConstructedMemory = true;

    if( !pRegisterSMstructs )
      LoadLibrary("SM5xxRegisterInRSU.dll");
    if( pRegisterSMstructs )
      pRegisterSMstructs();
  }

  return new KSM_RemoteClient( (char*)ObjName);
}*/

namespace rsu_cp {

	class RsuClientSafetyManagerH5xx : public RsuClient {
		KSM_RemoteClient* m_cl;

	protected:
		virtual int DoInit(int ret)
		{
			return m_cl ? m_cl->Init(ret) : -1;
		}

		virtual int DoStep0()
		{
			return m_cl ? m_cl->Step0() : -1;
		}

		virtual int DoStep1()
		{
			return m_cl ? m_cl->Step1() : -1;
		}

		virtual int DoStepT(double dt)
		{
			return m_cl ? m_cl->StepT(dt) : -1;
		}

		virtual int DoSaveState(const char* path)
		{
			return m_cl ? m_cl->SetData(sd_SaveRSUState, path) : -1;
		}

		virtual int DoSaveParams(const char* path)
		{
			return m_cl ? m_cl->SetData(sd_SaveRSUParams, path) : -1;
		}

		virtual int DoRestoreState(const char* path)
		{
			return m_cl ? m_cl->SetData(sd_RestoreRSUState, path) : -1;
		}

		virtual int DoRestoreParams(const char* path)
		{
			return m_cl ? m_cl->SetData(sd_RestoreRSUParams, path) : -1;
		}

		virtual int DoCleanup()
		{
			size_t rd = 0;
			return send_cmd(RSU_CMD::CLEANUP, NULL, 0, NULL, rd);
		}
	public:
		RsuClientSafetyManagerH5xx(const RsuSvrObjNames& a_names) : RsuClient(a_names)
		{
			std::string n = a_names.obj_name;
			if (!s_bConstructedMemory)
			{
				s_bConstructedMemory = true;

				if (!pRegisterSMstructs)
					dylib* lib = new dylib("SM5xxRegisterInRSU", true);
					//LoadLibrary("SM5xxRegisterInRSU.dll");
				if (pRegisterSMstructs)
					pRegisterSMstructs();
			}

			m_cl = new KSM_RemoteClient((char*)(n.c_str()), this);
		}
		~RsuClientSafetyManagerH5xx()
		{
			delete m_cl;
		}
	};

	class RsuClientListSafetyManagerH5xx : public RsuClientList {
	protected:
		virtual std::unique_ptr<RsuClient> create_client(const RsuSvrObjNames& a_ObjNames)
		{
			return std::make_unique<RsuClientSafetyManagerH5xx>(a_ObjNames);
		}
	public:
		RsuClientListSafetyManagerH5xx(std::string a_svr_lib, std::string a_cfg_name, std::string a_svr_io, std::string a_ws, std::string a_rs, size_t a_svr_io_sz) :
			RsuClientList{ a_svr_lib, a_cfg_name, a_svr_io, a_ws, a_rs, a_svr_io_sz }
		{

		}
	};

	typedef std::shared_ptr<RsuClientListSafetyManagerH5xx> PClients;
};

static std::vector<rsu_cp::PClients>* s_clients = nullptr;

rsu_cp::PClients get_clients(const char* a_Cfg)
{
	static const char* g_svr_lib = "SafetyManagerH5xx_Server";
	std::string c = (a_Cfg ? a_Cfg : "");
	assert(s_clients != nullptr);
	for (const rsu_cp::PClients& i : *s_clients)
	{
		if (i->cfg_name() == c)
			return i;
	}
	
	s_clients->push_back(std::make_shared<rsu_cp::RsuClientListSafetyManagerH5xx>(g_svr_lib, c,
		RSU_SVR_IO, RSU_SVR_CS, RSU_SVR_SS, RSU_SVR_IO_SZ));
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