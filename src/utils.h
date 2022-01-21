#pragma once

#include <filesystem>
#include <string>

namespace utils {

	// read contents of file into string
	std::string fileReadString(const std::filesystem::path &filePath);

	// this prototype might be useful in the future
	//std::vector<uint8_t> fileReadBytes(const fs::path &filePath);

}
