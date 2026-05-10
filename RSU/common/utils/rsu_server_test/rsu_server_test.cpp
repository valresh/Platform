// rsu_client_test.cpp : Defines the entry point for the application.
//

#include "rsu_servers_hlpr.h"
#include <iostream>
#include <string>
#include "rsu_proc.h"

/*
* #if defined(_WIN32)
	//
#else
#include <unistd.h>
#endif

void detach()
{
#if defined(_WIN32)
	//
#else
	//setsid();
	//system("setsid COMMAND </dev/null >/dev/null 2>/dev/null &");

	// if 0, change to the "/" directory
	int nochdir = 0;
	// if 0, redirect standard input, output and error to /dev/null
	// this is equivalent to "closing the file descriptors"
	int noclose = 0;

	// glibc call to daemonize this process without a double fork
	//if (daemon(nochdir, noclose))
	//	perror("daemon");
#endif
}
*/

int main()
{
	printf("RsuServer main() begin.\n");
	rsu_cp::RsuSvrProc p;
	if (p.exists())
	{ 
		printf("Another RsuServer exists! Exit.\n");
		return 0;
	}

	rsu_cp::RsuServerLdrHelper h;
	int ret = h.start();
	if (!ret)
		printf("RsuServer started successfully!\n");
	else
		printf("RsuServer start error!\n");	
	printf("Press ENTER to exit...\n");

	std::string l;
	std::getline(std::cin, l);

	h.stop();

	return 0;
}
