#include <crossplatform.h>
#include <rsuNoNames.h>
#include <rsuMemoryManager.h>
#include "QB_Server.h"
#include <rsuPaths.h>
#include <rsuErr.h>
#include <dylib.hpp>
#include "rsu_server_lib.h"
#include "rsu_server.h"

static KRsuMemoryManager *s_memory = nullptr;

static void* _ObjsMemAllocator(size_t size)
{    
    BYTE* p = s_memory->AllocMemory((DWORD)size);
    ASSD(p);
    if (!p)
    {
        p = new BYTE[size];
        ZeroMemory(p, size);
    }
    return p;
}

#ifdef _MANAGED
#pragma managed(push, off)
#endif

#ifndef _WIN32
void OnLoadLibraryQBS5() __attribute__((constructor));
#else
void OnLoadLibraryQBS5();

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
    OnLoadLibraryQBS5();
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

#endif
#ifdef _MANAGED
#pragma managed(pop)
#endif

void OnLoadLibraryQBS5()
{
    if (!s_memory)
        s_memory = new KRsuMemoryManager();
    dylib* lib = new dylib("H5xxServerMemCtrl", true);
}

KNoName *g_IOs;
static FILE *pLog = NULL;
static CBase* _IOsStructAllocatorImpl( LPCSTR ObjName, DWORD TypeID, int number )
{
  /*if( !pLog )
  {
    char szPath[MAX_PATH];
    _makepath_s( szPath, NULL, rsuGetMemoryPath(), "QBtegs", "csv" );
    fopen_s( &pLog, szPath, "wt" );
  }
  if( pLog )
  {
    fprintf_s( pLog, "%s;%d\n", ObjName, TypeID );
    fflush( pLog );
  }*/

  return g_IOs->CreateStruct( ObjName, TypeID, number );
}

static CBase* _FindIOPnt( LPCSTR ObjName, DWORD TypeID, int number, LPCSTR* pszEntry )
{
  CBase* p = g_IOs->FindStruct( ObjName, TypeID, number );
  if( !p )
    return NULL;
  g_IOs->GetNameEx( p->ID_PNT_NAME, p->ID_CLASS, NULL, pszEntry );
  return p;
}

static void RefCreatorImpl( LPCSTR ObjName, CBase *pSrc, int number )
{
  g_IOs->CreateRefToData( ObjName, pSrc, number );
}
//////////////////////////////////////////////////////////////////////////
static bool s_bConstructedMemory = false;
extern "C" _EXP IRsuModel* CreateServer(LPCSTR ObjName )
{
  if( !s_bConstructedMemory )
  {
    size_t ms = s_memory->Create( "QB5_Mem", 80 );

    KMemoryServiceQB5xx::Instance().InitMemAllocator( _ObjsMemAllocator );

    g_IOs = KNoName::Create( "QB5xx_IOs", true, 16000, 220000, 320*1024*1024 );
    KMemoryServiceQB5xx::Instance().InitIOsStructAllocator( _IOsStructAllocatorImpl, _FindIOPnt, RefCreatorImpl );
    s_bConstructedMemory = true;
    g_IOs->Reset();
  }

  IRsuModel *pModel = new KServer( ObjName );

  return pModel;
}


namespace rsu_cp {

	const unsigned g_RSU_SVR_LIB_IO_SZ = 4096;

	class RsuServerQuickBuilderH5xx : public RsuServer {
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

		RsuServerQuickBuilderH5xx(const RsuSvrObjNames* a_names) : RsuServer(a_names)
		{
			m_svr = (KServer*)CreateServer(a_names->obj_name);//new KServer(a_names->obj_name);
		}
		~RsuServerQuickBuilderH5xx()
		{
			//delete m_svr;
		}
	};

	class RsuServerListQuickBuilderH5xx : public RsuServerList {
	protected:
		virtual std::shared_ptr<RsuServer> create_server(RsuSvrObjNames* a_ObjNames)
		{
			return std::make_shared<RsuServerQuickBuilderH5xx>(a_ObjNames);
		}
	public:
		RsuServerListQuickBuilderH5xx(std::string a_svr_lib, std::string a_cfg_name) :
			RsuServerList{ a_svr_lib, a_cfg_name }
		{

		}
	};

	typedef std::shared_ptr<RsuServerListQuickBuilderH5xx> PServers;

};

static std::string g_svr_lib = "QuickBuilderH5xx_Server";
static std::vector<rsu_cp::PServers> g_servers;

rsu_cp::PServers get_servers(std::string a_cfg)
{
	for (const rsu_cp::PServers& i : g_servers)
	{
		if (i->cfg_name() == a_cfg)
			return i;
	}
	g_servers.push_back(std::make_shared<rsu_cp::RsuServerListQuickBuilderH5xx>(g_svr_lib, a_cfg));
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