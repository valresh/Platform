// rsu_client_test.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "rsu_ex.h"
#include "rsu_err.h"
#include "rsu_proc.h"
#include "rsu_utils.h"

bool g_active = true;

rsu_cp::RsuExchangeConfig g_cfg{
	"sem_test_io", 1024, "/sem_test_CS", "/sem_test_SS", 15000, 15000
};

namespace rsu_cp {
	class RsuTestExchangeServer : public RsuExchangeServer {
		int i = 0;
		char buf[512] = {0};
	protected:
		virtual bool before_loop()
		{ 
			printf("Server loop started.\n");
			return true; 
		}
		
		virtual void after_loop() 
		{
			printf("Server loop finished.\n");
		}
		
		virtual bool do_process(RsuRequestCmd* cmd)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			printf("Server received: cmd = %d, data:'%s'\n", cmd->cmd, (char*)cmd->data);
			sprintf(buf, "Server cmd data #%d", ++i);
			cmd->cmd = 2;
			cmd->data = buf;
			cmd->size = sizeof(buf);
			return g_active;
		}
	public:
		RsuTestExchangeServer(const RsuExchangeConfig &a_cfg) :
			RsuExchangeServer(a_cfg) {}
	};
}

void cl_loop()
{
	char buf[512];
	int i = 0;	
	rsu_cp::RsuExchangeClient c{ g_cfg };
	printf("Client loop: state = %d (%s, %s, %s)\n", c.state(),
		rsu_cp::GetErrorString(c.last_error(0)).c_str(), rsu_cp::GetErrorString(c.last_error(1)).c_str(), rsu_cp::GetErrorString(c.last_error(2)).c_str());

	if (c.state())
		while (g_active)
		{
			sprintf(buf, "Client cmd data #%d", ++i);
			rsu_cp::RsuRequestCmd cmd{ 1, buf, sizeof(buf) };
			printf("Client cmd: cmd = %d, data:'%s'\n", cmd.cmd, (char*) cmd.data);

			if (c.send_cmd(&cmd))
			{
				printf("Server responce: cmd = %d, data:'%s'\n", cmd.cmd, (char*) cmd.data);
			}
			else
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}		
	printf("Client loop finished\n");
}

void run_client()
{
	printf("The programm run as client in Simulator...\n");
	rsu_cp::RsuSvrProc p;
	if (p.create("rsu_ex_test", "s"))
	{
		printf("The server was successfully created.\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::thread t_cl(cl_loop);
		t_cl.detach();
		rsu_cp::wait_enter(g_active);
	}
	else
		printf("Server creation error: %s\n", rsu_cp::GetErrorString(p.last_err()).c_str());
}

void run_server()
{
	printf("The programm run as server in RsuServer...\n");
	rsu_cp::RsuTestExchangeServer s{ g_cfg };
	printf("Server state = %d (%s, %s, %s)\n", s.state(),
		rsu_cp::GetErrorString(s.last_error(0)).c_str(), rsu_cp::GetErrorString(s.last_error(1)).c_str(), rsu_cp::GetErrorString(s.last_error(2)).c_str());
	rsu_cp::wait_enter(g_active);
}

int main(int argc, char* argv[])
{	
	if (argc > 1 && std::string(argv[1]) == "d")
	{
		rsu_cp::RsuTestExchangeServer s{ g_cfg };
		printf("Server state = %d (%s, %s, %s)\n", s.state(),
			rsu_cp::GetErrorString(s.last_error(0)).c_str(), rsu_cp::GetErrorString(s.last_error(1)).c_str(), rsu_cp::GetErrorString(s.last_error(2)).c_str());
		std::thread t_cl(cl_loop);
		t_cl.detach();
		rsu_cp::wait_enter(g_active);
	} 
	else
	{
		if (argc > 1) 
		{
			printf("run_server...\n");
			run_server();
		}
		else
		{
			printf("run_client...\n");
			run_client();
		}
	}
	
	return 0;
}
