#pragma once
#ifdef _WIN32
    #include <Windows.h>
    #include <io.h>
    #include <stdexcept>
#else
    #include <termios.h>
    #include <unistd.h>
#endif
#include <iostream>
#include <stdio.h>

namespace console
{
    struct Console
    {
        FILE* hStdOut;
        FILE* hStdErr;
        FILE* hStdIn;
    };

    // @throws std::runtime_error
    Console AllocConsole()
    {
#ifdef _WIN32
        Console console = {};
        if (!::AllocConsole())
        {
            throw std::runtime_error("Can't allocate console");
        }

        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);

        // std::cout, std::clog, std::cerr, std::cin
        freopen_s(&console.hStdOut, "CONOUT$", "w", stdout);
        freopen_s(&console.hStdErr, "CONOUT$", "w", stderr);
        freopen_s(&console.hStdIn, "CONIN$", "r", stdin);
        std::cout.clear();
        std::clog.clear();
        std::cerr.clear();
        std::cin.clear();

        // std::wcout, std::wclog, std::wcerr, std::wcin
        //HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        //HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        //SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
        //SetStdHandle(STD_ERROR_HANDLE, hConOut);
        //SetStdHandle(STD_INPUT_HANDLE, hConIn);
        //std::wcout.clear();
        //std::wclog.clear();
        //std::wcerr.clear();
        //std::wcin.clear();
        return console;
#else
        throw std::runtime_error("Not supported yet");
#endif
    }

    void CloseConsole(Console & console)
    {
#ifdef _WIN32
        FreeConsole();
        std::cout.setstate(std::ios_base::badbit);
        std::clog.setstate(std::ios_base::badbit);
        std::cerr.setstate(std::ios_base::badbit);
        std::cin.setstate(std::ios_base::badbit);
        fclose(console.hStdErr);
        fclose(console.hStdIn);
        fclose(console.hStdOut);
        console = {};
#endif
    }

    // vladexl https://stackoverflow.com/a/1455007
    static bool SetStdinEcho(bool enable)
    {
#ifdef WIN32
        if (!_isatty(_fileno(stdin)))
#else
        if (!isatty(fileno(stdin)))
#endif
        {
            return false;
        }

#ifdef WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        if (!GetConsoleMode(hStdin, &mode))
        {
            return false;
        }
        if (!enable) {
            mode &= ~ENABLE_ECHO_INPUT;
        }
        else {
            mode |= ENABLE_ECHO_INPUT;
        }
        if (!SetConsoleMode(hStdin, mode))
        {
            return false;
        }
#else
        struct termios tty;
        if (tcgetattr(STDIN_FILENO, &tty) != 0)
        {
            return false;
        }
        if (!enable)
        {
            tty.c_lflag &= static_cast<decltype(tty.c_lflag)>(~ECHO);
        }
        else
        {
            tty.c_lflag |= ECHO;
        }

        if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) != 0)
        {
            return false;
        }
#endif
        return true;
    }
}
