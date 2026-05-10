#include <iostream>
#include <thread>
#include <signal.h>
#include <chrono>
#include "crossstring.h"
#include "crossruntime.h"
#include "crosssemaphore.h"
#include "LogWindow.h"
#include "SimulationControl.h"
#include "LogFile.h"
#include "rsu_proc.h"
#include "tray.h"
#include "MqttConfigurator.h"

const char* MODELEX_SEMAPHORE = "ModelEx";
const std::chrono::seconds MaxModelExTimeout(30);

void StopPreviousModelEx(int pid, std::chrono::seconds maxTimeout)
{
    if (!cross::runtime::is_running(pid))
    {
        return;
    }

    if (cross::runtime::is_running(pid))
    {
        CLogFile::LogEx(LOGLEVEL_MEDIUM, "Wait %i seconds for ModelEx[%i] stopping...", (int)maxTimeout.count(), pid);
    }
    
    // Wait within maxTimeout
    auto end = std::chrono::system_clock::now() + maxTimeout;
    while (cross::runtime::is_running(pid) && (std::chrono::system_clock::now() < end))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!cross::runtime::is_running(pid))
    {
        CLogFile::LogEx(LOGLEVEL_URGENT, "ModelEx[%i] has stopped", pid);
        return;
    }

    CLogFile::LogEx(LOGLEVEL_URGENT, "Timeout. Kill ModelEx[%i]...", pid);
    while (cross::runtime::is_running(pid))
    {
        cross::runtime::kill(pid);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    CLogFile::LogEx(LOGLEVEL_URGENT, "ModelEx[%i] has been killed", pid);
}

#ifndef _WIN32
static bool CanReadFromTerminal(pid_t pid)
{
    std::filesystem::path path = "/proc/" + std::to_string(pid) + "/fd/0";
    return std::filesystem::exists(path) &&
        std::filesystem::is_symlink(path) &&
        std::filesystem::read_symlink(path).string().rfind("/dev/pts/", 0) == 0;
}

// @brief Returns true if ModelEx user == Rsu user
static bool HasRsuSameUser()
{
    auto rsuPid = cross::runtime::get_process_id(RSU_SVR_PROC);
    if (rsuPid == -1)
    {
        return true;
    }

    try 
    {
        auto rsuProcessInfo = cross::runtime::get_process_info(rsuPid);
        auto modelexProcessInfo = cross::runtime::get_process_info(cross::runtime::current_process_id());
        return rsuProcessInfo.RealUID() == modelexProcessInfo.RealUID();
        
    }
    catch (std::runtime_error& e)
    {
        CLogFile::LogEx(LOGLEVEL_URGENT, e.what());
        return true;
    }
}
#endif

std::unique_ptr<LogWindow> p_logWindow = nullptr;

static void quit_cb(struct tray_menu* item)
{
    (void)item;
    tray_exit();
}

static void toggle_cb(struct tray_menu* item)
{
    if(p_logWindow != nullptr)
        p_logWindow->ShowToggle();
}

int main(int argc, char* argv[])
{
    std::cout << "ModelEx v4.0.0" << std::endl;
    bool consoleMode = false;
    bool defaultLogs = false;
    bool forceConfigureMqtt = false;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-m")
        {
            forceConfigureMqtt = true;
        }

#ifndef _WIN32
        if (arg == "-c")
        {
            consoleMode = true;
        }
        else if (arg == "-l")
        {
            defaultLogs = true;
        }
        else if (arg == "-h" || arg == "--help")
        {
            std::cout <<
                "Usage: ModelEx [options]\n"
                "Options:\n"
                "  -c    Run in console mode\n"
                "  -l    Enable default logging, like file Logging.txt created\n"
                "  -m    Configure MQTT connection security\n"
                << std::endl;
            return 0;
        }
        else
#endif
        {
            // When ModelEx received command Commands::StopSimulation:
            // ModelEx restarts and launches new ModelEx like "modelex.exe /pid=12345"
            // So during startup of the new ModelEx - old ModelEx must be stopped.
            // StopPreviousModelEx() will be called just in order to guarantee that ModelEx will be alive one only.
            auto index = std::string(argv[1]).find("/pid=");
            if (index >= 0)
            {
                char* spid = argv[1] + index + 5;
                int pid = atoi(spid);
                StopPreviousModelEx(pid, MaxModelExTimeout);
            }
        }
    }

#ifndef _WIN32
    if (!HasRsuSameUser())
    {
        CLogFile::LogEx(LOGLEVEL_URGENT, "Error: '%s' started under different account, stop it before", RSU_SVR_PROC);
        return 1;
    }
