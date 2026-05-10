#include "TrendsRemoteClient.h"
#include <macros/AutoCloser.h>
#include <rsuPaths.h>
#include <filesystem>
#include <dylib.hpp>
#include <macros/FileFind.h>

namespace fs = std::filesystem;

void KTrendsRemoteClient::LoadPlugins()
{
    if (m_Plugins[0].pInfo)
    {
        return;
    }

    std::string exePath = rsuGetExecutionPath();
#ifdef WIN32
    std::string mask = "rsuSharedTrend_*.dll";
#else
    std::string mask = "librsuSharedTrend_*.so";
#endif // WIN32
    KFileFind files(exePath, mask);
    if (0 == files.files().size())
    {
        return;
    }

    int index = 0;
    for (const auto& file : files.files())
    {
        try
        {
            auto lib = new dylib(file, false);
            if (lib->has_symbol("TrendInfoPoint"))
            {
                m_Plugins[index].pInfo = (tTrendInfoPoint)lib->get_symbol("TrendInfoPoint");
            }
            else
            {
                ZeroMemory(&m_Plugins[index], sizeof(m_Plugins[index]));
                continue;
            }

            if (lib->has_symbol("EnumObjs"))
            {
                m_Plugins[index].pEnumObjs = (tEnumIo_Objs)lib->get_symbol("EnumObjs");
            }

            if (lib->has_symbol("EnumIOs"))
            {
                m_Plugins[index].pEnumIOs = (tEnumIo_Objs)lib->get_symbol("EnumIOs");
            }

            index++;
            if (index == _countof(m_Plugins))
            {
                ASS(0);
                break;
            }

            m_loadedLibs.push_back(lib);
        }
        catch (const dylib::load_error& e)
        {
            // s_errors.push_back(std::string("Load library ") + s + e.what());
        }
        catch (const dylib::symbol_error& e)
        {
            // s_errors.push_back(std::string("Load symbol ") + s + e.what());
        }
        catch (...)
        {
            // s_errors.push_back(s + "Unknow error!");
        }
    }
}
