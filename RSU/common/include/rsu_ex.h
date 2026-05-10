#pragma once
#include <cassert>
#include <iostream>
#include "rsu_mio.h"
#include "rsu_time.h"
#include "rsu_err.h"
#include <rsus/rsu_api.h>
#include <stdio.h>
#include <crc32.h>
#include <thread>
#include <cstring>
#include <string>
#include "crossplatform.h"
#include "crosssemaphore.h"
#include "LogFile.h"

namespace rsu_cp {

#define RSU_IO_SZ 4096
#define RSU_IO_TOUT 300

#pragma pack(push,1)
	struct RsuExchangeHdr
	{
		int32_t cmd;
		int64_t size;
		uint32_t crc_32;
		RsuExchangeHdr(int32_t a_cmd, int64_t a_size, int32_t a_crc_32) :
			cmd{ a_cmd }, size{ a_size }, crc_32(a_crc_32)
		{}
	};

	struct RsuSvrObjNames {
		char lib_name[512];
		char lib_cfg[512];
		char obj_name[512];
		char io_name[256];
		char SS_name[128];
		char CS_name[128];
		size_t io_sz = 0;
		unsigned int cs_tout = 0;
		unsigned int ss_tout = 0;
		RsuSvrObjNames(const std::string a_lib_name, const std::string a_lib_cfg, const std::string a_obj_name)
		{
			std::memset(lib_name, 0, sizeof(lib_name));
			std::memset(lib_cfg, 0, sizeof(lib_cfg));
			std::memset(obj_name, 0, sizeof(obj_name));
			std::memset(io_name, 0, sizeof(io_name));
			std::memset(SS_name, 0, sizeof(SS_name));
			std::memset(CS_name, 0, sizeof(CS_name));
			std::memcpy(lib_name, a_lib_name.c_str(), a_lib_name.size() < sizeof(lib_name) ? a_lib_name.size() : sizeof(lib_name) - 1);
			std::memcpy(lib_cfg, a_lib_cfg.c_str(), a_lib_cfg.size() < sizeof(lib_cfg) ? a_lib_cfg.size() : sizeof(lib_cfg) - 1);
			std::memcpy(obj_name, a_obj_name.c_str(), a_obj_name.size() < sizeof(obj_name) ? a_obj_name.size() : sizeof(obj_name) - 1);
		}
	};
#pragma pack(pop)

	struct RsuStdObjNames {
		std::string lib = "";
		std::string cfg = "";
		std::string obj = "";
		std::string io = "";
		std::string ss = "";
		std::string cs = "";
		size_t io_sz = 0;
		unsigned int cs_t = 0;
		unsigned int ss_t = 0;
		RsuStdObjNames(const RsuSvrObjNames& n) :
			lib{ n.lib_name }, cfg{ n.lib_cfg }, obj{ n.obj_name }, io{ n.io_name }, ss{ n.SS_name }, cs{ n.CS_name },
			io_sz{ n.io_sz }, cs_t{ n.cs_tout }, ss_t{ n.ss_tout }
		{}

	};

	enum class RSU_CMD : int32_t {
		GET_OBJECT = 1, INIT, STEP_0, STEP_1, STEP_T, STATE_SAVE, STATE_RESTORE, PARAMS_SAVE, PARAMS_RESTORE, SHOW_OBJECT, CLEANUP
	};

	struct RsuRequestCmd {
		int32_t cmd;
		const void* data;
		int64_t size;
		RsuRequestCmd() : cmd{ 0 }, data{ NULL }, size(0) {}
		RsuRequestCmd(RSU_CMD a_cmd, const void* a_data, int64_t a_size) : cmd{ static_cast<int32_t>(a_cmd) }, data{ a_data }, size(a_size) {}
	};

	enum class RSU_ERR : int32_t {
		SUCCESS = 0, ERR = -1
	};

