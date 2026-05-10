#pragma once

#include <string>

namespace rsu_cp {

	inline std::string GetErrorString(int err)
	{
		if (err == 0) {
			return std::string();
		}
		const std::error_condition econd =
			std::system_category().default_error_condition(err);
		return econd.message();
	}

}