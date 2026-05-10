#include "Server.h"
//#include <io.h>
#include <CommonRSUs.h>
#include "StateSerLocal.h"
#include <crosspath.h>
#include <crossstring.h>
#include <crossplatform.h>

namespace fs = std::filesystem;

DWORD m_currentVersion = 0;

int KServer::StateSave(LPCSTR pszPath)
{
    fs::path filePath(pszPath);
    if (fs::exists(filePath) && 0 != rsu_cp::cp_access(filePath, rsu_cp::cp_path_access::F))
    {
        return 1;
    }

    try
    {
        fs::remove(filePath);
        fs::path p_tmp = filePath.string().append(".tmp");
        KStateSerLocal saver(p_tmp.generic_string().c_str(), true);
        IFlexStateSer* pStateSer = saver.GetInterface();
        pStateSer->SetConvertToUtf8(m_currentVersion != StateVersion_utf8);
        pStateSer->SimpleWrite(&StateVersion_utf8, sizeof(StateVersion_utf8));
        KFlexRsu::SaveState(*pStateSer);
        saver.Close();

        std::string p_zip = filePath.replace_extension(".datx").filename().generic_string();
        RsuPackager(p_tmp.generic_string().c_str(), pszPath, p_zip.c_str());
        fs::remove(p_tmp);

        return 0;
    }
    catch (const std::exception&)
    {
        return 2;
    }
}

int KServer::StateRestore(LPCSTR pszPath)
{
    fs::path filePath(pszPath);
    if (fs::exists(filePath) && 0 != rsu_cp::cp_access(filePath, rsu_cp::cp_path_access::R))
    {
        return 1;
    }

    try
    {
        std::fstream f;
        if (RsuExtractor(f, filePath.generic_string().c_str()))
        {
            KStateSerLocal reader(f, false);
            IFlexStateSer* pStateSer = reader.GetInterface();
            pStateSer->SimpleRead(&m_currentVersion, sizeof(m_currentVersion));
            pStateSer->SetConvertToUtf8(m_currentVersion != StateVersion_utf8);
            ASS(m_currentVersion);
            return KFlexRsu::RestoreState(*pStateSer, m_currentVersion);
        }
        else
        {
            return 1;
        }
    }
    catch (const std::exception& ex)
    {
        return 2;
    }
}
