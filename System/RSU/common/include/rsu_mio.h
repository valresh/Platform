#pragma once

#include <mio.hpp>
#include "rsu_fs.h"
#include <rsus/rsuPaths.h>

namespace rsu_cp {

	class RsuMio
	{
	private:
		std::string m_name = "";
		std::string m_path = "";
		std::error_code m_error;
		mio::mmap_sink hVarMapping;
		void* m_data = NULL;

	public:
		size_t page_size() { return mio::page_size(); }
		void* data() { return m_data; }
		std::string name() { return m_name; }
		std::string path() { return m_path; }
		std::error_code& last_error() { return m_error; }
		void close()
		{
			if (m_data)
			{
				hVarMapping.unmap();
				m_data = NULL;
				m_error.clear();
				m_name = "";
				m_path = "";
			}
		}
		bool open(const char* a_name, const size_t length, const size_t offset = 0, const char* a_path = nullptr)
		{
			close();
			m_name = a_name;
			m_path = build_path(m_name.c_str(), a_path);
			if (!length)
				hVarMapping.map(m_path.c_str(), m_error, true);
			else
				hVarMapping.map(m_path.c_str(), offset, length, m_error, true);
			int v = m_error.value();
			m_data = v ? NULL : hVarMapping.data();
			return state();
		}
		bool state() {
			return m_data != NULL;
		}
		bool flush()
		{
			if (m_data)
			{
				hVarMapping.sync(m_error);
				return !m_error;
			}
			
			return false;
		}
		~RsuMio() {
			close();
		}

	private:
		std::string build_path(const char* name, const char* path)
		{
			return (std::filesystem::path(path ? path : rsuGetMemoryPath()) / name).string();
		}
	};
}

