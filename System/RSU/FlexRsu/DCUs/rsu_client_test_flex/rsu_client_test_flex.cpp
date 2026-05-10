#include <cassert>
#include <rsus/rsu_basemodel.h>
#include <rsus/rsu_api.h>
#include "rsu_utils.h"
#include <rsus/rsuPaths.h>
#include <crosspath.h>
#include <crossstring.h>
#include <thread>

#if defined(_WIN32)
//
#else
	#include <csignal>
#endif

static rsu_cp::RsuClientsHolder g_RsuHolder;

bool g_active = true;

enum
{
	eRM_COUNT = 32,
};

IBaseModel* g_RuntimeModels[eRM_COUNT] = { NULL };

void SysMSG(const char* fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	vfprintf(stdout, fmt, argptr);
	va_end(argptr);
}

fs::path ProjectModelPath()
{
	return fs::path(rsuGetProjectPath()) / fs::path("DATA") / fs::path("Acy") / fs::path("Models.csv");
}

IBaseModel* Load_ObjectEx(const std::string& DllName, const std::string& ObjName)
{
	return nullptr;
}

IBaseModel* Load_ObjectRSU(const std::string& DllName, const std::string& ObjName, bool& o_ValidLib)
{
	return g_RsuHolder.create_client(DllName, "", ObjName, o_ValidLib);
}

void LoadRsuModelsCP(int& iRuntimeModel)
{
	fs::path mp = ProjectModelPath();

	std::vector<std::string> rsu_vec;
	std::string fn = mp.generic_string();
	std::ifstream file_in(fn);
	if (file_in)
	{
		std::string line;
		std::getline(file_in, line); //Пропускаем строку с названиями полей
		while (std::getline(file_in, line))
			rsu_vec.push_back(cross::string::rtrim(line));
	}
	if (!rsu_vec.size())
		return;

	for (const auto& s : rsu_vec)
	{
		if (cross::string::starts_with(s.c_str(), U'#'))
			continue;

		std::vector<std::string> v_names;
		cross::string::split_strings(s, ";", std::back_inserter(v_names));

		SysMSG("#Загрузка библиотеки '%s'\n", v_names[0].c_str());

		if (v_names.size() < 2)
		{
			SysMSG("Ошибка загрузки библиотеки '%s' - отсутствует имя объекта\n", v_names[0].c_str());
			continue;
		}

		assert(iRuntimeModel < countof(g_RuntimeModels));
		bool v_ValidLib = false;
		IBaseModel* pO = Load_ObjectRSU(v_names[0], v_names[1], v_ValidLib);

		if (!pO)
			pO = Load_ObjectEx(v_names[0], v_names[1]);

		if (pO)
		{
			pO->PropsWasRead = v_names.size() > 2 && cross::string::stricmp(v_names[2], "Save");
			g_RuntimeModels[iRuntimeModel++] = pO;
			std::string v_msg = "#\nБиблиотека '" + v_names[0] + "' загружена успешно!\n";
			SysMSG(v_msg.c_str());
		}
		else
		{
			SysMSG("Ошибка загрузки библиотеки '%s'\n", v_names[0].c_str());
		}
	}
}

int do_step(int a_step)
{
	int res = 0;
	for (auto& i : g_RsuHolder.models())
	{
		if (i->api()->State())
		switch (a_step) {
		case -1:
			res = (i->api()->Init())(0);
			break;
		case 0:
			res = (i->api()->Step0())();
			break;
		case 1:	
			res = (i->api()->Step1())();
			break;
		default:
			res = (i->api()->StepT())(0.1);
			break;
		}
	}
	return res;
}

void test_loop()
{
	int i = -2;
	printf("Begin of test loop.\n");
	while (g_active && i < 100) 
		do_step(++i);
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
	g_RsuHolder.cleanup();
}

int main()
{
	int iRtmModels = 0;
	no_zombie_child();

	LoadRsuModelsCP(iRtmModels);
	bool v_LoadResult = g_RsuHolder.ErrorCount() == 0;
	printf("%s\n", v_LoadResult ? "RSU models successful loaded!" : "RSU models load error!");
	if (v_LoadResult)
	{		
		std::thread t_cl(test_loop);
		t_cl.join();
	}
	cleanup();
	printf("Work complete!\n");

	return 0;
}
