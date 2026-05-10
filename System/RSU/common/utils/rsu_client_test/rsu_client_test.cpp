#include <rsus/rsu_clients_hlpr.h>
#include <rsus/rsu_api.h>
#include "rsu_utils.h"
#include <thread>

#if defined(_WIN32)
//
#else
	#include <csignal>
#endif

rsu_cp::RsuClientsLdrHelper g_ldr;

struct RsuClientTestData {
	std::string n_lib = "";
	std::string n_cfg = "";
	std::string n_obj = "";	
	rsu_cp::RsuClientAPI api;
	RsuClientTestData(const std::string a_lib, const std::string a_cfg, const std::string a_obj) : n_lib{ a_lib }, n_cfg{ a_cfg }, n_obj { a_obj } {};
};

std::vector <RsuClientTestData> g_test_data = {
	 RsuClientTestData ("rsu_client_lib", "cfg1", "a"),
	 RsuClientTestData ("rsu_client_lib", "cfg1", "b"),
	 RsuClientTestData ("rsu_client_lib", "cfg2", "c"),
	 RsuClientTestData ("rsu_client_lib", "cfg2", "d"),		
	 RsuClientTestData ("rsu_client_lib1", "cfg11", "a1"),
	 RsuClientTestData ("rsu_client_lib1", "cfg11", "b1"),
	 RsuClientTestData ("rsu_client_lib1", "cfg12", "c1"),
	 RsuClientTestData ("rsu_client_lib1", "cfg12", "d1")	
};

bool g_active = true;

void print_res(int res, RsuClientTestData &d, std::string fn)
{
	if (res == 0)
		printf("Successfull call %s for %s.%s.%s\n", fn.c_str(), d.n_lib.c_str(), d.n_cfg.c_str(), d.n_obj.c_str());
	else
		printf("Call %s for %s.%s.%s return error %d: %s.\n", fn.c_str(), d.n_lib.c_str(), d.n_cfg.c_str(), d.n_obj.c_str(),
			RsuAPILastError(&d.api), RsuAPIErrorMsg(&d.api).c_str());
}

void do_step(int a_step)
{
	int res = 0;
	for (auto& i : g_test_data)
	{
		switch (a_step) {
		case -2:
			res = g_ldr.create_client(i.n_lib.c_str(), i.n_cfg.c_str(), i.n_obj.c_str(), NULL, &i.api);
			print_res(res, i, "create_client");
			break;
		case -1:
			if (i.api.State())
			{
				res = (i.api.Init())(0);
				print_res(res, i, "Init");
			}
			break;
		case 0:
			if (i.api.State())
			{
				res = (i.api.Step0())();
				print_res(res, i, "Step0");
			}
			break;
		case 1:	
			if (i.api.State())
			{
				res = (i.api.Step1())();
				print_res(res, i, "Step1");
			}
			break;
		default:
			if (i.api.State())
			{
				res = (i.api.StepT())(0.1);
				print_res(res, i, "StepT");
			}
			break;
		}
	}	
}

void test_loop()
{
	int i = -2;
	printf("Begin of test loop.\n");
	while (g_active && i < 100) 
		do_step(i++);
	printf("End of test loop.\n");
}

void no_zombie_child ()
{
#if defined(_WIN32)
	//
#else
	signal(SIGCHLD, SIG_IGN);
#endif
}

void cleanup()
{
	g_test_data.clear();
	g_ldr.cleanup();
}

int main()
{
	printf("anchor_абвгдеёж\n");
	printf("%s\n", g_ldr.ldr_state() ? "RsuClientsLdrHelper successful created!" : g_ldr.ldr_error().c_str());	
	no_zombie_child();

	std::thread t_cl(test_loop);

	//t_cl.detach();
	//rsu_cp::wait_enter(g_active);

	t_cl.join();
	cleanup();
	printf("Work complete!\n");

	return 0;
}
