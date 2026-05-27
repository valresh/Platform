#pragma once
//#include "rsu_api.h"
//#include "rsu_fs.h"
#include <vector>
#include <map>
#include <memory>
#include "dylib.hpp"
//#include "LogFile.h"


namespace rsu_cp {

	typedef void (*t_rsu_lib_cleanup_fn)();

	class RsuLib {
		dylib* m_Lib = NULL;
		std::string m_Name = "";
		std::string m_LastErr = "";
		dylib::native_symbol_type m_api_fn = nullptr;
		t_rsu_lib_cleanup_fn m_cleanup_fn = nullptr;
	protected:
		dylib* lib() {
			return m_Lib;
		}
		virtual bool load(std::string a_Lib, const std::string a_Path, const std::string fn_api, const std::string fn_cleanup)
		{
			if (m_Lib != NULL)
			{
				m_LastErr = "Library already loaded!";
				return a_Lib == m_Name;
			}

//			std::filesystem::path libraryPath = std::filesystem::path(a_Path == "" ? rsu_cp::executable_directory() : a_Path) / MakeRelativePath(a_Lib);
                        std::filesystem::path libraryPath = std::filesystem::path(a_Path) / MakeRelativePath(a_Lib);
                        std::filesystem::path saveCurrentPath = std::filesystem::current_path();
                        std::filesystem::current_path(saveCurrentPath);
            try
			{
        // Change current folder to the dll's folder. E.g. it's required for OG\OGCtrl.dll
                                std::filesystem::path libraryFolder = saveCurrentPath; // libraryPath.parent_path();
        //std::filesystem::current_path(libraryFolder);
				m_Lib = new dylib(libraryFolder, libraryPath.filename().string(), true);
        if ( m_Lib == NULL )
          {
          }
				m_api_fn = m_Lib->get_symbol(fn_api);
				if (fn_cleanup != "")
				{
					m_cleanup_fn = (t_rsu_lib_cleanup_fn)m_Lib->get_symbol(fn_cleanup);
				}

//				CLogFile::LogEx(LOGLEVEL_HIGH, "Load library %s\n", libraryPath.string().c_str());
				m_Name = a_Lib;

				// Restore to original
				std::filesystem::current_path(saveCurrentPath);
				return true;
			}
			catch (const dylib::load_error& e) {
				m_LastErr = std::string("Load library error: ") + std::string(e.what());
			}
			catch (const dylib::symbol_error& e) {
				m_LastErr = std::string("Load symbol error: ") + std::string(e.what());
			}
			catch (...) {
				m_LastErr = "Unknown error! " + libraryPath.string();
			}

			//printf("Load library %s: %s\n", libraryPath.string().c_str(), m_LastErr.c_str());

			// Restore to original
			std::filesystem::current_path(saveCurrentPath);
			cleanup (false);
			return false;
		}		

	public:
		virtual void cleanup(bool a_cleenup_err = true) {
			if (m_cleanup_fn)
				m_cleanup_fn();
			delete m_Lib;
			m_Lib = nullptr;
			m_cleanup_fn = nullptr;
			m_api_fn = nullptr;
			m_Name = "";
			if (a_cleenup_err)
				m_LastErr = "";
		}
		bool state()
		{
			return m_Lib != NULL;
		}
		std::string name()
		{
			return m_Name;
		}
		std::string last_error()
		{
			return m_LastErr;
		}
		dylib::native_symbol_type api_fn() {
			return m_api_fn;
		}
		dylib::native_symbol_type get_symbol(const std::string f_name) {
			dylib::native_symbol_type ret = nullptr;
			if (state())
			{
				try {
					ret = m_Lib->get_symbol(f_name);
				}
				catch (const dylib::symbol_error& e) {
					m_LastErr = std::string("Load symbol error: ") + std::string(e.what());
				}
				catch (...) {
					m_LastErr = "Unknown error!";
				}
			}
			return ret;
		}
		RsuLib(const std::string a_Lib, const std::string a_Path, const std::string fn_api, const std::string fn_cleanup)
		{
                bool Res = load(a_Lib, a_Path, fn_api, fn_cleanup);
                Res;
		}
		~RsuLib()
		{
			if (m_Name != "rsu_clients_ldr")
				cleanup();
		}

	private:
		std::filesystem::path MakeRelativePath(const std::string& filePath)
		{
			auto index1 = filePath.find_last_of('(');
			auto index2 = filePath.find_last_of(')');
			if (index2 < index1 || index1 == std::string::npos || index2 == std::string::npos)
			{
				return filePath;
			}

 			auto folderName = filePath.substr(index1 + 1, index2 - index1 - 1);
			auto fileName = filePath.substr(0, index1);
			return std::filesystem::path(folderName) / fileName;
		}
	};

	class RsuLibs {
		std::vector<std::unique_ptr<RsuLib>> m_Libs;
	public:
		void cleanup()
		{
			m_Libs.clear();
		}
		dylib::native_symbol_type lib_fn(const std::string a_LibName)
		{
			for (auto& element : m_Libs)
			{
				if (element->name() == a_LibName)
					return element->api_fn();
			}
			return nullptr;
		}
		dylib::native_symbol_type get_symbol(const std::string a_LibName, const std::string f_name)
		{
			for (auto& element : m_Libs)
			{
				if (element->name() == a_LibName)
					return element->get_symbol(f_name);
			}
			return nullptr;
		}
		bool lib_loaded(const std::string a_LibName)
		{
			for (auto& element : m_Libs)
			{
				if (element->name() == a_LibName)
					return true;
			}
			return false;
		}

		bool load_lib(const std::string a_LibName, const std::string a_LibPath, const std::string fn_api, const std::string fn_cleanup)
		{
			if (lib_loaded(a_LibName))
				return true;

			std::unique_ptr<RsuLib> p(new RsuLib(a_LibName, a_LibPath, fn_api, fn_cleanup));
			if (p->state())
			{
				m_Libs.push_back(std::move(p));
				return true;
			}

			return false;
		}
	};

}
