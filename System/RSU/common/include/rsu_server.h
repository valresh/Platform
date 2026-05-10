#pragma once
#include "rsu_mio.h"
#include <rsus/rsu_api.h>
#include "rsu_cfg_ldr.h"
#include <vector>
#include <memory>
#include <cstring>
#include <crossplatform.h>
#include <dylib.hpp>
#include "rsu_ex.h"
#include "LogFile.h"

namespace rsu_cp {

	class RsuServer : public RsuServerExObj {
		RsuSvrObjNames m_ObjNames;		
	protected:
		const RsuSvrObjNames* ObjNames() { return &m_ObjNames; };
		std::string prefix()
		{
			const RsuSvrObjNames* v_names = ObjNames();
			return std::string(v_names->lib_name) + "_" + std::string(v_names->lib_cfg) + "_" + std::string(v_names->obj_name);
		}

		void print_activity(const char* cmd, int dt = 0)
		{
			CLogFile::LogEx(LOGLEVEL_HIGH, ": server %s received the command %s (dt == %d ms).\n", prefix().c_str(), cmd, dt);
		}

		virtual int DoInitialize() = 0;
		virtual int DoAfterInit() = 0;
		virtual int DoAfterRestored() = 0;
		virtual int DoCalcStep(int dtMs) = 0;
		virtual int DoStateSave(const char* pszPath) = 0;
		virtual int DoStateRestore(const char* pszPath) = 0;
		virtual int DoParamsSave(const char* pszPath) = 0;
		virtual int DoParamsRestore(const char* pszPath) = 0;
	public:
		int Initialize()
		{
			return DoInitialize();
		}

		int AfterInit()
		{
			return DoAfterInit();
		}

		int AfterRestored()
		{
			return DoAfterRestored();
		}

		int CalcStep(int dtMs)
		{
			return DoCalcStep(dtMs);
		}

		int StateSave(const char* pszPath)
		{
			return DoStateSave(pszPath);
		}

		int StateRestore(const char* pszPath)
		{
			return DoStateRestore(pszPath);
		}

		int ParamsSave(const char* pszPath)
		{
			return DoParamsSave(pszPath);
		}

		int ParamsRestore(const char* pszPath)
		{
			return DoParamsRestore(pszPath);
		}

		void Bind(RsuServerAPI* api)
		{
			if (api)
			{
				api->setInitialize(std::bind(&RsuServer::Initialize, this));
				api->setAfterInit(std::bind(&RsuServer::AfterInit, this));
				api->setAfterRestored(std::bind(&RsuServer::AfterRestored, this));
				api->setCalcStep(std::bind(&RsuServer::CalcStep, this, std::placeholders::_1));
				api->setStateSave(std::bind(&RsuServer::StateSave, this, std::placeholders::_1));
				api->setStateRestore(std::bind(&RsuServer::StateRestore, this, std::placeholders::_1));
				api->setParamsSave(std::bind(&RsuServer::ParamsSave, this, std::placeholders::_1));
				api->setParamsRestore(std::bind(&RsuServer::ParamsRestore, this, std::placeholders::_1));
				api->setState(std::bind(&RsuServer::state, this));
			}
		}
		bool same_names(RsuSvrObjNames* a_names, bool fill_if_same)
		{
			bool ret = a_names
				&& strcmp(m_ObjNames.lib_name, a_names->lib_name) == 0
				&& strcmp(m_ObjNames.lib_cfg, a_names->lib_cfg) == 0
				&& strcmp(m_ObjNames.obj_name, a_names->obj_name) == 0;
			if (ret && fill_if_same)
				fill_names(a_names);
			return ret;
		}

		virtual bool create_svr(RsuSvrObjNames* names, RsuServerAPI* api) 
		{ 
			return false; 
		};

		RsuServer(const RsuSvrObjNames* a_names):
			m_ObjNames (*a_names)
		{

		}
	};

	class RsuServerProxy {
		RsuServer* m_svr;
	public:
		virtual int Initialize() { return m_svr ? m_svr->Initialize() : -1; }
		virtual int AfterInit() { return m_svr ? m_svr->AfterInit() : -1; }
		virtual int AfterRestored() { return m_svr ? m_svr->AfterRestored() : -1; }
		virtual int CalcStep(int dtMs) { return m_svr ? m_svr->CalcStep(dtMs) : -1; }
		virtual int StateSave(LPCSTR pszPath) { return m_svr ? m_svr->StateSave(pszPath) : -1; }
		virtual int StateRestore(LPCSTR pszPath) { return m_svr ? m_svr->StateRestore(pszPath) : -1; }
		virtual int ParamsSave(LPCSTR pszPath) { return m_svr ? m_svr->ParamsSave(pszPath) : -1; }
		virtual int ParamsRestore(LPCSTR pszPath) { return m_svr ? m_svr->ParamsRestore(pszPath) : -1; }
		virtual void ShowObject(LPCSTR pszName) { return; }

		void reset_server(RsuServer* a_svr) { m_svr = a_svr; }
		RsuServerProxy(RsuServer* a_svr) : m_svr(a_svr) {}
	};

	class RsuServerList {
		std::string m_svr_lib = "";
		std::string m_cfg_name = "";
		std::vector <std::shared_ptr<RsuServer>> m_servers;
	protected:
		virtual std::shared_ptr<RsuServer> create_server(RsuSvrObjNames *a_ObjNames) = 0;
	public:
		const std::string svr_lib()
		{
			return m_svr_lib;
		}

		std::string cfg_name()
		{
			return m_cfg_name;
		}

		std::shared_ptr<RsuServer>& get_server(RsuSvrObjNames* a_names)
		{
			for ( auto& i : m_servers)
			{
				if (i->same_names(a_names, true))
					return i;								
			}
			m_servers.push_back( create_server(a_names));
			return m_servers.back();
		}

		int rsu_create_server(RsuSvrObjNames* a_names, RsuServerAPI* api)
		{
			int ret = to_underlying(RSU_ERR::ERR);			
			if (a_names && api )
			{
				std::string v_lib_name = a_names->lib_name;
				std::string v_lib_cfg = a_names->lib_cfg;
				if (m_svr_lib == v_lib_name)
				{
					auto svr = get_server(a_names); //create_server(a_names);
					svr->Bind(api);
				
					if (svr->start())
					{							
						ret = to_underlying(RSU_ERR::SUCCESS);
						api->SetCreationRes(ret, "");
						svr->fill_names(a_names);							
					}
					else
					{
						std::string err = "Server object exchange initialization error. " + svr->ErrorStr();
						api->SetCreationRes(ret, err.c_str());
						svr->fill_names(a_names);
					}											
					//m_servers.push_back(std::move(svr));
				}				
			}
			else
			{
				if (api)
					api->SetCreationRes(ret, "Object names is null.");
			}

			return ret;
		}

		RsuServerList(std::string a_svr_lib, std::string a_cfg_name) :
			m_svr_lib{ a_svr_lib }, m_cfg_name { a_cfg_name }
		{

		}
	};

}