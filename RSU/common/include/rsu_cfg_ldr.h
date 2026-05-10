#pragma once

#include <vector>

namespace rsu_cp {
	class RsuObjCfg {
		std::string m_ObjName = "";
	public:
		std::string ObjName()
		{
			return m_ObjName;
		}
		RsuObjCfg(std::string a_ObjName)
		{
			m_ObjName = a_ObjName;
		}
	};

	class RsuCfgLdr {
		std::string m_cfg_name;
		std::vector<std::shared_ptr<RsuObjCfg>> m_Configs;
	protected:
		virtual void clear()
		{
			m_Configs.clear();
		}
		virtual void load() = 0;
		std::vector<std::shared_ptr<RsuObjCfg>>& configs() { return m_Configs; };
	public:
		std::string cfg_name()
		{
			return m_cfg_name;
		}
		const std::shared_ptr<RsuObjCfg> cfg(std::string a_ObjName)
		{
			for (const std::shared_ptr<RsuObjCfg>& c : m_Configs)
				if (c->ObjName() == a_ObjName)
					return c;
			return nullptr;
		}
		void load_cfg(std::string a_cfg_name)
		{
			clear();
			m_cfg_name = a_cfg_name;
			load();
		}		
		~RsuCfgLdr()
		{
			clear();
		}
	};
}