#endif

    if(defaultLogs)
    {
        CLogFile::SetEnable(true, true);
    }

    auto startupSemaphore = cross::counting_semaphore::open(MODELEX_SEMAPHORE);
    if (startupSemaphore != nullptr)
    {
        CLogFile::LogEx(LOGLEVEL_URGENT, "Error: Another instance of ModelEx is already started");
        return 1;
    }

    CLogFile::LogEx(LOGLEVEL_HIGH, "");
    CLogFile::LogEx(LOGLEVEL_HIGH, "------ ModelEx starting -------------------------------");
    try
    {
        startupSemaphore = std::make_unique<cross::counting_semaphore>(MODELEX_SEMAPHORE, 1);
    }
    catch (const std::system_error& e)
    {
        CLogFile::LogEx(LOGLEVEL_HIGH, "Warning: Impossible to create semaphore \'%s\'. Continue.", MODELEX_SEMAPHORE);
    }

    MqttConfigurator mqttConfigurator;
    auto mqttSettings = mqttConfigurator.ReadSettings();
    try
    {
        if (mqttSettings.IsMisconfigured() || forceConfigureMqtt)
        {
            mqttSettings = mqttConfigurator.Configure(mqttSettings);
        }
    }
    catch (std::runtime_error& e)
    {
        CLogFile::LogEx(LOGLEVEL_URGENT, e.what());
        return 2;
    }

    const std::string path = CLogFile::GetLogPath();
    bool trayOk = false;
    //проверим, можно ли использовать QT
    if(!consoleMode && !cross::runtime::is_wsl())
    {
        if (LogWindow::IsQtAvailable())
        {
            p_logWindow = std::make_unique<LogWindow>("ModelEx", "ModelEx.ico", path);
            p_logWindow->AddTrayMenuItem("Show / Hide", []() {p_logWindow->ShowToggle(); });
#ifdef _DEBUG
            p_logWindow->AddTrayMenuItem("Debug: Start simulation (N/N/Paused/1.0)", [] {SimulatorControl::OnStartPausedSimulation(); });
            p_logWindow->AddTrayMenuItem("Debug: Start simulation (N/N/Start/1.0)", [] {SimulatorControl::OnStartSimulation(); });
            p_logWindow->AddTrayMenuItem("Debug: Restart ModelEx", [] {SimulatorControl::OnRestartModelEx(); });
#endif
            p_logWindow->AddTrayMenuSeparator();
            p_logWindow->AddTrayMenuItem("Quit", []() {p_logWindow->Quit(); });

            trayOk = true;
        }
        else
        {
#ifdef _WIN32
            //init tray
            static struct tray_menu tm[] = {        
            {.text = "Show/Hide", .cb = toggle_cb},
        #ifdef _DEBUG
                        {.text = "Debug: Start simulation (N/N/Paused/1.0)", .cb = SimulatorControl::OnStartPausedSimulation},
                        {.text = "Debug: Start simulation (N/N/Start/1.0)", .cb = SimulatorControl::OnStartSimulation},
                        {.text = "Debug: Restart ModelEx", .cb = SimulatorControl::OnRestartModelEx},
        #endif
                        {.text = "-"},
                        {.text = "Quit", .cb = quit_cb},
                        {.text = NULL}
            };
            tray tray1 = {
                .icon = "ModelEx.ico",
                .menu = tm,
            };
            trayOk = tray_init(&tray1) >= 0;
            p_logWindow = std::make_unique<LogWindow>("ModelEx", "ModelEx.ico", path);
#else
            //временно, пока неживой GTK2
            bool trayOk = false;
#endif
        }
    }

    SimulatorControl::StartModelExServer(mqttSettings);
    if (trayOk && p_logWindow)
    {
        //go cicle
        p_logWindow->Work();
    }
    else
    {
#ifndef _WIN32
        bool isLinuxDebug = false;
#ifdef _DEBUG
        isLinuxDebug = true;
#endif
        if (!isLinuxDebug)
        {
            std::cout << "Press ENTER to exit...\n";
            int c;
            while ((c = getchar()) != '\n')
            {
                continue;
            }
        }
        else
        {
            if (!CanReadFromTerminal(cross::runtime::current_process_id()))
            {
                while (1)
                {
                    sleep(30);
                }
            }

            std::cout << "Press 'S' to Start simulation (N/N/Start/1.0)\n" <<
                "Press 'P' to Start simulation (N/N/Paused/1.0)\n" <<
                "Press 'R' to Restart ModelEx\n" <<
                "Press 'Q' to Quit" << std::endl;
            bool quit = false;
            while (!quit)
            {
                auto c = getchar();
                switch (c)
                {
#ifdef _DEBUG
                case 'S':
                    SimulatorControl::OnStartSimulation(nullptr);
                    break;
                case 'P':
                    SimulatorControl::OnStartPausedSimulation(nullptr);
                    break;
                case 'R':
                    SimulatorControl::OnRestartModelEx(nullptr);
                    break;

#endif
                case 'Q':
                    quit = true;
                    break;
                case 10:
                    break;
                default:
                    std::cout << "Invalid input." << std::endl;
                }
            }
        }
#endif
    }

    SimulatorControl::StopModelExServer();
    return 0;
}