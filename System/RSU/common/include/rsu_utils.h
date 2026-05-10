#pragma once

#include <iostream>
#include <thread>

namespace rsu_cp {

	void wait_enter(bool &key, unsigned tout = 3000)
	{
		printf("Press ENTER to exit...\n");
		std::string l;
		std::getline(std::cin, l);
		key = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(tout));
		printf("Work complete!\n");
	}

}