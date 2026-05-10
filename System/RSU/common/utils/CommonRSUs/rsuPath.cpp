#include <rsus/rsuPaths.h>
#include <rsu_fs.h>
#include <rsu_str.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <crosspath.h>
#include <crossstring.h>

class PropField {
    size_t m_pos = 0 ;
    std::string m_data = "";
    std::string m_name = "";
    std::string m_value = "";
public:
    size_t pos() { return m_pos; }
    const std::string &name() {
        return m_name;
    }
    const std::string &value() const {
        return m_value;
    }
    bool name_is(const std::string& a_name, bool a_strict = false) const
    {
        return a_strict ? a_name == m_name : m_name.find(a_name) != m_name.npos;
    }
    PropField(size_t a_pos, const std::string& a_data): m_pos (a_pos), m_data (a_data)
    {
        std::vector<std::string> f;
        rsu_cp::splitStrings(a_data, "#", std::back_inserter(f));
        m_name = f.size() > 0 ? f[0] : "";
        m_value = f.size() > 1 ? f[1] : "";
    }
};

class PropHolder {
    std::string m_id = "";
    std::vector<PropField> m_fields;
public:
    const PropField* field_by_name (const std::string& a_name, bool a_strict = false) const
    {
        const PropField* ret = nullptr;
        for (auto &x : m_fields)
        {
            if (x.name_is(a_name, a_strict))
            { 
                ret = &x;
                break;
            }
        }
        return ret;
    }
    PropHolder(std::string val) {
        std::vector<std::string> f;
        rsu_cp::splitStrings(val, ";", std::back_inserter(f));
        if (f.size())
        {
            m_id = f[0];
            f.erase(f.begin());
        }
        for (size_t i = 0; i < f.size(); ++i)
            m_fields.push_back(PropField(i, f[i]));
    }
};

static fs::path BinPath()
{
    return rsu_cp::executable_directory();
}

static fs::path BuildPath(const std::string a_dir, const std::string a_file = "")
{
    return BinPath().parent_path() / fs::path(a_dir) / fs::path(a_file);
}

const static std::string s_no_val = ""; 

class PropsLoader {
    bool m_state = false;
    std::vector<PropHolder> m_fields;
    std::vector<PropHolder> m_prj_fields;
    std::string m_PrjRootDir = ""; 
    std::string m_RsuPath = "";
    void clear()
    {
        m_state = false;
        m_PrjRootDir = "";
        m_RsuPath = "";
        m_fields.clear();
        m_prj_fields.clear();
    }
    bool ReadFields(const fs::path& p, bool a_read_prj)
    {
        m_state = false;
        std::string s_p = p.generic_string();
        std::vector<PropHolder>& v_h = a_read_prj ? m_prj_fields : m_fields;  
        v_h.clear();
        if (fs::exists(p))
        {
            std::ifstream f(s_p, std::ios::in);
            if (f.is_open())
            {
                m_state = true;
                std::string l;
                while (std::getline(f, l))
                {
                    cross::string::rtrim(l);
                    if (l != "")
                        v_h.push_back(PropHolder(l));
                }
            }
        }
        return m_state;
    }
public:    
    bool state() { return m_state; }
    const std::string& PrjRootDir() { 
        return m_PrjRootDir; 
    }
    const std::string& RsuPath() {
        return m_RsuPath;
    }
    const std::vector<PropHolder>& fields() { return m_fields; };
    const std::string& field_value(bool a_prj_field, const std::string& a_name, bool a_strict = false)
    {   
        std::vector<PropHolder>& v_h = a_prj_field ? m_prj_fields : m_fields;
        for (auto &x : v_h)
        {
            const PropField* f = x.field_by_name(a_name, a_strict);
            if (f)
                return f->value();
        }
        return s_no_val;
    }
    bool ReadProps()
    {
        clear();
        fs::path p = BuildPath("PROP", "Properties.csv");
        
        ReadFields(p, false);
        if (m_state)
        {
            std::string v_Root = field_value(false, "ROOT");
            fs::path pp = BuildPath("projects", v_Root);
            SetPrjRootDir(pp.generic_string());            
        }
        return m_state;
    }
    bool SetPrjRootDir(const std::string& a_PrjRoot)
    {
        fs::path p = fs::path(a_PrjRoot);
        m_PrjRootDir = p.generic_string();
        fs::path pp = p / fs::path("PROP") / fs::path("Properties.csv");
        ReadFields(pp, true);
        m_RsuPath = "";
        if ( m_state )
        {
            std::string v_rsu_dir = field_value(true, "RSU");
            if (v_rsu_dir == "")
                v_rsu_dir = "TRSU";
            fs::path rp = p / fs::path(v_rsu_dir);
            if (fs::exists(rp))
                m_RsuPath = rp.generic_string();
        }
        return m_state;
    }
    void SetRsuPath(const std::string& a_RsuPath)
    {
        m_RsuPath = a_RsuPath;
    }
    PropsLoader ()
    {
        ReadProps();
    }
    ~PropsLoader()
    {
        clear();
    }
};

static PropsLoader s_pl;
//static PropsLoader s_pl;

static const std::string& _Prop(bool a_prj_prop, const std::string& a_key, bool a_strict = false)
{    
    return s_pl.field_value (a_prj_prop, a_key, a_strict);
}

/*
std::string SetNewPath(std::string& a_OldPath, const std::string& a_NewPath)
{
    fs::path dir(a_NewPath);
    if (!fs::exists(dir))
        return "";
    a_OldPath = dir.generic_string();
    return dir.generic_string();
}
*/

void rsuSetProjectPath(const std::string& a_PrjRootDir )
{
    s_pl.SetPrjRootDir(a_PrjRootDir);
}

const std::string& rsuGetProjectPath()
{    
    return s_pl.PrjRootDir();
}

static std::string s_m_path = "";
const std::string& rsuGetMemoryPath()
{    
    if(s_m_path == "")
    {
        fs::path p = BuildPath(rsuGetProjectPath(), "Memory/");
        if (!fs::exists(p))
            std::filesystem::create_directory(p);
        s_m_path = p.generic_string();
    }
    return s_m_path;
}

static std::string g_ExePath = rsu_cp::executable_directory();
const std::string& rsuGetExecutionPath()
{    
    return g_ExePath;
}

const std::string& rsuGetPath2Rsu()
{
    return s_pl.RsuPath();
}

void rsuSetPath2Rsu( const std::string& a_path )
{
    s_pl.SetRsuPath(a_path);
}

void rsuTakeOffReadOnly(const std::string& a_FileName)
{
    fs::path p (a_FileName);
    if (fs::exists(p))
    {
        fs::permissions(p, fs::perms::others_write 
            | fs::perms::group_write | fs::perms::owner_write,
            std::filesystem::perm_options::add
        );
    }
}

// Возвращает путь к директории с файлами трендов
const std::string& rsuGetTrendPath()
{
    return rsuGetMemoryPath();
}

static std::string s_a_path = "";
const std::string& rsuGetAcyPath()
{
    if (s_a_path == "")
        s_a_path = rsu_cp::makepath_cp(rsuGetProjectPath(), "DATA", "Acy").generic_string();
    return s_a_path;
}
