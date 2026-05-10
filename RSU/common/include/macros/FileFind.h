#pragma once
#include <crossplatform.h>
#include <CommProc.h>
#include <regex>
//#include <crossstring.h>
#include <algorithm>
#include <string>

#define fs std::filesystem

/*
for( KFileFind ff(szPath); ff; ff.FindNext() )
{
ff.GetFullPathToFile( szPath );
}
*/
class KFileFind
{
    std::string m_root = "";
    std::string m_dir = "";
    std::vector<std::string> m_Files;
public:
    std::vector<std::string>& files() { return m_Files; }
    KFileFind() {}
    KFileFind(const std::string& a_root, const std::string& a_mask)
    {
        if (a_root != "" && a_mask != "")
            Find(a_root, a_mask);
    }
    KFileFind(const std::string& a_mask)
    {
        if (a_mask != "")
            Find(a_mask);
    }
    bool Find(const std::string& a_mask)
    {
        fs::path path(a_mask);
        std::string s_root = path.parent_path().generic_string();
        std::string s_mask = path.filename().string();
        if (s_root != "" && s_mask != "")
            return Find(s_root, s_mask);
        else
            return false;
    }
    std::string wildcardToRegex(const std::string& wildcardPattern) {
        std::string regexPattern;
        for (char c : wildcardPattern) {
            switch (c) {
            case '*':
                regexPattern += ".*";
                break;
            case '?':
                regexPattern += ".";
                break;
            case '.':
            case '+':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case '\\':
            case '^':
            case '$':
                regexPattern += '\\'; // Escape special regex characters
                regexPattern += c;
                break;
            default:
                regexPattern += c;
                break;
            }
        }
        return "^" + regexPattern + "$"; // Anchor the pattern
    }
    bool Find(const std::string& a_root, const std::string& a_mask)
    {
        std::string m_mask = wildcardToRegex(a_mask);
        auto vec{ std::vector<std::filesystem::path> {} };
        std::regex e(m_mask);
        if (std::filesystem::exists(a_root))
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(a_root))
            {
                if (entry.is_regular_file())
                {
                    const auto& fp = entry.path();
                    const auto& fn = fp.filename();
                    if (std::regex_match(fn.generic_string(), e))
                    {
                        m_Files.push_back(fp.generic_string());
                    }
                }
            }
        }

        return m_Files.size() > 0;
    }

    // @brief file.001 file.002 ...
    void SortByNumericExtension()
    {
        std::sort(
            m_Files.begin(), 
            m_Files.end(), 
            [](const std::string& file1, const std::string& file2) {
                auto ext1 = (std::filesystem::path(file1)).extension().string();
                auto ext2 = (std::filesystem::path(file2)).extension().string();
                auto numericExt1 = atoi(ext1.c_str() + 1);
                auto numericExt2 = atoi(ext2.c_str() + 1);
                return numericExt1 < numericExt2;
            });
    }
};
