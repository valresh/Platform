#include "rsu_servers_hlpr.h"
#include <iostream>
#include <string>
#include "rsu_proc.h"
#include <tray.h>
#include "crossruntime.h"
#include "LogWindow.h"

static void quit_cb(struct tray_menu* item);
struct tray tray1;
std::unique_ptr<LogWindow> logwindow = nullptr;

static void quit_cb(struct tray_menu* item) {
    (void)item;
    printf("quit cb\n");
    tray_exit();
}

static void toggle_cb(struct tray_menu* item) {
    if(logwindow != nullptr)
    {
        logwindow->ShowToggle();
    }
}

int main()
{
    CLogFile::LogEx(LOGLEVEL_HIGH, "RsuServer main() begin!\n");

    const std::string path = CLogFile::GetLogPath();
    bool trayOk = false;
    if (!cross::runtime::is_wsl())
    {
        //проверим, можно ли использовать QT
        if (LogWindow::IsQtAvailable())
        {
            logwindow = std::make_unique<LogWindow>("RsuServer", "RsuServer.ico", path);

            logwindow->AddTrayMenuItem("Show / Hide", []() {logwindow->ShowToggle(); });
            logwindow->AddTrayMenuSeparator();
            logwindow->AddTrayMenuItem("Quit", []() {logwindow->Quit(); });
            trayOk = true;
        }
        else
        {

#ifdef _WIN32
            struct tray_menu tm[] = {
            #ifdef WIN32
                {.text = (char*)"Show/Hide", .cb = toggle_cb},
                {.text = (char*)"-"},
            #endif
                {.text = (char*)"Quit", .cb = quit_cb},
                {.text = NULL}
            };

            tray1 = {
                .icon = (char*)"RsuServer.ico",
                .menu = tm,
            };
            trayOk = tray_init(&tray1) >= 0;

            logwindow = std::make_unique<LogWindow>("RsuServer", "RsuServer.ico", path);

#else
            trayOk = false;
#endif


        }
    }

    if (!trayOk)
    {
        CLogFile::LogEx(LOGLEVEL_HIGH, "failed to create tray\n");
    }

    rsu_cp::RsuSvrProc p;
    if (p.exists())
    {
        CLogFile::LogEx(LOGLEVEL_HIGH, "Another RsuServer exists! Exit.\n");
        return 0;
    }

    rsu_cp::RsuServerLdrHelper rsu_svr_hlpr;
    int ret = rsu_svr_hlpr.start();
    if (!ret)
    {
        CLogFile::LogEx(LOGLEVEL_HIGH, "RsuServer started successfully!\n");
    }
    else
    {
        CLogFile::LogEx(LOGLEVEL_HIGH, "RsuServer start error!\n");
    }

    if (trayOk && logwindow != nullptr)
    {
        logwindow->Work();
    }
    else
    {
        printf("Press ESC to exit...\n");
        int keyCode = -1;
        do
        {
            keyCode = static_cast<int>(std::cin.get());
        } while (keyCode != 27); // 27 -> ESC key code
    }

    rsu_svr_hlpr.stop();

    CLogFile::LogEx(LOGLEVEL_HIGH, "RsuServer stoped successfully!\n");
    return 0;
}
