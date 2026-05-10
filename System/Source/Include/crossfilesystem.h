#pragma once
#include <filesystem>
#include <vector>

namespace cross
{
	namespace filesystem
	{
		bool exists(const std::filesystem::path&);
		bool exists(const std::filesystem::path&, std::filesystem::path&);
		uintmax_t file_size(const std::filesystem::path&, std::error_code&);
		uintmax_t file_size(const std::filesystem::path&);
		std::filesystem::file_time_type last_write_time(const std::filesystem::path&);

		// @brief Linux: {$project}\\file1 -> {$project}/file1
		// @brief Win: {$project}/file1 -> {$project}\\file1
		std::filesystem::path normalize(const std::string& path);

		std::vector<std::filesystem::path> files(const std::filesystem::path folderPath, const std::string filenamePattern, bool ignoreCase = true);
	}
}