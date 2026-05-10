#include <crossplatform.h>
#include <rsuNoNames.h>
#include "SM_Server.h"
#include <rsuMemoryManager.h>
#include <MemoryServiceSM5xx.h>
#include <ConnectSM.h>
#include <dylib.hpp>
#include "rsu_server_lib.h"
#include "rsu_server.h"

#pragma comment( lib, "ConnectSM5xx.lib")

#ifdef _MANAGED
#pragma managed(push, off)
#endif
KNoName* g_IOs;
static KRsuMemoryManager* s_memoryHW = nullptr;

#ifndef _WIN32
void OnLoadLibrarySMS5() __attribute__((constructor));
#else
HINSTANCE g_hInst = 0;
void OnLoadLibrarySMS5();
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
      OnLoadLibrarySMS5();
    g_hInst = hModule;   
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}
#endif

void OnLoadLibrarySMS5()
{
    if (!s_memoryHW)
        s_memoryHW = new KRsuMemoryManager();
    dylib* lib = new dylib("H5xxServerMemCtrl", true);
}
#ifdef _MANAGED
#pragma managed(pop)
#endif

static void* _ObjsMemAllocatorHW( size_t size )
{
  void* p = s_memoryHW->AllocMemory( (DWORD)size );
  ASS( p );
  return p;
}

static FILE *pLog = NULL;

static CBase* _IOsStructAllocatorImpl( LPCSTR pszArmName, LPCSTR pszShortSmName, DWORD TypeID, int number )
{
  if( !pLog )
  {
    //char szPath[MAX_PATH];
    auto sPath = rsu_cp::makepath_with_ext_cp(rsuGetMemoryPath(), "SM5tegs", "csv");
    //_makepath_s( szPath, NULL, rsuGetMemoryPath(), "SM5tegs", "csv" );
    fopen_s( &pLog, sPath.string().c_str(), "wt");
  }
  if( pLog && pszArmName && *pszArmName )
  {
    fprintf( pLog, "%s;%d\n", pszArmName, TypeID );
    fflush( pLog );
  }

  CBase *pObj = NULL;
  if( pszArmName && *pszArmName )
  {
    pObj = g_IOs->CreateStruct( pszArmName, TypeID, number );
    if( pszShortSmName && *pszShortSmName )
    {
      int r = g_IOs->CreateRefToData( pszShortSmName, pObj );
      KKK();
    }
  }
  else
    pObj = g_IOs->CreateStruct( pszShortSmName, TypeID, number );
  return pObj;
}
//////////////////////////////////////////////////////////////////////////
static bool s_bConstructedMemory = false;

extern "C" _EXP IRsuModel* CreateServer(LPCSTR ObjName )
{
  if( !s_bConstructedMemory )
  {
    if (!s_memoryHW)
        s_memoryHW = new KRsuMemoryManager();
    s_memoryHW->Create( "SM5xx_Mem", 8 );
    g_IOs = KNoName::Create( "SM5xx_IOs", true );
    g_IOs->Reset();
    KMemoryServiceSM5xx::Instance().InitMemAllocator( _ObjsMemAllocatorHW );
    KMemoryServiceSM5xx::Instance().InitIOsStructAllocator( _IOsStructAllocatorImpl );
    s_bConstructedMemory = true;

    if( !pRegisterSMstructs )
      dylib* lib = new dylib("SM5xxRegisterInRSU", true);
    if( pRegisterSMstructs )
      pRegisterSMstructs();
  }

  IRsuModel *pModel = new KServer( ObjName );

  return pModel;
}


namespace rsu_cp {

	const unsigned g_RSU_SVR_LIB_IO_SZ = 4096;

	class RsuServerSafetyManagerH5xx : public RsuServer {
		KServer* m_svr;
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

		RsuServerSafetyManagerH5xx(const RsuSvrObjNames* a_names) : RsuServer(a_names)
		{
			m_svr = (KServer*)CreateServer(a_names->obj_name);//new KServer(a_names->obj_name);
		}
		~RsuServerSafetyManagerH5xx()
		{
			//delete m_svr;
		}
	};

	class RsuServerListSafetyManagerH5xx : public RsuServerList {
	protected:
		virtual std::shared_ptr<RsuServer> create_server(RsuSvrObjNames* a_ObjNames)
		{
			return std::make_shared<RsuServerSafetyManagerH5xx>(a_ObjNames);
		}
	public:
		RsuServerListSafetyManagerH5xx(std::string a_svr_lib, std::string a_cfg_name) :
			RsuServerList{ a_svr_lib, a_cfg_name }
		{

		}
	};

	typedef std::shared_ptr<RsuServerListSafetyManagerH5xx> PServers;

};

static std::string g_svr_lib = "SafetyManagerH5xx_Server";
static std::vector<rsu_cp::PServers> g_servers;

rsu_cp::PServers get_servers(std::string a_cfg)
{
	for (const rsu_cp::PServers& i : g_servers)
	{
		if (i->cfg_name() == a_cfg)
			return i;
	}
	g_servers.push_back(std::make_shared<rsu_cp::RsuServerListSafetyManagerH5xx>(g_svr_lib, a_cfg));
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