#pragma once

#include <string>
#include "rsu_fs.h"
#include "rsu_err.h"
#include <thread>
#include "LogFile.h"

#ifdef _WIN32

#else
#include <iostream>
#include <fstream>
#include <spawn.h>
#include <regex>
#endif


namespace rsu_cp {

#define RSU_SVR_PROC "rsu_server"

    class RsuSvrProc
    {
        int m_last_err = 0;
        std::string proc_name(const std::string a_name = "")
        {
            std::string v_name = a_name == "" ? RSU_SVR_PROC : a_name;
            return executable_directory() +
#ifdef _WIN32
                "\\"
#else
                "/"
#endif
                + a_name.c_str();
        }
    public:
        int last_err()
        {
            return m_last_err;
        }

        std::string last_err_msg() 
        {
            return GetErrorString(m_last_err);
        }

        bool exists(const std::string a_name = "")
        {
            std::string v_name = (a_name == "" ? RSU_SVR_PROC : a_name);
#ifdef _WIN32
            int pid = getProcIdByName(v_name);
            return pid != -1;
#else
            std::vector<int> pidlist;
            return pidof(v_name, pidlist);
#endif
        }

        bool create(const std::string a_name = "", const std::string a_params = "")
        {
            std::string v_name = (a_name == "" ? RSU_SVR_PROC : a_name);
            if (exists(v_name))
            {
                return true;
            }

            m_last_err = 0;
            std::string pn = proc_name(v_name);
#ifdef _WIN32
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            if (a_params != "")
                pn = pn + " " + a_params;
            
            CLogFile::LogEx(LOGLEVEL_HIGH, "Launch: \"%s\"\n", pn.c_str());
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            if (!CreateProcess(NULL,   // No module name (use command line)
                (char*)pn.c_str(),    // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                CREATE_NEW_CONSOLE,  // Creation flags //DETACHED_PROCESS
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory 
                &si,            // Pointer to STARTUPINFO structure
                &pi)           // Pointer to PROCESS_INFORMATION structure
                )
            {
                m_last_err = GetLastError();
                return false;
            }

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
#else		
            char* argV[] = { (char*)pn.c_str(), (char*)0, (char*)0 };
            if (a_params != "")
            {
                argV[1] = (char*)a_params.c_str();
            }

            std::string v_cmd = pn + " &";
            CLogFile::LogEx(LOGLEVEL_HIGH, "Launch: \"%s\"\n", v_cmd.c_str());
            system(v_cmd.c_str());
            CLogFile::LogEx(LOGLEVEL_HIGH, "RsuServer \"%s\" started!\n", argV[0]);
#endif		
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            return m_last_err == 0;
        }
    };
}	//rsu_cp