	template <typename E>
	constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
		return static_cast<typename std::underlying_type<E>::type>(e);
	}

	struct RsuExchangeConfig {
		std::string io_name = "";
		size_t io_sz = RSU_IO_SZ;
		std::string cs_name = "";
		unsigned int cs_tout = RSU_IO_TOUT;
		std::string ss_name = "";
		unsigned int ss_tout = RSU_IO_TOUT;
		RsuExchangeConfig(std::string a_io_name, size_t a_io_sz, std::string a_cs_name, std::string a_ss_name, unsigned int a_cs_tout, unsigned int a_ss_tout) :
			io_name{ a_io_name }, io_sz{ a_io_sz }, cs_name{ a_cs_name }, ss_name{ a_ss_name }, cs_tout{ a_cs_tout }, ss_tout{ a_ss_tout } {}
		RsuExchangeConfig() {};
	};

	class BaseExchangeClass
	{
	private:
		std::unique_ptr<cross::counting_semaphore> m_CS;
		std::unique_ptr<cross::counting_semaphore> m_SS;

	protected:
		BaseExchangeClass()
		{
		}

		bool hold_SS()
		{
			assert(m_SS);
			return acquire_semaphore(m_SS);
		}

		bool release_CS()
		{
			assert(m_CS);
			return release_semaphore(m_CS);
		}

		bool hold_CS()
		{
			assert(m_CS);
			return acquire_semaphore(m_CS);
		}

		bool release_SS()
		{
			assert(m_SS);
			return release_semaphore(m_SS);
		}

		bool open_cs(const std::string& semaphoreName)
		{
			return open_or_create_semaphore(semaphoreName, m_CS);
		}

		bool open_ss(const std::string& semaphoreName)
		{
			return open_or_create_semaphore(semaphoreName, m_SS);
		}

		bool state()
		{
			return m_CS && m_SS;
		}
		
		unsigned bin_state()
		{
			return ((bool)m_CS << 1) | ((bool)m_SS << 2);
		}

		std::string ErrorStr() 
		{
			std::string ret = "";
			if (!state())
			{
				if (!m_CS)
					ret += "CS error: No semaphore;";
				if (!m_SS)
					ret += "SS error: No semaphore;";
			}

			return ret;
		}

		void clear()
		{
			m_CS = nullptr;
			m_SS = nullptr;
		}

	private:
		bool acquire_semaphore(std::unique_ptr<cross::counting_semaphore> & semaphore)
		{
			if (!semaphore)
				throw std::runtime_error("Semaphore hasn't been created");

			try
			{
				semaphore->acquire();
				return true;
			}
			catch (const std::system_error& e) 
			{
				CLogFile::LogEx(LOGLEVEL_URGENT, "Impossible to acquire() semaphore, ignore");
				return false;
			}
		}

		bool release_semaphore(std::unique_ptr<cross::counting_semaphore>& semaphore)
		{
			try
			{
				semaphore->release();
			}
			catch (const std::system_error& e)
			{
				// Ignore all on release
				CLogFile::LogEx(LOGLEVEL_URGENT, "Impossible to release() semaphore, ignore");
			}

			return true;
		}

		bool open_or_create_semaphore(const std::string& semaphoreName, std::unique_ptr<cross::counting_semaphore> & semaphore)
		{
			if (semaphoreName.empty())
			{
				return false;
			}

			try
			{
				semaphore = std::make_unique<cross::counting_semaphore>(semaphoreName.c_str(), 0, INT_MAX);
				return true;
			}
			catch (const std::system_error& e)
			{
				CLogFile::LogEx(LOGLEVEL_URGENT, "Impossible to create/open semaphore '%s'", semaphoreName);
				return false;
			}
		}
	};

	class RsuExchangeServer : public BaseExchangeClass
	{
		RsuExchangeConfig m_config;
		bool m_quick_res = false;
		bool m_active = true;
		RsuMio m_io;		
		void* m_data = NULL;

		void loop()
		{
			if (before_loop())
			{
				while (m_active)
					if (!porcess_cmd())
						break;
			}
			after_loop();
		}
		static void svr_loop(RsuExchangeServer* c)
		{
			c->loop();
		}
		void write_res(RsuRequestCmd* cmd)
		{
			//printf("%s: server write res!\n", current_time_str().c_str());
			uint32_t v_crc_32 = cmd->size ? crc32_fast(cmd->data, cmd->size) : 0;
			RsuExchangeHdr hdr(cmd->cmd, cmd->size, v_crc_32);
			memcpy(m_io.data(), &hdr, sizeof(hdr));
			memcpy((uint8_t*)m_io.data() + sizeof(hdr), cmd->data, cmd->size);
			release_CS();
		}
		bool read_cmd(RsuRequestCmd* cmd)
		{
			bool ret = false;			
			if (hold_SS())
			{				
				RsuExchangeHdr* h = (RsuExchangeHdr*)m_io.data();
				uint32_t v_crc_32 = h->size ? crc32_fast((uint8_t*)m_io.data() + sizeof(*h), h->size) : 0;
				if (v_crc_32 == h->crc_32)
				{
					cmd->cmd = h->cmd;
					cmd->size = h->size;
					cmd->data = (uint8_t*)m_io.data() + sizeof(*h);
					ret = true;
				}
			}
			return ret;
		}
	protected:
		virtual bool before_loop() { return true; }
		virtual bool do_process(RsuRequestCmd* cmd) = 0;
		virtual void after_loop() { }
		bool porcess_cmd()
		{
			bool ret = true;
			RsuRequestCmd cmd;
			if (read_cmd(&cmd))
			{
				if (m_quick_res)
					write_res(&cmd);
				ret = do_process(&cmd);
				if (!m_quick_res)
					write_res(&cmd);			
			}
			return ret;
		}
	public:		
		const RsuExchangeConfig* config()
		{
			return &m_config;
		}
		void stop() { m_active = false; }
		bool state()
		{
			return m_io.state() && BaseExchangeClass::state();
		}
		unsigned bin_state()
		{
			return (unsigned) m_io.state() | BaseExchangeClass::bin_state();
		}

		std::string ErrorStr() {
			std::string ret = "";
			if (!state())
			{
				if (!m_io.state())
				{
					ret = "MIO error: " + GetErrorString(m_io.last_error().value()) + ";";
				}
				
				ret += BaseExchangeClass::ErrorStr();
			}

			return ret;
		}
		RsuExchangeServer(const RsuExchangeConfig& a_config, bool a_quick_res = false)
		{
			m_config = a_config;
			open_cs(m_config.cs_name);
			open_ss(m_config.ss_name);
			m_io.open(m_config.io_name.c_str(), m_config.io_sz);
			if (state())
			{
				m_active = true;
				std::thread rsu_svr(svr_loop, this);
				rsu_svr.detach();
			}			
		}
		~RsuExchangeServer()
		{
			m_active = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(m_config.ss_tout * 3));
		}

	};

	class RsuServerObj {
	public:
		virtual bool create_svr(RsuSvrObjNames* names, RsuServerAPI *api) = 0;
		virtual void process_cmd(RsuRequestCmd* cmd) = 0;
	};

	class RsuSvrExchangeServer : public RsuExchangeServer {
		RsuServerObj* m_svr;
	protected:
		virtual bool do_process(RsuRequestCmd* cmd)
		{
			bool ret = m_svr && cmd;
			if (ret)
			{
				if (cmd->cmd == to_underlying(RSU_CMD::GET_OBJECT))
				{
					cmd->cmd = to_underlying(RSU_ERR::ERR);
					if (cmd->size == sizeof(RsuSvrObjNames))
					{
						RsuSvrObjNames* n = (RsuSvrObjNames*)cmd->data;
						RsuServerAPI api;
						if (m_svr->create_svr(n, &api))
							cmd->cmd = to_underlying(RSU_ERR::SUCCESS);
					}					
				}
				else
					m_svr->process_cmd(cmd);				
			}
			return ret;
		}
	public:
		RsuSvrExchangeServer(RsuServerObj* a_svr, const RsuExchangeConfig& ex_cfg) :
			m_svr{ a_svr }, RsuExchangeServer(ex_cfg) {}
	};

	class RsuExchangeClient : public BaseExchangeClass
	{
		RsuExchangeConfig m_config;
		RsuMio m_io;
		void* m_data = NULL;

		bool write_cmd(RsuRequestCmd* cmd)
		{
			uint32_t v_crc_32 = cmd->size ? crc32_fast(cmd->data, cmd->size) : 0;
			RsuExchangeHdr hdr(cmd->cmd, cmd->size, v_crc_32);
			memcpy(m_io.data(), &hdr, sizeof(hdr));
			memcpy((uint8_t*)m_io.data() + sizeof(hdr), cmd->data, cmd->size);
			return release_SS();
		}

		bool read_res(RsuRequestCmd* cmd)
		{
			bool ret = hold_CS();
			if (ret)
			{
				RsuExchangeHdr* h = (RsuExchangeHdr*)m_io.data();
				uint32_t v_crc_32 = h->size ? crc32_fast((uint8_t*)m_io.data() + sizeof(*h), h->size) : 0;
				if (v_crc_32 == h->crc_32)
				{
					cmd->cmd = h->cmd;
					cmd->size = h->size;
					cmd->data = (uint8_t*)m_io.data() + sizeof(*h);
					ret = true;
				}
			}
			return ret;
		}

		void clear()
		{
			m_io.close();
			BaseExchangeClass::clear();
		}

		bool open_io()
		{
			bool ret = false;
			if (m_config.io_name != "")
			{
				ret = m_io.open(m_config.io_name.c_str(), m_config.io_sz);
			}
			
			return ret;
		}
	public:
		bool send_cmd(RsuRequestCmd* cmd)
		{
			bool ret = false;
			if (cmd && state() && (cmd->size + sizeof(RsuExchangeHdr) <= m_config.io_sz))
			{
				if (write_cmd(cmd))
				{
					ret = read_res(cmd);
				}
			}
			return ret;
		}

		bool state()
		{
			return m_io.state() && BaseExchangeClass::state();
		}

		unsigned bin_state()
		{
			return (unsigned)m_io.state() | BaseExchangeClass::bin_state();
		}

		std::string ErrorStr() 
		{
			std::string ret = "";
			if (!state())
			{
				if (!m_io.state())
				{
					ret = "MIO error: " + GetErrorString(m_io.last_error().value()) + ";";
				}

				ret += BaseExchangeClass::ErrorStr();
			}

			return ret;
		}

		bool reset_config(const RsuExchangeConfig& a_config)
		{
			clear();
			m_config = a_config;
			bool ret = open_cs(m_config.cs_name) && open_ss(m_config.ss_name) && open_io();
			return ret;
		}

		RsuExchangeClient(const RsuExchangeConfig& a_config)
		{
			reset_config(a_config);
		}

		~RsuExchangeClient()
		{
			clear();
		}

	};

	class RsuServerExObj: public RsuServerObj {
		RsuSvrExchangeServer* m_ex = NULL;	
		void clear()
		{
			delete m_ex;
			m_ex = NULL;
		}
	protected:
		virtual void get_ex_cfg(RsuExchangeConfig& a_cfg) = 0;
	public:
		bool state() {
			return m_ex && m_ex->state();
		}

		std::string ErrorStr()
		{
			if (m_ex)
				return m_ex->ErrorStr();
			else
				return "";
		}

		bool start()
		{
			if (state())
				return true;
			RsuExchangeConfig v_xcfg;
			get_ex_cfg(v_xcfg);
			m_ex = new RsuSvrExchangeServer(this, v_xcfg);
			if (!m_ex->state())
				clear();
			return state();
		}

		void stop()
		{
			clear();
		}

		void fill_names(RsuSvrObjNames *v_names)
		{
			if (!v_names)
				return;

			v_names->io_name[0] = 0;
			v_names->SS_name[0] = 0;
			v_names->CS_name[0] = 0;

			if (m_ex)
			{
				unsigned s = m_ex->bin_state();
				const RsuExchangeConfig* c = m_ex->config();
				if (s & 1)
					strcpy_s(v_names->io_name, c->io_name.c_str());
				if (s & 2)
					strcpy_s( v_names->CS_name, c->cs_name.c_str());
				if (s & 4)
					strcpy_s(v_names->SS_name, c->ss_name.c_str());

				v_names->io_sz = c->io_sz;
				v_names->cs_tout = c->cs_tout;
				v_names->ss_tout = c->ss_tout;
			}
		}

		~RsuServerExObj()
		{
			clear();
		}
	};

}
